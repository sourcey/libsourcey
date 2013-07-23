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
	enum Status {
		None,
		Resolving,
		Success,
		Failed
	} status;

	bool resolving() const { return status == Resolving; }
	bool success() const { return status == Success; }
	bool failed() const { return status == Failed; }
	bool complete() const { return status == Success 
		                        || status == Failed; }

	std::string host;
		/// The host to reolve

	UInt16 port;
		/// The host port to reolve

	net::Address addr;
		/// The resolved address

	struct addrinfo* info;
		/// The libuv uv_getaddrinfo result

	struct addrinfo* hints;
		/// libuv uv_getaddrinfo hints (optional)
	
	typedef void (*func)(const DNSResult&, void*);
	func callback;
		/// Result callback function

	void* opaque;
		/// Client data pointer

	DNSResult() : status(None), callback(0), 
		info(0), opaque(0), hints(nullptr)
	{
	}
};


//template<class C>
//template<class C>
//};

inline void onDNSResolved(uv_getaddrinfo_t* handle, int status, struct addrinfo* res)
{	
	DNSResult* dns = reinterpret_cast<DNSResult*>(handle->data);	
	dns->status = status == 0 ? DNSResult::Success : DNSResult::Failed;
	dns->info = res;
	dns->addr.swap(net::Address(res->ai_addr, 16));
	
	traceL("Network") << "DNS Resolved: " << dns->addr << std::endl;
	dns->callback(*dns, dns->opaque);

    uv_freeaddrinfo(res);	
	delete handle;
	delete dns;
}


inline bool resolveDNS(DNSResult* dns) 
{			
	traceL("Network") << "Resolving DNS: " << dns->host << ":" << dns->port << std::endl;

	assert(dns->port);
	assert(!dns->host.empty());
	assert(dns->callback);
	dns->status = DNSResult::Resolving;
	
	uv_getaddrinfo_t* handle = new uv_getaddrinfo_t;
	handle->data = dns;
	return uv_getaddrinfo(uv_default_loop(), handle, onDNSResolved, dns->host.c_str(), util::toString<UInt16>(dns->port).c_str(), dns->hints) == 0;
}


inline bool resolveDNS(const std::string& host, UInt16 port, DNSResult::func callback, void* opaque = NULL, struct addrinfo* hints = NULL)
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


#endif // SOURCEY_Net_Network_H


	
	//uv_getaddrinfo_t* handle = new uv_getaddrinfo_t;
	//handle->data = dns;
	//return uv_getaddrinfo(uv_default_loop(), handle, onDNSResolved, host.c_str(), util::toString<UInt16>(port).c_str(), hints) == 0;
/*
class Network: public uv::Base
{
public:
	Network(uv::Loop& loop = uv_default_loop());
	virtual ~Network();	
	
	//Signal<Result&> OnResolved;
};
*/