
/*
#include "Sourcey/Logger.h"
#include "Sourcey/Application.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Crypto.h"
#include "Sourcey/Util.h"

#include "Sourcey/HTTP/Server.h"

#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Media/FLVMetadataInjector.h"
#include "Sourcey/Media/FormatRegistry.h"

#include "Sourcey/HTTP/Util.h"
#include "Sourcey/HTTP/Packetizers.h"
#include "Sourcey/Util/Base64PacketEncoder.h"
#include "Sourcey/Util/StreamContainers.h"

#include "Sourcey/TURN/client/TCPClient.h"
#include "Sourcey/Containers.h"

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

#include "Sourcey/HTTP/WebSocket.h"


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
		stream.Emitter += packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
		stream.start();
	}

	virtual ~WebSocketRequestHandler() 
	{
		debugL("WebSocketRequestHandler", this) << "Destroy" << std::endl;			
	}
		
	void onClose() 
	{
		debugL("WebSocketRequestHandler", this) << "On close" << std::endl;
		
		stream.Emitter -= packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
		stream.stop();	
	}
	
	void onVideoEncoded(void* sender, RawPacket& packet)
	{
		debugL("WebSocketRequestHandler", this) << "Sending Packet: "
			<< &connection() << ": " << packet.size() << ": " << fpsCounter.fps << std::endl;
		//assert(!connection().socket().closed());

		try
		{	
			connection().send(packet.data(), packet.size(), http::WebSocket::FRAME_BINARY);
			fpsCounter.tick();		
		}
		catch (Exception& exc)
		{
			errorL("StreamingRequestHandler", this) << "Error: " << exc.message() << std::endl;
			connection().close();
		}
	}
	
	PacketStream stream;
	StreamingOptions options;
	av::FPSCounter fpsCounter;
};


} // namespace scy

