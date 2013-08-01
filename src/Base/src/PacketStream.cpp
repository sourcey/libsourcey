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


using namespace std;


namespace scy {


PacketStream::PacketStream(const string& name) : 
	PacketStreamAdapter(Emitter),
	_name(name),
	_clientData(nil)
{
	traceL("PacketStream", this) << "Creating" << endl;
	//_ready.set();
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

	if (stateEquals(PacketStreamState::Running)) {
		traceL("PacketStream", this) << "Already running" << endl;
		return;
	}

	//assert(numAdapters() > 0);
	PacketAdapterVec processors(this->processors());
	PacketAdapterVec sources(this->sources());
	{	
		Emitter.enable(true);

		// Setup the processor chain
		PacketProcessor* lastProc = nil;
		PacketProcessor* thisProc = nil;
		for (PacketAdapterVec::iterator pit = processors.begin(); pit != processors.end(); ++pit) {
			thisProc = reinterpret_cast<PacketProcessor*>((*pit).ptr);
			if (lastProc) {
				lastProc->emitter().attach(packetDelegate(thisProc, &PacketProcessor::process));
			}
			lastProc = thisProc;
		}

		// The last processor will emit the result
		// packet to the outside application.
		if (lastProc)
			lastProc->emitter().attach(packetDelegate(this, &PacketStream::emit));

		// Attach source emitters to the PacketStream::write method
		for (PacketAdapterVec::iterator sit = sources.begin(); sit != sources.end(); ++sit) {
			PacketStreamAdapter* source = (*sit).ptr;
			source->emitter().attach(packetDelegate(this, &PacketStream::write));
		}
		
		// Set state to Running
		setState(this, PacketStreamState::Running);
		
		try {	
			// Initialize synchronized sources
			// TODO: Start sources on next loop iteration, or ensure they are asynchronous
			for (PacketAdapterVec::iterator sit = sources.begin(); sit != sources.end(); ++sit) {
				PacketStreamAdapter* source = (*sit).ptr;
				if ((*sit).syncState) {				
					auto startable = dynamic_cast<abstract::Startable*>(source);
					if (startable) {
						traceL("PacketStream", this) << "Starting: " << source << endl;
						startable->start();
					}
					else assert(0 && "unknown synchronizable");
					/*
					else {
						auto runnable = dynamic_cast<abstract::Runnable*>(source);
						if (runnable) {
							traceL("PacketStream", this) << "Starting runnable: " << source << endl;
							runnable->run();
						}
					}
					*/
				}
			}		
		}
		catch (std::exception& exc) {
			errorL("PacketStream", this) << "Source error: " << exc.what() << endl;
			setState(this, PacketStreamState::Error, exc.what());
		}	
	}
}


void PacketStream::stop()
{
	traceL("PacketStream", this) << "Stopping" << endl;
	
	if (stateEquals(PacketStreamState::Stopped) ||
		stateEquals(PacketStreamState::Stopping) ||
		stateEquals(PacketStreamState::Closed)) {
		traceL("PacketStream", this) << "Already stopped" << endl;
		return;
	}
	
	setState(this, PacketStreamState::Stopping);
	Emitter.enable(false);

	//waitForReady();

	{		
		ScopedLock lock(_mutex);		

		// Detach the processor chain first
		PacketProcessor* lastProc = nil;
		PacketProcessor* thisProc = nil;
		for (PacketAdapterVec::iterator pit = _processors.begin(); pit != _processors.end(); ++pit) {
			thisProc = reinterpret_cast<PacketProcessor*>((*pit).ptr);
			if (lastProc)
				lastProc->emitter().detach(packetDelegate(thisProc, &PacketProcessor::process));
			lastProc = thisProc;
		}
		if (lastProc)
			lastProc->emitter().detach(packetDelegate(this, &PacketStream::emit));

		// Detach sources, and stop synchronized sources
		for (PacketAdapterVec::iterator sit = _sources.begin(); sit != _sources.end(); ++sit) {
			PacketStreamAdapter* source = (*sit).ptr;
			source->emitter().detach(packetDelegate(this, &PacketStream::write));
			if ((*sit).syncState) {
				auto startable = dynamic_cast<abstract::Startable*>((*sit).ptr);
				if (startable) {
					traceL("PacketStream", this) << "Stopping startable: " << source << endl;
					startable->stop();
				}
				else assert(0 && "unknown synchronizable");
				/*
				auto runnable = dynamic_cast<abstract::Runnable*>((*sit).ptr);
				if (runnable) {
					traceL("PacketStream", this) << "Stopping runnable: " << source << endl;
					runnable->cancel();
				}
				assert(startable || runnable && "unknown synchronized adapter");
				*/
			}
		}
	}
	
	setState(this, PacketStreamState::Stopped);

	// All adapters must have ceased packet transmission by now...
}


void PacketStream::reset()
{
	traceL("PacketStream", this) << "Resetting stream" << endl;
	
	// TODO: Refactor
	setState(this, PacketStreamState::Resetting);
	setState(this, PacketStreamState::Running);
}


void PacketStream::close()
{
	traceL("PacketStream", this) << "Closing" << endl;

	if (stateEquals(PacketStreamState::Closed)) {
		traceL("PacketStream", this) << "Already closed" << endl;
		return;
	}
	
	if (!stateEquals(PacketStreamState::Stopped) &&
		!stateEquals(PacketStreamState::Stopping))
		stop();
	
	cleanup();	

	setState(this, PacketStreamState::Closed); // may result in destruction
	traceL("PacketStream", this) << "Closing: OK" << endl;
}


void PacketStream::attachSource(PacketStreamAdapter* source, bool freePointer, bool syncState) 
{
	traceL("PacketStream", this) << "Attaching source: " << source << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot attach source to running stream.");

	//waitForReady();

	ScopedLock lock(_mutex);
	_sources.push_back(PacketAdapterReference(source, 0, freePointer, syncState));
	sort(_sources.begin(), _sources.end(), PacketAdapterReference::compareOrder);
}


void PacketStream::attachSource(PacketSignal& source)
{
	traceL("PacketStream", this) << "Attaching source signal: " << &source << endl;
		
	// TODO: unique_ptr for exception safe pointer creation
	// so we don't need to de state checks here as well as 
	// attachSource(PacketStreamAdapter*)
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot attach source to running stream.");

	attachSource(new PacketStreamAdapter(source), true, false);
}


bool PacketStream::detachSource(PacketStreamAdapter* source) 
{
	traceL("PacketStream", this) << "Detaching source adapter: " << source << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot detach source from a running stream.");

	//waitForReady();

	ScopedLock lock(_mutex);
	for (PacketAdapterVec::iterator it = _sources.begin(); it != _sources.end(); ++it) {
		if ((*it).ptr == source) {
			traceL("PacketStream", this) << "Detached source adapter: " << source << endl;
			(*it).ptr->emitter().detach(packetDelegate(this, &PacketStream::write));
			
			// Note: The PacketStream is no longer responsible
			// for deleting the managed pointer.
			_sources.erase(it);
			return true;
		}
	}
	return false;
}


bool PacketStream::detachSource(PacketSignal& source) 
{
	traceL("PacketStream", this) << "Detaching source signal: " << &source << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot detach source from a running stream.");

	//waitForReady();

	ScopedLock lock(_mutex);
	for (PacketAdapterVec::iterator it = _sources.begin(); it != _sources.end(); ++it) {
		if (&(*it).ptr->emitter() == &source) {
			traceL("PacketStream", this) << "Detached source signal: " << &source << endl;
			(*it).ptr->emitter().detach(packetDelegate(this, &PacketStream::write));

			// Free the PacketStreamAdapter wrapper,
			// not the detached emitter signal.
			assert((*it).freePointer);
			delete (*it).ptr;
			_sources.erase(it);
			return true;
		}
	}
	return false;
}


void PacketStream::attach(PacketProcessor* proc, int order, bool freePointer) 
{
	traceL("PacketStream", this) << "Attaching processor: " << proc << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot attach processor to running stream.");

	//waitForReady();

	ScopedLock lock(_mutex);
	_processors.push_back(PacketAdapterReference(proc, order == 0 ? _processors.size() : order, freePointer));
	sort(_processors.begin(), _processors.end(), PacketAdapterReference::compareOrder);
}


bool PacketStream::detach(PacketProcessor* proc) 
{
	traceL("PacketStream", this) << "Detaching processor: " << proc << endl;
	if (stateEquals(PacketStreamState::Running))
		throw Exception("Cannot detach processor from a running stream.");
	
	//waitForReady();

	ScopedLock lock(_mutex);
	for (PacketAdapterVec::iterator it = _processors.begin(); it != _processors.end(); ++it) {
		if ((*it).ptr == proc) {
			traceL("PacketStream", this) << "Detached processor: " << proc << endl;

			// Note: The PacketStream is no longer responsible
			// for deleting the managed pointer.
			_processors.erase(it);
			return true;
		}
	}
	return false;
}


void PacketStream::cleanup()
{
	traceL("PacketStream", this) << "Cleanup" << endl;		
	if (stateEquals(PacketStreamState::Running)) {
		assert(0);
		throw Exception("Cannot cleanup a running stream.");
	}
	
	//waitForReady();
	
	ScopedLock lock(_mutex);
	PacketAdapterVec::iterator sit = _sources.begin();
	while (sit != _sources.end()) {
		traceL("PacketStream", this) << "Detached source: " 
			<< (*sit).ptr << ": " << (*sit).freePointer << endl;
		(*sit).ptr->emitter().detach(packetDelegate(this, &PacketStream::write));

		// Delete source pointer asynchronously so the stream may be 
		// closed and/or destroyed from within the source callback scope.
		if ((*sit).freePointer)
			deleteLater<PacketStreamAdapter>((*sit).ptr);
			//delete (*sit).ptr;
		sit = _sources.erase(sit);
	}	
	
	PacketAdapterVec::iterator pit = _processors.begin();
	while (pit != _processors.end()) {
		traceL("PacketStream", this) << "Detached processor: "
			<< (*pit).ptr << ": " << (*pit).freePointer << endl;
		if ((*pit).freePointer)
			deleteLater<PacketStreamAdapter>((*pit).ptr);
			//delete (*pit).ptr;
		pit = _processors.erase(pit);
	}
}


void PacketStream::write(char* data, size_t len)
{
	write(RawPacket(data, len));
}


void PacketStream::write(const char* data, size_t len)
{
	write(RawPacket(data, len));
}
	

void PacketStream::write(IPacket& packet)
{	
	// traceL("PacketStream", this) << "Processing packet: " << packet.className() << endl;	

	// TODO: Single call to check that stream is enabled
	if (!running() || !Emitter.enabled()) {
		debugL("PacketStream", this) << "Dropping packet: " 
			<< &packet << ": " << Emitter.enabled() << ": " << state() << endl;	
		//_ready.set(); 
		return;
	}	

	try {	
		// Reset the ready event to prevent any new adapters
		// or stream destruction while processing.
		//_ready.reset();

		// Obtain the first processor in the queue
		PacketProcessor* firstProc = nil;
		{
			ScopedLock lock(_mutex);
			PacketAdapterVec::iterator it = _processors.begin();
			if (it != _processors.end()) {

				firstProc = reinterpret_cast<PacketProcessor*>((*it).ptr);
				if (!firstProc->accepts(packet)) {
					warnL("PacketStream", this) << "Source packet rejected: " 
						<< firstProc << ": " << packet.className() << endl;
					firstProc = nil;
				}
			}
		}
			
		if (firstProc) {
			// Send the packet to the first processor in the chain
			//traceL("PacketStream", this) << "Starting process chain: " 
			//	<< &packet << ": " << firstProc << ": " << packet.className() << endl;
			firstProc->process(packet);
			
			// If all went well the packet was processed and emitted
		}

		// If there are no processors then proxy the packet
		else {
			traceL("PacketStream", this) << "Proxying: " << packet.className() << endl;
			emit(packet);
		}
		
		// Set the ready event once processing is complete
		//_ready.set();
	}
				
	// Catch any exceptions thrown within the processor scope 
	// and set the stream error state.
	catch (std::exception& exc) {
		errorL("PacketStream", this) << "Processor error: " << exc.what() << endl;
		//_ready.set();
		setState(this, PacketStreamState::Error, exc.what());
	}	
	
	//traceL("PacketStream", this) << "End process chain: " 
	//	<< &packet << ": " << packet.className() << endl;	
}


void PacketStream::emit(IPacket& packet)
{
	// No more adapters to process, ensure the stream 
	// is still running before we dispatch the goods.
	if (!running() || !Emitter.enabled()) {
		debugL("PacketStream", this) << "Dropping late Packet: " 
			<< Emitter.enabled() << ": " << state() << endl;	
		return;
	}

	//traceL("PacketStream", this) << "Emitting: " << &packet << ": " << packet.className() << endl;
	Emitter.emit(this, packet);
	//traceL("PacketStream", this) << "Emitting: OK: " << &packet << ": " << packet.className() << endl;	
}


void PacketStream::onStateChange(PacketStreamState& state, const PacketStreamState& oldState)
{
	traceL("PacketStream", this) << "State changed from " << oldState << " to " << state << endl;

	if (state.id() != PacketStreamState::Closed &&
		state.id() != PacketStreamState::None) {

		// TODO: Better way of getting events to adapters?
		PacketAdapterVec adapters = this->adapters();
		for (PacketAdapterVec::iterator it = adapters.begin(); it != adapters.end(); ++it) {			
			PacketStreamAdapter* adapter = dynamic_cast<PacketStreamAdapter*>((*it).ptr);
			assert(adapter);
			if (adapter)
				adapter->onStreamStateChange(state);
		}
	}

	StatefulSignal<PacketStreamState>::onStateChange(state, oldState); // emit events
}


string PacketStream::name() const
{
	ScopedLock lock(_mutex);
	return _name;
}


bool PacketStream::locked() const
{
	ScopedLock lock(_mutex);
	return stateEquals(PacketStreamState::Locked);
}


bool PacketStream::lock()
{
	if (!stateEquals(PacketStreamState::None))
		throw Exception("Only an inactive stream can be locked");

	return setState(this, PacketStreamState::Locked);
}


int PacketStream::numSources() const
{
	ScopedLock lock(_mutex);
	return _sources.size();
}


int PacketStream::numProcessors() const
{
	ScopedLock lock(_mutex);
	return _processors.size();
}


int PacketStream::numAdapters() const
{
	ScopedLock lock(_mutex);
	return _sources.size() + _processors.size();
}
	
	
bool PacketStream::running() const
{
	return stateEquals(PacketStreamState::Running);
}

	
bool PacketStream::closed() const
{
	return stateEquals(PacketStreamState::Closed)
		|| stateEquals(PacketStreamState::Error);
}


PacketAdapterVec PacketStream::adapters() const
{
	ScopedLock lock(_mutex);
	return _processors;
}
	

void PacketStream::setClientData(void* data)
{
	ScopedLock lock(_mutex);
	_clientData = data;
}


void* PacketStream::clientData() const
{
	ScopedLock lock(_mutex);
	return _clientData;
}


PacketAdapterVec PacketStream::sources() const
{
	ScopedLock lock(_mutex);
	return _sources;
}


PacketAdapterVec PacketStream::processors() const
{
	ScopedLock lock(_mutex);
	return _processors;
}


//
// Packet Stream Adapter
//


PacketStreamAdapter::PacketStreamAdapter(PacketSignal& emitter) :
	_emitter(emitter)
{
}


void PacketStreamAdapter::emit(char* data, size_t len, unsigned flags)
{
	emit(RawPacket(this, data, len));
}


void PacketStreamAdapter::emit(const char* data, size_t len, unsigned flags)
{
	emit(RawPacket(this, data, len, flags));
}


void PacketStreamAdapter::emit(const std::string& str, unsigned flags)
{
	emit(RawPacket(this, str.c_str(), str.length(), flags));
}


void PacketStreamAdapter::emit(IPacket& packet)
{
	emitter().emit(this, packet);
}


PacketSignal& PacketStreamAdapter::emitter()
{
	return _emitter;
}


} // namespace scy





/*
void PacketStream::destroy()
{
	traceL("PacketStream", this) << "Destroy later" << endl;
	// 
	close();
	deleteLater<PacketStream>(this);
}
*/



/*
void PacketStream::write(const std::string& str)
{
	write(RawPacket(str.c_str(), str.length()));
}
*/
	

/*
void PacketStream::attach(const PacketDelegateBase& delegate)
{
	traceL("PacketStream", this) << "Attaching delegate: " << delegate.object() << endl;
	PacketSignal::attach(delegate);
}


bool PacketStream::detach(const PacketDelegateBase& delegate)
{
	traceL("PacketStream", this) << "Detaching delegate: " << delegate.object() << endl;
	return PacketSignal::detach(delegate);
}
*/
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