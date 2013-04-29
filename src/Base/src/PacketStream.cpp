//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/PacketStream.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/IStartable.h"

#include "Poco/Exception.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"


using namespace std;
using namespace Poco;


namespace Sourcey {


PacketStream::PacketStream(const string& name) : 
	_name(name),
	_ready(false),
	_clientData(NULL)
{
	LogTrace("PacketStream", this) << "Creating" << endl;
	_ready.set();
}


PacketStream::~PacketStream()
{
	LogTrace("PacketStream", this) << "Destroying" << endl;
	close();
	LogTrace("PacketStream", this) << "Destroying: OK" << endl;
}


void PacketStream::start()
{	
	LogTrace("PacketStream", this) << "Starting" << endl;

	if (stateEquals(PacketStreamState::Running))  {
		LogTrace("PacketStream", this) << "Already Running" << endl;
		return;
	}

	{	
		FastMutex::ScopedLock lock(_mutex);

		// Setup the processor chain
		IPacketProcessor* lastProc = NULL;
		IPacketProcessor* thisProc = NULL;
		for (PacketAdapterList::iterator pit = _processors.begin(); pit != _processors.end(); ++pit) {
			thisProc = reinterpret_cast<IPacketProcessor*>((*pit).ptr);
			if (lastProc) {
				lastProc->attach(packetDelegate(thisProc, &IPacketProcessor::process));
			}
			lastProc = thisProc;
		}

		// The last processor will call stream dispatch
		if (lastProc)
			lastProc->attach(packetDelegate(this, &PacketStream::onDispatchPacket));

		// Attach and start synchronized runnables
		for (PacketAdapterList::iterator sit = _sources.begin(); sit != _sources.end(); ++sit) {
			PacketEmitter* source = (*sit).ptr;
			source->attach(packetDelegate(this, &PacketStream::onSourcePacket));
			if ((*sit).syncState) {
				IStartable* runnable = dynamic_cast<IStartable*>((*sit).ptr);
				if (runnable) {
					LogTrace("PacketStream", this) << "Starting Source: " << source << endl;
					runnable->start();
				}
			}
		}

	}
	
	setState(this, PacketStreamState::Running);
}


void PacketStream::stop()
{
	LogTrace("PacketStream", this) << "Stopping" << endl;
	
	if (stateEquals(PacketStreamState::Stopped) ||
		stateEquals(PacketStreamState::Closed)) {
		LogTrace("PacketStream", this) << "Already Stopped" << endl;
		return;
	}

	{
		// Can't wait here or we deadlock if calling stop() 
		// from inside stream callback.
		//_ready.wait();
		FastMutex::ScopedLock lock(_mutex);		

		// Stop synchronized runnables
		for (PacketAdapterList::iterator sit = _sources.begin(); sit != _sources.end(); ++sit) {
			PacketEmitter* source = (*sit).ptr;
			source->detach(packetDelegate(this, &PacketStream::onSourcePacket));
			if ((*sit).syncState) {
				IStartable* runnable = dynamic_cast<IStartable*>((*sit).ptr);
				if (runnable) {
					LogTrace("PacketStream", this) << "Stopping Source: " << source << endl;
					runnable->stop();
				}
			}
		}

		// Detach the processor chain
		IPacketProcessor* lastProc = NULL;
		IPacketProcessor* thisProc = NULL;
		for (PacketAdapterList::iterator pit = _processors.begin(); pit != _processors.end(); ++pit) {
			thisProc = reinterpret_cast<IPacketProcessor*>((*pit).ptr);
			if (lastProc)
				lastProc->detach(packetDelegate(thisProc, &IPacketProcessor::process));
			lastProc = thisProc;
		}
		if (lastProc)
			lastProc->detach(packetDelegate(this, &PacketStream::onDispatchPacket));
	}
	
	setState(this, PacketStreamState::Stopped);
}


void PacketStream::reset()
{
	LogTrace("PacketStream", this) << "Resetting Stream" << endl;
	
	setState(this, PacketStreamState::Resetting);
	setState(this, PacketStreamState::Running);
}


void PacketStream::close()
{
	LogTrace("PacketStream", this) << "Closing" << endl;

	if (stateEquals(PacketStreamState::Closing) || 
		stateEquals(PacketStreamState::Closed)) {
		LogTrace("PacketStream", this) << "Already Closing" << endl;
		return;
	}
	
	if (!stateEquals(PacketStreamState::Stopped))
		stop();

	setState(this, PacketStreamState::Closing);	
	detachAll();	
	setState(this, PacketStreamState::Closed); // may result in destruction

	LogTrace("PacketStream", this) << "Closing: OK" << endl;
}


void PacketStream::onStateChange(PacketStreamState& state, const PacketStreamState& oldState)
{
	LogTrace("PacketStream", this) << "State changed from " << oldState << " to " << state << endl;
	if (state.id() != PacketStreamState::Closed &&
		state.id() != PacketStreamState::None) {
		PacketAdapterList adapters = this->adapters();
		for (PacketAdapterList::iterator it = adapters.begin(); it != adapters.end(); ++it)
			(*it).ptr->onStreamStateChange(state);
	}
	StatefulSignal<PacketStreamState>::onStateChange(state, oldState); // send events
}


void PacketStream::attach(PacketEmitter* source, bool freePointer, bool syncState) 
{
	LogTrace("PacketStream", this) << "Attaching Source " << source << endl;
	_ready.wait();
	FastMutex::ScopedLock lock(_mutex);
	_sources.push_back(PacketAdapterReference(source, 0, freePointer, syncState));
	sort(_sources.begin(), _sources.end(), PacketAdapterReference::CompareOrder);
	//source->attach(packetDelegate(this, &PacketStream::onNextPacket));
	//_iter = _sources.begin();
	LogTrace("PacketStream", this) << "Attaching Source " << source << ": OK" << endl;
}


void PacketStream::attach(IPacketProcessor* proc, int order, bool freePointer) 
{
	LogTrace("PacketStream", this) << "Attaching Processor " << proc << endl;
	_ready.wait();
	FastMutex::ScopedLock lock(_mutex);
	_processors.push_back(PacketAdapterReference(proc, order == 0 ? _processors.size() : order, freePointer));
	sort(_processors.begin(), _processors.end(), PacketAdapterReference::CompareOrder);
	//proc->attach(packetDelegate(this, &PacketStream::onNextPacket));
	//_iter = _processors.begin();
	LogTrace("PacketStream", this) << "Attaching Processor " << proc << ": OK" << endl;
}


void PacketStream::detach(PacketEmitter* source) 
{
	LogTrace("PacketStream", this) << "Detaching Source: " << source << endl;
	_ready.wait();
	FastMutex::ScopedLock lock(_mutex);
	for (PacketAdapterList::iterator it = _sources.begin(); it != _sources.end(); ++it) {
		if ((*it).ptr == source) {
			LogTrace("PacketStream", this) << "Detaching Source: " << source << ": OK" << endl;
			(*it).ptr->detach(packetDelegate(this, &PacketStream::onSourcePacket));
			if ((*it).freePointer)
				delete (*it).ptr;
			_sources.erase(it);
			//_iter = _sources.begin();
			break;
		}
	}
}


void PacketStream::detach(IPacketProcessor* proc) 
{
	LogTrace("PacketStream", this) << "Detaching Processor: " << proc << endl;
	_ready.wait();
	FastMutex::ScopedLock lock(_mutex);
	for (PacketAdapterList::iterator it = _processors.begin(); it != _processors.end(); ++it) {
		if ((*it).ptr == proc) {
			LogTrace("PacketStream", this) << "Detaching Processor: " << proc << ": OK" << endl;
			if ((*it).freePointer)
				delete (*it).ptr;
			_processors.erase(it);
			//_iter = _processors.begin();
			break;
		}
	}
}
	

void PacketStream::attach(const PacketDelegateBase& delegate)
{
	LogTrace("PacketStream", this) << "Attaching Delegate: " << delegate.object() << endl;
	PacketEmitter::attach(delegate);
}


bool PacketStream::detach(const PacketDelegateBase& delegate)
{
	LogTrace("PacketStream", this) << "Detaching Delegate: " << delegate.object() << endl;
	return PacketEmitter::detach(delegate);
}


void PacketStream::detachAll()
{
	LogTrace("PacketStream", this) << "Detaching All" << endl;
		
	_ready.wait();

	LogTrace("PacketStream", this) << "Detaching All: Sources" << endl;
		
	FastMutex::ScopedLock lock(_mutex);
	PacketAdapterList::iterator sit = _sources.begin();
	while (sit != _sources.end()) {
		LogTrace("PacketStream", this) << "Detaching Source: " << (*sit).ptr << endl;
		(*sit).ptr->detach(packetDelegate(this, &PacketStream::onSourcePacket));
		if ((*sit).freePointer)
			delete (*sit).ptr;
		LogTrace("PacketStream", this) << "Detaching Source: " << (*sit).ptr << ": OK" << endl;
		//	toDelete.push_back(*sit);
		sit = _sources.erase(sit);
	}	
	
	LogTrace("PacketStream", this) << "Detaching All: Processors" << endl;

	PacketAdapterList::iterator pit = _processors.begin();
	while (pit != _processors.end()) {
		LogTrace("PacketStream", this) << "Detaching Processor: " << (*pit).ptr << endl;
		//(*pit).ptr->detach(packetDelegate(this, &PacketStream::onNextPacket));
		if ((*pit).freePointer)
			delete (*pit).ptr;
		LogTrace("PacketStream", this) << "Detaching Processor: " << (*pit).ptr << ": OK" << endl;
		//	toDelete.push_back(*pit);
		pit = _processors.erase(pit);
	}

	LogTrace("PacketStream", this) << "Detaching All: OK" << endl;
}
	

void PacketStream::onSourcePacket(void*, IPacket& packet) 
{	
	//LogTrace("PacketStream", this) << "On Source Packet: " 
	//	<< &packet << ": " << packet.className() << endl;

	if (!isRunning() || !enabled()) {
		LogDebug("PacketStream", this) << "Dropping Source Packet: " 
			<< &packet << ": " << enabled() << ": " << state() << endl;	
		_ready.set(); 
		return;
	}

	try {		
		// Reset the ready event.
		// We don't want any new adapters or stream
		// destruction while we are processing.
		_ready.reset();

		IPacketProcessor* firstProc = NULL;
		{
			FastMutex::ScopedLock lock(_mutex);
			PacketAdapterList::iterator it = _processors.begin();
			if (it != _processors.end()) {

				firstProc = reinterpret_cast<IPacketProcessor*>((*it).ptr);

				// If the first processor rejects the packet then the 
				// packet will be dropped.
				if (!firstProc->accepts(packet)) {
					//LogWarn(this, "PacketStream") << "Source Packet Rejected: " 
					//	<< firstProc << ": " << packet.className() << endl;
					firstProc = NULL;
					_ready.set(); 
					return;
				}
			}
		}

		if (firstProc) {
			//LogTrace("PacketStream", this) << "Start Process Chain: " 
			//	<< &packet << ": " << firstProc << ": " << packet.className() << endl;

			firstProc->process(packet);
		}

		// If there are no processors we just proxy the packet.
		else {
			//LogTrace("PacketStream", this) << "Proxying: " 
			//	<< &packet << ": " << packet.className() << endl;
			onDispatchPacket(this, packet);
		}
	}
	catch (Exception& exc) {
		LogError("PacketStream", this) << "Processor Error: " << exc.displayText() << endl;
		// Set the ready event, allowing the stream to be
		// closed inside Failed event callback.
		_ready.set();
		setState(this, PacketStreamState::Error, exc.displayText());
		return;
	}	
	
	//LogTrace("PacketStream", this) << "End Process Chain: " 
	//	<< &packet << ": " << packet.className() << endl;

	// Set the ready event once processing is complete.
	_ready.set();
	return;
}


void PacketStream::onDispatchPacket(void*, IPacket& packet)
{
	// No more adapters to process, ensure the stream 
	// is still running before we dispatch the goods.
	if (!isRunning() || !enabled()) {
		LogDebug() << "[PacketStream:" << this << "] Dropping Late Packet: " 
			<< &packet << ": " << enabled() << ": " << state() << endl;	
		return;
	}

	//LogTrace("PacketStream", this) << "Dispatching: " << &packet << ": " << packet.className() << endl;
	emit(this, packet);
	//LogTrace("PacketStream", this) << "Dispatching: OK: " << &packet << ": " << packet.className() << endl;	
}


bool PacketStream::waitForReady()
{	
	_ready.wait();
	return true;
}


string PacketStream::name() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _name;
}


int PacketStream::numSources() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _sources.size();
}


int PacketStream::numProcessors() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _processors.size();
}


int PacketStream::numAdapters() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _sources.size() + _processors.size();
}
	
	
bool PacketStream::isRunning() const
{
	return stateEquals(PacketStreamState::Running);
}


PacketAdapterList PacketStream::adapters() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _processors;
}
	

void PacketStream::setClientData(void* data)
{
	FastMutex::ScopedLock lock(_mutex);
	_clientData = data;
}


void* PacketStream::clientData() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _clientData;
}


} // namespace Sourcey