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


#ifndef SOURCEY_UV_SSLBuffer_H
#define SOURCEY_UV_SSLBuffer_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Types.h"

/*
#include "Sourcey/UV/SSLSocket.h"
#include <uv.h>
#include <vector>
#include <iterator>
#include <algorithm>
*/

 
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <string>
#include <vector>
 
  // Wrapper for memory bios + non-blocking / async sockets
 
  // Usage, based on libuv (libevent or libev should be something similar):
 
  // 1) resolve a host, connect with tcp
  // 2) once connected:
  //      - create a SSL*
  //      - call: _sslBuffer.init(...)
  //      - call: SSL_set_connect_state(ssl)
  //      - call: SSL_do_handshake()
  //      - call: _sslBuffer.update()
  // 3) when you receive data from the socket, add this to the buffer:
  //      - call: _sslBuffer.addEncryptedData()
  //      - call: _sslBuffer.update()
 
 
  // - Whenever you need to write to the socket the 'write to socket callback' is called 
  // - Whenever there is decrypted data available the 'read decrypted data callback' is called
 
  // - For an example see: https://gist.github.com/gists/4000573/edit
 

using namespace std;


namespace scy {
namespace UV {

 
class SSLSocket; 
class SSLBuffer 
{
public:
	SSLBuffer(SSLSocket* socket);
	~SSLBuffer();

	void init(SSL* ssl = NULL);
	
	void shutdown();
		/// Shuts down the connection by attempting
		/// an orderly SSL shutdown, then actually
		/// shutting down the TCP connection.

	void update();
	void addEncryptedData(const char* data, size_t len);
	void addApplicationData(const std::string& data);
	void addApplicationData(const char* data, size_t len);
		
	int available() const;
		/// Returns the number of bytes available from the
		/// SSL buffer for immediate reading.

private:
	void handleError(int r);
	void checkOutgoingApplicationData();
	void flushWriteBIO();

private:
	SSLSocket* _socket;
	SSL* _ssl;
	BIO* _read_bio; // SSL reads from this buffer (so we write encrypted data into this)
	BIO* _write_bio; // SSL writes into this buffer (so we need to send this to the server)
	std::vector<char> _buffer_out; // application data, what needs to be encrypted and send to server
	SSLSocket* _tcp;

	friend class SSLSocket;
};


} } // namespace scy::UV


#endif // SOURCEY_UV_SSLBuffer_H




 
//typedef void (*cb_write_to_socket)(const char* data, size_t len, void* userdata);
//typedef void (*cb_read_decrypted)(const char* data, size_t len, void* userdata);