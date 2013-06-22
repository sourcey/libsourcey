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


#ifndef SOURCEY_NET_PacketSocketBase_H
#define SOURCEY_NET_PacketSocketBase_H


#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/IPacketSocket.h"
#include "Sourcey/Net/SocketBase.h"


namespace scy {
namespace Net {

	
template <class StreamSocketT, TransportProtocol TransportT, class ISocketT = IPacketSocket>
class PacketSocketBase: public SocketBase<StreamSocketT, TransportT, ISocketT>
	/// This class implements a reactor based packet socket.
{
public:	
	PacketSocketBase(Reactor& reactor) : 
		SocketBase<StreamSocketT, TransportT, ISocketT>(reactor)
	{
	}


	PacketSocketBase(const StreamSocketT& socket, Reactor& reactor) : 
		SocketBase<StreamSocketT, TransportT, ISocketT>(socket, reactor)
	{
	}

	PacketSocketBase(const /*Packet*/SocketBase& rhs) : 
		SocketBase<StreamSocketT, TransportT, ISocketT>(rhs)
	{
	}

	virtual ~PacketSocketBase() 
	{	
	}		
	
	virtual void attach(const PacketDelegateBase& delegate) 
		/// Override this method to resolve conflict with 
		/// attach() in Poco::Net::SecureStreamSocket.
	{
		PacketEmitter::attach(delegate);
	}

	virtual const char* className() const { return "PacketSocketBase"; }
		
protected:
	virtual void packetize(Buffer& buffer)
		// Creates and dispatches a packet utilizing the available 
		// creation strategies. For best performance the most used 
		// strategies should have the highest priority.
	{	
		IPacket* packet = PacketFactory::createPacket(buffer);
		if (!packet) {
			Log("warn", this) << "Unable to create data packet." << std::endl;	
			return;
		}

		packet->info = new PacketInfo(*this, peerAddress());
		PacketEmitter::emit(static_cast<ISocketT*>(this), *packet);	
		delete packet;
	}


	virtual void onConnect() 
	{
		Log("trace", this) << "On Connect" << std::endl;	
		
		// Register a DataPacket creation strategy for the 
		// PacketFactory if no strategies have been explicitly
		// registered.
		if (types().empty())
			PacketFactory::registerPacketType<DataPacket>(100);

		SocketBase<StreamSocketT, TransportT, ISocketT>::onConnect();
	}
};


} } // namespace scy::Net


#endif // SOURCEY_NET_PacketSocketBase_H




	/*
	virtual void send(IPacket& packet)
	{
		// Always try to cast packets as DataPacket types
		// so we can avoid copying data.
		DataPacket* dataPacket = packet.as<DataPacket>();
		if (dataPacket)
			send(*dataPacket);
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
		/// Poco::TimeoutException is thrown on failure.
	
	virtual void close();
		/// Closes the underlying socket.
		//
		/// On a side note: When the StreamSocket's reference
		/// count reaches 0 the underlying socket will be
		/// automatically closed.

	virtual int send(const char* data, int size);
	virtual int send(const char* data, int size, const Address& peerAddress);
	virtual int send(const DataPacket& packet);
	virtual int send(const DataPacket& packet, const Address& peerAddress);
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