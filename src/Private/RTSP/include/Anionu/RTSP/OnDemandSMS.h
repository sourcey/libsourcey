#ifndef ON_DEMAND_SMS_H
#define ON_DEMAND_SMS_H


#include "Common.h"

#include <OnDemandServerMediaSubsession.hh>
#include <AudioInputDevice.hh>

#include "H264VideoSource.h"
#include "H263PVideoSource.h"
#include "H263VideoSource.h"
#include "MJPEGVideoSource.h"

#include "PCMToMP3Filter.h"
#include "PCMToAMRFilter.h"


namespace Anionu {
namespace RTSP {


/**
 * On Demand Server Media Subsession.
 * It gives sink and source classes when requested by the streams.
 * Derives OnDemandServerMediaSubsession of LiveMedia.
 */
class OnDemandSMS : public OnDemandServerMediaSubsession
{
public:
	//
 	// Static method.
	// @param env UsageEnvironment, see LiveMedia doc. here
	//        http://www.live555.com/liveMedia/public/doxygen/html
	// @param reuseFirstSource Should the class reuse the source when finished another one?
	// @param fps Frames Per Second media output.
	// @param videoStream A VideoStream class, can be a MultiVideoSource or a 
	//        SingleVideoSource kind of class.
	// @param encodingCodec Stream encoding parameters.
	// @param audioId Audio port mapped from the system. It is used only if 
	//        videoStream is NULL, and vice versa
	// @param streamId Stream id. Defaults to 0.
	//
  	static OnDemandSMS *
	createNew(UsageEnvironment& env, bool reuseFirstSource, int fps, Anionu::Input::VideoStream *videoStream,
			   std::string codecId, int audioId, int streamId = 0);

	virtual ~OnDemandSMS();

private: // Functions
	//
	// Constructor.
	// @see createNew
	//
	OnDemandSMS(UsageEnvironment& env, bool reuseFirstSource, int fps, Anionu::Input::VideoStream *videoStream,  
					    std::string codecId, int audioId, int streamId);

	//
	// Creates a new stream source.
	// @param clientSessionId
	// @param estBitrate It's the stream's estimated bitate, in kbps.
	// @return A FramedSource instance. See liveMedia doc for this. 
	// @see http://www.live555.com/liveMedia/public/doxygen/html
	//
	virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);

	//
	// Creates a new RTP sink.
	// @param rtpGroupsock 
	// @param rtpPayloadTypeIfDynamic
	// @param inputSource
	// @return A new RTPSink instance.
	// @see http://www.live555.com/liveMedia/public/doxygen/html
	//
	virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
				    unsigned char rtpPayloadTypeIfDynamic,
				    FramedSource* inputSource);

private: // Data
	// VideoStream instance */
	Anionu::Input::VideoStream *_videoStream;
	
	// 
	// Encoding type.
	//	
	std::string _codecId;
	
	// Frames per second.
	int _fps;

	// Stream id
	int _streamId;
	
	// Audio port mapped by the system
	int _audioPort;
};


} } // namespace Anionu::RTSP


#endif	//ON_DEMAND_SMS_H

