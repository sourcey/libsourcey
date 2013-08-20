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
		// stream state changes.	
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


class PacketStream: public PacketStreamAdapter, public Stateful<PacketStreamState>, public basic::Startable
	/// This class implements a flexible and lightweight packet processing stream.
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
	/// For lengthy operations you add an AsyncPacketQueue instance to the 
	/// stream so as not to block the source thread while processing completes.
{ 
public:	
	PacketStream(const std::string& name = ""); 
	~PacketStream();
	
	virtual void start();
		// Starts the stream and any synchronized sources.

	virtual void stop();
		// Stops the stream and any synchronized sources.

	virtual void reset();
		// Resets the internal state of all packet adapters in the
		// stream. Useful for correcting metadata generation when
		// new receivers connect to the stream.

	virtual void close();
		// Closes the stream and transitions the internal state to Closed.

	virtual void write(char* data, std::size_t len);
		// Writes data to the stream (nocopy).
	
	virtual void write(const char* data, std::size_t len);
		// Writes data to the stream (copied).

	virtual void write(IPacket& packet);
		// Writes an incoming packet to the stream.

	virtual void emit(IPacket& packet);
		// Called internally to emits the final packet.
		// Exposed publicly for flexible use.

	virtual void attachSource(PacketSignal& source);
		// Attaches a source packet emitter to the stream.
	
	virtual void attachSource(PacketStreamAdapter* source, bool freePointer = true, bool syncState = false);
		// Attaches a source packet emitter to the stream.
		// The source packet adapter can be another PacketStream.
		// If freePointer is true, the pointer will be deleted when
		// the stream is closed.
		// If syncState and the PacketSignal is castable to a 
		// basic::Stratable the source's start() and stop() methods
		// will be synchronized with the internal stream state.
	
	virtual bool detachSource(PacketSignal& source);
		// Detaches the given source packet signal from the stream.

	virtual bool detachSource(PacketStreamAdapter* source);
		// Detaches the given source packet adapter from the stream.
		//
		// Note: The pointer will be forgotten about, and the freePointer
		// flag set when calling attach() will have no more effect.

	virtual void attach(PacketProcessor* proc, int order = 0, bool freePointer = true);
		// Attaches a source packet emitter to the stream.
		// Order determines the position of the processor  in the
		// stream queue.
		// If freePointer is true, the pointer will be deleted when
		// the stream closes.

	virtual bool detach(PacketProcessor* proc);
		// Detaches a source packet processor to the stream.
		//
		// Note: The pointer will be forgotten about, and the freePointer
		// flag set when calling attach() will have no effect.

	PacketSignal Emitter;
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
		// from the stream until the stream is stopped or restarted.
	
	virtual bool locked() const;
		// Returns true is the stream is in the locked.
	
	virtual void setClientData(void* data);
	virtual void* clientData() const;
		// Client data pointer.

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

protected:
	virtual void cleanup();
		// Detaches all stream adapters and deletes managed adapters.
	
	virtual void onStateChange(PacketStreamState& state, const PacketStreamState& oldState);

protected:
	friend struct std::default_delete/*friend class AsyncDeleter*/<PacketStream>;

	mutable Mutex	_mutex;

	std::string _name;
	PacketAdapterVec _sources;
	PacketAdapterVec _processors;
	void* _clientData;
};


typedef std::vector<PacketStream*> PacketStreamVec;


} // namespace scy


#endif // SOURCEY_PacketStream_H




	/*
//template<class InterfaceT>
class PacketSource: public PacketStreamAdapter//, public InterfaceT
	// PacketSource is the original source for packets
	// processed by a PacketStream.
	//
	// This class can be used in conjunction with various 
	// interfaces such as Startable and Runnable to be 
	// synchronized with the PacketStream state.
{ 
public:
	virtual void operator >> (char* data) { emit(rawPacket(data, sizeof(data))); };
	virtual void operator >> (const char* data) { emit(rawPacket(data, sizeof(data))); };
	virtual void operator >> (const std::string& str) { emit(rawPacket(str.c_str(), str.length())); };
	virtual void operator >> (IPacket& packet) { emit(packet); };
		// Stream operator aliases for emit()
};
		*/
	
	/*
	
	//virtual void write(const std::string& str);
		// Writes data to the stream (copied).
	//virtual ~PacketStream();

	//virtual void onFinalPacket(IPacket& packet);
	virtual void attach(const PacketDelegateBase& delegate);
		// Attaches an output delegate to the stream.
		// Resulting packets will be emitted to output delegates.

	virtual bool detach(const PacketDelegateBase& delegate);
		// Detaches an output delegate from the stream.
		*/

/*	

	virtual void process(IPacket& packet) = 0; { process(packet); };
		//
		// Note: The implementation should re-dispatch the 
		// packet even if it was not compatible.
		// Stream operator alias for process()

//typedef PacketSource<basic::Startable> StartablePacketSource;
	// A PacketSource which may be started and stopped.


//typedef PacketSource<basic::Runnable> RunnablePacketSource;
	// A PacketSource which may be run and cancelled.
	//virtual void parse(IPacket& packet) = 0;
		// This method performs processing on the given
		// packet and emits the resulting packet.
		//
		// If packet processing is asynchronous the 
		// packet data must be copied. Copied data can be 
		// freed directly after the call to emit() when 
		// dispatching the packet.
	//write(rawPacket(data, len));, std::size_t len
	//write(rawPacket(data, len));
class PacketParser: public PacketStreamAdapter
{ 
public:
	virtual void parse(IPacket& packet) = 0;
		// This method performs processing on the given
		// packet and emits the resulting packet.
		//
		// If packet processing is asynchronous the  
		// packet data must be copied. Copied data can be 
		// freed directly aFter the call to emit() when
		// the packet is dispatched.

	virtual bool accepts(IPacket&) { return true; };
		// This method ensures compatibility with the given 
		// packet type. Return false to reject the packet.

	virtual void operator << (IPacket& packet) { parse(packet); };
		// Stream operator alias for parse()
};
*/

	
	//Poco::Event _ready;
	//virtual bool waitForReady();
		// Locks until the internal ready event is signalled.
		// This enables safe stream adapter access after calling
		// stop() by waiting until the current adapter queue
		// iteration is complete.


/*
//template <class T>
class IDepacketizer: public PacketProcessor
	// This class is a virtual interface for implementing depacketization
	// of IPacket types.
{
public:
	IDepacketizer() :
		_sequenceNumber(0) {}

	virtual void process(IPacket& packet) = 0;
		// Processes incoming packets.

	virtual void emit(IPacket& packet)
		// Sends a packet to the handler and increments
		// the sequence counter.
	{
		_sequenceNumber++;	
		PacketProcessor::emit(this, packet);
	}

	void setSequenceNumber(int n) { _sequenceNumber =  n; }

protected:
	int _sequenceNumber;
};
*/


/*
class IPacketizer: public PacketProcessor
	// This class is a virtual interface for implementing packetization
	// of IPacket types.
{
public:
	IPacketizer(int mtu = 1450) :
		_mtu(mtu) {}

	virtual void process(IPacket& packet) = 0;
		// Processes incoming packets and outputs packetized packets.

	virtual void setMTU(int n)
		// MTU defines the maximum transmission unit, which is the size 
		// of the largest packet that a network protocol can transmit. 
		// For UDP the maximum Ethernet frame size is 1500. In is the
		// responsibility of subclasses to ensure this is implemented. 
		// Emitted packets by the packetizer must not exceed the MTU.
	{ _mtu =  n; }

protected:
	int _mtu;
};
*/
