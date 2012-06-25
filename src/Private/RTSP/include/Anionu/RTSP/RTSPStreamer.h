#ifndef ANIONU_RTSP_STREAMER_H
#define ANIONU_RTSP_STREAMER_H


#include "Common.h"
#include "OnDemandSMS.h"
#include <BasicUsageEnvironment.hh>
#include <RTSPServer.hh>

#include <map>


namespace Anionu {
namespace RTSP {


typedef std::map<std::string, ServerMediaSession*> SMSMap;


/**
 * Unicast Streamer class.
 * Will manage commands like start, stop,
 * and will raise one (single-stream) or more (multi-stream) RTSP servers.
 */
class RTSPStreamer
{
public: 
	//
	// Constructor.
	// @param params The RTSPServer streaming parameters.
	// @param videoParams Video encoding parameters. 
	// @param audioParams Audio encoding parameters. 
	//
	RTSPStreamer(StreamingParams params,
				VideoParams videoParams, 
				AudioParams audioParams);

	virtual ~RTSPStreamer();

	// 
	// Adds an authenticable user to the server
	// @param The user username. 
	// @param The user password. 
	//	
	//virtual void addUser(std::string username, std::string password);

	// 
	// Creates a standard RTSP server instance.
	//
	virtual void createRTSPInstance();

	//
	// Adds a Media Session to the setver
	// @param subsessionUrl The "testStream" part out of an url like "rtsp://12.23.45.56:554/testStream"
	// @param videoId Number of input camera mapped by system.
	// @param audioId Number of input microphone mapped by system.
	// @return The rtsp url playable by a rtsp-compatible program. 
	// Something like like "rtsp://12.23.45.56:554/testStream".
	//	
	virtual std::string addChannel(ChannelData& channel);
		
	//
	// Dynamically removes a Server Media Session.
	// @return true on success, false on failure.	 
	//
	virtual bool removeChannel(ChannelData& channel);
		
	//
	// Dynamically removes all Server Media Sessions
	// and loads new ones from the channel vector supplied.
	// @return true on success, false on failure.	 
	//
	virtual bool setChannels(ChannelsData& channels);

	// 
	// Returns if Server Media Session exists given it's subsession key.
	// @return true on success, false on failure.	 
	//	
	virtual bool channelExists(ChannelData& channel);

	//
	// Starts the RTPS Server loop.
	// @return true on success, false on failure.	 
	//	
	virtual bool start();

	// 
	// Stop streaming command. 
	// @return true on success, false on failure.	 
	//	
	virtual bool stop();

protected: // Functions
	static void dummyTask(void* clientData);

protected:  // Data
	// @see RTSPStreamer constructor
	char _watchVariable;
	
	// Specify weather we are using video or not
	bool _usingVideo;

	// Specify weather we are using audio or not
	bool _usingAudio;
	
	// STL container for ServerMediaSession pointers
	SMSMap _smsMap;

	// The streaming server parameters
	StreamingParams	_params;

	// The video codec to encode output
	VideoParams	_videoParams;

	// The audio codec to encode output
	AudioParams	_audioParams;
	
	// @see http://www.live555.com/liveMedia/public/doxygen/html
	RTSPServer *_rtspServer;
	TaskScheduler *_scheduler;
	UsageEnvironment *_env;
};


} } // namespace Anionu::RTSP


#endif	//RTSP_RTSP_H

