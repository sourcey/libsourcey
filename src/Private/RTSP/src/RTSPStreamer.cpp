#include "Anionu/RTSP/RTSPStreamer.h"


using namespace std;
using namespace Poco;
using namespace Anionu;
using namespace Anionu::RTSP;
using namespace Anionu::Input;


// ============================================================================
//
// RTSP Streamer
//
// ============================================================================
RTSPStreamer::RTSPStreamer(
	StreamingParams params,
	VideoParams videoParams,		
	AudioParams audioParams) :
		_params(params),
		_videoParams(videoParams),
		_audioParams(audioParams),
		_rtspServer(NULL),
		_watchVariable(0)
{
	cout << "RTSP Streamer Intializing:\n"
		<< "\n"
		<< "Streaming Information:\n"
		<< "Using Port Number: " << _params.activePortNum << "\n"
		<< "Port Number: " << _params.primaryPortNum << "\n"
		<< "Fallback Port Number: " << _params.fallbackPortNum << "\n"
		<< "IP Address: " << _params.remoteIp << "\n"
		<< "\n"
		<< "Video Information:\n"
		<< "Codec: " << _videoParams.codec << "\n"
		<< "FPS: " << _videoParams.fps << "\n"
		<< "Height: " << _videoParams.height << "\n"
		<< "Width: " << _videoParams.width << "\n"
		<< "Pixel Format: " << _videoParams.pixelFormat << "\n"
		<< "\n"
		<< "Audio Information:\n"
		<< "Codec: " << _audioParams.codec << "\n"
		<< "Channels: " << _audioParams.channels << "\n"
		<< "Bit Rate: " << _audioParams.bitRate << "\n"
		<< "Sample Rate: " << _audioParams.sampleRate << "\n"
		<< endl;

	_usingVideo = !_videoParams.codec.empty() ? true : false;
	_usingAudio = !_audioParams.codec.empty() ? true : false;

	_scheduler = BasicTaskScheduler::createNew();
	_env = BasicUsageEnvironment::createNew(*_scheduler);
}


RTSPStreamer::~RTSPStreamer() 
{
	cout << "RTSPStreamer::~RTSPStreamer()" << endl;

	// To destroy the RTSPServer properly we must destroy it's
	// objects in the reverse order from which we created them.
	if (_rtspServer)	Medium::close(_rtspServer);
    if (_scheduler)		delete _scheduler;
	if (_env)			_env->reclaim();
}


void RTSPStreamer::createRTSPInstance() 
{	
	_rtspServer = RTSPServer::createNew(*_env, _params.activePortNum, 0);
	if (_rtspServer == NULL)
		throw Poco::Exception("Failed to create RTSP server: " + string(_env->getResultMsg()));
}


string RTSPStreamer::addChannel(ChannelData& channel) 
{	
	cout << "RTSP Streamer Adding Channel:\n"
		<< "\n"
		<< "ID: " << channel.id << "\n"
		<< "Name: " << channel.name << "\n"
		<< "Audio Id: " << channel.audioId << "\n"
		<< "Video Id: " << channel.videoId << "\n"
		<< endl;

	assert(_rtspServer);
	assert(channel.id);
	assert(!channel.name.empty());

	// Throw if the media session already exists
	if (channelExists(channel))
		throw Poco::Exception(channel.name + " is already streaming.");

	// Create a new Server Media Session	
	ServerMediaSession *serverMediaSession = ServerMediaSession::createNew(*_env, channel.name.c_str(), 0, "Session streamed by Spot",  true);
	OnDemandSMS *onDemandSMS;
	
	// Video stream
	if (_usingVideo && channel.videoId >= 0) 
	{			
		VideoStream *videoStream = new VideoStream(channel.videoId);
		onDemandSMS = OnDemandSMS::createNew(*_env, false, _videoParams.fps, videoStream, _videoParams.codec.c_str(), -1, 0);
		serverMediaSession->addSubsession(onDemandSMS);
	}

	// Audio stream
	if (_usingAudio && channel.audioId >= 0) 
	{
		// If we're reading from a single input source "reuseFirstSource" should be "True".
		// This tells the server to use the same input source object, even if more than one client 
		// is streaming from the server concurrently. 
		onDemandSMS = OnDemandSMS::createNew(*_env, true, NULL, NULL, _audioParams.codec.c_str(), channel.audioId, 1);
		serverMediaSession->addSubsession(onDemandSMS);
	}

	// Store the session in our session map
	_smsMap[channel.name] = serverMediaSession;

	// Finally add the session to our server
	_rtspServer->addServerMediaSession(serverMediaSession);

	/*
	// Live555 will return our local IP url which is not much 
	// good to us if we will be connecting from remote devices!
	// Let's replace that with our remote/public IP...
	std::string url = _rtspServer->rtspURL(serverMediaSession);
	std::string ip;
	string::size_type start = 0;
	string::size_type end = 0;
	start = url.find("//");
	if (start != string::npos) {
		start += 2;
		end = url.find("/", start);
		ip = url.substr(start, end-start);
	}
	url.replace(url.find(ip), ip.size(), _params.remoteIp);

	// Add metadata containing the RTSP URL to our Streaming Mode
	if (!url.empty()) {
		_api->AddModeMetadata.notify(this, 
			ModeMetadata(
				channel.id,
				"streaming", 
				"URL", 
				url
			)
		); 
	}	


	cout << "Connect here for stream(s): " << url << endl;
	return url;
	*/
	cout << "Connect here for stream(s): " << _rtspServer->rtspURL(serverMediaSession) << endl;
	return _rtspServer->rtspURL(serverMediaSession);
}


bool RTSPStreamer::removeChannel(ChannelData& channel)
{
	cout << "RTSPStreamer::removeChannel" << endl;
	assert(_rtspServer);
	assert(channel.id);
	assert(!channel.name.empty());

	SMSMap::iterator it = _smsMap.find(channel.name);
	if (it != _smsMap.end()) {
		cout << "RTSPStreamer::removeChannel: Found channel.. destroying..." << endl;
		_rtspServer->removeServerMediaSession((*it).second->streamName());
		_smsMap.erase(it);
		return true;
	}
	/*
	// Remove the metadata containing the rtsp url from our Streaming Mode
	_api->RemoveModeMetadata.notify(this, 
		ModeMetadata(
			channel.id,
			"streaming", 
			"URL"
		)
	);
	*/

	return false;
}


bool RTSPStreamer::setChannels(vector<ChannelData>& channels) 
{
	cout << "RTSPStreamer::setChannels" << endl;
	assert(_rtspServer);

	// Kill all current channels
	for (SMSMap::iterator it=_smsMap.begin(); it!=_smsMap.end(); ++it) {	
		cout << "RTSPStreamer::removeChannel: Found channel.. destroying..." << endl;
		_rtspServer->removeServerMediaSession((*it).second->streamName());
		//_smsMap.erase(it);
	}
	_smsMap.clear();

	// Add the new ones
	for (vector<ChannelData>::iterator it=channels.begin(); it!=channels.end(); ++it) {	
		addChannel((*it));
	}

	return true;
}


bool RTSPStreamer::channelExists(ChannelData& channel) 
{
	cout << "RTSPStreamer::channelExists" << endl;
	assert(_rtspServer);
	assert(channel.id);
	assert(!channel.name.empty());

	SMSMap::iterator it = _smsMap.find(channel.name);
	if (it != _smsMap.end())
		return true;
	return false;
}


void RTSPStreamer::dummyTask(void* clientData) 
{
	UsageEnvironment* env = (UsageEnvironment*)clientData;

	// Call this again, after a brief delay:
	int uSecsToDelay = 100000; // 100 ms
	env->taskScheduler().scheduleDelayedTask(uSecsToDelay,
                                            (TaskFunc*)dummyTask, env);
}


bool RTSPStreamer::start()  
{
	// Run a dummy task which will call the scheduler every 100ms. This will 
	// ensure that as soon as our watchVariable is changed the EventLoop will
	// terminate.
	dummyTask(_env);

	// Returns when the watch variable is != 0 as from 
	// http://www.live.com/liveMedia/doxygen/html/UsageEnvironment_8hh-source.html
	// Note: The watch variable ill not be triggered unless there is an active client 
	// connection so we may need to destroy this manually.
	_env->taskScheduler().doEventLoop(&_watchVariable); 

	// We reach this point after stop(), 
	// which sets the watchVariable to 1: the doEventLoop returns.
	return true;
}


bool RTSPStreamer::stop() 
{
	cout << "RTSPStreamer::stop()" << endl;

	// Set the watchVariable to 1 exiting the event loop.
	_watchVariable = ~0;

	return true;
}