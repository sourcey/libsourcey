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


#include "Sourcey/UV/Network.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace uv {

	
	/*
NetworkInterfaces::NetworkInterfaces(uv_loop_t* loop) :
	Base(loop)
{
	traceL("NetworkInterfaces", this) << "Creating" << endl;
	//_handle.data = this;
    //uv_idle_init(loop, &_handle);
}

	
NetworkInterfaces::~NetworkInterfaces() 
{	
	traceL("NetworkInterfaces", this) << "Destroying" << endl;
}
	

void NetworkInterfaces::getInterfaces(std::vector<net::Address>& hosts)
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

        printf("Name: %s\n", iface.name);
        printf("Internal? %s\n", iface.is_internal ? "Yes" : "No");
        
        if (interface.address.address4.sin_family == AF_INET) {
            uv_ip4_name(&iface.address.address4, buf, sizeof(buf));
            printf("IPv4 address: %s\n", buf);
        }
        else if (interface.address.address4.sin_family == AF_INET6) {
            uv_ip6_name(&iface.address.address6, buf, sizeof(buf));
            printf("IPv6 address: %s\n", buf);
        }
		*/


} } // namespace scy::uv