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
	

Socket::Socket(SocketEmitter* emitter) :
	_base(NULL), 
	_emitter(emitter ? 
		emitter : new SocketEmitter(this))
{
}


Socket::Socket(SocketBase* base, bool shared, SocketEmitter* emitter) :
	_base(base), 
	_emitter(emitter ? 
		emitter : new SocketEmitter(this))
{
	if (_base) {
		_base->addObserver(*_emitter, shared);
	}
}


Socket::Socket(const Socket& socket, SocketEmitter* emitter) :
	_base(socket._base), 
	_emitter(emitter ? 
		emitter : new SocketEmitter(this))
{	
	if (_base) {
		_base->addObserver(*_emitter, true);
	}
}

	
Socket& Socket::operator = (const Socket& socket)
{
	return assign(socket._base, true);
}


Socket& Socket::assign(SocketBase* base, bool shared)
{	
	//traceL("Socket", this) << "Assigning: " << base << endl;	
	if (_base != base)
	{
		if (_base) _base->removeObserver(*_emitter);
		_base = base;
		if (_base) _base->addObserver(*_emitter, shared);
	}
	return *this;
}


Socket::~Socket()
{
	if (_base)
		_base->removeObserver(*_emitter);
	if (_emitter)
		delete _emitter;
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
	return _base->send(data, len, flags);
}


int Socket::send(const char* data, int len, const Address& peerAddress, int flags)
{
	return _base->send(data, len, peerAddress, flags);
}


int Socket::send(const IPacket& packet, int flags)
{
	return _base->send(packet, flags);
}


int Socket::send(const IPacket& packet, const Address& peerAddress, int flags)
{
	return _base->send(packet, peerAddress, flags);
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
	

SocketBase& Socket::base() const
{
	assert(_base);
	return *_base;
}
	

SocketEmitter& Socket::emitter() const
{
	assert(_emitter);
	return *_emitter;
}


int Socket::isNull() const
{
	return _base == 0;
}


//
// SocketObserver methods
//

SocketObserver::SocketObserver(int priority) : 
	priority(priority)
{
	//traceL("SocketObserver", this) << "Creating" << endl;	
}
	

SocketObserver::~SocketObserver()
{
	//traceL("SocketObserver", this) << "Destroying" << endl;	
}


//
// SocketEmitter methods
//

SocketEmitter::SocketEmitter(Socket* socket, int priority) : 
	SocketObserver(priority), socket(socket)
{
	//traceL("SocketEmitter", this) << "Creating" << endl;	
}
	

SocketEmitter::~SocketEmitter()
{
	//traceL("SocketEmitter", this) << "Destroying" << endl;	
}


void SocketEmitter::onSocketConnect()
{
	//traceL("SocketEmitter", this) << "On Connect: " << socket->Connect.refCount() << endl;	
	socket->Connect.emit(socket);
}


void SocketEmitter::onSocketRecv(Buffer& buf, const Address& peerAddr)
{
	//traceL("SocketEmitter", this) << "On Recv: " << socket->Recv.refCount() << endl;	
	SocketPacket packet(*socket, buf, peerAddr);
	socket->Recv.emit(socket, packet);
}


void SocketEmitter::onSocketError(int syserr, const string& message) 
{
	//traceL("SocketEmitter", this) << "On Error: " << socket->Error.refCount() << ": " << message << endl;	
	socket->Error.emit(socket, syserr, message);
}


void SocketEmitter::onSocketClose()
{
	//traceL("SocketEmitter", this) << "On Close: " << socket->Close.refCount() << endl;	
	socket->Close.emit(socket);
}


//
// SocketBase methods
//

void SocketBase::addObserver(SocketObserver& observer, bool shared) 
{
	traceL("SocketBase", this) << "Duplicating socket: " << &observer << endl;
	_observers.push_back(&observer);		
	sortObservers();
	if (shared)
		duplicate();
	traceL("SocketBase", this) << "Duplicated socket: " << &observer << endl;
}


void SocketBase::removeObserver(SocketObserver& observer)  
{	
	/// TODO: Ensure socket destruction when released?
	for (vector<SocketObserver*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		if ((*it) == &observer) {
			traceL("SocketBase", this) << "Releasing socket: " << &observer << endl;
			_observers.erase(it);
			sortObservers();
			release();
			return;
		}
	}
	assert(0 && "unknown socket observer");
}


void SocketBase::sortObservers()  
{	
	sort(_observers.begin(), _observers.end(), SocketObserver::compareProiroty);
}


int SocketBase::send(const IPacket& packet, int flags)
{
	return SocketBase::send(packet, peerAddress(), flags);
}


int SocketBase::send(const IPacket& packet, const Address& peerAddress, int flags)
{	
	// Always try to cast as RawPacket so we can try 
	// to send without copying any data.

	const RawPacket* dataPacket = dynamic_cast<const RawPacket*>(&packet);
	if (dataPacket)
		return send((const char*)dataPacket->data(), dataPacket->size(), peerAddress, flags);
	else {

		// A nocopy solution for sending IPackets is not currently
		// possible since some packets are only generated when
		// IPacket::write() is called, such as STUN and RTP packets.

		Buffer buf;
		packet.write(buf);
		traceL("SocketBase", this) << "Send IPacket: " << buf.size() << endl;	
		return send(buf.begin(), buf.size(), peerAddress, flags);
	}
}


void SocketBase::emitConnect() 
{
	for (int i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketConnect();
}


void SocketBase::emitRecv(Buffer& buf, const Address& peerAddr)
{
	for (int i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketRecv(buf, peerAddr);
}


void SocketBase::emitError(int syserr, const string& message)
{
	for (int i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketError(syserr, message);
}


void SocketBase::emitClose()
{
	for (int i = 0; i < _observers.size(); i++) 
		_observers[i]->onSocketClose();
}


} } // namespace scy::uv





	



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
		traceL("Stream", this) << "Destroying: Handle" << endl;
		delete _handle;
		_handle = NULL;
	}
	traceL("Stream", this) << "Destroying: OK" << endl;
	*/
	/*
	


void Stream::close() 
{	
	traceL("Stream", this) << "Send Close" << endl;
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
	traceL("Stream") << "After Close: " << peer << endl;
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