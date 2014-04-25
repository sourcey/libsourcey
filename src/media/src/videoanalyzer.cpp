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


#include "scy/media/videoanalyzer.h"


#ifdef HAVE_FFMPEG


using std::endl;


namespace scy {
namespace av {


VideoAnalyzer::VideoAnalyzer(const Options& options) : 
	_options(options),
	_video(nullptr),
	_audio(nullptr),
	_videoConv(nullptr)
{
	TraceLS(this) << "Create" << endl;
}


VideoAnalyzer::~VideoAnalyzer() 
{
	TraceLS(this) << "Destroy" << endl;
	uninitialize();
}


void VideoAnalyzer::initialize()
{
	if (_options.ifile.empty())
		throw std::runtime_error("Please specify an input file.");

	TraceLS(this) << "Loading: " << _options.ifile << endl;

	_error = "";

	// Open the input file decoder.
	_reader.openFile(_options.ifile);
		
	if (_reader.video()) {
		_videoConv = nullptr;
		_video = new VideoAnalyzer::Stream("Video", _options.rdftSize);
		_video->initialize();	
	}

	if (_reader.audio()) {
		_audio = new VideoAnalyzer::Stream("Audio", _options.rdftSize);
		_audio->initialize();
	}
	
	_reader += packetDelegate(this, &VideoAnalyzer::onVideo);
	_reader += packetDelegate(this, &VideoAnalyzer::onAudio);

	_reader.ReadComplete += sdelegate(this, &VideoAnalyzer::onReadComplete);
}

	
void VideoAnalyzer::uninitialize()
{
	//TraceL << "[VideoAnalyzerStream: " << this << ": " << name << "] Uninitializing" << endl;	
	stop();
	
	//Mutex::ScopedLock lock(_mutex); 

	if (_video)
		delete _video;
	if (_videoConv)
		delete _videoConv;
	if (_audio)
		delete _audio;
}


void VideoAnalyzer::start()
{
	Mutex::ScopedLock lock(_mutex); 
	
	try 
	{
		if (!_reader.audio() && !_reader.video())
			throw std::runtime_error("Video reader not initialized");

		//if (_options.blocking)
		//	_reader.run();
		//else
		_reader.start();
	} 
	catch (std::exception& exc) 
	{
		_error = exc.what();
		ErrorLS(this) << "Error: " << _error << endl;
		throw exc; //.rethrow()
	}
}


void VideoAnalyzer::stop() 
{
	
	// Can't lock here in case we inside a callback.
	//Mutex::ScopedLock lock(_mutex); 
	
	_reader.ReadComplete -= sdelegate(this, &VideoAnalyzer::onReadComplete);
	_reader.detach(this);
	_reader.stop();
}


void VideoAnalyzer::onVideo(void*, VideoPacket& packet)
{
	//TraceLS(this) << "On video: " 
	//	<< packet.size() << ": " << packet.time << endl;
	
	VideoAnalyzer::Packet pkt(packet.time);
	AVFrame* greyFrame = getGrayVideoFrame();
    int width  = greyFrame->width;
    int height = greyFrame->height;
	int step   = greyFrame->linesize[0];
	int frames = 0;
	//VideoDecoderContext* video = _reader.video();
	
	Mutex::ScopedLock lock(_mutex); 
	
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

		TraceLS(this) << "Video Output: "
			<< pkt.time << ", " << pkt.value << endl;
		PacketOut.emit(this, *_video, pkt);
	}
}

void VideoAnalyzer::onAudio(void*, AudioPacket& packet)
{
	//TraceLS(this) << "On Audio: " 
	//  << packet.size() << ": " << packet.time << endl;	

	Mutex::ScopedLock lock(_mutex);		
	
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

		TraceLS(this) << "Audio Output: "
			<< pkt.time << ", " << pkt.value << endl;
		PacketOut.emit(this, *_audio, pkt);
	}
}


AVFrame* VideoAnalyzer::getGrayVideoFrame()
{		
	Mutex::ScopedLock lock(_mutex); 
	VideoDecoderContext* video = _reader.video();

	// TODO: Conversion via decoder?
	if (_videoConv == nullptr) {		
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
	if (_videoConv == nullptr)
		throw std::runtime_error("Video Analyzer: Unable to initialize the video conversion context.");	
		
	// Convert the source image to grayscale
	return _videoConv->convert(video->frame);
}


void VideoAnalyzer::onReadComplete(void* sender)
{
	TraceLS(this) << "On Read Complete" << endl;	

	AVInputReader* reader = reinterpret_cast<AVInputReader*>(sender);	
	{
		Mutex::ScopedLock lock(_mutex); 
		if (_error.empty())
			_error = reader->error();
	}

	Complete.emit(this);
}


AVInputReader& VideoAnalyzer::reader()
{ 
	Mutex::ScopedLock lock(_mutex);
	return _reader; 
}


VideoAnalyzer::Options& VideoAnalyzer::options()
{ 
	Mutex::ScopedLock lock(_mutex);
	return _options; 
}


std::string VideoAnalyzer::error() const
{
	Mutex::ScopedLock lock(_mutex);	
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
	rdft(nullptr), rdftData(nullptr), frames(0), filled(0)
{	
	TraceL << "[VideoAnalyzerStream: " << this << ": " << name << "] Creating: " 
		<< rdftSize << ": " << rdftBits << endl;	

    assert(rdftSize);
    assert(rdftBits < kMaxFFTPow2Size);

	// We only allow power of two.
    assert(int(1UL << rdftBits) == rdftSize);
}
	

VideoAnalyzer::Stream::~Stream()
{
	uninitialize();
}


void VideoAnalyzer::Stream::initialize()
{
	frames		= 0;
	filled		= 0;

	assert(rdft == nullptr);
	rdft = av_rdft_init(rdftBits, DFT_R2C);
	if (rdft == nullptr)
		throw std::runtime_error("Cannot allocate FFT context");
	
	assert(rdftData == nullptr);
	rdftData = (FFTSample*)av_malloc(rdftSize * sizeof(*rdftData));	
	if (rdftData == nullptr)
		throw std::runtime_error("Cannot allocate FFT buffer");
}

	
void VideoAnalyzer::Stream::uninitialize()
{
	//TraceL << "[VideoAnalyzerStream: " << this << ": " << name << "] Uninitializing" << endl;	
	
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


} } // namespace scy::av


#endif