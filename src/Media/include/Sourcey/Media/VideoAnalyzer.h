//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_MEDIA_VideoAnalyzer_H
#define SOURCEY_MEDIA_VideoAnalyzer_H


#include "Sourcey/Stateful.h"
#include "Sourcey/Signal.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/IStartable.h"
#include "Sourcey/IPacketProcessor.h"
#include "Sourcey/Media/AVFileReader.h"
#include "Sourcey/Media/FPSCounter.h"


extern "C" {
#include <libavcodec/avfft.h>
}

//#include "Sourcey/Media/MatrixConverter.h"
//#include "Sourcey/Media/MotionDetector.h"
//#include "Sourcey/Media/FFT.h"

//#include <iostream>

//#include "Poco/Thread.h"
//#include "Poco/Stopwatch.h"
//#include "Poco/Timestamp.h"


namespace Sourcey {
namespace Media {


/*
struct VideoAnalyzerState: public State
{
	enum Type
	{
		None,
		Working,
		Success,
		Failed
	};
	
	std::string str(unsigned int id) const 
	{ 	
		switch (id) {
		case None:		return "None";
		case Working:	return "Working";
		case Success:	return "Success";
		case Failed:	return "Failed";
		}
		return "None";
	};
};
*/


class VideoAnalyzer//: public StatefulSignal<VideoAnalyzerState>
	/// This class provides basic AV spectrum analysis on a 
	/// video using the Fourier Transform algorithm.
	/// Data is outputted in CSV format.
	///
	/// TODO: 
	///		- Extend with more options and algorithms
	///		- Normalize audio and video level values
	///		- Differentiate audio channels
{
public:
	struct Options 
	{	
		std::string ifile;	// The input video file.
		std::string ofile;	// The output CSV file.

		int numFFTBits;		// Size of the FFT input array: 
							// (1 << nbits) for DCT-II, DCT-III and DST-I (1 << nbits) + 1 for DCT-I

		int maxFramerate;	// Maximum processing frame rate. 0 for no limit.

		Options() {
			numFFTBits = 8;
			maxFramerate = 0;
		}
	};

	struct Stream 
	{
		std::string name;
		RDFTContext* rdft;
		FFTSample* rdftData;
		double lastPTS;
		Int64 frames;
		int filled;
		
		Stream(const std::string& name);
		~Stream();
		
		void initialize(int rdftBits, int rdftSize);
		void uninitialize();
	};

public:
	VideoAnalyzer(const VideoAnalyzer::Options& options = VideoAnalyzer::Options());
	virtual ~VideoAnalyzer();
	
public:		
	virtual void start();
	virtual void stop();
		
	virtual Options& options();
	virtual std::string error() const;

	NullSignal Complete;
	
protected:
	virtual void processSpectrum(VideoAnalyzer::Stream& name, double time);
	virtual void writeLine(const std::string& name, double time, double value, double min = 0, double max = 0);
	
	virtual void onReadComplete(void* sender);
	virtual void onVideo(void* sender, VideoPacket& packet);
	virtual void onAudio(void* sender, AudioPacket& packet);

	const char* className() const { return "VideoAnalyzer"; }

protected:
	mutable Poco::FastMutex _mutex;
	
	Options _options;
	std::string	_error;
	std::ofstream _file;
	AVFileReader _reader;
	VideoAnalyzer::Stream* _video;
	VideoAnalyzer::Stream* _audio;	
	VideoConversionContext* _videoConv;
};


///
/// FFT Helpers
/// Based on http://www.codeproject.com/Articles/6855/FFT-of-waveIn-audio-signals
///

inline double GetFrequencyIntensity(double re, double im)
{
    return sqrt((re*re)+(im*im));
}

inline double GetDecibels(double re, double im)
{
	return (re == 0 && im == 0) ? 0 : 
		10.0 * log10(re*re+im*im);
}

inline double GetAmplitude(double re, double im, int len) 
{
	return GetFrequencyIntensity(re,im)/(len);
}

inline double GetAmplitudeScaled(double re, double im, int len, int scale) 
{
	return static_cast<int>(GetAmplitude(re,im,len)) % scale;
}


} } // namespace Sourcey::Media


#endif