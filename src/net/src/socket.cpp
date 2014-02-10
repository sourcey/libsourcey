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


#include "scy/net/socket.h"
#include "scy/net/socketadapter.h"
#include "scy/net/types.h"
#include "scy/net/address.h"

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace net {


//
// Socket
//


Socket::Socket() //: 
	//_adapter(nullptr)//, //SharedObject(true), //new DeferredDeleter<Socket>()),
	//_insideCallback(false)
{
	TraceLS(this) << "Create" << endl;	
}


Socket::~Socket()
{
	TraceLS(this) << "Destroy" << endl;	

	// Delete the adapter, if any
	//if (_adapter)
	//	delete _adapter;

	// The destructor never be called from inside a callback.
	// Deferred destruction ensures this never occurs.
	//assert(!_insideCallback && "destructor scope error");
}

	
void Socket::connect(const std::string& host, UInt16 port) 
{
	TraceLS(this) << "Connect to host: " << host << ":" << port << endl;
	if (Address::validateIP(host))
		connect(Address(host, port));
	else {
		init();
		assert(!closed());
		net::resolveDNS(host, port, [](const net::DNSResult& dns) 
		{	
			auto* sock = reinterpret_cast<Socket*>(dns.opaque);
			TraceL << "DNS resolved: " << dns.success() << endl;

			// Return if the socket was closed while resolving
			if (sock->closed()) {			
				WarnL << "DNS resolved but socket closed" << endl;
				return;
			}

			// Set the connection error if DNS failed
			if (!dns.success()) {
				sock->setError("Failed to resolve DNS for " + dns.host);
				return;
			}

			try {	
				// Connect to resolved host
				sock->connect(dns.addr);
			}
			catch (...) {
				// Swallow errors
				// Can be handled by Socket::Error signal
			}	
		}, this); 
	}
}

	
/*
int Socket::send(const char* data, std::size_t len, int flags)
{
	TraceLS(this) << "Send: " << len << endl;

	if (_adapter) {
		assert(_adapter->socket == this);
		return _adapter->send(data, len, flags);
	}
	return _base->send(data, len, flags);
}


int Socket::send(const char* data, std::size_t len, const Address& peerAddress, int flags)
{
	TraceLS(this) << "Send to peer: " << peerAddress << ": " << len << endl;

	if (_adapter) {
		assert(_adapter->socket == this);
		return _adapter->send(data, len, peerAddress, flags);
	}
	return _base->send(data, len, peerAddress, flags);
}
*/


/*
void Socket::setAdapter(SocketAdapter* adapter)
{	
	// Assign the new adapter pointer
	_adapter = adapter; //.swap(adapter);
	if (_adapter)
		_adapter->socket = this;
}


void Socket::replaceAdapter(SocketAdapter* adapter)
{
	// NOTE: Just swap the SocketAdapter pointers as
	// we don't want to modify the container since we
	// may be inside the old adapter's callback scope.
	//_base->swapObserver(_adapter, adapter);

	// Defer deletion to the next iteration.
	// The old adapter will receive no more callbacks.	
	if (_adapter)
		deleteLater<SocketAdapter>(_adapter);
	
	setAdapter(adapter);
}
	

SocketAdapter* Socket::adapter() const
{
	return _adapter;
}
*/


#if 0
Socket::Socket() : //SocketAdapter* adapter
	_base(nullptr), 
	_adapter(nullptr)//, 
	//_adapter(adapter)
{
	//if (!_adapter) 
	//	_adapter = new SocketAdapter(this);	
	//if (!_adapter->socket) 
	//	_adapter->socket = this;	
}


Socket::Socket(Socket* base, bool shared) : //, SocketAdapter* adapter
	_base(base), 
	_adapter(nullptr)
{
	//if (!_adapter) 
	//	_adapter = new SocketAdapter(this);	
	//if (!_adapter->socket) 
	//	_adapter->socket = this;	
	if (_base) {
		_base->addObserver(this, shared);
	}
}


Socket::Socket(const Socket& socket) : //, SocketAdapter* adapter
	_base(socket._base), 
	_adapter(nullptr)//, 
	//_adapter(adapter)
{	
	if (_base) {
		_base->addObserver(this, true);
	}
}

	
Socket& Socket::operator = (const Socket& socket)
{
	return assign(socket._base, true);
}


Socket& Socket::assign(Socket* base, bool shared)
{	
	if (_base != base) {
		if (_base) _base->removeObserver(this);
		_base = base;
		if (_base) _base->addObserver(this, shared);
	}
	return *this;
}


Socket::~Socket()
{
	if (_base)
		_base->removeObserver(this);
	if (_adapter)
		delete _adapter;
}


bool Socket::shutdown()
{
	return _base->shutdown();
}


void Socket::close()
{
	_base->close();
}
	

bool Socket::closed() const
{
	return _base->closed();
}


void Socket::connect(const Address& address) 
{
	return _base->connect(address);
}


void Socket::bind(const Address& address) 
{
	return _base->bind(address);
}
	

int Socket::send(const char* data, std::size_t len, int flags)
{
	TraceLS(this) << "Send: " << len << endl;

	if (_adapter) {
		assert(_adapter->socket == this);
		return _adapter->send(data, len, flags);
	}
	return _base->send(data, len, flags);
}


int Socket::send(const char* data, std::size_t len, const Address& peerAddress, int flags)
{
	TraceLS(this) << "Send to peer: " << peerAddress << ": " << len << endl;

	if (_adapter) {
		assert(_adapter->socket == this);
		return _adapter->send(data, len, peerAddress, flags);
	}
	return _base->send(data, len, peerAddress, flags);
}


int Socket::send(const IPacket& packet, int flags)
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


int Socket::send(const IPacket& packet, const Address& peerAddress, int flags)
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


void Socket::send(void*, IPacket& packet)
{
	int res = send(packet);
	if (res < 0)
		throw std::runtime_error("Invalid socket operation");
}


Address Socket::address() const
{
	return _base->address();
}


Address Socket::peerAddress() const
{
	return _base->peerAddress();
}


void Socket::listen(int backlog) 
{ 
	_base->listen(backlog); 
}


net::TransportType Socket::transport() const 
{ 
	return _base->transport();
}
	
		
void Socket::setError(const scy::Error& err) 
{ 
	return _base->setError(err);
}
	
	
void Socket::connect(const std::string& host, UInt16 port) 
{
	return _base->connect(host, port);
}


void Socket::onSocketConnect()
{
	//TraceLS(this) << "On connect: " << socket->Connect.refCount() << endl;	
	if (_adapter) {
		_adapter->onSocketConnect();
	}
	else {
		Connect.emit(this);
	}
}


void Socket::onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress)
{
	//TraceLS(this) << "On recv: " << buf.size() << endl;	
	if (_adapter) {
		_adapter->onSocketRecv(buf, peerAddr);
	}
	else {
		SocketPacket packet(this, buf, peerAddr);
		Recv.emit(this, packet);
	}
}


void Socket::onSocketError(const scy::Error& error)
{
	//TraceLS(this) << "On error: " << error.message << endl;
	if (_adapter) {
		_adapter->onSocketError(error);
	}
	else {
		Error.emit(this, error);
	}
}


void Socket::onSocketClose()
{
	//TraceLS(this) << "On close" << endl;	
	if (_adapter) {
		_adapter->onSocketClose();
	}
	else {
		assert(closed());
		Close.emit(this);
	}
}

	
const Error& Socket::error() const 
{ 
	return _base->error();
}
	

Socket& Socket::base() const
{
	assert(_base);
	return *_base;
}
	

SocketAdapter* Socket::adapter() const
{
	return _adapter;
}


void Socket::setAdapter(SocketAdapter* adapter)
{	
	// Assign the new adapter pointer
	_adapter = adapter;
	if (_adapter)
		_adapter->socket = this;
}


void Socket::replaceAdapter(SocketAdapter* adapter)
{
	// NOTE: Just swap the SocketAdapter pointers as
	// we don't want to modify the container since we
	// may be inside the old adapter's callback scope.
	//_base->swapObserver(_adapter, adapter);

	// Defer deletion to the next iteration.
	// The old adapter will receive no more callbacks.	
	if (_adapter)
		deleteLater<SocketAdapter>(_adapter);
	
	setAdapter(adapter);
}


int Socket::isNull() const
{
	return _base == 0;
}


/*
bool SocketAdapter::compareProiroty(const SocketAdapter* l, const SocketAdapter* r) 
{
	return l->priority > r->priority;
}
*/
#endif

#if 0
//
// Socket Adapter
//


SocketAdapter::SocketAdapter(Socket::Ptr socket) : //, int priority
	socket(socket)//, priority(priority)
{
	//TraceLS(this) << "Create" << endl;	
}
	

SocketAdapter::~SocketAdapter()
{
	//TraceLS(this) << "Destroy" << endl;	
}


void SocketAdapter::onSocketConnect()
{
	//TraceLS(this) << "On connect: " << socket->Connect.refCount() << endl;	
	socket->Connect.emit(socket.get());
}


void SocketAdapter::onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress)
{
	//TraceLS(this) << "Recv: " << socket->Recv.refCount() << endl;	
	SocketPacket packet(socket.get(), buf, peerAddr);
	socket->Recv.emit(socket.get(), packet);
}


void SocketAdapter::onSocketError(const Error& error) //const Error& error
{
	//TraceLS(this) << "Error: " << socket->Error.refCount() << ": " << message << endl;	syserr, message
	socket->Error.emit(socket.get(), error);
}


void SocketAdapter::onSocketClose()
{
	//TraceLS(this) << "On close: " << socket->Close.refCount() << endl;	
	socket->Close.emit(socket.get());
}

	
int SocketAdapter::send(const char* data, std::size_t len, int flags)
{
	return socket->/*base().*/send(data, len, flags);
}


int SocketAdapter::send(const char* data, std::size_t len, const Address& peerAddress, int flags)
{
	return socket->/*base().*/send(data, len, peerAddress, flags);
}
#endif


} } // namespace scy::net




/*
void Socket::replaceAdapter(SocketAdapter* adapter)
{
	// NOTE: Just swap the SocketAdapter pointers as
	// we don't want to modify the container since we
	// may be inside the old adapter's callback scope.
	//_base->swapObserver(_adapter, adapter);

	// Defer deletion to the next iteration.
	// The old adapter will receive no more callbacks.	
	//if (_adapter)
	//	deleteLater<SocketAdapter>(_adapter);
	
	setAdapter(adapter);
}
*/


/*
void Socket::swapObserver(SocketAdapter* a, SocketAdapter* b)
{
	for (std::vector<Socket*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		if ((*it) == a) {
			*it = b;
			TraceLS(this) << "swapAdapter: " << a << ": " << b << endl;
			return;
		}
	}
	assert(0 && "unknown socket adapter");
}
*/


/*
void Socket::sortObservers()  
{	
	sort(_observers.begin(), _observers.end(), SocketAdapter::compareProiroty);
}
	

bool Socket::connected() const 
{ 
	return _connected;
}
*/






/*
void Socket::setError(const Error& err) 
{ 
	TraceLS(this) << "Set error: " << err.message << endl;	
	_error = err;
}
*/
		
	//SocketAdapter* oldAdapter = _adapter;
	//_base->addObserver(adapter, true);
	//if (_adapter) {
	//_base->removeObserver(_adapter);
	//delete _adapter;
	//}
//
// SocketAdapter methods
//
/*
int Socket::send(const IPacket& packet, int flags)
{
	return _base->send(packet, flags);
}


int Socket::send(const IPacket& packet, const Address& peerAddress, int flags)
{
	return _base->send(packet, peerAddress, flags);
}
*/
/*
void Socket::duplicate() 
{ 
	_base->duplicate(); 
}


void Socket::release() 
{ 
	_base->release(); 
}


int base().refCount() const 
{ 
	return _base->refCount();
}
*/
/*
SocketAdapter::SocketAdapter(int priority) : 
	priority(priority)
{
	//TraceLS(this) << "Create" << endl;	
}
	

SocketAdapter::~SocketAdapter()
{
	//TraceLS(this) << "Destroy" << endl;	
}
*/

	



/*
// -------------------------------------------------------------------
//
class Socket: public Handle<Socket>
	// Socket is the base class for accessing socket contexts.
	// It provides a disposible layer which referencing the 
	// internal socket context.
	//
	// The common interface provides basic socket operations
	// can be extended as necessary for different protocols.
{
public:
	Socket() {};

	Socket(const Socket& socket);
		// Attaches the socket context from the other socket and
		// increments the reference count of the socket context.
		
	Socket(Socket* context);
		// Creates the Socket and attaches the given Socket.
		// The socket takes owership of the Socket.

	Socket& operator = (const Socket& socket);
		// Assignment operator.
		//
		// Releases the socket's socket context and
		// attaches the socket context from the other socket and
		// increments the reference count of the socket context.
		
	~Socket();
		// Destroys the Socket and releases the socket context.

	Socket* base() const;
		// Returns the Socket for this socket.
		
	void connect(const Address& address);
	bool shutdown();
	void close();

	void bind(const Address& address);
	void listen(int backlog = 64);
		
	int send(const char* data, std::size_t len, int flags = 0);
	//int send(const char* data, std::size_t len, const Address& peerAddress, int flags = 0);
	//int send(const IPacket& packet, int flags = 0);
	//int send(const IPacket& packet, const Address& peerAddress, int flags = 0);
	
	Address address() const;
		// The locally bound address.

	Address peerAddress() const;
		// The connected peer address.

	net::TransportType transport() const;
		// The transport protocol: TCP, UDP or SSLTCP.
		// See TransportType definition.
		
	void duplicate();
	void release();
	int refCount() const;

protected:
	Socket* _base;
};
*/

/*
*/


/*
void Stream::onError(const int errno)
{
}


void Stream::onClose()
{
}
*/
	//_stream.reset(stream); // TODO: No need for this, managed by CountedHandle
	
	//if ()ptr ? ptr : 
	//stream->data = ptr;
	//assert(_stream->data == nullptr ||
	//	static_cast<Stream*>(_stream->data) == this);
	//_stream->data = this;

	// TODO: Close if needed.	
	//TraceLS(this) << "Destroy: Send Close" << endl;
	// afterClose not always getting called
	//uv_close((uv_handle_t*)stream(), Stream::afterClose);
	//shutdown();
	//readStop();
	//close();

	/*


int Stream::writeQueueSize() const
{
	return stream()->write_queue_size;
}
	if (_handle) {		
		//TraceLS(this) << "Destroy: Handle" << endl;
		delete _handle;
		_handle = nullptr;
	}
	//TraceLS(this) << "Destroy: OK" << endl;
	*/
	/*
	


void Stream::close() 
{	
	//TraceLS(this) << "Send Close" << endl;
	assert(0);
	// afterClose not always getting called
    //uv_close((uv_handle_t*)stream(), nullptr); //Stream::afterClose
	//delete this;
	//onClose();
}
	//uv_close((uv_handle_t*)req->handle, afterClose);
	//Stream* io = static_cast<Stream*>(req->data);
	//io->close();

void Stream::afterClose(uv_handle_t* peer) 
{	
	//TraceL << "After Close: " << peer << endl;
	// NOTE: Sending Closed from close() method as afterClose
	// does not always fire depending on server response.
	//io->Close.emit(io, io->error().code);
	Stream* io = static_cast<Stream*>(peer->data);
	assert((uv_handle_t*)io->stream() == peer);
	delete peer;	
	release();
	//::free(peer);
}
	*/