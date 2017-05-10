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


#include "scy/net/address.h"
#include "scy/util.h"
#include "scy/uv/handle.h"
#include <cstdint>
#include <vector>


namespace scy {
namespace net {


///
/// DNS Resolver
///
/// Example:
///
/// ```
/// static void onDNSResult(const net::DNSResult& result)
/// {
///     do something with result
/// }
/// net::resolveDNS("google.com", 80, onDNSResult);
/// ```


struct DNSResult
{
    /// The host to resolve
    std::string host;

    /// The host port to resolve
    uint16_t port;

    /// The resolved address
    net::Address addr;

    /// The libuv uv_getaddrinfo result
    struct addrinfo* info;

    /// libuv uv_getaddrinfo hints (optional)
    struct addrinfo* hints;

    /// Result callback function
    std::function<void(const DNSResult&)> callback;

    /// Client data pointer
    void* opaque;

    /// Resolve status
    enum Status
    {
        None,
        Resolving,
        Success,
        Failed
    } status;

    bool resolving() const { return status == Resolving; }
    bool success() const { return status == Success; }
    bool failed() const { return status == Failed; }
    bool complete() const { return status == Success || status == Failed; }

    DNSResult()
        : info(nullptr)
        , hints(nullptr)
        , opaque(nullptr)
        , status(None)
    {
    }
};


inline void onDNSResolved(uv_getaddrinfo_t* handle, int status, struct addrinfo* res)
{
    auto dns = reinterpret_cast<DNSResult*>(handle->data);
    dns->info = res;

    if (status == 0) {
        net::Address resolved(res->ai_addr, 16);
        TraceA("DNS resolved:", resolved)
        dns->status = DNSResult::Success;
        dns->addr.swap(resolved);
    }
    else {
        TraceA("DNS failed to resolve:", dns->host, ":", dns->port)
        dns->status = DNSResult::Failed;
    }

    dns->callback(*dns);

    uv_freeaddrinfo(res);
    delete handle;
    delete dns;
}


inline bool resolveDNS(DNSResult* dns)
{
    // TraceA("Resolving DNS:", dns->host, ":", dns->port)

    assert(dns->port);
    assert(!dns->host.empty());
    assert(dns->callback);
    dns->status = DNSResult::Resolving;

    uv_getaddrinfo_t* handle = new uv_getaddrinfo_t;
    handle->data = dns;
    return uv_getaddrinfo(
               uv_default_loop(), handle, onDNSResolved, dns->host.c_str(),
               util::itostr<uint16_t>(dns->port).c_str(), dns->hints) == 0;
}


inline bool resolveDNS(const std::string& host, uint16_t port,
                       std::function<void(const DNSResult&)> callback,
                       void* opaque = nullptr, struct addrinfo* hints = nullptr)
{
    auto dns = new DNSResult();
    dns->host = host;
    dns->port = port;
    dns->opaque = opaque;
    dns->hints = hints;
    dns->callback = callback;
    return resolveDNS(dns);
}


} // namespace net
} // namespace scy


#endif // SCY_Net_DNS_H


/// @\}
