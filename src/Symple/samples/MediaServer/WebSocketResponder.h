
/*
#include "scy/logger.h"
#include "scy/application.h"
#include "scy/packetstream.h"
#include "scy/packetstream.h"
#include "scy/crypto.h"
#include "scy/util.h"

#include "scy/http/server.h"

#include "scy/media/mediafactory.h"
#include "scy/media/avencoder.h"
#include "scy/media/avinputreader.h"
#include "scy/media/flvmetadatainjector.h"
#include "scy/media/formatregistry.h"

#include "scy/http/util.h"
#include "scy/http/packetizers.h"
#include "scy/util/base64packetencoder.h"
#include "scy/util/streamcollection.h"

#include "scy/turn/client/tcpclient.h"
#include "scy/collection.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


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

#include "MediaServer.h"

#include "scy/http/websocket.h"


namespace scy { 

// ----------------------------------------------------------------------------
//
class WebSocketRequestHandler: public http::ServerResponder
{
public:
	WebSocketRequestHandler(http::ServerConnection& connection, const StreamingOptions& options) : 		
		http::ServerResponder(connection), options(options)
	{	
		debugL("WebSocketRequestHandler", this) << "Create" << std::endl;
		
		// Create the packet stream
		MediaServer::setupPacketStream(stream, options);

		// Start the stream
		stream.emitter += packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
		stream.start();
	}

	virtual ~WebSocketRequestHandler() 
	{
		debugL("WebSocketRequestHandler", this) << "Destroy" << std::endl;			
	}
		
	void onClose() 
	{
		debugL("WebSocketRequestHandler", this) << "On close" << std::endl;
		
		stream.emitter -= packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
		stream.stop();	
	}
	
	void onVideoEncoded(void* sender, RawPacket& packet)
	{
		debugL("WebSocketRequestHandler", this) << "Sending Packet: "
			<< &connection() << ": " << packet.size() << ": " << fpsCounter.fps << std::endl;
		//assert(!connection().socket().closed());

		try
		{				
			connection().socket().send(packet.data(), packet.size(), http::WebSocket::Binary);
			//connection().sendData(packet.data(), packet.size(), http::WebSocket::Binary);
			fpsCounter.tick();		
		}
		catch (std::exception& exc)
		{
			errorL("StreamingRequestHandler", this) << "Error: " << exc.what() << std::endl;
			connection().close();
		}
	}
	
	PacketStream stream;
	StreamingOptions options;
	av::FPSCounter fpsCounter;
};


} // namespace scy

