//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#include "Sourcey/Net/UDPSocket.h"
#include "Sourcey/Logger.h"

#include "Poco/Net/NetException.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Net {


UDPSocket::UDPSocket(Reactor& reactor, Runner& runner) : 
	_reactor(reactor),
	_closed(true),
	_runner(runner),
	_buffer(MAX_UDP_PACKET_SIZE),
	_queue(new PacketQueue(*this, _runner))
{
	Log("debug") << "[UDPSocket:" << this << "] Creating" << endl;
}

	
UDPSocket::UDPSocket(const UDPSocket& r) : 
	DatagramSocket(r),
	_reactor(r._reactor),
	_closed(r._closed),
	_runner(r._runner)
{
}


UDPSocket::~UDPSocket() 
{	
	Log("debug") << "[UDPSocket:" << this << "] Destroying" << endl;
	_queue->destroy();
}


void UDPSocket::connect(const Address& address)
{	
	Log("debug") << "[UDPSocket:" << this << "] Connecting: " << address.toString() << endl;
	
	if (isConnected())
		throw Exception("Socket already bound/connected");

	try {
		FastMutex::ScopedLock lock(_mutex);
		DatagramSocket::connect(address);
		bindEvents();
		_queue->start();
		_closed = false;
		Connected.dispatch(this);
	} 
	catch (Poco::Exception& e) {
		unbindEvents();
		_error = e.displayText();
		Log("error") << "[UDPSocket:" << this << "] Connection failed: " << _error << endl;
		e.rethrow();
	}
}


void UDPSocket::bind(const Address& address, bool reuseAddress)
{	
	Log("debug") << "[UDPSocket:" << this << "] Binding: " << address.toString() << endl;

	if (isConnected())
		throw Exception("Socket already bound/connected");

	try {
		FastMutex::ScopedLock lock(_mutex); 	

		_error = "";
		DatagramSocket::bind(address, reuseAddress);
		bindEvents();
		_queue->start();
		_closed = false;
	} 
	catch (Poco::Exception& e) {
		unbindEvents();
		_error = e.displayText();
		Log("error") << "[UDPSocket:" << this << "] Connection failed: " << _error << endl;
		e.rethrow();
	}
}
	

void UDPSocket::close()
{	
	Log("debug") << "[UDPSocket:" << this << "] Closing" << endl;

	unbindEvents();
	FastMutex::ScopedLock lock(_mutex); 
	if (!_closed) {
		try	{
			// If the socket is already closed for whatever
			// reason an InvalidSocketException will be thrown. 
			// Just swallow it.
			DatagramSocket::close();
		}
		catch (Poco::Net::NetException& exc) {
			Log("debug") << "[TCPSocket:" << this << "] Closing: " << exc.displayText() << endl;
		}
		_queue->stop();
		_closed = true;	
		Closed.dispatch(this);
	}
}


void UDPSocket::onReadable()
{
	Log("debug") << "[UDPSocket:" << this << "] On Readable" << endl;
	
	try	{
		//int size = 0;
		//Buffer* buffer = NULL;
		Address peerAddress;
		//{
		// No locking required here because we always read
		// from the reactor thread.
		// FastMutex::ScopedLock lock(_mutex); 
		//_receiving = true; 
		//buffer = &_buffer;
		int size = min(available(), MAX_TCP_PACKET_SIZE);
		assert(_buffer.capacity() >= size);
		size = receiveFrom(_buffer.bytes(), size, peerAddress);
		_buffer.setPosition(0);
		_buffer.setSize(size);
		//}		
		if (size > 0) {	
			Log("trace") << "[UDPSocket:" << this << "] RECV: " << size << ": " << peerAddress.toString() << endl;
			recv(_buffer, peerAddress);
		}
	}
	catch (Poco::Net::NetException& exc) {
		Log("error") << "[UDPSocket:" << this << "] Error: " << exc.displayText() << endl;
		close();
	}

	//FastMutex::ScopedLock alock(_mutex);
	//_receiving = false; 
}


void UDPSocket::recv(Buffer& buffer, const Address& peerAddress)
{
	packetize(buffer, peerAddress);
}


void UDPSocket::packetize(Buffer& buffer, const Address& peerAddress)
{	
	IPacket* packet = createPacket(buffer);
	if (!packet) {
		Log("warn") << "[UDPSocket:" << this << "] Unable to create data packet." << endl;	
		return;
	}	

	packet->info = new PacketInfo(*this, peerAddress);
	//dispatch(this, *packet);
	_queue->push(packet);
}



void UDPSocket::setError(const std::string& err)
{
	Log("error") << "[UDPSocket:" << this << "] Error: " << err << endl;
	_error = err;
	close();
}


int UDPSocket::send(const char* data, int size) 
{	
	// NOTE: Must be connected to peer to send
	return send(data, size, peerAddress());
}


int UDPSocket::send(const char* data, int size, const Address& peerAddress) 
{	
	assert(size <= MAX_TCP_PACKET_SIZE);
	try	{
		Log("trace") << "[UDPSocket:" << this << "] SEND: " 
			<< size << ": " << peerAddress.toString() << endl;
		return DatagramSocket::sendTo(data, size, peerAddress);
	}
	catch (Poco::Exception& exc) {
		setError(exc.displayText());
	}
	return -1; // error
}


int UDPSocket::send(const DataPacket& packet)
{
	// Most large packets, ie. MediaPackets derive from
	// DataPacket, so we can avoid memcpy altogether.
	return send((const char*)packet.data(), packet.size(), peerAddress());
}


int UDPSocket::send(const DataPacket& packet, const Address& peerAddress)
{
	return send((const char*)packet.data(), packet.size(), peerAddress);
}


int UDPSocket::send(const IPacket& packet, const Address& peerAddress) 
{
	// Unlike TCP sockets our packets we are generally
	// limited by a 1500 MTU so let's just copy the 
	// packet data.
	Buffer buf(MAX_UDP_PACKET_SIZE);
	packet.write(buf);	
	return send(buf.data(), buf.size(), peerAddress);
}


int UDPSocket::send(const IPacket& packet) 
{
	return send(packet, peerAddress());
}


void UDPSocket::send(IPacket& packet)
{
	// Always try to cast packets as DataPacket type
	// so we can avoid copying data.
	DataPacket* dataPacket = packet.as<DataPacket>();
	if (dataPacket)
		send(*dataPacket);
	else
		send(packet, peerAddress());
}

	
void UDPSocket::onError() 
{
	Log("warn") << "[UDPSocket:" << this << "] On Error" << endl;
	close();
}


void UDPSocket::bindEvents()
{
	FastMutex::ScopedLock lock(_mutex); 

	_reactor.detach(*this, reactorDelegate(this, &UDPSocket::onReadable, SocketReadable));
	_reactor.detach(*this, reactorDelegate(this, &UDPSocket::onError, SocketError));
}


void UDPSocket::unbindEvents()
{
	FastMutex::ScopedLock lock(_mutex); 

	_reactor.detach(*this, reactorDelegate(this, &UDPSocket::onReadable, SocketReadable));
	_reactor.detach(*this, reactorDelegate(this, &UDPSocket::onError, SocketError));
}


std::string UDPSocket::error() const
{
	FastMutex::ScopedLock lock(_mutex); 
	return _error;
}


bool UDPSocket::isConnected()
{
	FastMutex::ScopedLock lock(_mutex);
	return !_closed;
}


bool UDPSocket::isError()
{
	FastMutex::ScopedLock lock(_mutex);
	return !_error.empty();
}


Reactor& UDPSocket::reactor()
{
	FastMutex::ScopedLock lock(_mutex);
	return _reactor;
}


Address UDPSocket::address() const 
{
	try	{
		return DatagramSocket::address();
	}
	catch (Poco::Net::InvalidSocketException& exc) {
		// swallow it
	}
	return Address();
}


Address UDPSocket::peerAddress() const 
{
	try	{
		return DatagramSocket::peerAddress();
	}
	catch (Poco::Net::InvalidSocketException& exc) {
		// swallow it
	}
	return Address();
}


int UDPSocket::errorno() const 
{
	try	{
		return impl()->socketError();
	}
	catch (Poco::Net::InvalidSocketException& exc) {
		// swallow it
	}
	return -1; // invalid socket
}


TransportProtocol UDPSocket::transport() const
{
	return Net::UDP;
}


} } // namespace Sourcey::Net