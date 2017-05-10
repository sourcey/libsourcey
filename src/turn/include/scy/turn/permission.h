///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#ifndef SCY_TURN_Permission_H
#define SCY_TURN_Permission_H


#include "scy/turn/fivetuple.h"
#include "scy/net/address.h"
#include "scy/util/timeout.h"

#include <string>
#include <vector>


namespace scy {
namespace turn {


/// Permission Lifetime MUST be 300 seconds (= 5 minutes).
const int PERMISSION_LIFETIME = 3 * 60 * 1000;


/// TURN permission for a user session
struct Permission
{
    std::string ip;
    Timeout timeout;

    Permission(const std::string& ip)
        : ip(ip)
        , timeout(PERMISSION_LIFETIME)
    {
        refresh();
    }

    void refresh() { timeout.reset(); }

    bool operator==(const std::string& r) const { return ip == r; }
};


typedef std::vector<Permission> PermissionList;


} // namespace turn
} // namespace scy


#endif // SCY_TURN_Permission_H


/// @\}
