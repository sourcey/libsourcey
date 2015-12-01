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


#include "scy/media/avencoder.h"

#ifdef HAVE_FFMPEG

#include "scy/media/videocapture.h"
#include "scy/logger.h"
#include "scy/platform.h"
#include "scy/timer.h"
#include "scy/media/flvmetadatainjector.h"

#include "assert.h"

#if WIN32
#define snprintf _snprintf
#endif

extern "C" {
#include "libavutil/time.h" // av_gettime (depreciated)
}


using std::endl;


namespace scy {
namespace av {


AVEncoder::AVEncoder(const EncoderOptions& options) :
	_options(options),	
	_formatCtx(nullptr),
	_video(nullptr),
	_audio(nullptr),
	_audioFifo(nullptr),
	_audioBuffer(nullptr),
	_ioBuffer(nullptr),
	_ioBufferSize(MAX_VIDEO_PACKET_SIZE),
	_videoPtsRemainder(0.0)
{
	TraceLS(this) << "Create" << endl;
	initializeFFmpeg();
}


AVEncoder::AVEncoder() : 	
	_formatCtx(nullptr),
	_video(nullptr),
	_audio(nullptr),
	_audioFifo(nullptr),
	_audioBuffer(nullptr),
	_ioBuffer(nullptr),
	_ioBufferSize(MAX_VIDEO_PACKET_SIZE),
	_videoPtsRemainder(0.0)
{
	TraceLS(this) << "Create" << endl;
	initializeFFmpeg();
}


AVEncoder::~AVEncoder()
{
	TraceLS(this) << "Destroy" << endl;
	uninitialize();
	uninitializeFFmpeg();
}


static int dispatchOutputPacket(void* opaque, UInt8* buffer, int bufferSize)
{
	// Callback example at: http://lists.mplayerhq.hu/pipermail/libav-client/2009-May/003034.html
	AVEncoder* klass = reinterpret_cast<AVEncoder*>(opaque);
	if (klass) {
		TraceL << "Dispatching packet: " << bufferSize << endl;	
		if (!klass->isActive()) {
			WarnL << "Dropping packet: " << bufferSize << ": " << klass->state() << endl;	
			return bufferSize;
		}
		MediaPacket packet((char*)buffer, bufferSize);
		klass->emitter.emit(klass, packet);
		TraceL << "Dispatching packet: OK: " << bufferSize << endl;
	}   

    return bufferSize;
}


void AVEncoder::initialize() 
{
	assert(!isActive());

	TraceLS(this) << "Initialize:"
		<< "\n\tInput Format: " << _options.iformat.toString()
		<< "\n\tOutput Format: " << _options.oformat.toString()
		<< "\n\tDuration: " << _options.duration
		<< endl;

	try {
		// Lock mutex during initialization
		//Mutex::ScopedLock lock(_mutex);

		if (!_options.oformat.video.enabled && 
			!_options.oformat.audio.enabled)
			throw std::runtime_error("Either video or audio parameters must be specified.");

		if (_options.oformat.id.empty())
			throw std::runtime_error("An output container format must be specified.");	

		// TODO: Only need to call this once, but it does not leak memory.
		// Also consider using av_lockmgr_register to protect avcodec_open/avcodec_close
		// See http://src.chromium.org/svn/branches/1229_12/src/media/filters/ffmpeg_glue.cc
		// http://cloudobserver.googlecode.com/svn/trunk/CloudClient/src/filters/multiplexer/multiplexer.cpp
		av_register_all(); 			

		// Allocate the output media context
		assert(!_formatCtx);
		_formatCtx = avformat_alloc_context();
		if (!_formatCtx) 
			throw std::runtime_error("Cannot allocate format context.");

		if (!_options.ofile.empty())
			snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _options.ofile.c_str());
		
		// Set the container codec
		std::string ofmt = _options.ofile.empty() ? ("." + std::string(_options.oformat.id)) : _options.ofile;		
		_formatCtx->oformat = av_guess_format(_options.oformat.id.c_str(), ofmt.c_str(), nullptr);	
		if (!_formatCtx->oformat)
			throw std::runtime_error("Cannot find suitable encoding format for " + _options.oformat.name);			

	// Initialize encoder contexts
	if (_options.oformat.video.enabled)
		createVideo();
	if (_options.oformat.audio.enabled)
		createAudio();		

		if (_options.ofile.empty()) {

			// Operating in streaming mode. Generated packets can be
			// obtained by connecting to the outgoing PacketSignal.
			// Setup the output IO context for our output stream.
			_ioBuffer = new unsigned char[_ioBufferSize];
			_ioCtx = avio_alloc_context(_ioBuffer, _ioBufferSize, 0, this, 0, dispatchOutputPacket, 0);
			//_ioCtx->is_streamed = 1;
			_formatCtx->pb = _ioCtx;
		}
		else {

			// Operating in file mode.  
			// Open the output file...
			if (!(_formatCtx->oformat->flags & AVFMT_NOFILE)) {
				//if (url_fopen(&_formatCtx->pb, _options.ofile.c_str(), URL_WRONLY) < 0) {
				if (avio_open(&_formatCtx->pb, _options.ofile.c_str(), AVIO_FLAG_WRITE) < 0) {
					throw std::runtime_error("AVWriter: Unable to open the output file");
				}
			}
		}

		// Write the stream header (if any)
		// TODO: After Ready state
		avformat_write_header(_formatCtx, nullptr);

		// Send the format information to sdout
		av_dump_format(_formatCtx, 0, _options.ofile.c_str(), 1);
				
		// Get realtime presentation timestamp
		_formatCtx->start_time_realtime = av_gettime();

#if 0   // Live PTS testing
		// Open the output file
		//_file.open("test.flv", ios::out | ios::binary);	
			
		_videoPts = 0;
		_audioPts = 0;
		Int64 delta;
		if (_realtime) {
			if (!_formatCtx->start_time_realtime) {
				_formatCtx->start_time_realtime = av_gettime();
				_videoPts = 0;
			}
			else {
				delta = av_gettime() - _formatCtx->start_time_realtime;
				_videoPts = delta * (float) _video->stream->time_base.den / (float) _video->stream->time_base.num / (float) 1000000;
			}
		}

		stream->time_base.den
		// Setup the PTS calculator for variable framerate inputs
		if (_video) {
			_videoPtsCalc = new PTSCalculator;
			_videoPtsCalc->timeBase = _video->ctx->time_base;
		}
		if (_audio) {
			_audioPtsCalc = new PTSCalculator;
			_audioPtsCalc->timeBase = _audio->ctx->time_base;
		}
#endif
		
		setState(this, EncoderState::Ready);
	} 
	catch (std::exception& exc) {
		ErrorLS(this) << "Error: " << exc.what() << endl;		
		setState(this, EncoderState::Error, exc.what());
		cleanup();
		throw exc; //.rethrow()
	}

	TraceLS(this) << "Initialize: OK" << endl;
}


void AVEncoder::uninitialize()
{
	TraceLS(this) << "Uninitialize" << endl;

 	// Write the trailer and dispatch the tail packet if any
	if (_formatCtx &&
		_formatCtx->pb) 
		av_write_trailer(_formatCtx);

	TraceLS(this) << "Uninitializing: Wrote trailer" << endl;

	// Free memory
	cleanup();	
	setState(this, EncoderState::Stopped);

	TraceLS(this) << "Uninitialize: OK" << endl;
}


void AVEncoder::cleanup()
{
	TraceLS(this) << "Cleanup" << endl;

    // Delete stream encoders
	freeVideo();
	freeAudio();

	// Close the format
	if (_formatCtx) {

	 	// Free all remaining streams
		for (unsigned int i = 0; i < _formatCtx->nb_streams; i++) {
			av_freep(&_formatCtx->streams[i]->codec);
			av_freep(&_formatCtx->streams[i]);
		}

	 	// Close the output file (if any)
		if (!_options.ofile.empty() &&
			_formatCtx->pb &&  
			_formatCtx->oformat && !(_formatCtx->oformat->flags & AVFMT_NOFILE))
			avio_close(_formatCtx->pb);
			//avio_url_fclose(_formatCtx->pb);
		
	 	// Free the format context
		av_free(_formatCtx);
		_formatCtx = nullptr;
	}
	
	//if (_videoPtsCalc) {
	//	delete _videoPtsCalc;
	//	_videoPtsCalc = nullptr;
	//}
	
	if (_ioBuffer) {
		delete _ioBuffer;
		_ioBuffer = nullptr;
	}

	TraceLS(this) << "Cleanup: OK" << endl;
}


EncoderOptions& AVEncoder::options()
{
	//Mutex::ScopedLock lock(_mutex);
	return _options;
}


VideoEncoderContext* AVEncoder::video()
{
	//Mutex::ScopedLock lock(_mutex);
	return _video;
}


AudioEncoderContext* AVEncoder::audio()
{
	//Mutex::ScopedLock lock(_mutex);
	return _audio;
}


//
// Video stuff
//


void AVEncoder::createVideo()
{
	//Mutex::ScopedLock lock(_mutex);	
	assert(!_video);
	assert(_options.oformat.video.enabled);
	assert(_formatCtx->oformat->video_codec != AV_CODEC_ID_NONE);
	_video = new VideoEncoderContext(_formatCtx);
	_video->iparams = _options.iformat.video;
	_video->oparams = _options.oformat.video;
	_video->create();
	_video->open();
}


void AVEncoder::freeVideo()
{
	//Mutex::ScopedLock lock(_mutex);	

	if (_video) {
		delete _video;
		_video = nullptr;
	}
}


bool AVEncoder::encodeVideo(unsigned char* buffer, int bufferSize, int width, int height, UInt64 /* time */)
{
	TraceLS(this) << "Encoding video: " << bufferSize << endl;	
	
	EncoderOptions* options = nullptr;		
	AVFormatContext* formatCtx = nullptr;
	VideoEncoderContext* video = nullptr;	
	{	
		// Lock the mutex while encoding
		//Mutex::ScopedLock lock(_mutex);
		options = &_options;
		formatCtx = _formatCtx;
		video = _video;
	}
	
	assert(isActive());
	assert(video && video->frame);

	if (!isActive())
		throw std::runtime_error("The encoder is not initialized");
	
	if (!video) 
		throw std::runtime_error("No video context");

	if (!buffer || !bufferSize || !width || !height)
		throw std::runtime_error("Invalid video frame");
	
	// Recreate the video conversion context on the fly
	// if the input resolution changes.
	if (options->iformat.video.width != width || 
		options->iformat.video.height != height) {			
		options->iformat.video.width = width;
		options->iformat.video.height = height;
		video->iparams.width = width;
		video->iparams.height = height;
		TraceLS(this) << "Recreating video conversion context" << endl;
		video->freeConverter();
		video->createConverter();
	}

	AVPacket opacket;
	if (formatCtx->oformat->flags & AVFMT_RAWPICTURE) {
		opacket.flags |= AV_PKT_FLAG_KEY;
		opacket.stream_index = video->stream->index;
		opacket.data = (UInt8*)buffer;
		opacket.size = sizeof(AVPicture);
	} 
	else {
		
 		// Encode the frame
		if (!video->encode(buffer, bufferSize, /*calc ? calc->tick() : */AV_NOPTS_VALUE, opacket)) {
			WarnL << "Cannot encode video frame" << endl;
			return false;
		}
	}

	if (opacket.size > 0) {
		assert(opacket.stream_index == video->stream->index);
		opacket.dts = AV_NOPTS_VALUE; 
		
		// Calculate our own PTS from stream time
		// TODO: Setting PTS with audio seems to throw mp4
		// encoding out of sync; need to test more...
		if (!options->oformat.audio.enabled) {
			Int64 delta;
			delta = av_gettime() - _formatCtx->start_time_realtime;
			double framePTS = delta * (double) _video->stream->time_base.den / (double) _video->stream->time_base.num / (double) 1000000;
			double ptsWhole;
			_videoPtsRemainder += modf(framePTS, &ptsWhole); // fixme
			opacket.pts = (Int64)ptsWhole;
			opacket.dts = AV_NOPTS_VALUE; 
			if (static_cast<int>(_videoPtsRemainder) > 1) {
				_videoPtsRemainder--;
				opacket.pts++;
			}
		}		
		
#if 0
		TraceLS(this) << "Writing video:" 
			<< "\n\tPTS: " << opacket.pts
			<< "\n\tDTS: " << opacket.dts
			<< "\n\tFPS: " << video->fps.fps
			//<< "\n\tTime: " << time
			<< "\n\tDuration: " << opacket.duration
			<< endl;
#endif
		
		// Write the encoded frame to the output file / stream.
		assert(isActive());
		if (av_interleaved_write_frame(formatCtx, &opacket) < 0) {
			WarnL << "Cannot write video frame" << endl;
			return false;
		}		
	}

	return true;
}


//
// Audio stuff
//


void AVEncoder::createAudio()
{
	TraceLS(this) << "Create Audio" << endl;

	//Mutex::ScopedLock lock(_mutex);	
	assert(!_audio);
	assert(_options.oformat.audio.enabled);
	assert(_formatCtx->oformat->audio_codec != AV_CODEC_ID_NONE);

	_audio = new AudioEncoderContext(_formatCtx);
	_audio->iparams = _options.iformat.audio;
	_audio->oparams = _options.oformat.audio;
	_audio->create();
	_audio->open();
		
	// The encoder may require a minimum number of raw audio
	// samples for each encoding but we can't guarantee we'll
	// get this minimum each time an audio frame is decoded
	// from the in file, so we use a FIFO to store up incoming
	// raw samples until we have enough to call the codec.
	_audioFifo = av_fifo_alloc(_audio->outputFrameSize * 2);

	// Allocate a buffer to read OUT of the FIFO into. 
	// The FIFO maintains its own buffer internally.
	_audioBuffer = (UInt8*)av_malloc(_audio->outputFrameSize);
}


void AVEncoder::freeAudio()
{
	//Mutex::ScopedLock lock(_mutex);	

	if (_audio) {
		delete _audio;
		_audio = nullptr;
	}
	
	if (_audioFifo) {
		av_fifo_free(_audioFifo);
		_audioFifo = nullptr;
	}
	
	if (_audioBuffer) {
		av_free(_audioBuffer);
		_audioBuffer = nullptr;
	}
}


bool AVEncoder::encodeAudio(unsigned char* buffer, int bufferSize, UInt64 /* time */)
{
	//TraceLS(this) << "Encoding Audio Packet: " << bufferSize << endl;	
	assert(buffer);
	assert(bufferSize);

	EncoderOptions* options = nullptr;		
	AVFormatContext* formatCtx = nullptr;
	AudioEncoderContext* audio = nullptr;	
	AVFifoBuffer* audioFifo = nullptr;	
	UInt8* audioBuffer = nullptr;	
	{	
		//Mutex::ScopedLock lock(_mutex);
		options = &_options;
		formatCtx = _formatCtx;
		audio = _audio;
		audioFifo = _audioFifo;	
		audioBuffer = _audioBuffer;	
	}

	if (!isActive())
		throw std::runtime_error("The encoder is not initialized");
	
	if (!audio) 
		throw std::runtime_error("No audio context");
	
	if (!buffer || !bufferSize) 
		throw std::runtime_error("Invalid audio input");
		
	// TODO: Move FIFO to the encoder context.
	bool res = false;
	av_fifo_generic_write(audioFifo, (UInt8*)buffer, bufferSize, nullptr);
	while (av_fifo_size(audioFifo) >= _audio->outputFrameSize) {
		av_fifo_generic_read(audioFifo, audioBuffer, audio->outputFrameSize, nullptr);
		
		AVPacket opacket;
			
		//assert(calc);
		if (audio->encode(audioBuffer, audio->outputFrameSize, /*calc ? calc->tick() : */AV_NOPTS_VALUE, opacket)) {
			assert(opacket.stream_index == audio->stream->index);
			assert(opacket.data);
			assert(opacket.size);
			
			/*	
			opacket.pts = _videoPts; //AV_NOPTS_VALUE;
			opacket.dts = AV_NOPTS_VALUE; 

			TraceLS(this) << "Writing Audio:" 
				<< "\n\tPacket Size: " << opacket.size
				<< "\n\tPTS: " << opacket.pts
				<< "\n\tDTS: " << opacket.dts
				<< "\n\tDuration: " << opacket.duration
				<< endl;
				*/

	 		// Write the encoded frame to the output file
			assert(isActive());
			if (av_interleaved_write_frame(formatCtx, &opacket) != 0) {
				WarnL << "Cannot write audio frame" << endl;
			}
			else res = true;
		} 
	}
	
	return res;
}


} } // namespace scy::av


#endif