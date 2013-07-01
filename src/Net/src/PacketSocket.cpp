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

//#include "Sourcey/Net/TCPSocket.h"


using namespace std;


namespace scy {
namespace net {


/*
Socket::Socket(SocketBase* base):
	_base(base)
{
	assert(_base);
}


Socket::Socket(const SocketHandle& socket):
	_base(socket._base)
{
	assert(_base);

	_base->duplicate();
}

	
Socket& Socket::operator = (const SocketHandle& socket)
{
	if (&socket != this)
	{
		if (_base) _base->release();
		_base = socket._base;
		if (_base) _base->duplicate();
	}
	return *this;
}


Socket::~Socket()
{
	_base->release();
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


} } // namespace scy::uv




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