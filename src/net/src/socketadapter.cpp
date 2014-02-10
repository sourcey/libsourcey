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


#include "scy/net/socketadapter.h"
#include "scy/net/socket.h"


using std::endl;


namespace scy {
namespace net {


SocketAdapter::SocketAdapter(SocketAdapter* sender, SocketAdapter* receiver) : 
	 _sender(sender)//, _recvAdapter(recvAdapter)
{
	//TraceLS(this) << "Create" << endl;	
	assert(sender != this);
	//assert(receiver != this);

	if (receiver)
		addReceiver(receiver);
}
	

SocketAdapter::~SocketAdapter()
{
	//TraceLS(this) << "Destroy" << endl;	
	
#if 0
	// Delete child adapters
	// In order to prevent deletion, the outside 
	// application must nullify the adapter pointers
	if (_recvAdapter)
		delete _recvAdapter;
	if (_sender)
		delete _sender;
#endif
}

	
int SocketAdapter::send(const char* data, std::size_t len, int flags)
{
	assert(_sender); // should have output adapter if default impl is used
	if (!_sender) return -1;
	return _sender->send(data, len, flags);
}


int SocketAdapter::send(const char* data, std::size_t len, const Address& peerAddress, int flags)
{
	assert(_sender); // should have output adapter if default impl is used
	if (!_sender) return -1;
	return _sender->send(data, len, peerAddress, flags);
}


int SocketAdapter::sendPacket(const IPacket& packet, int flags)
{	
	// Try to cast as RawPacket so we can send without copying any data.
	auto raw = dynamic_cast<const RawPacket*>(&packet);
	if (raw)
		return send((const char*)raw->data(), raw->size(), flags);
	
	// Dynamically generated packets need to be written to a
	// temp buffer for sending. 
	else {
		Buffer buf;
		packet.write(buf);
		return send(buf.data(), buf.size(), flags);
	}
}


int SocketAdapter::sendPacket(const IPacket& packet, const Address& peerAddress, int flags)
{	
	// Try to cast as RawPacket so we can send without copying any data.
	auto raw = dynamic_cast<const RawPacket*>(&packet);
	if (raw)
		return send((const char*)raw->data(), raw->size(), peerAddress, flags);
	
	// Dynamically generated packets need to be written to a
	// temp buffer for sending. 
	else {
		Buffer buf; //(2048);
		//buf.reserve(2048);
		packet.write(buf);
		return send(buf.data(), buf.size(), peerAddress, flags);
	}
}


void SocketAdapter::sendPacket(IPacket& packet)
{
	int res = sendPacket(packet, 0);
	if (res < 0)
		throw std::runtime_error("Invalid socket operation");
}


void SocketAdapter::onSocketConnect()
{
	/*
	//TraceLS(this) << "On connect: " << adapter->Connect.refCount() << endl;
	//assert(adapter);
	if (_recvAdapter)
		_recvAdapter->onSocketConnect();
		//adapter->Connect.emit(adapter);
	else
	*/
		Connect.emit(self());
}


void SocketAdapter::onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress)
{
	/*
	//TraceLS(this) << "Recv: " << adapter->Recv.refCount() << endl;	
	//SocketPacket packet(socket ? adapter : this, buf, peerAddr);
	if (_recvAdapter)
		_recvAdapter->onSocketRecv(buf, peerAddr);
		//adapter->Recv.emit(adapter, buf, peerAddr);
	else
	*/
		Recv.emit(self(), buffer, peerAddress);
}


void SocketAdapter::onSocketError(const scy::Error& error) //const Error& error
{
	//TraceLS(this) << "Error: " << adapter->Error.refCount() << ": " << message << endl;	syserr, message
	/*
	if (_recvAdapter)
		_recvAdapter->onSocketError(error);
		//adapter->Error.emit(adapter, error);
	else
	*/
		Error.emit(self(), error);
}


void SocketAdapter::onSocketClose()
{
	//TraceLS(this) << "On close: " << adapter->Close.refCount() << endl;	
	/*
	if (_recvAdapter)
		_recvAdapter->onSocketClose();
		//adapter->Close.emit(adapter);
	else
	*/
		Close.emit(self());
}


/*
void SocketAdapter::addReceiver(SocketAdapter* adapter, bool freeExisting)
{
	if (_recvAdapter == adapter) return;
	if (_recvAdapter && freeExisting)
		delete _recvAdapter;
	_recvAdapter = adapter;
	_recvAdapter->Connect += sdelegate(_recvAdapter, &net::SocketAdapter::onSocketConnect);
}


SocketAdapter* SocketAdapter::recvAdapter()
{
	return _recvAdapter;
}
	void addReceiver(SocketAdapter* adapter);
*/


void SocketAdapter::addReceiver(SocketAdapter* adapter, int priority) 
{	
	Connect += delegate(adapter, &net::SocketAdapter::onSocketConnect, priority);
	Recv += delegate(adapter, &net::SocketAdapter::onSocketRecv, priority);
	Error += delegate(adapter, &net::SocketAdapter::onSocketError, priority);
	Close += delegate(adapter, &net::SocketAdapter::onSocketClose, priority);

	//TraceLS(this) << "Duplicating socket: " << &adapter << endl;
	//_observers.push_back(socket);		
	//sortObservers();
	//if (shared)
	//	duplicate();
	//TraceLS(this) << "Duplicated socket: " << &adapter << endl;
}


void SocketAdapter::removeReceiver(SocketAdapter* adapter)  
{	
	Connect -= delegate(adapter, &net::SocketAdapter::onSocketConnect);
	Recv -= delegate(adapter, &net::SocketAdapter::onSocketRecv);
	Error -= delegate(adapter, &net::SocketAdapter::onSocketError);
	Close -= delegate(adapter, &net::SocketAdapter::onSocketClose);
	/*
	// TODO: Ensure socket destruction when released?
	for (auto it = _observers.begin(); it != _observers.end(); ++it) {
		if (*it == socket) {
			//TraceLS(this) << "Releasing socket: " << &adapter << endl;
			_observers.erase(it);
			//sortObservers();
			release();
			return;
		}
	}
	assert(0 && "unknown socket adapter");
	*/
}


void SocketAdapter::setSender(SocketAdapter* adapter, bool freeExisting)
{
	if (_sender == adapter) return;
	if (_sender && freeExisting)
		delete _sender;
	_sender = adapter;
}


#if 0
SocketAdapter* SocketAdapter::sendAdapter()
{
	return _sender;
}


void SocketAdapter::onSocketConnect() 
{
	if (_adapter)
		_adapter->onSocketConnect();
	else
		Connect.emit(self());

	/*
	_insideCallback = true;
	//for (auto observer : _observers) //for (auto& observer : _observers)
	//	observer->onSocketConnect();
	for (size_t i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketConnect();
	_insideCallback = false;
	*/
}


void SocketAdapter::onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress)
{
	if (_adapter)
		_adapter->onSocketRecv(buf, peerAddr);
	else {
		SocketPacket packet(this, buf, peerAddr);
		Recv.emit(self(), packet);
	}

	/*
	_insideCallback = true;
	//for (auto observer : _observers) //for (auto& observer : _observers)
	//	observer->onSocketRecv(buf, peerAddr);
	for (size_t i = 0; i < _observers.size(); i++)
		_observers[i]->onSocketRecv(buf, peerAddr);
	_insideCallback = false;
	*/
}


void SocketAdapter::onSocketError(const scy::Error& error)
{
	if (_adapter)
		_adapter->onSocketError(error);
	else {
		Error.emit(self(), error);
	}

	/*
	_insideCallback = true;
	//for (auto observer : _observers) //for (auto& observer : _observers)
	//	observer->onSocketError(error);
	for (size_t i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketError(error);
	_insideCallback = false;
	*/
}


void SocketAdapter::onSocketClose()
{
	if (_adapter)
		_adapter->onSocketClose();
	else
		Close.emit(self());

	/*
	_insideCallback = true;
	//for (auto observer : _observers) //for (auto& observer : _observers)
	//	observer->onSocketClose();
	for (size_t i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketClose();
	_insideCallback = false;
	*/
}
#endif


} } // namespace scy::net




/*
SocketAdapter::SocketAdapter(const SocketAdapter::Ptr& adapter) : //, int priority
	adapter(adapter)//, priority(priority)
{
	//TraceLS(this) << "Create" << endl;	
}
*/