///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_DNS_H
#define SCY_Net_DNS_H


#include "scy/request.h"
#include "scy/util.h"
#include "scy/net/address.h"
// #include <cstdint>
// #include <vector>


namespace scy {
namespace net {


/// Get address info request callback event.
struct GetAddrInfoEvent
{
    int status {0};
    net::Address addr;
};


/// DNS resolver request to get the IP address of a hostname.
struct GetAddrInfoReq : public uv::Request<uv_getaddrinfo_t, GetAddrInfoEvent>
{
    typedef uv::Request<uv_getaddrinfo_t, GetAddrInfoEvent> Request;

    GetAddrInfoReq()
    {
        req.data = this;
    }

    static void getAddrInfoCallback(Request::Type* req, int status, struct addrinfo* res)
    {
        auto wrap = static_cast<GetAddrInfoReq*>(req->data);
        if (wrap->callback) {
            if (status)
                wrap->callback(GetAddrInfoEvent{status});
            else
                wrap->callback(GetAddrInfoEvent{status, net::Address{res->ai_addr, 16}});
        }
        uv_freeaddrinfo(res);
        delete wrap;
    }

    bool resolve(const std::string& host, int port, uv::Loop* loop = uv::defaultLoop())
    {
        return invoke(&uv_getaddrinfo, loop, &req, &getAddrInfoCallback,
                      host.c_str(), util::itostr(port).c_str(), nullptr);
    }
};


// ///
// /// DNS Resolver
// ///
// /// Example:
// ///
// /// ```
// /// static void onDNSResult(const net::DNSResult& result)
// /// {
// ///     do something with result
// /// }
// /// net::resolveDNS("google.com", 80, onDNSResult);
// /// ```
//
//
// struct DNSResult
// {
//     /// The host to resolve
//     std::string host;
//
//     /// The host port to resolve
//     uint16_t port;
//
//     /// The resolved address
//     net::Address addr;
//
//     /// The libuv uv_getaddrinfo result
//     struct addrinfo* info;
//
//     /// libuv uv_getaddrinfo hints (optional)
//     struct addrinfo* hints;
//
//     /// Result callback function
//     std::function<void(const DNSResult&)> callback;
//
//     /// Client data pointer
//     void* opaque;
//
//     /// Resolve status
//     enum Status
//     {
//         None,
//         Resolving,
//         Success,
//         Failed
//     } status;
//
//     bool resolving() const { return status == Resolving; }
//     bool success() const { return status == Success; }
//     bool failed() const { return status == Failed; }
//     bool complete() const { return status == Success || status == Failed; }
//
//     DNSResult()
//         : info(nullptr)
//         , hints(nullptr)
//         , opaque(nullptr)
//         , status(None)
//     {
//     }
// };
//
//
// inline void onDNSResolved(uv_getaddrinfo_t* handle, int status, struct addrinfo* res)
// {
//     auto dns = reinterpret_cast<DNSResult*>(handle->data);
//     dns->info = res;
//
//     if (status == 0) {
//         net::Address resolved(res->ai_addr, 16);
//         TraceA("DNS resolved:", resolved)
//         dns->status = DNSResult::Success;
//         dns->addr.swap(resolved);
//     }
//     else {
//         TraceA("DNS failed to resolve:", dns->host, ":", dns->port)
//         dns->status = DNSResult::Failed;
//     }
//
//     dns->callback(*dns);
//
//     uv_freeaddrinfo(res);
//     delete handle;
//     delete dns;
// }
//
//
// inline bool resolveDNS(DNSResult* dns)
// {
//     // TraceA("Resolving DNS:", dns->host, ":", dns->port)
//
//     assert(dns->port);
//     assert(!dns->host.empty());
//     assert(dns->callback);
//     dns->status = DNSResult::Resolving;
//
//     uv_getaddrinfo_t* handle = new uv_getaddrinfo_t;
//     handle->data = dns;
//     return uv_getaddrinfo(
//                uv_default_loop(), handle, onDNSResolved, dns->host.c_str(),
//                util::itostr<uint16_t>(dns->port).c_str(), dns->hints) == 0;
// }
//
//
// inline bool resolveDNS(const std::string& host, uint16_t port,
//                        std::function<void(const DNSResult&)> callback,
//                        void* opaque = nullptr, struct addrinfo* hints = nullptr)
// {
//     auto dns = new DNSResult();
//     dns->host = host;
//     dns->port = port;
//     dns->opaque = opaque;
//     dns->hints = hints;
//     dns->callback = callback;
//     return resolveDNS(dns);
// }


} // namespace net
} // namespace scy


#endif // SCY_Net_DNS_H


/// @\}
