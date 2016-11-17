///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup turn
/// @{

#ifndef SCY_TURN_ServerAllocation_H
#define SCY_TURN_ServerAllocation_H

#include "scy/turn/fivetuple.h"
#include "scy/turn/iallocation.h"

namespace scy {
namespace turn {

class Server;

class ServerAllocation : public IAllocation
{
public:
    ServerAllocation(Server& server, const FiveTuple& tuple,
                     const std::string& username, std::int64_t lifetime);

    virtual bool handleRequest(Request& request);
    virtual void handleRefreshRequest(Request& request);
    virtual void handleCreatePermission(Request& request);

    /// Asynchronous timer callback for updating the allocation
    /// permissions and state etc.
    /// If this call returns false the allocation will be deleted.
    virtual bool onTimer();

    virtual std::int64_t timeRemaining() const;
    virtual std::int64_t maxTimeRemaining() const;
    virtual Server& server();

    virtual void print(std::ostream& os) const;

protected:
    /// IMPORTANT: The destructor should never be called directly
    /// as the allocation is deleted via the timer callback.
    /// See onTimer()
    virtual ~ServerAllocation();

    friend class Server;

    std::uint32_t _maxLifetime;
    Server& _server;

private:
    ServerAllocation(const ServerAllocation&) = delete;
    ServerAllocation(ServerAllocation&&) = delete;
    ServerAllocation& operator=(const ServerAllocation&) = delete;
    ServerAllocation& operator=(ServerAllocation&&) = delete;
};

} // namespace turn
} // namespace scy

#endif // SCY_TURN_ServerAllocation_H

/// @\}
