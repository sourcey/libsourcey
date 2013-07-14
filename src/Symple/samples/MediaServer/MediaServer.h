#ifndef SOURCEY_MediaServer_H
#define SOURCEY_MediaServer_H

#include "Sourcey/Logger.h"
#include "Sourcey/Application.h"
#include "Sourcey/Util.h"
#include "Sourcey/SyncPacketStream.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/HTTP/Server.h"

/*
//#include "Sourcey/Crypto.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/HTTP/WebSocket.h"

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

	StreamingOptions(MediaServer* server = NULL, 
		av::VideoCapture* videoCapture = NULL,
		av::AudioCapture* audioCapture = NULL);

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

	static void setupPacketStream(PacketStream& stream, const StreamingOptions& options, bool freeAudio = true, bool attachPacketizers = false);
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