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


#ifndef SOURCEY_Net_Network_H
#define SOURCEY_Net_Network_H


#include "Sourcey/UV/UVPP.h"

#include "Sourcey/Net/Address.h"
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
	bool success;
	net::Address addr;
	struct addrinfo* info;
	typedef void (*Callback)(const DNSResult&);
	Callback callback;

	DNSResult() : //(const net::Address& addr, Callback callback) : 
		success(false), info(0) {}
};


void onDNSResolved(uv_getaddrinfo_t* handle, int status, struct addrinfo* res)
{	
	DNSResult* dns = reinterpret_cast<DNSResult*>(handle->data);	
	dns->success = status == 0;
	dns->info = res;
	dns->addr.swap(net::Address(res->ai_addr, 16));
	
	traceL("Network") << "DNS Resolved: " << dns->addr << std::endl;

	dns->callback(*dns);
    uv_freeaddrinfo(res);
	
	delete handle;
	delete dns;
}


bool resolveDNS(const std::string& host, int port, DNSResult::Callback callback, struct addrinfo* hints = NULL) 
{		
	traceL("Network") << "Resolving DNS: " << host << ":" << port << std::endl;
	uv_getaddrinfo_t* handle = new uv_getaddrinfo_t;
	DNSResult* dns = new DNSResult();
	dns->callback = callback;
	handle->data = dns;
	return uv_getaddrinfo(uv_default_loop(), handle, onDNSResolved, host.c_str(), util::toString<int>(port).c_str(), hints) == 0;
}



} } // namespace scy::net


#endif // SOURCEY_Net_Network_H


	
/*
class Network: public uv::Base
{
public:
	Network(uv::Loop& loop = uv_default_loop());
	virtual ~Network();	
	
	//Signal<Result&> OnResolved;
};
*/