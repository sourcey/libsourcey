// icey — Copyright (c) 2005, icey <https://0state.com>
// SPDX-License-Identifier: LGPL-2.1+
// Camera or test file → H.264 → WebRTC → browser.
// Usage: webcam-streamer -host localhost -port 4500 -camera 1

#include "icy/application.h"
#include "icy/av/devicemanager.h"
#include "icy/av/videocapture.h"
#include "icy/av/videopacketencoder.h"
#include "icy/logger.h"
#include "icy/packetstream.h"
#include "icy/symple/client.h"
#include "icy/webrtc/codecnegotiator.h"
#include "icy/webrtc/peersession.h"
#include "icy/webrtc/support/symplesignaller.h"

#include <memory>
#include <optional>
#include <string>

using namespace icy;

class WebcamStreamer
{
    smpl::Client client;
    wrtc::SympleSignaller signaller;
    std::unique_ptr<wrtc::PeerSession> session;
    av::VideoCapture capture;
    std::shared_ptr<av::VideoPacketEncoder> encoder;
    PacketStream stream{"webcam-stream"};
    bool pipelineReady = false;

public:
    explicit WebcamStreamer(const smpl::Client::Options& opts)
        : client(opts), signaller(client)
    {
    }

    void start(bool useCamera)
    {
        auto camera = useCamera
            ? av::DeviceManager().negotiateVideoCapture("", 1280, 720, 30.0)
            : std::nullopt;
        if (camera) {
            const auto& [device, cap] = *camera;
            capture.openVideo(device.id, cap.width, cap.height, cap.maxFps, cap.pixelFormat);
        } else {
            if (useCamera)
                LWarn("No camera found, falling back to test file");
            capture.openFile(ICY_DATA_DIR "/test.mp4");
            capture.setLoopInput(true);
            capture.setLimitFramerate(true);
        }

        encoder = std::make_shared<av::VideoPacketEncoder>();
        capture.getEncoderVideoCodec(encoder->iparams);
        const auto& input = encoder->iparams;
        encoder->oparams = wrtc::CodecNegotiator::resolveWebRtcVideoCodec(
            av::VideoCodec("H264", "libx264", input.width, input.height, input.fps));

        wrtc::PeerSession::Config config;
        config.rtcConfig.iceServers.emplace_back("stun:stun.l.google.com:19302");
        config.media.videoCodec = encoder->oparams;
        config.media.videoDirection = rtc::Description::Direction::SendOnly;
        config.enableDataChannel = false;
        session = std::make_unique<wrtc::PeerSession>(signaller, config);
        session->IncomingCall += [this](const std::string&) { session->accept(); };
        session->StateChanged += [this](wrtc::PeerSession::State state) {
            if (state == wrtc::PeerSession::State::Active)
                startStreaming();
            else if (state == wrtc::PeerSession::State::Ended)
                stopStreaming();
        };
        client.Announce += [](const int& status) {
            if (status != 200)
                LError("Auth failed: ", status);
        };
        client.StateChange += [this](void*, smpl::ClientState& state, const smpl::ClientState&) {
            if (state.id() == smpl::ClientState::Online) {
                LInfo("Online as ", client.ourID());
                client.joinRoom("public");
            }
        };
        client.CreatePresence += [](smpl::Peer& peer) { peer["agent"] = "icey"; };
        client.start();
    }

    void shutdown()
    {
        stopStreaming();
        stream.close();
        session->hangup("shutdown");
        session.reset();
        client.stop();
    }

private:
    void startStreaming()
    {
        if (!session->media().hasVideo())
            return;
        if (!pipelineReady) {
            stream.attachSource(&capture, false, true);
            stream.attach(encoder, 1, true);
            stream.attach(&session->media().videoSender(), 5, false);
            pipelineReady = true;
        }
        stream.start();
    }

    void stopStreaming()
    {
        // Join capture before stream teardown takes the processing lock.
        capture.stop();
        stream.stop();
    }
};

int main(int argc, char** argv)
{
    Logger::instance().add(std::make_unique<ConsoleChannel>("console", Level::Info));
    smpl::Client::Options opts;
    opts.user = "webcam-streamer";
    opts.name = "Webcam Streamer";
    opts.type = "streamer";
    bool useCamera = false;

    for (int i = 1; i + 1 < argc; i += 2) {
        std::string key = argv[i];
        std::string val = argv[i + 1];
        if (key == "-host") opts.host = val;
        else if (key == "-port") opts.port = static_cast<uint16_t>(std::stoi(val));
        else if (key == "-token") opts.token = val;
        else if (key == "-user") opts.user = val;
        else if (key == "-name") opts.name = val;
        else if (key == "-camera") useCamera = val == "1";
    }

    WebcamStreamer app(opts);
    app.start(useCamera);
    waitForShutdown([](void* opaque) {
        static_cast<WebcamStreamer*>(opaque)->shutdown();
    }, &app);
    return 0;
}
