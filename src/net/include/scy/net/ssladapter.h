///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_SSLAdapter_H
#define SCY_Net_SSLAdapter_H


#include "scy/crypto/crypto.h"
#include "scy/net/address.h"
#include "scy/net/net.h"
#include "scy/handle.h"

#include <string>
#include <vector>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>


namespace scy {
namespace net {



/// A wrapper for the OpenSSL SSL connection context
///
/// TODO: Decouple from SSLSocket implementation
class Net_API SSLSocket;
class Net_API SSLAdapter
{
public:
    SSLAdapter(net::SSLSocket* socket);
    ~SSLAdapter();

    /// Initializes the SSL context as a client.
    void initClient();

    /// Initializes the SSL context as a server.
    void initServer();

    /// Returns true when SSL context has been initialized.
    bool initialized() const;

    /// Returns true when the handshake is complete.
    bool ready() const;

    /// Start/continue the SSL handshake process.
    void handshake();

    /// Returns the number of bytes available in
    /// the SSL buffer for immediate reading.
    int available() const;

    /// Issues an orderly SSL shutdown.
    void shutdown();

    /// Flushes the SSL read/write buffers.
    void flush();

    void addIncomingData(const char* data, size_t len);
    void addOutgoingData(const std::string& data);
    void addOutgoingData(const char* data, size_t len);

protected:
    void handleError(int rc);

    void flushReadBIO();
    void flushWriteBIO();

protected:
    friend class net::SSLSocket;

    net::SSLSocket* _socket;
    SSL* _ssl;
    BIO* _readBIO;  ///< The incoming buffer we write encrypted SSL data into
    BIO* _writeBIO; ///<  The outgoing buffer we write to the socket
    std::vector<char> _bufferOut; ///<  The outgoing payload to be encrypted and sent
};


} // namespace net
} // namespace scy


#endif // SCY_Net_SSLAdapter_H


/// @\}
