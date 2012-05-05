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
#include "Sourcey/Media/MatrixConverter.h"
#include "Sourcey/Media/MotionDetector.h"
#include "Sourcey/Media/FFT.h"

//#include <iostream>

//#include "Poco/Thread.h"
//#include "Poco/Stopwatch.h"
//#include "Poco/Timestamp.h"


namespace Sourcey {
namespace Media {


/*
struct VideoAnalyzerState: public StateT
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
	struct Options 
	{	
		int fftPoints;

		Options() {		
			fftPoints = 1024; //2048; //4096;
		}
	};

public:
	VideoAnalyzer(const VideoAnalyzer::Options& options = VideoAnalyzer::Options());
	virtual ~VideoAnalyzer();
	
public:		
	virtual void start(const std::string& ifile, const std::string& ofile);
	virtual void stop();
		
	virtual Options& options();
	virtual std::string ifile() const;
	virtual std::string ofile() const;
	virtual std::string error() const;

	NullSignal Complete;
	
protected:
	virtual void writeLine(const std::string& channel, double timestamp, double value, double min = 0, double max = 0, double avg = 0);
		
	virtual void onReadComplete(void* sender);
	virtual void onVideo(void* sender, VideoPacket& packet);
	virtual void onAudio(void* sender, AudioPacket& packet);

	const char* className() const { return "VideoAnalyzer"; }

protected:
	mutable Poco::FastMutex _mutex;
	
	Options _options;
	std::string	_error;
	std::string	_ifile;
	std::string	_ofile;
	std::ofstream _file;
	AVFileReader _reader;
	MotionDetector _motionDetector;
	MatrixConverter _matrixConverter;
	FFT _fft;
	std::vector<FFT::Complex> _fftData;
};


} } // namespace Sourcey::Media


#endif