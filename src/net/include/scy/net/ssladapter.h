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


#ifndef SCY_Net_SSLAdapter_H
#define SCY_Net_SSLAdapter_H


#include "scy/uv/uvpp.h"
#include "scy/net/address.h"
#include "scy/net/types.h"
 
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <string>
#include <vector>


namespace scy {
namespace net {

 
class SSLSocket;
class SSLAdapter 
    /// A wrapper for the OpenSSL SSL connection context
    /// TODO: Decouple from SSLSocket implementation
{
public:
    SSLAdapter(net::SSLSocket* socket);
    ~SSLAdapter();

    void init(SSL* ssl = nullptr);
        // Initializes the BIO buffers from the given SSL pointer.

    bool initialized() const;
        // Returns true when the handshake is complete.
        
    int available() const;
        // Returns the number of bytes available in 
        // the SSL buffer for immediate reading.
    
    void shutdown();
        // Issues an orderly SSL shutdown.

    void flush();
        // Flushes the SSL read/write buffers.

    void addIncomingData(const char* data, std::size_t len);
    void addOutgoingData(const std::string& data);
    void addOutgoingData(const char* data, std::size_t len);

protected:
    void handleError(int rc);

    void flushWriteBIO();

protected:
    friend class net::SSLSocket;

    net::SSLSocket* _socket;
    SSL* _ssl;
    BIO* _readBIO; // The incoming buffer we write encrypted SSL data into
    BIO* _writeBIO; // The outgoing buffer we write to the socket
    std::vector<char> _bufferOut; // The outgoing payload to be encrypted and sent
};


} } // namespace scy::net


#endif // SCY_Net_SSLAdapter_H
