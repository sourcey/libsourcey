#ifndef ANIONU_SPOT_IChannel_H
#define ANIONU_SPOT_IChannel_H


#include "Sourcey/Base.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Spot/IModeManager.h"
#include "Sourcey/Spot/IMediaManager.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/DeviceManager.h"

#include <iostream>


namespace Sourcey {
namespace Spot {


class IChannel: public ILoggable
{
public:
	IChannel(/*IEnvironment& env, */const std::string& name, const Media::Device& video, const Media::Device& audio);
	IChannel(/*IEnvironment& env, */const std::string& name);
	virtual ~IChannel();

	virtual bool valid() const;	
	
	//virtual int id() const;
	virtual std::string name() const;
	virtual std::string videoInputFile() const;
	virtual Media::VideoCapture* videoCapture(bool whiny = false) const;
	virtual Media::AudioCapture* audioCapture(bool whiny = false) const;
	virtual Media::Device videoDevice() const;
	virtual Media::Device audioDevice() const;
	
	virtual IModeManager& modes() = 0;	
	
	Signal<std::string&> ChannelChanged;
		// Signals change to the internal channel data, namely
		// devices and name. In the case of a name change the
		// signal will broadcast the old name, and the new name
		// may be accessed via the name() method.

	virtual void print(std::ostream& ost) const;

	virtual const char* className() const { return "Channel"; }

protected:
	//int			_id;
	std::string	_name;
	std::string	_videoInputFile;
	Media::Device _videoDevice;
	Media::Device _audioDevice;
	mutable Poco::FastMutex _mutex;
};


/*
	Signal<std::string&> IChannelClosing;
class IChannel: public IChannel
{
public:
	IChannel(const std::string& name, const Media::Device& video, const Media::Device& audio);
	IChannel(const std::string& name);
	virtual ~IChannel();

	virtual bool valid() const;
	virtual std::string toString() const;

	virtual std::string name() const   { return _name; }
	virtual Media::Device videoDevice() const { return _videoDevice; }
	virtual Media::Device audioDevice() const { return _audioDevice; }
	virtual Media::VideoCapture* videoCapture() const;
	virtual Media::AudioCapture* audioCapture() const;
	virtual IModeManager* modes() const { return _modes; }

	virtual Media::VideoPacket* getSnapshot(int width = 0, 
											int height = 0, 
											bool base64 = false) const;
		// Returns a current JPEG image from the channel video
		// device with optional Base64 protocol. For preformance 
		// reasons this method is only meant for constant use.
	
	virtual void setName(const std::string& name);
	virtual void setVideoDevice(const Media::Device& dev);
	virtual void setAudioDevice(const Media::Device& dev);
	virtual void setIModeManager(IModeManager* modes);

	Signal<std::string&> ChannelChanged;
	Signal<std::string&> IChannelClosing;

protected:
	std::string		_name;
	Device			_audioDevice;
	Device			_videoDevice;
	IModeManager*	_modes;
};
*/


typedef std::vector<IChannel*> IChannelList;
typedef std::map<std::string, IChannel*> IChannelMap;


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IChannel_H