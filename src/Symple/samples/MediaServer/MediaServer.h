#ifndef SCY_MediaServer_H
#define SCY_MediaServer_H

#include "scy/logger.h"
#include "scy/application.h"
#include "scy/util.h"
#include "scy/packetstream.h"
#include "scy/media/avencoder.h"
#include "scy/media/mediafactory.h"
#include "scy/http/server.h"

/*
//#include "scy/crypto.h"
#include "scy/packetstream.h"
#include "scy/http/websocket.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>
*/


/*
// Detect Win32 memory Leaks - leak free!
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif

using namespace std;
using namespace scy;
using namespace scy::av;
*/



namespace scy { 
	

class MediaServer;


// ----------------------------------------------------------------------------
// HTTP Streaming Options
//
struct StreamingOptions: public av::RecordingOptions
{	
	std::string packetizer;		// HTTP response packetizer [chunked, multipart]
	std::string encoding;		// The packet content encoding method [Base64, ...]
		
	MediaServer* server;		// Media server instance
	av::VideoCapture* videoCapture; // Video capture instance
	av::AudioCapture* audioCapture; // Audio capture instance

	StreamingOptions(MediaServer* server = nullptr, 
		av::VideoCapture* videoCapture = nullptr,
		av::AudioCapture* audioCapture = nullptr);

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