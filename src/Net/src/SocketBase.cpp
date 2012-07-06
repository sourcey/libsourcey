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


/*
#include "Sourcey/Net/SocketBase.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Logger.h"
#include "Poco/Net/NetException.h"
#include <iostream>


using namespace std;
using Poco::Net::StreamSocket;
using Poco::Timespan;
using Poco::FastMutex;
using Poco::ScopedLock;
*/


namespace Sourcey {
namespace Net {


//SocketImpl::SocketImpl(Reactor& reactor) : 
//	_reactor(reactor),
//	_connected(false),
//	//_reading(false),
//	_deleteOnClose(false)
//{
//	Log("trace") << "[SocketImpl:" << this << "] Creating" << endl;	
//}
//
//
//SocketImpl::SocketImpl(const Poco::Net::StreamSocket& socket, Reactor& reactor) :
//	Poco::Net::StreamSocket(socket),
//	_reactor(reactor),
//	_connected(false),
//	_deleteOnClose(false)
//{
//	Log("trace") << "[SocketImpl:" << this << "] Creating from " << socket.address().toString() << endl;
//	bindEvents();
//}
//
//
//SocketImpl::SocketImpl(const SocketImpl& r) : 
//	StreamSocket(r),
//	_reactor(r._reactor),
//	_connected(r._connected),
//	_deleteOnClose(r._deleteOnClose)
//{
//	Log("trace") << "[SocketImpl:" << this << "] Creating from " << r.address() << endl;
//	bindEvents();
//}
//
//
//SocketImpl::~SocketImpl() 
//{	
//	Log("trace") << "[SocketImpl:" << this << "] Destroying: " << impl() << endl;
//	unbindEvents();
//	Log("trace") << "[SocketImpl:" << this << "] Destroying: OK: " << impl() << endl;
//}
//
//
//void SocketImpl::connect(const Address& peerAddress) 
//{
//	connect(peerAddress, 0);
//}
//
//
//void SocketImpl::connect(const Address& peerAddress, int timeout) 
//{
//	Log("trace") << "[SocketImpl:" << this << "] Connecting to " << peerAddress.toString() << endl;	
//
//	if (isConnected())
//		throw Exception("Socket already connected");
//
//	try {
//		_error = "";
//		if (timeout)
//			impl()->connect(peerAddress, Timespan(timeout, 0));
//		else
//			impl()->connect(peerAddress);
//		bindEvents();
//		resetBuffer();
//	} 
//	catch (Poco::Exception& exc) {
//		Log("error") << "[SocketImpl:" << this << "] Connection failed: " << exc.displayText() << endl;	
//		setError(exc.displayText());
//		exc.rethrow();
//	}
//}
//
//
//void SocketImpl::close()
//{
//	Log("trace") << "[SocketImpl:" << this << "] Closing" << endl;	
//
//	if (isConnected()) {
//		unbindEvents();
//		try	{
//			// If the socket is already closed for whatever
//			// reason an /*InvalidSocketException*/NetException will be thrown. 
//			// Just swallow it.
//			impl()->close();
//			onClose();
//		}
//		catch (Poco::IOException& exc) {
//			Log("warn") << "[SocketImpl:" << this << "] Closing Error: " << exc.displayText() << endl;
//		}
//	}
//	Log("trace") << "[SocketImpl:" << this << "] Closing: OK" << endl;	
//}
//
//
//void SocketImpl::onReadable() 
//{
//	Log("trace") << "[SocketImpl:" << this << "] On Readable: " << impl() << endl;
//
//	try	{
//		int size = 0;
//		Buffer* buffer = NULL;;
//		{
//			FastMutex::ScopedLock lock(_mutex); 			
//
//			// Resize our buffer as required.
//			// This is preferable to allocating a huge buffer on startup.
//			size = min(impl()->available(), MAX_TCP_PACKET_SIZE);
//			if (_buffer.capacity() < size) {
//				Log("trace") << "[SocketImpl:" << this << "] Resizing buffer: " << size << endl;
//				_buffer.reserve(size);
//			}
//			size = impl()->receiveBytes(_buffer.bytes(), size);
//			Log("trace") << "[SocketImpl:" << this << "] RECV: " << size << ": "
//				<< impl()->address().toString() << "<--" 
//				<< impl()->peerAddress().toString() << endl;
//			_buffer.setPosition(0);
//			_buffer.setSize(size);
//			buffer = &_buffer;
//		}
//
//		if (size) {
//			recv(*buffer);
//		}
//		else {
//			Log("trace") << "[SocketImpl:" << this << "] Received EOF" << endl;
//			throw Poco::IOException("Peer closed connection");
//		}
//	}
//	// ConnectionAbortedException
//	// ConnectionResetException
//	catch (Poco::IOException& exc) {
//		Log("error") << "[SocketImpl:" << this << "] RECV: " << exc.displayText() << endl;
//		setError(exc.displayText()); // will probably result in deletion
//	}
//}
//
//
//int SocketImpl::send(const char* data, int size) 
//{
//	assert(size <= MAX_TCP_PACKET_SIZE);
//	try	{
//		Log("trace") << "[SocketImpl:" << this << "] SEND: " << size << ": " 
//			<< address().toString() << "-->" 
//			<< peerAddress().toString() << endl;
//		return impl()->sendBytes(data, size);
//	}	
//	catch (Poco::IOException& exc) {
//		// Don't set the error here, the error
//		// will always come though the reactor.
//		Log("error") << "[SocketImpl:" << this << "] SEND: " << exc.displayText() << endl;
//	}
//	return -1; // error
//}
//
//
//int SocketImpl::send(const char* data, int size, const Address& peerAddress)
//{
//	assert(peerAddress == this->peerAddress());
//	return send(data, size);
//}
//
//
//int SocketImpl::send(const DataPacket& packet) 
//{
//	// Most large packets, ie. MediaPackets derive from
//	// DataPacket, so we can avoid memcpy altogether.
//	return send((const char*)packet.data(), packet.size());
//}
//
//
//int SocketImpl::send(const DataPacket& packet, const Address& peerAddress)
//{
//	assert(peerAddress == this->peerAddress());
//	return send(packet);
//}
//
//
//int SocketImpl::send(const IPacket& packet) 
//{
//	// Currently a nocopy solution for sending IPackets is
//	// impossible unless a generic data member exists such
//	// as for DataPacket. 
//	// Furthermore, dynamically generated packets may opt
//	// to return a size of 0, in which case a smaller buffer
//	// of 1500 bytes is allocated, but if packet data exceeds
//	// this size memcpy will be called twice; once on create,
//	// and once on reserve.
//	Buffer buf(packet.size() > 0 ? packet.size() : 1500);
//	packet.write(buf);
//	return send(buf.data(), buf.size());
//}
//
//
//int SocketImpl::send(const IPacket& packet, const Address& peerAddress)
//{
//	assert(peerAddress == this->peerAddress());
//	return send(packet);
//}
//
//
//void SocketImpl::send(IPacket& packet)
//{
//	// Always try to cast packets as DataPacket types so
//	// we can avoid copying data.
//	DataPacket* dataPacket = packet.as<DataPacket>();
//	if (dataPacket)
//		send(*dataPacket);
//	else
//		send(reinterpret_cast<const IPacket&>(packet));
//}
//
//
//void SocketImpl::recv(Buffer& buffer)
//{
//	// May be overridden for custom handling...
//	packetize(buffer);
//}
//
//
//void SocketImpl::packetize(Buffer& buffer)
//{	
//	// Create the outgoing packet from the available creation
//	// strategies. For best performance the most used strategies
//	// should have the highest priority.
//	IPacket* packet = createPacket(buffer);
//	if (!packet) {
//		Log("warn") << "[SocketImpl:" << this << "] Unable to create data packet." << endl;	
//		return;
//	}
//
//	packet->info = new PacketInfo(*this, SocketImpl::peerAddress());
//	dispatch(this, *packet);	
//	delete packet;
//}
//
//
//void SocketImpl::onConnect() 
//{
//	Log("trace") << "[SocketImpl:" << this << "] On Connect: " << impl() << endl;	
//	{
//		FastMutex::ScopedLock lock(_mutex); 
//
//		// This event tells us that we are connected ie. the 
//		// connection is writable. There is no more need for
//		// this event so we can remove the delegate.
//		_reactor.detach(*this, reactorDelegate(this, &SocketImpl::onConnect, SocketWritable));	
//		_connected = true;
//	}
//	Connected.dispatch(this);
//}
//
//
//void SocketImpl::onClose()
//{
//	Log("trace") << "[SocketImpl:" << this << "] On Close: " << impl() << endl;
//	
//	// This method must be called from close()
//
//	bool destroy = false;
//	{
//		FastMutex::ScopedLock lock(_mutex); 
//		assert(_connected);
//		if (!_connected)
//			return;
//		_connected = false;
//		destroy = _deleteOnClose;
//	}
//
//	Closed.dispatch(this);
//
//	if (destroy) {
//		Log("trace") << "[SocketImpl:" << this << "] Delete on close" << endl;	
//		delete this;
//	}
//
//	Log("trace") << "[SocketImpl:" << this << "] On Close: OK" << endl;
//}
//
//
//void SocketImpl::onError() 
//{
//	Log("warn") << "[SocketImpl:" << this << "] On Error" << endl;
//
//	// We received an error notification from the reactor.
//	// Close the socket.
//	close();
//}
//
//
//void SocketImpl::setError(const std::string& err)
//{
//	Log("error") << "[SocketImpl:" << this << "] Error: " << err << endl;
//	_error = err;
//	int n = errorno();
//	Error.dispatch(this, _error, n);
//	close();
//	Log("error") << "[SocketImpl:" << this << "] Error: OK" << endl;
//}
//
//
//void SocketImpl::resetBuffer() 
//{	
//	FastMutex::ScopedLock lock(_mutex); 
//
//	// Set the initial buffer size to the getReceiveBufferSize value.
//	// If read sizes surpass this value the buffer size will be adjusted
//	// accordingly.
//	int recvSize = impl()->getReceiveBufferSize();
//	if (recvSize != _buffer.capacity()) {
//		Log("trace") << "[SocketImpl:" << this << "] Buffer Size: " << recvSize << endl;
//		_buffer.reserve(recvSize);
//	}	
//	_buffer.clear();
//}
//
//
//void SocketImpl::bindEvents()
//{		
//	FastMutex::ScopedLock lock(_mutex); 
//
//	_reactor.attach(*this, reactorDelegate(this, &SocketImpl::onReadable, SocketReadable));
//	_reactor.attach(*this, reactorDelegate(this, &SocketImpl::onConnect, SocketWritable));
//	_reactor.attach(*this, reactorDelegate(this, &SocketImpl::onError, SocketError));
//}
//
//
//void SocketImpl::unbindEvents()
//{	
//	FastMutex::ScopedLock lock(_mutex); 
//
//	_reactor.detach(*this, reactorDelegate(this, &SocketImpl::onReadable, SocketReadable));
//	_reactor.detach(*this, reactorDelegate(this, &SocketImpl::onConnect, SocketWritable));
//	_reactor.detach(*this, reactorDelegate(this, &SocketImpl::onError, SocketError));
//}
//
//
//Reactor& SocketImpl::reactor()
//{
//	FastMutex::ScopedLock lock(_mutex); 
//	return _reactor;
//}
//
//
//bool SocketImpl::isConnected()
//{
//	FastMutex::ScopedLock lock(_mutex); 
//	return _connected;
//}
//
//
//bool SocketImpl::isError()
//{
//	FastMutex::ScopedLock lock(_mutex);
//	return !_error.empty();
//}
//
//
//void SocketImpl::deleteOnClose(bool flag)
//{
//	FastMutex::ScopedLock lock(_mutex);
//	_deleteOnClose = flag;
//}
//
//
//std::string SocketImpl::error() const
//{
//	FastMutex::ScopedLock lock(_mutex); 
//	return _error;
//}
//
//
//int SocketImpl::errorno() const 
//{
//	try	{
//		return impl()->socketError();
//	}
//	catch (Poco::Net::/*InvalidSocketException*/NetException& exc) {
//		// swallow it
//	}
//	return -1; // invalid socket
//}
//
//
//Address SocketImpl::address() const 
//{
//	try	{
//		return impl()->address();
//	}
//	catch (Poco::Net::/*InvalidSocketException*/NetException& exc) {
//		// swallow it
//	}
//	return Address();
//}
//
//
//Address SocketImpl::peerAddress() const 
//{
//	try	{
//		return impl()->peerAddress();
//	}
//	catch (Poco::Net::/*InvalidSocketException*/NetException& exc) {
//		// swallow it
//	}
//	return Address();
//}
//
//
//TransportProtocol SocketImpl::transport() const 
//{ 
//	return Net::TCP; 
//}


} } // namespace Sourcey::Net





	//if (!stateEquals(ClientState::Error))
	//	setState(this, ClientState::Disconnected);
	//Connect.dispatch(this);
	//nf->release();
	//if (!stateEquals(ClientState::Error)) {
	//	setState(this, ClientState::Disconnected, "Connection error");
	//	close();
	//}

	//if (!stateEquals(ClientState::Disconnected)) 		
		//_error = e.displayText();
		//setState(this, ClientState::Disconnected, e.displayText());
	//Log("trace") << "[SocketImpl:" << this << "] Closing 1" << endl;	
		//destroy = _deleteOnClose;

	//if (destroy) {
	//	Log("trace") << "[SocketImpl:" << this << "] Destroying On Closing" << endl;	
	//	delete this;
	//}
	//Log("trace") << "[SocketImpl:" << this << "] Closing: OK" << endl;	
	
	
	//Poco::Observer<SocketImpl, WritableNotification>(*this, &SocketImpl::onConnect));
	//assert(0);
	/*


int SocketImpl::bytesAvailable()
{
	return StreamSocket::available();
}
	FastMutex::ScopedLock lock(_mutex); 
	FastMutex::ScopedLock lock(_mutex); 
	FastMutex::ScopedLock lock(_mutex); 
	setState(this, ClientState::Connected);
	// In high traffic situations the socket may become
	// writable with data in the read buffers, and the
	// readable notification get's missed. Let's flush 
	// the beffers now...
	if (bytesAvailable() > 0) {
		ReactorEvent& rNf = new ReadableNotification(&_reactor);
		onReadable(rNf);
	}
	*/

/*
*/

/*
StreamSocket& SocketImpl::impl()
{
	FastMutex::ScopedLock lock(_mutex); 
	return *this;
}
*/

/*
void SocketImpl::recv(const char* data, int size, const Address& peerAddress)
{
	try	{
		Log("trace") << "[SocketImpl:" << this << "] RECV: " << size << ": " << address().toString() << "<--" << peerAddress.toString() << endl;
		
		/// Create the outgoing packet from the available handler protocols.
		/// TODO: To attain best performance, the most used socket packet
		/// interfaces should have the highest priority.
		Buffer buffer(data, size);
		IPacket* packet = createPacket(buffer);
		if (!packet) {
			Log("warn") << "[SocketImpl:" << this << "] Unable to create data packet." << endl;	
			return;
		}

		packet->info = new PacketInfo(*this, address(), peerAddress);
		dispatch(this, *packet);
	
		delete packet;
	}	
	catch (StopPropagation&) {
		//Log("trace") << "[SocketImpl:" << this << "] Stopping Event Propagation" << endl;
	}
}
*/


/*
void SocketImpl::transferTo(SocketImpl* r)
{
	reinterpret_cast<StreamSocket&>(*r) 
		= reinterpret_cast<const StreamSocket&>(*this);

	//StreamSocket& rImpl = r->impl();
	// rImpl = *this;
	delete this;
	//r = NULL;
	r->bindEvents();
}
*/
