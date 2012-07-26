//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
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


namespace Sourcey {
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


} } // namespace Sourcey::TURN


#endif // SOURCEY_TURN_Permission_H
