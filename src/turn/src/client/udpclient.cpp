///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#include "scy/turn/client/udpclient.h"
#include "scy/net/udpsocket.h"


using namespace std;


namespace scy {
namespace turn {


UDPClient::UDPClient(ClientObserver& observer, const Options& options)
    : Client(observer, options, net::makeSocket<net::UDPSocket>())
{
    TraceA("Create")
}


UDPClient::~UDPClient()
{
    TraceA("Destroy")
}


} } // namespace scy::turn


/// @\}
