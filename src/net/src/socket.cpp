///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/socket.h"
#include "scy/net/address.h"
#include "scy/net/socketadapter.h"
#include "scy/net/net.h"

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace net {


Socket::Socket()
    : opaque(nullptr)
{
    // TraceS(this) << "Create" << endl;
}


Socket::~Socket()
{
    // TraceS(this) << "Destroy" << endl;
}


void Socket::connect(const std::string& host, uint16_t port)
{
    TraceS(this) << "Connect to host: " << host << ":" << port << endl;

    if (Address::validateIP(host)) {
        connect(Address(host, port));
    }
    else if (host == "localhost") {
        // NOTE: Forcefully translate localhost to 127.0.0.1 since
        // the DNS service returns 0.0.0.0 on some systems resulting
        // in connection failure.
        connect(Address("127.0.0.1", port));
    }
    else {
        init();
        assert(!closed());
        net::resolveDNS(host, port, [&](const net::DNSResult& dns) {

            // Return if the socket was closed while resolving
            if (closed()) {
                WarnS(this) << "DNS resolved but socket closed" << endl;
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
                // Swallow exceptions
                // Errors should be handled via Error signal
            }
        });
    }
}


} // namespace net
} // namespace scy


/// @\}
