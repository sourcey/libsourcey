///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#pragma once

#include "whip-session.h"

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace icy {

class WhipSessionManager
{
public:
    struct CreateResult
    {
        std::shared_ptr<WhipSession> session;
        std::string answerSdp;
        std::string error;
        int status = 201;
    };

    WhipSessionManager(std::filesystem::path outputDir,
                       size_t maxSessions,
                       std::vector<std::string> iceServers);

    CreateResult create(const std::string& offerSdp);
    [[nodiscard]] std::shared_ptr<WhipSession> find(const std::string& id) const;
    bool remove(const std::string& id, const std::string& reason);
    void sweepExpired();

    [[nodiscard]] size_t size() const;

private:
    [[nodiscard]] std::string generateId() const;

    std::filesystem::path _outputDir;
    size_t _maxSessions = 16;
    std::vector<std::string> _iceServers;
    mutable std::mutex _mutex;
    std::unordered_map<std::string, std::shared_ptr<WhipSession>> _sessions;
};

} // namespace icy
