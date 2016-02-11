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


#ifndef SCY_Net_UDPSocket_H
#define SCY_Net_UDPSocket_H


#include "scy/uv/uvpp.h"
#include "scy/net/socket.h"    
#include "scy/net/types.h"
#include "scy/net/address.h"


namespace scy {
namespace net {

    
class UDPSocket: public net::Socket, public uv::Handle
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
    virtual int send(const char* data, std::size_t len, const net::Address& peerAddress, int flags = 0);
    
    virtual bool setBroadcast(bool flag);
    virtual bool setMulticastLoop(bool flag);
    virtual bool setMulticastTTL(int ttl);
    
    virtual net::Address address() const;
    virtual net::Address peerAddress() const;

    net::TransportType transport() const;
        /// Returns the UDP transport protocol.
            
    virtual void setError(const scy::Error& err);        
    virtual const scy::Error& error() const;

    virtual bool closed() const;
        /// Returns true if the native socket 
        /// handle is closed.

    virtual uv::Loop* loop() const;
    
    virtual void onRecv(const MutableBuffer& buf, const net::Address& address);

protected:    
    virtual void init();    
    virtual bool recvStart();
    virtual bool recvStop();

    static void onRecv(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);
    static void afterSend(uv_udp_send_t* req, int status); 
    static void allocRecvBuffer(uv_handle_t *handle, std::size_t suggested_size, uv_buf_t* buf);

    virtual void onError(const scy::Error& error);
    virtual void onClose();
    
    net::Address _peer;
    Buffer _buffer;
};


} } // namespace scy::net


#endif // SCY_Net_UDPSocket_H