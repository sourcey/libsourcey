#ifndef SCY_MediaServer_H
#define SCY_MediaServer_H


#include "config.h"
#include "scy/application.h"
#include "scy/av/mediafactory.h"
#include "scy/av/multiplexencoder.h"
#include "scy/http/server.h"
#include "scy/logger.h"
#include "scy/packetstream.h"
#include "scy/util.h"


namespace scy {


class MediaServer;


// ----------------------------------------------------------------------------
// HTTP Streaming Options
//
struct StreamingOptions : public av::EncoderOptions
{
    std::string framing;  // HTTP response framing [chunked, multipart]
    std::string encoding; // The packet content encoding method [Base64, ...]

    MediaServer* server;                // Media server instance
    av::VideoCapture::Ptr videoCapture; // Video capture instance
    av::AudioCapture::Ptr audioCapture; // Audio capture instance

    StreamingOptions(MediaServer* server = nullptr,
                     av::VideoCapture::Ptr videoCapture = nullptr,
                     av::AudioCapture::Ptr audioCapture = nullptr);

    virtual ~StreamingOptions();
};


// ----------------------------------------------------------------------------
// HTTP Media Server
//
class MediaServer : public http::Server
{
public:
    MediaServer(std::uint16_t port);
    virtual ~MediaServer();

    static void setupPacketStream(PacketStream& stream,
                                  const StreamingOptions& options,
                                  bool freeCaptures = true,
                                  bool attachPacketizers = false);
};


// ----------------------------------------------------------------------------
// HTTP Streaming Connection Factory
//
class HTTPStreamingConnectionFactory : public http::ServerResponderFactory
{
public:
    HTTPStreamingConnectionFactory(MediaServer* server);

    http::ServerResponder* createResponder(http::ServerConnection& conn);
    StreamingOptions createStreamingOptions(http::ServerConnection& conn);
    MediaServer* _server;
};


} // namespace scy


#endif
