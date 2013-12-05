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


#include "scy/turn/server/server.h"
#include "scy/logger.h"
#include "scy/util.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>


using namespace std;


namespace scy {
namespace turn {


#define ENABLE_LOCAL_IPS 1


IAllocation::IAllocation(const FiveTuple& tuple, 
						 const std::string& username, 
						 Int64 lifetime) : 
	_createdAt(static_cast<Int64>(time(0))), 
	_updatedAt(static_cast<Int64>(time(0))), 
	_username(username), 
	_lifetime(lifetime), 
	_bandwidthLimit(0),
	_bandwidthUsed(0),
	_tuple(tuple),
	_deleted(false)
{	
}


IAllocation::~IAllocation() 
{
	TraceL << "Destroy" << endl;	
	_permissions.clear();
}


void IAllocation::updateUsage(Int64 numBytes)
{
	//Mutex::ScopedLock lock(_mutex);
	TraceL << "Update usage: " << _bandwidthUsed << ": " << numBytes << endl;	
	_updatedAt = time(0);
	_bandwidthUsed += numBytes;
}


Int64 IAllocation::timeRemaining() const
{
	//Mutex::ScopedLock lock(_mutex);
	//UInt32 remaining = static_cast<Int64>(_lifetime - (time(0) - _updatedAt));	
	Int64 remaining = _lifetime - static_cast<Int64>(time(0) - _updatedAt);
	return remaining > 0 ? remaining : 0;
}


bool IAllocation::expired() const
{
	return timeRemaining() == 0
		|| bandwidthRemaining() == 0;
}


bool IAllocation::deleted() const
{
	return _deleted || expired();
}


void IAllocation::setLifetime(Int64 lifetime)
{
	//Mutex::ScopedLock lock(_mutex);
	_lifetime = lifetime;
	_updatedAt = static_cast<Int64>(time(0));
	TraceL << "Updating Lifetime: " << _lifetime << endl;
}


void IAllocation::setBandwidthLimit(Int64 numBytes)
{
	//Mutex::ScopedLock lock(_mutex);
	_bandwidthLimit = numBytes;
}


Int64 IAllocation::bandwidthLimit() const
{
	//Mutex::ScopedLock lock(_mutex);
	return _bandwidthLimit;
}


Int64 IAllocation::bandwidthUsed() const
{
	//Mutex::ScopedLock lock(_mutex);
	return _bandwidthUsed;
}


Int64 IAllocation::bandwidthRemaining() const
{
	//Mutex::ScopedLock lock(_mutex);
	return _bandwidthLimit > 0
		? (_bandwidthLimit > _bandwidthUsed 
			? _bandwidthLimit - _bandwidthUsed : 0) : 99999999;
}


FiveTuple& IAllocation::tuple() 
{ 
	//Mutex::ScopedLock lock(_mutex);
	return _tuple; 
}


std::string IAllocation::username() const 
{ 
	//Mutex::ScopedLock lock(_mutex);
	return _username; 
}


Int64 IAllocation::lifetime() const 
{ 
	//Mutex::ScopedLock lock(_mutex);
	return _lifetime; 
}


PermissionList IAllocation::permissions() const 
{ 
	//Mutex::ScopedLock lock(_mutex);
	return _permissions; 
}


void IAllocation::addPermission(const std::string& ip) 
{
	//Mutex::ScopedLock lock(_mutex);

	// If the permission is already in the list then refresh it.
	for (auto it = _permissions.begin(); it != _permissions.end(); ++it) {
		if ((*it).ip == ip) {
			TraceL << "Refreshing permission: " << ip << endl;
			(*it).refresh();
			return;
		}
	}

	// Otherwise create it...
	TraceL << "Create permission: " << ip << endl;
	_permissions.push_back(Permission(ip));
}


void IAllocation::addPermissions(const IPList& ips)
{
	for (auto it = ips.begin(); it != ips.end(); ++it) {
		addPermission(*it);
	}
}


void IAllocation::removePermission(const std::string& ip) 
{
	//Mutex::ScopedLock lock(_mutex);

	for (auto it = _permissions.begin(); it != _permissions.end();) {
		if ((*it).ip == ip) {
			it = _permissions.erase(it);
			return;
		} else 
			++it;
	}	
}


void IAllocation::removeAllPermissions()
{
	//Mutex::ScopedLock lock(_mutex);
	_permissions.clear();
}


void IAllocation::removeExpiredPermissions() 
{
	//Mutex::ScopedLock lock(_mutex);
	for (auto it = _permissions.begin(); it != _permissions.end();) {
		if ((*it).timeout.expired()) {
			InfoL << "Removing Expired Permission: " << (*it).ip << endl;
			it = _permissions.erase(it);
		} else 
			++it;
	}
}


bool IAllocation::hasPermission(const std::string& peerIP) 
{
	for (auto it = _permissions.begin(); it != _permissions.end(); ++it) {
		if (*it == peerIP)
			return true;
	}

#if ENABLE_LOCAL_IPS
	if (peerIP.find("192.168.") == 0 || peerIP.find("127.") == 0) {
		WarnL << "Granting permission for local IP without explicit permission: " << peerIP << endl;
		return true;
	}
#endif

	TraceL << "No permission for: " << peerIP << endl;
	return false;
}


} } // namespace scy::turn