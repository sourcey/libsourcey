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


#include "Sourcey/Net/Socket.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"

#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace net {
	

Socket::Socket() : //SocketAdapter* adapter
	_base(nil), 
	_adapter(nil)//, 
	//_adapter(adapter)
{
	//if (!_adapter) 
	//	_adapter = new SocketAdapter(this);	
	//if (!_adapter->socket) 
	//	_adapter->socket = this;	
}


Socket::Socket(SocketBase* base, bool shared) : //, SocketAdapter* adapter
	_base(base), 
	_adapter(nil)
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
	_adapter(nil)//, 
	//_adapter(adapter)
{	
	//if (!_adapter) 
	//	_adapter = new SocketAdapter(this);	
	//if (!_adapter->socket) 
	//	_adapter->socket = this;	
	if (_base) {
		_base->addObserver(this, true);
	}
}

	
Socket& Socket::operator = (const Socket& socket)
{
	return assign(socket._base, true);
}


Socket& Socket::assign(SocketBase* base, bool shared)
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
	

int Socket::send(const char* data, int len, int flags)
{
	/*
	if (Sender.numProcessors() > 0) {
		Sender.write(RawPacket(data, len));
		return len;
	}
	*/
	if (_adapter) {
		assert(_adapter->socket == this);
		return _adapter->send(data, len, flags);
	}
	return _base->send(data, len, flags);
}


int Socket::send(const char* data, int len, const Address& peerAddress, int flags)
{
	// TODO: Make SocketPacket for address
	//if (Sender.numProcessors() > 0) {
	//	Sender.write(RawPacket(data, len));
	//	return len;
	//}
	if (_adapter) {
		assert(_adapter->socket == this);
		return _adapter->send(data, len, flags);
	}
		
	return _base->send(data, len, peerAddress, flags);
}


int Socket::send(const IPacket& packet, int flags)
{
	return send(packet, peerAddress(), flags);
}


int Socket::send(const IPacket& packet, const Address& peerAddress, int flags)
{	
	// Try to cast as RawPacket so we can avoid copying anything.
	// All large packet types, such as video and audio inherit from
	// RawPacket for this reason.
	const RawPacket* rawPacket = dynamic_cast<const RawPacket*>(&packet);
	if (rawPacket)
		return send((const char*)rawPacket->data(), rawPacket->size(), peerAddress, flags);
	
	// Other smaller dynamically generated packets need to be
	// written to a buffer before sending. 
	else {
		Buffer buf;
		packet.write(buf);
		traceL("Socket", this) << "Send IPacket: " << buf.available() << endl;	
		return send(buf.begin(), buf.available(), peerAddress, flags);
	}
}


void Socket::send(void*, IPacket& packet)
{
	int res = send(packet);
	if (res < 0)
		throw Exception("Invalid socket operation");
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
	//traceL("SocketAdapter", this) << "On connect: " << socket->Connect.refCount() << endl;	
	if (_adapter) {
		_adapter->onSocketConnect();
	}
	else {
		Connect.emit(this);
	}
}


void Socket::onSocketRecv(Buffer& buf, const Address& peerAddr)
{
	//traceL("SocketAdapter", this) << "On recv: " << socket->Recv.refCount() << endl;	
	if (_adapter) {
		_adapter->onSocketRecv(buf, peerAddr);
	}
	else {
		SocketPacket packet(*this, buf, peerAddr);
		Recv.emit(this, packet);
	}
}


void Socket::onSocketError(const scy::Error& error) //const Error& error
{
	//traceL("SocketAdapter", this) << "On error: " << socket->Error.refCount() << ": " << message << endl;	syserr, message
	if (_adapter) {
		_adapter->onSocketError(error);
	}
	else {
		Error.emit(this, error);
	}
}


void Socket::onSocketClose()
{
	//traceL("SocketAdapter", this) << "On close: " << socket->Close.refCount() << endl;	
	if (_adapter) {
		_adapter->onSocketClose();
	}
	else {
		Close.emit(this);
	}
}


	
const Error& Socket::error() const 
{ 
	return _base->error();
}
	

SocketBase& Socket::base() const
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
	traceL("Socket", this) << "Replacing adapter: " 
		<< _adapter << ": " << adapter << endl;

	// Assign the new adapter pointer
	_adapter = adapter;
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


//
// SocketAdapter methods
//

SocketAdapter::SocketAdapter(Socket* socket) : //, int priority
	socket(socket)//, priority(priority)
{
	//traceL("SocketAdapter", this) << "Creating" << endl;	
}
	

SocketAdapter::~SocketAdapter()
{
	//traceL("SocketAdapter", this) << "Destroying" << endl;	
}


void SocketAdapter::onSocketConnect()
{
	//traceL("SocketAdapter", this) << "On connect: " << socket->Connect.refCount() << endl;	
	socket->Connect.emit(socket);
}


void SocketAdapter::onSocketRecv(Buffer& buf, const Address& peerAddr)
{
	//traceL("SocketAdapter", this) << "On recv: " << socket->Recv.refCount() << endl;	
	SocketPacket packet(*socket, buf, peerAddr);
	socket->Recv.emit(socket, packet);
}


void SocketAdapter::onSocketError(const Error& error) //const Error& error
{
	//traceL("SocketAdapter", this) << "On error: " << socket->Error.refCount() << ": " << message << endl;	syserr, message
	socket->Error.emit(socket, error);
}


void SocketAdapter::onSocketClose()
{
	//traceL("SocketAdapter", this) << "On close: " << socket->Close.refCount() << endl;	
	socket->Close.emit(socket);
}

		
int SocketAdapter::send(const char* data, int len, int flags)
{
	return socket->base().send(data, len, flags);
}


int SocketAdapter::send(const char* data, int len, const Address& peerAddress, int flags)
{
	return socket->base().send(data, len, peerAddress, flags);
}


//
// SocketBase
//


SocketBase::SocketBase() : 
	CountedObject(new DeferredDeleter<SocketBase>()),
	_insideCallback(false)
{
	//traceL("SocketAdapter", this) << "Creating" << endl;	
}


SocketBase::~SocketBase()
{
	//traceL("SocketAdapter", this) << "Destroying" << endl;	

	// The destructor never be called from inside a callback.
	// Deferred destruction ensures this never occurs.
	assert(!_insideCallback && "destructor scope error");
}
	

void SocketBase::addObserver(Socket* socket, bool shared) 
{
	//traceL("SocketBase", this) << "Duplicating socket: " << &adapter << endl;
	_observers.push_back(socket);		
	//sortObservers();
	if (shared)
		duplicate();
	//traceL("SocketBase", this) << "Duplicated socket: " << &adapter << endl;
}


void SocketBase::removeObserver(Socket* socket)  
{	
	// TODO: Ensure socket destruction when released?
	for (vector<Socket*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		if (*it == socket) {
			//traceL("SocketBase", this) << "Releasing socket: " << &adapter << endl;
			_observers.erase(it);
			//sortObservers();
			release();
			return;
		}
	}
	assert(0 && "unknown socket adapter");
}


namespace internal {

	void onHostResolved(const net::DNSResult& dns, void* opaque)
	{	
		auto* sock = reinterpret_cast<SocketBase*>(opaque);
		traceL("SocketBase", sock) << "DNS resolved: " << dns.success() << endl;

		// Return if the socket was closed while resolving
		if (sock->closed()) {			
			warnL("SocketBase", sock) << "DNS resolved but socket closed" << endl;
			return;
		}

		// Set the connection error if DNS failed
		if (!dns.success()) {
			sock->setError("Failed to resolve host DNS for " + dns.host);
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
	}

}

	
void SocketBase::connect(const std::string& host, UInt16 port) 
{
	traceL("SocketBase", this) << "Connect to host: " << host << ":" << port << endl;
	if (Address::validateIP(host))
		connect(Address(host, port));
	else
		net::resolveDNS(host, port, internal::onHostResolved, this); 
}


void SocketBase::emitConnect() 
{
	_insideCallback = true;
	for (size_t i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketConnect();
	_insideCallback = false;
}


void SocketBase::emitRecv(Buffer& buf, const Address& peerAddr)
{
	_insideCallback = true;
	for (size_t i = 0; i < _observers.size(); i++)
		_observers[i]->onSocketRecv(buf, peerAddr);
	_insideCallback = false;
}


void SocketBase::emitError(const Error& error)
{
	_insideCallback = true;
	for (size_t i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketError(error);
	_insideCallback = false;
}


void SocketBase::emitClose()
{
	_insideCallback = true;
	for (size_t i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketClose();
	_insideCallback = false;
}


} } // namespace scy::net




/*
void SocketBase::swapObserver(SocketAdapter* a, SocketAdapter* b)
{
	for (vector<Socket*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		if ((*it) == a) {
			*it = b;
			traceL("SocketBase", this) << "swapAdapter: " << a << ": " << b << endl;
			return;
		}
	}
	assert(0 && "unknown socket adapter");
}
*/


/*
void SocketBase::sortObservers()  
{	
	sort(_observers.begin(), _observers.end(), SocketAdapter::compareProiroty);
}
	

bool SocketBase::connected() const 
{ 
	return _connected;
}
*/






/*
void SocketBase::setError(const Error& err) 
{ 
	traceL("SocketBase", this) << "Set error: " << err.message << endl;	
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
	//traceL("SocketAdapter", this) << "Creating" << endl;	
}
	

SocketAdapter::~SocketAdapter()
{
	//traceL("SocketAdapter", this) << "Destroying" << endl;	
}
*/

	



/*
// -------------------------------------------------------------------
//
class Socket: public Handle<SocketBase>
	/// Socket is the base class for accessing socket contexts.
	/// It provides a disposible layer which referencing the 
	/// internal socket context.
	///
	/// The common interface provides basic socket operations
	/// can be extended as necessary for different protocols.
{
public:
	Socket() {};

	Socket(const Socket& socket);
		/// Attaches the socket context from the other socket and
		/// increments the reference count of the socket context.
		
	Socket(SocketBase* context);
		/// Creates the Socket and attaches the given SocketBase.
		/// The socket takes owership of the SocketBase.

	Socket& operator = (const Socket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's socket context and
		/// attaches the socket context from the other socket and
		/// increments the reference count of the socket context.
		
	~Socket();
		/// Destroys the Socket and releases the socket context.

	SocketBase* base() const;
		/// Returns the SocketBase for this socket.
		
	void connect(const Address& address);
	bool shutdown();
	void close();

	void bind(const Address& address);
	void listen(int backlog = 64);
		
	int send(const char* data, int len, int flags = 0);
	//int send(const char* data, int len, const Address& peerAddress, int flags = 0);
	//int send(const IPacket& packet, int flags = 0);
	//int send(const IPacket& packet, const Address& peerAddress, int flags = 0);
	
	Address address() const;
		/// The locally bound address.

	Address peerAddress() const;
		/// The connected peer address.

	net::TransportType transport() const;
		/// The transport protocol: TCP, UDP or SSLTCP.
		/// See TransportType definition.
		
	void duplicate();
	void release();
	int refCount() const;

protected:
	SocketBase* _base;
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
	//assert(_stream->data == NULL ||
	//	static_cast<Stream*>(_stream->data) == this);
	//_stream->data = this;

	// TODO: Close if needed.	
	//traceL("Stream", this) << "Destroying: Send Close" << endl;
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
		//traceL("Stream", this) << "Destroying: Handle" << endl;
		delete _handle;
		_handle = NULL;
	}
	//traceL("Stream", this) << "Destroying: OK" << endl;
	*/
	/*
	


void Stream::close() 
{	
	//traceL("Stream", this) << "Send Close" << endl;
	assert(0);
	// afterClose not always getting called
    //uv_close((uv_handle_t*)stream(), NULL); //Stream::afterClose
	//delete this;
	//onClose();
}
	//uv_close((uv_handle_t*)req->handle, afterClose);
	//Stream* io = static_cast<Stream*>(req->data);
	//io->close();

void Stream::afterClose(uv_handle_t* peer) 
{	
	//traceL("Stream") << "After Close: " << peer << endl;
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