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


#include "scy/turn/client/udpclient.h"
#include "scy/net/udpsocket.h"


using namespace std;


namespace scy {
namespace turn {


UDPClient::UDPClient(ClientObserver& observer, const Options& options) : 
	Client(observer, options)
{
	log() << "Create" << endl;
	_socket.assign(new net::UDPBase, false);
}


UDPClient::~UDPClient() 
{
	log() << "Destroy" << endl;
}


} } //  namespace scy::turn

