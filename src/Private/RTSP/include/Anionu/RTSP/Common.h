#ifndef RTSP_COMMON_H
#define	RTSP_COMMON_H


#include <GroupsockHelper.hh>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Poco/Foundation.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"

#include <time.h>

#include "Anionu/Common/DataTypes.h"
#include "Anionu/AV/AudioEncoder.h"
#include "Anionu/AV/VideoEncoder.h"
#include "Anionu/AV/AVWriter.h"
#include "Anionu/Input/AudioInput.h"
#include "Anionu/Input/VideoInput.h"


namespace Anionu {
namespace RTSP {


struct StreamerState {				
	ConnectionState state;
	ChannelsData channels;
	StreamerState(
		ConnectionState state=StateDisconnected,
		ChannelsData channels=ChannelsData()) :
			state(state),
			channels(channels) {}
};


struct StreamingParams {
	ChannelsData channels;
	short primaryPortNum;
	short fallbackPortNum;
	short activePortNum;
	std::string remoteIp;
	StreamingParams(
		short primaryPortNum=554,
		short fallbackPortNum=7070,
		short fps=20) :
			primaryPortNum(primaryPortNum),
			fallbackPortNum(fallbackPortNum),
			activePortNum(0),
			remoteIp("") {}
};


#define OUTPUT_PACKET_BUFFER_MAXSIZE 512000

#define MAX_AUDIO_PACKET_SIZE (128 * 1024)

#define MAX_VIDEO_PACKET_SIZE (512 * 1024) 


} } // namespace Anionu::RTSP


#endif

