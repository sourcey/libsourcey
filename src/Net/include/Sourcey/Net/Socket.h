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


#ifndef SOURCEY_NET_SocketBase_H
#define SOURCEY_NET_SocketBase_H


#include "Sourcey/Base.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Signal.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Network.h"


namespace scy {
namespace net {


class Socket;
class SocketPacket;
class SocketAdapter;


class SocketBase: public SharedObject
	/// SocketBase is the base socket 
	/// implementation which all sockets derive.
{
public:
	SocketBase();
	
	virtual void connect(const Address& address) = 0;
		// Connects to the given peer IP address.
		//
		// Throws an exception if the address is malformed.
		// Connection errors can be handled via the Error signal.

	virtual void connect(const std::string& host, UInt16 port);
		// Resolves and connects to the given host address.
		//
		// Throws an Exception if the host is malformed.
		// Since the DNS callback is asynchronous implementations need 
		// to listen for the Error signal for handling connection errors.		

	virtual bool shutdown() { throw std::runtime_error("Not implemented by protocol"); };
		// Sends the shutdown packet which should result is socket 
		// closure via callback.

	virtual void close() = 0;
		// Closes the underlying socket.

	virtual void reset() { throw std::runtime_error("Not implemented by protocol"); };
		// Resets the closed socket for reuse.

	virtual void bind(const Address& address, unsigned flags = 0) = 0;
		// Bind a local address to the socket.
		// The address may be IPv4 or IPv6 (if supported).
		//
		// Throws an Exception on error.

	virtual void listen(int backlog = 64) { (void)backlog; };
		// Listens the socket on the given address.
		//
		// Throws an Exception on error.
		
	virtual int send(const char* data, int len, int flags = 0) = 0;
		// Sends the given data buffer to the connected peer.

	virtual int send(const char* data, int len, const Address& peerAddress, int flags = 0) = 0;
		// Sends the given data buffer to the given peer address.
		//
		// For TCP sockets the given peer address must match the
		// connected peer address.
	
	virtual Address address() const = 0;
		// The locally bound address.
		//
		// This function will not throw.
		// A Wildcard 0.0.0.0:0 address is returned if 
		// the socket is closed or invalid.

	virtual Address peerAddress() const = 0;
		// The connected peer address.
		//
		// This function will not throw.
		// A Wildcard 0.0.0.0:0 address is returned if 
		// the socket is closed or invalid.

	virtual net::TransportType transport() const = 0;
		// The transport protocol: TCP, UDP or SSLTCP.
		
	virtual void setError(const scy::Error& err) = 0;
		// Sets the socket error.
		//
		// Setting the error will result in socket closure.

	virtual const scy::Error& error() const = 0;
		// Return the socket error if any.

	//virtual bool initialized() const = 0;
		// Returns true if the native socket handle is initialized.

	virtual bool closed() const = 0;
		// Returns true if the native socket handle is closed.

	virtual uv::Loop* loop() const = 0;
		// Returns the socket event loop.
	
	void* opaque;
		// Optional client data pointer.
		//
		// The pointer is not initialized or managed
		// by the socket base.
	
	virtual void emitConnect();
	virtual void emitRecv(const MutableBuffer& buf, const Address& peerAddr);
	virtual void emitError(const scy::Error& error);
	virtual void emitClose();
	
	virtual void addObserver(Socket* socket, bool shared = false);
	virtual void removeObserver(Socket* socket);
	//virtual void sortObservers();
	//virtual void swapAdapter(SocketAdapter* a, SocketAdapter* b);

protected:
	virtual ~SocketBase();
	friend struct std::default_delete<SocketBase>;	
	
	std::vector<Socket*> _observers;
	volatile bool _insideCallback;	
	
	friend class Socket;
	friend class SocketAdapter;
};


typedef std::vector<SocketBase*> SocketBaseList;


//
// Socket
//


class Socket
	/// Socket is a wrapper class for accessing the underlying 
	/// reference counted SocketBase instance.
	///
	/// Each Socket class has its own SocketBase instance. 
	/// See UDPSocket, TCPSocket and SSLSocket.
	///
	/// Socket exposes all basic SocketBase operations and can  
	/// be extended as necessary for different protocols.
{
public:
	Socket(const Socket& socket);
		// Attaches the SocketBase from the given socket and
		// increments the SocketBase reference count.
		
	Socket(SocketBase* base, bool shared);
		// Creates the Socket from the given SocketBase and attaches
		// the given or default SocketAdapter.
		//
		// If shared is true the SocketBase reference count will be
		// incremented. If not we do not increment the reference count.
		// This effectively means are taking ownership of the SocketBase, 
		// as the destruction of this Socket object will be directly 
		// responsible for the destruction of the SocketBase.

	Socket& operator = (const Socket& socket);
		// Assignment operator.
		//
		// Releases the socket's socket context and
		// attaches the socket context from the other socket and
		// increments the reference count of the SocketBase.

	virtual Socket& assign(SocketBase* base, bool shared);
		// Assigns the SocketBase instance for this socket.
		// Any methods that assigns the base instance should 
		// assign() so that subclasses can manage instance
		// pointer changes.
		
	virtual ~Socket();
		// Destroys the Socket and releases the socket context.
				
	virtual void connect(const std::string& host, UInt16 port);
	virtual void connect(const Address& address);
	virtual bool shutdown();
	virtual void close();

	virtual void bind(const Address& address);
	virtual void listen(int backlog = 64);
		
	virtual int send(const char* data, int len, int flags = 0);
	virtual int send(const char* data, int len, const Address& peerAddress, int flags = 0);
		// Sends the given data pointer to the connected peer.
		// Returns the number of bytes sent or -1 on error.
		// No exception will be thrown.

	virtual int send(const IPacket& packet, int flags = 0);
	virtual int send(const IPacket& packet, const Address& peerAddress, int flags = 0);
		// Sends the given packet to the connected peer.
		// Returns the number of bytes sent or -1 on error.
		// No exception will be thrown.

	virtual void send(void*, IPacket& packet);
		// Sends the given packet to the connected peer.
		// This method provides delegate compatability, and unlike
		// other send methods throws an exception if the underlying 
		// socket is closed.

	virtual void setError(const scy::Error& err);
		// Sets the socket error.
		// Setting the error will result in socket closure.

	bool closed() const;
		// Returns true if the native socket handle is closed.
	
	const scy::Error& error() const;
		// Return the socket error if any.

	net::TransportType transport() const;
		// The transport protocol: TCP, UDP or SSLTCP.
		// See TransportType definition.
	
	Address address() const;
		// The locally bound address.

	Address peerAddress() const;
		// The connected peer address.
	
	NullSignal Connect;
		// Signals that the socket is connected.

	Signal<SocketPacket&> Recv;
		// Signals when data is received by the socket.

	Signal<const scy::Error&> Error;
		// Signals that the socket is closed in error.
		// This signal will be sent just before the 
		// Closed signal.

	NullSignal Close;
		// Signals that the underlying socket is closed,
		// maybe in error.

	SocketBase& base() const;
		// Returns the SocketBase for this socket.

	SocketAdapter* adapter() const;
		// Returns the SocketAdapter for this socket.
		
	void setAdapter(SocketAdapter* adapter);
		// Sets the new SocketAdapter.
		// The old instance will not be destroyed.

	void replaceAdapter(SocketAdapter* adapter);
		// Sets the SocketAdapter instance,
		// and deletes the old one.

	virtual void onSocketConnect();
	virtual void onSocketRecv(const MutableBuffer& buf, const Address& peerAddr);
	virtual void onSocketError(const scy::Error& error);
	virtual void onSocketClose();
		
	int isNull() const;

protected:		
	Socket();
		// Creates a nullptr socket.

	friend class SocketBase;

	SocketBase* _base;
	SocketAdapter* _adapter;
};


//
// Socket Adapter
//


class SocketAdapter
	/// SocketAdapter is an proxy layer which is attached to a
	/// Socket instance to handle socket events in various ways.
	/// 
	/// This class also be extended to implement custom processing 
	/// for received socket data before it is dispatched to the application.
	/// See the PacketSocketAdapter and Transaction classes for ideas.
{
public:
	SocketAdapter(Socket* socket = nullptr);
		// Creates the SocketAdapter.
		//
		// The Socket instance can be nullptr, but it must be set 
		// before any callbacks come back.
	
	virtual ~SocketAdapter();

	virtual void onSocketConnect();
	virtual void onSocketRecv(const MutableBuffer& buf, const Address& peerAddr);
	virtual void onSocketError(const Error& error);
	virtual void onSocketClose();
		// These virtual methods can be overridden as necessary
		// to intercept socket events before they hit the application.
		
	virtual int send(const char* data, int len, int flags = 0);
	virtual int send(const char* data, int len, const Address& peerAddress, int flags = 0);
	
	Socket* socket;

private:
	SocketAdapter(const SocketAdapter&); // = delete;
	SocketAdapter(SocketAdapter&&); // = delete;
	SocketAdapter& operator=(const SocketAdapter&); // = delete;
	SocketAdapter& operator=(SocketAdapter&&); // = delete;
};


//
// Packet Info
//


struct PacketInfo: public IPacketInfo
	/// Provides information about packets emitted from a socket.
	/// See SocketPacket.
{ 
	Socket* socket;
		// The source socket

	Address peerAddress;	
		// The originating peer address.
		// For TCP this will always be connected address.

	PacketInfo(Socket* socket, const Address& peerAddress) :
		socket(socket), peerAddress(peerAddress) {}		

	PacketInfo(const PacketInfo& r) : 
		socket(r.socket), peerAddress(r.peerAddress) {}
	
	virtual IPacketInfo* clone() const {
		return new PacketInfo(*this);
	}

	virtual ~PacketInfo() {}; 
};


//
// Socket Packet
//


class SocketPacket: public RawPacket 
	/// SocketPacket is the default packet type emitted by sockets.
	/// SocketPacket provides peer address information and a buffer
	/// reference for nocopy binary operations.
	///
	/// The referenced packet buffer lifetime is only guaranteed 
	/// for the duration of the receiver callback.
{	
public:
	PacketInfo* info;
		// PacketInfo pointer

	SocketPacket(Socket* socket, const MutableBuffer& buffer, const Address& peerAddress) : 
		RawPacket(bufferCast<char*>(buffer), buffer.size(), 0, &socket, nullptr, 
			new PacketInfo(socket, peerAddress))
	{
		info = (PacketInfo*)RawPacket::info;
	}

	SocketPacket(const SocketPacket& that) : 
		RawPacket(that), info(that.info)
	{
	}
	
	virtual ~SocketPacket() 
	{
	}

	virtual void print(std::ostream& os) const 
	{ 
		os << className() << ": " << info->peerAddress << std::endl; 
	}

	virtual IPacket* clone() const 
	{
		return new SocketPacket(*this);
	}	

	virtual bool read(const ConstBuffer&) 
	{ 
		assert(0 && "write only"); 
		return false;
	}

	virtual void write(Buffer& buf) const 
	{	
		buf.insert(buf.end(), data(), data() + size()); 
		//buf.append(data(), size()); 
	}
	
	virtual const char* className() const 
	{ 
		return "SocketPacket"; 
	}
};


} } // namespace scy::net


#endif // SOURCEY_NET_Socket_H



	
	/*
	int priority;
		// A higher priority gives the current observer
		// precedence in the socket callback chain.

	static bool compareProiroty(const SocketAdapter* l, const SocketAdapter* r);
	*/

/*
	//void duplicate();
	//void release();
	//int refCount() const;

// -------------------------------------------------------------------
//
class SocketAdapter
	/// SocketAdapter is the short and sweet socket event handling 
	/// interface which is also directly responsible for incrementing and 
	/// deincrementing the reference count of the underlying SocketBase.
	/// 
	/// This class can also be extended to implement custom processing 
	/// for received socket data before it is dispatched to the application.
	/// See the SocketAdapter, PacketSocket and Transaction classes for ideas.
	///
	/// TODO: SocketBase pointer here
	///
{
public:
	SocketAdapter(int priority = 0);
	
	virtual ~SocketAdapter();

	virtual void onSocketConnect() = 0;
	virtual void onSocketRecv(const MutableBuffer& buf, const Address& peerAddr) = 0;
	virtual void onSocketError(const Error& error) = 0;
	virtual void onSocketClose() = 0;
};


	/// SocketAdapter is an proxy layer which is attached to a
	/// SocketBase instance to handle socket events.
	///
	/// SocketAdapters are directly responsible for incrementing and 
	/// deincrementing the reference count of the managing SocketBase.
	/// 
	/// This class can also be extended to implement custom processing 
	/// for received socket data before it is dispatched to the application.
	/// See the PacketSocketAdapter and Transaction classes for ideas.
	///
	/// TODO: Rename to SocketAdapter, and extend as SocketAdapter with signals
	///
*/

	//virtual Socket& assign(SocketBase* ptr);
	//virtual Socket& assign(const Socket& ptr);


 // { throw std::runtime_error("Not implemented by protocol"); }; // { throw std::runtime_error("Not implemented by protocol"); };

/*
template<class SocketT = SocketBase>
class SocketHandle: public Handle<SocketT>
	/// SocketHandle is a disposable socket wrapper for
	/// SocketBase types which can be created on the stack
	/// for easy reference counted memory management for 
	/// the underlying socket instance.
{
public:		
	typedef SocketT Base;
	typedef std::vector<SocketHandle<SocketT>> List;

	SocketHandle() :
		// NOTE: Only compiles for derived SocketBase implementations
		Handle<SocketT>(new SocketT)
	{
	}

	SocketHandle(SocketBase* ptr) :
		Handle<SocketT>(ptr)
	{
		assertInstance(ptr);
	}

	SocketHandle(const SocketHandle& ptr) : 
		Handle<SocketT>(ptr)
	{
		assertInstance(ptr);
	}

	SocketHandle& operator = (SocketBase* ptr)
	{
		assertInstance(ptr);
		Handle<SocketT>::operator = (socket);
		return *this;
	}
		
	SocketHandle& operator = (const SocketHandle& socket)
	{
		assertInstance(socket.base());
		Handle<SocketT>::operator = (socket);
		return *this;
	}

	~SocketHandle()
	{
	}
	
	void assertInstance(const SocketBase* ptr) 
	{	
		if (!dynamic_cast<const SocketT*>(ptr))
			throw std::runtime_error("Cannot assign incompatible socket");
	}
};
*/



/*
//template<class SocketT = SocketBase>
class SocketHandle: public Handle<SocketBase>
	/// SocketHandle is a disposable socket wrapper for
	/// SocketBase types which can be created on the stack
	/// for easy reference counted memory management for 
	/// the underlying socket instance.
{
public:		
	typedef SocketBase Base;
	typedef std::vector<SocketBase> List;

	SocketHandle(SocketBase* ptr) :
		Handle<SocketBase>(ptr)
	{
		assertInstance(ptr);
	}

	SocketHandle(const SocketHandle& socket) : 
		Handle<SocketBase>(socket)
	{
		assertInstance(socket.base());
	}

	virtual ~SocketHandle()
	{
	}

	SocketHandle& operator = (SocketBase* ptr)
	{
		assertInstance(ptr);
		Handle<SocketBase>::operator = (ptr);
		return *this;
	}
		
	SocketHandle& operator = (const SocketHandle& socket)
	{
		assertInstance(socket.base());
		Handle<SocketBase>::operator = (socket);
		return *this;
	}
	
	virtual void assertInstance(const SocketBase* ptr) 
	{	
		if (!dynamic_cast<const SocketBase*>(ptr))
			throw std::runtime_error("Cannot assign incompatible socket");
	}

protected:
	SocketHandle() 
		//:
		// NOTE: Only compiles for derived SocketBase implementations
		//Handle<SocketT>(new SocketT)
	{
	}
};

	template<class T>
	SocketT* as()
		// Attempt to cast and return the internal socket 
	{
		return dynamic_cast<T*>(get());
	}
*/

/*

	template<class T>
	SocketT* as()
		// Attempt to cast and return the internal socket 
	{
		return dynamic_cast<T*>(get());
	}
*/

/*
	template<class T>
	bool is()
	{
		return as<T>() != nullptr;
	}
	*/


	//Signal2<Buffer&, const net::PacketInfo&> MulticastRecv;
		// Signals data received by the socket.
		// The address argument is for UDP compatibility.
		// For TCP it will always return the peerAddress.

/* //CountedBase
	typedef SharedPtr<SocketBase> SocketHandle;
class SocketHandle: public CountedBase //SharedObject
	/// SocketBase is the abstract base interface from      
	/// which all socket contexts derive.
{
	typedef SharedPtr<SocketBase> SocketHandle;
public:
}

SSLSocket::SSLSocket(const SocketHandle& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<SSLBase*>(get()))
		throw std::runtime_error("Cannot assign incompatible socket");
}
*/
	//virtual void bind6(const Address& address, unsigned flags = 0) { throw std::runtime_error("Not implemented by protocol"); };
