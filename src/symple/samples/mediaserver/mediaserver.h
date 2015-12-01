#ifndef SCY_MediaServer_H
#define SCY_MediaServer_H


#include "scy/logger.h"
#include "scy/application.h"
#include "scy/util.h"
#include "scy/packetstream.h"
#include "scy/media/avencoder.h"
#include "scy/media/mediafactory.h"
#include "scy/http/server.h"
#include "config.h"


namespace scy { 
	

class MediaServer;


// ----------------------------------------------------------------------------
// HTTP Streaming Options
//
struct StreamingOptions: public av::EncoderOptions
{	
	std::string framing;		// HTTP response framing [chunked, multipart]
	std::string encoding;		// The packet content encoding method [Base64, ...]
		
	MediaServer* server;		// Media server instance
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
class MediaServer: public http::Server
{
public:
	MediaServer(UInt16 port);
	virtual ~MediaServer();

	static void setupPacketStream(PacketStream& stream, const StreamingOptions& options, bool freeCaptures = true, bool attachPacketizers = false);
};


// ----------------------------------------------------------------------------
// HTTP Streaming Connection Factory
//
class HTTPStreamingConnectionFactory: public http::ServerResponderFactory
{
public:
	HTTPStreamingConnectionFactory(MediaServer* server);	
	
	http::ServerResponder* createResponder(http::ServerConnection& conn);

	MediaServer* _server;
};


} // namespace scy


#endif