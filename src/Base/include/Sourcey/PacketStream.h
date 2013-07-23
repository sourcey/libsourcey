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
#include "Sourcey/PacketSignal.h"
#include "Sourcey/Interfaces.h"


namespace scy {
	

struct PacketStreamState;


//
// Packet Stream Adapter
//


class PacketStreamAdapter: public PacketSignal
	/// The PacketStreamAdapter is used as glue to wrap  
	/// external classes for integrating with the PacketStream
	/// data flow.
{ 
public:
	virtual void onStreamStateChange(const PacketStreamState&) {};
		/// Called by the PacketStream to notify when the
		/// internal state changes.
	
	void emit(char* data, size_t len, void* opaque = nullptr);

	void emit(const char* data, size_t len, void* opaque = nullptr);

	void emit(const std::string& str, void* opaque = nullptr);

	void emit(IPacket& packet);
};


typedef PacketStreamAdapter IDepacketizer;
	/// For 0.8.x compatibility


//
// Packet Stream Adapter interfaces
//


template<class InterfaceT>
class PacketSource: public PacketStreamAdapter, public InterfaceT
	/// PacketSource is the original source for packets
	/// processed by a PacketStream.
	///
	/// This class can be used in conjunction with various 
	/// interfaces such as Startable and Runnable to be 
	/// synchronized with the PacketStream state.
{ 
public:
	virtual void operator >> (char* data) { emit(RawPacket(data, sizeof(data))); };
		/// Stream operator alias for emit()

	virtual void operator >> (const char* data) { emit(RawPacket(data, sizeof(data))); };
		/// Stream operator alias for emit()

	virtual void operator >> (const std::string& str) { emit(RawPacket(str.data(), str.length())); };
		/// Stream operator alias for emit()

	virtual void operator >> (IPacket& packet) { emit(packet); };
		/// Stream operator alias for emit()
};


typedef PacketSource<abstract::Startable> StartablePacketSource;
	/// A PacketSource which may be started and stopped.


typedef PacketSource<abstract::Runnable> RunnablePacketSource;
	/// A PacketSource which may be run and cancelled.


/*	
	//virtual void parse(IPacket& packet) = 0;
		/// This method performs processing on the given
		/// packet and emits the resulting packet.
		///
		/// If packet processing is asynchronous the 
		/// packet data must be copied. Copied data can be 
		/// freed directly after the call to emit() when 
		/// dispatching the packet.
	//write(RawPacket(data, len));, size_t len
	//write(RawPacket(data, len));
class PacketParser: public PacketStreamAdapter
{ 
public:
	virtual void parse(IPacket& packet) = 0;
		/// This method performs processing on the given
		/// packet and emits the resulting packet.
		///
		/// If packet processing is asynchronous the  
		/// packet data must be copied. Copied data can be 
		/// freed directly aFter the call to emit() when
		/// the packet is dispatched.

	virtual bool accepts(IPacket&) { return true; };
		/// This method ensures compatibility with the given 
		/// packet type. Return false to reject the packet.

	virtual void operator << (IPacket& packet) { parse(packet); };
		/// Stream operator alias for parse()
};
*/


class PacketProcessor: public PacketStreamAdapter
	/// This class is a virtual interface for processors that
	/// process and emit the IPacket type. 
	/// PacketProcessor derive classes generally belong to a 
	/// PacketStream.
{ 
public:
	virtual void process(IPacket& packet) = 0;
		/// This method performs processing on the given
		/// packet and emits the resulting packet.
		///
		/// NOTE: If packet processing is asynchronous the  
		/// packet data must be copied. Copied data can be 
		/// freed directly aFter the call to emit() when
		/// the packet is dispatched.
		///
		/// NOTE: The implementation should re-dispatch the 
		/// packet even if it was not compatible.

	virtual bool accepts(IPacket&) { return true; };
		/// This method ensures compatibility with the given 
		/// packet type. Return false to reject the packet.

	virtual void operator << (IPacket& packet) { process(packet); };
		/// Stream operator alias for process()
};


typedef PacketProcessor IPacketizer;


//
// Packet Adapter Reference
//


struct PacketAdapterReference
	/// Provides a reference to a PacketSignal instance.
{
	PacketSignal* ptr;
	int order;
	bool freePointer;	
	bool syncState;

	PacketAdapterReference(PacketSignal* ptr = nullptr, int order = 0, 
		bool freePointer = true, bool syncState = false) : 
		ptr(ptr), order(order), freePointer(freePointer), 
		syncState(syncState) {}
		
	static bool CompareOrder(const PacketAdapterReference& l, const PacketAdapterReference& r) 
	{
		return l.order < r.order;
	}
};


typedef std::vector<PacketAdapterReference> PacketAdapterList;


//
// Packet Stream State
//


struct PacketStreamState: public State 
{
	enum Type 
	{
		None = 0,
		Running,
		Stopped,
		Resetting,
		Closing,
		Closed,
		Error,
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:				return "None";
		case Running:			return "Running";
		case Stopped:			return "Stopped";
		case Resetting:			return "Resetting";
		case Closing:			return "Closing";
		case Closed:			return "Closed";
		case Error:				return "Error";
		default:				assert(false);
		}
		return "undefined"; 
	}
};


//
// Packet Stream
//


class PacketStream: public PacketSignal, public StatefulSignal<PacketStreamState>, public abstract::Startable
	/// This class provides an interface for processing packets.
	/// A PacketStream consists of a single PacketSource,
	/// one or many PacketProcessors, and one or many 
	/// delegate receivers.
{ 
public:	
	PacketStream(const std::string& name = ""); 
	~PacketStream(); 

	//virtual void destroy();
	
	virtual void start(); // depreciated
		/// Starts the stream

	virtual void write(char* data, size_t len, void* opaque = nullptr);
		/// Writes data to the stream (nocopy).
		///
		/// The opaque is assigned to IPacket::opaque for access 
		/// by stream processors.

	virtual void write(const char* data, size_t len, void* opaque = nullptr);
		/// Writes data to the stream (copied).
		///
		/// The opaque is assigned to IPacket::opaque for access 
		/// by stream processors.

	virtual void write(const std::string& str, void* opaque = nullptr);
		/// Writes data to the stream (copied).
		///
		/// The opaque is assigned to IPacket::opaque for access 
		/// by stream processors.

	virtual void write(IPacket& packet);
		/// Processes an incoming source packet.

	virtual void emit(IPacket& packet);
		/// Called internally to emits the final packet.
		/// Exposed publicly for flexible use.

	virtual void stop(); // depreciated
		/// Stops the stream

	virtual void reset();
		/// Resets the internal state of all packet adapters in the
		/// stream. Useful for correcting metadata generation when
		/// new receivers connect to the stream.

	virtual void close();
		/// Closes the stream and transitions the internal state
		/// to Closed.
		///
		/// This method is also called by the destructor.

	virtual void attachSource(PacketSignal* source, bool freePointer = true, bool syncState = false);
		/// Attaches a source packet emitter to the stream.
		/// The source packet emitter can be another PacketStream.
		///
		/// If freePointer is true, the pointer will be deleted when
		/// the stream is closed.
		///
		/// If syncState and the PacketSignal is castable to a 
		/// IStratable the source's start() and stop() methods will be 
		/// called when the stream is started and stopped.

	virtual bool detachSource(PacketSignal* source);
		/// Detaches a source packet emitter to the stream.
		/// The source packet emitter can be another PacketStream.
		///
		/// NOTE: If you use this method to detach a packet source emitter then
		/// the PacketStream is no longer responsible for freeing the pointer,
		/// even if freePointer was set when calling attach().

	virtual void attach(PacketProcessor* proc, int order = 0, bool freePointer = true);
		/// Attaches a source packet emitter to the stream.
		/// The order value determines the order in which stream 
		/// processors are called.
		///
		/// If freePointer is true, the pointer will be deleted when
		/// the stream closes.

	virtual bool detach(PacketProcessor* proc);
		/// Detaches a source packet processor to the stream.
		/// NOTE: If you use this method the pointer will not be
		/// freed when the stream closes, even if freePointer was  
		/// true when calling attach().
	
	virtual void attach(const PacketDelegateBase& delegate);
		/// Attaches an output delegate to the stream.
		/// Resulting packets will be emitted to output delegates.

	virtual bool detach(const PacketDelegateBase& delegate);
		/// Detaches an output delegate from the stream.
	
	virtual std::string name() const;
		/// Returns the name of the stream.
	
	virtual bool running() const;
		/// Returns true is the stream is in the Running state.
	
	virtual bool closed() const;
		/// Returns true is the stream is in the Closing, 
		/// Closed or Error state.
	
	virtual void setClientData(void* data);
	virtual void* clientData() const;
		/// Provides the ability to associate a non managed
		/// arbitrary client data pointer with the stream.

	PacketAdapterList adapters() const;
		/// Returns a list of all stream sources and processors.

	virtual int numSources() const;
	virtual int numProcessors() const;
	virtual int numAdapters() const;
	
	template <class AdapterT>
	AdapterT* getSource(int index = 0)
	{
		int x = 0;
		for (unsigned i = 0; i < _sources.size(); i++) {
			AdapterT* source = dynamic_cast<AdapterT*>(_sources[i].ptr);
			if (source) {
				if (index == x)
					return source;
				else x++;
			}
		}
		return NULL;
	}

	template <class AdapterT>
	AdapterT* getProcessor(int index = 0)
	{
		int x = 0;
		for (unsigned i = 0; i < _processors.size(); i++) {
			AdapterT* processor = dynamic_cast<AdapterT*>(_processors[i].ptr);
			if (processor) {
				if (index == x)
					return processor;
				else x++;
			}
		}
		return NULL;
	}

protected:
	//virtual ~PacketStream();

	virtual void cleanup();
		/// Detaches all stream adapters and deletes 
		/// managed adapters.
	
	//virtual void onFinalPacket(IPacket& packet);
	virtual void onStateChange(PacketStreamState& state, const PacketStreamState& oldState);

protected:
	friend class GCDeleter<PacketStream>;

	mutable Mutex	_mutex;

	std::string _name;
	PacketAdapterList _sources;
	PacketAdapterList _processors;
	void* _clientData;
};


typedef std::vector<PacketStream*> PacketStreamList;


} // namespace scy


#endif // SOURCEY_PacketStream_H



	
	//Poco::Event _ready;
	//virtual bool waitForReady();
		/// Locks until the internal ready event is signalled.
		/// This enables safe stream adapter access after calling
		/// stop() by waiting until the current adapter queue
		/// iteration is complete.


/*
//template <class T>
class IDepacketizer: public PacketProcessor
	/// This class is a virtual interface for implementing depacketization
	/// of IPacket types.
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
	/// This class is a virtual interface for implementing packetization
	/// of IPacket types.
{
public:
	IPacketizer(int mtu = 1450) :
		_mtu(mtu) {}

	virtual void process(IPacket& packet) = 0;
		/// Processes incoming packets and outputs packetized packets.

	virtual void setMTU(int n)
		/// MTU defines the maximum transmission unit, which is the size 
		/// of the largest packet that a network protocol can transmit. 
		/// For UDP the maximum Ethernet frame size is 1500. In is the
		/// responsibility of subclasses to ensure this is implemented. 
		/// Emitted packets by the packetizer must not exceed the MTU.
	{ _mtu =  n; }

protected:
	int _mtu;
};
*/
