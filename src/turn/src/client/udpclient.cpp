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
    : Client(observer, options)
{
    TraceL << "Create" << endl;

    _socket = net::makeSocket<net::UDPSocket>();
    //_socket.assign(new net::UDPSocket, false);
}


UDPClient::~UDPClient()
{
    TraceL << "Destroy" << endl;
}


}} // namespace scy::turn


/// @\}
