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
#include "Sourcey/InterProc.h"
#include "Sourcey/PacketSignal.h"
#include "Sourcey/Interface.h"


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
	PacketStreamAdapter(PacketSignal& emitter);
	virtual ~PacketStreamAdapter() {};

	virtual void emit(char* data, std::size_t len, unsigned flags = 0);
	virtual void emit(const char* data, std::size_t len, unsigned flags = 0);
	virtual void emit(const std::string& str, unsigned flags = 0);
	virtual void emit(IPacket& packet);

	PacketSignal& emitter();
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

	PacketSignal* _emitter;
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
	PacketProcessor(PacketSignal& emitter) :
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


//
// Packet Stream State
//


struct PacketStreamState: public State 
{
	enum Type 
	{
		None = 0,
		Locked,
		Running,
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
		case Running:		return "Running";
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


class PacketStream: 
	public PacketStreamAdapter, 
	public PacketSignal, 
	public Stateful<PacketStreamState>, 
	public basic::Startable
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
	~PacketStream();
	
	virtual void start();
		// Start the stream and any synchronized sources.

	virtual void stop();
		// Stop the stream and any synchronized sources.

	virtual void pause();
		// Pause the stream.

	virtual void resume();
		// Resume the stream.

	virtual void reset();
		// Reset the internal state of all packet adapters in the
		// stream. Useful for correcting metadata generation when
		// new receivers connect to the stream.

	virtual void close();
		// Close the stream and transition the internal state to Closed.

	virtual void write(char* data, std::size_t len);
		// Write data to the stream (nocopy).
	
	virtual void write(const char* data, std::size_t len);
		// Write data to the stream (copied).

	virtual void write(IPacket& packet);
		// Writes an incoming packet to the stream.

	virtual void emit(IPacket& packet);
		// Called internally to emits the final packet.
		// Exposed publicly for flexible use.

	virtual void attachSource(PacketSignal& source);
		// Attach a source packet emitter to the stream.
		// The source packet adapter can be another PacketStream::emitter.
	
	virtual void attachSource(PacketStreamAdapter* source, bool freePointer = true, bool syncState = false);
		// Attach a source packet emitter to the stream.
		// If freePointer is true, the pointer will be deleted when the stream is closed.
		// If syncState and the PacketSignal is castable to a  basic::Stratable
		// the source's start() and stop() methods will be synchronized with 
		// the internal stream state.
	
	virtual bool detachSource(PacketSignal& source);
		// Detaches the given source packet signal from the stream.

	virtual bool detachSource(PacketStreamAdapter* source);
		// Detaches the given source packet adapter from the stream.
		//
		// Note: The pointer will be forgotten about, so if the freePointer
		// flag set when calling attachSource() will have no effect.

	virtual void attach(PacketProcessor* proc, int order = 0, bool freePointer = true);
		// Attach a packet processor to the stream.
		// Order determines the position of the processor in the stream queue.
		// If freePointer is true, the pointer will be deleted when the stream closes.

	virtual bool detach(PacketProcessor* proc);
		// Detaches a packet processor from the stream.
		//
		// Note: The pointer will be forgotten about, so if the freePointer
		// flag set when calling attach() will have no effect.

	//PacketSignal emitter;
		// Signals to delegates on outgoing packets.
	
	virtual std::string name() const;
		// Returns the name of the stream.
	
	virtual bool running() const;
		// Returns true when the stream is in the Running state.
	
	virtual bool closed() const;
		// Returns true when the stream is in the Closing, Closed or Error state.
	
	virtual bool lock();
		// Sets the stream to locked state.
		//
		// In a locked state no new adapters can be added or removed
		// from the stream until the stream is stopped.
	
	virtual bool locked() const;
		// Returns true is the stream is currently locked.
	
	virtual void setClientData(void* data);
	virtual void* clientData() const;
		// Client data pointer.

	virtual bool waitForReady();
		// Locks until the internal ready event is signalled.
		// This blocks the active thread until the current precessor iteration
		// is complete, as such is it only useful after calling stop() or pause().

	PacketAdapterVec adapters() const;
		// Returns a combined list of all stream sources and processors.

	PacketAdapterVec sources() const;
		// Returns a list of all stream sources.

	PacketAdapterVec processors() const;
		// Returns a list of all stream processors.

	virtual int numSources() const;
	virtual int numProcessors() const;
	virtual int numAdapters() const;
	
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
	virtual void cleanup();
		// Detaches all stream adapters and deletes managed adapters.
	
	virtual void onStateChange(PacketStreamState& state, const PacketStreamState& oldState);

protected:
	friend struct std::default_delete<PacketStream>;

	mutable Mutex _mutex;

	std::string _name;
	PacketAdapterVec _sources;
	PacketAdapterVec _processors;
	std::atomic<int> _scopeRef;
	void* _clientData;
};


typedef std::vector<PacketStream*> PacketStreamVec;


} // namespace scy


#endif // SOURCEY_PacketStream_H