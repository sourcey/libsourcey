///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/socket.h"
#include "scy/net/address.h"
#include "scy/net/socketadapter.h"
#include "scy/net/types.h"

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace net {


Socket::Socket()
{
    TraceS(this) << "Create" << endl;
}


Socket::~Socket()
{
    TraceS(this) << "Destroy" << endl;
}


void Socket::connect(const std::string& host, std::uint16_t port)
{
    TraceS(this) << "Connect to host: " << host << ":" << port << endl;

    if (Address::validateIP(host))
        connect(Address(host, port));
    else {
        init();
        assert(!closed());
        net::resolveDNS(host, port, [&](const net::DNSResult& dns) {
            TraceL << "DNS resolved: " << dns.success() << endl;

            // Return if the socket was closed while resolving
            if (closed()) {
                WarnL << "DNS resolved but socket closed" << endl;
                return;
            }

            // Set the connection error if DNS failed
            if (!dns.success()) {
                setError("Failed to resolve DNS for " + dns.host);
                return;
            }

            try {
                // Connect to resolved host
                connect(dns.addr);
            } catch (...) {
                // Swallow errors
                // Errors can be handled by Socket::Error signal
            }
        });
    }
}


} // namespace net
} // namespace scy


/// @\}
