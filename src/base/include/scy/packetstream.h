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


#ifndef SCY_PacketStream_H
#define SCY_PacketStream_H


#include "scy/types.h"
#include "scy/logger.h"
#include "scy/memory.h"
#include "scy/exception.h"
#include "scy/stateful.h"
#include "scy/interface.h"
#include "scy/queue.h"
#include "scy/packetsignal.h"


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
	typedef std::shared_ptr<PacketAdapterReference> Ptr;

	PacketStreamAdapter* ptr;
	ScopedPointer* deleter;
	int order;
	//bool freePointer;	
	bool syncState;

	PacketAdapterReference(PacketStreamAdapter* ptr = nullptr, ScopedPointer* deleter = nullptr, int order = 0, bool syncState = false) : //bool freePointer = true
		ptr(ptr), deleter(deleter), order(order), syncState(syncState) //freePointer(freePointer), 		
	{
	}

	~PacketAdapterReference()
	{
		if (deleter)
			delete deleter;
	}
		
	static bool compareOrder(const PacketAdapterReference::Ptr& l, const PacketAdapterReference::Ptr& r) 
	{
		return l->order < r->order;
	}
};


typedef std::vector<PacketAdapterReference::Ptr> PacketAdapterVec;


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
// Packet Stream
//


class PacketStream: public Stateful<PacketStreamState>
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
	typedef std::shared_ptr<PacketStream> Ptr;

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

	virtual void close();
		// Close the stream and transition the internal state to Closed.

	virtual void reset();
		// Cleanup all managed stream adapters and reset the stream state.
	
	virtual bool active() const;
		// Returns true when the stream is in the Active state.
	
	virtual bool stopped() const;
		// Returns true when the stream is in the Stopping or Stopped state.
	
	virtual bool closed() const;
		// Returns true when the stream is in the Closed or Error state.
	
	virtual bool lock();
		// Sets the stream to locked state.
		// In a locked state no new adapters can be added or removed
		// from the stream until the stream is stopped.
	
	virtual bool locked() const;
		// Returns true is the stream is currently locked.

	virtual void write(char* data, std::size_t len);
		// Writes data to the stream (nocopy).
	
	virtual void write(const char* data, std::size_t len);
		// Writes data to the stream (copied).

	virtual void write(IPacket& packet);
		// Writes an incoming packet onto the stream.

	virtual void attachSource(PacketSignal& source);
		// Attaches a source packet emitter to the stream.
		// The source packet adapter can be another PacketStream::emitter.
	
	virtual void attachSource(PacketStreamAdapter* source, bool freePointer = true, bool syncState = false);
		// Attaches a source packet emitter to the stream.
		// If freePointer is true, the pointer will be deleted when the stream is closed.
		// If syncState is true and the source is a basic::Stratable, then
		// the source's start()/stop() methods will be synchronized when
		// calling startSources()/stopSources().

	template <class C> void attachSource(std::shared_ptr<C> ptr, bool syncState = false)
		// Attaches a source packet emitter to the stream.
		// This method enables compatibility with shared_ptr managed adapter instances.
	{
		auto source = dynamic_cast<PacketStreamAdapter*>(ptr.get());
		if (!source) {			
			assert(0 && "invalid adapter");
			throw std::runtime_error("Cannot attach incompatible packet source.");
		}

		attachSource(std::make_shared<PacketAdapterReference>(
			source, new ScopedSharedPointer<C>(ptr), 0, syncState));
	}
	
	virtual bool detachSource(PacketSignal& source);
		// Detaches the given source packet signal from the stream.

	virtual bool detachSource(PacketStreamAdapter* source);
		// Detaches the given source packet adapter from the stream.
		// Note: The pointer will be forgotten about, so if the freePointer
		// flag set when calling attachSource() will have no effect.

	virtual void attach(PacketProcessor* proc, int order = 0, bool freePointer = true);
		// Attaches a packet processor to the stream.
		// Order determines the position of the processor in the stream queue.
		// If freePointer is true, the pointer will be deleted when the stream closes.

	template <class C> void attach(std::shared_ptr<C> ptr, bool syncState = false)
		// Attaches a packet processor to the stream.
		// This method enables compatibility with shared_ptr managed adapter instances.
	{
		auto proc = dynamic_cast<PacketProcessor*>(ptr.get());
		if (!proc) {			
			assert(0 && "invalid adapter");
			throw std::runtime_error("Cannot attach incompatible packet processor.");
		}

		attach(std::make_shared<PacketAdapterReference>(
			source, new ScopedSharedPointer<C>(ptr), 0, syncState));
	}

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
		// Accessors for the unmanaged client data pointer.
	
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
			AdapterT* source = dynamic_cast<AdapterT*>(_sources[i]->ptr);
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
			AdapterT* processor = dynamic_cast<AdapterT*>(_processors[i]->ptr);
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
			PacketProcessor* processor = dynamic_cast<PacketProcessor*>(_processors[i]->ptr);
			if (processor && _processors[i]->order == order) {
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

	void emit(IPacket& packet);
		// Emit the final packet to listeners.
		//
		// Synchronized signals such as Close and Error are sent
		// from this method. See synchronizeOutput()
	
	void attachSource(PacketAdapterReference::Ptr ref);
	void attach(PacketAdapterReference::Ptr ref);
	
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
	
	bool hasQueuedState(PacketStreamState::ID state) const;
		// Returns true if the given state ID is queued.
	
	void assertNotActive();
		// Asserts that the stream is not in or pending the Active state.

	mutable Mutex _mutex;
	mutable Mutex _procMutex;
	std::string _name;
	PacketAdapterVec _sources;
	PacketAdapterVec _processors;
	std::deque<PacketStreamState> _states;
	std::exception_ptr _error;
	bool _closeOnError;
	void* _clientData;
};


typedef std::vector<PacketStream*> PacketStreamVec;
typedef std::vector<PacketStream::Ptr> PacketStreamPtrVec;


} // namespace scy


#endif // SCY_PacketStream_H