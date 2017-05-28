///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
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
#include "scy/net/net.h"
#include "scy/stream.h"
#include "scy/handle.h"


namespace scy {
namespace net {


/// TCP socket implementation.
class Net_API TCPSocket : public Stream<uv_tcp_t>, public net::Socket
{
public:
    typedef std::shared_ptr<TCPSocket> Ptr;
    typedef std::vector<Ptr> Vec;

    TCPSocket(uv::Loop* loop = uv::defaultLoop());
    virtual ~TCPSocket();

    virtual bool shutdown();
    virtual void close() override;

    virtual void connect(const net::Address& peerAddress) override;
    virtual void connect(const std::string& host, uint16_t port) override;

    virtual ssize_t send(const char* data, size_t len, int flags = 0) override;
    virtual ssize_t send(const char* data, size_t len, const net::Address& peerAddress, int flags = 0) override;

    virtual void bind(const net::Address& address, unsigned flags = 0) override;
    virtual void listen(int backlog = 64) override;

    virtual void acceptConnection();

    bool setReusePort();
    bool setNoDelay(bool enable);
    bool setKeepAlive(bool enable, int delay);
    bool setSimultaneousAccepts(bool enable);

    void setMode(SocketMode mode);
    const SocketMode mode() const;

    virtual void setError(const scy::Error& err) override;
    const scy::Error& error() const override;

    /// Returns true if the native socket handle is closed.
    virtual bool closed() const override;

    /// Returns the IP address and port number of the socket.
    /// A wildcard address is returned if the socket is not connected.
    net::Address address() const override;

    /// Returns the IP address and port number of the peer socket.
    /// A wildcard address is returned if the socket is not connected.
    net::Address peerAddress() const override;

    /// Returns the TCP transport protocol.
    net::TransportType transport() const override;

    virtual uv::Loop* loop() const override;

    virtual void* self() override;

    Signal<void(const net::TCPSocket::Ptr&)> AcceptConnection;

public:
    virtual void onConnect();
    virtual void onRead(const char* data, size_t len) override;
    virtual void onRecv(const MutableBuffer& buf);
    virtual void onError(const scy::Error& error) override;
    virtual void onClose() override;

protected:
    virtual void init() override;
    virtual void reset() override;

    SocketMode _mode;
};


} // namespace net
} // namespace scy


#endif // SCY_Net_TCPSocket_H


/// @\}
