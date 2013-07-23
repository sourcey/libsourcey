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
#include "Sourcey/Memory.h"
#include "Sourcey/Interfaces.h"

//#include "Sourcey/PacketStream.h"
//#include "Poco/Exception.h"
//#include "Poco/NumberParser.h"
//#include "Poco/NumberFormatter.h"
//#include "Poco/String.h"


using namespace std;
//using namespace Poco;


namespace scy {


PacketStream::PacketStream(const string& name) : 
	_name(name),
	//_ready(false),
	_clientData(nullptr)
{
	traceL("PacketStream", this) << "Creating" << endl;
	//_ready.set();
}


PacketStream::~PacketStream()
{
	traceL("PacketStream", this) << "Destroying" << endl;
	//close();
	assert(closed());
	traceL("PacketStream", this) << "Destroying: OK" << endl;
}


/*
void PacketStream::destroy()
{
	traceL("PacketStream", this) << "Destroy later" << endl;
	// 
	close();
	deleteLater<PacketStream>(this);
}
*/


void PacketStream::start()
{	
	traceL("PacketStream", this) << "Starting" << endl;

	if (stateEquals(PacketStreamState::Running))  {
		traceL("PacketStream", this) << "Already Running" << endl;
		return;
	}

	//assert(numAdapters() > 0);
	{	
		enable(true);
	
		setState(this, PacketStreamState::Running);

		Mutex::ScopedLock lock(_mutex);

		// Setup the processor chain
		PacketProcessor* lastProc = nullptr;
		PacketProcessor* thisProc = nullptr;
		for (PacketAdapterList::iterator pit = _processors.begin(); pit != _processors.end(); ++pit) {
			thisProc = reinterpret_cast<PacketProcessor*>((*pit).ptr);
			if (lastProc) {
				traceL("PacketStream", this) << "Connecting Processor: " << thisProc << endl;
				lastProc->attach(packetDelegate(thisProc, &PacketProcessor::process));
			}
			lastProc = thisProc;
		}

		// The last processor will emit the result
		// packet to the outside application.
		if (lastProc)
			lastProc->attach(packetDelegate(this, &PacketStream::emit));

		// Attach and start synchronized runnables
		for (PacketAdapterList::iterator sit = _sources.begin(); sit != _sources.end(); ++sit) {
			PacketSignal* source = (*sit).ptr;
			traceL("PacketStream", this) << "Connecting source: " << source << endl;
			source->attach(packetDelegate(this, &PacketStream::write));
			if ((*sit).syncState) {
				/*
				abstract::Startable* runnable = dynamic_cast<abstract::Startable*>((*sit).ptr);
				if (runnable) {
					traceL("PacketStream", this) << "Starting Source: " << source << endl;
					runnable->start();
				}
				*/
				
				auto startable = dynamic_cast<abstract::Startable*>((*sit).ptr);
				if (startable) {
					traceL("PacketStream", this) << "Starting startable: " << source << endl;
					startable->start();
				}
				auto runnable = dynamic_cast<abstract::Runnable*>((*sit).ptr);
				if (runnable) {
					traceL("PacketStream", this) << "Starting runnable: " << source << endl;
					runnable->run();
				}
				assert(startable || runnable && "unknown synchronized adapter");
			}
		}
	}
}


void PacketStream::stop()
{
	traceL("PacketStream", this) << "Stopping" << endl;
	
	if (stateEquals(PacketStreamState::Stopped) ||
		stateEquals(PacketStreamState::Closed)) {
		traceL("PacketStream", this) << "Already stopped" << endl;
		return;
	}

	{
		//waitForReady();
		enable(false);

		Mutex::ScopedLock lock(_mutex);		

		// Detach the processor chain first
		PacketProcessor* lastProc = nullptr;
		PacketProcessor* thisProc = nullptr;
		for (PacketAdapterList::iterator pit = _processors.begin(); pit != _processors.end(); ++pit) {
			thisProc = reinterpret_cast<PacketProcessor*>((*pit).ptr);
			if (lastProc)
				lastProc->detach(packetDelegate(thisProc, &PacketProcessor::process));
			lastProc = thisProc;
		}
		if (lastProc)
			lastProc->detach(packetDelegate(this, &PacketStream::emit));

		// Detach sources, and stop synchronized sources
		for (PacketAdapterList::iterator sit = _sources.begin(); sit != _sources.end(); ++sit) {
			PacketSignal* source = (*sit).ptr;
			source->detach(packetDelegate(this, &PacketStream::write));
			if ((*sit).syncState) {
				auto startable = dynamic_cast<abstract::Startable*>((*sit).ptr);
				if (startable) {
					traceL("PacketStream", this) << "Stopping startable: " << source << endl;
					startable->stop();
				}
				auto runnable = dynamic_cast<abstract::Runnable*>((*sit).ptr);
				if (runnable) {
					traceL("PacketStream", this) << "Stopping runnable: " << source << endl;
					runnable->cancel();
				}
				assert(startable || runnable && "unknown synchronized adapter");
			}
		}
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


void PacketStream::attachSource(PacketSignal* source, bool freePointer, bool syncState) 
{
	traceL("PacketStream", this) << "Attaching source: " << source << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot attach source to running stream.");
	
	//waitForReady();

	Mutex::ScopedLock lock(_mutex);
	_sources.push_back(PacketAdapterReference(source, 0, freePointer, syncState));
	sort(_sources.begin(), _sources.end(), PacketAdapterReference::CompareOrder);
	traceL("PacketStream", this) << "Attaching source: " << source << ": OK" << endl;
}


bool PacketStream::detachSource(PacketSignal* source) 
{
	traceL("PacketStream", this) << "Detaching source: " << source << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot detach source from a running stream.");

	//waitForReady();

	Mutex::ScopedLock lock(_mutex);
	for (PacketAdapterList::iterator it = _sources.begin(); it != _sources.end(); ++it) {
		if ((*it).ptr == source) {
			traceL("PacketStream", this) << "Detaching source: " << source << ": OK" << endl;
			(*it).ptr->detach(packetDelegate(this, &PacketStream::write));
			//if ((*it).freePointer)
			//	delete (*it).ptr;
			if ((*it).freePointer)
				deleteLater<PacketSignal>((*it).ptr);
			_sources.erase(it);
			return true;
		}
	}
	return false;
}


void PacketStream::attach(PacketProcessor* proc, int order, bool freePointer) 
{
	traceL("PacketStream", this) << "Attaching processor " << proc << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot attach processor to running stream.");
	
	//if (!dynamic_cast<PacketParser*>(proc) &&
	//	!dynamic_cast<PacketProcessor*>(proc))
	//	assert(0 && "unknown stream adapter");	

	//waitForReady();

	Mutex::ScopedLock lock(_mutex);
	_processors.push_back(PacketAdapterReference(proc, order == 0 ? _processors.size() : order, freePointer));
	sort(_processors.begin(), _processors.end(), PacketAdapterReference::CompareOrder);
	traceL("PacketStream", this) << "Attaching processor " << proc << ": OK" << endl;
}


bool PacketStream::detach(PacketProcessor* proc) 
{
	traceL("PacketStream", this) << "Detaching processor: " << proc << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot detach processor from a running stream.");
	
	//waitForReady();

	Mutex::ScopedLock lock(_mutex);
	for (PacketAdapterList::iterator it = _processors.begin(); it != _processors.end(); ++it) {
		if ((*it).ptr == proc) {
			traceL("PacketStream", this) << "Detaching processor: " << proc << ": OK" << endl;
			//if ((*it).freePointer)
			//	delete (*it).ptr;
			if ((*it).freePointer)
				deleteLater<PacketSignal>((*it).ptr);
			//assert(0);
			_processors.erase(it);
			return true;
		}
	}
	return false;
}
	

void PacketStream::attach(const PacketDelegateBase& delegate)
{
	traceL("PacketStream", this) << "Attaching Delegate: " << delegate.object() << endl;
	PacketSignal::attach(delegate);
}


bool PacketStream::detach(const PacketDelegateBase& delegate)
{
	traceL("PacketStream", this) << "Detaching Delegate: " << delegate.object() << endl;
	return PacketSignal::detach(delegate);
}


void PacketStream::cleanup()
{
	traceL("PacketStream", this) << "Cleanup" << endl;		
	if (stateEquals(PacketStreamState::Running)) {
		assert(0);
		throw Exception("Cannot cleanup a running stream.");
	}
	
	//waitForReady();

	traceL("PacketStream", this) << "Detaching all sources" << endl;		
	Mutex::ScopedLock lock(_mutex);
	PacketAdapterList::iterator sit = _sources.begin();
	while (sit != _sources.end()) {
		traceL("PacketStream", this) << "Detaching source: " << (*sit).ptr << endl;
		(*sit).ptr->detach(packetDelegate(this, &PacketStream::write));

		// Delete source pointer asynchronously so the stream may be 
		// closed and/or destroyed from within the source callback scope.
		if ((*sit).freePointer)
			deleteLater<PacketSignal>((*sit).ptr);
		//if ((*sit).freePointer)
		//	deleteLater<PacketSignal>((*sit).ptr);
			//delete (*sit).ptr;

		traceL("PacketStream", this) << "Detaching Source: " << (*sit).ptr << ": OK" << endl;
		sit = _sources.erase(sit);
	}	
	
	traceL("PacketStream", this) << "Detaching All: Processors" << endl;
	PacketAdapterList::iterator pit = _processors.begin();
	while (pit != _processors.end()) {
		traceL("PacketStream", this) << "Detaching Processor: " << (*pit).ptr << endl;
		if ((*pit).freePointer)
			deleteLater<PacketSignal>((*pit).ptr);
		//if ((*pit).freePointer)
		//	deleteLater<PacketProcessor>((*pit).ptr);
			//delete (*pit).ptr;
		traceL("PacketStream", this) << "Detaching Processor: " << (*pit).ptr << ": OK" << endl;
		pit = _processors.erase(pit);
	}

	traceL("PacketStream", this) << "Detaching All: OK" << endl;
}


void PacketStream::write(char* data, size_t len, void* opaque)
{
	write(RawPacket(data, len));
}


void PacketStream::write(const char* data, size_t len, void* opaque)
{
	write(RawPacket(data, len));
}


void PacketStream::write(const std::string& str, void* opaque)
{
	write(RawPacket(str.data(), str.length()));
}
	

void PacketStream::write(IPacket& packet)  //void*, 
{	
	traceL("PacketStream", this) << "Process packet: " 
		<< &packet << ": " << packet.className() << endl;
	
	try {	
		do {
			if (!running() || !enabled()) {
				debugL("PacketStream", this) << "Dropping packet: " 
					<< &packet << ": " << enabled() << ": " << state() << endl;	
				//_ready.set(); 
				break;
			}	

			// Reset the ready event.
			// We don't want any new adapters or stream
			// destruction while we are processing.
			//_ready.reset();

			// Obtain the first processor in the queue
			PacketProcessor* firstProc = nullptr;
			{
				Mutex::ScopedLock lock(_mutex);
				PacketAdapterList::iterator it = _processors.begin();
				if (it != _processors.end()) {

					firstProc = reinterpret_cast<PacketProcessor*>((*it).ptr);

					// If the first processor in the chain rejects  
					// the packet then the packet will be dropped.
					if (!firstProc->accepts(packet)) {
						warnL("PacketStream", this) << "Source packet rejected: " 
							<< firstProc << ": " << packet.className() << endl;
						firstProc = nullptr;
						//_ready.set(); 
						break;
					}
				}
			}
			
			// Send the packet to the first processor, it will pass 
			// the packet on down the line...
			if (firstProc) {
				traceL("PacketStream", this) << "Start process chain: " 
					<< &packet << ": " << firstProc << ": " << packet.className() << endl;

				firstProc->process(packet);
			}

			// If there are no processors then we proxy the packet.
			else {
				traceL("PacketStream", this) << "Proxying: " 
					<< &packet << ": " << packet.className() << endl;
				emit(packet);
			}
			
		} while(0);
		
		// Set the ready event once processing is complete.
		//_ready.set();
		//_procMutex.unlock();
	}
				
	// Catch any exceptions thrown within the processor scope 
	// and set the stream to error state.
	//catch (Exception& exc) {
	catch (std::exception& exc) {
		errorL("PacketStream", this) << "Processor error: " << exc.what() << endl;
		// Set the ready event to allow the stream to be
		// closed/destroyed inside Error state callback.
		//_ready.set();
		//_procMutex.unlock();
		setState(this, PacketStreamState::Error, exc.what());
	}	
	
	//traceL("PacketStream", this) << "End Process Chain: " 
	//	<< &packet << ": " << packet.className() << endl;
}


void PacketStream::emit(IPacket& packet)
{
	// No more adapters to process, ensure the stream 
	// is still running before we dispatch the goods.
	if (!running() || !enabled()) {
		debugL("PacketStream", this) << "Dropping late Packet: " 
			<< &packet << ": " << enabled() << ": " << state() << endl;	
		return;
	}

	traceL("PacketStream", this) << "Emitting: " << &packet << ": " << packet.className() << endl;
	PacketSignal::emit(this, packet);
	traceL("PacketStream", this) << "Emitting: OK: " << &packet << ": " << packet.className() << endl;	
}


void PacketStream::onStateChange(PacketStreamState& state, const PacketStreamState& oldState)
{
	traceL("PacketStream", this) << "State changed from " << oldState << " to " << state << endl;
	if (state.id() != PacketStreamState::Closed &&
		state.id() != PacketStreamState::None) {
		PacketAdapterList adapters = this->adapters();

		// TODO: Better way of getting events to managed adapters?
		for (PacketAdapterList::iterator it = adapters.begin(); it != adapters.end(); ++it) {			
			PacketStreamAdapter* adapter = dynamic_cast<PacketStreamAdapter*>((*it).ptr);
			if (adapter)
				adapter->onStreamStateChange(state);
		}
	}
	StatefulSignal<PacketStreamState>::onStateChange(state, oldState); // send events
}


string PacketStream::name() const
{
	Mutex::ScopedLock lock(_mutex);
	return _name;
}


int PacketStream::numSources() const
{
	Mutex::ScopedLock lock(_mutex);
	return _sources.size();
}


int PacketStream::numProcessors() const
{
	Mutex::ScopedLock lock(_mutex);
	return _processors.size();
}


int PacketStream::numAdapters() const
{
	Mutex::ScopedLock lock(_mutex);
	return _sources.size() + _processors.size();
}
	
	
bool PacketStream::running() const
{
	return stateEquals(PacketStreamState::Running);
}

	
bool PacketStream::closed() const
{
	return stateEquals(PacketStreamState::Closing)
		|| stateEquals(PacketStreamState::Closed)
		|| stateEquals(PacketStreamState::Error);
}


PacketAdapterList PacketStream::adapters() const
{
	Mutex::ScopedLock lock(_mutex);
	return _processors;
}
	

void PacketStream::setClientData(void* data)
{
	Mutex::ScopedLock lock(_mutex);
	_clientData = data;
}


void* PacketStream::clientData() const
{
	Mutex::ScopedLock lock(_mutex);
	return _clientData;
}


//
// Packet Stream Adapter
//


void PacketStreamAdapter::emit(char* data, size_t len, void* opaque)
{
	PacketSignal::emit(this, RawPacket(data, len, opaque));
}


void PacketStreamAdapter::emit(const char* data, size_t len, void* opaque)
{
	PacketSignal::emit(this, RawPacket(data, len, opaque));
}


void PacketStreamAdapter::emit(const std::string& str, void* opaque)
{
	PacketSignal::emit(this, RawPacket(str.data(), str.length(), opaque));
}


void PacketStreamAdapter::emit(IPacket& packet)
{
	PacketSignal::emit(this, packet);
}


} // namespace scy




/*
bool PacketStream::waitForReady()
{	
	traceL("PacketStream", this) << "Waiting for ready" << endl;
	bool res = true; //_ready.tryWait(2000);
	traceL("PacketStream", this) << "Waiting for ready: " << res << endl;
	assert(res && "packet stream locked");
	return res;
	return true;
}
*/