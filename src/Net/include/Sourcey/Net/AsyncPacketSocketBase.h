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


#ifndef SOURCEY_NET_AsyncPacketSocketBase_H
#define SOURCEY_NET_AsyncPacketSocketBase_H


#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/PacketQueue.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/IPacketSocket.h"
#include "Sourcey/Net/Reactor.h"

#include "Poco/Net/NetException.h"
#include "Poco/Mutex.h"


namespace Scy {
namespace Net {

	
template <class DatagramSocketT, TransportProtocol TransportT, class ISocketT = IPacketSocket>
class AsyncPacketSocketBase: public DatagramSocketT, public ISocketT, public IPolymorphic
	/// This class implements a reactor based packet socket.
	/// This class is designed to used with UDP socket types. 
{
public:	
	typedef typename ISocketT InterfaceT;


	AsyncPacketSocketBase(Reactor& reactor, Runner& runner) : 
		_reactor(reactor),
		_runner(runner),
		_bound(false),
		_connected(false),
		_deleteOnClose(false),
		_buffer(MAX_UDP_PACKET_SIZE),
		_queue(new PacketQueue(*this, _runner))
	{
		Log("trace", this) << "Creating" << std::endl;
	}


	AsyncPacketSocketBase(const DatagramSocketT& socket, Reactor& reactor, Runner& runner) : 
		DatagramSocketT(socket),
		_reactor(reactor),
		_runner(runner),
		_bound(false),
		_connected(false),
		_deleteOnClose(false),
		_buffer(MAX_UDP_PACKET_SIZE),
		_queue(new PacketQueue(*this, _runner))
	{
		Log("trace", this) << "Creating from " << socket.address().toString() << std::endl;
		bindEvents();
	}


	AsyncPacketSocketBase(const AsyncPacketSocketBase& r) : 
		DatagramSocketT(r),
		_reactor(r._reactor),
		_runner(r._runner),
		_bound(r._bound),
		_connected(r._connected),
		_deleteOnClose(r._deleteOnClose),
		_queue(new PacketQueue(*this, _runner))
	{
		Log("trace", this) << "Creating from " << r.address() << std::endl;	
		bindEvents();
	}


	virtual ~AsyncPacketSocketBase() 
	{	
		Log("trace", this) << "Destroying" << std::endl;
		_queue->destroy();
		unbindEvents();
		Log("trace", this) << "Destroying: OK" << std::endl;
	}
	

	virtual void bind(const Address& address, bool reuseAddress = false)
	{	
		Log("trace", this) << "Binding on " << address << endl;

		if (isBound())
			throw Exception("Socket already bound.");

		try {		
			DatagramSocket::bind(address, reuseAddress);
			bindEvents();	
			onBind();
		} 
		catch (Poco::Exception& exc) {
			Log("error", this) << "Bind failed: " << exc.displayText() << std::endl;	
			setError(exc.displayText());
			exc.rethrow();
		}	
	}
	

	virtual void connect(const Address& peerAddress) 
	{
		Log("trace", this) << "Connecting to " << peerAddress << std::endl;	

		if (isConnected())
			throw Exception("Socket already connected.");

		try {
			DatagramSocketT::connect(peerAddress);
			bindEvents();
			onConnect();
		} 
		catch (Poco::Exception& exc) {
			Log("error", this) << "Connection failed: " << exc.displayText() << std::endl;	
			setError(exc.displayText());
			exc.rethrow();
		}
	}


	virtual void close()
	{
		Log("trace", this) << "Closing" << std::endl;

		if (isConnected()) {
			unbindEvents();
			try	{
				// If the socket is already closed an
				// InvalidSocketException will be thrown. 
				// Just swallow it.
				DatagramSocketT::close();
			}
			catch (Poco::IOException& exc) {
				Log("warn", this) << "Closing Error: " << exc.displayText() << std::endl;
			}
			onClose(); // will probably result in destruction
		}
	}


	virtual int send(const char* data, int size) 
	{
		// NOTE: Must be connected to peer to send
		// without specifying peer address.
		assert(isBound());
		return send(data, size, peerAddress());
	}


	virtual int send(const char* data, int size, const Address& peerAddress)
	{
		assert(size <= MAX_UDP_PACKET_SIZE);
		try	{
			Log("trace", this) << "SEND: " << size << ": " 
				<< address() << "-->" 
				<< peerAddress << std::endl;
			return DatagramSocketT::sendBytes(data, size);
		}	
		catch (Poco::IOException& exc) {
			// Don't set the error here, the error
			// always comes back though the reactor.
			Log("error", this) << "Send Error: " << exc.displayText() << std::endl;
		}
		return -1; // error
	}


	virtual int send(const DataPacket& packet) 
	{
		// Most large packets, ie. MediaPackets derive 
		// from DataPacket, so they can be sent directly 
		// without buffering any data.
		return send((const char*)packet.data(), packet.size());
	}


	virtual int send(const DataPacket& packet, const Address& peerAddress)
	{
		assert(peerAddress == this->peerAddress());
		return send(packet);
	}


	virtual int send(const IPacket& packet) 
	{
		Buffer buf(packet.size() > 0 ? packet.size() : MAX_UDP_PACKET_SIZE);
		packet.write(buf);
		return send(buf.data(), buf.size());
	}


	virtual int send(const IPacket& packet, const Address& peerAddress)
	{
		// Unlike TCP sockets our packets are generally
		// limited by a 1500 MTU so let's just copy the 
		// packet data.
		Buffer buf(MAX_UDP_PACKET_SIZE);
		packet.write(buf);	
		return send(buf.data(), buf.size(), peerAddress);
	}


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


	virtual void setDeleteOnClose(bool flag)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_deleteOnClose = flag;
	}


	virtual Reactor& reactor()
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 
		return _reactor;
	}


	virtual std::string error() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 
		return _error;
	}


	virtual int errorno() const 
	{
		try	{
			return DatagramSocketT::impl()->socketError();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return -1; // invalid socket
	}


	virtual Address address() const 
	{
		try	{
			return DatagramSocketT::address();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return Address();
	}


	virtual Address peerAddress() const 
	{
		try	{
			return DatagramSocketT::peerAddress();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return Address();
	}


	virtual bool isBound() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 
		return _bound;
	}


	virtual bool isConnected() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 
		return _connected;
	}


	virtual bool isError() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return !_error.empty();
	}


	virtual TransportProtocol transport() const 
	{ 
		return TransportT; 
	}
	

	virtual const char* className() const { return "AsyncPacketSocketBase"; }

	
protected:
	virtual void onReadable() 
	{
		Log("trace", this) << "On Readable" << std::endl;

		try	{
			int size = 0;
			Buffer* buffer = NULL;
			Address peerAddress;
			{
				Poco::FastMutex::ScopedLock lock(_mutex); 			

				// Get the readable size.
				size = min(DatagramSocketT::available(), MAX_UDP_PACKET_SIZE);

				// NOTE: This is not available for SecureStreamSocket in
				// which case we use buffer capacity to determine read size.
				//size = size ? size : _buffer.capacity();

				// Resize our buffer as required.
				// This is preferable to allocating a large buffer at startup.
				assert(_buffer.capacity() >= size);
				//if (_buffer.capacity() < size) {
				//	Log("trace", this) << "Resizing buffer: " << size << std::endl;
				//	_buffer.reserve(size);
				//}

				// Read bytes from the socket into the output buffer.
				size = DatagramSocketT::receiveFrom(_buffer.bytes(), size, peerAddress);
				Log("trace", this) << "RECV: " << size << ": "
					<< DatagramSocketT::address().toString() << "<--" 
					<< peerAddress << std::endl;
				_buffer.setPosition(0);
				_buffer.setSize(size);
				buffer = &_buffer;
			}

			if (size) {
				recv(*buffer, peerAddress);
			}
			else {
				Log("trace", this) << "Received EOF" << std::endl;
				throw Poco::IOException("Peer closed connection");
			}
		}
		// ConnectionAbortedException
		// ConnectionResetException
		catch (Poco::IOException& exc) {
			setError(exc.displayText()); // will probably result in deletion
		}
	}


	virtual void recv(Buffer& buffer, const Address& peerAddress)
		/// Receives data from the socket.
	{
		// May be overridden for custom handling...
		packetize(buffer, peerAddress);
	}
	

	virtual void packetize(Buffer& buffer, const Address& peerAddress)
		/// Creates and dispatches a packet from received data.
	{	
		IPacket* packet = PacketFactory::createPacket(buffer);
		if (!packet) {
			Log("warn", this) << "Unable to create data packet." << endl;	
			return;
		}	

		packet->info = new PacketInfo(*this, peerAddress);

		// Push the packet onto the queue
		_queue->push(packet);
		
		//PacketEmitter::emit(this, *packet);
		//Data.emit(this, buffer, peerAddress());	
	}


	virtual void onBind() 
	{
		Log("trace", this) << "On Bind" << std::endl;	
		Poco::FastMutex::ScopedLock lock(_mutex); 
		_error = "";
		_bound = true;
		_queue->start();
	}


	virtual void onConnect() 
	{
		Log("trace", this) << "On Connect" << std::endl;	
		{
			Poco::FastMutex::ScopedLock lock(_mutex); 
			_error = "";
			_connected = true;
			_queue->start();
		}
		Connected.emit(static_cast<ISocketT*>(this));
	}


	virtual void onClose()
	{
		Log("trace", this) << "On Close" << std::endl;
	
		// This method must be called from close()

		bool destroy = false;
		{
			Poco::FastMutex::ScopedLock lock(_mutex); 
			assert(_connected);
			if (!_connected)
				return;
			_connected = false;
			destroy = _deleteOnClose;
		}

		Closed.emit(static_cast<ISocketT*>(this));

		if (destroy) {
			Log("trace", this) << "Delete on close" << std::endl;	
			delete this;
		}
	}


	virtual void onError() 
	{
		Log("warn", this) << "On Error" << std::endl;

		// We received an error notification from the reactor.
		// Close the socket.
		close();
	}


	virtual void setError(const std::string& err)
	{
		Log("error", this) << "Error: " << err << std::endl;
		{
			Poco::FastMutex::ScopedLock lock(_mutex); 
			_error = err;
		}
		int n = errorno();
		Error.emit(static_cast<ISocketT*>(this), n, _error);
		close();
	}


	virtual void bindEvents()
	{		
		Poco::FastMutex::ScopedLock lock(_mutex); 

		_reactor.attach(*this, reactorDelegate(this, &AsyncPacketSocketBase::onReadable, SocketReadable));
		_reactor.attach(*this, reactorDelegate(this, &AsyncPacketSocketBase::onError, SocketError));
	}


	virtual void unbindEvents()
	{	
		Poco::FastMutex::ScopedLock lock(_mutex); 

		_reactor.detach(*this, reactorDelegate(this, &AsyncPacketSocketBase::onReadable, SocketReadable));
		_reactor.detach(*this, reactorDelegate(this, &AsyncPacketSocketBase::onError, SocketError));
	}

	
protected:
	mutable Poco::FastMutex _mutex;
	Buffer			_buffer;
	Reactor&		_reactor;
	Runner&			_runner;
	PacketQueue*	_queue;
	std::string		_error;
	bool			_bound;
	bool			_connected;
	bool			_deleteOnClose;
};


} } // namespace Scy::Net


#endif // SOURCEY_NET_AsyncPacketSocketBase_H






	/*
		//

	virtual void packetize(Buffer& buffer)
		// Create the outgoing packet from the available creation
		// strategies. For best performance the most used strategies
		// should have the highest priority.
	{	
		IPacket* packet = createPacket(buffer);
		if (!packet) {
			Log("warn", this) << "Unable to create data packet." << std::endl;	
			return;
		}

		packet->info = new PacketInfo(*this, peerAddress());
		emit(static_cast<ISocketT*>(this), *packet);	
		delete packet;
	}
	*/

	/*
	virtual void resetBuffer() 
	{	
		Poco::FastMutex::ScopedLock lock(_mutex); 

		// Set the initial buffer size to the getReceiveBufferSize value.
		// If read sizes surpass this value the buffer size will be adjusted
		// accordingly.
		int recvSize = DatagramSocketT::getReceiveBufferSize();
		if (recvSize != static_cast<int>(_buffer.capacity())) {
			Log("trace", this) << "Buffer Size: " << recvSize << std::endl;
			_buffer.reserve(recvSize);
		}	
		_buffer.clear();
	}
	*/

	/*
	Socket(Reactor& reactor = Reactor::getDefault(), bool deleteOnClose = false);
	Socket(const DatagramSocketT& socket, Reactor& reactor = Reactor::getDefault(), bool deleteOnClose = false);
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