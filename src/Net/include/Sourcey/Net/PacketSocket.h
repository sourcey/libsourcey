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


#ifndef SOURCEY_NET_PacketSocket_H
#define SOURCEY_NET_PacketSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/PacketSignal.h"
#include "Sourcey/PacketFactory.h"
#include "Sourcey/Net/Socket.h"


namespace scy {
namespace net {


struct PacketInfo;
class PacketSocket;

class PacketSocketAdapter: public SocketAdapter, public PacketSignal
{	
public:	
	PacketFactory factory;

	PacketSocketAdapter(Socket* socket = NULL) : //, int priority = 50
		SocketAdapter(socket) //, priority
		/// Creates the PacketSocketAdapter
		/// This class should have a higher priority than standard
		/// sockets so we can parse data packets first.		
	{
		traceL("PacketSocketAdapter", this) << "Creating: " << socket << std::endl;
	}
		
	virtual void onSocketRecv(Buffer& buf, const Address& peerAddr)
		/// Creates and dispatches a packet utilizing the available 
		/// creation strategies. For best performance the most used 
		/// strategies should have the highest priority.
	{	
		traceL("PacketSocketAdapter", this) << "Recv: " << buf.available() << std::endl;
		assert(buf.position() == 0);
		buf.position(0);

		IPacket* pkt = factory.createPacket(buf);
		if (!pkt) {
			warnL("PacketSocketAdapter", this) << "Cannot create packet." << std::endl;	
			return;
		}

		pkt->info = new PacketInfo(*socket, peerAddr);
		onPacket(*pkt);
		delete pkt;
	}

	virtual void onPacket(IPacket& pkt) 
	{		
		traceL("PacketSocketAdapter", this) << "onPacket: Emitting: " << pkt.size() << std::endl;
		PacketSignal::emit(socket, pkt);	
	}
};


class PacketSocket: public Socket
{
public:	
	PacketSocket(const Socket& socket) : 
		Socket(socket) //(this)
	{
		replaceAdapter(new PacketSocketAdapter);
		assert(Socket::base().refCount() >= 2);
	}

	PacketSocket(SocketBase* base, bool shared = false) : 
		Socket(base, shared) //(this), new PacketSocketAdapter
	{		
		replaceAdapter(new PacketSocketAdapter);
		assert(!shared || Socket::base().refCount() >= 2);
	}

	virtual ~PacketSocket() 
	{
	}

	PacketSocketAdapter& adapter() const
		/// Returns the PacketSocketAdapter for this socket.
	{
		return (PacketSocketAdapter&)*_adapter;
	}
		
	
	virtual void send(IPacket& packet)
		/// Compatibility method for PacketSignal delegates.
	{
		traceL("PacketSocket", this) << "IPacket Delegate" << std::endl;	
		Socket::send(packet);
	}
};


} } // namespace scy::Net


#endif // SOURCEY_NET_PacketSocket_H




	/*
	virtual int send(const RawPacket& packet, int flags = 0) 
	{
		/// Most large packets, ie. MediaPackets derive 
		/// from RawPacket, so they can be sent directly 
		/// without buffering any data.
		traceL("PacketSocket", this) << "RawPacket" << packet.size() << std::endl;	
		return base().send((const char*)packet.array(), packet.size(), flags);
	}

	virtual int send(const RawPacket& packet, const Address& peerAddress, int flags = 0)
	{
		traceL("PacketSocket", this) << "Send Data Packet: " << peerAddress << std::endl;	
		assert(peerAddress == base().peerAddress());
		return send(packet, flags);
	}

	virtual int send(const IPacket& packet, int flags = 0) 
	{
		/// N nocopy solution for sending IPackets is not currently
		/// possible since some packets are only generated when
		/// IPacket::write() is called, such as for STUN and RTP packets.
		/// For sending data packets the send(RawPacket&) should be used.
		//
		/// Furthermore, some dynamically generated packets may return
		/// a size of 0, in which case a smaller buffer of 1500 bytes is 
		/// allocated, but if packet data exceeds this size memcpy will be 
		/// called twice; once on Buffer::create(), and once on Buffer::reserve().
		Buffer buf(packet.size() > 0 ? packet.size() : 1500);
		packet.write(buf);
		traceL("PacketSocket", this) << "Send IPacket: " << buf.available() << std::endl;	
		return base().send(buf.begin(), buf.available(), flags);
	}
	
	virtual int send(const IPacket& packet, const Address& peerAddress, int flags = 0)
	{
		return send(packet, flags);
	}
	*/
		/*
		/// Always try to cast packets as RawPacket types
		/// so we can avoid copying data.
		RawPacket* rawPacket = packet.as<RawPacket>();
		if (rawPacket)
			send(*rawPacket);
		else
			send(reinterpret_cast<const IPacket&>(packet));
			*/
	//DefineSocketWrapperFields(PacketSocket, SocketBase)
	//typedef SocketBase Base;
	//typedef std::vector<PacketSocket> List;
		

	//Socket<> socket;
	//Handle<SocketBase> socket;
	//PacketSocket()
	//{
	//}

	//PacketSocket(const Socket& socket) : 
	//	socket(socket) 
	//{
	//}
	
	/*
	virtual const char* className() const { return "PacketSocket"; }
protected:
	*/
	/*


	virtual void onConnect() 
	{
		traceL("trace", this) << "On connect" << std::endl;	
		
		/// Register a RawPacket creation strategy for the 
		/// PacketFactory if no strategies have been explicitly
		/// registered.
		if (types().empty())
			PacketFactory::registerPacketType<RawPacket>(100);

		SocketBase<StreamSocketT, TransportT, SocketBaseT>::onConnect();
	}
	PacketInfo& operator = (const PacketInfo& r) {
		socket = r.socket;
		localAddress = r.localAddress;
		peerAddress = r.peerAddress;
		return *this;
	}
	*/



		
	/*
			
	SocketBase* get() const
		/// Returns the SocketBase for this socket.
	{
		return _base; //reinterpret_cast<SocketBase*>(_base);
	}
	*/

	
	/*
	virtual void attach(const PacketDelegateBase& delegate) 
		/// Override this method to resolve conflict with 
		/// attach() in Poco::Net::SecureStreamSocket.
	{
		PacketSignal::attach(delegate);
	}
	*/

	/*
	virtual void send(IPacket& packet)
	{
		/// Always try to cast packets as RawPacket types
		/// so we can avoid copying data.
		RawPacket* rawPacket = packet.as<RawPacket>();
		if (rawPacket)
			send(*rawPacket);
		else
			send(reinterpret_cast<const IPacket&>(packet));
	}
	*/

	/*
//#include "Sourcey/Net/Types.h"
	Socket(Reactor& reactor = Reactor::getDefault(), bool deleteOnClose = false);
	Socket(const StreamSocketT& socket, Reactor& reactor = Reactor::getDefault(), bool deleteOnClose = false);
	Socket(const Socket& r);
	
	virtual ~Socket();
	
	virtual void connect(const Address& peerAddress);
	virtual void connect(const Address& peerAddress, int timeout);
		/// Connects to the given peer address.
		///
		/// A Poco::Net::ConnectionRefusedException or a
		/// TimeoutException is thrown on failure.
	
	virtual void close();
		/// Closes the underlying socket.
		//
		/// On a side note: When the StreamSocket's reference
		/// count reaches 0 the underlying socket will be
		/// automatically closed.

	virtual int send(const char* data, int size);
	virtual int send(const char* data, int size, const Address& peerAddress);
	virtual int send(const RawPacket& packet);
	virtual int send(const RawPacket& packet, const Address& peerAddress);
	virtual int send(const IPacket& packet);
	virtual int send(const IPacket& packet, const Address& peerAddress);
	virtual void send(IPacket& packet);

	virtual void setError(const std::string& err);
		/// Sets the error message and closes the socket.
	
	virtual bool isConnected();
	virtual bool isError();

	virtual void deleteOnClose(bool flag);
		/// When true the socket instance will automatically delete 
		/// itself after a call to close().

	TransportProtocol transport() const;
	Address address() const;
	Address peerAddress() const;
	std::string error() const;
	int errorno() const;
	Reactor& reactor();

	virtual void bindEvents();	
	virtual void unbindEvents();
	
protected:		
	virtual void recv(Buffer& buffer);
	virtual void packetize(Buffer& buffer);

	virtual void onReadable();
	virtual void onConnect();
	virtual void onClose();
	virtual void onError();
		
	virtual void resetBuffer();
	*/