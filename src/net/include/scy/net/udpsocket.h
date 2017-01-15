///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_UDPSocket_H
#define SCY_Net_UDPSocket_H


#include "scy/net/address.h"
#include "scy/net/socket.h"
#include "scy/net/types.h"
#include "scy/uv/uvpp.h"


namespace scy {
namespace net {


class SCY_EXTERN UDPSocket : public net::Socket, public uv::Handle
{
public:
    typedef std::shared_ptr<UDPSocket> Ptr;
    typedef std::vector<Ptr> Vec;

    UDPSocket(uv::Loop* loop = uv::defaultLoop());
    virtual ~UDPSocket();

    virtual void connect(const net::Address& peerAddress);
    virtual void close();

    virtual void bind(const net::Address& address, unsigned flags = 0);

    virtual int send(const char* data, std::size_t len, int flags = 0);
    virtual int send(const char* data, std::size_t len,
                     const net::Address& peerAddress, int flags = 0);

    virtual bool setBroadcast(bool flag);
    virtual bool setMulticastLoop(bool flag);
    virtual bool setMulticastTTL(int ttl);

    virtual net::Address address() const;
    virtual net::Address peerAddress() const;

    /// Returns the UDP transport protocol.
    net::TransportType transport() const;

    virtual void setError(const scy::Error& err);
    virtual const scy::Error& error() const;

    /// Returns true if the native socket
    /// handle is closed.
    virtual bool closed() const;

    virtual uv::Loop* loop() const;

    virtual void onRecv(const MutableBuffer& buf, const net::Address& address);

protected:
    virtual void init();
    virtual bool recvStart();
    virtual bool recvStop();

    static void onRecv(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf,
                       const struct sockaddr* addr, unsigned flags);
    static void afterSend(uv_udp_send_t* req, int status);
    static void allocRecvBuffer(uv_handle_t* handle, std::size_t suggested_size,
                                uv_buf_t* buf);

    virtual void onError(const scy::Error& error);
    virtual void onClose();

    net::Address _peer;
    Buffer _buffer;
};


} // namespace net
} // namespace scy


#endif // SCY_Net_UDPSocket_H


/// @\}
