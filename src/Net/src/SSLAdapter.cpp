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


#include "Sourcey/Net/SSLAdapter.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Logger.h"
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>

using namespace std;


namespace scy {
namespace net {

 
SSLAdapter::SSLAdapter(net::SSLBase* socket) :
	_socket(socket),
	_ssl(nullptr),
	_readBIO(nullptr),
	_writeBIO(nullptr)
{
	traceL("SSLAdapter", this) << "Create" << endl;
}


SSLAdapter::~SSLAdapter() 
{	
	traceL("SSLAdapter", this) << "Destroy" << endl;
	if (_ssl) {
		SSL_free(_ssl);
		_ssl = nullptr;
	}
}


void SSLAdapter::init(SSL* ssl) 
{
	traceL("SSLAdapter", this) << "Init: " << ssl << endl;
	assert(_socket);
	//assert(_socket->initialized());
	_ssl = ssl;
	_readBIO = BIO_new(BIO_s_mem());
	_writeBIO = BIO_new(BIO_s_mem());
	SSL_set_bio(_ssl, _readBIO, _writeBIO);
}


void SSLAdapter::shutdown()
{
	traceL("SSLAdapter", this) << "Shutdown" << endl;
	if (_ssl)
	{        
		traceL("SSLAdapter", this) << "Shutdown SSL" << endl;

        // Don't shut down the socket more than once.
        int shutdownState = SSL_get_shutdown(_ssl);
        bool shutdownSent = (shutdownState & SSL_SENT_SHUTDOWN) == SSL_SENT_SHUTDOWN;
        if (!shutdownSent)
        {
			// A proper clean shutdown would require us to
			// retry the shutdown if we get a zero return
			// value, until SSL_shutdown() returns 1.
			// However, this will lead to problems with
			// most web browsers, so we just set the shutdown
			// flag by calling SSL_shutdown() once and be
			// done with it.
			int rc = SSL_shutdown(_ssl);
			if (rc < 0) handleError(rc);
		}
	}
}


bool SSLAdapter::initialized() const
{
	return SSL_is_init_finished(_ssl);
}


int SSLAdapter::available() const
{
	assert(_ssl);
	return SSL_pending(_ssl);
}


void SSLAdapter::addIncomingData(const char* data, size_t len) 
{
	traceL("SSLAdapter") << "Add incoming data: " << len << endl;
	BIO_write(_readBIO, data, len);
	flush();
}


void SSLAdapter::addOutgoingData(const std::string& s)
{
	addOutgoingData(s.c_str(), s.size());
}


void SSLAdapter::addOutgoingData(const char* data, size_t len) 
{
	std::copy(data, data+len, std::back_inserter(_bufferOut));
}


void SSLAdapter::flush() 
{
	traceL("SSLAdapter") << "Flushing" << endl;

	if (!initialized()) {
		int r = SSL_connect(_ssl);
		if (r < 0) {
			traceL("SSLAdapter") << "Flush: Handle error" << endl;
			handleError(r);
		}
		return;
	}
	
	// Read any decrypted SSL data from the read BIO
	// NOTE: Overwriting the socket's raw SSL recv buffer
	int nread = 0;
	while ((nread = SSL_read(_ssl, _socket->_buffer.data(), _socket->_buffer.capacity())) > 0) {
		//_socket->_buffer.limit(nread);
		_socket->onRecv(mutableBuffer(_socket->_buffer.data(), nread));
	}
	
	// Flush any pending outgoing data
	if (SSL_is_init_finished(_ssl)) { 
		if (_bufferOut.size() > 0) {
			int r = SSL_write(_ssl, &_bufferOut[0], _bufferOut.size()); // causes the write_bio to fill up (which we need to flush)
			if (r < 0) {
				handleError(r);
			}
			_bufferOut.clear();
			flushWriteBIO();
		}
	}
}


void SSLAdapter::flushWriteBIO() 
{
	// flushes encrypted data 
	char buffer[1024*16]; // optimize!
	int nread = 0;
	while ((nread = BIO_read(_writeBIO, buffer, sizeof(buffer))) > 0) {
		
		// Write encrypted data to the socket stream output
		_socket->write(buffer, nread);
	}
}


void SSLAdapter::handleError(int rc)
{
	if (rc >= 0) return;
	int error = SSL_get_error(_ssl, rc);	
	switch (error)
	{
	case SSL_ERROR_ZERO_RETURN:
		return;
	case SSL_ERROR_WANT_READ:
		flushWriteBIO();
 		break;
	case SSL_ERROR_WANT_WRITE:
		// TODO
		assert(0);
 		break;
	case SSL_ERROR_WANT_CONNECT: 
	case SSL_ERROR_WANT_ACCEPT:
	case SSL_ERROR_WANT_X509_LOOKUP:
		// should not occur
		assert(0);
 		break;
	default:
		char buffer[256];
		ERR_error_string_n(ERR_get_error(), buffer, sizeof(buffer));
		std::string msg(buffer);
		throw std::runtime_error("SSL connection error: " + msg);
 		break;
	}
}


} } // namespace scy::net
