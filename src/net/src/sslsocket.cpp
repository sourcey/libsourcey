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


#include "scy/net/sslsocket.h"
#include "scy/net/sslmanager.h"
#include "scy/logger.h"


using namespace std;


namespace scy {
namespace net {


#if 0
SSLSocket::SSLSocket(uv::Loop* loop) : 
    net::Socket(new SSLSocket(loop), false)
{
}


SSLSocket::SSLSocket(SSLSocket* base, bool shared) : 
    net::Socket(base, shared) 
{
}


SSLSocket::SSLSocket(const Socket& socket) : 
    net::Socket(socket)
{
    if (!dynamic_cast<SSLSocket*>(_base))
        throw std::runtime_error("Cannot assign incompatible socket");
}
    

SSLSocket& SSLSocket::base() const
{
    return static_cast<SSLSocket&>(*_base);
}
#endif


SSLSocket::SSLSocket(uv::Loop* loop) : 
    TCPSocket(loop),
    // TODO: Using client context, should assert no bind()/listen() on this socket
    _context(SSLManager::instance().defaultClientContext()), 
    _session(nullptr), 
    _sslAdapter(this)
{
    TraceLS(this) << "Create" << endl;
}


SSLSocket::SSLSocket(SSLContext::Ptr context, uv::Loop* loop) : 
    TCPSocket(loop),
    _context(context), 
    _session(nullptr), 
    _sslAdapter(this)
{
    TraceLS(this) << "Create" << endl;
}
    

SSLSocket::SSLSocket(SSLContext::Ptr context, SSLSession::Ptr session, uv::Loop* loop) : 
    TCPSocket(loop),
    _context(context), 
    _session(session), 
    _sslAdapter(this)
{
    TraceLS(this) << "Create" << endl;
}

    
SSLSocket::~SSLSocket() 
{    
    TraceLS(this) << "Destroy" << endl;
}


int SSLSocket::available() const
{
    return _sslAdapter.available();
}


void SSLSocket::close()
{
    TCPSocket::close();
}


bool SSLSocket::shutdown()
{
    TraceLS(this) << "Shutdown" << endl;
    try {
        // Try to gracefully shutdown the SSL connection
        _sslAdapter.shutdown();
    }
    catch (...) {}
    return TCPSocket::shutdown();
}


int SSLSocket::send(const char* data, std::size_t len, int flags) 
{    
    return send(data, len, peerAddress(), flags);
}


int SSLSocket::send(const char* data, std::size_t len, const net::Address& /* peerAddress */, int /* flags */) 
{    
    TraceLS(this) << "Send: " << len << endl;    
    assert(Thread::currentID() == tid());
    //assert(len <= net::MAX_TCP_PACKET_SIZE);

    if (!active()) {
        WarnL << "Send error" << endl;    
        return -1;
    }    

    //assert(initialized());
    
    // Send unencrypted data to the SSL context
    _sslAdapter.addOutgoingData(data, len);
    _sslAdapter.flush();
    return len;
}


SSLSession::Ptr SSLSocket::currentSession()
{
    if (_sslAdapter._ssl) {
        SSL_SESSION* session = SSL_get1_session(_sslAdapter._ssl);
        if (session) {
            if (_session && session == _session->sslSession()) {
                SSL_SESSION_free(session);
                return _session;
            }
            else return std::make_shared<SSLSession>(session); // new SSLSession(session);
        }
    }
    return 0;
}

    
void SSLSocket::useSession(SSLSession::Ptr session)
{
    _session = session;
}


bool SSLSocket::sessionWasReused()
{
    if (_sslAdapter._ssl)
        return SSL_session_reused(_sslAdapter._ssl) != 0;
    else
        return false;
}


net::TransportType SSLSocket::transport() const
{ 
    return net::SSLTCP; 
}


//
// Callbacks
// 

void SSLSocket::onRead(const char* data, std::size_t len)
{
    TraceLS(this) << "On SSL read: " << len << endl;

    // SSL encrypted data is sent to the SSL conetext
    _sslAdapter.addIncomingData(data, len);
    _sslAdapter.flush();
}


void SSLSocket::onConnect(uv_connect_t* handle, int status)
{
    TraceLS(this) << "On connect" << endl;
    if (status) {
        setUVError("SSL connect error", status);
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
 
    _sslAdapter.init(ssl);
    _sslAdapter.flush();

    //emitConnect();
    onSocketConnect();
    TraceLS(this) << "On connect: OK" << endl;
}


} } // namespace scy::net