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
	

Socket::Socket(SocketAdapter* adapter) :
	_base(NULL), 
	_adapter(adapter ? 
		adapter : new SocketAdapter(this))
{
}


Socket::Socket(SocketBase* base, bool shared, SocketAdapter* adapter) :
	_base(base), 
	_adapter(adapter ? 
		adapter : new SocketAdapter(this))
{
	if (_base) {
		_base->addAdapter(_adapter, shared);
	}
}


Socket::Socket(const Socket& socket, SocketAdapter* adapter) :
	_base(socket._base), 
	_adapter(adapter ? 
		adapter : new SocketAdapter(this))
{	
	if (_base) {
		_base->addAdapter(_adapter, true);
	}
}

	
Socket& Socket::operator = (const Socket& socket)
{
	return assign(socket._base, true);
}


Socket& Socket::assign(SocketBase* base, bool shared)
{	
	if (_base != base) {
		if (_base) _base->removeAdapter(_adapter);
		_base = base;
		if (_base) _base->addAdapter(_adapter, shared);
	}
	return *this;
}


Socket::~Socket()
{
	if (_base)
		_base->removeAdapter(_adapter);
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
	return _adapter->send(data, len, flags);
}


int Socket::send(const char* data, int len, const Address& peerAddress, int flags)
{
	return _adapter->send(data, len, peerAddress, flags);
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
		//traceL("Socket", this) << "Send IPacket: " << buf.size() << endl;	
		return send(buf.begin(), buf.size(), peerAddress, flags);
	}
}


void Socket::send(void*, IPacket& packet)
{
	send(packet);
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
	

bool Socket::connected() const 
{ 
	return _base->connected();
}
	

SocketBase& Socket::base() const
{
	assert(_base);
	return *_base;
}
	

SocketAdapter& Socket::adapter() const
{
	assert(_adapter);
	return *_adapter;
}


void Socket::replaceAdapter(SocketAdapter* adapter)
{
	traceL("Socket", this) << "Replacing adapter: " 
		<< _adapter << ": " << adapter << endl;
	assert(_adapter);

	// NOTE: Just swap the SocketAdapter pointers as
	// we don't want to modify the container since we
	// may be inside the old adapter's callback scope.
	_base->swapAdapter(_adapter, adapter);

	// Defer deletion to the next iteration.
	// The old adapter will receive no more callbacks.	
	deleteLater<SocketAdapter>(_adapter);
	
	// Assign the new adapter pointer
	_adapter = adapter;
	_adapter->socket = this;
}


int Socket::isNull() const
{
	return _base == 0;
}


//
// SocketAdapter methods
//

SocketAdapter::SocketAdapter(Socket* socket, int priority) : 
	socket(socket), priority(priority)
{
	//traceL("SocketAdapter", this) << "Creating" << endl;	
}
	

SocketAdapter::~SocketAdapter()
{
	//traceL("SocketAdapter", this) << "Destroying" << endl;	
}


void SocketAdapter::onSocketConnect()
{
	//traceL("SocketAdapter", this) << "On Connect: " << socket->Connect.refCount() << endl;	
	socket->Connect.emit(socket);
}


void SocketAdapter::onSocketRecv(Buffer& buf, const Address& peerAddr)
{
	//traceL("SocketAdapter", this) << "On Recv: " << socket->Recv.refCount() << endl;	
	SocketPacket packet(*socket, buf, peerAddr);
	socket->Recv.emit(socket, packet);
}


void SocketAdapter::onSocketError(const Error& error) //const Error& error
{
	//traceL("SocketAdapter", this) << "On Error: " << socket->Error.refCount() << ": " << message << endl;	syserr, message
	socket->Error.emit(socket, error);
}


void SocketAdapter::onSocketClose()
{
	//traceL("SocketAdapter", this) << "On Close: " << socket->Close.refCount() << endl;	
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


bool SocketAdapter::compareProiroty(const SocketAdapter* l, const SocketAdapter* r) 
{
	return l->priority > r->priority;
}


//
// SocketBase
//


SocketBase::SocketBase() : 
	CountedObject(new DeferredDeleter<SocketBase>()),
	_connected(false),
	_insideCallback(false)
{
	//traceL("SocketAdapter", this) << "Creating" << endl;	
}


SocketBase::~SocketBase()
{
	//traceL("SocketAdapter", this) << "Destroying" << endl;	

	// The socket base destructor never be called
	// from inside a callback.
	// The grabage collector implementation should
	// ensure this never occurs.
	assert(!_insideCallback && "destructor scope error");
}
	

void SocketBase::addAdapter(SocketAdapter* adapter, bool shared) 
{
	//traceL("SocketBase", this) << "Duplicating socket: " << &adapter << endl;
	_adapters.push_back(adapter);		
	sortAdapters();
	if (shared)
		duplicate();
	//traceL("SocketBase", this) << "Duplicated socket: " << &adapter << endl;
}


void SocketBase::removeAdapter(SocketAdapter* adapter)  
{	
	// TODO: Ensure socket destruction when released?
	for (vector<SocketAdapter*>::iterator it = _adapters.begin(); it != _adapters.end(); ++it) {
		if (*it == adapter) {
			//traceL("SocketBase", this) << "Releasing socket: " << &adapter << endl;
			_adapters.erase(it);
			sortAdapters();
			release();
			return;
		}
	}
	assert(0 && "unknown socket adapter");
}


void SocketBase::swapAdapter(SocketAdapter* a, SocketAdapter* b)
{
	for (vector<SocketAdapter*>::iterator it = _adapters.begin(); it != _adapters.end(); ++it) {
		if ((*it) == a) {
			*it = b;
			traceL("SocketBase", this) << "swapAdapter: " << a << ": " << b << endl;
			return;
		}
	}
	assert(0 && "unknown socket adapter");
}


void SocketBase::sortAdapters()  
{	
	sort(_adapters.begin(), _adapters.end(), SocketAdapter::compareProiroty);
}
	

bool SocketBase::connected() const 
{ 
	return _connected;
}


void SocketBase::emitConnect() 
{
	_insideCallback = true;
	_connected = true;
	for (int i = 0; i < _adapters.size(); i++) 
		_adapters[i]->onSocketConnect();
	_insideCallback = false;
}


void SocketBase::emitRecv(Buffer& buf, const Address& peerAddr)
{
	_insideCallback = true;
	for (int i = 0; i < _adapters.size(); i++)
		_adapters[i]->onSocketRecv(buf, peerAddr);
	_insideCallback = false;
}


void SocketBase::emitError(const Error& error)
{
	_insideCallback = true;
	_connected = false;
	for (int i = 0; i < _adapters.size(); i++) 
		_adapters[i]->onSocketError(error);
	_insideCallback = false;
}


void SocketBase::emitClose()
{
	_insideCallback = true;
	_connected = false;
	for (int i = 0; i < _adapters.size(); i++) 
		_adapters[i]->onSocketClose();
	_insideCallback = false;
}


} } // namespace scy::net






		
	//SocketAdapter* oldAdapter = _adapter;
	//_base->addAdapter(adapter, true);
	//if (_adapter) {
	//_base->removeAdapter(_adapter);
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