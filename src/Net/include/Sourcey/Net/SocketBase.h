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
#include "Sourcey/Logger.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/ISocket.h"
#include "Sourcey/Net/Reactor.h"

#include "Poco/Net/NetException.h"
#include "Poco/Mutex.h"


namespace scy {
namespace Net {

	
template <class StreamSocketT, TransportProtocol TransportT, class ISocketT = ISocket>
class SocketBase: public StreamSocketT, public ISocketT, public IPolymorphic
	/// This class implements a reactor based socket.
	/// This class is designed to used with TCP and SSL
	/// socket types. 
{
public:	
	typedef typename ISocketT InterfaceT;


	SocketBase(Reactor& reactor) : 
		_reactor(reactor),
		_connected(false),
		_deleteOnClose(false)
	{
		log() << "Creating" << std::endl;
	}


	SocketBase(const StreamSocketT& socket, Reactor& reactor) : 
		StreamSocketT(socket),
		_reactor(reactor),
		_connected(false),
		_deleteOnClose(false)
	{
		log() << "Creating from " << socket.address().toString() << std::endl;
		bindEvents();
	}


	SocketBase(const SocketBase& r) : 
		StreamSocketT(r),
		_reactor(r._reactor),
		_connected(r._connected),
		_deleteOnClose(r._deleteOnClose)
	{
		log() << "Creating from " << r.address() << std::endl;	
		bindEvents();
	}


	virtual ~SocketBase() 
	{	
		log() << "Destroying" << std::endl;
		unbindEvents();
		log() << "Destroying: OK" << std::endl;
	}


	virtual void connect(const Address& peerAddress) 
	{
		connect(peerAddress, 0);
	}


	virtual void connect(const Address& peerAddress, int timeout) 
	{
		log() << "Connecting to " << peerAddress.toString() << std::endl;	

		if (isConnected())
			throw Exception("Socket already connected");

		try {
			_error = "";
			if (timeout)
				StreamSocketT::connect(peerAddress, Poco::Timespan(timeout, 0));
			else
				StreamSocketT::connect(peerAddress);
				//StreamSocketT::connectNB(peerAddress);
			bindEvents();
			resetBuffer();
		} 
		catch (Poco::Exception& exc) {
			log("error") << "Connection failed: " << exc.displayText() << std::endl;	
			setError(exc.displayText());
			exc.rethrow();
		}
	}


	virtual void close()
	{
		log() << "Closing" << std::endl;

		if (isConnected()) {
			unbindEvents();
			try	{
				// If the socket is already closed an
				// InvalidSocketException will be thrown. 
				// Just swallow it.
				StreamSocketT::close();
			}
			catch (Poco::IOException& exc) {
				log("warn") << "Closing Error: " << exc.displayText() << std::endl;
			}
			onClose(); // May result in destruction
		}
	}


	virtual int send(const char* data, int size, int flags = 0) 
	{
		assert(size <= MAX_TCP_PACKET_SIZE);
		int ret = -1;
		try	{
			log() << "SEND: " << size << ": " 
				<< address().toString() << "-->" 
				<< peerAddress().toString() << std::endl;
			ret = StreamSocketT::sendBytes(data, size, flags);
			log() << "SEND: 1" << std::endl;
		}	
		catch (Poco::IOException& exc) {
			log("error") << "Send Error: " << exc.displayText() << std::endl;
			// Don't set the error here, the error
			// always comes back though the reactor.
			//assert(error().empty());
			exc.rethrow(); // rethrow
		}
		return ret; // error
	}


	virtual int send(const char* data, int size, const Address& peerAddress, int flags = 0)
	{
		assert(peerAddress == this->peerAddress());
		return send(data, size, flags);
	}


	virtual int send(const DataPacket& packet, int flags = 0) 
	{
		// Most large packets, ie. MediaPackets derive 
		// from DataPacket, so they can be sent directly 
		// without buffering any data.
		return send((const char*)packet.data(), packet.size(), flags);
	}


	virtual int send(const DataPacket& packet, const Address& peerAddress, int flags = 0)
	{
		assert(peerAddress == this->peerAddress());
		return send(packet, flags);
	}


	virtual int send(const IPacket& packet, int flags = 0) 
	{
		// N nocopy solution for sending IPackets is not currently
		// possible since some packets are only generated when
		// IPacket::write() is called, such as for STUN and RTP packets.
		// For sending data packets the send(DataPacket&) should be used.
		//
		// Furthermore, some dynamically generated packets may return
		// a size of 0, in which case a smaller buffer of 1500 bytes is 
		// allocated, but if packet data exceeds this size memcpy will be 
		// called twice; once on Buffer::create(), and once on Buffer::reserve().
		Buffer buf(packet.size() > 0 ? packet.size() : 1500);
		packet.write(buf);
		return send(buf.data(), buf.size(), flags);
	}


	virtual int send(const IPacket& packet, const Address& peerAddress, int flags = 0)
	{
		assert(peerAddress == this->peerAddress());
		return send(packet, flags);
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
			return StreamSocketT::impl()->socketError();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return -1; // invalid socket
	}


	virtual Address address() const 
	{
		try	{
			return StreamSocketT::address();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return Address();
	}


	virtual Address peerAddress() const 
	{
		try	{
			return StreamSocketT::peerAddress();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return Address();
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
	

	virtual const char* className() const { return "SocketBase"; }

	
protected:
	virtual void onReadable() 
	{
		log() << "On Readable" << std::endl;

		try	{
			int size = 0;
			Buffer* buffer = NULL;;
			{
				Poco::FastMutex::ScopedLock lock(_mutex); 			

				// Get the readable size.
				// NOTE: This is not available for SecureStreamSocket in
				// which case we use buffer capacity to determine read size.
				size = std::min<int>(StreamSocketT::available(), MAX_TCP_PACKET_SIZE);
				size = size ? size : _buffer.capacity();

				// Resize our buffer as required.
				// This is preferable to allocating a large buffer at startup.
				if (_buffer.capacity() < size) {
					log() << "Resizing buffer: " << size << std::endl;
					_buffer.reserve(size);
				}

				// Read bytes from the socket into the output buffer.
				size = StreamSocketT::receiveBytes(_buffer.bytes(), size);
				log() << "RECV: " << size << ": "
					<< StreamSocketT::address().toString() << "<--" 
					<< StreamSocketT::peerAddress().toString() << std::endl;
				_buffer.setPosition(0);
				_buffer.setSize(size);
				buffer = &_buffer;
			}

			if (size) {
				recv(*buffer);
			}
			else {
				log() << "Received EOF" << std::endl;
				throw Poco::IOException("Peer closed connection");
			}
		}
		// ConnectionAbortedException
		// ConnectionResetException
		catch (Poco::IOException& exc) {
			setError(exc.displayText()); // may result in deletion
		}
	}


	virtual void recv(Buffer& buffer)
		/// Receives data from the socket.
	{
		// May be overridden for custom handling...
		packetize(buffer);
	}
	

	virtual void packetize(Buffer& buffer)
		/// Creates and dispatches a packet from received data.
	{		
		// May be overridden for custom handling...	
		Data.emit(this, buffer, peerAddress());	
	}


	virtual void onConnect() 
	{
		log() << "On Connect" << std::endl;	

		//StreamSocketT::setBlocking(true);
		{
			Poco::FastMutex::ScopedLock lock(_mutex); 

			// The SocketWritable event tells us the socket is
			// connected to the peer, after we receive it just
			// remove the delegate.
			_reactor.detach(*this, reactorDelegate(this, &SocketBase::onConnect, SocketWritable));	
			_connected = true;
		}
		Connected.emit(static_cast<ISocketT*>(this));
	}


	virtual void onClose()
	{
		log() << "On Close" << std::endl;
	
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
			log() << "On Close: Deleting" << std::endl;	
			// TODO: Delayed garbage collection.
			// Leaving as is for now because it will help us locate bugs.
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
		log("error") << "Socket Error: " << err << std::endl;
		{
			Poco::FastMutex::ScopedLock lock(_mutex); 
			_error = err;
		}
		int n = errorno();
		Error.emit(static_cast<ISocketT*>(this), n, _error);
		close();
	}


	virtual void resetBuffer() 
	{	
		Poco::FastMutex::ScopedLock lock(_mutex); 

		// Set the initial buffer size to the getReceiveBufferSize value.
		// If read sizes surpass this value the buffer size will be adjusted
		// accordingly.
		int recvSize = StreamSocketT::getReceiveBufferSize();
		if (recvSize != static_cast<int>(_buffer.capacity())) {
			log() << "Buffer Size: " << recvSize << std::endl;
			_buffer.reserve(recvSize);
		}	
		_buffer.clear();
	}


	virtual void bindEvents()
	{		
		Poco::FastMutex::ScopedLock lock(_mutex); 

		_reactor.attach(*this, reactorDelegate(this, &SocketBase::onReadable, SocketReadable));
		_reactor.attach(*this, reactorDelegate(this, &SocketBase::onConnect, SocketWritable));
		_reactor.attach(*this, reactorDelegate(this, &SocketBase::onError, SocketError));
	}


	virtual void unbindEvents()
	{	
		Poco::FastMutex::ScopedLock lock(_mutex); 

		_reactor.detach(*this, reactorDelegate(this, &SocketBase::onReadable, SocketReadable));
		_reactor.detach(*this, reactorDelegate(this, &SocketBase::onConnect, SocketWritable));
		_reactor.detach(*this, reactorDelegate(this, &SocketBase::onError, SocketError));
	}

	
protected:
	Buffer		_buffer;
	Reactor&	_reactor;
	std::string	_error;
	bool		_connected;
	bool		_deleteOnClose;

private:
	mutable Poco::FastMutex _mutex;
};


} } // namespace scy::Net


#endif // SOURCEY_NET_SocketBase_H




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