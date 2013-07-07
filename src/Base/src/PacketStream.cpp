//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "Sourcey/PacketStream.h"
#include "Sourcey/IStartable.h"

//#include "Sourcey/PacketStream.h"
//#include "Poco/Exception.h"
//#include "Poco/NumberParser.h"
//#include "Poco/NumberFormatter.h"
//#include "Poco/String.h"


using namespace std;
using namespace Poco;


namespace scy {


PacketStream::PacketStream(const string& name) : 
	_name(name),
	_ready(false),
	_clientData(NULL)
{
	traceL("PacketStream", this) << "Creating" << endl;
	_ready.set();
}


PacketStream::~PacketStream()
{
	traceL("PacketStream", this) << "Destroying" << endl;
	close();
	traceL("PacketStream", this) << "Destroying: OK" << endl;
}


void PacketStream::start()
{	
	traceL("PacketStream", this) << "Starting" << endl;

	if (stateEquals(PacketStreamState::Running))  {
		traceL("PacketStream", this) << "Already Running" << endl;
		return;
	}

	assert(numAdapters() > 0);
	{	
		FastMutex::ScopedLock lock(_mutex);

		// Setup the processor chain
		IPacketProcessor* lastProc = NULL;
		IPacketProcessor* thisProc = NULL;
		for (PacketAdapterList::iterator pit = _processors.begin(); pit != _processors.end(); ++pit) {
			thisProc = reinterpret_cast<IPacketProcessor*>((*pit).ptr);
			if (lastProc) {
				traceL("PacketStream", this) << "Connecting Processor: " << thisProc << endl;
				lastProc->attach(packetDelegate(thisProc, &IPacketProcessor::process));
			}
			lastProc = thisProc;
		}

		// The last processor will call stream emit the final
		// packet to the outside application.
		if (lastProc)
			lastProc->attach(packetDelegate(this, &PacketStream::onDispatchPacket));

		// Attach and start synchronized runnables
		for (PacketAdapterList::iterator sit = _sources.begin(); sit != _sources.end(); ++sit) {
			PacketEmitter* source = (*sit).ptr;
			traceL("PacketStream", this) << "Connecting Source: " << source << endl;
			source->attach(packetDelegate(this, &PacketStream::onSourcePacket));
			if ((*sit).syncState) {
				IStartable* runnable = dynamic_cast<IStartable*>((*sit).ptr);
				if (runnable) {
					traceL("PacketStream", this) << "Starting Source: " << source << endl;
					runnable->start();
				}
			}
		}
	}
	
	setState(this, PacketStreamState::Running);
}


void PacketStream::stop()
{
	traceL("PacketStream", this) << "Stopping" << endl;
	
	if (stateEquals(PacketStreamState::Stopped) ||
		stateEquals(PacketStreamState::Closed)) {
		traceL("PacketStream", this) << "Already Stopped" << endl;
		return;
	}

	{
		// Can't wait here or we deadlock if calling stop() 
		// from inside stream callback.
		waitForReady();
		FastMutex::ScopedLock lock(_mutex);		

		// Detach sources, and stop synchronized startables
		for (PacketAdapterList::iterator sit = _sources.begin(); sit != _sources.end(); ++sit) {
			PacketEmitter* source = (*sit).ptr;
			source->detach(packetDelegate(this, &PacketStream::onSourcePacket));
			if ((*sit).syncState) {
				IStartable* runnable = dynamic_cast<IStartable*>((*sit).ptr);
				if (runnable) {
					traceL("PacketStream", this) << "Stopping Source: " << source << endl;
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
	traceL("PacketStream", this) << "Resetting Stream" << endl;
	
	// TODO: Refactor
	setState(this, PacketStreamState::Resetting);
	setState(this, PacketStreamState::Running);
}


void PacketStream::close()
{
	traceL("PacketStream", this) << "Closing" << endl;

	if (stateEquals(PacketStreamState::Closing) || 
		stateEquals(PacketStreamState::Closed)) {
		traceL("PacketStream", this) << "Already Closing" << endl;
		return;
	}
	
	if (!stateEquals(PacketStreamState::Stopped))
		stop();

	setState(this, PacketStreamState::Closing);	
	cleanup();	
	setState(this, PacketStreamState::Closed); // may result in destruction

	traceL("PacketStream", this) << "Closing: OK" << endl;
}


void PacketStream::attach(PacketEmitter* source, bool freePointer, bool syncState) 
{
	traceL("PacketStream", this) << "Attaching Source " << source << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot attach source to running stream.");
	
	waitForReady();

	FastMutex::ScopedLock lock(_mutex);
	_sources.push_back(PacketAdapterReference(source, 0, freePointer, syncState));
	sort(_sources.begin(), _sources.end(), PacketAdapterReference::CompareOrder);
	traceL("PacketStream", this) << "Attaching Source " << source << ": OK" << endl;
}


void PacketStream::attach(IPacketProcessor* proc, int order, bool freePointer) 
{
	traceL("PacketStream", this) << "Attaching Processor " << proc << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot attach processor to running stream.");
	
	waitForReady();

	FastMutex::ScopedLock lock(_mutex);
	_processors.push_back(PacketAdapterReference(proc, order == 0 ? _processors.size() : order, freePointer));
	sort(_processors.begin(), _processors.end(), PacketAdapterReference::CompareOrder);
	traceL("PacketStream", this) << "Attaching Processor " << proc << ": OK" << endl;
}


bool PacketStream::detach(PacketEmitter* source) 
{
	traceL("PacketStream", this) << "Detaching Source: " << source << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot detach source from running stream.");

	waitForReady();

	FastMutex::ScopedLock lock(_mutex);
	for (PacketAdapterList::iterator it = _sources.begin(); it != _sources.end(); ++it) {
		if ((*it).ptr == source) {
			traceL("PacketStream", this) << "Detaching Source: " << source << ": OK" << endl;
			(*it).ptr->detach(packetDelegate(this, &PacketStream::onSourcePacket));
			//if ((*it).freePointer)
			//	delete (*it).ptr;
			assert(0);
			_sources.erase(it);
			return true;
		}
	}
	return false;
}


bool PacketStream::detach(IPacketProcessor* proc) 
{
	traceL("PacketStream", this) << "Detaching Processor: " << proc << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot detach processor from running stream.");
	
	waitForReady();

	FastMutex::ScopedLock lock(_mutex);
	for (PacketAdapterList::iterator it = _processors.begin(); it != _processors.end(); ++it) {
		if ((*it).ptr == proc) {
			traceL("PacketStream", this) << "Detaching Processor: " << proc << ": OK" << endl;
			//if ((*it).freePointer)
			//	delete (*it).ptr;
			assert(0);
			_processors.erase(it);
			return true;
		}
	}
	return false;
}
	

void PacketStream::attach(const PacketDelegateBase& delegate)
{
	traceL("PacketStream", this) << "Attaching Delegate: " << delegate.object() << endl;
	PacketEmitter::attach(delegate);
}


bool PacketStream::detach(const PacketDelegateBase& delegate)
{
	traceL("PacketStream", this) << "Detaching Delegate: " << delegate.object() << endl;
	return PacketEmitter::detach(delegate);
}


void PacketStream::cleanup()
{
	traceL("PacketStream", this) << "Detaching All" << endl;		
	if (stateEquals(PacketStreamState::Running)) {
		assert(0);
		throw Exception("Cannot detach apapters from running stream.");
	}
	
	waitForReady();

	traceL("PacketStream", this) << "Detaching All: Sources" << endl;		
	FastMutex::ScopedLock lock(_mutex);
	PacketAdapterList::iterator sit = _sources.begin();
	while (sit != _sources.end()) {
		traceL("PacketStream", this) << "Detaching Source: " << (*sit).ptr << endl;
		(*sit).ptr->detach(packetDelegate(this, &PacketStream::onSourcePacket));

		// Delete source pointer asynchronously so the stream may be 
		// closed and/or destroyed from within the source callback scope.
		//if ((*sit).freePointer)
		//	deleteLater<PacketEmitter>((*sit).ptr);
			//delete (*sit).ptr;

		traceL("PacketStream", this) << "Detaching Source: " << (*sit).ptr << ": OK" << endl;
		sit = _sources.erase(sit);
	}	
	
	traceL("PacketStream", this) << "Detaching All: Processors" << endl;
	PacketAdapterList::iterator pit = _processors.begin();
	while (pit != _processors.end()) {
		traceL("PacketStream", this) << "Detaching Processor: " << (*pit).ptr << endl;
		//if ((*pit).freePointer)
		//	deleteLater<IPacketProcessor>((*pit).ptr);
			//delete (*pit).ptr;
		traceL("PacketStream", this) << "Detaching Processor: " << (*pit).ptr << ": OK" << endl;
		pit = _processors.erase(pit);
	}

	traceL("PacketStream", this) << "Detaching All: OK" << endl;
}
	

void PacketStream::onSourcePacket(void*, IPacket& packet) 
{	
	traceL("PacketStream", this) << "On Source Packet: " 
		<< &packet << ": " << packet.className() << endl;
	
	try {	
		do {
			if (!isRunning() || !enabled()) {
				debugL("PacketStream", this) << "Dropping Source Packet: " 
					<< &packet << ": " << enabled() << ": " << state() << endl;	
				//_ready.set(); 
				break;
			}	

			// Reset the ready event.
			// We don't want any new adapters or stream
			// destruction while we are processing.
			//_ready.reset();

			// Obtain the first processor in the queue
			IPacketProcessor* firstProc = NULL;
			{
				FastMutex::ScopedLock lock(_mutex);
				PacketAdapterList::iterator it = _processors.begin();
				if (it != _processors.end()) {

					firstProc = reinterpret_cast<IPacketProcessor*>((*it).ptr);

					// If the first processor in the chain rejects  
					// the packet then the packet will be dropped.
					if (!firstProc->accepts(packet)) {
						warnL("PacketStream", this) << "Source Packet Rejected: " 
							<< firstProc << ": " << packet.className() << endl;
						firstProc = NULL;
						//_ready.set(); 
						break;
					}
				}
			}
			
			// Send the packet to the first processor, it will pass 
			// the packet on down the line...
			if (firstProc) {
				traceL("PacketStream", this) << "Start Process Chain: " 
					<< &packet << ": " << firstProc << ": " << packet.className() << endl;

				firstProc->process(packet);
			}

			// If there are no processors then we proxy the packet.
			else {
				traceL("PacketStream", this) << "Proxying: " 
					<< &packet << ": " << packet.className() << endl;
				onDispatchPacket(this, packet);
			}
			
		} while(0);
		
		// Set the ready event once processing is complete.
		//_ready.set();
		//_procMutex.unlock();
	}
				
	// Catch any exceptions thrown within the processor scope 
	// and set the stream to error state.
	catch (Exception& exc) {
		errorL("PacketStream", this) << "Processor Error: " << exc.displayText() << endl;
		// Set the ready event, allowing the stream to be
		// closed inside Error state callback.
		//_ready.set();
		//_procMutex.unlock();
		setState(this, PacketStreamState::Error, exc.displayText());
	}	
	
	traceL("PacketStream", this) << "End Process Chain: " 
		<< &packet << ": " << packet.className() << endl;
}


void PacketStream::onDispatchPacket(void*, IPacket& packet)
{
	// No more adapters to process, ensure the stream 
	// is still running before we dispatch the goods.
	if (!isRunning() || !enabled()) {
		debugL() << "[PacketStream:" << this << "] Dropping Late Packet: " 
			<< &packet << ": " << enabled() << ": " << state() << endl;	
		return;
	}

	traceL("PacketStream", this) << "Dispatching: " << &packet << ": " << packet.className() << endl;
	emit(this, packet);
	traceL("PacketStream", this) << "Dispatching: OK: " << &packet << ": " << packet.className() << endl;	
}


void PacketStream::onStateChange(PacketStreamState& state, const PacketStreamState& oldState)
{
	traceL("PacketStream", this) << "State changed from " << oldState << " to " << state << endl;
	if (state.id() != PacketStreamState::Closed &&
		state.id() != PacketStreamState::None) {
		PacketAdapterList adapters = this->adapters();
		for (PacketAdapterList::iterator it = adapters.begin(); it != adapters.end(); ++it)
			(*it).ptr->onStreamStateChange(state);
	}
	StatefulSignal<PacketStreamState>::onStateChange(state, oldState); // send events
}


bool PacketStream::waitForReady()
{	
	traceL("PacketStream", this) << "Waiting for ready" << endl;
	bool res = true; //_ready.tryWait(2000);
	traceL("PacketStream", this) << "Waiting for ready: " << res << endl;
	assert(res && "packet stream locked");
	return res;
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


} // namespace scy