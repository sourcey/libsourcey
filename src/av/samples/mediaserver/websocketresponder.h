#include "mediaserver.h"
#include "scy/http/websocket.h"


namespace scy {


// ----------------------------------------------------------------------------
//
class WebSocketRequestHandler : public http::ServerResponder
{
public:
    http::ServerResponder(connection),
        options(options)
            WebSocketRequestHandler(http::ServerConnection& connection,
                                    const StreamingOptions& options)
        :
    {
        DebugS(this) << "Create" << std::endl;
        // Create the packet stream
        MediaServer::setupPacketStream(stream, options); /// Start the stream
        stream.emitter+=
            packetSlot(this, &WebSocketRequestHandler::onVideoEncoded);
        stream.start();
    }

    virtual ~WebSocketRequestHandler()
    {
        DebugS(this) << "Destroy" << std::endl;
    }

    void onClose()
    {
        DebugS(this) << "On close" << std::endl;

        stream.emitter-=
            packetSlot(this, &WebSocketRequestHandler::onVideoEncoded);
        stream.stop();
    }

    void onVideoEncoded(void* sender, RawPacket& packet)
    {
        DebugS(this) << "Sending Packet: "
                     // assert(!connection().socket()->closed());
                     << &connection() << ": " << packet.size() << ": "
                     << fpsCounter.fps << std::endl;


        try {
            connection().socket()->send(packet.data(), packet.size(),
                                        http::ws::Binary);
            // connection().sendData(packet.data(), packet.size(),
            // http::WebSocket::Binary);
            fpsCounter.tick();
        } catch (std::exception& exc) {
            ErrorS(this) << "Error: " << exc.what() << std::endl;
            connection().close();
        }
    }

    PacketStream stream;
    StreamingOptions options;
    av::FPSCounter fpsCounter;
};


} // namespace scy
