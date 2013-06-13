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


#ifndef SOURCEY_TURN_Permission_H
#define SOURCEY_TURN_Permission_H


#include "Sourcey/TURN/FiveTuple.h"
#include "Sourcey/Timeout.h"
#include "Sourcey/Net/Address.h"

#include <string>
#include <vector>
#include <list>
#include <map>


namespace Scy {
namespace TURN {


// The Permission Lifetime MUST be 300 seconds (= 5 minutes).
const int PERMISSION_LIFETIME = 3 * 60 * 1000;


struct Permission 
{
	Net::IP ip;
	Timeout timeout;
	Permission(const Net::IP& ip) : 
		ip(ip), timeout(PERMISSION_LIFETIME) 
	{
		refresh();
	}

	virtual void refresh()
	{
		timeout.reset();
	}

	bool operator ==(const Net::IP& r) const
	{
		return ip == r;
	}
};


typedef std::vector<Permission> PermissionList;


} } // namespace Scy::TURN


#endif // SOURCEY_TURN_Permission_H
