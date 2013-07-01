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


#include "Sourcey/Net/SSLContext.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Logger.h"
#include <vector>
#include <iterator>
#include <algorithm>


using namespace std;


namespace scy {
namespace net {

 
SSLContext::SSLContext(net::SSLBase* socket) :
	_socket(socket),
	_ssl(NULL),
	_readBIO(NULL),
	_writeBIO(NULL)
{
	traceL("SSLContext", this) << "Creating" << endl;
}


SSLContext::~SSLContext() 
{	
	traceL("SSLContext", this) << "Destroying" << endl;
	if (_ssl) {
		SSL_free(_ssl);
		_ssl = NULL;
	}
}


void SSLContext::init(SSL* ssl) 
{
	traceL("SSLContext", this) << "Init: " << ssl << endl;
	assert(_socket);
	//assert(_socket->initialized());
	_ssl = ssl;
	_readBIO = BIO_new(BIO_s_mem());
	_writeBIO = BIO_new(BIO_s_mem());
	SSL_set_bio(_ssl, _readBIO, _writeBIO);
}


void SSLContext::shutdown()
{
	traceL("SSLBase", this) << "Shutdown" << endl;
	if (_ssl)
	{        
		traceL("SSLBase", this) << "Shutdown SSL" << endl;

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


int SSLContext::available() const
{
	assert(_ssl);
	return SSL_pending(_ssl);
}


void SSLContext::addEncryptedData(const char* data, size_t len) 
{
	Log("trace") << "SSLContext: addEncryptedData: " << len << endl;
	BIO_write(_readBIO, data, len);
	update();
}


void SSLContext::addApplicationData(const std::string& s)
{
	addApplicationData(s.c_str(), s.size());
}


void SSLContext::addApplicationData(const char* data, size_t len) 
{
	std::copy(data, data+len, std::back_inserter(_bufferOut));
}


void SSLContext::update() 
{
	Log("trace") << "SSLContext: update" << endl;
	if (!SSL_is_init_finished(_ssl)) {
		int r = SSL_connect(_ssl);
		if (r < 0) {
			Log("trace") << "SSLContext: update: handleError" << endl;
			handleError(r);
		}
	}
	else {
		Log("trace") << "SSLContext: update: Reading" << endl;
		// Reuse the unencrypted SSL recv buffer
		
		int nread = 0;
		while ((nread = SSL_read(_ssl, _socket->_buffer.begin(), _socket->_buffer.capacity())) > 0) {
			_socket->_buffer.size(nread);
			_socket->onRecv(_socket->_buffer);
		}
		/*
		char in_buf[1024 * 16];
		int nread = 0;
		while ((nread = SSL_read(_ssl, in_buf, sizeof(in_buf))) > 0) {
			_socket->onRead(in_buf, nread);
		}    
		*/
	}
	checkOutgoingApplicationData();
}


void SSLContext::flushWriteBIO() 
{
	// flushes encrypted data 
	Log("trace") << "SSLContext: flushWriteBIO" << endl;
	char buffer[1024*16]; // optimize!
	int nread = 0;
	while ((nread = BIO_read(_writeBIO, buffer, sizeof(buffer))) > 0) {
		
		// Write encrypted data directly to the socket output
		_socket->write(buffer, nread);
		/*
		//uv_write_t* req = new uv_write_t;
		uv_buf_t buf = uv_buf_init((char *)&buffer, nread);
		int r = uv_write(&_socket->_writeReq, _socket->handle<uv_stream_t>(), &buf, 1, NULL);
		if (r == -1) {
			_socket->setError(uv_last_error(_socket->loop()));
		}
		*/
	}
}


void SSLContext::handleError(int r)
{
	// Any of the SSL_* functions can cause a error. 
	// We need to handle the SSL_ERROR_WANT_READ/WRITE
	int error = SSL_get_error(_ssl, r);
	if (error == SSL_ERROR_WANT_READ) {

		Log("trace") << "SSLContext: handleError: SSL_ERROR_WANT_READ" << endl;
		flushWriteBIO();
	}
}


void SSLContext::checkOutgoingApplicationData() 
{
	// Is there data pending in the out buffer which should send?
	if (SSL_is_init_finished(_ssl)) { 
		if (_bufferOut.size() > 0) {
			int r = SSL_write(_ssl, &_bufferOut[0], _bufferOut.size()); // causes the write_bio to fill up (which we need to flush)
			_bufferOut.clear();
			handleError(r);
			flushWriteBIO();
		}
	}
}


} } // namespace scy::uv