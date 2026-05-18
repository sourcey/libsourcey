///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#pragma once

#include "whip-file-sink.h"
#include "whip-signaller.h"

#include "icy/webrtc/peersession.h"

#include <chrono>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace icy {

class WhipSession : public std::enable_shared_from_this<WhipSession>
{
public:
    struct CandidateFragment
    {
        std::string candidate;
        std::string mid;
    };

    WhipSession(std::string id,
                std::filesystem::path outputDir,
                std::vector<std::string> iceServers);
    ~WhipSession();

    WhipSession(const WhipSession&) = delete;
    WhipSession& operator=(const WhipSession&) = delete;

    [[nodiscard]] const std::string& id() const;
    [[nodiscard]] std::string etag() const;
    [[nodiscard]] const std::filesystem::path& outputFile() const;

    std::string start(const std::string& offerSdp,
                      std::chrono::milliseconds timeout);
    void addRemoteCandidates(const std::vector<CandidateFragment>& candidates);
    void stop(const std::string& reason);

    [[nodiscard]] bool expired() const;

    static std::vector<CandidateFragment> parseCandidateFragment(const std::string& body);

private:
    static wrtc::PeerSession::Config makeConfig(const std::vector<std::string>& iceServers);
    void onStateChanged(wrtc::PeerSession::State state);

    std::string _id;
    std::string _peerId;
    WhipSignaller _signaller;
    std::unique_ptr<wrtc::PeerSession> _peerSession;
    WhipFileSink _sink;
    bool _started = false;
    bool _closed = false;
    mutable std::mutex _mutex;
};

} // namespace icy
