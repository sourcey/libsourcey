#include "mediaserver.h"
#include "scy/http/websocket.h"


namespace scy {


// ----------------------------------------------------------------------------
//
class WebSocketRequestHandler : public http::ServerResponder
{
public:
    WebSocketRequestHandler(http::ServerConnection& connection, const StreamingOptions& options)
        : http::ServerResponder(connection)
        , options(options)
    {
        SDebug << "Create" << std::endl;

        // Create the packet stream
        MediaServer::setupPacketStream(stream, options); 
        
        // Start the stream
        stream.emitter += packetSlot(this, &WebSocketRequestHandler::onVideoEncoded);
        stream.start();
    }

    virtual ~WebSocketRequestHandler()
    {
        SDebug << "Destroy" << std::endl;
    }

    void onClose()
    {
        SDebug << "On close" << std::endl;

        stream.emitter -= packetSlot(this, &WebSocketRequestHandler::onVideoEncoded);
        stream.stop();
    }

    void onVideoEncoded(RawPacket& packet)
    {
        SDebug << "Sending Packet: "
                     // assert(!connection().socket()->closed());
                     << &connection() << ": " << packet.size() << ": "
                     << fpsCounter.fps << std::endl;

        try {
            connection().socket()->send(packet.data(), packet.size(), http::ws::Binary);
            // connection().sendData(packet.data(), packet.size(), http::WebSocket::Binary);
            fpsCounter.tick();
        } catch (std::exception& exc) {
            SError << "Error: " << exc.what() << std::endl;
            connection().close();
        }
    }

    PacketStream stream;
    StreamingOptions options;
    av::FPSCounter fpsCounter;
};


} // namespace scy
