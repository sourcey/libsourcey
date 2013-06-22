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


#include "Sourcey/UV/SSLBuffer.h"
#include "Sourcey/UV/SSLSocket.h"
#include "Sourcey/Logger.h"
#include <vector>
#include <iterator>
#include <algorithm>


using namespace std;


namespace scy {
namespace UV {

 
SSLBuffer::SSLBuffer(SSLSocket* socket) :
	_socket(socket),
	_ssl(NULL),
	_read_bio(NULL),
	_write_bio(NULL)
{
}


SSLBuffer::~SSLBuffer() 
{	
	if (_ssl) {
		SSL_free(_ssl);
		_ssl = NULL;
	}
}


void SSLBuffer::init(SSL* ssl) 
{
	assert(_socket);
	assert(_socket->initialized());
	_ssl = ssl;
	_read_bio = BIO_new(BIO_s_mem());
	_write_bio = BIO_new(BIO_s_mem());
	SSL_set_bio(_ssl, _read_bio, _write_bio);
}


void SSLBuffer::shutdown()
{
	LogTrace("SSLContext", this) << "Shutdown" << endl;
	if (_ssl)
	{        
		LogTrace("SSLContext", this) << "Shutdown SSL" << endl;

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


int SSLBuffer::available() const
{
	poco_check_ptr (_ssl);
	return SSL_pending(_ssl);
}


void SSLBuffer::addEncryptedData(const char* data, size_t len) 
{
	Log("trace") << "SSLBuffer: addEncryptedData: " << len << endl;
	BIO_write(_read_bio, data, len);
	update();
}


void SSLBuffer::addApplicationData(const std::string& s)
{
	addApplicationData(s.c_str(), s.size());
}


void SSLBuffer::addApplicationData(const char* data, size_t len) 
{
	std::copy(data, data+len, std::back_inserter(_buffer_out));
}


void SSLBuffer::update() 
{
	Log("trace") << "SSLBuffer: update" << endl;
	if (!SSL_is_init_finished(_ssl)) {
		int r = SSL_connect(_ssl);
		if (r < 0) {
			Log("trace") << "SSLBuffer: update: handleError" << endl;
			handleError(r);
		}
	}
	else {
		Log("trace") << "SSLBuffer: update: Reading" << endl;
		char in_buf[1024 * 16];
		int bytes_read = 0;
		while ((bytes_read = SSL_read(_ssl, in_buf, sizeof(in_buf))) > 0) {
			_socket->onRecv(in_buf, bytes_read);
		}      
	}
	checkOutgoingApplicationData();
}


// flushes encrypted data 
void SSLBuffer::flushWriteBIO() 
{
	Log("trace") << "SSLBuffer: flushWriteBIO" << endl;
	char buffer[1024*16];
	int bytes_read = 0;
	while ((bytes_read = BIO_read(_write_bio, buffer, sizeof(buffer))) > 0) {
		
		// Write encrypted data directly to the socket output
		//uv_write_t* req = new uv_write_t;
		uv_buf_t buf = uv_buf_init((char *)&buffer, bytes_read);
		int r = uv_write(&_socket->write_req, _socket->stream(), &buf, 1, NULL);
		if (r == -1) {
			_socket->setErrno(uv_last_error(_socket->loop()));
		}
	}
}


// Any of the SSL_* functions can cause a error. 
// We need to handle the SSL_ERROR_WANT_READ/WRITE
void SSLBuffer::handleError(int r)
{
	int error = SSL_get_error(_ssl, r);
	if (error == SSL_ERROR_WANT_READ) {

		Log("trace") << "SSLBuffer: handleError: SSL_ERROR_WANT_READ" << endl;
		flushWriteBIO();
	}
}


// Is there data pending in the out buffer which should send?
void SSLBuffer::checkOutgoingApplicationData() 
{
	if (SSL_is_init_finished(_ssl)) { 
		if (_buffer_out.size() > 0) {
			int r = SSL_write(_ssl, &_buffer_out[0], _buffer_out.size()); // causes the write_bio to fill up (which we need to flush)
			_buffer_out.clear();
			handleError(r);
			flushWriteBIO();
		}
	}
}


} } // namespace scy::UV


			
			//if(read_decrypted_callback) {
			//	read_decrypted_callback(in_buf, bytes_read, read_decrypted_callback_data);
			//}
		//printf("ERROR: write_tosocket error: %s\n", uv_err_name(uv_last_error(_loop)));
		//tcp->send(buf, bytes_read);
		//send direct from here
		//if(write_to_socket_callback) {
		//	write_to_socket_callback(buf, bytes_read, write_to_socket_callback_data);
		//}