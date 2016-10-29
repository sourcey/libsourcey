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


#include "scy/packetstream.h"
#include "scy/packetqueue.h"
#include "scy/memory.h"


using std::endl;


namespace scy {


PacketStream::PacketStream(const std::string& name) :
    _name(name),
    _autoStart(false),
    _closeOnError(true),
    _clientData(nullptr)
{
    TraceS(this) << "Create" << endl;
}


PacketStream::~PacketStream()
{
    TraceS(this) << "Destroy" << endl;

    close();

    // Delete managed adapters
    reset();

    // The event machine should always be complete
    assert(stateEquals(PacketStreamState::None)
        || stateEquals(PacketStreamState::Closed)
        || stateEquals(PacketStreamState::Error));

    // Make sure all adapters have been cleaned up
    assert(_sources.empty());
    assert(_processors.empty());

    //TraceS(this) << "Destroy: OK" << endl;
}


void PacketStream::start()
{
    TraceS(this) << "Start" << endl;

    //Mutex::ScopedLock lock(_mutex);
    if (stateEquals(PacketStreamState::Active)) {
        TraceS(this) << "Start: Already active" << endl;
        //assert(0);
        return;
    }

    // Setup the delegate chain
    setup();

    // Set state to Active
    setState(this, PacketStreamState::Active);

    // Lock the processor mutex to synchronize multi source streams
    Mutex::ScopedLock lock(_procMutex);

    // Start synchronized sources
    startSources();
}


void PacketStream::stop()
{
    TraceS(this) << "Stop" << endl;

    //Mutex::ScopedLock lock(_mutex);
    if (stateEquals(PacketStreamState::Stopped) ||
        stateEquals(PacketStreamState::Stopping) ||
        stateEquals(PacketStreamState::Closed)) {
        TraceS(this) << "Stop: Already stopped" << endl;
        //assert(0);
        return;
    }

    setState(this, PacketStreamState::Stopping);
    setState(this, PacketStreamState::Stopped);

    // Lock the processor mutex to synchronize multi source streams
    Mutex::ScopedLock lock(_procMutex);

    // Stop synchronized sources
    stopSources();

    TraceS(this) << "Stop: OK" << endl;
}


void PacketStream::pause()
{
    TraceS(this) << "Pause" << endl;
    //Mutex::ScopedLock lock(_mutex);
    setState(this, PacketStreamState::Paused);
}


void PacketStream::resume()
{
    TraceS(this) << "Resume" << endl;
    //Mutex::ScopedLock lock(_mutex);
    if (!stateEquals(PacketStreamState::Paused)) {
        TraceS(this) << "Resume: Not paused" << endl;
        return;
    }

    setState(this, PacketStreamState::Active);
}


// void PacketStream::reset()
// {
//     TraceS(this) << "Reset" << endl;
//
//     //Mutex::ScopedLock lock(_mutex);
//     setState(this, PacketStreamState::Resetting);
//     setState(this, PacketStreamState::Active);
// }


void PacketStream::close()
{
    //Mutex::ScopedLock lock(_mutex);
    if (stateEquals(PacketStreamState::None) ||
        stateEquals(PacketStreamState::Closed)) {
        //TraceS(this) << "Already closed" << endl;
        //assert(0);
        return;
    }

    // Stop the stream gracefully (if running)
    if (!stateEquals(PacketStreamState::Stopped) &&
        !stateEquals(PacketStreamState::Stopping))
        stop();

    TraceS(this) << "Closing" << endl;

    // Queue the Closed state
    setState(this, PacketStreamState::Closed);

    {
        // Lock the processor mutex to synchronize multi source streams
        Mutex::ScopedLock lock(_procMutex);

        // Teardown the adapter delegate chain
        teardown();

        // Wait for thread-based runners to stop running in order
        // to ensure safe destruction of stream adapters.
        // This call does nothing for non thread-based runners.
        //waitForRunner();

        // Synchronize any pending states
        // This should be safe since the adapters won't be receiving
        // any more incoming packets after teardown.
        // This call is essential when using the event loop otherwise
        // failing to close some handles could result in deadlock.
        // See SyncQueue::cancel()
        synchronizeStates();

        // Clear and delete managed adapters
        // Note: Can't call this because if closeOnCleanup is true
        // we may be inside a queue loop which will be destroyed by
        // the call to reset()
        //reset();
    }

    // Send the Closed signal
    Close.emit(this);

    TraceS(this) << "Close: OK" << endl;
}


void PacketStream::write(char* data, std::size_t len)
{
    RawPacket p(data, len);
    process(p);
}


void PacketStream::write(const char* data, std::size_t len)
{
    RawPacket p(data, len);
    process(p);
}

void PacketStream::write(IPacket& packet)
{
    process(packet);
}


bool PacketStream::locked() const
{
    return stateEquals(PacketStreamState::Locked);
}


#if 0
void PacketStream::attachSource(PacketStreamAdapter* source, bool freePointer, bool syncState)
{
    //Mutex::ScopedLock lock(_mutex);
    attachSource(source, freePointer, syncState);
}


void PacketStream::attachSource(PacketSignal& source)
{
    //Mutex::ScopedLock lock(_mutex);
    attachSource(new PacketStreamAdapter(source), true, false);
}


bool PacketStream::detachSource(PacketStreamAdapter* source)
{
    //Mutex::ScopedLock lock(_mutex);
    return detachSource(source);
}


bool PacketStream::detachSource(PacketSignal& source)
{
    //Mutex::ScopedLock lock(_mutex);
    return detachSource(source);
}


void PacketStream::attach(PacketProcessor* proc, int order, bool freePointer)
{
    //Mutex::ScopedLock lock(_mutex);
    attach(proc, order, freePointer);
}


bool PacketStream::detach(PacketProcessor* proc)
{
    //Mutex::ScopedLock lock(_mutex);
    return detach(proc);
}


void PacketStream::synchronizeOutput(uv::Loop* loop)
{
    //Mutex::ScopedLock lock(_mutex);
    synchronizeOutput(loop);
}


PacketStream& PacketStream::base() const
{
    Mutex::ScopedLock lock(_mutex);
    if (!_base)
        throw std::runtime_error("Packet stream context not initialized.");
    return *_base;
}


PacketStream::PacketStream(PacketStream* stream) :
    _stream(stream),
    _closeOnError(false)
{
    TraceS(this) << "Create" << endl;
}


PacketStream::~PacketStream()
{
    TraceS(this) << "Destroy" << endl;

    // The event machine should always be complete
    assert(stateEquals(PacketStreamState::None)
        || stateEquals(PacketStreamState::Closed)
        || stateEquals(PacketStreamState::Error));

    // Make sure all adapters have been cleaned up
    assert(_sources.empty());
    assert(_processors.empty());

    TraceS(this) << "Destroy: OK" << endl;
}


const std::exception_ptr& PacketStream::error()
{
    //Mutex::ScopedLock lock(_mutex);
    return error();
}


bool PacketStream::async() const
{
   return _runner && _runner->async();
}
#endif


bool PacketStream::lock()
{
    //Mutex::ScopedLock lock(_mutex);
    if (!stateEquals(PacketStreamState::None))
        return false;

    setState(this, PacketStreamState::Locked);
    return true;
}


bool PacketStream::active() const
{
    //Mutex::ScopedLock lock(_mutex);
    return stateEquals(PacketStreamState::Active);
}


bool PacketStream::closed() const
{
    //Mutex::ScopedLock lock(_mutex);
    return stateEquals(PacketStreamState::Closed)
        || stateEquals(PacketStreamState::Error);
}


bool PacketStream::stopped() const
{
    //Mutex::ScopedLock lock(_mutex);
    return stateEquals(PacketStreamState::Stopping)
        || stateEquals(PacketStreamState::Stopped);
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


void PacketStream::autoStart(bool flag)
{
    //Mutex::ScopedLock lock(_mutex);
    _autoStart = flag;
}


void PacketStream::closeOnError(bool flag)
{
    //Mutex::ScopedLock lock(_mutex);
    _closeOnError = flag;
}


std::string PacketStream::name() const
{
    Mutex::ScopedLock lock(_mutex);
    return _name;
}


//
// Packet Stream Base
//


void PacketStream::synchronizeStates()
{
    // Process queued internal states first
    while (!_states.empty()) {
        PacketStreamState state;
        {
            Mutex::ScopedLock lock(_mutex);
            state = _states.front();
            _states.pop_front();
        }

        TraceS(this) << "Set queued state: " << state << endl;

        // Send the stream state to packet adapters.
        // This is done inside the processor thread context so
        // packet adapters do not need to consider thread safety.
        auto adapters = this->adapters();
        for (auto& ref : adapters) {
            auto adapter = dynamic_cast<PacketStreamAdapter*>(ref->ptr);
            if (adapter)
                adapter->onStreamStateChange(state);
            else assert(0);
        }
    }
}


void PacketStream::process(IPacket& packet)
{
    TraceS(this) << "Processing packet: "
        << state() << ": " << packet.className() << endl;
    //assert(Thread::currentID() == _runner->tid());

    try {

        // Auto start the stream if required and currently inactive
        if (_autoStart &&
            stateEquals(PacketStreamState::None) // &&
            // stateEquals(PacketStreamState::Locked)
            )
            start();

        // Process the packet if the stream is active
        PacketProcessor* firstProc = nullptr;
        if (stateEquals(PacketStreamState::Active) && !packet.flags.has(PacketFlags::NoModify)) {
            {
                Mutex::ScopedLock lock(_mutex);
                firstProc = !_processors.empty() ?
                    reinterpret_cast<PacketProcessor*>(_processors[0]->ptr) : nullptr;
            }
            if (firstProc) {

                // Lock the processor mutex to synchronize multi source streams
                Mutex::ScopedLock lock(_procMutex);

                // Sync queued states
                synchronizeStates();

                // Send the packet to the first processor in the chain
                if (stateEquals(PacketStreamState::Active)) {
                    // TraceS(this) << "Starting process chain: "
                    //     << firstProc << ": " << packet.className() << endl;
                    //assert(stateEquals(PacketStreamState::Active));
                    if (firstProc->accepts(packet))
                        firstProc->process(packet);
                    else
                        firstProc->emit(packet);

                    // TraceS(this) << "After process chain: "
                    //     << firstProc << ": " << packet.className() << endl;
                    // If all went well the packet was processed and emitted...
                }

                // Proxy packets which are rejected by the first processor
                else {
                    WarnS(this) << "Source packet rejected: "
                        << firstProc << ": " << packet.className() << endl;
                    firstProc = nullptr;
                }
            }
        }

        // Otherwise just proxy and emit the packet
        // TODO: Should we pass the packet to the PacketSyncQueue if
        // synchronizeOutput was used?
        if (!firstProc) {
            TraceS(this) << "Proxying packet: " << state() << ": " << packet.className() << endl;
            emit(packet);
        }

        // Stop the packet stream on the final packet
        if (stateEquals(PacketStreamState::Active) && packet.flags.has(PacketFlags::Final)) {
            this->stop();
        }
    }

    // Catch any exceptions thrown within the processor
    catch (std::exception& exc) {
        ErrorS(this) << "Processor error: " << exc.what() << endl;

        // Set the stream Error state. No need for queueState
        // as we are currently inside the processor context.
        setState(this, PacketStreamState::Error, exc.what());

        // Capture the exception so it can be rethrown elsewhere.
        // The Error signal will be sent on next call to emit()
        _error = std::current_exception();
        /*stream()->*/Error.emit(this, _error);

        //_syncError = true;
        if (_closeOnError) {
            TraceS(this) << "Close on error" << endl;
            this->close();
        }
    }

    // TraceS(this) << "End process chain: "
    //     << state() << ": " << packet.className() << endl;
}


void PacketStream::emit(IPacket& packet)
{
    TraceS(this) << "Emit: " << packet.size() << endl;

    // PacketStream* stream = this->stream();
    // const PacketStreamState& state = this->state();

    // Synchronize the error if required
    // if (_syncError && state.equals(PacketStreamState::Error)) {
    //     _syncError = false;
    //     if (stream)
    //         stream->Error.emit(stream, error());
    //
    //     if (_closeOnError) {
    //         TraceS(this) << "Close on error" << endl;
    //         stream->close();
    //         return;
    //     }
    // }

    // Ensure the stream is still running
    if (!stateEquals(PacketStreamState::Active)) {
        TraceS(this) << "Dropping packet on inactive stream: " << state() << endl;
        return;
    }

    try {
        // Emit the result packet
        if (emitter.enabled()) {
            emitter.emit(this, packet);
        }
        else {
            TraceS(this) << "Dropping packet: No emitter: " << state() << endl;
        }
    }
    catch (std::exception& exc) {
        ErrorL << "Emit error: " << exc.what() << std::endl;

        // Set the stream Error state. No need for queueState
        // as we are currently inside the processor context.
        setState(this, PacketStreamState::Error, exc.what());

        // Capture the exception so it can be rethrown elsewhere.
        // The Error signal will be sent on next call to emit()
        _error = std::current_exception();
        /*stream()->*/Error.emit(this, _error);

        //_syncError = true;
        if (_closeOnError) {
            TraceS(this) << "Close on error" << endl;
            this->close();
        }
    }

    TraceS(this) << "Emit: OK: " << packet.size() << endl;
}


void PacketStream::setup()
{
    try {
        Mutex::ScopedLock lock(_mutex);

        // Setup the processor chain
        PacketProcessor* lastProc = nullptr;
        PacketProcessor* thisProc = nullptr;
        for (auto& proc : _processors) {
            thisProc = reinterpret_cast<PacketProcessor*>(proc->ptr);
            if (lastProc) {
                lastProc->getEmitter().attach(packetDelegate(thisProc, &PacketProcessor::process));
            }
            lastProc = thisProc;
        }

        // The last processor will emit the packet to the application
        if (lastProc)
            lastProc->getEmitter().attach(packetDelegate(this, &PacketStream::emit));

        // Attach source emitters to the PacketStream::process method
        for (auto& source : _sources) {
            source->ptr->getEmitter().attach(packetDelegate(this, &PacketStream::process));
        }
    }
    catch (std::exception& exc) {
        ErrorS(this) << "Cannot start stream: " << exc.what() << endl;
        setState(this, PacketStreamState::Error, exc.what());
        throw exc;
    }
}


void PacketStream::teardown()
{
    TraceS(this) << "Teardown" << endl;
    Mutex::ScopedLock lock(_mutex);

    // Detach the processor chain first
    PacketProcessor* lastProc = nullptr;
    PacketProcessor* thisProc = nullptr;
    for (auto& proc : _processors) {
        thisProc = reinterpret_cast<PacketProcessor*>(proc->ptr);
        if (lastProc)
            lastProc->getEmitter().detach(packetDelegate(thisProc, &PacketProcessor::process));
        lastProc = thisProc;
    }
    if (lastProc)
        lastProc->getEmitter().detach(packetDelegate(this, &PacketStream::emit));

    // Detach sources
    for (auto& source : _sources) {
        source->ptr->getEmitter().detach(packetDelegate(this, &PacketStream::process));
    }

    TraceS(this) << "Teardown: OK" << endl;
}


void PacketStream::reset()
{
    TraceS(this) << "Reset" << endl;

    assert(stateEquals(PacketStreamState::None)
        || stateEquals(PacketStreamState::Closed));

    Mutex::ScopedLock lock(_mutex);
    auto sit = _sources.begin();
    while (sit != _sources.end()) {
        //TraceS(this) << "Remove source: " << (*sit)->ptr << endl; // << ": " << (*sit).freePointer
        //if ((*sit).freePointer) {
//#ifdef _DEBUG
            //delete (*sit)->ptr;
//#else
//            deleteLater<PacketStreamAdapter>((*sit)->ptr);
//#endif
        //}
        sit = _sources.erase(sit);
    }

    auto pit = _processors.begin();
    while (pit != _processors.end()) {
        //TraceS(this) << "Remove processor: " << (*pit)->ptr << endl; //<< ": " << (*pit).freePointer
        //if ((*pit).freePointer) {
//#ifdef _DEBUG
            //delete (*pit)->ptr;
//#else
//            deleteLater<PacketStreamAdapter>((*pit)->ptr);
//#endif
        //}
        pit = _processors.erase(pit);
    }

    //TraceS(this) << "Cleanup: OK" << endl;
}


void PacketStream::attachSource(PacketStreamAdapter* source, bool freePointer, bool syncState)
{
    //TraceS(this) << "Attach source: " << source << endl;
    attachSource(std::make_shared<PacketAdapterReference>(source, freePointer ?
        new ScopedRawPointer<PacketStreamAdapter>(source) : nullptr, 0, syncState)); //freePointer,
}


void PacketStream::attachSource(PacketAdapterReference::Ptr ref)
{
    assertCanModify();

    Mutex::ScopedLock lock(_mutex);
    _sources.push_back(ref);
    std::sort(_sources.begin(), _sources.end(), PacketAdapterReference::compareOrder);
}


void PacketStream::attachSource(PacketSignal& source)
{
    //TraceS(this) << "Attach source signal: " << &source << endl;
    assertCanModify();

    // TODO: unique_ptr for exception safe pointer creation so we
    // don't need to do state checks here as well as attachSource(PacketStreamAdapter*)
    attachSource(new PacketStreamAdapter(source), true, false);
}


bool PacketStream::detachSource(PacketStreamAdapter* source)
{
    //TraceS(this) << "Detach source adapter: " << source << endl;
    assertCanModify();

    Mutex::ScopedLock lock(_mutex);
    for (auto it = _sources.begin(); it != _sources.end(); ++it) {
        if ((*it)->ptr == source) {
            (*it)->ptr->getEmitter().detach(packetDelegate(this, &PacketStream::write));
            TraceS(this) << "Detached source adapter: " << source << endl;

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
    //TraceS(this) << "Detach source signal: " << &source << endl;
    assertCanModify();

    Mutex::ScopedLock lock(_mutex);
    for (auto it = _sources.begin(); it != _sources.end(); ++it) {
        if (&(*it)->ptr->getEmitter() == &source) {
            (*it)->ptr->getEmitter().detach(packetDelegate(this, &PacketStream::write));
            TraceS(this) << "Detached source signal: " << &source << endl;

            // Free the PacketStreamAdapter wrapper instance,
            // not the referenced PacketSignal.
            //assert((*it).freePointer);
            //delete (*it)->ptr;
            _sources.erase(it);
            return true;
        }
    }
    return false;
}


void PacketStream::attach(PacketProcessor* proc, int order, bool freePointer)
{
    //TraceS(this) << "Attach processor: " << proc << endl;
    assert(order >= -1 && order <= 101);
    assertCanModify();

    Mutex::ScopedLock lock(_mutex);
    //_processors.push_back(std::make_shared<PacketAdapterReference>(proc,
    //    order == 0 ? _processors.size() : order, freePointer));

    _processors.push_back(std::make_shared<PacketAdapterReference>(proc,
        freePointer ? new ScopedRawPointer<PacketStreamAdapter>(proc) : nullptr, order == -1 ? _processors.size() : order)); //freePointer,

    sort(_processors.begin(), _processors.end(), PacketAdapterReference::compareOrder);
}


bool PacketStream::detach(PacketProcessor* proc)
{
    //TraceS(this) << "Detach processor: " << proc << endl;
    assertCanModify();

    Mutex::ScopedLock lock(_mutex);
    for (auto it = _processors.begin(); it != _processors.end(); ++it) {
        if ((*it)->ptr == proc) {
            TraceS(this) << "Detached processor: " << proc << endl;

            // Note: The PacketStream is no longer responsible
            // for deleting the managed pointer.
            _processors.erase(it);
            return true;
        }
    }
    return false;
}


void PacketStream::attach(PacketAdapterReference::Ptr ref)
{
    assertCanModify();

    Mutex::ScopedLock lock(_mutex);
    _processors.push_back(ref);
    std::sort(_processors.begin(), _processors.end(), PacketAdapterReference::compareOrder);
}


void PacketStream::startSources()
{
    //Mutex::ScopedLock lock(_mutex);
    auto sources = this->sources();
    for (auto& source : sources) {
        if (source->syncState) {
            auto startable = dynamic_cast<async::Startable*>(source->ptr);
            if (startable) {
                TraceS(this) << "Start source: " << startable << endl;
                startable->start();
            }
            else assert(0 && "unknown synchronizable");
#if 0
            auto runnable = dynamic_cast<async::Runnable*>(source);
            if (runnable) {
                TraceS(this) << "Starting runnable: " << source << endl;
                runnable->run();
            }
#endif
        }
    }
}


void PacketStream::stopSources()
{
    //Mutex::ScopedLock lock(_mutex);
    auto sources = this->sources();
    for (auto& source : sources) {
        if (source->syncState) {
            auto startable = dynamic_cast<async::Startable*>(source->ptr);
            if (startable) {
                TraceS(this) << "Stop source: " << startable << endl;
                startable->stop();
            }
            else assert(0 && "unknown synchronizable");
#if 0
            auto runnable = dynamic_cast<async::Runnable*>(source);
            if (runnable) {
                TraceS(this) << "Stop runnable: " << source << endl;
                runnable->cancel();
            }
#endif
        }
    }
}


bool PacketStream::waitForRunner()
{
    /*
    TraceS(this) << "Wait for sync: "
        << times << ": "
        << !_runner->cancelled() << ": "
        << _runner->running()
        << endl;

    if (!_runner || !_runner->async())
        return false;

    //assert(Thread::currentID() != _runner->tid());

    TraceS(this) << "Wait for sync" << endl;
    int times = 0;
    while (!_runner->cancelled() || _runner->running()) { //!_runner->cancelled() ||
        TraceS(this) << "Wait for sync: "
            << times << ": "
            << !_runner->cancelled() << ": "
            << _runner->running()
            << endl;
        scy::sleep(10);
        if (times++ > 500) {
            assert(0 && "deadlock; calling inside stream scope?"); // 5 secs
        }
    }
        */

    TraceS(this) << "Wait for sync: OK" << endl;
    return true;
}


bool PacketStream::waitForStateSync(PacketStreamState::ID state)
{
    int times = 0;
    TraceS(this) << "Wait for sync state: " << state << endl;
    while (!stateEquals(state) || hasQueuedState(state)) {
        TraceS(this) << "Wait for sync state: " << state << ": " << times << endl;
        scy::sleep(10);
        if (times++ > 500) {
            assert(0 && "deadlock; calling inside stream scope?"); // 5 secs
        }
    }
    TraceS(this) << "Wait for sync state: " << state << ": OK" << endl;
    return true;
}


bool PacketStream::hasQueuedState(PacketStreamState::ID state) const
{
    Mutex::ScopedLock lock(_mutex);
    for (auto const& st : _states) {
        if (st.id() == state)
            return true;
    }
    return false;
}


void PacketStream::assertCanModify()
{
    if (stateEquals(PacketStreamState::Locked) ||
        stateEquals(PacketStreamState::Stopping) ||
        stateEquals(PacketStreamState::Active)) {
        ErrorS(this) << "Cannot modify an " << state() << " packet stream" << endl;
        assert(0 && "cannot modify active packet stream");
        throw std::runtime_error("Cannot modify an active packet stream.");
    }
}


void PacketStream::synchronizeOutput(uv::Loop* loop)
{
    assertCanModify();

    // Add a SyncPacketQueue as the final processor so output
    // packets will be synchronized when they hit the emit() method
    attach(new SyncPacketQueue<>(loop), 101, true);
}


void PacketStream::onStateChange(PacketStreamState& state, const PacketStreamState& oldState)
{
    TraceS(this) << "On state change: " << oldState << " => " << state << endl;

    // Queue state for passing to adapters
    Mutex::ScopedLock lock(_mutex);
    _states.push_back(state);
}


const std::exception_ptr& PacketStream::error()
{
    Mutex::ScopedLock lock(_mutex);
    return _error;
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


PacketAdapterVec PacketStream::adapters() const
{
    Mutex::ScopedLock lock(_mutex);
    PacketAdapterVec res(_sources);
    res.insert(res.end(), _processors.begin(), _processors.end());
    return res;
}


PacketAdapterVec PacketStream::sources() const
{
    Mutex::ScopedLock lock(_mutex);
    return _sources;
}


PacketAdapterVec PacketStream::processors() const
{
    Mutex::ScopedLock lock(_mutex);
    return _processors;
}


//
// Packet Stream Adapter
//


PacketStreamAdapter::PacketStreamAdapter(PacketSignal& emitter) :
    _emitter(emitter)
{
}


void PacketStreamAdapter::emit(char* data, std::size_t len, unsigned flags)
{
    RawPacket p(data, len, flags);
    emit(p);
}


void PacketStreamAdapter::emit(const char* data, std::size_t len, unsigned flags)
{
    RawPacket p(data, len, flags);
    emit(p);
}


void PacketStreamAdapter::emit(const std::string& str, unsigned flags)
{
    RawPacket p(str.c_str(), str.length(), flags);
    emit(p);
}


void PacketStreamAdapter::emit(unsigned flags)
{
    FlagPacket p(flags);
    emit(p);
}


void PacketStreamAdapter::emit(IPacket& packet)
{
    getEmitter().emit(this, packet);
}


PacketSignal& PacketStreamAdapter::getEmitter()
{
    return _emitter;
}


} // namespace scy
