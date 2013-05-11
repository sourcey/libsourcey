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


using namespace std;
using namespace Poco;


namespace Scy {
namespace Media {


VideoAnalyzer::VideoAnalyzer(const Options& options) : 
	_options(options),
	_video(NULL),
	_audio(NULL),
	_videoConv(NULL)
{
	LogTrace() << "[VideoAnalyzer:" << this <<"] Creating" << endl;
}


VideoAnalyzer::~VideoAnalyzer() 
{
	LogTrace() << "[VideoAnalyzer:" << this <<"] Destroying" << endl;
	uninitialize();
}


void VideoAnalyzer::initialize()
{
	if (_options.ifile.empty())
		throw Poco::FileException("Please specify an input file.");

	LogTrace("VideoAnalyzer", this) << "Loading: " << _options.ifile << endl;

	_error = "";

	// Open the input file decoder.
	_reader.openFile(_options.ifile);
		
	if (_reader.video()) {
		_videoConv = NULL;
		_video = new VideoAnalyzer::Stream("Video", _options.rdftSize);
		_video->initialize();	
	}

	if (_reader.audio()) {
		_audio = new VideoAnalyzer::Stream("Audio", _options.rdftSize);
		_audio->initialize();
	}
	
	_reader += packetDelegate(this, &VideoAnalyzer::onVideo);
	_reader += packetDelegate(this, &VideoAnalyzer::onAudio);

	_reader.ReadComplete += delegate(this, &VideoAnalyzer::onReadComplete);
}

	
void VideoAnalyzer::uninitialize()
{
	//LogTrace() << "[VideoAnalyzerStream:" << this << ":" << name << "] Uninitializing" << endl;	
	stop();
	
	//FastMutex::ScopedLock lock(_mutex); 

	if (_video)
		delete _video;
	if (_videoConv)
		delete _videoConv;
	if (_audio)
		delete _audio;
}


void VideoAnalyzer::start()
{
	FastMutex::ScopedLock lock(_mutex); 
	
	try 
	{
		if (!_reader.audio() && !_reader.video())
			throw Exception("Video reader not initialized");

		//if (_options.blocking)
		//	_reader.run();
		//else
		_reader.start();
	} 
	catch (Exception& exc) 
	{
		_error = exc.displayText();
		LogError("VideoAnalyzer", this) << "Error: " << _error << endl;
		exc.rethrow();
	}
}


void VideoAnalyzer::stop() 
{
	
	// Can't lock here in case we inside a callback.
	//FastMutex::ScopedLock lock(_mutex); 
	
	_reader.ReadComplete -= delegate(this, &VideoAnalyzer::onReadComplete);
	_reader.detach(this);
	_reader.stop();
}


void VideoAnalyzer::onVideo(void* sender, VideoPacket& packet)
{
	//LogTrace("VideoAnalyzer", this) << "On Video: " 
	//	<< packet.size() << ": " << packet.time << endl;
	
	VideoAnalyzer::Packet pkt(packet.time);
	AVFrame* greyFrame = getGrayVideoFrame();
    int width  = greyFrame->width;
    int height = greyFrame->height;
	int step   = greyFrame->linesize[0];
	int frames = 0;
	//VideoDecoderContext* video = _reader.video();
	
	FastMutex::ScopedLock lock(_mutex); 
	
	// Prepeare FFT input data array
	//	http://stackoverflow.com/questions/7790877/forward-fft-an-image-and-backward-fft-an-image-to-get-the-same-result
	//	https://code.google.com/p/video-processing-application/source/browse/trunk/+video-processing-application/untitled6/Fourier/highpassrgb.cpp?r=2
	//	http://codepaste.ru/9226/
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			_video->rdftData[_video->filled++] = (float)greyFrame->data[0][y * step + x] * pow(-1.0, y + x);
			if (_video->filled == _video->rdftSize) {
				_video->fft();
				pkt.value += CalculateFrequencyIntensity(*_video);
				frames++;
				_video->frames++;
				_video->filled = 0;
			}
		}
	}

	if (frames) {

		// Calculate and scale the average value for this video frame
		pkt.value /= frames;
		pkt.value = sqrt(pkt.value); ///= _video->rdftSize;

		LogTrace("VideoAnalyzer", this) << "Video Output: "
			<< pkt.time << ", " << pkt.value << endl;
		PacketOut.emit(this, *_video, pkt);
	}
}

void VideoAnalyzer::onAudio(void* sender, AudioPacket& packet)
{
	//LogTrace("VideoAnalyzer", this) << "On Audio: " 
	//  << packet.size() << ": " << packet.time << endl;	

	FastMutex::ScopedLock lock(_mutex);		
	
	VideoAnalyzer::Packet pkt(packet.time);
	short const* data = reinterpret_cast<short*>(packet.data());
	int channels = _reader.audio()->stream->codec->channels;
	int size = packet.size();
	int frames = 0;
	
	// Packet size / 2 = 2 bytes per sample (short)
	// Example at http://blackhole.ubitux.fr/bench-fftw-ffmpeg-fft/fft.c
    for (int i = 0; i < size / 2; i += channels) {
        int k, v = 0;
        for (k = 0; k < channels; k++) // mix channels
            v += data[i + k];

        _audio->rdftData[_audio->filled++] = (float)v / channels / SHRT_MAX;
		if (_audio->filled == _audio->rdftSize) {
			_audio->fft();
			pkt.value += CalculateFrequencyIntensity(*_audio);
			frames++;
			_audio->frames++;
			_audio->filled = 0;
		}
    }

	if (frames) {

		// Calculate the average value for this video frame
		pkt.value /= frames;

		LogTrace("VideoAnalyzer", this) << "Audio Output: "
			<< pkt.time << ", " << pkt.value << endl;
		PacketOut.emit(this, *_audio, pkt);
	}
}


AVFrame* VideoAnalyzer::getGrayVideoFrame()
{		
	FastMutex::ScopedLock lock(_mutex); 
	VideoDecoderContext* video = _reader.video();

	// TODO: Conversion via decoder?
	if (_videoConv == NULL) {		
		VideoCodec iparams;
		iparams.width = video->ctx->width;
		iparams.height = video->ctx->height;
		iparams.pixelFmt = av_get_pix_fmt_name(video->ctx->pix_fmt);
		VideoCodec oparams;
		oparams.width = video->ctx->width;
		oparams.height = video->ctx->height;
		oparams.pixelFmt = "gray";		
		_videoConv = new VideoConversionContext();
		_videoConv->create(iparams, oparams);
	}
	if (_videoConv == NULL)
		throw Exception("Video Analyzer: Unable to initialize the video conversion context.");	
		
	// Convert the source image to grayscale
	return _videoConv->convert(video->frame);
}


void VideoAnalyzer::onReadComplete(void* sender)
{
	LogTrace("VideoAnalyzer", this) << "On Read Complete" << endl;	

	AVInputReader* reader = reinterpret_cast<AVInputReader*>(sender);	
	{
		FastMutex::ScopedLock lock(_mutex); 
		if (_error.empty())
			_error = reader->error();
	}

	Complete.emit(this);
}


AVInputReader& VideoAnalyzer::reader()
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _reader; 
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
VideoAnalyzer::Packet::Packet(double time, double value) : //, double min, double max
			time(time), value(value)
{	
	//this->min = min;
	//this->max = max;
}
	

// ---------------------------------------------------------------------
//
const int kMaxFFTPow2Size = 24;


VideoAnalyzer::Stream::Stream(const std::string& name, int rdftSize) : 
	name(name), rdftSize(rdftSize), rdftBits(static_cast<int>(log2(rdftSize))), 
	rdft(NULL), rdftData(NULL), frames(0), filled(0)
{	
	LogTrace() << "[VideoAnalyzerStream:" << this << ":" << name << "] Creating: " 
		<< rdftSize << ": " << rdftBits << endl;	

    assert(rdftSize);
    assert(rdftBits < kMaxFFTPow2Size);

	// We only allow power of two.
    assert(1UL << rdftBits == rdftSize);
}
	

VideoAnalyzer::Stream::~Stream()
{
	uninitialize();
}


void VideoAnalyzer::Stream::initialize()
{
	frames		= 0;
	filled		= 0;

	assert(rdft == NULL);
	rdft = av_rdft_init(rdftBits, DFT_R2C);
	if (rdft == NULL)
		throw Exception("Cannot allocate FFT context");
	
	assert(rdftData == NULL);
	rdftData = (FFTSample*)av_malloc(rdftSize * sizeof(*rdftData));	
	if (rdftData == NULL)
		throw Exception("Cannot allocate FFT buffer");
}

	
void VideoAnalyzer::Stream::uninitialize()
{
	//LogTrace() << "[VideoAnalyzerStream:" << this << ":" << name << "] Uninitializing" << endl;	
	
	if (rdft)
		av_rdft_end(rdft);
	if (rdftData)
		av_free(rdftData);
}


void VideoAnalyzer::Stream::fft()
{
	assert(filled == rdftSize);
    av_rdft_calc(rdft, rdftData);	
}


// ---------------------------------------------------------------------
//
double CalculateCentroidFrequency(VideoAnalyzer::Stream& stream)
{
	// These two values are used for getting the Spectral Centroid
	// http://en.wikipedia.org/wiki/Spectral_centroid
	double centroidFnXn = 0;
	double centroidXn = 0;	
			
	for (int i = 0; i < stream.filled / 2; i += 2) {				
		centroidFnXn += i * abs(stream.rdftData[i]);
		centroidXn += abs(stream.rdftData[i]);
	}
			
	return centroidFnXn / centroidXn;
}


double CalculateFrequencyIntensity(VideoAnalyzer::Stream& stream)
{
	double intensity = 0.0;			
	for (int i = 0; i < stream.filled / 2; i += 2) {
		intensity += GetFrequencyIntensity(stream.rdftData[i], stream.rdftData[i+1]);
	}
	return intensity;
}


double GetFrequencyIntensity(double re, double im)
{
    return sqrt((re*re)+(im*im));
}


double GetDecibels(double re, double im)
{
	return (re == 0 && im == 0) ? 0 : 
		10.0 * log10(re*re+im*im);
}

double GetAmplitude(double re, double im, int len) 
{
	return GetFrequencyIntensity(re,im)/(len);
}

double GetAmplitudeScaled(double re, double im, int len, int scale) 
{
	return static_cast<int>(GetAmplitude(re,im,len)) % scale;
}


#ifdef WIN32
double log2(double n) 
{
	return log(n) / log(double(2));
}
#endif


} } // namespace Scy::Media




	//FFMIN(_reader.audio()->frameSize, packet.size());
	//_audio->filled = 0;
	//if (_audio->filled != _audio->rdftSize)
	//	LogWarn() << "################# [VideoAnalyzer:" << this << "] Audio FFT Wrong Size: " 
	//		<< _audio->filled << " != " << _audio->rdftSize << endl;	

	//calculateFrequencyIntensity(*_audio, packet.time);
	/*
	

	// TODO: Better way of gathering avg?
	// Not all FFT values are useful.
	avg = sum / (stream.filled / 4);
	packet.value = avg;	
	
	LogTrace("VideoAnalyzer", this) << "Video Output:"
		<< packet.time << "," << packet.value << "," 
		<< packet.min << "," << packet.max << endl;

	PacketOut.emit(this, stream, packet);
	*/
	
			//if (_options.maxFPS)
			//	_reader.video()->maxFPS = _options.maxFPS;
			
			/*
			int i, rdft_bits, win_size, h;

			// RDFT window size (precision) according to the requested output frame height
			for (rdft_bits = 1; 1 << rdft_bits < 2 * _reader.video()->ctx->height; rdft_bits++);
			win_size = 1 << rdft_bits;

			LogTrace() << "[VideoAnalyzer:" << this <<"] rdft_bits: " << rdft_bits << ": " << win_size << ": " << (sizeof(*_video->rdftData) *
					(_reader.video()->ctx->width * 
					 _reader.video()->ctx->height)) << endl;
					 */
	

/*
void VideoAnalyzer::writeCSV(const VideoAnalyzer::Packet& packet) //, double avg
{
	LogTrace("VideoAnalyzer", this) << "Writing:"
		<< packet.name << "," << packet.time << "," << packet.value << "," 
		<< packet.min << "," << packet.max << endl;

	_ofile << packet.name << "," << packet.time << "," << 
		packet.value << "," << packet.min << "," << packet.max << "\r\n";
}
*/

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

	stream.name, 
	
	double value = 0.0;
	double min = 99999.9;
	double max = -99999.9;
	double sum = 0.0;
	double avg = 0.0;

	//for (int i = 0; i < _options.fftPoints / 2; ++i) {
	//for (int i = 1; i < N/2; i += 2) {
	for (int i = 0; i < stream.filled / 2; i += 2) {
	    value = GetFrequencyIntensity(stream.rdftData[i], stream.rdftData[i+1]);
		if (value > max)
			max = value;
		if (value < min)
			min = value;
		//printf("re=%10f im=%10f im=%10f\n", stream.rdftData[i], stream.rdftData[i+1]);
		//LogTrace("VideoAnalyzer", this) << "Value: " 
		//	<< stream.rdftData[i] << ": " << stream.rdftData[i+1] << endl;
		sum += value; 
	}

	// TODO: Better way of gathering avg?
	// Not all FFT values are useful.
	avg = sum / (stream.filled / 4);
	value = avg;	
	
	LogTrace("VideoAnalyzer", this) << "Video Output:"
		<< time << "," << value << "," 
		<< min << "," << max << endl;






	VideoAnalyzer::Packet packet(time);
	double sum = 0.0;
	double avg = 0.0;

	//for (int i = 0; i < _options.fftPoints / 2; ++i) {
	//for (int i = 1; i < N/2; i += 2) {
	for (int i = 0; i < stream.filled / 2; i += 2) {
	    packet.value = GetFrequencyIntensity(stream.rdftData[i], stream.rdftData[i+1]);
		if (packet.value > packet.max)
			packet.max = packet.value;
		if (packet.value < packet.min)
			packet.min = packet.value;
		//printf("re=%10f im=%10f im=%10f\n", stream.rdftData[i], stream.rdftData[i+1]);
		//LogTrace("VideoAnalyzer", this) << "Value: " 
		//	<< stream.rdftData[i] << ": " << stream.rdftData[i+1] << endl;
		sum += packet.value; 
	}

	// TODO: Better way of gathering avg?
	// Not all FFT values are useful.
	avg = sum / (stream.filled / 4);
	packet.value = avg;	
	
	LogTrace("VideoAnalyzer", this) << "Video Output:"
		<< packet.time << "," << packet.value << "," 
		<< packet.min << "," << packet.max << endl;

	PacketOut.emit(this, stream, packet);
	//writeCSV(stream.name, time, avg, min, max);
	*/
	
	
	/*video->frame
	VideoDecoderContext* video = _reader.video();
	// TODO: Conversion via decoder?
	if (_videoConv == NULL) {		
		VideoCodec iparams;
		iparams.width = video->ctx->width;
		iparams.height = video->ctx->height;
		iparams.pixelFmt = av_get_pix_fmt_name(video->ctx->pix_fmt);
		VideoCodec oparams;
		oparams.width = video->ctx->width;
		oparams.height = video->ctx->height;
		oparams.pixelFmt = "gray";		
		_videoConv = new VideoConversionContext();
		_videoConv->create(iparams, oparams);
	}
	if (_videoConv == NULL)
		throw Exception("Video Analyzer: Unable to initialize the video conversion context.");	
		
	// Convert the source image to grayscale
	greyFrame = _videoConv->convert(video->frame);
	*/
	/*	

	// Populate the FFT input data.
	

    /load img1's data to fftw input 
    for( i = 0, k = 0 ; i < height ; i++ ) {
        for( j = 0 ; j < width ; j++ ) {
            data_in[k][0] = ( double )img1_data[i * step + j]* pow(-1,i+j);
            data_in[k][1] = 0.0;
            k++;
        }
    }
	LogTrace("VideoAnalyzer", this) << "^^^^^^^^^^^^^^^^^^ Video: " 
		<< greyFrame->fr << endl;


	*/
	/*
			//_video->filled = y * video->ctx->width + x; // * 3;0.0; //
			//_video->filled++;
			//_video->rdftData[_video->filled++] = 0.0;
	    for (i = 0; i < n; i += NCHANNELS) {
        int k, v = 0;

        for (k = 0; k < NCHANNELS; k++) // mix channels
            v += data[i + k];
        z.rdft_data[z.filled++] = (float)v / NCHANNELS / SHRT_MAX;
        if (z.filled == N) {
            process_spectrum();
            z.filled = 0;
        }
    }
	LogWarn() << "@@@@ [VideoAnalyzer:" << this << "] Filled: " << _video->filled << endl;	
	*/

	
	//if (_video->filled != _video->rdftSize)
	//	LogWarn() << "################# [VideoAnalyzer:" << this << "] Video FFT Wrong Size: " 
	//		<< _video->filled << " != " << _video->rdftSize << endl;	
	/*
	// Skip frames if we exceed the maximum processing framerate.
	double fps = _audio->frames / packet.time;
	if (_options.maxFPS > 0 && fps > _options.maxFPS) {
		LogTrace("VideoAnalyzer", this) << "Skipping audio frame at fps: " << fps << endl;
		return;
	}
	*/

	
	/*
	// Skip frames if we exceed the maximum processing framerate.
	double fps = _video->frames / packet.time;
	if (_options.maxFPS > 0 && fps > _options.maxFPS) {
		LogTrace("VideoAnalyzer", this) << "Skipping video frame at fps: " << fps << endl;
		return;
	}
	*/
		
	/*
	// Create and allocate the conversion frame.
	if (oframe == NULL) {
		oframe = avcodec_alloc_frame();	
		if (oframe == NULL)
			throw Exception("Video Analyzer: Unable to allocate the output video frame.");

		avpicture_alloc(reinterpret_cast<AVPicture*>(oframe), 
			PIX_FMT_GRAY8, video->ctx->width, video->ctx->height);
	}
	
	// Convert the image from its native format to GREY8.
	if (_videoConv == NULL) {
		_videoConv = sws_getContext(
			video->ctx->width, video->ctx->height, video->ctx->pix_fmt, 
			video->ctx->width, video->ctx->height, PIX_FMT_GRAY8, 
			SWS_BICUBIC, NULL, NULL, NULL);
	}
	*/