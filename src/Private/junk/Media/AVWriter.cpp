#include "Sourcey/Media/AVWriter.h"
#include "Sourcey/Base/Logger.h"

#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Media Writer
//
// ============================================================================
FastMutex AVWriter::_mutex;


AVWriter::AVWriter(const string& filename, VideoParams* videoParams, AudioParams* audioParams) : 
	_filename(filename),
	_videoParams(videoParams),
	_videoBuffer(NULL),
	_videoInFrame(NULL),
	_videoOutFrame(NULL),
	_videoPTS(0),
	_videoLastPTS(-1),
	_videoStream(NULL),
	_videoConvCtx(NULL),
	_audioParams(audioParams),
	_audioBuffer(NULL),
	_audioFifo(NULL),
	_audioFifoOutBuffer(NULL),
	_audioStream(NULL),
	_formatCtx(NULL),
	_isInitialized(false)
{
	Logger::send("debug") << "AVWriter::AVWriter()" << endl;
	assert(_filename.size());

	try {
		initialize();
	} catch (Exception& exc) {
		Logger::send("debug", Error) << "AV Writer Error: " << exc.displayText() << endl;
		close();
		throw exc;
	}
}


AVWriter::~AVWriter()
{
	Logger::send("debug") << "AVWriter::~AVWriter()" << endl;
	if (_isInitialized)
		close();
}


void AVWriter::initialize() 
{
	Logger::send("debug") << "AVWriter::initialize()" << endl;

	// Lock our mutex during initialization
	FastMutex::ScopedLock lock(_mutex);

	_isInitialized = false;

	// Register all codecs
	av_register_all(); 

	// Allocate the output media context
	_formatCtx = av_alloc_format_context();
	if (!_formatCtx) {
		throw Exception("AVWriter: Unable to allocate format context!");
	}
	_snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _filename.data());

	// Set our output container format with defaults
	// TODO: Get FFMpeg friendly codec string for guessing format
	_formatCtx->oformat = av_guess_format(/*_videoParams->codec.data()*/"", _filename.data(), NULL);	
	if (!_formatCtx->oformat) {
		throw Exception("AVWriter: Could not find suitable output format.");
		//_formatCtx->oformat = av_guess_format("mpeg", NULL, NULL);
	}

	// Set the sub-codecs we want to use if they were specified
	if (_videoParams)
		_formatCtx->oformat->video_codec = static_cast<CodecID>(_videoParams->codec);
	if (_audioParams)
		_formatCtx->oformat->audio_codec = static_cast<CodecID>(_audioParams->codec);

	// Set the output parameters (must be done even if no parameters)
	if (av_set_parameters(_formatCtx, NULL) < 0) {
		throw Exception("AVWriter: Invalid output format parameters!");
	}	

	// Add the audio and video streams using the default format codecs
	// and initialize the codecs
	if (_videoParams && _formatCtx->oformat->video_codec != CODEC_ID_NONE)
		openVideo(); 

	if (_audioParams && _formatCtx->oformat->audio_codec != CODEC_ID_NONE)
		openAudio();

	// Open the output file
	if (!(_formatCtx->oformat->flags & AVFMT_NOFILE)) {
		if (url_fopen(&_formatCtx->pb, _filename.data(), URL_WRONLY) < 0) {
			throw Exception("AVWriter: Unable to open the output file!");
			//result = false;
		}
	}	

	// Write the stream header
	av_write_header(_formatCtx);

	// Send the format information to sdout
	dump_format(_formatCtx, 0, _filename.data(), 1);

	_isInitialized = true;
}


void AVWriter::close()
{
 	// Lock our mutex during closure
	FastMutex::ScopedLock lock(_mutex);

 	// Write the trailer
    if (_formatCtx &&
		_formatCtx->pb) 
		av_write_trailer(_formatCtx);

    // Close each codec
    if (_videoStream) closeVideo();	
    if (_audioStream) closeAudio();
	if (_formatCtx) 
	{
	 	// Free the streams
		for (unsigned int i = 0; i < _formatCtx->nb_streams; i++) {
			av_freep(&_formatCtx->streams[i]->codec);
			av_freep(&_formatCtx->streams[i]);
		}

	 	// Close the output file
		if (_formatCtx->pb &&  
			_formatCtx->oformat && !(_formatCtx->oformat->flags & AVFMT_NOFILE))
			url_fclose(_formatCtx->pb);

	 	// Free the format context
		av_free(_formatCtx);
	}
}


// -------------------------------------------------------------------
//
// Video Stuff
//
// -------------------------------------------------------------------
void AVWriter::openVideo()
{	
	assert(_videoParams);

	// Initializes the video buffer
	_videoBuffer = new unsigned char[MAX_VIDEO_PACKET_SIZE];

	// Add a video stream that uses the format's default video 
	// codec to the format context's streams[] array.
	_videoStream = av_new_stream(_formatCtx, 0);
	if (!_videoStream)
		throw Exception("AVWriter: Unable to initialize the video stream!");

	// Set a few optimal pixel formats for lossless codecs of interest.
	//int bitrateScale = 64;
	switch (_formatCtx->oformat->video_codec) {
	case CODEC_ID_JPEGLS:
		// BGR24 or GRAY8 depending on is_color...
		_videoStream->codec->pix_fmt = PIX_FMT_BGR24;
		break;
	case CODEC_ID_HUFFYUV:
		_videoStream->codec->pix_fmt = PIX_FMT_YUV422P;
		break;
	case CODEC_ID_MJPEG:
	case CODEC_ID_LJPEG:
	  _videoStream->codec->pix_fmt = PIX_FMT_YUVJ420P;
	  //bitrateScale = 128;
	  break;
	case CODEC_ID_RAWVIDEO:
	default:
		// Good for lossy formats, MPEG, etc.
		_videoStream->codec->pix_fmt = PIX_FMT_YUV420P;
		break;
	}

	//_videoStream->codec->codec_id = av_guess_codec(_formatCtx->oformat, NULL, _formatCtx->filename, NULL, CODEC_TYPE_VIDEO);
	_videoStream->codec->codec_id = _formatCtx->oformat->video_codec;	
	_videoStream->codec->codec_type = CODEC_TYPE_VIDEO;

	// Put sample parameters
	_videoStream->codec->bit_rate = _videoParams->bitRate; 
	//_videoStream->codec->bit_rate = 1000 * bitrateScale;
	//_videoStream->codec->bit_rate = _videoParams->width * _videoParams->height * bitrateScale; 

	// Resolution must be a multiple of two
	_videoStream->codec->width = _videoParams->width;
	_videoStream->codec->height = _videoParams->height;

	// time base: this is the fundamental unit of time (in seconds) in terms
	// of which frame timestamps are represented. for fixed-fps content,
	// timebase should be 1/framerate and timestamp increments should be
	// identically 1.
	_videoStream->codec->time_base.den = _videoParams->fps;
	if (_videoStream->codec->codec_id == CODEC_ID_FLV1) {
		_videoStream->codec->time_base.num = 1000;
	} else {
		_videoStream->codec->time_base.num = 1;
	}	

	// Emit one intra frame every twelve frames at most
	_videoStream->codec->gop_size = 12; 

	if (_videoStream->codec->codec_id == CODEC_ID_MPEG2VIDEO) {
		_videoStream->codec->max_b_frames = 2;
	}

	if (_videoStream->codec->codec_id == CODEC_ID_MPEG1VIDEO || 
		_videoStream->codec->codec_id == CODEC_ID_MSMPEG4V3) {
		// Needed to avoid using macroblocks in which some coecs overflow
 		// this doesn't happen with normal video, it just happens here as the
 		// motion of the chroma plane doesn't match the luma plane 
 		// avoid Ffmpeg warning 'clipping 1 dct coefficients...'
		_videoStream->codec->mb_decision = 2;
	}

	// H264 specifics
	if (_videoStream->codec->codec_id == CODEC_ID_H264) {
		_videoStream->codec->me_range = 5;
		_videoStream->codec->max_qdiff = 5;
		_videoStream->codec->qmin = 20;
		_videoStream->codec->qmax = 30;
		_videoStream->codec->qcompress = 0.6f;
		_videoStream->codec->qblur = 0.1f;
		_videoStream->codec->gop_size = 3;
		_videoStream->codec->max_b_frames = 1;
		_videoStream->codec->flags = CODEC_FLAG_LOW_DELAY;
	}

	// Some formats want stream headers to be separate
	if (_formatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
		_videoStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	
	AVCodec* codec = avcodec_find_encoder(_videoStream->codec->codec_id);
	if (!codec)
   		throw Exception("AVWriter: Video codec not found!");

	// Open the video codec
	if (avcodec_open(_videoStream->codec, codec) < 0)
   		throw Exception("AVWriter: Unable to open the video codec!");

	// This _videoInFrame will be converted to a stream-compatible 
	// AVFrame [see _videoOutFrame]
	_videoInFrame = createVideoFrame(static_cast<PixelFormat>(_videoParams->pixfmt));
	if (!_videoInFrame)
		throw Exception("AVWriter: Could not allocate input frame!");
	
	// We should now create the frame on which we can store the 
	// converted bytes ready to be encoded
	_videoOutFrame = createVideoFrame(_videoStream->codec->pix_fmt);
	if (!_videoOutFrame)
		throw Exception("AVWriter: Could not allocate output frame!");

	assert(_videoStream);
	assert(_videoInFrame);
	assert(_videoOutFrame);
}


void AVWriter::closeVideo()
{
	avcodec_close(_videoStream->codec);
    if (_videoInFrame) {
		//if (_videoInFrame->data)
		//	av_free(_videoInFrame->data[0]);
		av_free(_videoInFrame);
	}
    if (_videoOutFrame) {
		if (_videoOutFrame->data)
			av_free(_videoOutFrame->data[0]);
        av_free(_videoOutFrame);
    }
    if (_videoBuffer) 
		delete _videoBuffer;
    if (_videoParams) 
		delete _videoParams;	
    if (_videoConvCtx) 
		sws_freeContext(_videoConvCtx);
}


bool AVWriter::encodeVideoFrame(unsigned char* buffer, int bufferSize, int width, int height, int durationInMS)
{
	assert(buffer);
	assert(bufferSize);
	assert(width);
	assert(height);
	//assert(durationInMS);
	assert(_isInitialized);

	/*
	Logger::send("debug") << "AVWriter: Input video frame:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << MAX_VIDEO_PACKET_SIZE << "\n"
		<< "Duration: " << durationInMS << "\n" << endl;
		*/

	if (!buffer || !bufferSize || !width || !height || !_isInitialized)
		throw Exception("AVWriter: Failed to encode video frame.");

	if (bufferSize > MAX_VIDEO_PACKET_SIZE)
		throw Exception("AVWriter: Image size too big.");

	// This is a hack to speed the things up a bit. We just assign the pointer buffer
	// to _videoInFrame->data[0] without using a memcpy.
	_videoInFrame->data[0] = (UInt8*)buffer;
	//memcpy(_videoInFrame->data[0], buffer, bufferSize);

	// Initialize image scale conversion context if unitinitialized or 
	// if the video input size has changed.
	if (!_videoConvCtx ||
		_videoParams->width != width ||
		_videoParams->height != height) {
		if (_videoConvCtx) {
			sws_freeContext(_videoConvCtx);
			_videoConvCtx = 0;
		}
		_videoConvCtx = sws_getContext(
			width, height, static_cast<PixelFormat>(_videoParams->pixfmt),
			_videoStream->codec->width, _videoStream->codec->height, _videoStream->codec->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);
	}

	if (sws_scale(_videoConvCtx,
		_videoInFrame->data, _videoInFrame->linesize, 0, height,
		_videoOutFrame->data, _videoOutFrame->linesize) < 0) {
		throw Exception("AVWriter: Pixel format conversion not supported.");
	}

	if (_formatCtx->oformat->flags & AVFMT_RAWPICTURE) {
		AVPacket packet;
		av_init_packet(&packet);
		packet.flags |= PKT_FLAG_KEY;
		packet.stream_index = _videoStream->index;
		packet.data = (UInt8 *)_videoOutFrame;
		packet.size = sizeof(AVPicture);

		int size = av_interleaved_write_frame(_formatCtx, &packet);
		if (size == 0) {
			Logger::send("debug", Error) << "AVWriter: Encoded video frame size is NULL; it may have been buffered." << endl;
			return false;
		}
	} 
	else 
	{
 		// Encode the frame 
		int size = avcodec_encode_video(_videoStream->codec, _videoBuffer, MAX_VIDEO_PACKET_SIZE, _videoOutFrame);	
		if (size > 0) 
		{
			AVPacket packet;
			av_init_packet(&packet);	

			if (_videoStream->codec->coded_frame->pts != AV_NOPTS_VALUE)
				packet.pts = av_rescale_q(_videoStream->codec->coded_frame->pts, _videoStream->codec->time_base, _videoStream->time_base);
			/* 
			if (_videoStream->codec->coded_frame->key_frame)
				packet.flags |= PKT_FLAG_KEY;
			*/		

	 		// Calculate the frame PTS based on the variable durationInMS value
			//if (_videoStream->codec->codec_id == CODEC_ID_FLV1)
			//	_videoPTS += 1000/(1/(durationInMS/1000.0));
			//else
			//_videoPTS += _videoStream->codec->time_base.den/(1/(durationInMS/1000.0));
			_videoPTS += _videoStream->codec->time_base.den/(_videoStream->codec->time_base.num/(durationInMS/1000.0));

			/*
			Logger::send("debug") << "AVWriter:  _videoStream->codec->time_base.den:" << _videoStream->codec->time_base.den << endl;	
			Logger::send("debug") << "AVWriter: durationInMS:" << durationInMS << endl;	
			Logger::send("debug") << "AVWriter: (1/(durationInMS/1000.0):" << (int)1/(durationInMS/1000.0) << endl;	
			//Logger::send("debug") << "AVWriter: Ffmpeg Video PTS: " << av_rescale_q(_videoStream->codec->coded_frame->pts, _videoStream->codec->time_base, _videoStream->time_base) << endl;	
			Logger::send("debug") << "AVWriter: Actual Video PTS: " << packet.pts << endl;
			Logger::send("debug") << "AVWriter: Current Video PTS:" << _videoPTS << endl;	
			Logger::send("debug") << "AVWriter: Prev Video PTS:" << _videoLastPTS << endl;	
			*/

			packet.pts = _videoPTS;
			//if (_videoLastPTS == packet.pts)
			//	throw Exception("AVWriter: Skipping frame at duplicate PTS");

			_videoLastPTS = packet.pts;
			if(_videoStream->codec->coded_frame->key_frame)
				packet.flags |= PKT_FLAG_KEY;

			packet.stream_index = _videoStream->index;
			packet.data = _videoBuffer;
			packet.size = size;	

	 		// Write the encoded frame to the output file
			int result = av_interleaved_write_frame(_formatCtx, &packet);
			if (result != 0) {
				throw Exception("AVWriter: Failed to write video frame.");
			}
		}
		else {
			Logger::send("debug", Error) << "AVWriter: Encoded video frame size is NULL; it may have been buffered." << endl;
			return false;
		}
	}

	return true;
}


AVFrame* AVWriter::createVideoFrame(PixelFormat pixfmt)
{
	assert(_videoParams);

    AVFrame* picture;
    UInt8 *pictureBuf;
    int pictureSize;
    
    picture = avcodec_alloc_frame();
    if (!picture)
        return 0;
	
    pictureSize = avpicture_get_size(pixfmt, _videoParams->width, _videoParams->height);
    pictureBuf = (UInt8*)av_malloc(pictureSize);
    if (!pictureBuf) {
        av_free(picture);
        return 0;
	}
		
 	// Lets fill picture with the pictureBuf just created
	avpicture_fill((AVPicture *)picture, pictureBuf, pixfmt, _videoParams->width, _videoParams->height);

    return picture;
}


// -------------------------------------------------------------------
//
// Audio Stuff
//
// -------------------------------------------------------------------
void AVWriter::openAudio()
{
	// Initializes the audio buffers
	_audioBuffer = new unsigned char[MAX_AUDIO_PACKET_SIZE];

	// Add an audio stream that uses the format's default video codec 
	// to the format context's streams[] array.
	_audioStream = av_new_stream(_formatCtx, 0);
	if (!_audioStream)
		throw Exception("AVWriter: Could not alloc stream");

	// Now we'll setup the parameters of AVCodecContext
	avcodec_get_context_defaults2(_audioStream->codec, CODEC_TYPE_AUDIO);
	_audioStream->codec->codec_id = _formatCtx->oformat->audio_codec;
	_audioStream->codec->codec_type = CODEC_TYPE_AUDIO;
	_audioStream->codec->bit_rate = _audioParams->bitRate;				// 64000
	if (_audioStream->codec->codec_id == CODEC_ID_AMR_NB) {
		_audioStream->codec->sample_rate = 8000;
		_audioStream->codec->channels = 1;
	} else {
		_audioStream->codec->sample_rate = _audioParams->sampleRate;	// 44100
		_audioStream->codec->channels = _audioParams->channels;	 		// 2
	}

	// Find the encoder
	_audioCodec = avcodec_find_encoder(_audioStream->codec->codec_id);
	if (!_audioCodec)
		throw Exception("AVWriter: Audio codec not found.");

	// Open the codec
	if (avcodec_open(_audioStream->codec, _audioCodec) < 0)
		throw Exception("AVWriter: Unable to open the audio codec.");

	// Set the frame size
	if (_audioStream->codec->frame_size <= 1) {
		_audioStream->codec->frame_size = MAX_AUDIO_PACKET_SIZE / _audioStream->codec->channels;
		switch(_audioStream->codec->codec_id) {
		case CODEC_ID_PCM_S16LE:
		case CODEC_ID_PCM_S16BE:
		case CODEC_ID_PCM_U16LE:
		case CODEC_ID_PCM_U16BE:
			_audioStream->codec->frame_size >>= 1;
			break;
		default:
			break;
		}
	} else {
		_audioStream->codec->frame_size = _audioStream->codec->frame_size;
	}

	// Set the encoded output frame size
	//int outSize = av_get_bits_per_sample_format(_audioStream->codec->sample_fmt)/8;
	int outSize = 2;
	_audioOutSize = _audioStream->codec->frame_size * outSize * _audioStream->codec->channels;

	// The encoder may require a minimum number of raw audio samples for each encoding but we can't
	// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
	// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
	_audioFifo = av_fifo_alloc(2 * MAX_AUDIO_PACKET_SIZE);

	// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
	if ((_audioFifoOutBuffer = (UInt8*)av_malloc(2 * MAX_AUDIO_PACKET_SIZE)) == 0)
		throw Exception("AVWriter: Can't allocate buffer to read into from audio FIFO.");
}


void AVWriter::closeAudio()
{
    avcodec_close(_audioStream->codec);
	if (_audioBuffer)		delete _audioBuffer;
    if (_audioParams)		delete _audioParams;
	if (_audioFifoOutBuffer)	av_free(_audioFifoOutBuffer);
	if (_audioFifo)			av_fifo_free(_audioFifo);
}


bool AVWriter::encodeAudioFrame(unsigned char* buffer, int bufferSize, int durationInMS)
{

	assert(buffer);
	assert(bufferSize);
	//assert(durationInMS);
	assert(_isInitialized);
	/*
	Logger::send("debug") << "AVWriter: Input audio frame:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << MAX_VIDEO_PACKET_SIZE << "\n"
		<< "Duration: " << durationInMS << "\n" << endl;
		*/

	if (!buffer || !bufferSize || /*!durationInMS ||*/ !_isInitialized) 
		throw Exception("AVWriter: Unable to encode audio frame.");

	if (bufferSize > MAX_VIDEO_PACKET_SIZE) 
		throw Exception("AVWriter: Audio frame too big.");

	av_fifo_generic_write(_audioFifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audioFifo) >= _audioOutSize) 
	{
		//Logger::send("debug") << "av_fifo_size(_audioFifo): " << av_fifo_size(_audioFifo) << endl;
		//Logger::send("debug") << "_audioOutSize: " << _audioOutSize << endl;

		av_fifo_generic_read(_audioFifo, _audioFifoOutBuffer, _audioOutSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		int size = avcodec_encode_audio(_audioStream->codec, _audioBuffer, _audioOutSize, (short*)_audioFifoOutBuffer);
		if (size) 
		{
			AVPacket packet;
			av_init_packet(&packet);
			if (_audioStream->codec->coded_frame && _audioStream->codec->coded_frame->pts != AV_NOPTS_VALUE) 
				packet.pts = av_rescale_q(_audioStream->codec->coded_frame->pts, _audioStream->codec->time_base, _audioStream->time_base);
			packet.flags |= PKT_FLAG_KEY;
			packet.stream_index = _audioStream->index;
			packet.data = _audioBuffer;
			packet.size = size;

	 		// Write the encoded frame to the output file
			int result = av_interleaved_write_frame(_formatCtx, &packet);
			if (result != 0) 
				throw Exception("AVWriter: Failed to write audio frame.");
		} 
		else {
			Logger::send("debug", Error) << "AVWriter: Encoded video frame size is NULL; it may have been buffered." << endl;
			return false;
		}
	}

	return true;
}


} } // namespace Sourcey::Media