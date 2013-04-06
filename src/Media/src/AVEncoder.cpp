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


#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Media/FLVMetadataInjector.h"

#include "assert.h"

#if WIN32
#define snprintf _snprintf
#endif


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


FastMutex AVEncoder::_mutex;


AVEncoder::AVEncoder(const RecorderOptions& options) :
	_options(options),
	_outIOBuffer(NULL),
	_outIOBufferSize(MAX_VIDEO_PACKET_SIZE),
	_formatCtx(NULL),
	_startTime(0),
	_video(NULL),
	_videoPTS(0),
	_videoLastPTS(0),
	_videoTime(0),
	_videoLastTime(0),
	_audio(NULL),
	_audioFifo(NULL),
	_audioFifoOutBuffer(NULL)
{
}


AVEncoder::AVEncoder() : 
	_outIOBuffer(NULL),
	_outIOBufferSize(MAX_VIDEO_PACKET_SIZE),
	_formatCtx(NULL),
	_startTime(0),
	_video(NULL),
	_videoPTS(0),
	_videoLastPTS(0),
	_videoTime(0),
	_videoLastTime(0),
	_audio(NULL),
	_audioFifo(NULL),
	_audioFifoOutBuffer(NULL)
{
	LogDebug("AVEncoder", this) << "Initializing" << endl;
}


AVEncoder::~AVEncoder()
{
	LogDebug("AVEncoder", this) << "Destroying" << endl;
	
	//setState(this, EncoderState::Stopped);
	//cleanup();
	uninitialize();
}


int DispatchOutputPacket(void* opaque, UInt8* buffer, int bufferSize)
{
	// Callback example at: http://lists.mplayerhq.hu/pipermail/libav-client/2009-May/003034.html
	AVEncoder* klass = reinterpret_cast<AVEncoder*>(opaque);
	if (klass) {
		//LogTrace("AVEncoder", klass) << "Dispatching Packet: " << bufferSize << endl;
		MediaPacket packet(buffer, bufferSize);
		klass->dispatch(klass, packet);
	}   

    return bufferSize;
}


void AVEncoder::initialize() 
{
	assert(!isActive());

	LogDebug("AVEncoder", this) << "Starting:"
		<< "\n\tPID: " << this
		<< "\n\tFormat: " << _options.oformat.label
		<< "\n\tVideo In: " << _options.oformat.video.toString()
		<< "\n\tVideo Out: " << _options.oformat.video.toString()
		<< "\n\tAudio In: " << _options.oformat.audio.toString()
		<< "\n\tAudio Out: " << _options.oformat.audio.toString()
		<< "\n\tDuration: " << _options.duration
		<< endl;

	try {
		{
			// Lock our mutex during initialization
			FastMutex::ScopedLock lock(_mutex);

			if (!_options.oformat.video.enabled && 
				!_options.oformat.audio.enabled)
				throw Exception("Either video or audio parameters must be specified.");

			if (!_options.oformat.id)
				throw Exception("An output container format must be specified.");		

			av_register_all(); 

			// Allocate the output media context
			_formatCtx = avformat_alloc_context();
			if (!_formatCtx) 
				throw Exception("Cannot allocate format context.");

			if (!_options.ofile.empty())
				snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _options.ofile.data());
		
			// Set the container codec
			string ofmt = _options.ofile.empty() ? ("." + string(_options.oformat.id)) : _options.ofile;		
			_formatCtx->oformat = av_guess_format(_options.oformat.id, ofmt.data(), NULL);	
			if (!_formatCtx->oformat)
				throw Exception("Cannot find suitable encoding format for " + _options.oformat.label);
		}

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
				_outIOBuffer = new unsigned char[_outIOBufferSize];
				_outIO = avio_alloc_context(_outIOBuffer, _outIOBufferSize, 0, this, 0, DispatchOutputPacket, 0); //_outIO, 
				//_outIO->is_streamed = 1;
				_formatCtx->pb = _outIO;
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
		}

		setState(this, EncoderState::Ready);
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
	LogDebug("AVEncoder", this) << "Stopping" << endl;
	
	//setState(this, EncoderState::None);

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
	
		if (_outIOBuffer) {
			delete _outIOBuffer;
			_outIOBuffer = NULL;
		}
	}

	LogDebug("AVEncoder", this) << "Cleanup: OK" << endl;
}


void AVEncoder::process(IPacket& packet)
{	
	if (!isActive()) {
		LogDebug("AVEncoder", this) << "Encoder not ready: Dropping Packet: " << packet.className() << endl;
		return;
	}	

	//LogTrace("AVEncoder", this) << "Processing Packet: " << &packet << ": " << packet.className() << endl;

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

	dispatch(this, packet);

	LogError("AVEncoder", this) << "Failed to encode packet" << endl;
}

					
void AVEncoder::onStreamStateChange(const PacketStreamState& state) 
{ 
	LogDebug("AVEncoder", this) << "Stream State Changed: " << state << endl;

	switch (state.id()) {
	case PacketStreamState::Running:
		if (stateEquals(EncoderState::None))
			initialize();
		break;
		
	case PacketStreamState::Stopped:
	case PacketStreamState::Failed:
	case PacketStreamState::Resetting:
		if (stateEquals(EncoderState::Encoding))
			uninitialize();
		break;
	//case PacketStreamState::None:
	//case PacketStreamState::Closing:
	//case PacketStreamState::Closed:
	}

	PacketDispatcher::onStreamStateChange(state);
}


RecorderOptions& AVEncoder::options()
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
	LogTrace("AVEncoder", this) << "Creating Video" << endl;
	assert(!_video);

	// Initialize the video encoder (if required)
	if (_options.oformat.video.enabled && _formatCtx->oformat->video_codec != CODEC_ID_NONE) {
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
	//LogTrace("AVEncoder", this) << "Encoding Video Packet: " << bufferSize << endl;

	// Lock the mutex while encoding
	FastMutex::ScopedLock lock(_mutex);
	
	assert(_video);
	assert(_video->frame);

	if (!isActive())
		throw Exception("The encoder is not initialized.");
	
	if (!_video) 
		throw Exception("No video context");

	if (!buffer || !bufferSize || !width || !height)
		throw Exception("Failed to encode video frame.");
	
	// Recreate the conversion context if the
	// input resolution changes.
	if (_options.iformat.video.width != width || 
		_options.iformat.video.height != height) {			
		_options.iformat.video.width = width;
		_options.iformat.video.height = height;
		_video->iparams.width = width;
		_video->iparams.height = height;
		LogDebug("AVEncoder", this) << "Recreating video conversion context" << endl;
		_video->freeConverter();
		_video->createConverter();
	}

	AVPacket opacket;
	if (_formatCtx->oformat->flags & AVFMT_RAWPICTURE) {
		opacket.flags |= AV_PKT_FLAG_KEY;
		opacket.stream_index = _video->stream->index;
		opacket.data = (UInt8*)buffer;
		opacket.size = sizeof(AVPicture);
	} 
	else {
		
		/*
		//_video->oframe->pts = _video->pts;

		// PTS value will increment by 1 for input each frame at defined FPS value.
		// PTS value will need to be dynamically generated for variable FPS rates.	
		_fpsCounter.tick();
		//if (_video->oframe->pts == AV_NOPTS_VALUE)
		//	_video->oframe->pts = 0;
		//else {
			double fpsDiff = (_video->codec->time_base.den / _fpsCounter.fps);
			_video->pts = _video->pts + fpsDiff;
			//_video->dts = _video->pts + fpsDiff;
			//_video->oframe->pts = _video->pts;
		//}
		*/
		
 		// Encode the frame 
		if (!_video->encode(buffer, bufferSize, opacket)) {
			LogWarn("AVEncoder", this) << "Failed to encode video frame" << endl;
			return false;
		}
	}

	if (opacket.size > 0) {
		 
		/*
		LogTrace("AVEncoder", this) << "Writing Video Packet: " 
			<< opacket.pts << ": " 
			<< opacket.dts << ": " 
			//<< _video->pts << ": "
			//<< ((1000.0 / _fpsCounter.fps) * _fpsCounter.frames)
			<< endl;
		
		if (opacket.pts == AV_NOPTS_VALUE) 
			opacket.pts = (1000.0 / _fpsCounter.fps) * _fpsCounter.frames;
			*/
	
		// Write the encoded frame to the output file / stream.
		if (av_interleaved_write_frame(_formatCtx, &opacket) < 0) {
			LogError("AVEncoder", this) << "Failed to write video frame" << endl;
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

	// Initialize the audio encoder (if required)
	if (_options.oformat.audio.enabled && _formatCtx->oformat->audio_codec != CODEC_ID_NONE) {
		_audio = new AudioEncoderContext(_formatCtx);
		_audio->iparams = _options.iformat.audio;
		_audio->oparams = _options.oformat.audio;
		_audio->create();
		_audio->open();			

		// Set the output frame size for encoding
		//_audio->outputFrameSize = _audio->stream->codec->frame_size * 
		//	av_get_bytes_per_sample(_audio->stream->codec->sample_fmt) * 
		//	_audio->stream->codec->channels;

		// The encoder may require a minimum number of raw audio samples for each encoding but we can't
		// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
		// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
		_audioFifo = av_fifo_alloc(2 * MAX_AUDIO_PACKET_SIZE);

		// Allocate a buffer to read OUT of the FIFO into. 
		// The FIFO maintains its own buffer internally.
		_audioFifoOutBuffer = (UInt8*)av_malloc(2 * MAX_AUDIO_PACKET_SIZE);
	}
}


void AVEncoder::freeAudio()
{
	FastMutex::ScopedLock lock(_mutex);	

	if (_audio) {
		delete _audio;
		_audio = NULL;
	}
}


bool AVEncoder::encodeAudio(unsigned char* buffer, int bufferSize)
{
	/*
	LogTrace("AVEncoder", this) << "Encoding Audio Packet:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << _audio->bufferSize << "\n"
		//<< "Duration: " << frameDuration << "\n" 
		<< endl;
	*/

	// Lock the mutex while encoding
	FastMutex::ScopedLock lock(_mutex);	

	/*
	// If we are encoding a multiplex stream wait for the first
	// video frame to be encoded before we start encoding audio
	// otherwise we get errors for some codecs.
	if (_video && _fpsCounter.frames == 0) {
		LogTrace("AVEncoder", this) << "Encoding Audio Packet: Dropping audio frames until we have video." << endl;
		return false;
	}
	*/
	
	assert(buffer);
	assert(bufferSize);

	if (!isActive())
		throw Exception("The encoder is not initialized");
	
	if (_audio) 
		throw Exception("No audio context");
	
	if (!buffer || !bufferSize) 
		throw Exception("Invalid audio input");
	
	// TODO: Does latest FFmpeg still require use of fifo?
	av_fifo_generic_write(_audioFifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audioFifo) >= _audio->outputFrameSize) {
		av_fifo_generic_read(_audioFifo, _audioFifoOutBuffer, _audio->outputFrameSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		AVPacket opacket;	

		//int size = _audio->encode(_audioFifoOutBuffer, _audio->outputFrameSize, opacket);
		if (_audio->encode(_audioFifoOutBuffer, _audio->outputFrameSize, opacket)) {

	 		// Write the encoded frame to the output file
			if (av_interleaved_write_frame(_formatCtx, &opacket) != 0) {
				LogError("AVEncoder", this) << "Failed to write audio frame" << endl;
				return false;
			}
		} 
		else {
			// Encoded video frame is empty, it may have been buffered.
			return false;
		}
	}
	
	return true;
}


} } // namespace Sourcey::Media




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