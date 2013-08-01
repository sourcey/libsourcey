#include "MediaServer.h"


namespace scy { 


class StreamingRequestHandler: public http::ServerResponder
{
public:
	StreamingRequestHandler(http::ServerConnection& connection, const StreamingOptions& options) :
		http::ServerResponder(connection), options(options)
	{	
		debugL("StreamingRequestHandler", this) << "Creating" << std::endl;
	}

	virtual ~StreamingRequestHandler() 
	{
		debugL("StreamingRequestHandler", this) << "Destroying" << std::endl;
	}
		
	virtual void onRequest(http::Request& request, http::Response& response) 
	{
		debugL("StreamingRequestHandler", this) << "Running: " 
			//<< "\n\tOutput Format: " << options.oformat.name
			<< "\n\tOutput Encoding: " << options.encoding
			<< "\n\tOutput Packetizer: " << options.packetizer
			<< std::endl;

		// We will be sending our own headers
		connection().shouldSendHeader(false);	

		// Create the packet stream
		MediaServer::setupPacketStream(stream, options, true, true);

		// Start the stream
		stream.Emitter += packetDelegate(this, &StreamingRequestHandler::onVideoEncoded);
		stream.start();
	}

	virtual void onClose()
	{
		debugL("StreamingRequestHandler", this) << "On close" << std::endl;
		stream.Emitter -= packetDelegate(this, &StreamingRequestHandler::onVideoEncoded);
		stream.stop();
	}

	void onVideoEncoded(void* sender, RawPacket& packet)
	{
		debugL("StreamingRequestHandler", this) << "Sending Packet: " 
			<< packet.size() << ": " << fpsCounter.fps << std::endl;
		//assert(!connection().socket().closed());

		try {	
			connection().send((const char*)packet.data(), packet.size());
			fpsCounter.tick();		
		}
		catch (Exception& exc) {
			errorL("StreamingRequestHandler", this) << "Error: " << exc.message() << std::endl;
			connection().close();
		}
	}
	
	PacketStream stream;
	StreamingOptions options;
	av::FPSCounter fpsCounter;
};


} // namespace scy