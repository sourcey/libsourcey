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


#ifndef SOURCEY_NET_Types_H
#define SOURCEY_NET_Types_H


#include "Sourcey/Stateful.h"


namespace Scy {	
namespace Net {
	

const int MAX_TCP_PACKET_SIZE = 64 * 1024;
const int MAX_UDP_PACKET_SIZE = 1500;


enum TransportProtocol 
{
	UDP,
	TCP,
	SSLTCP
};


struct Error 
{
	std::string message;
	int error;
};


} } // namespace Scy::Net


#endif


