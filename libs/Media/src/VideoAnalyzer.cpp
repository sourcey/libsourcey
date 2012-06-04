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
	_video(NULL),
	_audio(NULL)
{
	Log("trace") << "[VideoAnalyzer:" << this <<"] Creating" << endl;
}


VideoAnalyzer::~VideoAnalyzer() 
{
	Log("trace") << "[VideoAnalyzer:" << this <<"] Destroying" << endl;

	if (_video)
		delete _video;
	if (_audio)
		delete _audio;
}

void VideoAnalyzer::start()
{
	FastMutex::ScopedLock lock(_mutex); 

	if (_options.ifile.empty())
		throw Poco::FileException("Please specify an input file.");

	if (_options.ofile.empty())
		throw Poco::FileException("Please specify an output file.");

	Log("trace") << "[VideoAnalyzer:" << this <<"] Starting: " << _options.ifile << endl;

	_error = "";
	
	try 
	{
		// Open the output file.
		_file.open(_options.ofile.data(), ios::out | ios::binary);
		if (!_file.is_open())		
			throw Poco::OpenFileException("Cannot open output file: " + _options.ofile);

		// Open the input file decoder.
		_reader.open(_options.ifile);
		
		if (_reader.video()) {
			_video = new VideoAnalyzer::Stream("Video");
			_video->initialize(_options.numFFTBits, sizeof(*_video->rdftData) *
				(_reader.video()->codec->width * 
				 _reader.video()->codec->height));
			_videoConvCtx = NULL;
			_videoGrayFrame = NULL;
		}

		if (_reader.audio()) {
			_audio = new VideoAnalyzer::Stream("Audio");
			_audio->initialize(_options.numFFTBits, sizeof(*_audio->rdftData) *
				(_reader.audio()->stream->codec->frame_size * 2 * 
				 _reader.audio()->stream->codec->channels));
		}
	
		_reader += packetDelegate(this, &VideoAnalyzer::onVideo);
		_reader += packetDelegate(this, &VideoAnalyzer::onAudio);

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


void VideoAnalyzer::processSpectrum(VideoAnalyzer::Stream& stream, double time)
{
    av_rdft_calc(stream.rdft, stream.rdftData);

	/*
//#define NBITS   8
//#define N       (1<<(NBITS)) // 256
    volatile FFTSample av_re = stream.rdftData[0];
    volatile FFTSample av_im = 0;
    for (int i = 1; i < N/2; i += 2) {
        av_re = stream.rdftData[i];
        av_im = stream.rdftData[i+1];
		printf("re=%10f im=%10f im=%10f\n", prefix##_re, prefix##_im)
    }
    av_re = stream.rdftData[1];
    av_im = 0;
	return;
	*/

	double val = 0.0;
	double sum = 0.0;
	double min = 99999.9;
	double max = -99999.9;
	double avg = 0.0;

	//for (int i = 0; i < _options.fftPoints / 2; ++i) {
	//for (int i = 1; i < N/2; i += 2) {
	for (int i = 0; i < stream.filled / 2; i += 2) {
	    val = GetFrequencyIntensity(stream.rdftData[i], stream.rdftData[i+1]);
		if (val > max)
			max = val;
		if (val < min)
			min = val;
		//printf("re=%10f im=%10f im=%10f\n", stream.rdftData[i], stream.rdftData[i+1]);
		//Log("trace") << "[VideoAnalyzer:" << this << "] Value: " 
		//	<< stream.rdftData[i] << ": " << stream.rdftData[i+1] << endl;
		sum += val; 
	}

	// TODO: Better way of gathering avg?
	// Not all FFT values are useful.
	avg = sum / (stream.filled / 4);

	writeLine(stream.name, time, avg, min, max);
}


void VideoAnalyzer::writeLine(const string& channel, double time, double value, double min, double max) //, double avg
{
	Log("trace") << "[VideoAnalyzer:" << this << "] Writing:"
		<< channel << "," << time << "," << value << "," 
		<< min << "," << max << endl;

	_file << channel << "," << time << "," << 
		value << "," << min << "," << max << "\r\n";
}


void VideoAnalyzer::onVideo(void* sender, VideoPacket& packet)
{
	Log("trace") << "[VideoAnalyzer:" << this << "] On Video: " 
		<< packet.size() << ": " << packet.time << endl;
	
	FastMutex::ScopedLock lock(_mutex); 

	VideoDecoderContext* video = _reader.video();
	
	// Skip frames if we exceed the maximum processing framerate.
	double fps = _video->frames / packet.time;
	if (_options.maxFramerate > 0 && fps > _options.maxFramerate) {
		//Log("trace") << "[VideoAnalyzer:" << this << "] Skipping frame at fps: " << fps << endl;
		return;
	}
	_video->frames++;	
		
	// Create and allocate the conversion frame.
	if (_videoGrayFrame == NULL) {
		_videoGrayFrame = avcodec_alloc_frame();	
		if (_videoGrayFrame == NULL)
			throw Exception("Video Analyzer: Unable to allocate the output video frame.");

		avpicture_alloc(reinterpret_cast<AVPicture*>(_videoGrayFrame), 
			PIX_FMT_GRAY8, video->codec->width, video->codec->height);
	}
	
	// Convert the image from its native format to GREY8.
	if (_videoConvCtx == NULL) {
		_videoConvCtx = sws_getContext(
			video->codec->width, video->codec->height, video->codec->pix_fmt, 
			video->codec->width, video->codec->height, PIX_FMT_GRAY8, 
			SWS_BICUBIC, NULL, NULL, NULL);
	}
	if (_videoConvCtx == NULL)
		throw Exception("Video Analyzer: Unable to initialize the video conversion context.");	
		
	// Scales the source data according to our SwsContext settings.
	if (sws_scale(_videoConvCtx,
		video->frame->data, video->frame->linesize, 0, video->codec->height,
		_videoGrayFrame->data, _videoGrayFrame->linesize) < 0)
		throw Exception("Video Analyzer: Pixel format conversion not supported.");

	// Populate the FFT input data.
	// Examples:
	//	http://stackoverflow.com/questions/7790877/forward-fft-an-image-and-backward-fft-an-image-to-get-the-same-result
	//	http://code.google.com/p/video-processing-application/source/browse/trunk/+video-processing-application/untitled6/Fourier/highpassrgb.cpp
	//	http://codepaste.ru/9226/
	_video->filled = 0;
	for (int y = 0; y < video->codec->height; y++) {
		for (int x = 0; x < video->codec->width; x++) {
			_video->filled = y * video->codec->width + x; // * 3;
			_video->rdftData[_video->filled] = (float)_videoGrayFrame->data[0][_video->filled] * pow(-1.0, y + x);
		}
	}

	processSpectrum(*_video, packet.time);
}


void VideoAnalyzer::onAudio(void* sender, AudioPacket& packet)
{
	Log("trace") << "[VideoAnalyzer:" << this << "] On Audio: " << packet.size() << ": " << packet.time << endl;	

	FastMutex::ScopedLock lock(_mutex); 
		
	// Skip frames if we exceed the maximum processing framerate.
	double fps = _audio->frames / packet.time;
	if (_options.maxFramerate > 0 && fps > _options.maxFramerate) {
		//Log("trace") << "[VideoAnalyzer:" << this << "] Skipping frame at fps: " << fps << endl;
		return;
	}
	_audio->frames++;
	
	short const* data = reinterpret_cast<short*>(packet.data());	
	int size = FFMIN(_reader.audio()->frameSize, packet.size());
	int channels = _reader.audio()->stream->codec->channels;
	int filled = 0;
	
	// Packet size / 2 = 2 bytes per sample (short)
	// Example at http://blackhole.ubitux.fr/bench-fftw-ffmpeg-fft/fft.c
	_audio->filled = 0;
    for (int i = 0; i < size / 2; i += channels) {
        int k, v = 0;
        for (k = 0; k < channels; k++) // mix channels
            v += data[i + k];

        _audio->rdftData[_audio->filled++] = (float)v / channels / SHRT_MAX;
        //if (filled == _options.fftPoints) {
        //    processSpectrum(_audio, packet.time);
        //    filled = 0;
        //}
    }

	processSpectrum(*_audio, packet.time);
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

	Complete.dispatch(this);
}


VideoAnalyzer::Options& VideoAnalyzer::options()
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _options; 
}


string VideoAnalyzer::error() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _error;
}


// ---------------------------------------------------------------------
//
VideoAnalyzer::Stream::Stream(const std::string& name) : //, int rdftBits, int rdftSize
	name(name), rdft(NULL), rdftData(NULL), lastPTS(0), frames(0), filled(0)
{	
}
	

VideoAnalyzer::Stream::~Stream()
{
	uninitialize();
}

	
void VideoAnalyzer::Stream::initialize(int rdftBits, int rdftSize)
{
	lastPTS		= 0;
	frames		= 0;
	filled		= 0;
	rdft		= av_rdft_init(rdftBits, DFT_R2C);
	rdftData	= (FFTSample*)av_malloc(rdftSize);
}

	
void VideoAnalyzer::Stream::uninitialize()
{
	if (rdft)
		av_rdft_end(rdft);
	if (rdftData)
		av_free(rdftData);
}


} } // namespace Sourcey::Media