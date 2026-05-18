///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#include "whip-signaller.h"

#include "icy/logger.h"

namespace icy {

void WhipSignaller::sendSdp(const std::string&,
                            const std::string& type,
                            const std::string& sdp)
{
    if (type != "answer")
        return;

    {
        std::lock_guard lock(_mutex);
        _answer = sdp;
    }
    notifyUpdated();
}


void WhipSignaller::sendCandidate(const std::string&,
                                  const std::string& candidate,
                                  const std::string& mid)
{
    {
        std::lock_guard lock(_mutex);
        _candidates.push_back({candidate, mid});
    }
    notifyUpdated();
}


void WhipSignaller::sendControl(const std::string&,
                                const std::string& type,
                                const std::string& reason)
{
    LDebug("WHIP signalling control: ", type, " reason=", reason);
}


std::optional<std::string> WhipSignaller::answer() const
{
    std::lock_guard lock(_mutex);
    return _answer;
}


std::vector<WhipSignaller::CandidateMessage> WhipSignaller::candidates() const
{
    std::lock_guard lock(_mutex);
    return _candidates;
}


void WhipSignaller::setUpdatedCallback(std::function<void()> callback)
{
    std::lock_guard lock(_mutex);
    _updated = std::move(callback);
}


void WhipSignaller::notifyUpdated()
{
    std::function<void()> updated;
    {
        std::lock_guard lock(_mutex);
        updated = _updated;
    }
    if (updated)
        updated();
}

} // namespace icy
