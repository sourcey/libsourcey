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


#include "Sourcey/Media/VideoAnalyzer.h"


using namespace cv;
using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


VideoAnalyzer::VideoAnalyzer(const Options& options) : 
	_options(options),
	_fft(options.fftPoints),
	_fftData(options.fftPoints)
{
	Log("trace") << "[VideoAnalyzer:" << this <<"] Creating" << endl;
}


VideoAnalyzer::~VideoAnalyzer() 
{
	Log("trace") << "[VideoAnalyzer:" << this <<"] Destroying" << endl;
}


void VideoAnalyzer::start(const string& ifile, const string& ofile) 
{
	Log("trace") << "[VideoAnalyzer:" << this <<"] Starting: " << ifile << endl;

	FastMutex::ScopedLock lock(_mutex); 

	_error = "";
	_ifile = ifile;
	_ofile = ofile;
	
	try 
	{
		// Open the input file decoder.
		_reader.open(ifile);
		
		// Open the output file.
		_file.open(ofile.data(), ios::out | ios::binary);
		if (!_file.is_open())		
			throw Poco::OpenFileException("Unable to open output file: " + ofile);
	
		// Receive decoded video from the file decoder and
		// run it through the motion detector. The motion 
		// level will represent the output waveform level.
		_reader += packetDelegate(this, &VideoAnalyzer::onVideo);
				
		// Receive decoded audio from the file decoder and
		// run it through the FFT algorithm to gather
		// visualization data.
		_reader += packetDelegate(this, &VideoAnalyzer::onAudio);

		// Kick off the decoder...
		_reader.ReadComplete += delegate(this, &VideoAnalyzer::onReadComplete);
		_reader.start();
	} 
	catch (Exception& exc) 
	{
		_error = exc.displayText();
		Log("error") << "[VideoAnalyzer:" << this << "] Error: " << _error << endl;
		exc.rethrow();
	}
}


void VideoAnalyzer::stop() 
{
	Log("trace") << "[VideoAnalyzer:" << this <<"] Destroying" << endl;
	
	// Can't lock here in case we inside a callback.
	//FastMutex::ScopedLock lock(_mutex); 
	
	_reader.ReadComplete -= delegate(this, &VideoAnalyzer::onReadComplete);
	_reader -= packetDelegate(this, &VideoAnalyzer::onVideo);
	_reader -= packetDelegate(this, &VideoAnalyzer::onAudio);
	_reader.stop();
	_file.close();
}


void VideoAnalyzer::writeLine(const string& channel, double timestamp, double value, double min, double max, double avg)
{
	Log("trace") << "[VideoAnalyzer:" << this << "] Writing:"
		<< channel << "," << timestamp << "," << value << "," 
		<< min << "," << max << "," << avg << endl;

	_file << channel << "," << timestamp << "," << 
		value << "," << min << "," << max << "," << avg << "\r\n";
}


void VideoAnalyzer::onReadComplete(void* sender)
{
	Log("trace") << "[VideoAnalyzer:" << this << "] On Read Complete" << endl;	

	AVFileReader* reader = reinterpret_cast<AVFileReader*>(sender);	
	{
		FastMutex::ScopedLock lock(_mutex); 
		if (_error.empty())
			_error = reader->error();
	}

	Complete.emit(this);
}


void VideoAnalyzer::onVideo(void* sender, VideoPacket& packet)
{
	Log("trace") << "[VideoAnalyzer:" << this << "] On Video: " << packet.size() << ": " << packet.time << endl;
	
	FastMutex::ScopedLock lock(_mutex); 
	
	// Convert decoded video packets to OpenCV matrix
	// images for feeding into the motion detector.
	_matrixConverter.process(packet);

	// Process the video motion level and use the
	// output as out video level.
	_motionDetector.process(packet);

	// TODO: Compute min, max and average levels for video.
	writeLine("video", packet.time, _motionDetector.motionLevel());	
				
	//cv::imshow("Analyzer", *packet.mat);
	//cv::waitKey(10);
}


void VideoAnalyzer::onAudio(void* sender, AudioPacket& packet)
{
	Log("trace") << "[VideoAnalyzer:" << this << "] On Audio: " << packet.size() << ": " << packet.time << endl;
	
	FastMutex::ScopedLock lock(_mutex); 

	short const* data16 = reinterpret_cast<short*>(packet.data());

	// Fill the FFT buffer
	for (int i = 0; i < _options.fftPoints; ++i) {
		//int audioLeft = (int)data16[i * 2];
		//int audioRight = (int)data16[i * 2 + 1];
		//(audioLeft + audioRight) * (1.0f / 65536.0f); //packet.data[i]; //buf[i];
		_fftData[i] = packet.data()[i];
	}
		
	// Run fourier transform
	vector<FFT::Complex> frequencies = _fft.transform(_fftData);

	// Compute frequency magnitude
	double level = 0.0;
	double sum = 0.0;
	double min = 99999.9;
	double max = -99999.9;
	double avg = 0.0;
	for (int i = 0; i < _options.fftPoints / 2; ++i) {
	    level = _fft.getIntensity(frequencies[i]); //getDecibels(frequencies[i]);
		if (level > max)
			max = level;
		if (level < min)
			min = level;
		sum += level; 
	}

	// TODO: Better way of gathering avg?
	// Not all FFT values are useful.
	avg = sum / frequencies.size(); 

	writeLine("audio", packet.time, sum, min, max, avg);	
}


VideoAnalyzer::Options& VideoAnalyzer::options()
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _options; 
}


string VideoAnalyzer::ifile() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _ifile;
}


string VideoAnalyzer::ofile() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _ofile;
}


string VideoAnalyzer::error() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _error;
}


} } // namespace Sourcey::Media
