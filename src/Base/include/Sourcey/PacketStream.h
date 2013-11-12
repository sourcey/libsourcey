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


#ifndef SOURCEY_PacketStream_H
#define SOURCEY_PacketStream_H


#include "Sourcey/Types.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Exception.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Interface.h"
#include "Sourcey/Queue.h"
#include "Sourcey/PacketSignal.h"


namespace scy {
	

struct PacketStreamState;


//
// Packet Stream Adapter
//


class PacketStreamAdapter
	/// This class is a wrapper for integrating external
	/// classes with the a PacketStream's data flow and
	/// state machine.
{ 
public:
	PacketStreamAdapter(PacketSignal& emitter); // = nullptr
	virtual ~PacketStreamAdapter() {};

	virtual void emit(char* data, std::size_t len, unsigned flags = 0);
	virtual void emit(const char* data, std::size_t len, unsigned flags = 0);
	virtual void emit(const std::string& str, unsigned flags = 0);
	virtual void emit(IPacket& packet);

	PacketSignal& getEmitter();
		// Returns a reference to the outgoing packet signal.

	virtual void onStreamStateChange(const PacketStreamState&) {};
		// Called by the PacketStream to notify when the internal
		// Stream state changes.	
		// On receiving the Stopped state, it is the responsibility
		// of the adapter to have ceased all outgoing packet transmission,
		// especially in multi-thread scenarios.

protected:
	PacketStreamAdapter(const PacketStreamAdapter&); // = delete;
	PacketStreamAdapter(PacketStreamAdapter&&); // = delete;
	PacketStreamAdapter& operator=(const PacketStreamAdapter&); // = delete;
	PacketStreamAdapter& operator=(PacketStreamAdapter&&); // = delete;

	PacketSignal& _emitter;
};


typedef PacketStreamAdapter PacketSource;
	/// For 0.8.x compatibility


//
// PacketProcessor
//


class PacketProcessor: public PacketStreamAdapter
	/// This class is a virtual interface for creating 
	/// PacketStreamAdapters which process that and emit
	/// the IPacket type. 
{ 
public:
	PacketProcessor(PacketSignal& emitter) : // = nullptr
		PacketStreamAdapter(emitter)
	{
	}
	
	virtual void process(IPacket& packet) = 0;
		// This method performs processing on the given
		// packet and emits the result.
		//
		// Note: If packet processing is async (the packet is not in
		// the current thread scope) then packet data must be copied.
		// Copied data can be freed directly aFter the async call to
		// emit() the outgoing packet.

	virtual bool accepts(IPacket&) { return true; };
		// This method ensures compatibility with the given 
		// packet type. Return false to reject the packet.	 

	virtual void operator << (IPacket& packet) { process(packet); };
		// Stream operator alias for process()
};


typedef PacketProcessor IPacketizer;
typedef PacketProcessor IDepacketizer;
	// For 0.8.x compatibility


//
// Packet Adapter Reference
//


struct PacketAdapterReference
	/// Provides a reference to a PacketSignal instance.
{
	PacketStreamAdapter* ptr;
	int order;
	bool freePointer;	
	bool syncState;

	PacketAdapterReference(PacketStreamAdapter* ptr = nullptr, int order = 0, 
		bool freePointer = true, bool syncState = false) : 
		ptr(ptr), order(order), freePointer(freePointer), 
		syncState(syncState) {}
		
	static bool compareOrder(const PacketAdapterReference& l, const PacketAdapterReference& r) 
	{
		return l.order < r.order;
	}
};


typedef std::vector<PacketAdapterReference> PacketAdapterVec;


enum PacketFlags 
	/// Flags which determine how the packet is handled by the PacketStream
{	
	NoModify = 0x01,    // The packet should not be modified by processors.
	Final		        // The final packet in the stream.
};


//
// Packet Stream State
//


struct PacketStreamState: public State 
{
	enum Type 
	{
		None = 0,
		Locked,
		Active,
		Paused,
		Resetting,
		Stopping,
		Stopped,
		Closed,
		Error,
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Locked:		return "Locked";
		case Active:		return "Active";
		case Paused:		return "Paused";
		case Resetting:		return "Resetting";
		case Stopping:		return "Stopping";
		case Stopped:		return "Stopped";
		case Closed:		return "Closed";
		case Error:			return "Error";
		default:			assert(false);
		}
		return "undefined"; 
	}
};


//
// Packet Stream Context
//


class PacketStream;
class PacketStreamBase: public Stateful<PacketStreamState>//, public RunnableQueue<IPacket>
	/// PacketStreamBase contains methods relevent to the internal stream API.
{	
	PacketStreamBase(PacketStream* stream);
	virtual ~PacketStreamBase();

public:	
	PacketStream* stream() const;
		// Returns a pointer to the parent stream wrapper.
	
	PacketAdapterVec adapters() const;
		// Returns a combined list of all stream sources and processors.

	PacketAdapterVec sources() const;
		// Returns a list of all stream sources.

	PacketAdapterVec processors() const;
		// Returns a list of all stream processors.
	
	bool waitForRunner();
		// Block the calling thread until all packets have been flushed,
		// and internal states have been synchronized.
		// This function is only useful after calling stop() or pause().

	bool waitForStateSync(PacketStreamState::ID state);
		// Block the calling thread until the given state is synchronized.

	int numSources() const;
	int numProcessors() const;
	int numAdapters() const;

	template <class AdapterT>
	AdapterT* getSource(int index = 0)
	{
		int x = 0;
		Mutex::ScopedLock lock(_mutex);
		for (unsigned i = 0; i < _sources.size(); i++) {
			AdapterT* source = dynamic_cast<AdapterT*>(_sources[i].ptr);
			if (source) {
				if (index == x)
					return source;
				else x++;
			}
		}
		return nullptr;
	}

	template <class AdapterT>
	AdapterT* getProcessor(int index = 0)
	{
		int x = 0;
		Mutex::ScopedLock lock(_mutex);
		for (unsigned i = 0; i < _processors.size(); i++) {
			AdapterT* processor = dynamic_cast<AdapterT*>(_processors[i].ptr);
			if (processor) {
				if (index == x)
					return processor;
				else x++;
			}
		}
		return nullptr;
	}

	PacketProcessor* getProcessor(int order = 0)
		// Returns the PacketProcessor at the given position.
	{
		Mutex::ScopedLock lock(_mutex);
		for (unsigned i = 0; i < _processors.size(); i++) {
			PacketProcessor* processor = dynamic_cast<PacketProcessor*>(_processors[i].ptr);
			if (processor && _processors[i].order == order) {
				return processor;
			}
		}
		return nullptr;
	}

protected:		
	void setup();
		// Attach the source and processor delegate chain.

	void teardown();
		// Detach the source and processor delegate chain.

	void cleanup();
		// Remove all stream adapters and delete managed adapters.

	void emit(IPacket& packet);
		// Emit the final packet to listeners.
		//
		// Synchronized signals such as Close and Error are sent
		// from this method. See synchronizeOutput()
	
	void attachSource(PacketSignal& source);
		// Attach a source packet emitter to the stream.
		// The source packet adapter can be another PacketStream::emitter.
	
	void attachSource(PacketStreamAdapter* source, bool freePointer = true, bool syncState = false);
		// Attach a source packet emitter to the stream.
		// If freePointer is true, the pointer will be deleted when the stream is closed.
		// If syncState is true and the source is a basic::Stratable, then
		// the source's start()/stop() methods will be synchronized when
		// calling startSources()/stopSources().
	
	bool detachSource(PacketSignal& source);
		// Detaches the given source packet signal from the stream.

	bool detachSource(PacketStreamAdapter* source);
		// Detaches the given source packet adapter from the stream.
		//
		// Note: The pointer will be forgotten about, so if the freePointer
		// flag set when calling attachSource() will have no effect.

	void attach(PacketProcessor* proc, int order = 0, bool freePointer = true);
		// Attach a packet processor to the stream.
		// Order determines the position of the processor in the stream queue.
		// If freePointer is true, the pointer will be deleted when the stream closes.

	bool detach(PacketProcessor* proc);
		// Detaches a packet processor from the stream.
		//
		// Note: The pointer will be forgotten about, so if the freePointer
		// flag set when calling attach() will have no effect.

	void synchronizeOutput(uv::Loop* loop);
		// Synchronize stream output and events with the given event loop.

	const std::exception_ptr& error();
		// Returns the stream error (if any).
	
	virtual void process(IPacket& packet);
		// Overrides RunnableQueue::dispatch to process an incoming packet.
	
	void startSources();
		// Start synchronized sources.

	void stopSources();
		// Stop synchronized sources.
	
	void synchronizeStates();
		// Synchronize queued states with adapters.
	
	virtual void onStateChange(PacketStreamState& state, const PacketStreamState& oldState);
		// Override the Stateful::onStateChange method 
	
	//void queueState(PacketStreamState::ID state);
	bool hasQueuedState(PacketStreamState::ID state) const;
	
	void assertNotActive();
		// Asserts that the stream is not in or pending the Active state.
	
	void setStream(PacketStream* stream);
		// Sets the stream wrapper instance.

	friend class PacketStream;
	friend struct std::default_delete<PacketStreamBase>;
	
	mutable Mutex _mutex;
	mutable Mutex _procMutex;
	PacketStream* _stream;
	PacketAdapterVec _sources;
	PacketAdapterVec _processors;
	std::deque<PacketStreamState> _states;
	//async::Runner::ptr _runner;
	std::exception_ptr _error;
	bool _closeOnError;
	//bool _syncError;
};


//
// Packet Stream
//


class PacketStream
	/// This class is used for processing and boradcasting IPackets in a flexible way.
	/// A PacketStream consists of one or many PacketSources, one or many
	/// PacketProcessors, and one or many delegate receivers.
	///
	/// This class enables the developer to setup a processor chain in order
	/// to perform arbitrary processing on data packets using interchangeable 
	/// packet adapters, and pump the output to any delegate function, 
	/// or even another PacketStream.
	///
	/// Note that PacketStream itself inherits from PacketStreamAdapter, 
	/// so a PacketStream be the source of another PacketStream.
	///
	/// All PacketStream methods are thread-safe, but once the stream is 
	/// running you will not be able to attach or detach stream adapters.
	///
	/// In order to synchronize output packets with the application event
	/// loop take a look at the SyncPacketQueue class.
	/// For lengthy operations you can add an AsyncPacketQueue to the start
	/// of the stream to defer processing from the PacketSource thread.
{ 
public:	
	PacketStream(const std::string& name = "");
	virtual ~PacketStream();
	
	virtual void start();
		// Start the stream and synchronized sources.

	virtual void stop();
		// Stop the stream and synchronized sources.

	virtual void pause();
		// Pause the stream.

	virtual void resume();
		// Resume the stream.

	virtual void reset();
		// Reset the internal state of all packet adapters in the stream. 
		// Useful for correcting metadata generation when new receivers
		// connect to the stream.

	virtual void close();
		// Close the stream and transition the internal state to Closed.
	
	virtual bool active() const;
		// Returns true when the stream is in the Active state.
	
	virtual bool stopped() const;
		// Returns true when the stream is in the Stopping or Stopped state.
	
	virtual bool closed() const;
		// Returns true when the stream is in the Closed or Error state.

	//virtual bool async() const;
		// Returns true is the underlying Runner is set and is thread-based.
	
	virtual bool lock();
		// Sets the stream to locked state.
		// In a locked state no new adapters can be added or removed
		// from the stream until the stream is stopped.
	
	virtual bool locked() const;
		// Returns true is the stream is currently locked.

	virtual void write(char* data, std::size_t len);
		// Write data to the stream (nocopy).
	
	virtual void write(const char* data, std::size_t len);
		// Write data to the stream (copied).

	virtual void write(IPacket& packet);
		// Write an incoming packet onto the stream.

	virtual void attachSource(PacketSignal& source);
		// Attach a source packet emitter to the stream.
		// The source packet adapter can be another PacketStream::emitter.
	
	virtual void attachSource(PacketStreamAdapter* source, bool freePointer = true, bool syncState = false);
		// Attach a source packet emitter to the stream.
		// If freePointer is true, the pointer will be deleted when the stream is closed.
		// If syncState is true and the source is a basic::Stratable, then
		// the source's start()/stop() methods will be synchronized when
		// calling startSources()/stopSources().
	
	virtual bool detachSource(PacketSignal& source);
		// Detaches the given source packet signal from the stream.

	virtual bool detachSource(PacketStreamAdapter* source);
		// Detaches the given source packet adapter from the stream.
		// Note: The pointer will be forgotten about, so if the freePointer
		// flag set when calling attachSource() will have no effect.

	virtual void attach(PacketProcessor* proc, int order = 0, bool freePointer = true);
		// Attach a packet processor to the stream.
		// Order determines the position of the processor in the stream queue.
		// If freePointer is true, the pointer will be deleted when the stream closes.

	virtual bool detach(PacketProcessor* proc);
		// Detaches a packet processor from the stream.
		// Note: The pointer will be forgotten about, so if the freePointer
		// flag set when calling attach() will have no effect.

	virtual void synchronizeOutput(uv::Loop* loop);
		// Synchronize stream output packets with the given event loop.
	
	virtual void closeOnError(bool flag);
		// Set the stream to be closed on error.
	
	virtual void setClientData(void* data);
	virtual void* clientData() const;
		// Unmanaged client data pointer.

	PacketStreamBase& base() const;
		// Returns a reference to the base implementation.
	
	const std::exception_ptr& error();
		// Returns the stream error (if any).
	
	std::string name() const;
		// Returns the name of the packet stream.

	PacketSignal emitter;
		// Signals to delegates on outgoing packets.
	
	Signal<const std::exception_ptr&> Error;
		// Signals that the PacketStream is in Error state.
		// If stream output is synchronized then the Error signal will be
		// sent from the synchronization context, otherwise it will be sent from 
		// the async processor context. See synchronizeOutput()

	NullSignal Close;
		// Signals that the PacketStream is in Close state.
		// This signal is sent immediately via the close() method, 
		// and as such will be sent from the calling thread context.

protected:
	friend class PacketStreamBase;

	mutable Mutex _mutex;
	void* _clientData;
	std::string _name;
	std::shared_ptr<PacketStreamBase> _base;
};


typedef std::vector<PacketStream*> PacketStreamVec;


} // namespace scy


#endif // SOURCEY_PacketStream_H



	//void dispose();
		// Schedule the pointer for deletion.
	
	//void write(IPacket& packet);
		// Write an incoming packet onto the stream.
	//virtual void setRunner(async::Runner::ptr runner);
		// Set the asynchronous context for packet processing.
		// This may be a Thread or another derivative of Async.
		// Must be set before the stream is activated.


	//virtual bool dispatchNext();
		// Overrides RunnableQueue::dispatchNext to ensure that stream 
		// states are correctly synchronized with the async context.
	//bool _syncClose;
	
	//bool _deleted;

	//void sync(IPacket& packet);
		// Synchronizes the outgoing packet before it is emitted.
		// If no synchronization context is set then proxies to emit()
	
	
	//void setEmitter(PacketSignal* emitter);
		// Sets the emitter instance.
		// Once the emitter instance is set it cannot be changed.
	//std::shared_ptr<PacketStreamBase> _self;
	//virtual void run();
	//void processSyncStates();

	//void processPendingStates();

	//scy::Error _error;
	// _queue; // thread-safe
	//std::deque<std::pair<PacketStreamState, PacketStreamState>> _syncStates;

	//void assertCmdThread();

	//virtual void sync(IPacket& packet);
		// Synchronize the final packet with the main thread before emitting.
