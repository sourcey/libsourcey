///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_TCPSocket_H
#define SCY_Net_TCPSocket_H


#include "scy/net/address.h"
#include "scy/net/socket.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/types.h"
#include "scy/stream.h"
#include "scy/uv/uvpp.h"


namespace scy {
namespace net {


class SCY_EXTERN TCPSocket : public Stream, public net::Socket
{
public:
    typedef std::shared_ptr<TCPSocket> Ptr;
    typedef std::vector<Ptr> Vec;

    TCPSocket(uv::Loop* loop = uv::defaultLoop());
    virtual ~TCPSocket();

    virtual bool shutdown();
    virtual void close();

    virtual void connect(const net::Address& peerAddress);

    virtual int send(const char* data, std::size_t len, int flags = 0);
    virtual int send(const char* data, std::size_t len,
                     const net::Address& peerAddress, int flags = 0);

    virtual void bind(const net::Address& address, unsigned flags = 0);
    virtual void listen(int backlog = 64);

    virtual void acceptConnection();

    virtual void setNoDelay(bool enable);
    virtual void setKeepAlive(int enable, unsigned int delay);

    virtual uv::Loop* loop() const;

    void setMode(SocketMode mode);
    const SocketMode mode() const;

    void setError(const scy::Error& err);
    const scy::Error& error() const;

    /// Returns true if the native socket handle is closed.
    virtual bool closed() const;

    /// Returns the IP address and port number of the socket.
    /// A wildcard address is returned if the socket is not connected.
    net::Address address() const;

    /// Returns the IP address and port number of the peer socket.
    /// A wildcard address is returned if the socket is not connected.
    net::Address peerAddress() const;

    /// Returns the TCP transport protocol.
    net::TransportType transport() const;

#ifdef _WIN32
    void setSimultaneousAccepts(bool enable);
#endif

    Signal<void(const net::TCPSocket::Ptr&)> AcceptConnection;

public:
    virtual void onConnect(uv_connect_t* handle, int status);
    virtual void onAcceptConnection(uv_stream_t* handle, int status);
    virtual void onRead(const char* data, std::size_t len);
    virtual void onRecv(const MutableBuffer& buf);
    virtual void onError(const scy::Error& error);
    virtual void onClose();

protected:
    virtual void init();

    // std::unique_ptr<uv_connect_t> _connectReq;
    uv_connect_t* _connectReq;
    SocketMode _mode;
};


} // namespace net
} // namespace scy


#endif // SCY_Net_TCPSocket_H


/// @\}
