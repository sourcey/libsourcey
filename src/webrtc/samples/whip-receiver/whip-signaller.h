///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#pragma once

#include "icy/webrtc/signalling.h"

#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace icy {

class WhipSignaller final : public wrtc::SignallingInterface
{
public:
    struct CandidateMessage
    {
        std::string candidate;
        std::string mid;
    };

    void sendSdp(const std::string& peerId,
                 const std::string& type,
                 const std::string& sdp) override;

    void sendCandidate(const std::string& peerId,
                       const std::string& candidate,
                       const std::string& mid) override;

    void sendControl(const std::string& peerId,
                     const std::string& type,
                     const std::string& reason = {}) override;

    [[nodiscard]] std::optional<std::string> answer() const;
    [[nodiscard]] std::vector<CandidateMessage> candidates() const;

    void setUpdatedCallback(std::function<void()> callback);

private:
    void notifyUpdated();

    mutable std::mutex _mutex;
    std::optional<std::string> _answer;
    std::vector<CandidateMessage> _candidates;
    std::function<void()> _updated;
};

} // namespace icy
