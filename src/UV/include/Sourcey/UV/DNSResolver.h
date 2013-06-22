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


#ifndef SOURCEY_UV_DNSResolver_H
#define SOURCEY_UV_DNSResolver_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Signal.h"


namespace scy {
namespace UV {
	


class DNSResolver: public UV::Base
{
public:
	DNSResolver(uv_loop_t* loop = uv_default_loop());
	virtual ~DNSResolver();	
	
	virtual void resolve(const std::string& host, int port, struct addrinfo* hints = NULL);	
	
	struct Result {
		bool success;
		Net::Address addr;
		struct addrinfo* info;
	};

	Signal<Result&> OnResolved;

protected:
	uv_getaddrinfo_t _handle;
};


//
// UV Callbacks
//

//UVStatusCallback(DNSResolver, onIdle, uv_idle_t);


} } // namespace scy::UV


#endif // SOURCEY_UV_DNSResolver_H
