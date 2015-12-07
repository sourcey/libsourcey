#include "mediaserver.h"
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
        DebugLS(this) << "Create" << std::endl;
        
        // Create the packet stream
        MediaServer::setupPacketStream(stream, options);

        // Start the stream
        stream.emitter += packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
        stream.start();
    }

    virtual ~WebSocketRequestHandler() 
    {
        DebugLS(this) << "Destroy" << std::endl;            
    }
        
    void onClose() 
    {
        DebugLS(this) << "On close" << std::endl;
        
        stream.emitter -= packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
        stream.stop();    
    }
    
    void onVideoEncoded(void* sender, RawPacket& packet)
    {
        DebugLS(this) << "Sending Packet: "
            << &connection() << ": " << packet.size() << ": " << fpsCounter.fps << std::endl;
        //assert(!connection().socket()->closed());

        try {                
            connection().socket()->send(packet.data(), packet.size(), http::ws::Binary);
            //connection().sendData(packet.data(), packet.size(), http::WebSocket::Binary);
            fpsCounter.tick();        
        }
        catch (std::exception& exc) {
            ErrorLS(this) << "Error: " << exc.what() << std::endl;
            connection().close();
        }
    }
    
    PacketStream stream;
    StreamingOptions options;
    av::FPSCounter fpsCounter;
};


} // namespace scy

