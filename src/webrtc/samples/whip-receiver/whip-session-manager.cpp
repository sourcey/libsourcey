///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#include "whip-session-manager.h"

#include "icy/logger.h"

#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>

namespace icy {

WhipSessionManager::WhipSessionManager(std::filesystem::path outputDir,
                                       size_t maxSessions,
                                       std::vector<std::string> iceServers)
    : _outputDir(std::move(outputDir))
    , _maxSessions(maxSessions)
    , _iceServers(std::move(iceServers))
{
}


WhipSessionManager::CreateResult
WhipSessionManager::create(const std::string& offerSdp)
{
    {
        std::lock_guard lock(_mutex);
        if (_sessions.size() >= _maxSessions) {
            return {.error = "maximum WHIP session count reached", .status = 503};
        }
    }

    std::string id;
    for (;;) {
        id = generateId();
        std::lock_guard lock(_mutex);
        if (_sessions.find(id) == _sessions.end())
            break;
    }

    auto session = std::make_shared<WhipSession>(id, _outputDir, _iceServers);
    std::string answer;
    try {
        answer = session->start(offerSdp, std::chrono::seconds(5));
    }
    catch (const std::exception& exc) {
        LWarn("WHIP session setup failed: ", exc.what());
        session->stop("setup failed");
        return {.error = exc.what(), .status = 422};
    }

    {
        std::lock_guard lock(_mutex);
        if (_sessions.size() >= _maxSessions) {
            session->stop("maximum session count reached");
            return {.error = "maximum WHIP session count reached", .status = 503};
        }
        _sessions.emplace(id, session);
    }

    LInfo("WHIP session created: ", id, " output=", session->outputFile().string());
    return {.session = session, .answerSdp = std::move(answer), .status = 201};
}


std::shared_ptr<WhipSession> WhipSessionManager::find(const std::string& id) const
{
    std::lock_guard lock(_mutex);
    auto it = _sessions.find(id);
    return it == _sessions.end() ? nullptr : it->second;
}


bool WhipSessionManager::remove(const std::string& id, const std::string& reason)
{
    std::shared_ptr<WhipSession> session;
    {
        std::lock_guard lock(_mutex);
        auto it = _sessions.find(id);
        if (it == _sessions.end())
            return false;
        session = std::move(it->second);
        _sessions.erase(it);
    }
    session->stop(reason);
    LInfo("WHIP session removed: ", id);
    return true;
}


void WhipSessionManager::sweepExpired()
{
    std::vector<std::shared_ptr<WhipSession>> expired;
    {
        std::lock_guard lock(_mutex);
        for (auto it = _sessions.begin(); it != _sessions.end();) {
            if (it->second->expired()) {
                expired.push_back(std::move(it->second));
                it = _sessions.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    for (auto& session : expired) {
        LInfo("Sweeping expired WHIP session: ", session->id());
        session->stop("expired");
    }
}


size_t WhipSessionManager::size() const
{
    std::lock_guard lock(_mutex);
    return _sessions.size();
}


std::string WhipSessionManager::generateId() const
{
    static thread_local std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<uint64_t> dist;
    const auto value = dist(rng);
    std::ostringstream out;
    out << std::hex << std::setfill('0') << std::setw(16) << value;
    return out.str().substr(0, 12);
}

} // namespace icy
