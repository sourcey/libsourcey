#ifndef MP3_FROM_PCM_SOURCE_H
#define MP3_FROM_PCM_SOURCE_H


#include "Common.h"
#include <liveMedia.hh>


namespace Anionu {
namespace RTSP {


/**
 * A filter class that converts PCM packets into MP3 packets.
 * Uses internally ffmpeg for this conversion.
 */
class PCMToMP3Source : public FramedSource
{
public:
	//
	// Static method.
	// @param env UsageEnvironment, see LiveMedia doc. here
	//        http://www.live555.com/liveMedia/public/doxygen/html
	// @param audioSource A source of the kind of AudioInputDevice. 
	// @param bitRate The bitrate to encode: Defaults to 64000.
	// @param numChannels Number of channels to encode: 1- mono, 2- stereo. Defaults to 2.
	// @param sampleRate The samplerate to encode: Defaults to 44100.
	// @see LinuxAudioInputDevice, WindowsAudioInputDevice.
	//	
	static PCMToMP3Source* createNew(UsageEnvironment& env, FramedSource* audioSource, 
									int bitRate = 64000, 
									int numChannels = 2, 
									int sampleRate = 44100);

	virtual ~PCMToMP3Source ();
protected:
	//
	// Constructor.
	// @see createNew-
	//
	PCMToMP3Source(UsageEnvironment& env, FramedSource* audioSource, 
									int bitRate = 64000, 
									int numChannels = 2, 
									int sampleRate = 44100);

private: // Functions
	//
 	// Inherited function.\  Used to process next frame to send.
 	//
	virtual void doGetNextFrame();

	// Provides RTP encapsulation
	void deliverFrame();

	//
	// @return MIME type string. Something like "audio/MPEG"
	//
	char const* MIMEtype() const;

private: // Data
	// Specify whether the class is initialized or not
	bool _isInitialized;

	// Pointer to a AudioEncoder instance
	Anionu::AV::AudioEncoder* _audioEncoder;

	// 
	// "FramedSource" means something "discrete" being sent/parsed/used. In this
	// case its a PCM discrete data.
	//
	FramedSource* _audioSource;

	// Internal buffer for PCM data */
	unsigned char* _inputBuffer;
	unsigned int _inputBufferSize;

	// The bitrate to use in the encoding */
	int _bitRate;

	// The samplerate to use in the encoding */
	int _sampleRate;

	// Number of channels to use in the encoding */
	int _numChannels;

	// The size of each MP3 frame: FrameLengthInBytes = 144 * BitRate / SampleRate + Padding */
	unsigned int _mp3FrameSize;

	// The duration for each MP3 frame: FrameLengthInBytes * 8.0 / _bitRate */
	unsigned int _mp3FrameDuration;
};


} } // namespace Anionu::RTSP


#endif // MP3_FROM_PCM_SOURCE_H

