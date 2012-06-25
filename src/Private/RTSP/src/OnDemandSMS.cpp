#include "Anionu/RTSP/OnDemandSMS.h"


using namespace std;
using namespace Anionu;
using namespace Anionu::RTSP;
using namespace Anionu::Input;
using namespace Anionu::AV;


// ============================================================================
//
// On Demand Server Media Session
//
// ============================================================================
OnDemandSMS::OnDemandSMS(UsageEnvironment& env, 
	bool reuseFirstSource, 
	int fps,
	VideoStream *videoStream,
	std::string codecId, 
	int audioId,
	int streamId) 
	: OnDemandServerMediaSubsession(env, reuseFirstSource),
		_videoStream(videoStream),
		_fps(fps),
		_codecId(codecId),
		_audioPort(audioId),
		_streamId(streamId)
{	
	cout << "OnDemandSMS::OnDemandSMS()" << endl;
}


OnDemandSMS *
OnDemandSMS::createNew(UsageEnvironment& env, bool reuseFirstSource, int fps, Anionu::Input::VideoStream *videoStream,
			  				   std::string codecId, int audioId, int streamId /* = 0 */) 
{
	return new OnDemandSMS(env, reuseFirstSource, fps, videoStream, codecId, audioId, streamId);
}


// WARNING: This destructor will only be called by Medium::close(RTSPServer)
// if it hasn't been activated yet. ie. it has no RTPSink / FramedSource
// associated with it. To this end RTPSink and FramedSource must be responsible 
// for their own deletion. This appears to be a bug/shortcoming in the Live555 
// libraries.
OnDemandSMS::~OnDemandSMS()
{
	cout << "OnDemandSMS::~OnDemandSMS()" << endl;
}


FramedSource* OnDemandSMS::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) 
{
	cout << "OnDemandSMS::createNewStreamSource()" << endl;
	estBitrate = 64; // kbps, estimate
	FramedSource* framedSource;

	//
	// VIDEO FORMATS
	//

	// H264
	// Currently not implemented!
	if (_codecId == "h264")
	{
		//	cout << "FramedSource: CODEC_ID_H264" << endl;
		//	framedSource = H264VideoSource::createNew(envir(), NULL, _videoStream, _streamId, _fps, 400, 300);
		//	break;
	} 

	// H263P
	else if (_codecId == "h263p") 
	{
		cout << "FramedSource: CODEC_ID_H263P" << endl;
		framedSource = H263PVideoSource::createNew(envir(), _videoStream, _streamId, _fps, 400, 300);
	}

	// H263
	else if (_codecId == "h263") 
	{
		cout << "FramedSource: CODEC_ID_H263" << endl;
		framedSource = H263VideoSource::createNew(envir(), _videoStream, _streamId, _fps);
	}

	// MJPEG
	// Currently not implemented!
	else if (_codecId == "mjpeg")
	{
		framedSource = MJPEGVideoSource::createNew(envir(), _videoStream, _streamId, _fps); //, 400, 300
	}

	//
	// AUDIO FORMATS
	//

	// MP3
	else if (_codecId == "mp3") 
	{
		// Grab audio from microphone
		FramedSource* asPCM = AudioInputDevice::createNew(envir(), 0, 16, 1, 8000, 20);
		FramedSource* asMP3 = PCMToMP3Filter::createNew(envir(), asPCM, 64000, 1, 8000);
#ifdef RTSP_USING_MP3_ADU
		FramedSource* asADU = ADUFro_mp3Source::createNew(envir(), asMP3);
		framedSource = asADU;
#else
		framedSource = asMP3;
#endif
	}

	// AMR-NB
	else if (_codecId == "amr-nb") // Currently not implemented!
	{
		//framedSource = AMRAudioFileSource::createNew(envir(), "c:/Temp/test.amr");
		FramedSource* asPCM = AudioInputDevice::createNew(envir(), 0, 16, 1, 8000, 20);
		framedSource = PCMToMP3Filter::createNew(envir(), asPCM, 16, 1, 8000);
	}

	// Unsupported
	else {
		cout << "Streaming Plugin: Error: Unsupported codec. " << endl;
	}

	return framedSource;
}
  

RTPSink* OnDemandSMS::createNewRTPSink(Groupsock* rtpGroupsock,
				    unsigned char rtpPayloadTypeIfDynamic,
					FramedSource* inputSource) 
{
	cout << "OnDemandSMS::createNewRTPSink()" << endl;
	RTPSink* rtpSink;


	//
	// VIDEO FORMATS
	//

	// H264
	// Currently not implemented!
	if (_codecId == "h264")
	{
		//	cout << "RTPSink: rtpSink CODEC_ENCODING_H264:" << endl;			
		//	//Baseline profile: profile_id 66 = 0x42
		//	//Main profile: profile_id 77 = 0x4D
		//	//Extended profile: profile_id 88 = 0x58		
		//	{
		//		char spropParams[256];
		//		char* params = NULL;	
		//		((H264VideoSource*)inputSource)->getH264SpropParams(&params);
		//		strcpy(spropParams, params);
		//		rtpSink = H264VideoRTPSink::createNew(envir(), rtpGroupsock, 96, 0x42, spropParams);
		//	} 
	} 

	// H263P
	else if (_codecId == "h263p") 
	{
			cout << "RTPSink: CODEC_ID_H263P:" << endl;
			rtpSink = H263plusVideoRTPSink::createNew(envir(), rtpGroupsock, 96);
	}

	// H263
	else if (_codecId == "h263") 
	{
			cout << "RTPSink: CODEC_ID_H263:" << endl;
			rtpSink = H263plusVideoRTPSink::createNew(envir(), rtpGroupsock, 96);
	}

	// MJPEG
	// Currently not implemented!
	else if (_codecId == "mjpeg") 
	{
		//	cout << "RTPSink: CODEC_ID_MJPEG:" << endl;
		//	rtpSink = JPEGVideoRTPSink::createNew(envir(), rtpGroupsock);
	}

	//
	// AUDIO FORMATS
	//

	// MP3
	else if (_codecId == "mp3") 
	{
		cout << "RTPSink: rtpSink CODEC_ID_MP3" << endl;
#ifdef RTSP_USING_MP3_ADU
		rtpSink = MP3ADURTPSink::createNew(envir(), rtpGroupsock, 96);
#else
		rtpSink = MPEG1or2AudioRTPSink::createNew(envir(), rtpGroupsock);
#endif
	}

	// AMR-NB
	else if (_codecId == "amr-nb") // Currently not implemented!
	{
		cout << "RTPSink: CODEC_ID_AMR_NB" << endl;	
		rtpSink = AMRAudioRTPSink::createNew(envir(), rtpGroupsock, 96);
	}

	// Unsupported
	else {
		rtpSink = NULL;
		cout << "Streaming Plugin: Error: Unsupported codec. " << endl;
	}
	
	return rtpSink;
}