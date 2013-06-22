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


#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Media/FLVMetadataInjector.h"

#include "assert.h"

#if WIN32
#define snprintf _snprintf
#endif


using namespace std;
using namespace Poco;


namespace Scy {
namespace Media {


FastMutex AVEncoder::_mutex;


AVEncoder::AVEncoder(const RecordingOptions& options) :
	_options(options),	
	_formatCtx(NULL),
	_video(NULL),
	_audio(NULL),
	_audioFifo(NULL),
	_audioBuffer(NULL),
	_ioBuffer(NULL),
	_ioBufferSize(MAX_VIDEO_PACKET_SIZE),
	_startTime(0)
{
}


AVEncoder::AVEncoder() : 	
	_formatCtx(NULL),
	_video(NULL),
	_audio(NULL),
	_audioFifo(NULL),
	_audioBuffer(NULL),
	_ioBuffer(NULL),
	_ioBufferSize(MAX_VIDEO_PACKET_SIZE),
	_startTime(0)
{
	LogDebug("AVEncoder", this) << "Initializing" << endl;
}


AVEncoder::~AVEncoder()
{
	LogDebug("AVEncoder", this) << "Destroying" << endl;
	uninitialize();
}


int DispatchOutputPacket(void* opaque, UInt8* buffer, int bufferSize)
{
	// Callback example at: http://lists.mplayerhq.hu/pipermail/libav-client/2009-May/003034.html
	AVEncoder* klass = reinterpret_cast<AVEncoder*>(opaque);
	if (klass) {
		LogTrace("AVEncoder", klass) << "Dispatching Packet: " << bufferSize << endl;		
		assert(klass->isActive());
		MediaPacket packet(buffer, bufferSize);
		klass->emit(klass, packet);
		LogTrace("AVEncoder", klass) << "Dispatching Packet: OK: " << bufferSize << endl;
	}   

    return bufferSize;
}


void AVEncoder::initialize() 
{
	assert(!isActive());

	LogDebug("AVEncoder", this) << "Initializing:"
		<< "\n\tInput Format: " << _options.iformat.toString()
		<< "\n\tOutput Format: " << _options.oformat.toString()
		<< "\n\tDuration: " << _options.duration
		<< endl;

	try {
		{
			LogDebug("AVEncoder", this) << "Initializing: 1" << endl;

			// Lock our mutex during initialization
			FastMutex::ScopedLock lock(_mutex);
			
			LogDebug("AVEncoder", this) << "Initializing: 2" << endl;

			if (!_options.oformat.video.enabled && 
				!_options.oformat.audio.enabled)
				throw Exception("Either video or audio parameters must be specified.");

			if (_options.oformat.id.empty())
				throw Exception("An output container format must be specified.");		
			
			LogDebug("AVEncoder", this) << "Initializing: 3" << endl;

			// TODO: Only need to call this once, but it does not leak memory.
			// Also consoder using av_lockmgr_register to protect avcodec_open/avcodec_close
			// See http://src.chromium.org/svn/branches/1229_12/src/media/filters/ffmpeg_glue.cc
			// http://cloudobserver.googlecode.com/svn/trunk/CloudClient/src/filters/multiplexer/multiplexer.cpp
			av_register_all(); 			
			LogDebug("AVEncoder", this) << "Initializing: 4" << endl;

			// Allocate the output media context
			assert(!_formatCtx);
			_formatCtx = avformat_alloc_context();
			if (!_formatCtx) 
				throw Exception("Cannot allocate format context.");

			if (!_options.ofile.empty())
				snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _options.ofile.data());
		
			// Set the container codec
			string ofmt = _options.ofile.empty() ? ("." + string(_options.oformat.id)) : _options.ofile;		
			_formatCtx->oformat = av_guess_format(_options.oformat.id.data(), ofmt.data(), NULL);	
			if (!_formatCtx->oformat)
				throw Exception("Cannot find suitable encoding format for " + _options.oformat.name);			
		}
		LogDebug("AVEncoder", this) << "Initializing: 5" << endl;

		// Initialize encoder contexts
		createVideo();
		createAudio();		
		{
			// Lock our mutex during initialization
			FastMutex::ScopedLock lock(_mutex);

			if (_options.ofile.empty()) {

				// Operating in streaming mode. Generated packets can be
				// obtained by connecting to the PacketEncoded signal.
				// Setup the output IO context for our output stream.
				_ioBuffer = new unsigned char[_ioBufferSize];
				_ioCtx = avio_alloc_context(_ioBuffer, _ioBufferSize, 0, this, 0, DispatchOutputPacket, 0);
				//_ioCtx->is_streamed = 1;
				_formatCtx->pb = _ioCtx;
			}
			else {

				// Operating in file mode.  
				// Open the output file...
				if (!(_formatCtx->oformat->flags & AVFMT_NOFILE)) {
					//if (url_fopen(&_formatCtx->pb, _options.ofile.data(), URL_WRONLY) < 0) {
					if (avio_open(&_formatCtx->pb, _options.ofile.data(), AVIO_FLAG_WRITE) < 0) {
						throw Exception("AVWriter: Unable to open the output file");
					}
				}
			}
		
			// Open the output file
			//_file.open("test.flv", ios::out | ios::binary);	

			// Write the stream header (if any)
			avformat_write_header(_formatCtx, NULL);

			// Send the format information to sdout
			av_dump_format(_formatCtx, 0, _options.ofile.data(), 1);

			_startTime = clock();
		}
		
		LogDebug("AVEncoder", this) << "Initializing: 9" << endl;
		setState(this, EncoderState::Ready);
		LogDebug("AVEncoder", this) << "Initializing: 10" << endl;
	} 
	catch (Exception& exc) {
		LogError("AVEncoder", this) << "Error: " << exc.displayText() << endl;		
		cleanup();
		setState(this, EncoderState::Error, exc.displayText());
		exc.rethrow();
	}
}


void AVEncoder::uninitialize()
{
	LogDebug("AVEncoder", this) << "Uninitializing" << endl;
	cleanup();	
	setState(this, EncoderState::Stopped);
}


void AVEncoder::cleanup()
{
	LogDebug("AVEncoder", this) << "Cleanup" << endl;
	{
 		// Lock our mutex during closure
		FastMutex::ScopedLock lock(_mutex);	

 		// Write the trailer
		if (_formatCtx &&
			_formatCtx->pb) 
			av_write_trailer(_formatCtx);
	}

    // Delete stream encoders
	freeVideo();
	freeAudio();
	{
 		// Lock our mutex during closure
		FastMutex::ScopedLock lock(_mutex);	

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
			_formatCtx = NULL;
		}
	
		if (_ioBuffer) {
			delete _ioBuffer;
			_ioBuffer = NULL;
		}
	}

	LogDebug("AVEncoder", this) << "Cleanup: OK" << endl;
}


void AVEncoder::process(IPacket& packet)
{	
	// TODO: Move to AVPacketEncoder class
	if (!isActive()) {
		LogWarn("AVEncoder", this) << "Encoder not initialized: Dropping Packet: " << packet.className() << endl;
		return;
	}

	// We may be receiving either audio or video packets	
	VideoPacket* videoPacket = dynamic_cast<VideoPacket*>(&packet);
	if (videoPacket) {		
		encodeVideo(videoPacket->data(), videoPacket->size(), videoPacket->width, videoPacket->height);
		return;
	}

	AudioPacket* audioPacket = dynamic_cast<AudioPacket*>(&packet);
	if (audioPacket) {		
		encodeAudio(audioPacket->data(), audioPacket->size());
		return;
	}
	
	LogWarn("AVEncoder", this) << "Cannot encode source packet" << endl;
	emit(this, packet);
}

					
void AVEncoder::onStreamStateChange(const PacketStreamState& state) 
{ 
	// TODO: Move to AVPacketEncoder class
	LogDebug("AVEncoder", this) << "Stream State Changed: " << state << endl;

	switch (state.id()) {
	case PacketStreamState::Running:
		if (stateEquals(EncoderState::None))
			initialize();
		break;
		
	case PacketStreamState::Stopped:
	case PacketStreamState::Error:
	case PacketStreamState::Resetting:
		if (stateEquals(EncoderState::Encoding))
			uninitialize();
		break;
	//case PacketStreamState::None:
	//case PacketStreamState::Closing:
	//case PacketStreamState::Closed:
	}

	PacketEmitter::onStreamStateChange(state);
}


RecordingOptions& AVEncoder::options()
{
	FastMutex::ScopedLock lock(_mutex);
	return _options;
}


VideoEncoderContext* AVEncoder::video()
{
	FastMutex::ScopedLock lock(_mutex);
	return _video;
}


AudioEncoderContext* AVEncoder::audio()
{
	FastMutex::ScopedLock lock(_mutex);
	return _audio;
}


//
// Video Stuff
//

void AVEncoder::createVideo()
{
	FastMutex::ScopedLock lock(_mutex);	
	assert(!_video);

	// Initialize the video encoder (if required)
	if (_options.oformat.video.enabled) { //&& _formatCtx->oformat->video_codec != CODEC_ID_NONE		
		LogTrace("AVEncoder", this) << "Creating Video" << endl;
		_video = new VideoEncoderContext(_formatCtx);
		_video->iparams = _options.iformat.video;
		_video->oparams = _options.oformat.video;
		_video->create();
		_video->open();
	}
}


void AVEncoder::freeVideo()
{
	FastMutex::ScopedLock lock(_mutex);	

	if (_video) {
		delete _video;
		_video = NULL;
	}
}


bool AVEncoder::encodeVideo(unsigned char* buffer, int bufferSize, int width, int height)
{
	LogTrace("AVEncoder", this) << "Encoding Video Packet: " << bufferSize << endl;	
	
	RecordingOptions* options = NULL;		
	AVFormatContext* formatCtx = NULL;
	VideoEncoderContext* video = NULL;	
	{	
		// Lock the mutex while encoding
		FastMutex::ScopedLock lock(_mutex);
		options = &_options;
		formatCtx = _formatCtx;
		video = _video;
	}

	assert(video);
	assert(video->frame);

	if (!isActive())
		throw Exception("The encoder is not initialized.");
	
	if (!video) 
		throw Exception("No video context");

	if (!buffer || !bufferSize || !width || !height)
		throw Exception("Cannot encode video frame.");
	
	// Recreate the video conversion context if the
	// input resolution changes.
	if (options->iformat.video.width != width || 
		options->iformat.video.height != height) {			
		options->iformat.video.width = width;
		options->iformat.video.height = height;
		video->iparams.width = width;
		video->iparams.height = height;
		LogDebug("AVEncoder", this) << "Recreating video conversion context" << endl;
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
		if (!video->encode(buffer, bufferSize, opacket)) {
			LogWarn("AVEncoder", this) << "Cannot encode video frame" << endl;
			return false;
		}
	}

	if (opacket.size > 0) {
		assert(opacket.stream_index == video->stream->index);
		
		// Calculate dynamic PTS based on duration
		// Using MythTV AVFormatWriter as a guide
		opacket.pts = (int64_t)video->fps.duration * 
			video->stream->time_base.den / 
			video->stream->time_base.num;
		opacket.dts = AV_NOPTS_VALUE;
		
		/*
		LogTrace() << "[AVEncoder:" << this << "] Writing Video:" 
			<< "\n\tPTS: " << opacket.pts
			<< "\n\tDTS: " << opacket.dts
			<< "\n\tDuration: " << opacket.duration
			<< "\n\tFPS Duration: " << videoFPS.duration
			<< "\n\tOpts Duration: " << options->duration
			<< endl;
			*/

		// Write the encoded frame to the output file / stream.
		assert(isActive());
		if (av_interleaved_write_frame(formatCtx, &opacket) < 0) {
			LogError("AVEncoder", this) << "Cannot write video frame" << endl;
			return false;
		}
	}

	return true;
}


//
// Audio Stuff
//

void AVEncoder::createAudio()
{
	FastMutex::ScopedLock lock(_mutex);	
	assert(!_audio);

	// Initialize the audio encoder (if required)
	if (_options.oformat.audio.enabled) { //&& _formatCtx->oformat->audio_codec != CODEC_ID_NONE		
		LogTrace("AVEncoder", this) << "Creating Audio" << endl;
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
}


void AVEncoder::freeAudio()
{
	FastMutex::ScopedLock lock(_mutex);	

	if (_audio) {
		delete _audio;
		_audio = NULL;
	}
	
	if (_audioFifo) {
		av_fifo_free(_audioFifo);
		_audioFifo = NULL;
	}
	
	if (_audioBuffer) {
		av_free(_audioBuffer);
		_audioBuffer = NULL;
	}
}


bool AVEncoder::encodeAudio(unsigned char* buffer, int bufferSize)
{
	//LogTrace("AVEncoder", this) << "Encoding Audio Packet: " << bufferSize << endl;	
	assert(buffer);
	assert(bufferSize);

	RecordingOptions* options = NULL;		
	AVFormatContext* formatCtx = NULL;
	AudioEncoderContext* audio = NULL;	
	AVFifoBuffer* audioFifo = NULL;	
	UInt8* audioBuffer = NULL;	
	{	
		FastMutex::ScopedLock lock(_mutex);
		options = &_options;
		formatCtx = _formatCtx;
		audio = _audio;
		audioFifo = _audioFifo;	
		audioBuffer = _audioBuffer;	
	}

	if (!isActive())
		throw Exception("The encoder is not initialized");
	
	if (!audio) 
		throw Exception("No audio context");
	
	if (!buffer || !bufferSize) 
		throw Exception("Invalid audio input");
		
	// TODO: Move FIFO to the encoder context.
	bool res = false;
	av_fifo_generic_write(audioFifo, (UInt8*)buffer, bufferSize, NULL);
	while (av_fifo_size(audioFifo) >= _audio->outputFrameSize) {
		av_fifo_generic_read(audioFifo, audioBuffer, audio->outputFrameSize, NULL);
		
		AVPacket opacket;
		if (audio->encode(audioBuffer, audio->outputFrameSize, opacket)) {
			assert(opacket.stream_index == audio->stream->index);
			assert(opacket.data);
			assert(opacket.size);
			
			/*
			// NOTE: No need to specify audio PTS for MPEG4,
			// but we should run further tests with other codecs.
			//_audioFPS.tick();
			//opacket.pts = (double)_audioFPS.duration * _audio->stream->time_base.den / _audio->stream->time_base.num;
			//opacket.dts = AV_NOPTS_VALUE;
			
			LogTrace() << "[AVEncoder:" << this << "] Writing Audio:" 
				<< "\n\tPacket Size: " << opacket.size
				<< "\n\tPTS: " << opacket.pts
				<< "\n\tDTS: " << opacket.dts
				<< "\n\tDuration: " << opacket.duration
				<< endl;
				*/

	 		// Write the encoded frame to the output file
			if (av_interleaved_write_frame(formatCtx, &opacket) != 0) {
				LogError("AVEncoder", this) << "Cannot write audio frame" << endl;
			}
			else res = true;
		} 
	}
	
	return res;
}


} } // namespace Scy::Media


		
	
	/*
	// If we are encoding a multiplex stream wait for the first
	// video frame to be encoded before we start encoding audio
	// otherwise we get errors for some codecs.
	if (_video && _videoFPS.frames == 0) {
		LogTrace("AVEncoder", this) << "Encoding Audio Packet: Dropping audio frames until we have video." << endl;
		return false;
	}
	*/
	/*
		//if (_audio->encode(buffer, bufferSize, opacket)) { //_audio->outputFrameSizebufferSize
		*/
		
	// while(packet->size > 0)
	// {
	// 	 int ret = avcodec_decode_video2(..., ipacket);
	// 	 if(ret < -1)
	//		throw std::exception("error");
	//
	//	 ipacket->size -= ret;
	//	 ipacket->data += ret;
	// }

	//unsigned char* bytes = buffer;
	//int bytesRemaining = bufferSize;
	//assert(bufferSize > _audio->outputFrameSize);
	//while (bytesRemaining) // && !frameDecoded
	//{
		/*
		avcodec_get_frame_defaults(frame);
		bytesDecoded = avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);		
		if (bytesDecoded < 0) {
			LogError() << "[AudioDecoderContext:" << this << "] Decoder Error" << endl;
			error = "Decoder error";
			throw Exception(error);
		}
		*/
	//}
		
		//bytesRemaining -= _audio->outputFrameSize;
		//buffer += _audio->outputFrameSize;

		
			// Calculate dynamic PTS based on framerate
			//_audioFPS.tick();
			//_audioPTS += _audio->ctx->time_base.den / _audioFPS.fps;
			//if (_audioPTS < opacket.pts)
			//	_audioPTS = opacket.pts;
			//_audioPTS++;
			//opacket.pts = _audioPTS;
			//opacket.dts = _audioPTS;		
		
			//opacket.pts = AV_NOPTS_VALUE;
			//opacket.dts = AV_NOPTS_VALUE;	
					
			// NOTE: Duration and PTS have no bearing on output 
			// file duration and playback speed when audio only.
			//double frameDuration = _audioTime ? ((clock() - _audioTime)) : 1;	// / CLOCKS_PER_SEC
			//_audioTime = clock();
			//opacket.duration = frameDuration;


		// Set the frame duration of dynamic frames.
		// FFmpeg will calculate the PTS for us.		
			
		//unsigned frameDuration = _videoTime ? (clock() - _videoTime) : 1;		

		//opacket.pts = AV_NOPTS_VALUE;
		//opacket.duration = 100; 
		//AV_NOPTS_VALUE

		//opacket.duration = 5; //frameDuration * 2; //_frameDuration ? (clock() - _frameDuration) : 1;
		//LogDebug("AVEncoder", this) << "_videoTime: " << _videoTime << endl;
		//LogDebug("AVEncoder", this) << "opacket.duration: " << opacket.duration << endl;
		
			
		// PTS value will increment by 1 for input each frame at defined FPS value.
		// PTS value will need to be dynamically generated for variable FPS rates.
		//_videoFPS.tick();
		//if (_video->frame->pts == AV_NOPTS_VALUE)
		//	_video->frame->pts = 0;
		//else {
			//_video->frame->pts = _videoPTS;
		//}
		/*		
		
			//<< "\n\tTest PTS: " << av_rescale_q(_videoPTS, _video->ctx->time_base, _video->stream->time_base)
			//<< "\n\tTest PTS 1: " << av_rescale_q(frameDuration, _video->ctx->time_base, _video->stream->time_base)
			//<< "\n\tTest PTS 2: " << (_video->ctx->time_base.den * 1.0) / (_video->stream->time_base.num  * 1.0)
			//<< "\n\tTest PTS 4 " << av_rescale_q(_video->pts, _video->ctx->time_base, _video->stream->time_base)
			//<< "\n\tFrame Duration: " << (frameDuration)
			//<< "\n\tFPS Den: " << (_video->ctx->time_base.den)
			<< "\n\tFPS: " << (_videoFPS.fps)
			<< "\n\t>codec->time_base: " << (_video->stream->time_base.den / _video->stream->time_base.num / 1000)
			<< "\n\t>codec->time_base: " << (_video->stream->time_base.den / _video->stream->time_base.num)
			<< "\n\t>stream->time_base: " << (av_gettime() - _startTime)
			<< "\n\tFPS Duration: " << (_videoFPS.duration)
			<< "\n\t_videoPTS: " << _videoPTS
			//<< "\n\tFrame PTS: " << _video->frame->ti
			//<< "\n\tVIDEO PTS: " << _video->pts


		end = clock();
		duration += (double)(end - start) / CLOCKS_PER_SEC;
		frames++;
		fps = (1.0 * frames) / duration;
		return fps;		
		
		_videoFPS.tick();
		double val = (_video->ctx->time_base.den * 1.0) / (_videoFPS.fps * 1.0);
		//val = 
		double fpsDiff = val; //_videoFPS.frames > val ? (_videoFPS.frames * 1.0) : val; //val; //fps ? () : 1; //  * 1.0
		_videoPTS = _videoPTS + fpsDiff;
		opacket.pts = _videoPTS;
		opacket.dts = _videoPTS;
		*/
		// ------------------------------------------
		//(double)
		//   * 1.0		//double fpsDiff = _videoFPS.fps ? ((_video->ctx->time_base.den * 1.0) / (_videoFPS.fps  * 1.0)) : 1;
		//double val = ((double)(_video->ctx->time_base.den * 1.0) / (double)(_videoFPS.fps  * 1.0));
			 // * 1.0; //_videoPTS + val; //fpsDiff; //_videoPTS + fpsDiff;
		//double _videoFPS.frames > ((int)_videoPTS + val) ? (double)(_videoFPS.frames * 1.0) : val;
		//if (_videoPTS < opacket.pts)
		// ---------------------------------------------


		//audio_pts = (double)audio_st->pts.val * audio_st->time_base.num / audio_st->time_base.den;
			
		//double frameDuration = _videoTime ? ((clock() - _videoTime)) : 1;	// / CLOCKS_PER_SEC
		//_videoPTS = _videoPTS + ((_video->ctx->time_base.den * 1.0) / frameDuration);
		//_videoTime = clock();
		//LogDebug("AVEncoder", this) << "### Frame Duration: " << frameDuration << endl;
		//_videoPTS + 
		//_videoPTS = _videoPTS + frameDuration; //(frameDuration * ((_video->ctx->time_base.den * 1.0) / (_videoFPS.fps  * 1.0)));
			//_video->stream->time_base.den / (1000000.0 * _video->stream->time_base.num));
			//_videoPTS + frameDuration;
		//opacket.pts = (double)opacket.pts * _video->stream->time_base.num / _video->stream->time_base.den;
		
		//_videoPTS; //int64_t((double)frameDuration * _video->stream->time_base.den / (1000000.0 * _video->stream->time_base.num)); //
		
		
			//opacket.pts = _videoPTS;
			//opacket.dts = _videoPTS;

		//LogDebug("AVEncoder", this) << "_video->stream->start_time: " << _video->stream->start_time << endl;
		//LogDebug("AVEncoder", this) << "opacket.pts: " << opacket.pts << endl;
       //fPacket.pts = int64_t((double)encodeInfo->start_time
       //        * fStream->time_base.den / (1000000.0 * fStream->time_base.num)
       //        + 0.5);
	   /*
       TRACE_PACKET("  PTS: %lld  (stream->time_base: (%d/%d), "
               "codec->time_base: (%d/%d))\n", fPacket.pts,
               fStream->time_base.num, fStream->time_base.den,
               fStream->codec->time_base.num, fStream->codec->time_base.den);
			   */

		//fPacket.pts = (encodeInfo->start_time * fStream->time_base.den / fStream->time_base.num) / 1000000;

		//opacket.pts /= 1000; //av_rescale_q(_videoPTS, _video->ctx->time_base, _video->stream->time_base); //_videoPTS; //av_rescale_q(_videoPTS, _video->ctx->time_base, _video->stream->time_base);
		//LogDebug("AVEncoder", this) << "opacket.pts: " << opacket.pts << endl;

		//_videoTime = clock();	
		//_frameDuration = clock();	
		
		//LogTrace() << "@@@@@@@@@@@@@@@@@@@@ [AVEncoder:" << this << "] Video PTS:" << opacket.pts++ << endl;

		/*
		_videoPTS += _video->ctx->time_base.den / (_videoFPS.fps + 3);
		if (_videoPTS < opacket.pts)
			_videoPTS = opacket.pts;
		opacket.pts = _videoPTS;
		opacket.dts = _videoPTS; //AV_NOPTS_VALUE;
		*/
		
		//m_pkt->pts = tc * m_videoStream->time_base.den / m_videoStream->time_base.num / 1000; // / 1000;

		/*
		_videoPTS += 5;
		opacket.pts = AV_NOPTS_VALUE;
		opacket.dts = AV_NOPTS_VALUE;_videoPTS; //
					
		//double frameDuration = _videoTime ? ((clock() - _videoTime)) : 1;	// / CLOCKS_PER_SEC
		//_videoTime = clock();
		//_videoFPS.tick();
		//opacket.duration = frameDuration;

		//if (!_videoTime)
		//	_videoTime = clock();
		//else
		//	_videoTime = clock();
		*/
		

		// Set the output frame size for encoding
		//_audio->outputFrameSize = _audio->stream->codec->frame_size * 
		//	av_get_bytes_per_sample(_audio->stream->codec->sample_fmt) * 
		//	_audio->stream->codec->channels;
		/*
		//_video->oframe->pts = _video->pts;

		// PTS value will increment by 1 for input each frame at defined FPS value.
		// PTS value will need to be dynamically generated for variable FPS rates.	
		_videoFPS.tick();
		//if (_video->oframe->pts == AV_NOPTS_VALUE)
		//	_video->oframe->pts = 0;
		//else {
			double fpsDiff = (_video->codec->time_base.den / _videoFPS.fps);
			_video->pts = _video->pts + fpsDiff;
			//_video->dts = _video->pts + fpsDiff;
			//_video->oframe->pts = _video->pts;
		//}
		*/

		/*

			// Set the encoder codec
			if (_options.oformat.video.enabled)
				_formatCtx->oformat->video_codec = static_cast<CodecID>(_options.oformat.video.id);
			if (_options.oformat.audio.enabled)
				_formatCtx->oformat->audio_codec = static_cast<CodecID>(_options.oformat.audio.id);	

		for (int i=0; i< _formatCtx->nb_streams; i++) {
			if (_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && 
				_video == NULL) {
				_video = new VideoDecoderContext();
				_video->open(_formatCtx, i);
			}
			else if (_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO &&
				_audio == NULL) {
				_audio = new AudioDecoderContext();
				_audio->open(_formatCtx, i);
			}
		}
		if (_video == NULL && 
			_audio == NULL)
			throw Exception("Could not find a valid media stream");
		*/


		
		/*		 	
		Log("debug") << "[AVEncoder" << this << "] _frameDuration: " << _frameDuration << endl;

		Log("debug") << "[AVEncoder" << this << "] frameDuration: " << frameDuration << endl;
		unsigned frameDuration = 
		_frameDuration = clock();	
		// PTS value will increment by 1 for input each frame at defined FPS value.
		// PTS value will need to be dynamically generated for variable FPS rates.
		_videoFPS.tick();
		//if (_videoOutFrame->pts == AV_NOPTS_VALUE)
		//	_videoOutFrame->pts = 0;
		//else {
			Log("debug") << "[AVEncoder" << this << "] Last PTS: " << _videoPTS << endl;
			Log("debug") << "[AVEncoder" << this << "] Current FPS: " << _videoFPS.fps << endl;
			Log("debug") << "[AVEncoder" << this << "] FPS Den: " << _video->ctx->time_base.den << endl;
			Log("debug") << "[AVEncoder" << this << "] FPS Den: " << ( _video->ctx->time_base.den * 1.0) << endl;
			Log("debug") << "[AVEncoder" << this << "] FPS Den: " << ((_video->ctx->time_base.den * 1.0) / _videoFPS.fps) << endl;
			double fpsDiff = _videoFPS.fps > 0 ? ((_video->ctx->time_base.den * 1000.0) / _videoFPS.fps) : 0;
			//_videoPTS = _videoPTS + fpsDiff;
			//_videoPTS += _video->stream->codec->time_base.den/(_video->stream->codec->time_base.num/(_videoFPS.duration/1000.0));
			opacket.pts = _videoPTS;
			//_videoOutFrame->pts = _videoPTS;
		
			Log("debug") << "[AVEncoder" << this << "] FPS Difference: " << fpsDiff << endl;
			Log("debug") << "[AVEncoder" << this << "] Calculated PTS: " << _videoPTS << endl;
			Log("debug") << "[AVEncoder" << this << "] FPS Difference 1: " << (1.0 / fpsDiff) << endl;
			Log("debug") << "[AVEncoder" << this << "] Using PTS: " << _videoOutFrame->pts << endl;
		}
		*/

		/*
		_videoFPS.tick();
		LogTrace("AVEncoder", this) << "Writing Video Packet: " 
			<< opacket.pts << ": " 
			<< opacket.dts << ": " 
			<< _video->pts << ": "
			//<< ((1000.0 / _videoFPS.fps) * _videoFPS.frames)
			<< endl;
		
		//if (opacket.pts == AV_NOPTS_VALUE) 
		//opacket.pts = (_video->codec->time_base.den / _videoFPS.fps) * _videoFPS.frames;		
		//_videoPTS += ctx->time_base.den/(ctx->time_base.num/(/durationInMS/50/1000.0));
		//pts += 1000/(1/(/durationInMS/100/1000.0)); //ctx->time_base.den/(ctx->time_base.num/(/durationInMS/100/1000.0));
		//	if (_videoStream->codec->coded_frame->pts != AV_NOPTS_VALUE)
		//		packet.pts = av_rescale_q(_videoStream->codec->coded_frame->pts, _videoStream->codec->time_base, _videoStream->time_base);
		//	_videoPTS += _videoStream->codec->time_base.den/(_videoStream->codec->time_base.num/(durationInMS/1000.0));
		*/