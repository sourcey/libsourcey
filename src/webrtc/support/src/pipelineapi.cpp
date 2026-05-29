///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//


#include "icy/graft/host/pipeline.h"

#include "icy/av/mediacapture.h"
#include "icy/av/videopacketencoder.h"
#include "icy/loop.h"
#include "icy/packetstream.h"
#include "icy/symple/client.h"
#include "icy/webrtc/codecnegotiator.h"
#include "icy/webrtc/peersession.h"
#include "icy/webrtc/support/symplesignaller.h"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>


namespace {


struct SignallingEndpoint
{
    std::string host = "127.0.0.1";
    std::uint16_t port = 4500;
    bool secure = false;
};


[[nodiscard]] bool startsWith(const std::string& value, const char* prefix)
{
    return value.rfind(prefix, 0) == 0;
}


[[nodiscard]] SignallingEndpoint parseSignallingUrl(const char* raw)
{
    SignallingEndpoint endpoint;
    std::string url = raw ? raw : "";
    if (url.empty())
        return endpoint;

    if (startsWith(url, "ws://")) {
        url.erase(0, 5);
    }
    else if (startsWith(url, "wss://")) {
        endpoint.secure = true;
        url.erase(0, 6);
    }

    const auto slash = url.find('/');
    if (slash != std::string::npos)
        url.erase(slash);

    if (url.empty())
        throw std::invalid_argument("signalling URL host is empty");

    const auto colon = url.rfind(':');
    if (colon != std::string::npos) {
        endpoint.host = url.substr(0, colon);
        const auto portText = url.substr(colon + 1);
        std::size_t consumed = 0;
        const auto port = std::stoul(portText, &consumed);
        if (consumed != portText.size())
            throw std::invalid_argument("signalling URL port is invalid");
        if (port == 0 || port > 65535)
            throw std::out_of_range("signalling URL port is out of range");
        endpoint.port = static_cast<std::uint16_t>(port);
    }
    else {
        endpoint.host = url;
    }

    if (endpoint.host.empty())
        throw std::invalid_argument("signalling URL host is empty");

    return endpoint;
}


[[nodiscard]] bool optionFieldAvailable(const icy_pipeline_options_t* options,
                                        std::size_t offset,
                                        std::size_t size)
{
    return options && options->struct_size >= offset + size;
}


bool closeLoopDrained(icy::uv::Loop* loop)
{
    if (!loop)
        return true;

    for (int attempts = 0; attempts != 64; ++attempts) {
        if (icy::uv::closeLoop(loop))
            return true;
        uv_run(loop, UV_RUN_NOWAIT);
    }

    uv_walk(loop, [](uv_handle_t* handle, void*) {
        if (!uv_is_closing(handle))
            uv_close(handle, nullptr);
    }, nullptr);
    uv_run(loop, UV_RUN_DEFAULT);

    return icy::uv::closeLoop(loop);
}


class Pipeline
{
public:
    explicit Pipeline(const icy_pipeline_options_t* options)
    {
        if (options && options->struct_size > 0) {
            if (!optionFieldAvailable(options,
                                      offsetof(icy_pipeline_options_t, name),
                                      sizeof(options->name))) {
                throw std::invalid_argument("pipeline options struct is too small");
            }
            if (options->name && *options->name)
                _name = options->name;
            if (optionFieldAvailable(options,
                                     offsetof(icy_pipeline_options_t, signalling_token),
                                     sizeof(options->signalling_token))
                && options->signalling_token) {
                _signallingToken = options->signalling_token;
            }
            if (optionFieldAvailable(options,
                                     offsetof(icy_pipeline_options_t, room),
                                     sizeof(options->room))
                && options->room && *options->room) {
                _room = options->room;
            }
            if (optionFieldAvailable(options,
                                     offsetof(icy_pipeline_options_t, ice_server),
                                     sizeof(options->ice_server))
                && options->ice_server && *options->ice_server) {
                _iceServer = options->ice_server;
            }
        }

        if (_name.empty())
            _name = "icey-pipeline";
        if (_room.empty())
            _room = "public";
        if (_iceServer.empty())
            _iceServer = "stun:stun.l.google.com:19302";
    }

    ~Pipeline() noexcept
    {
        try {
            stop();
        }
        catch (...) {
        }

        if (_loop) {
            closeLoopDrained(_loop);
            delete _loop;
            _loop = nullptr;
        }
    }

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    icy_graft_status_t attachRtspSource(const char* url)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        if (_started)
            return setError(ICY_GRAFT_STATUS_INVALID_STATE,
                            "cannot change source after pipeline start");
        if (!url || !*url)
            return setError(ICY_GRAFT_STATUS_INVALID_ARGUMENT,
                            "RTSP source URL must not be empty");

        _sourceUrl = url;
        clearError();
        return ICY_GRAFT_STATUS_OK;
    }

    icy_graft_status_t attachWebRtcSender(const char* signallingUrl,
                                          const char* peerId)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        if (_started)
            return setError(ICY_GRAFT_STATUS_INVALID_STATE,
                            "cannot change WebRTC sender after pipeline start");

        try {
            _endpoint = parseSignallingUrl(signallingUrl);
        }
        catch (const std::exception& exc) {
            return setError(ICY_GRAFT_STATUS_INVALID_ARGUMENT, exc.what());
        }

        _peerId = (peerId && *peerId) ? peerId : _name;
        clearError();
        return ICY_GRAFT_STATUS_OK;
    }

    icy_graft_status_t start()
    {
        std::lock_guard<std::mutex> guard(_mutex);
        if (_started)
            return ICY_GRAFT_STATUS_OK;
        if (_sourceUrl.empty())
            return setError(ICY_GRAFT_STATUS_INVALID_STATE,
                            "pipeline has no RTSP source");
        if (_peerId.empty())
            return setError(ICY_GRAFT_STATUS_INVALID_STATE,
                            "pipeline has no WebRTC sender");

        try {
            _loop = icy::uv::createLoop();

            _stream = std::make_unique<icy::PacketStream>(_name);
            _capture = std::make_shared<icy::av::MediaCapture>();
            _capture->setOpenOptions({
                {"rtsp_transport", "tcp"},
                {"fflags", "nobuffer"},
                {"flags", "low_delay"},
            });
            _capture->openFile(_sourceUrl);
            _capture->setLimitFramerate(true);
            _capture->setRealtimePTS(true);

            icy::smpl::Client::Options clientOptions;
            clientOptions.host = _endpoint.host;
            clientOptions.port = _endpoint.port;
            clientOptions.secure = _endpoint.secure;
            clientOptions.reconnection = false;
            clientOptions.user = _peerId;
            clientOptions.name = _name;
            clientOptions.type = "pipeline";
            clientOptions.token = _signallingToken;

            _client = std::make_unique<icy::smpl::Client>(clientOptions, _loop);
            _client->Announce += [this](const int& status) {
                if (status != 200)
                    setErrorThreadSafe("Symple authentication failed: "
                                       + std::to_string(status));
            };
            _client->StateChange += icy::slot(this, &Pipeline::onClientStateChange);
            _client->CreatePresence += icy::slot(this, &Pipeline::onCreatePresence);

            initStopSignal();
            _client->start();
            _started = true;
            _loopThread = std::thread([this]() {
                icy::uv::runLoop(_loop);
            });
        }
        catch (const std::exception& exc) {
            cleanupAfterFailedStart();
            return setError(ICY_GRAFT_STATUS_ERROR, exc.what());
        }

        clearError();
        return ICY_GRAFT_STATUS_OK;
    }

    icy_graft_status_t stop()
    {
        bool sendStop = false;

        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (!_started)
                return ICY_GRAFT_STATUS_OK;

            if (_stopping) {
                lock.unlock();
                joinLoopThread();
                closeStoppedLoop();
                return ICY_GRAFT_STATUS_OK;
            }

            _stopping = true;
            sendStop = _stopAsyncInitialized;
        }

        if (sendStop)
            uv_async_send(&_stopAsync);
        else
            stopOnLoop();

        joinLoopThread();
        closeStoppedLoop();
        return ICY_GRAFT_STATUS_OK;
    }

    const char* lastError() const
    {
        thread_local std::string errorCopy;
        std::lock_guard<std::mutex> guard(_mutex);
        errorCopy = _lastError;
        return errorCopy.empty() ? nullptr : errorCopy.c_str();
    }

private:
    void initStopSignal()
    {
        if (uv_async_init(_loop, &_stopAsync, [](uv_async_t* handle) {
                auto* pipeline = static_cast<Pipeline*>(handle->data);
                pipeline->stopOnLoop();
            })
            != 0) {
            throw std::runtime_error("failed to initialize pipeline stop signal");
        }

        _stopAsync.data = this;
        _stopAsyncInitialized = true;
    }

    void stopOnLoop()
    {
        shutdownRuntime(true);

        if (_stopAsyncInitialized && !uv_is_closing(reinterpret_cast<uv_handle_t*>(&_stopAsync))) {
            uv_close(reinterpret_cast<uv_handle_t*>(&_stopAsync), [](uv_handle_t* handle) {
                auto* pipeline = static_cast<Pipeline*>(handle->data);
                std::lock_guard<std::mutex> guard(pipeline->_mutex);
                pipeline->_stopAsyncInitialized = false;
            });
        }

        std::lock_guard<std::mutex> guard(_mutex);
        _started = false;
        _stopping = false;
    }

    void shutdownRuntime(bool threadSafeErrors)
    {
        try {
            stopStreaming();
            if (_stream)
                _stream->close();
            if (_session)
                _session->hangup("pipeline stopped");
        }
        catch (const std::exception& exc) {
            if (threadSafeErrors)
                setErrorThreadSafe(exc.what());
            else
                _lastError = exc.what();
        }

        _session.reset();
        _signaller.reset();
        _encoder.reset();
        _capture.reset();
        _stream.reset();

        if (_client)
            _client->stop();
        _client.reset();
    }

    void cleanupAfterFailedStart()
    {
        shutdownRuntime(false);
        if (_stopAsyncInitialized && !uv_is_closing(reinterpret_cast<uv_handle_t*>(&_stopAsync))) {
            uv_close(reinterpret_cast<uv_handle_t*>(&_stopAsync), nullptr);
            uv_run(_loop, UV_RUN_DEFAULT);
            _stopAsyncInitialized = false;
        }
        if (_loop) {
            closeLoopDrained(_loop);
            delete _loop;
            _loop = nullptr;
        }
        _started = false;
        _stopping = false;
        _pipelineReady = false;
    }

    void joinLoopThread()
    {
        if (_loopThread.joinable() &&
            _loopThread.get_id() != std::this_thread::get_id()) {
            _loopThread.join();
        }
    }

    void closeStoppedLoop()
    {
        icy::uv::Loop* loop = nullptr;

        {
            std::lock_guard<std::mutex> guard(_mutex);
            if (!_loop || _loopThread.joinable())
                return;

            loop = _loop;
            _loop = nullptr;
            _pipelineReady = false;
        }

        closeLoopDrained(loop);
        delete loop;
    }

    void onClientStateChange(void*, icy::smpl::ClientState& state,
                             const icy::smpl::ClientState&)
    {
        if (state.id() == icy::smpl::ClientState::Online) {
            try {
                _client->joinRoom(_room);
                createSession();
            }
            catch (const std::exception& exc) {
                setErrorThreadSafe(exc.what());
            }
        }
        else if (state.id() == icy::smpl::ClientState::Error) {
            setErrorThreadSafe("Symple client entered error state");
        }
    }

    void onCreatePresence(icy::smpl::Peer& peer)
    {
        peer["agent"] = "icey";
        peer["type"] = "pipeline";
    }

    void createSession()
    {
        icy::av::VideoCodec inputCodec;
        _capture->getEncoderVideoCodec(inputCodec);

        auto videoCodec = icy::wrtc::CodecNegotiator::resolveWebRtcVideoCodec(
            icy::av::VideoCodec("H264",
                                "libx264",
                                inputCodec.width,
                                inputCodec.height,
                                inputCodec.fps));

        icy::wrtc::PeerSession::Config config;
        config.rtcConfig.iceServers.emplace_back(_iceServer);
        config.media.videoCodec = videoCodec;
        config.enableDataChannel = false;

        _signaller = std::make_unique<icy::wrtc::SympleSignaller>(*_client);
        _session = std::make_unique<icy::wrtc::PeerSession>(*_signaller, config);

        _session->IncomingCall += [this](const std::string&) {
            try {
                _session->accept();
            }
            catch (const std::exception& exc) {
                setErrorThreadSafe(exc.what());
            }
        };

        _session->StateChanged += [this](icy::wrtc::PeerSession::State state) {
            try {
                if (state == icy::wrtc::PeerSession::State::Active)
                    startStreaming();
                else if (state == icy::wrtc::PeerSession::State::Ended)
                    stopStreaming();
            }
            catch (const std::exception& exc) {
                setErrorThreadSafe(exc.what());
            }
        };
    }

    void startStreaming()
    {
        if (!_session || !_session->media().hasVideo())
            return;

        if (!_pipelineReady) {
            _encoder = std::make_shared<icy::av::VideoPacketEncoder>();

            _capture->getEncoderVideoCodec(_encoder->iparams);
            _encoder->oparams = icy::wrtc::CodecNegotiator::resolveWebRtcVideoCodec(
                icy::av::VideoCodec("H264",
                                    "libx264",
                                    _encoder->iparams.width,
                                    _encoder->iparams.height,
                                    _encoder->iparams.fps));

            _stream->attachSource(_capture.get(), false, true);
            _stream->attach(&_session->media().videoSender(), 5, false);
            _stream->attach(_encoder, 1, true);
            _pipelineReady = true;
        }

        _stream->start();
        _capture->start();
    }

    void stopStreaming()
    {
        if (_capture)
            _capture->stop();
        if (_stream)
            _stream->stop();
    }

    void clearError()
    {
        _lastError.clear();
    }

    void setErrorThreadSafe(const std::string& message)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        _lastError = message;
    }

    icy_graft_status_t setError(icy_graft_status_t status,
                                const std::string& message)
    {
        _lastError = message;
        return status;
    }

    mutable std::mutex _mutex;
    std::string _name;
    std::string _signallingToken;
    std::string _room;
    std::string _iceServer;
    std::string _sourceUrl;
    std::string _peerId;
    std::string _lastError;
    SignallingEndpoint _endpoint;

    icy::uv::Loop* _loop = nullptr;
    uv_async_t _stopAsync{};
    bool _stopAsyncInitialized = false;
    std::thread _loopThread;

    std::unique_ptr<icy::smpl::Client> _client;
    std::unique_ptr<icy::wrtc::SympleSignaller> _signaller;
    std::unique_ptr<icy::wrtc::PeerSession> _session;
    std::shared_ptr<icy::av::MediaCapture> _capture;
    std::shared_ptr<icy::av::VideoPacketEncoder> _encoder;
    std::unique_ptr<icy::PacketStream> _stream;

    bool _started = false;
    bool _stopping = false;
    bool _pipelineReady = false;
};


template <typename Callback>
icy_graft_status_t withPipeline(icy_pipeline_t* pipeline, Callback&& callback)
{
    if (!pipeline)
        return ICY_GRAFT_STATUS_INVALID_ARGUMENT;

    try {
        return callback(*reinterpret_cast<Pipeline*>(pipeline));
    }
    catch (const std::bad_alloc&) {
        return ICY_GRAFT_STATUS_ERROR;
    }
    catch (const std::exception&) {
        return ICY_GRAFT_STATUS_ERROR;
    }
}


icy_graft_status_t pipelineCreate(const icy_pipeline_options_t* options,
                                  icy_pipeline_t** outPipeline)
{
    if (!outPipeline)
        return ICY_GRAFT_STATUS_INVALID_ARGUMENT;

    *outPipeline = nullptr;

    try {
        auto pipeline = std::make_unique<Pipeline>(options);
        *outPipeline = reinterpret_cast<icy_pipeline_t*>(pipeline.release());
        return ICY_GRAFT_STATUS_OK;
    }
    catch (const std::bad_alloc&) {
        return ICY_GRAFT_STATUS_ERROR;
    }
    catch (const std::invalid_argument&) {
        return ICY_GRAFT_STATUS_INVALID_ARGUMENT;
    }
    catch (const std::exception&) {
        return ICY_GRAFT_STATUS_ERROR;
    }
}


void pipelineDestroy(icy_pipeline_t* pipeline)
{
    delete reinterpret_cast<Pipeline*>(pipeline);
}


icy_graft_status_t pipelineAttachRtspSource(icy_pipeline_t* pipeline,
                                            const char* url)
{
    return withPipeline(pipeline, [url](Pipeline& target) {
        return target.attachRtspSource(url);
    });
}


icy_graft_status_t pipelineAttachWebRtcSender(icy_pipeline_t* pipeline,
                                              const char* signallingUrl,
                                              const char* peerId)
{
    return withPipeline(pipeline, [signallingUrl, peerId](Pipeline& target) {
        return target.attachWebRtcSender(signallingUrl, peerId);
    });
}


icy_graft_status_t pipelineStart(icy_pipeline_t* pipeline)
{
    return withPipeline(pipeline, [](Pipeline& target) {
        return target.start();
    });
}


icy_graft_status_t pipelineStop(icy_pipeline_t* pipeline)
{
    return withPipeline(pipeline, [](Pipeline& target) {
        return target.stop();
    });
}


const char* pipelineLastError(const icy_pipeline_t* pipeline)
{
    if (!pipeline)
        return nullptr;

    return reinterpret_cast<const Pipeline*>(pipeline)->lastError();
}


const icy_pipeline_api_t PIPELINE_API = {
    ICY_PIPELINE_API_ABI_VERSION,
    static_cast<std::uint32_t>(sizeof(icy_pipeline_api_t)),
    pipelineCreate,
    pipelineDestroy,
    pipelineAttachRtspSource,
    pipelineAttachWebRtcSender,
    pipelineStart,
    pipelineStop,
    pipelineLastError,
};


} // namespace


ICY_GRAFT_HOST_SURFACE(icy_pipeline_api_manifest,
                       "icy.pipeline",
                       "icey Pipeline API",
                       "0.1.0",
                       ICY_PIPELINE_API_ENTRYPOINT)


extern "C" ICY_GRAFT_EXPORT const icy_pipeline_api_t* icy_pipeline_api()
{
    return &PIPELINE_API;
}
