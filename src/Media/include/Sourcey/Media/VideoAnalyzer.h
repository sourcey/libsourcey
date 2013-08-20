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


#ifndef SOURCEY_MEDIA_VideoAnalyzer_H
#define SOURCEY_MEDIA_VideoAnalyzer_H


#include "Sourcey/Stateful.h"
#include "Sourcey/Signal.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Interface.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Media/FPSCounter.h"
#include "Sourcey/Media/Format.h"

extern "C" {
#include <libavcodec/avfft.h>
}


namespace scy {
namespace av {


class VideoAnalyzer //: public PacketProcessor
	/// This class provides basic AV spectrum analysis on a 
	/// video using the Fourier Transform algorithm.
	/// Data is outputted in CSV format.
	///
	/// TODO: 
	///		- Pluggable algorithms and processors
	///		- Normalization (scaling) for output values
	///		- Process multiple audio channels properly
	///		- Inherit from PacketProcessor
{
public:
	struct Options 
	{	
		std::string ifile;	// The input video file.
		int rdftSize;		// Size of the FFT input array
		//bool blocking;		// Blocking mode (disable async)

		Options() {
			rdftSize = 1024;
			//blocking = false;
		}
	};

	struct Stream 
	{
		std::string name;
		RDFTContext* rdft;
		FFTSample* rdftData;
		int rdftSize;
		int rdftBits;
		Int64 frames;
		int filled;
		
		Stream(const std::string& name, int rdftSize);
		~Stream();
		
		void initialize();
		void uninitialize();

		void fft();
			// Preforms FFT on internal rdftData
	};

	struct Packet 
	{
		double time;
		double value;
		//double min;
		//double max;
		Packet(double time = 0.0, double value = 0.0); //, double min = 99999.9, double max = -99999.9
	};

public:
	VideoAnalyzer(const VideoAnalyzer::Options& options = VideoAnalyzer::Options());
	virtual ~VideoAnalyzer();
				
	virtual void initialize();
		// Set everything up, and open the input file.

	virtual void uninitialize();
		// Stop processing and free everything.

	virtual void start();
		// Begin processing.

	virtual void stop();
		// Stop processing.
	
	Signal2<const VideoAnalyzer::Stream&, const VideoAnalyzer::Packet&> PacketOut;
		// Signals on VideoAnalyzer::Packet output
		// Raw FFT data is available via VideoAnalyzer::Stream->rdftData

	NullSignal Complete;
		// Signals on analysis complete
		
	virtual AVInputReader& reader();
	virtual Options& options();
	virtual std::string error() const;
	
protected:
	AVFrame* getGrayVideoFrame();
	
	virtual void onReadComplete(void* sender);
	virtual void onVideo(void* sender, VideoPacket& packet);
	virtual void onAudio(void* sender, AudioPacket& packet);

	const char* className() const { return "VideoAnalyzer"; }

protected:
	mutable Mutex _mutex;
	
	Options _options;
	std::string	_error;
	AVInputReader _reader;
	VideoAnalyzer::Stream* _video;
	VideoAnalyzer::Stream* _audio;	
	VideoConversionContext* _videoConv;
};


// ---------------------------------------------------------------------
// FFT Helpers
// Based on http://www.codeproject.com/Articles/6855/FFT-of-waveIn-audio-signals
//
inline double CalculateFrequencyIntensity(VideoAnalyzer::Stream& stream);
inline double CalculateCentroidFrequency(VideoAnalyzer::Stream& stream);

inline double GetFrequencyIntensity(double re, double im);
inline double GetDecibels(double re, double im);
inline double GetAmplitude(double re, double im, int len);
inline double GetAmplitudeScaled(double re, double im, int len, int scale);

#ifdef WIN32
inline double log2(double n);
#endif


} } // namespace scy::av


#endif




	//virtual void writeCSV(const VideoAnalyzer::Packet& packet);, double time
	//std::ofstream _ofile;