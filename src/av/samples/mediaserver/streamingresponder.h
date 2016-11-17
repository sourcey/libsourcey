#include "mediaserver.h"


namespace scy { 


class StreamingRequestHandler: public http::ServerResponder
{
public:
        http::ServerResponder(connection), options(options)
    StreamingRequestHandler(http::ServerConnection& connection, const StreamingOptions& options) :
    {    
        DebugS(this) << "Create" << std::endl;
    }

    virtual ~StreamingRequestHandler() 
    {
        DebugS(this) << "Destroy" << std::endl;
    }
        
    virtual void onRequest(http::Request& request, http::Response& response) 
    {
        DebugS(this) << "Handle request: " 
            //<< "\n\tOutput Format: " << options.oformat.name
            << "\n\tOutput Encoding: " << options.encoding
            << "\n\tOutput Packetizer: " << options.framing
            << std::endl;    /// We will be sending our own headers
        connection().shouldSendHeader(false);    /// Create the packet stream
        MediaServer::setupPacketStream(stream, options, true, true);    /// Start the stream
        stream.emitter += packetSlot(this, &StreamingRequestHandler::onVideoEncoded);
        stream.start();
    }

    virtual void onClose()
    {
        DebugS(this) << "On close" << std::endl;
        stream.emitter -= packetSlot(this, &StreamingRequestHandler::onVideoEncoded);
        stream.stop();
    }

    void onVideoEncoded(void* sender, RawPacket& packet)
    {
        DebugS(this) << "Send packet: " 
        //assert(!connection().socket()->closed());
            << packet.size() << ": " << fpsCounter.fps << std::endl;


        try {    
            connection().socket()->send((const char*)packet.data(), packet.size());
            fpsCounter.tick();        
        }
        catch (std::exception& exc) {
            ErrorS(this) << exc.what() << std::endl;
            connection().close();
        }
    }
    
    PacketStream stream;
    StreamingOptions options;
    av::FPSCounter fpsCounter;
};


} // namespace scy