///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#include "whip-session.h"

#include "icy/logger.h"

#include <rtc/peerconnection.hpp>

#include <sstream>
#include <stdexcept>
#include <thread>

namespace icy {
namespace {

bool startsWith(std::string_view value, std::string_view prefix)
{
    return value.size() >= prefix.size() &&
           value.substr(0, prefix.size()) == prefix;
}

void trimTrailingCr(std::string& line)
{
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
}

} // namespace

WhipSession::WhipSession(std::string id,
                         std::filesystem::path outputDir,
                         std::vector<std::string> iceServers)
    : _id(std::move(id))
    , _peerId("whip-publisher-" + _id)
    , _peerSession(std::make_unique<wrtc::PeerSession>(_signaller, makeConfig(iceServers)))
    , _sink(std::move(outputDir), _id)
{
    _peerSession->StateChanged += [this](wrtc::PeerSession::State state) {
        onStateChanged(state);
    };
    _sink.attach(_peerSession->media());
}


WhipSession::~WhipSession()
{
    stop("destroyed");
}


const std::string& WhipSession::id() const
{
    return _id;
}


std::string WhipSession::etag() const
{
    return "\"" + _id + "\"";
}


const std::filesystem::path& WhipSession::outputFile() const
{
    return _sink.outputFile();
}


std::string WhipSession::start(const std::string& offerSdp,
                               std::chrono::milliseconds timeout)
{
    {
        std::lock_guard lock(_mutex);
        if (_started)
            throw std::logic_error("WHIP session already started");
        _started = true;
    }

    _signaller.ControlReceived.emit(_peerId, "init", {});
    _peerSession->accept();
    _signaller.SdpReceived.emit(_peerId, "offer", offerSdp);

    auto pc = _peerSession->peerConnection();
    if (!pc)
        throw std::runtime_error("PeerSession did not create a PeerConnection");

    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (_signaller.answer() &&
            pc->gatheringState() == rtc::PeerConnection::GatheringState::Complete) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (pc->gatheringState() != rtc::PeerConnection::GatheringState::Complete)
        throw std::runtime_error("Timed out waiting for ICE gathering to complete");

    if (auto local = pc->localDescription())
        return std::string(*local);

    if (auto answer = _signaller.answer())
        return *answer;

    throw std::runtime_error("PeerSession did not produce an SDP answer");
}


void WhipSession::addRemoteCandidates(const std::vector<CandidateFragment>& candidates)
{
    for (const auto& candidate : candidates) {
        if (candidate.candidate.empty())
            continue;
        _signaller.CandidateReceived.emit(_peerId, candidate.candidate, candidate.mid);
    }
}


void WhipSession::stop(const std::string& reason)
{
    {
        std::lock_guard lock(_mutex);
        if (_closed)
            return;
        _closed = true;
    }

    _sink.close();
    if (_peerSession)
        _peerSession->hangup(reason);
}


bool WhipSession::expired() const
{
    {
        std::lock_guard lock(_mutex);
        if (_closed)
            return true;
    }

    if (!_peerSession)
        return true;

    auto pc = _peerSession->peerConnection();
    if (pc) {
        switch (pc->state()) {
        case rtc::PeerConnection::State::Disconnected:
        case rtc::PeerConnection::State::Failed:
        case rtc::PeerConnection::State::Closed:
            return true;
        default:
            break;
        }
    }

    return _started && _peerSession->state() == wrtc::PeerSession::State::Idle;
}


std::vector<WhipSession::CandidateFragment>
WhipSession::parseCandidateFragment(const std::string& body)
{
    std::vector<CandidateFragment> candidates;
    std::string currentMid;
    std::istringstream input(body);
    std::string line;
    while (std::getline(input, line)) {
        trimTrailingCr(line);
        if (startsWith(line, "a=mid:")) {
            currentMid = line.substr(6);
        }
        else if (startsWith(line, "a=candidate:")) {
            candidates.push_back({line.substr(2), currentMid.empty() ? "0" : currentMid});
        }
        else if (startsWith(line, "candidate:")) {
            candidates.push_back({line, currentMid.empty() ? "0" : currentMid});
        }
    }
    return candidates;
}


wrtc::PeerSession::Config
WhipSession::makeConfig(const std::vector<std::string>& iceServers)
{
    wrtc::PeerSession::Config config;
    for (const auto& server : iceServers)
        config.rtcConfig.iceServers.emplace_back(server);

    config.rtcConfig.disableAutoGathering = false;
    config.rtcConfig.forceMediaTransport = true;
    config.enableDataChannel = false;
    config.media.videoCodec = av::VideoCodec("H264", 0, 0, 30.0);
    config.media.audioCodec = av::AudioCodec("opus", 2, 48000);
    config.media.videoDirection = rtc::Description::Direction::RecvOnly;
    config.media.audioDirection = rtc::Description::Direction::RecvOnly;
    return config;
}


void WhipSession::onStateChanged(wrtc::PeerSession::State state)
{
    LInfo("WHIP session ", _id, " state: ", wrtc::stateToString(state));
    if (state == wrtc::PeerSession::State::Active) {
        try {
            _peerSession->media().requestKeyframe();
        }
        catch (const std::exception& exc) {
            LDebug("Ignoring keyframe request error: ", exc.what());
        }
    }
    else if (state == wrtc::PeerSession::State::Ending ||
             state == wrtc::PeerSession::State::Ended) {
        _sink.close();
    }
}

} // namespace icy
