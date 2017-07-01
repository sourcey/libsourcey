///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#include "scy/logger.h"
#include "scy/turn/server/server.h"
#include "scy/util.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>


using namespace std;


namespace scy {
namespace turn {


#define ENABLE_LOCAL_IPS 1


IAllocation::IAllocation(const FiveTuple& tuple, const std::string& username,
                         std::int64_t lifetime)
    : _tuple(tuple)
    , _username(username)
    , _lifetime(lifetime)
    , _bandwidthLimit(0)
    , _bandwidthUsed(0)
    , _createdAt(static_cast<std::int64_t>(time(0)))
    , _updatedAt(static_cast<std::int64_t>(time(0)))
    , _deleted(false)
{
}


IAllocation::~IAllocation()
{
    LTrace("Destroy")
    _permissions.clear();
}


void IAllocation::updateUsage(std::int64_t numBytes)
{
    LTrace("Update usage: ", _bandwidthUsed, ": ", numBytes)
    _updatedAt = time(0);
    _bandwidthUsed += numBytes;
}


std::int64_t IAllocation::timeRemaining() const
{
    // uint32_t remaining = static_cast<std::int64_t>(_lifetime - (time(0) - _updatedAt));
    std::int64_t remaining = _lifetime - static_cast<std::int64_t>(time(0) - _updatedAt);
    return remaining > 0 ? remaining : 0;
}


bool IAllocation::expired() const
{
    return timeRemaining() == 0 || bandwidthRemaining() == 0;
}


bool IAllocation::deleted() const
{
    return _deleted || expired();
}


void IAllocation::setLifetime(std::int64_t lifetime)
{
    _lifetime = lifetime;
    _updatedAt = static_cast<std::int64_t>(time(0));
    LTrace("Updating Lifetime: ", _lifetime)
}


void IAllocation::setBandwidthLimit(std::int64_t numBytes)
{
    _bandwidthLimit = numBytes;
}


std::int64_t IAllocation::bandwidthLimit() const
{
    return _bandwidthLimit;
}


std::int64_t IAllocation::bandwidthUsed() const
{
    return _bandwidthUsed;
}


std::int64_t IAllocation::bandwidthRemaining() const
{
    return _bandwidthLimit > 0 ? (_bandwidthLimit > _bandwidthUsed
                                      ? _bandwidthLimit - _bandwidthUsed
                                      : 0) : 99999999;
}


FiveTuple& IAllocation::tuple()
{
    return _tuple;
}


std::string IAllocation::username() const
{
    return _username;
}


std::int64_t IAllocation::lifetime() const
{
    return _lifetime;
}


PermissionList IAllocation::permissions() const
{
    return _permissions;
}


void IAllocation::addPermission(const std::string& ip)
{
    // If the permission is already in the list then refresh it.
    for (auto it = _permissions.begin(); it != _permissions.end(); ++it) {
        if ((*it).ip == ip) {
            LTrace("Refreshing permission: ", ip)
            (*it).refresh();
            return;
        }
    }

    // Otherwise create it...
    LTrace("Create permission: ", ip)
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

    _permissions.clear();
}


void IAllocation::removeExpiredPermissions()
{

    for (auto it = _permissions.begin(); it != _permissions.end();) {
        if ((*it).timeout.expired()) {
            LInfo("Removing Expired Permission: ", (*it).ip)
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
        LWarn("Granting permission for local IP without explicit permission: ", peerIP)
        return true;
    }
#endif

    LTrace("No permission for: ", peerIP)
    return false;
}


} // namespace turn
} // namespace scy


/// @\}
