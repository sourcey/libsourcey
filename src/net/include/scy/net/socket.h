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


#ifndef SCY_Net_Socket_H
#define SCY_Net_Socket_H


#include "scy/base.h"
#include "scy/memory.h"
#include "scy/packetstream.h"
#include "scy/net/types.h"
#include "scy/net/address.h"
#include "scy/net/network.h"
#include "scy/net/socketadapter.h"


namespace scy {
namespace net {


template<class SocketT>
inline std::shared_ptr<SocketT> makeSocket(uv::Loop* loop = uv::defaultLoop())
	// Helper method for instantiating Sockets wrapped in a std::shared_ptr
	// which will be garbage collected on destruction.
	// It is always recommended to use deferred deletion for Sockets.
{
	return std::shared_ptr<SocketT>(
		new SocketT(loop), deleter::Deferred<SocketT>());
}


class Socket: public SocketAdapter
	/// Socket is the base socket implementation
	/// from which all sockets derive.
{
public:
	typedef std::shared_ptr<Socket> Ptr;
	typedef std::vector<Ptr> Vec;

	Socket();
	virtual ~Socket();
	
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

	virtual void bind(const Address& address, unsigned flags = 0) = 0;
		// Bind a local address to the socket.
		// The address may be IPv4 or IPv6 (if supported).
		//
		// Throws an Exception on error.

	virtual void listen(int backlog = 64) { (void)backlog; };
		// Listens the socket on the given address.
		//
		// Throws an Exception on error.

	virtual bool shutdown() { assert("not implemented by protocol"); return false; };
		// Sends the shutdown packet which should result is socket 
		// closure via callback.

	virtual void close() = 0;
		// Closes the underlying socket.
	
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

	virtual bool closed() const = 0;
		// Returns true if the native socket handle is closed.

	virtual uv::Loop* loop() const = 0;
		// Returns the socket event loop.

protected:
	virtual void init() = 0;
		// Initializes the underlying socket context.

	virtual void reset() {};
		// Resets the socket context for reuse.

	virtual void* self() { return this; };
		// Returns the derived instance pointer for casting SocketAdapter
		// signal callback sender arguments from void* to Socket.
		// Note: This method must not be derived by subclasses or casting
		// will fail for void* pointer callbacks.
};


//
// Packet Info
//


struct PacketInfo: public IPacketInfo
	/// Provides information about packets emitted from a socket.
	/// See SocketPacket.
{ 
	Socket::Ptr socket;
		// The source socket

	Address peerAddress;	
		// The originating peer address.
		// For TCP this will always be connected address.

	PacketInfo(const Socket::Ptr& socket, const Address& peerAddress) :
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

	SocketPacket(const Socket::Ptr& socket, const MutableBuffer& buffer, const Address& peerAddress) : 
		RawPacket(bufferCast<char*>(buffer), buffer.size(), 0, socket.get(), nullptr, 
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

	virtual std::size_t read(const ConstBuffer&) 
	{ 
		assert(0 && "write only"); 
		return 0;
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


//
// Socket Helpers
//

	
#if WIN32
#define nativeSocketFd(handle) ((handle)->socket)
#else
namespace { #include "unix/internal.h" } // uv__stream_fd
#define nativeSocketFd(handle) (uv__stream_fd(handle))
#endif


template<class NativeT> int getServerSocketSendBufSize(uv::Handle& handle)
{
	int fd = nativeSocketFd(handle.ptr<NativeT>());
	int optval = 0; 
	socklen_t optlen = sizeof(int); 
	int err = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&optval, &optlen);
	if (err < 1) {
		errorL("Socket") << "Cannot get snd sock size on fd " << fd << std::endl;
	}
	return optval;
}


template<class NativeT> int getServerSocketRecvBufSize(uv::Handle& handle)
{
	int fd = nativeSocketFd(handle.ptr<NativeT>());
	int optval = 0; 
	socklen_t optlen = sizeof(int); 
	int err = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&optval, &optlen);
	if (err < 1) {
		errorL("Socket") << "Cannot get rcv sock size on fd " << fd << std::endl;
	}
	return optval;
}


template<class NativeT> int setServerSocketBufSize(uv::Handle& handle, int size)
{
	int fd = nativeSocketFd(handle.ptr<NativeT>());
	int sz;

	sz = size;
	while (sz > 0) {
		if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&sz, (socklen_t)sizeof(sz)) < 0) {
			sz = sz / 2;
		} else break;
	}

	if (sz < 1) {
		errorL("Socket") << "Cannot set rcv sock size " << size << " on fd " << fd << std::endl;
	}

	// Get the value to ensure it has propagated through the OS
	traceL("Socket") << "Recv sock size " << getServerSocketRecvBufSize<NativeT>(handle) << " on fd " << fd << std::endl;

	return sz;
}


} } // namespace scy::net


#endif // SCY_Net_Socket_H