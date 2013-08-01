//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SOURCEY_MEDIA_AudioCapture_H
#define SOURCEY_MEDIA_AudioCapture_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/ICapture.h"
#include "Sourcey/Media/Format.h"
#include "Sourcey/Signal.h"

#include "RtAudio.h"

#include <iostream>
#include <queue>
#include <map>


namespace scy {
namespace av {


DefinePolymorphicDelegateWithArg(audioDelegate, IPacket, PacketDelegateBase, Void, nil)


class AudioCapture: public ICapture
{
public:	
	AudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format = RTAUDIO_SINT16);
	virtual ~AudioCapture();
	
  	virtual void open();
  	virtual void close();

  	virtual void start();
  	virtual void stop();

	int deviceId() const;
	int sampleRate() const;
	int numChannels() const;
	bool running() const;
	bool isOpen() const;
	RtAudioFormat format() const;

	void getEncoderFormat(Format& iformat);

protected:
	virtual void setError(const std::string& message);
		// Sets the error message and throws an exception.

	static int callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
					    double streamTime, RtAudioStreamStatus status, void *data);

private:
	mutable Mutex _mutex;
	int _deviceId; 
	int _channels;
	int _sampleRate;
	bool _opened;
	RtAudio _audio;
	RtAudioFormat _format;
	RtAudio::StreamParameters _iParams;
	std::string _error;
};


typedef std::map<int, AudioCapture*> AudioCaptureMap;


} } // namespace scy::av


#endif



	
/*
typedef signed short AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT16

typedef char AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT8

typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT24

typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT32

typedef float AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_FLOAT32

typedef double AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_FLOAT64
*/


/* //int channels, int sampleRate, RtAudioFormat format = RTAUDIO_SINT16)
	
	// virtual void onStreamStateChange(const PacketStreamState&) {};
	
	//virtual void attach(const PacketDelegateBase& delegate);
	//virtual bool detach(const PacketDelegateBase& delegate);
struct Samples 
{
	unsigned char* data; 
	int size;
	double time;
};


/// This is the base class for all instantiations of
/// the AudioCallback template.
class AbstractAudioCallback
{
public:
	AbstractAudioCallback() {};
	virtual ~AbstractAudioCallback() {};
	
	virtual void invoke(Samples& frame) const = 0;
	virtual AbstractAudioCallback* clone() const = 0;
	virtual void* object() const = 0;
};


typedef std::vector<AbstractAudioCallback*> CallbackList;


/// This template class implements an adapter that sits between
/// a AudioCapture and an object's method invoked by the capture.
template <class C> 
class AudioCallback: public AbstractAudioCallback
{
public:
	typedef void (C::*Callback)(Samples&);

	AudioCallback(C& object, Callback method) : 
		_object(&object), _method(method) {}
	AudioCallback(const AudioCallback& callback) : 
		_object(callback._object), _method(callback._method) {}

	void invoke(Samples& frame) const
	{
		(_object->*_method)(frame);
	}

	AbstractAudioCallback* clone() const
	{
		return new AudioCallback(*this);
	}

	void* object() const
	{
		return _object;
	}

private:
	AudioCallback() {};

	C*       _object;
	Callback _method;
};
	

template <class C>
static Callback<C, const AudioPacket, false> AudioCallback(C* object, void (C::*Method)(const AudioPacket&)) 
	/// Defines a callback for audio packets.
{
	return Callback<C, const AudioPacket, false>(object, Method);
};
*/
