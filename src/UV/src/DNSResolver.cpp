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


#include "Sourcey/UV/DNSResolver.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace UV {

	
DNSResolver::DNSResolver(uv_loop_t* loop) :
	Base(loop)
{
	LogTrace("DNSResolver", this) << "Creating" << endl;
	_handle.data = this;
}

	
DNSResolver::~DNSResolver() 
{	
	LogTrace("DNSResolver", this) << "Destroying" << endl;
}


void onDNSResolved(uv_getaddrinfo_t* resolver, int status, struct addrinfo* res)
{	
	DNSResolver* self = static_cast<DNSResolver*>(resolver->data);
	
	DNSResolver::Result result;
	result.success = status == 0;
	result.info = res;
	result.addr.swap(Net::Address(res->ai_addr, 16));
	
	LogTrace("DNSResolver", self) << "Resolved: " << result.addr << endl;
	self->OnResolved.emit(self, result);
	
    uv_freeaddrinfo(res);
}
	

void DNSResolver::resolve(const string& host, int port, struct addrinfo* hints) 
{		
	LogTrace("DNSResolver", this) << "Resolving: " << host << ":" << port << endl;
	int r = uv_getaddrinfo(loop(), &_handle, onDNSResolved, host.c_str(), Util::toString<int>(port).c_str(), hints);
}



} } // namespace scy::UV