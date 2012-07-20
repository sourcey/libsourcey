#ifndef ANIONU_MEDIA_IRecorder_H
#define ANIONU_MEDIA_IRecorder_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Base/IPacketBroadcaster.h"
/*
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/CaptureEncoder.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Base/Stateful.h"
#include "Sourcey/Base/Signal.h"
*/

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"

#include "assert.h"


namespace Sourcey {
namespace Media {


	/*
class IRecorder: public IEncoder, public IPacketBroadcaster
{
public:
	IRecorder() {};
	virtual ~IRecorder() {};
	
	virtual void initialize() = 0;
	virtual void uninitialize() = 0;	
	
	//virtual void start() = 0;
	//virtual void stop() = 0;	
	
	virtual RecorderParams& params() = 0;
};
*/


} } // namespace Sourcey::Media


#endif // ANIONU_MEDIA_IRecorder_H