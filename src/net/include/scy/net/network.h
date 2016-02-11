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


#ifndef SCY_Net_Network_H
#define SCY_Net_Network_H


#include <cstdint>
#include "scy/uv/uvpp.h"

#include "scy/net/address.h"
#include <vector>


namespace scy {
namespace net {
    

//
// Network Interfaces
//


inline void getNetworkInterfaces(std::vector<net::Address>& hosts)
{    
    uv_interface_address_t *info;
    int count, i;

    uv_interface_addresses(&info, &count);
    i = count;

    while (i--) {
        uv_interface_address_t iface = info[i];
        hosts.push_back(net::Address(reinterpret_cast<const sockaddr*>(&iface.address.address4), 16));
    }

    uv_free_interface_addresses(info, count);
}


//
/// DNS Resolver
//
// Example: 
//
// static void onDNSResult(const net::DNSResult& result)
// {
//     do something with result
// }
// net::resolveDNS("google.com", 80, onDNSResult);


struct DNSResult
{
    std::string host;
        // The host to resolve

    std::uint16_t port;
        // The host port to resolve

    net::Address addr;
        // The resolved address

    struct addrinfo* info;
        // The libuv uv_getaddrinfo result

    struct addrinfo* hints;
        // libuv uv_getaddrinfo hints (optional)
    
    std::function<void(const DNSResult&)> callback;
        // Result callback function

    void* opaque;
        // Client data pointer

    enum Status {
        None,
        Resolving,
        Success,
        Failed
    } status;

    bool resolving() const { return status == Resolving; }
    bool success() const { return status == Success; }
    bool failed() const { return status == Failed; }
    bool complete() const { return status == Success || status == Failed; }

    DNSResult() : info(nullptr), hints(nullptr), opaque(nullptr), status(None) {}
};


inline void onDNSResolved(uv_getaddrinfo_t* handle, int status, struct addrinfo* res)
{    
    // Check that res is not NULL.
    // Adding this check after receiving some weird late callbacks with NULL res.
    if (!res) return;

    net::Address resolved(res->ai_addr, 16);
    traceL("Network") << "DNS resolved: " << resolved << std::endl;

    DNSResult* dns = reinterpret_cast<DNSResult*>(handle->data);    
    dns->status = status == 0 ? DNSResult::Success : DNSResult::Failed;
    dns->info = res;
    dns->addr.swap(resolved);
    
    dns->callback(*dns);

    uv_freeaddrinfo(res);    
    delete handle;
    delete dns;
}


inline bool resolveDNS(DNSResult* dns) 
{            
    //traceL("Network") << "Resolving DNS: " << dns->host << ":" << dns->port << std::endl;

    assert(dns->port);
    assert(!dns->host.empty());
    assert(dns->callback);
    dns->status = DNSResult::Resolving;
    
    uv_getaddrinfo_t* handle = new uv_getaddrinfo_t;
    handle->data = dns;
    return uv_getaddrinfo(uv_default_loop(), handle, onDNSResolved, dns->host.c_str(), util::itostr<std::uint16_t>(dns->port).c_str(), dns->hints) == 0;
}


inline bool resolveDNS(const std::string& host, std::uint16_t port, std::function<void(const DNSResult&)> callback, void* opaque = nullptr, struct addrinfo* hints = nullptr)
{        
    DNSResult* dns = new DNSResult();
    dns->host = host;
    dns->port = port;
    dns->opaque = opaque;
    dns->hints = hints;
    dns->callback = callback;
    return resolveDNS(dns);
}


} } // namespace scy::net


#endif // SCY_Net_Network_H
