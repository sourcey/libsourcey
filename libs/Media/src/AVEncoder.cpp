//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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

#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


FastMutex AVEncoder::_mutex;


AVEncoder::AVEncoder(const RecorderParams& params) :
	_params(params),
	_outIOBuffer(NULL),
	_outIOBufferSize(MAX_VIDEO_PACKET_SIZE),
	_formatCtx(NULL),
	_video(NULL),
	_audio(NULL)
{
}


AVEncoder::AVEncoder() : 
	_outIOBuffer(NULL),
	_outIOBufferSize(MAX_VIDEO_PACKET_SIZE),
	_formatCtx(NULL),
	_video(NULL),
	_audio(NULL)
{
	Log("debug") << "[AVEncoder" << this << "] Initializing" << endl;
}


AVEncoder::~AVEncoder()
{
	Log("debug") << "[AVEncoder" << this << "] Destroying" << endl;
	
	setState(this, EncoderState::Closing);
	cleanup();
}


int DispatchOutputPacket(void* opaque, uint8_t* buffer, int bufferSize)
{
	// Callback example at: http://lists.mplayerhq.hu/pipermail/libav-client/2009-May/003034.html
	AVEncoder* klass = reinterpret_cast<AVEncoder*>(opaque);
	if (klass) {
		Log("trace") << "[AVEncoder" << klass << "] Dispatching Packet: " << bufferSize << endl;

		//MediaPacket(buffer, bufferSize);
		//packet.data = buffer;
		//packet.time = time(0); // Set to pts?
		//packet.size = bufferSize;
		//klass->PacketEncoded.dispatch(klass, packet);
		MediaPacket packet(buffer, bufferSize);
		klass->dispatch(klass, packet);
	}   

    return bufferSize;
}


void AVEncoder::initialize() 
{
	assert(!isReady());

	Log("debug") << "[AVEncoder" << this << "] Starting:"
		<< "\n\tPID: " << this
		<< "\n\tFormat: " << _params.oformat.label
		<< "\n\tStarting At: " << time(0)
		<< "\n\tVideo: " << _params.oformat.video.toString()
		<< "\n\tAudio: " << _params.oformat.audio.toString()
		<< "\n\tStopping At: " << _params.stopAt
		<< "\n\tDuration: " << _params.stopAt-time(0) 
		<< endl;
	
	//Log("debug") << "[AVEncoder" << this << "] Input Format:" << endl;
	//_params.iformat.print(cout);

	//Log("debug") << "[AVEncoder" << this << "] Output Format:" << endl;
	//_params.oformat.print(cout);

	try {
		// Lock our mutex during initialization
		FastMutex::ScopedLock lock(_mutex);

		if (!_params.oformat.video.enabled && !_params.oformat.audio.enabled)
			throw Exception("Either video or audio parameters must be specified");

		if (!_params.oformat.id)
			throw Exception("An output container format must be specified");		

		av_register_all(); 

		// Allocate the output media context
		_formatCtx = avformat_alloc_context();
		if (!_formatCtx) 
			throw Exception("Unable to allocate format context");

		if (!_params.ofile.empty())
			snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _params.ofile.data());
		
		// Set the container codec
		//string 
		_formatCtx->oformat = av_guess_format(
			NULL, 
			_params.ofile.empty() ? 
				(string("") + _params.oformat.name()).data() : 
				_params.ofile.data(), NULL);	
			//_params.ofile.empty() ? NULL : _params.ofile.data(), NULL);	
		if (!_formatCtx->oformat)
			throw Exception("Unable to find suitable output format");

		// Set the encoder codec
		if (_params.oformat.video.enabled)
			_formatCtx->oformat->video_codec = static_cast<CodecID>(_params.oformat.video.id);
		if (_params.oformat.audio.enabled)
			_formatCtx->oformat->audio_codec = static_cast<CodecID>(_params.oformat.audio.id);

		// Set the output parameters
		if (av_set_parameters(_formatCtx, NULL) < 0)
			throw Exception("Invalid output format parameters");		
		
		/*
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

		// Initialize the video encoder (if required)
		if (_params.oformat.video.enabled && _formatCtx->oformat->video_codec != CODEC_ID_NONE) {
			_video = new VideoEncoderContext();
			_video->iparams = _params.iformat.video;
			_video->oparams = _params.oformat.video;
			_video->open(_formatCtx);
		}

		// Initialize the audio encoder (if required)
		if (_params.oformat.audio.enabled && _formatCtx->oformat->audio_codec != CODEC_ID_NONE) {
			_audio = new AudioEncoderContext();
			_audio->iparams = _params.iformat.audio;
			_audio->oparams = _params.oformat.audio;
			_audio->open(_formatCtx);
		}

		if (_params.ofile.empty()) {

			// Operating in streaming mode. Generated packets can be
			// obtained by connecting to the PacketEncoded signal.
			// Setup the output IO context for our output stream.
			_outIO = new ByteIOContext();
			_outIOBuffer = new unsigned char[_outIOBufferSize];
			init_put_byte(_outIO, _outIOBuffer, _outIOBufferSize, 0, this, 0, DispatchOutputPacket, 0);
			_formatCtx->pb = _outIO;
		}
		else {

			// Operating in file mode.  
			// Open the output file...
			if (!(_formatCtx->oformat->flags & AVFMT_NOFILE)) {
				if (url_fopen(&_formatCtx->pb, _params.ofile.data(), URL_WRONLY) < 0) {
					throw Exception("AVWriter: Unable to open the output file");
				}
			}
		}

		// Write the stream header (if any)
		av_write_header(_formatCtx);

		// Send the format information to sdout
		dump_format(_formatCtx, 0, _params.ofile.data(), 1);

		setState(this, EncoderState::Ready);
	} 
	catch (Exception& exc) {
		Log("error") << "[AVEncoder" << this << "] Error: " << exc.displayText() << endl;
		
		cleanup();
		setState(this, EncoderState::Failed, exc.displayText());
		exc.rethrow();
	}
}


void AVEncoder::uninitialize()
{
	Log("debug") << "[AVEncoder" << this << "] Stopping" << endl;
	
	setState(this, EncoderState::None);

	cleanup();
}


void AVEncoder::cleanup()
{
	Log("debug") << "[AVEncoder" << this << "] Cleanup" << endl;

 	// Lock our mutex during closure
	FastMutex::ScopedLock lock(_mutex);	

 	// Write the trailer
    if (_formatCtx &&
		_formatCtx->pb) 
		av_write_trailer(_formatCtx);

    // Close each codec 
	if (_video) {
		delete _video;
		_video = NULL;
	}
	if (_video) {
		delete _video;
		_video = NULL;
	}
	
    // Close the format
	if (_formatCtx) 
	{
	 	// Free the streams
		for (unsigned int i = 0; i < _formatCtx->nb_streams; i++) {
			av_freep(&_formatCtx->streams[i]->codec);
			av_freep(&_formatCtx->streams[i]);
		}

	 	// Close the output file (if any)
		if (!_params.ofile.empty() &&
			_formatCtx->pb &&  
			_formatCtx->oformat && !(_formatCtx->oformat->flags & AVFMT_NOFILE))
			url_fclose(_formatCtx->pb);
		
	 	// Free the format context
		av_free(_formatCtx);
		_formatCtx = NULL;
	}
	
	if (_outIOBuffer) {
		delete _outIOBuffer;
		_outIOBuffer = NULL;
	}

	/*
 	// Write the trailer
    if (_formatCtx &&
		_formatCtx->pb) 
		av_write_trailer(_formatCtx);

    // Close each codec 
    if (_video->stream) closeVideo();	
    if (_audio->stream) closeAudio();
	if (_formatCtx) 
	{
	 	// Free the streams
		for (unsigned int i = 0; i < _formatCtx->nb_streams; i++) {
			av_freep(&_formatCtx->streams[i]->codec);
			av_freep(&_formatCtx->streams[i]);
		}

	 	// Close the output file (if any)
		if (!_params.ofile.empty() &&
			_formatCtx->pb &&  
			_formatCtx->oformat && !(_formatCtx->oformat->flags & AVFMT_NOFILE))
			url_fclose(_formatCtx->pb);
		
	 	// Free the format context
		av_free(_formatCtx);
		_formatCtx = NULL;
	}
	
	if (_outIOBuffer) {
		delete _outIOBuffer;
		_outIOBuffer = NULL;
	}

	// Small memory leak here, but pointer appears freed?
	//if (_outIO) {
	//	delete _outIO;
		_outIO = NULL;
	//}	

	_video->PTS = 0;
	_video->LastPTS = -1;
	_video->Time = 0;
	_audio->Time = 0;
	*/

	Log("debug") << "[AVEncoder" << this << "] Cleanup: OK" << endl;
}


void AVEncoder::process(IPacket& packet)
{	
	if (!isReady()) {
		Log("debug") << "[AVEncoder" << this << "] Encoder not ready: Dropping Packet: " << packet.className() << endl;
		return;
	}

	Log("trace") << "[AVEncoder" << this << "] Processing Packet: " << packet.className() << endl;

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

	Log("error") << "[AVEncoder" << this << "] Failed to encode packet" << endl;
}

					
void AVEncoder::onStreamStateChange(const PacketStreamState& state) 
{ 
	Log("debug") << "[AVEncoder" << this << "] Stream State Changed: " << state << endl;

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



// -------------------------------------------------------------------
//
// Video Stuff
//
// -------------------------------------------------------------------
bool AVEncoder::encodeVideo(unsigned char* buffer, int bufferSize, int width, int height)
{
	//Log("trace") << "[AVEncoder" << this << "] Encoding Video Packet: " << bufferSize << endl;

	// Lock the mutex while encoding
	FastMutex::ScopedLock lock(_mutex);
	
	assert(_video->iframe);
	assert(_video->oframe);

	if (!isReady())
		throw Exception("The encoder is not initialized");

	if (_params.iformat.video.width != width || 
		_params.iformat.video.height != height)
		throw Exception("The input video frame is the wrong size");

	if (!buffer || !bufferSize || !width || !height)
		throw Exception("Failed to encode video frame");

	/*
	AVStream* stream = _video->stream;
	AVCodecContext* codec = stream->codec;

	// This is a hack to speed the things up a bit. We just assign the
	// pointer buffer to _video->iframe->data[0] without using a memcpy.
	_video->iframe->data[0] = (UInt8*)buffer;
	//memcpy(_video->iframe->data[0], buffer, bufferSize);

	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (_video->ConvCtx == NULL) {
		_video->ConvCtx = sws_getContext(
			width, height, (::PixelFormat)_params.iformat.video.pixfmt, //static_cast<::PixelFormat>()
			codec->width, codec->height, codec->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		Log("trace") << "[AVEncoder" << this << "] Initializing Video Conversion Context:\n" 
			<< "\n\tInput Width: " << width
			<< "\n\tInput Height: " << height
			<< "\n\tInput Pixel Format: " << _params.iformat.video.pixfmt
			<< "\n\tOutput Width: " << codec->width
			<< "\n\tOutput Height: " << codec->height
			<< "\n\tOutput Pixel Format: " << codec->pix_fmt
			<< endl;
	}
	
	if (sws_scale(_video->ConvCtx,
		_video->iframe->data, _video->iframe->linesize, 0, height,
		_video->oframe->data, _video->oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported");
	*/

	AVPacket opacket;
	if (_formatCtx->oformat->flags & AVFMT_RAWPICTURE) {
		av_init_packet(&opacket);
		opacket.flags |= AV_PKT_FLAG_KEY;
		opacket.stream_index = _video->stream->index;
		opacket.data = (UInt8*)_video->oframe;
		opacket.size = sizeof(AVPicture);
	} 
	else {
		// PTS value will increment by 1 for input each frame at defined FPS value.
		// PTS value will need to be dynamically generated for variable FPS rates.
		_fpsCounter.tick();
		if (_video->oframe->pts == AV_NOPTS_VALUE)
			_video->oframe->pts = 0;
		else {
			double fpsDiff = (_video->codec->time_base.den / _fpsCounter.fps);
			_video->pts = _video->pts + fpsDiff;
			_video->oframe->pts = _video->pts;
		}
		
 		// Encode the frame 
		int size = _video->encode(buffer, bufferSize, opacket);	
		if (size < 0) 
			Log("warn") << "[AVEncoder" << this << "] Failed to encode video frame" << endl;
	}

	Log("trace") << "[AVEncoder" << this << "] Frame Information:\n" 
		<< "\n\tCalculated PTS: " << _video->oframe->pts
		<< "\n\tFFmpeg PTS: " << opacket.pts
		<< "\n\tCurrent FPS: " << _fpsCounter.fps
		<< "\n\tPacket Size: " << opacket.size
		<< endl;

	if (opacket.size > 0) {
	
		// Write the encoded frame to the output file / stream.
		//int res = av_write_frame(_formatCtx, &packet);
		int res = av_interleaved_write_frame(_formatCtx, &opacket);
		if (res < 0) {
			Log("error") << "[AVEncoder" << this << "] Failed to write video frame" << endl;
			return false;
		}
	}

	return true;
}


// -------------------------------------------------------------------
//
// Audio Stuff
//
// -------------------------------------------------------------------
bool AVEncoder::encodeAudio(unsigned char* buffer, int bufferSize)
{
	Log("trace") << "[AVEncoder" << this << "] Encoding Audio Packet" << endl;

	// Lock the mutex while encoding
	FastMutex::ScopedLock lock(_mutex);	
	
	if (!isReady())
		throw Exception("The encoder is not initialized");

	assert(buffer);
	assert(bufferSize);
	assert(_audio->stream);
	
	Log("debug") << "[AVEncoder" << this << "] Input audio frame:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << _video->bufferSize << "\n"
		//<< "Duration: " << frameDuration << "\n" 
		<< endl;
	
	if (!buffer || !bufferSize) 
		throw Exception("Invalid audio input");

	if (bufferSize > _video->bufferSize) 
		throw Exception("Audio frame too big");
	
 	// Encode a frame of AudioOutSize bytes
	AVPacket opacket;
	int size = _audio->encode(buffer, bufferSize, opacket);
	if (size) {

	 	// Write the encoded frame to the output file
		int result = av_interleaved_write_frame(_formatCtx, &opacket);
		if (result != 0) {
			Log("error") << "[AVEncoder" << this << "] Failed to write audio frame" << endl;
			return false;
		}
	} 
	else {
		Log("warn") << "[AVEncoder" << this << "] Encoded video frame size is NULL; it may have been buffered" << endl;
		return false;
	}

	return true;

	/*
	
	Log("trace") << "[AVEncoder" << this << "] Audio Packet: " << size << endl;


	av_fifo_generic_write(_audio->Fifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audio->Fifo) >= _audio->FrameSize) 
	{
		av_fifo_generic_read(_audio->Fifo, _audio->FifoOutBuffer, _audio->FrameSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		int size = avcodec_encode_audio->(_audio->stream->codec, _audio->Buffer, _audio->FrameSize, (short*)_audio->FifoOutBuffer);
		if (size) {
			AVPacket packet;
			av_init_packet(&packet);
			if (_audio->stream->codec->coded_frame && 
				_audio->stream->codec->coded_frame->pts != AV_NOPTS_VALUE) 
				packet.pts = av_rescale_q(_audio->stream->codec->coded_frame->pts, _audio->stream->codec->time_base, _audio->stream->time_base);
			
			if (_audio->stream->codec->coded_frame &&
				_audio->stream->codec->coded_frame->key_frame) 
                packet.flags |= AV_PKT_FLAG_KEY;

				Log("debug") << "[AVEncoder" << this << "] Encoded audio frame:" 
					<< "\n\tFrame Size: " << size << "\n"
					<< "\n\tCoded Frame: " << _audio->stream->codec->coded_frame << "\n"
					<< "\n\tKey Frame: " << (packet.flags & AV_PKT_FLAG_KEY) << "\n"
					<< endl;
            //if (codec->coded_frame->key_frame)
            //    packet.flags |= PKT_FLAG_KEY;
			//}
			//packet.flags |= AV_PKT_FLAG_KEY;
			packet.stream_index = _audio->stream->index;
			packet.data = _audio->Buffer;
			packet.size = size;

	 		// Write the encoded frame to the output file
			int result = av_interleaved_write_frame(_formatCtx, &packet);
			if (result != 0) {
				Log("error") << "[AVEncoder" << this << "] Failed to write audio frame" << endl;
				return false;
			}
		} 
		else {
			Log("warn") << "[AVEncoder" << this << "] Encoded video frame size is NULL; it may have been buffered" << endl;
			return false;
		}
	}

	return _audio->encode(buffer, bufferSize) > 0;;
		*/
}


} } // namespace Sourcey::Media
