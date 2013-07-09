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


#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Logger.h"

#include "Poco/Exception.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/SSLManager.h"


using namespace std;


namespace scy {
namespace net {


SSLSocket::SSLSocket() : 
	net::Socket(new SSLBase, false)
{
}


SSLSocket::SSLSocket(SSLBase* base, bool shared) : 
	net::Socket(base, shared) 
{
}


SSLSocket::SSLSocket(const Socket& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<SSLBase*>(_base))
		throw Exception("Cannot assign incompatible socket");
}
	

SSLBase& SSLSocket::base() const
{
	return static_cast<SSLBase&>(*_base);
}


// -------------------------------------------------------------------
//
SSLBase::SSLBase() : 
	_context(Poco::Net::SSLManager::instance().defaultClientContext()), 
	_session(NULL), 
	_sslBuffer(this)
{
	traceL("SSLBase", this) << "Creating" << endl;
	assert(_handle);
	_handle->data = this;
	connectReq.data = this;
}


SSLBase::SSLBase(Poco::Net::Context::Ptr context) : 
	_context(context), 
	_session(NULL), 
	_sslBuffer(this)
{
	traceL("SSLBase", this) << "Creating" << endl;
	assert(_handle);
	_handle->data = this;
	connectReq.data = this;
}
	

SSLBase::SSLBase(Poco::Net::Context::Ptr context, Poco::Net::Session::Ptr session) : 
	_context(context), 
	_session(session), 
	_sslBuffer(this)
{
	traceL("SSLBase", this) << "Creating" << endl;
	assert(_handle);
	_handle->data = this;
	connectReq.data = this;
}

	
SSLBase::~SSLBase() 
{	
	traceL("SSLBase", this) << "Destroying" << endl;
}


int SSLBase::available() const
{
	assert(initialized());
	return _sslBuffer.available();
}


void SSLBase::close()
{
	traceL("SSLBase", this) << "Close" << endl;
	TCPBase::close();
}


bool SSLBase::shutdown()
{
	traceL("SSLBase", this) << "Shutdown" << endl;
	try {
		// Try to gracefully shutdown the SSL connection
		_sslBuffer.shutdown();
	}
	catch (...) {}
	return TCPBase::shutdown();
}


int SSLBase::send(const char* data, int len, int flags) 
{	
	return send(data, len, peerAddress(), flags);
}


int SSLBase::send(const char* data, int len, const net::Address& peerAddress, int flags) 
{	
	assert(len <= net::MAX_TCP_PACKET_SIZE);	
	assert(initialized());
	traceL("SSLBase", this) << "Send: " << len << endl;
	
	// Send unencrypted data to the SSL context
	_sslBuffer.addOutgoingData(data, len);
	_sslBuffer.update();
	return len;
}


Poco::Net::Session::Ptr SSLBase::currentSession()
{
	if (_sslBuffer._ssl) {
		SSL_SESSION* session = SSL_get1_session(_sslBuffer._ssl);
		if (session) {
			if (_session && session == _session->sslSession()) {
				SSL_SESSION_free(session);
				return _session;
			}
			else return new Poco::Net::Session(session);
		}
	}
	return 0;
}

	
void SSLBase::useSession(Poco::Net::Session::Ptr session)
{
	_session = session;
}


bool SSLBase::sessionWasReused()
{
	if (_sslBuffer._ssl)
		return SSL_session_reused(_sslBuffer._ssl) != 0;
	else
		return false;
}


//
// Callbacks
// 

void SSLBase::onRead(const char* data, int len)
{
	traceL("SSLBase", this) << "On SSL Read: " << len << endl;

	// SSL encrypted data is sent to the SSL conetext
	_sslBuffer.addIncomingData(data, len);
	_sslBuffer.update();
}


void SSLBase::onConnect(int status)
{
	traceL("SSLBase", this) << "On Connected" << endl;
	if (status) {
		setLastError();
		errorL("SSLBase", this) << "Connect Failed: " << error().message << endl;
		return;
	}
	else
		readStart();
 
	SSL* ssl = SSL_new(_context->sslContext());

	// TODO: Automatic SSL session handling.
	// Maybe add a stored session to the network manager.
	if (_session)
		SSL_set_session(ssl, _session->sslSession());
 
	SSL_set_connect_state(ssl);
	SSL_do_handshake(ssl);
 
	_sslBuffer.init(ssl);
	_sslBuffer.update();

	//Connect.emit(this);	
	SocketBase::emitConnect();
	traceL("SSLBase", this) << "On Connected: OK" << endl;
}


} } // namespace scy::uv





/*
void SSLBase::connect(const Address& peerAddress) 
{
	if (!_context) 
		throw Exception("Cannot connect without SSL context");

	traceL("SSLBase", this) << "Connecting to " << peerAddress << endl;
	init();
	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(peerAddress.addr());
	assert(connectReq.data == this);
	int r = uv_tcp_connect(&connectReq, (uv_tcp_t*)stream(), *addr, uv::onConnected);
	if (r) {
		uv_err_t err = uv_last_error(loop());
		setError(err);
		throw Poco::Exception(uv_strerror(err)); // TODO: make exception setError option
	}
}
*/