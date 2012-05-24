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


AVEncoder::AVEncoder(const RecorderParams& params) :
	_params(params),
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
	_audioOutSize(0),
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
	_audioOutSize(0),
	_audioFifo(NULL),
	_audioFifoOutBuffer(NULL)
{
	Log("debug") << "[AVEncoder:" << this << "] Initializing" << endl;
}


AVEncoder::~AVEncoder()
{
	Log("debug") << "[AVEncoder:" << this << "] Destroying" << endl;
	
	//setState(this, EncoderState::Stopped);
	//cleanup();
	uninitialize();
}


int DispatchOutputPacket(void* opaque, UInt8* buffer, int bufferSize)
{
	// Callback example at: http://lists.mplayerhq.hu/pipermail/libav-client/2009-May/003034.html
	AVEncoder* klass = reinterpret_cast<AVEncoder*>(opaque);
	if (klass) {
		//Log("trace") << "[AVEncoder::" << klass << "] Dispatching Packet Size: " << bufferSize << endl;

		klass->_file.write((const char *)buffer, bufferSize);

		MediaPacket packet(buffer, bufferSize);
		klass->dispatch(klass, packet);
	}   

    return bufferSize;
}


void AVEncoder::initialize() 
{
	assert(!isReady());

	Log("debug") << "[AVEncoder:" << this << "] Starting:"
		<< "\n\tPID: " << this
		<< "\n\tFormat: " << _params.oformat.label
		<< "\n\tStarting At: " << time(0)
		<< "\n\tVideo: " << _params.oformat.video.toString()
		<< "\n\tAudio: " << _params.oformat.audio.toString()
		<< "\n\tStopping At: " << _params.stopAt
		<< "\n\tDuration: " << _params.stopAt-time(0) 
		<< endl;
	
	//Log("debug") << "[AVEncoder:" << this << "] Input Format:" << endl;
	//_params.iformat.print(cout);

	//Log("debug") << "[AVEncoder:" << this << "] Output Format:" << endl;
	//_params.oformat.print(cout);

	try {
		// Lock our mutex during initialization
		FastMutex::ScopedLock lock(_mutex);

		if (!_params.oformat.video.enabled && 
			!_params.oformat.audio.enabled)
			throw Exception("Either video or audio parameters must be specified.");

		if (!_params.oformat.id)
			throw Exception("An output container format must be specified.");		

		av_register_all(); 

		// Allocate the output media context
		_formatCtx = avformat_alloc_context();
		if (!_formatCtx) 
			throw Exception("Cannot allocate format context.");

		if (!_params.ofile.empty())
			snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _params.ofile.data());
		
		// Set the container codec
		string ofmt = _params.ofile.empty() ? "." + 
			_params.oformat.extension() : 
			_params.ofile;		
		_formatCtx->oformat = av_guess_format(/*_params.oformat.encoderName().data()*/NULL, ofmt.data(), NULL);	
		if (!_formatCtx->oformat)
			throw Exception("Cannot find suitable output format for " + _params.oformat.encoderName());

		// Set the encoder codec
		if (_params.oformat.video.enabled)
			_formatCtx->oformat->video_codec = static_cast<CodecID>(_params.oformat.video.id);
		if (_params.oformat.audio.enabled)
			_formatCtx->oformat->audio_codec = static_cast<CodecID>(_params.oformat.audio.id);		
		
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

			// Set the output frame size for encoding
			_audioOutSize = _audio->stream->codec->frame_size * 2 * _audio->stream->codec->channels;

			// The encoder may require a minimum number of raw audio samples for each encoding but we can't
			// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
			// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
			_audioFifo = av_fifo_alloc(2 * MAX_AUDIO_PACKET_SIZE);

			// Allocate a buffer to read OUT of the FIFO into. 
			// The FIFO maintains its own buffer internally.
			_audioFifoOutBuffer = (UInt8*)av_malloc(2 * MAX_AUDIO_PACKET_SIZE);
		}

		if (_params.ofile.empty()) {

			// Operating in streaming mode. Generated packets can be
			// obtained by connecting to the PacketEncoded signal.
			// Setup the output IO context for our output stream.
			_outIOBuffer = new unsigned char[_outIOBufferSize];
			_outIO = avio_alloc_context(_outIOBuffer, _outIOBufferSize, 0, this, 0, DispatchOutputPacket, 0); //_outIO, 
			_formatCtx->pb = _outIO;
		}
		else {

			// Operating in file mode.  
			// Open the output file...
			if (!(_formatCtx->oformat->flags & AVFMT_NOFILE)) {
				//if (url_fopen(&_formatCtx->pb, _params.ofile.data(), URL_WRONLY) < 0) {
				if (avio_open(&_formatCtx->pb, _params.ofile.data(), AVIO_FLAG_WRITE) < 0) {
					throw Exception("AVWriter: Unable to open the output file");
				}
			}
		}
		
		// Open the output file
		//_file.open("test.flv", ios::out | ios::binary);	

		// Write the stream header (if any)
		avformat_write_header(_formatCtx, NULL);

		// Send the format information to sdout
		av_dump_format(_formatCtx, 0, _params.ofile.data(), 1);

		setState(this, EncoderState::Ready);
	} 
	catch (Exception& exc) {
		Log("error") << "[AVEncoder:" << this << "] Error: " << exc.displayText() << endl;
		
		cleanup();
		setState(this, EncoderState::Error, exc.displayText());
		exc.rethrow();
	}
}


void AVEncoder::uninitialize()
{
	Log("debug") << "[AVEncoder:" << this << "] Stopping" << endl;
	
	//setState(this, EncoderState::None);

	cleanup();
	
	setState(this, EncoderState::Stopped);
}


void AVEncoder::cleanup()
{
	Log("debug") << "[AVEncoder:" << this << "] Cleanup" << endl;

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
	if (_formatCtx) {

	 	// Free the streams
		for (unsigned int i = 0; i < _formatCtx->nb_streams; i++) {
			av_freep(&_formatCtx->streams[i]->codec);
			av_freep(&_formatCtx->streams[i]);
		}

	 	// Close the output file (if any)
		if (!_params.ofile.empty() &&
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

	_file.close();

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

	Log("debug") << "[AVEncoder:" << this << "] Cleanup: OK" << endl;
}


void AVEncoder::process(IPacket& packet)
{	
	if (!isReady()) {
		Log("debug") << "[AVEncoder:" << this << "] Encoder not ready: Dropping Packet: " << packet.className() << endl;
		return;
	}	

	//Log("trace") << "[AVEncoder:" << this << "] Processing Packet: " << &packet << ": " << packet.className() << endl;

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

	Log("error") << "[AVEncoder:" << this << "] Failed to encode packet" << endl;
}

					
void AVEncoder::onStreamStateChange(const PacketStreamState& state) 
{ 
	Log("debug") << "[AVEncoder:" << this << "] Stream State Changed: " << state << endl;

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
	//Log("trace") << "[AVEncoder:" << this << "] Encoding Video Packet: " << bufferSize << endl;

	//if (_fpsCounter.frames > 0)
	//	return true;

	// Lock the mutex while encoding
	//FastMutex::ScopedLock lock(_mutex);
	
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

		Log("trace") << "[AVEncoder:" << this << "] Initializing Video Conversion Context:\n" 
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

		/*
		//double			_videoPTS;
		//Int64			_videoLastPTS;
		//clock_t			_videoTime;
		//_videoTime = clock();		
		//unsigned pts = 0;
		unsigned pts = 0;
		if (_videoLastTime == 0) {
			_videoLastTime = clock();
			_videoTime = _videoLastTime;
			_videoPTS = 0;
			//pts = _videoTime - _videoLastTime;
		}
		else {
			_videoTime = clock();
			_videoPTS += _videoTime - _videoLastTime;
			_videoLastTime = _videoTime;
		}

		pts = _videoPTS; //_videoTime - _videoLastTime;
		*/

		
		//_videoPTS;
		//Int64
		//_videoTime = clock();
		//_videoLastTime = _videoTime; //clock();
		
		//_video->codec->coded_frame->pts = (double)_video->stream->pts.val * _video->stream->codec->time_base.num / _video->stream->codec->time_base.den;
		//_video->oframe->pts = (double)_video->stream->pts.val * _video->stream->codec->time_base.num / _video->stream->codec->time_base.den;

		//unsigned pts = (double)_video->stream->pts.val * _video->stream->codec->time_base.num / _video->stream->codec->time_base.den;
		//unsigned pts = (double)_video->stream->pts.val * _video->stream->time_base.num / _video->stream->time_base.den;
		
 		// Encode the frame 
		int size = _video->encode(buffer, bufferSize, opacket);	//, pts
		if (size < 0) {
			Log("warn") << "[AVEncoder:" << this << "] Failed to encode video frame" << endl;
			return false;
		}
		
		/*
		opacket.pts = _video->pts;
		_fpsCounter.tick();

		//_video->codec->coded_frame->pts = (double)_video->stream->pts.val * _video->stream->codec->time_base.num / _video->stream->codec->time_base.den;
		//opacket.pts = _video->codec->coded_frame->pts; //pts;

		if (_startTime == 0)
			_startTime = clock();

		opacket.pts = clock() - _startTime;

		Log("trace") << "[AVEncoder:" << this << "] Video Frame Information:\n" 
			//<< "\n\tCalculated PTS: " << pts //_video->oframe->pts
			//<< "\n\tCalculated PTS 1: " << ((double)_video->stream->pts.val * _video->stream->codec->time_base.num / _video->stream->codec->time_base.den) //_video->oframe->pts
			<< "\n\tPacket PTS: " << opacket.pts
			<< "\n\tFrame PTS: " << _video->oframe->pts
			<< "\n\tCoded Frame PTS: " << _video->codec->coded_frame->pts
			<< "\n\tStream PTS: " << (double)_video->stream->pts.val
			//<< "\n\tCurrent FPS: " << _fpsCounter.fps
			//<< "\n\tPacket Size: " << opacket.size
			<< endl;
			*/

		//opacket.pts = _video->pts; 
		//_fpsCounter.frames; //
		//_fpsCounter.tick();
	}

	/*
		*/

	if (opacket.size > 0) {
	
		// Write the encoded frame to the output file / stream.
		//int res = av_write_frame(_formatCtx, &packet);
		int res = av_interleaved_write_frame(_formatCtx, &opacket);
		if (res < 0) {
			Log("error") << "[AVEncoder:" << this << "] Failed to write video frame" << endl;
			return false;
		}
	}

	return true;
}


//
// Audio Stuff
//
bool AVEncoder::encodeAudio(unsigned char* buffer, int bufferSize)
{
	/*
	Log("trace") << "[AVEncoder:" << this << "] Encoding Audio Packet:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << _audio->bufferSize << "\n"
		//<< "Duration: " << frameDuration << "\n" 
		<< endl;

	// Lock the mutex while encoding
	//FastMutex::ScopedLock lock(_mutex);	
		*/
	
	// If we are encoding a multiplex stream wait for the first
	// video frame to be encoded before we start encoding audio
	// otherwise we get errors for some codecs.
	if (_video && _fpsCounter.frames == 0) {
		Log("trace") << "[AVEncoder:" << this << "] Encoding Audio Packet: Dropping until we have video." << endl;
		return false;
	}

	if (!isReady())
		throw Exception("The encoder is not initialized");
	
	assert(buffer);
	assert(bufferSize);
	assert(_audio);
	assert(_audio->stream);
	
	if (!buffer || !bufferSize) 
		throw Exception("Invalid audio input");

	if (bufferSize > _audio->bufferSize) 
		throw Exception("Audio frame too big"); 
	
	av_fifo_generic_write(_audioFifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audioFifo) >= _audioOutSize) {
		av_fifo_generic_read(_audioFifo, _audioFifoOutBuffer, _audioOutSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		AVPacket opacket;
		//int size = _audio->encode(buffer, bufferSize, opacket);		
		int size = _audio->encode(_audioFifoOutBuffer, _audioOutSize, opacket);
		if (size) {
			
			/*
			if (_startTime == 0)
				_startTime = clock();
			
			opacket.pts = clock() - _startTime;

			//if (_video->oframe->pts == AV_NOPTS_VALUE)
			//	_video->oframe->pts = 0;
			//else {
			//double fpsDiff = (_video/_audio/->codec->time_base.den / _fpsCounter.fps);
			//_audio->pts = _audio->pts + fpsDiff;
			//}	
			//opacket.pts = _audio->pts; //0; //_fpsCounter.frames; //_video->pts;
			//opacket.pts = _video->pts; // + 1;
			//_fpsCounter.tick();
			//_audio->stream->pts.val; //
			//opacket.pts = (_video->oframe->pts / 2)  + 1;
			
			Log("trace") << "[AVEncoder:" << this << "] Audio Frame Information:\n" 
				<< "\n\tPacket PTS: " << opacket.pts
				<< "\n\tPacket Size: " << opacket.size
				<< "\n\tStream PTS: " << (double)_audio->stream->pts.val
				<< endl;			
			*/

	 		// Write the encoded frame to the output file
			int result = av_interleaved_write_frame(_formatCtx, &opacket);
			if (result != 0) {
				Log("error") << "[AVEncoder:" << this << "] Failed to write audio frame" << endl;
				return false;
			}
		} 
		else {
			Log("warn") << "[AVEncoder:" << this << "] Encoded video frame size is NULL; it may have been buffered" << endl;
			return false;
		}
	}
	
	return true;

	/*
 	// Encode a frame of AudioOutSize bytes
	AVPacket opacket;
	int size = _audio->encode(buffer, bufferSize, opacket);
	if (size) {

	 	// Write the encoded frame to the output file
		int result = av_interleaved_write_frame(_formatCtx, &opacket);
		if (result != 0) {
			Log("error") << "[AVEncoder:" << this << "] Failed to write audio frame" << endl;
			return false;
		}
	} 
	else {
		Log("warn") << "[AVEncoder:" << this << "] Encoded video frame size is NULL; it may have been buffered" << endl;
		return false;
	}
	
	return true;
	*/

	/*	
	
	Log("trace") << "[AVEncoder:" << this << "] Audio Packet: " << size << endl;


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

				Log("debug") << "[AVEncoder:" << this << "] Encoded audio frame:" 
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
				Log("error") << "[AVEncoder:" << this << "] Failed to write audio frame" << endl;
				return false;
			}
		} 
		else {
			Log("warn") << "[AVEncoder:" << this << "] Encoded video frame size is NULL; it may have been buffered" << endl;
			return false;
		}
	}

	return _audio->encode(buffer, bufferSize) > 0;;
	*/
}


} } // namespace Sourcey::Media
