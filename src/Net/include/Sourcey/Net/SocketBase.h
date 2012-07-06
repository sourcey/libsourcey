//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
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



#include "Poco/Net/HTTPRequest.h"



namespace Sourcey {
namespace Net {

	
template <class SocketImplT, TransportProtocol TransportT, class ISocketT = ISocket>
class SocketBase: public SocketImplT, public ISocketT, public ILoggable
	/// This class implements a reactor based socket.
{
public:	
	//typedef TransportProtocol TransportT   Transport;
	typedef typename ISocketT     Interface;

	SocketBase(Reactor& reactor) : //Poco::Net::Socket& socket, 
		//_socket(socket),
		_reactor(reactor),
		_connected(false),
		_deleteOnClose(false)
	{
		Log("trace", this) << "Creating" << std::endl;
	}


	SocketBase(const SocketImplT& socket, Reactor& reactor) : //SocketImplT& socket, 
		SocketImplT(socket),
		//_socket(socket),
		_reactor(reactor),
		_connected(false),
		_deleteOnClose(false)
	{
		Log("trace", this) << "Creating from " << socket.address().toString() << std::endl;
		bindEvents();
	}


	SocketBase(const SocketBase& r) : 
		SocketImplT(r),
		_reactor(r._reactor),
		_connected(r._connected),
		_deleteOnClose(r._deleteOnClose)
	{
		Log("trace", this) << "Creating from " << r.address() << std::endl;	
		bindEvents();
	}


	virtual ~SocketBase() 
	{	
		Log("trace", this) << "Destroying" << std::endl;
		unbindEvents();
		Log("trace", this) << "Destroying: OK" << std::endl;
	}


	virtual void connect(const Address& peerAddress) 
	{
		connect(peerAddress, 0);
	}


	virtual void connect(const Address& peerAddress, int timeout) 
	{
		Log("trace", this) << "Connecting to " << peerAddress.toString() << std::endl;	

		if (isConnected())
			throw Exception("Socket already connected");

		try {
			_error = "";
			if (timeout)
				SocketImplT::connect(peerAddress, Poco::Timespan(timeout, 0));
			else
				SocketImplT::connect(peerAddress);
				//SocketImplT::connectNB(peerAddress);
			bindEvents();
			resetBuffer();

			// Register a DataPacket creator types if no
			// packet types have been registered yet.
			if (types().empty())
				registerPacketType<DataPacket>(1);
		} 
		catch (Poco::Exception& exc) {
			Log("trace", this) << "Connection failed: " << exc.displayText() << std::endl;	
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
				SocketImplT::close();
			}
			catch (Poco::IOException& exc) {
				Log("warn", this) << "Closing Error: " << exc.displayText() << std::endl;
			}
			onClose(); // will probably result in destruction
		}
	}


	virtual int send(const char* data, int size) 
	{
		assert(size <= MAX_TCP_PACKET_SIZE);
		try	{
			Log("trace", this) << "SEND: " << size << ": " 
				<< address().toString() << "-->" 
				<< peerAddress().toString() << std::endl;
			return SocketImplT::sendBytes(data, size);
		}	
		catch (Poco::IOException& exc) {
			// Don't set the error here, the error
			// always comes back though the reactor.
			Log("error", this) << "SEND: " << exc.displayText() << std::endl;
		}
		return -1; // error
	}


	virtual int send(const char* data, int size, const Address& peerAddress)
	{
		assert(peerAddress == this->peerAddress());
		return send(data, size);
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
		// Currently a nocopy solution for sending IPackets is
		// impossible unless a generic data member exists such
		// as for DataPacket. 
		// Furthermore, dynamically generated packets may opt
		// to return a size of 0, in which case a smaller buffer
		// of 1500 bytes is allocated, but if packet data exceeds
		// this size memcpy will be called twice; once on create,
		// and once on reserve.
		Buffer buf(packet.size() > 0 ? packet.size() : 1500);
		packet.write(buf);
		return send(buf.data(), buf.size());
	}


	virtual int send(const IPacket& packet, const Address& peerAddress)
	{
		assert(peerAddress == this->peerAddress());
		return send(packet);
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

	
	virtual void attach(const PacketDelegateBase& delegate) 
	{
		PacketDispatcher::attach(delegate);
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
			return SocketImplT::impl()->socketError();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return -1; // invalid socket
	}


	virtual Address address() const 
	{
		try	{
			return SocketImplT::address();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return Address();
	}


	virtual Address peerAddress() const 
	{
		try	{
			return SocketImplT::peerAddress();
		}
		catch (Poco::Net::InvalidSocketException&) {
			// swallow it
		}
		return Address();
	}


	virtual bool isConnected()
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 
		Log("trace") << "[StatefulBase:" << this << "] isConnected: " << _connected << std::endl;
		return _connected;
	}


	virtual bool isError()
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
		Log("trace", this) << "On Readable" << std::endl;

		try	{
			int size = 0;
			Buffer* buffer = NULL;;
			{
				Poco::FastMutex::ScopedLock lock(_mutex); 			

				// Get the readable size.
				// NOTE: This is not available for SecureStreamSocket in
				// which case we use buffer capacity to determine read size.
				size = min(SocketImplT::available(), MAX_TCP_PACKET_SIZE);
				size = size ? size : _buffer.capacity();

				// Resize our buffer as required.
				// This is preferable to allocating a large buffer at startup.
				if (_buffer.capacity() < size) {
					Log("trace", this) << "Resizing buffer: " << size << std::endl;
					_buffer.reserve(size);
				}

				// Read bytes from the socket into the output buffer.
				size = SocketImplT::receiveBytes(_buffer.bytes(), size);
				Log("trace", this) << "RECV: " << size << ": "
					<< SocketImplT::address().toString() << "<--" 
					<< SocketImplT::peerAddress().toString() << std::endl;
				_buffer.setPosition(0);
				_buffer.setSize(size);
				buffer = &_buffer;
			}

			if (size) {
				recv(*buffer);
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


	virtual void recv(Buffer& buffer)
	{
		// May be overridden for custom handling...
		packetize(buffer);
	}


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
		dispatch(static_cast<ISocketT*>(this), *packet);	
		delete packet;
	}


	virtual void onConnect() 
	{
		Log("trace", this) << "On Connect" << std::endl;	

		//SocketImplT::setBlocking(true);
		{
			Poco::FastMutex::ScopedLock lock(_mutex); 

			// The SocketWritable event tells us the socket is
			// connected to the peer, after we receive it just
			// remove the delegate.
			_reactor.detach(*this, reactorDelegate(this, &SocketBase::onConnect, SocketWritable));	
			_connected = true;
		}
		//Connected.dispatch(this);
		Connected.dispatch(static_cast<ISocketT*>(this));
	}


	virtual void onClose()
	{
		Log("trace") << "[StatefulBase:" << this << "] On Close" << std::endl;
		//Log("trace", this) << "On Close" << std::endl;
	
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

		Closed.dispatch(static_cast<ISocketT*>(this));

		if (destroy) {
			Log("trace", this) << "Delete on close" << std::endl;	
			delete this;
		}

		//Log("trace", this) << "On Close: OK" << std::endl;
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
		_error = err;
		int n = errorno();
		Error.dispatch(static_cast<ISocketT*>(this), n, _error);
		close();
	}


	virtual void resetBuffer() 
	{	
		Poco::FastMutex::ScopedLock lock(_mutex); 

		// Set the initial buffer size to the getReceiveBufferSize value.
		// If read sizes surpass this value the buffer size will be adjusted
		// accordingly.
		int recvSize = SocketImplT::getReceiveBufferSize();
		if (recvSize != static_cast<int>(_buffer.capacity())) {
			Log("trace", this) << "Buffer Size: " << recvSize << std::endl;
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
	mutable Poco::FastMutex _mutex;
	//Poco::Net::Socket&		_socket;
	Buffer					_buffer;
	Reactor&				_reactor;
	std::string				_error;
	bool					_connected;
	bool					_deleteOnClose;
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_SocketBase_H




	/*
	Socket(Reactor& reactor = Reactor::getDefault(), bool deleteOnClose = false);
	Socket(const SocketImplT& socket, Reactor& reactor = Reactor::getDefault(), bool deleteOnClose = false);
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