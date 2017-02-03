///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_Util_H
#define SCY_Net_Util_H


#include "scy/net/address.h"
#include "scy/uv/uvpp.h"
#include <vector>


namespace scy {
namespace net {


//
// Network Interfaces
//


inline void getNetworkInterfaces(std::vector<net::Address>& hosts)
{
    uv_interface_address_t* info;
    int count, i;

    uv_interface_addresses(&info, &count);
    i = count;

    while (i--) {
        uv_interface_address_t iface = info[i];
        hosts.push_back(net::Address(
            reinterpret_cast<const sockaddr*>(&iface.address.address4), 16));
    }

    uv_free_interface_addresses(info, count);
}


} // namespace net
} // namespace scy


#endif // SCY_Net_Util_H


/// @\}
