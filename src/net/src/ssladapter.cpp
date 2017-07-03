///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/ssladapter.h"
#include "scy/logger.h"
#include "scy/net/sslmanager.h"
#include "scy/net/sslsocket.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace std;


namespace scy {
namespace net {


SSLAdapter::SSLAdapter(net::SSLSocket* socket)
    : _socket(socket)
    , _ssl(nullptr)
    , _readBIO(nullptr)
    , _writeBIO(nullptr)
{
    // LTrace("Create")
}


SSLAdapter::~SSLAdapter()
{
    // LTrace("Destroy")
    if (_ssl) {
        SSL_free(_ssl);
        _ssl = nullptr;
    }
    // LTrace("Destroy: OK")
}


void SSLAdapter::initClient()
{
    // LTrace("Init client")
    assert(_socket);
    if (!_socket->context())
        _socket->useContext(SSLManager::instance().defaultClientContext());
    assert(!_socket->context()->isForServerUse());

    _ssl = SSL_new(_socket->context()->sslContext());

    // TODO: Improve automatic SSL session handling.
    // Maybe add a stored session to the network manager.
    if (_socket->currentSession())
        SSL_set_session(_ssl, _socket->currentSession()->sslSession());

    _readBIO = BIO_new(BIO_s_mem());
    _writeBIO = BIO_new(BIO_s_mem());
    SSL_set_bio(_ssl, _readBIO, _writeBIO);
    SSL_set_connect_state(_ssl);
    SSL_do_handshake(_ssl);
}


void SSLAdapter::initServer() //(SSL* ssl)
{
    // LTrace("Init server")
    assert(_socket);
    if (!_socket->context())
        _socket->useContext(SSLManager::instance().defaultServerContext());
    assert(_socket->context()->isForServerUse());

    _ssl = SSL_new(_socket->context()->sslContext());
    _readBIO = BIO_new(BIO_s_mem());
    _writeBIO = BIO_new(BIO_s_mem());
    SSL_set_bio(_ssl, _readBIO, _writeBIO);
    SSL_set_accept_state(_ssl);
    SSL_do_handshake(_ssl);
}


void SSLAdapter::shutdown()
{
    // LTrace("Shutdown")
    if (_ssl) {
        // LTrace("Shutdown SSL")

        // Don't shut down the socket more than once.
        int shutdownState = SSL_get_shutdown(_ssl);
        bool shutdownSent =
            (shutdownState & SSL_SENT_SHUTDOWN) == SSL_SENT_SHUTDOWN;
        if (!shutdownSent) {
            // A proper clean shutdown would require us to
            // retry the shutdown if we get a zero return
            // value, until SSL_shutdown() returns 1.
            // However, this will lead to problems with
            // most web browsers, so we just set the shutdown
            // flag by calling SSL_shutdown() once and be
            // done with it.
            int rc = SSL_shutdown(_ssl);
            if (rc < 0)
                handleError(rc);
        }
    }
}


bool SSLAdapter::initialized() const
{
    return !!_ssl;
}


bool SSLAdapter::ready() const
{
    return _ssl && SSL_is_init_finished(_ssl);
}


int SSLAdapter::available() const
{
    assert(_ssl);
    return SSL_pending(_ssl);
}


void SSLAdapter::addIncomingData(const char* data, size_t len)
{
    // LTrace("Add incoming data: ", len)
    assert(_readBIO);
    BIO_write(_readBIO, data, (int)len);
    flush();
}


void SSLAdapter::addOutgoingData(const std::string& s)
{
    addOutgoingData(s.c_str(), s.size());
}


void SSLAdapter::addOutgoingData(const char* data, size_t len)
{
    std::copy(data, data + len, std::back_inserter(_bufferOut));
}


void SSLAdapter::handshake()
{
    int r = SSL_do_handshake(_ssl);
    if (r < 0)
        handleError(r);
}


void SSLAdapter::flush()
{
    LTrace("Flushing")

    // Keep trying to handshake until initialized
    if (!ready())
        return handshake();

    // Read any decrypted remote data from SSL and emit to the app
    flushReadBIO();

    // Write any local data to SSL for excryption
    if (_bufferOut.size() > 0) {
        int r = SSL_write(_ssl, &_bufferOut[0], (int)_bufferOut.size());
        if (r < 0) {
            handleError(r);
        }
        _bufferOut.clear();
        // flushWriteBIO();
    }

    // Send any encrypted data from SSL to the remote peer
    flushWriteBIO();
}


void SSLAdapter::flushReadBIO()
{
    size_t npending = BIO_ctrl_pending(_readBIO);
    if (npending > 0) {
        int nread;
        char buffer[MAX_TCP_PACKET_SIZE]; // TODO: allocate only npending bytes
        while ((nread = SSL_read(_ssl, buffer, MAX_TCP_PACKET_SIZE)) > 0) {
            _socket->onRecv(mutableBuffer(buffer, nread));
        }
    }
}


void SSLAdapter::flushWriteBIO()
{
    size_t npending = BIO_ctrl_pending(_writeBIO);
    if (npending > 0) {
        char buffer[MAX_TCP_PACKET_SIZE]; // TODO: allocate only npending bytes
        int nread = BIO_read(_writeBIO, buffer, MAX_TCP_PACKET_SIZE);
        if (nread > 0) {
            _socket->write(buffer, nread);
        }
    }
}


void SSLAdapter::handleError(int rc)
{
    if (rc >= 0)
        return;
    int error = SSL_get_error(_ssl, rc);
    switch (error) {
        case SSL_ERROR_ZERO_RETURN:
            // LTrace("SSL_ERROR_ZERO_RETURN")
            return;
        case SSL_ERROR_WANT_READ:
            // LTrace("SSL_ERROR_WANT_READ")
            flushWriteBIO();
            break;
        case SSL_ERROR_WANT_WRITE:
            // LTrace("SSL_ERROR_WANT_WRITE")
            assert(0 && "not implemented");
            break;
        case SSL_ERROR_WANT_CONNECT:
        case SSL_ERROR_WANT_ACCEPT:
        case SSL_ERROR_WANT_X509_LOOKUP:
            assert(0 && "should not occur");
            break;
        default:
            char buffer[256];
            ERR_error_string_n(ERR_get_error(), buffer, sizeof(buffer));
            std::string msg(buffer);
            // throw std::runtime_error("SSL connection failed: " + msg);
            _socket->setError("SSL connection failed: " + msg);
            break;
    }
}


} // namespace net
} // namespace scy


/// @\}
