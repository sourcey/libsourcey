#include "mediaserver.h"


namespace scy { 


class StreamingRequestHandler: public http::ServerResponder
{
public:
    StreamingRequestHandler(http::ServerConnection& connection, const StreamingOptions& options) :
        http::ServerResponder(connection), options(options)
    {    
        DebugLS(this) << "Create" << std::endl;
    }

    virtual ~StreamingRequestHandler() 
    {
        DebugLS(this) << "Destroy" << std::endl;
    }
        
    virtual void onRequest(http::Request& request, http::Response& response) 
    {
        DebugLS(this) << "Handle request: " 
            //<< "\n\tOutput Format: " << options.oformat.name
            << "\n\tOutput Encoding: " << options.encoding
            << "\n\tOutput Packetizer: " << options.framing
            << std::endl;

        // We will be sending our own headers
        connection().shouldSendHeader(false);    

        // Create the packet stream
        MediaServer::setupPacketStream(stream, options, true, true);

        // Start the stream
        stream.emitter += packetDelegate(this, &StreamingRequestHandler::onVideoEncoded);
        stream.start();
    }

    virtual void onClose()
    {
        DebugLS(this) << "On close" << std::endl;
        stream.emitter -= packetDelegate(this, &StreamingRequestHandler::onVideoEncoded);
        stream.stop();
    }

    void onVideoEncoded(void* sender, RawPacket& packet)
    {
        DebugLS(this) << "Send packet: " 
            << packet.size() << ": " << fpsCounter.fps << std::endl;
        //assert(!connection().socket()->closed());

        try {    
            connection().socket()->send((const char*)packet.data(), packet.size());
            fpsCounter.tick();        
        }
        catch (std::exception& exc) {
            ErrorLS(this) << exc.what() << std::endl;
            connection().close();
        }
    }
    
    PacketStream stream;
    StreamingOptions options;
    av::FPSCounter fpsCounter;
};


} // namespace scy