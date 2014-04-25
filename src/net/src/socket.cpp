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


#include "scy/net/socket.h"
#include "scy/net/socketadapter.h"
#include "scy/net/types.h"
#include "scy/net/address.h"

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace net {


Socket::Socket()
{
	TraceLS(this) << "Create" << endl;	
}


Socket::~Socket()
{
	TraceLS(this) << "Destroy" << endl;	
}

	
void Socket::connect(const std::string& host, UInt16 port) 
{
	TraceLS(this) << "Connect to host: " << host << ":" << port << endl;
	if (Address::validateIP(host))
		connect(Address(host, port));
	else {
		init();
		assert(!closed());
		net::resolveDNS(host, port, [](const net::DNSResult& dns) 
		{	
			auto* sock = reinterpret_cast<Socket*>(dns.opaque);
			TraceL << "DNS resolved: " << dns.success() << endl;

			// Return if the socket was closed while resolving
			if (sock->closed()) {			
				WarnL << "DNS resolved but socket closed" << endl;
				return;
			}

			// Set the connection error if DNS failed
			if (!dns.success()) {
				sock->setError("Failed to resolve DNS for " + dns.host);
				return;
			}

			try {	
				// Connect to resolved host
				sock->connect(dns.addr);
			}
			catch (...) {
				// Swallow errors
				// Can be handled by Socket::Error signal
			}	
		}, this); 
	}
}


} } // namespace scy::net