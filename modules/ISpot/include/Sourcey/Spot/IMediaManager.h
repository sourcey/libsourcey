#ifndef ANIONU_SPOT_IMediaManager_H
#define ANIONU_SPOT_IMediaManager_H


#include "Sourcey/TimedManager.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Media/Format.h"
#include "Sourcey/Logger.h"

#include "Sourcey/Media/CaptureFactory.h"
#include "Sourcey/Media/FormatRegistry.h"
#include "Sourcey/Media/DeviceManager.h"


namespace Sourcey {
namespace Spot {


class IChannel;


struct RecordingInfo 
{
	std::string token;
	Media::IPacketEncoder* encoder;
	RecordingInfo(const std::string& token = "", Media::IPacketEncoder* encoder = NULL) : 
		token(token), 
		encoder(encoder) {}
};


class IMediaManager: public TimedManager<std::string, PacketStream>, public ILoggable
{
public:
	typedef TimedManager<std::string, PacketStream>	Manager;
	typedef Manager::Base::Map						Map;

public:
	IMediaManager();
	virtual ~IMediaManager();

	virtual void initializeParams(IChannel& channel, Media::RecorderParams& params) = 0;
		/// Initializes default recorder parameters based on the
		/// current configuration.
	
	virtual RecordingInfo startRecording(IChannel& channel, const Media::RecorderParams& params) = 0;
		/// Initializes a recorder instance for the current channel.
		/// The default encoder format parameters are provided by
		/// the application configuration.
		/// The returned string is the recorder's index token used 
		/// as a reference by the StreamManager.

	virtual bool stopRecording(const std::string& token, bool whiny = true) = 0;
		/// Stops the recorder session matching the given token.

	Signal2<const Media::RecorderParams&, Media::IPacketEncoder*&> InitializeEncoder;
		/// Provides plugins with the ability to override creation
		/// of the default recording encoder. A valid IPacketEncoder
		/// instance must be returned. Encoders should always use
		/// the channel's device sources.
	
	virtual Media::FormatRegistry& streamingFormats() = 0;
	virtual Media::FormatRegistry& recordingFormats() = 0;
	
	virtual const char* className() const { return "MediaManager"; }
};


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IMediaManager_H