///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#include "whip-session-manager.h"

#include "icy/application.h"
#include "icy/buffer.h"
#include "icy/http/request.h"
#include "icy/http/response.h"
#include "icy/http/server.h"
#include "icy/logger.h"
#include "icy/net/address.h"
#include "icy/net/sslmanager.h"
#include "icy/net/sslsocket.h"
#include "icy/timer.h"
#include "icy/util.h"

#include <filesystem>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace icy {
namespace {

constexpr std::string_view kEndpoint = "/whip";
constexpr size_t kMaxBodySize = 1024 * 1024;

struct Options
{
    std::string host = "127.0.0.1";
    uint16_t port = 8080;
    std::string tlsCert;
    std::string tlsKey;
    std::string authToken;
    std::filesystem::path outputDir = "./recordings";
    size_t maxSessions = 16;
    std::vector<std::string> iceServers;
};


http::StatusCode statusFromInt(int status)
{
    switch (status) {
    case 200: return http::StatusCode::OK;
    case 201: return http::StatusCode::Created;
    case 204: return http::StatusCode::NoContent;
    case 400: return http::StatusCode::BadRequest;
    case 401: return http::StatusCode::Unauthorized;
    case 404: return http::StatusCode::NotFound;
    case 412: return http::StatusCode::PreconditionFailed;
    case 413: return http::StatusCode::EntityTooLarge;
    case 415: return http::StatusCode::UnsupportedMediaType;
    case 422: return http::StatusCode::UnprocessableEntity;
    case 503: return http::StatusCode::Unavailable;
    default: return http::StatusCode::InternalServerError;
    }
}


std::string pathOnly(std::string uri)
{
    if (auto query = uri.find('?'); query != std::string::npos)
        uri.erase(query);
    if (auto fragment = uri.find('#'); fragment != std::string::npos)
        uri.erase(fragment);
    return uri;
}


bool isEndpointPath(const std::string& path)
{
    return path == kEndpoint || path == std::string(kEndpoint) + "/";
}


std::optional<std::string> sessionIdFromPath(const std::string& path)
{
    const std::string prefix = std::string(kEndpoint) + "/";
    if (path.rfind(prefix, 0) != 0)
        return std::nullopt;
    auto id = path.substr(prefix.size());
    if (id.empty() || id.find('/') != std::string::npos)
        return std::nullopt;
    return id;
}


bool isWhipPath(const std::string& path)
{
    return isEndpointPath(path) || sessionIdFromPath(path).has_value();
}


bool isUnsignedDecimal(std::string_view value)
{
    if (value.empty())
        return false;
    for (const char ch : value) {
        if (ch < '0' || ch > '9')
            return false;
    }
    return true;
}


uint16_t parsePort(std::string_view value)
{
    if (!isUnsignedDecimal(value))
        throw std::invalid_argument("invalid --port: " + std::string(value));

    const auto port = std::stoul(std::string(value));
    if (port == 0 || port > 65535)
        throw std::invalid_argument("invalid --port: " + std::string(value));

    return static_cast<uint16_t>(port);
}


size_t parseMaxSessions(std::string_view value)
{
    if (!isUnsignedDecimal(value))
        throw std::invalid_argument("invalid --max-sessions: " + std::string(value));

    const auto count = std::stoull(std::string(value));
    if (count == 0 || count > std::numeric_limits<size_t>::max())
        throw std::invalid_argument("invalid --max-sessions: " + std::string(value));

    return static_cast<size_t>(count);
}


bool mediaTypeIs(const std::string& contentType, std::string_view expected)
{
    auto mediaType = contentType;
    if (auto semicolon = mediaType.find(';'); semicolon != std::string::npos)
        mediaType.erase(semicolon);
    util::trimInPlace(mediaType);
    return util::icompare(mediaType, expected) == 0;
}


std::string originFor(const http::Request& request, const http::ServerConnection& connection)
{
    const auto& host = request.get("Host", http::Message::EMPTY);
    return std::string(connection.secure() ? "https://" : "http://") +
           (host.empty() ? "localhost" : host);
}


void addWhipHeaders(http::Response& response)
{
    response.set("Access-Control-Allow-Origin", "*");
    response.set("Access-Control-Allow-Methods", "OPTIONS, POST, PATCH, DELETE, GET");
    response.set("Access-Control-Allow-Headers", "Authorization, Content-Type, If-Match");
    response.set("Access-Control-Expose-Headers", "Location, ETag, Accept-Post");
}


class WhipResponder final : public http::ServerResponder
{
public:
    WhipResponder(http::ServerConnection& connection,
                  WhipSessionManager& sessions,
                  std::string authToken)
        : http::ServerResponder(connection)
        , _sessions(sessions)
        , _authToken(std::move(authToken))
    {
    }

    void onPayload(const MutableBuffer& body) override
    {
        if (_body.size() + body.size() > kMaxBodySize) {
            _bodyTooLarge = true;
            return;
        }
        const auto* data = bufferCast<const char*>(body);
        _body.append(data, body.size());
    }

    void onRequest(http::Request& request, http::Response& response) override
    {
        addWhipHeaders(response);
        response.set("Connection", "close");

        if (_bodyTooLarge) {
            send(response, http::StatusCode::EntityTooLarge, "request body too large\n");
            return;
        }

        const auto path = pathOnly(request.getURI());
        const auto& method = request.getMethod();

        if (util::icompare(method, http::Method::Options) == 0) {
            if (!isWhipPath(path)) {
                send(response, http::StatusCode::NotFound, "not found\n");
                return;
            }
            response.set("Accept-Post", "application/sdp");
            sendNoBody(response, http::StatusCode::OK);
            return;
        }

        if (util::icompare(method, http::Method::Get) == 0) {
            if (!isEndpointPath(path)) {
                send(response, http::StatusCode::NotFound, "not found\n");
                return;
            }
            sendNoBody(response, http::StatusCode::NoContent);
            return;
        }

        if (!authorized(request)) {
            response.set("WWW-Authenticate", "Bearer");
            send(response, http::StatusCode::Unauthorized, "unauthorized\n");
            return;
        }

        if (util::icompare(method, http::Method::Post) == 0) {
            handlePost(request, response, path);
            return;
        }

        if (util::icompare(method, "PATCH") == 0) {
            handlePatch(request, response, path);
            return;
        }

        if (util::icompare(method, http::Method::Delete) == 0) {
            handleDelete(response, path);
            return;
        }

        send(response, http::StatusCode::NotFound, "not found\n");
    }

private:
    bool authorized(const http::Request& request) const
    {
        if (_authToken.empty())
            return true;
        const auto expected = "Bearer " + _authToken;
        return request.get("Authorization", http::Message::EMPTY) == expected;
    }

    void handlePost(http::Request& request,
                    http::Response& response,
                    const std::string& path)
    {
        if (!isEndpointPath(path)) {
            send(response, http::StatusCode::NotFound, "not found\n");
            return;
        }
        if (!mediaTypeIs(request.getContentType(), "application/sdp")) {
            send(response, http::StatusCode::UnsupportedMediaType,
                 "expected Content-Type: application/sdp\n");
            return;
        }
        if (_body.empty()) {
            send(response, http::StatusCode::BadRequest, "empty SDP offer\n");
            return;
        }

        auto created = _sessions.create(_body);
        if (!created.session) {
            send(response, statusFromInt(created.status), created.error + "\n");
            return;
        }

        response.setStatus(http::StatusCode::Created);
        response.setContentType("application/sdp");
        response.set("Location", originFor(request, connection()) + std::string(kEndpoint) +
                                 "/" + created.session->id());
        response.set("ETag", created.session->etag());
        response.setContentLength(created.answerSdp.size());
        connection().sendHeader();
        connection().sendOwned(Buffer(created.answerSdp.begin(), created.answerSdp.end()));
        connection().close();
    }

    void handlePatch(http::Request& request,
                     http::Response& response,
                     const std::string& path)
    {
        auto id = sessionIdFromPath(path);
        if (!id) {
            send(response, http::StatusCode::NotFound, "not found\n");
            return;
        }
        if (!mediaTypeIs(request.getContentType(), "application/trickle-ice-sdpfrag")) {
            send(response, http::StatusCode::UnsupportedMediaType,
                 "expected Content-Type: application/trickle-ice-sdpfrag\n");
            return;
        }

        auto session = _sessions.find(*id);
        if (!session) {
            send(response, http::StatusCode::NotFound, "session not found\n");
            return;
        }

        const auto& ifMatch = request.get("If-Match", http::Message::EMPTY);
        if (!ifMatch.empty() && ifMatch != "*" && ifMatch != session->etag()) {
            send(response, http::StatusCode::PreconditionFailed, "ETag mismatch\n");
            return;
        }

        session->addRemoteCandidates(WhipSession::parseCandidateFragment(_body));
        sendNoBody(response, http::StatusCode::NoContent);
    }

    void handleDelete(http::Response& response, const std::string& path)
    {
        auto id = sessionIdFromPath(path);
        if (!id || !_sessions.remove(*id, "client request")) {
            send(response, http::StatusCode::NotFound, "session not found\n");
            return;
        }
        sendNoBody(response, http::StatusCode::OK);
    }

    void sendNoBody(http::Response& response, http::StatusCode status)
    {
        response.setStatus(status);
        response.setContentLength(0);
        connection().sendHeader();
        connection().close();
    }

    void send(http::Response& response,
              http::StatusCode status,
              const std::string& body)
    {
        response.setStatus(status);
        response.setContentType("text/plain");
        response.setContentLength(body.size());
        connection().sendHeader();
        if (!body.empty())
            connection().sendOwned(Buffer(body.begin(), body.end()));
        connection().close();
    }

    WhipSessionManager& _sessions;
    std::string _authToken;
    std::string _body;
    bool _bodyTooLarge = false;
};


class WhipResponderFactory final : public http::ServerConnectionFactory
{
public:
    WhipResponderFactory(WhipSessionManager& sessions, std::string authToken)
        : _sessions(sessions)
        , _authToken(std::move(authToken))
    {
    }

    std::unique_ptr<http::ServerResponder> createResponder(
        http::ServerConnection& connection) override
    {
        return std::make_unique<WhipResponder>(connection, _sessions, _authToken);
    }

private:
    WhipSessionManager& _sessions;
    std::string _authToken;
};


bool parseArgs(int argc, char** argv, Options& options)
{
    for (int i = 1; i < argc; ++i) {
        std::string key = argv[i];
        auto next = [&]() -> std::string {
            if (i + 1 >= argc)
                throw std::invalid_argument("missing value for " + key);
            return argv[++i];
        };

        if (key == "--help" || key == "-h") {
            return false;
        }
        if (key == "--host") options.host = next();
        else if (key == "--port") options.port = parsePort(next());
        else if (key == "--tls-cert") options.tlsCert = next();
        else if (key == "--tls-key") options.tlsKey = next();
        else if (key == "--auth-token") options.authToken = next();
        else if (key == "--output-dir") options.outputDir = next();
        else if (key == "--max-sessions") options.maxSessions = parseMaxSessions(next());
        else if (key == "--stun") options.iceServers.push_back(next());
        else throw std::invalid_argument("unknown option: " + key);
    }
    return true;
}


void printUsage(const char* argv0)
{
    std::cout
        << "Usage: " << argv0 << " [options]\n"
        << "  --host <host>             Bind host (default 127.0.0.1)\n"
        << "  --port <port>             Bind port (default 8080)\n"
        << "  --tls-cert <path>         PEM certificate for HTTPS\n"
        << "  --tls-key <path>          PEM private key for HTTPS\n"
        << "  --auth-token <token>      Optional Bearer token\n"
        << "  --output-dir <path>       Recording directory (default ./recordings)\n"
        << "  --max-sessions <n>        Concurrent session cap (default 16)\n"
        << "  --stun <url>              ICE server, repeatable\n";
}

} // namespace
} // namespace icy


int main(int argc, char** argv)
{
    using namespace icy;

    Logger::instance().add(std::make_unique<ConsoleChannel>("debug", Level::Debug));

    Options options;
    try {
        if (!parseArgs(argc, argv, options)) {
            printUsage(argv[0]);
            return 0;
        }
    }
    catch (const std::exception& exc) {
        std::cerr << exc.what() << '\n';
        printUsage(argv[0]);
        return 1;
    }

    const bool tlsConfigured = !options.tlsCert.empty() || !options.tlsKey.empty();
    if (tlsConfigured && (options.tlsCert.empty() || options.tlsKey.empty())) {
        std::cerr << "--tls-cert and --tls-key must be supplied together\n";
        return 1;
    }

    WhipSessionManager sessions(options.outputDir, options.maxSessions, options.iceServers);
    auto factory = std::make_unique<WhipResponderFactory>(sessions, options.authToken);
    const net::Address address(options.host, options.port);

    std::unique_ptr<http::Server> server;
    if (tlsConfigured) {
        net::SSLManager::initNoVerifyServer(options.tlsKey, options.tlsCert);
        server = std::make_unique<http::Server>(
            address,
            std::make_shared<net::SSLSocket>(
                net::SSLManager::instance().defaultServerContext()),
            std::move(factory));
    }
    else {
        server = std::make_unique<http::Server>(address, uv::defaultLoop(), std::move(factory));
    }

    Timer sweepTimer(5000, 5000);
    sweepTimer.Timeout += [&sessions]() { sessions.sweepExpired(); };
    sweepTimer.start();

    server->start();
    std::cout << "WHIP receiver listening on "
              << (tlsConfigured ? "https://" : "http://")
              << options.host << ":" << options.port << "/whip\n"
              << "Recordings: " << options.outputDir << '\n';

    waitForShutdown([&](void*) {
        sweepTimer.stop();
        server->stop();
        sessions.sweepExpired();
        net::SSLManager::destroy();
    });

    Logger::destroy();
    return 0;
}
