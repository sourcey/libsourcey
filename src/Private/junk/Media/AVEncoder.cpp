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
	_formatCtx(NULL)
	/*,
	_videoBuffer(NULL),
	_videoBufferSize(0),
	_videoInFrame(NULL),
	_videoOutFrame(NULL),
	_videoStream(NULL),
	_videoConvCtx(NULL),
	_videoPTS(0),
	_videoLastPTS(-1),
	_videoTime(0),
	_audioBuffer(NULL),
	_audioBufferSize(0),
	_audioFifo(NULL),
	_audioFifoOutBuffer(NULL),
	_audioStream(NULL),
	_audioTime(0)
	*/
{
}


AVEncoder::AVEncoder() : 
	_outIOBuffer(NULL),
	_outIOBufferSize(MAX_VIDEO_PACKET_SIZE),
	_formatCtx(NULL)
	/*,
	_videoBuffer(NULL),
	_videoBufferSize(0),
	_videoInFrame(NULL),
	_videoOutFrame(NULL),
	_videoStream(NULL),
	_videoConvCtx(NULL),
	_videoPTS(0),
	_videoLastPTS(-1),
	_videoTime(0),
	_audioBuffer(NULL),
	_audioBufferSize(0),
	_audioFifo(NULL),
	_audioFifoOutBuffer(NULL),
	_audioStream(NULL),
	_audioTime(0)
	*/
{
	Log("debug") << "[AVEncoder" << this << "] Initializing" << endl;
}


AVEncoder::~AVEncoder()
{
	Log("debug") << "[AVEncoder" << this << "] Destroying" << endl;
	
	setState(this, EncoderState::Closing);
	cleanup();
}


int WriteOutputPacket(void* opaque, uint8_t* buffer, int bufferSize)
{
	// Callback example at: http://lists.mplayerhq.hu/pipermail/libav-client/2009-May/003034.html
	AVEncoder* klass = reinterpret_cast<AVEncoder*>(opaque);
	if (klass) {
		//Log("trace") << "[AVEncoder" << opaque << "] Broadcasting Output Packet: " << bufferSize << endl;

		//MediaPacket(buffer, bufferSize);
		//packet.data = buffer;
		//packet.time = time(0); // Set to pts?
		//packet.size = bufferSize;
		//klass->PacketEncoded.emit(klass, packet);
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
			throw Exception("Either video or audio parameters must be specified.");

		if (!_params.oformat.id)
			throw Exception("An output container format must be specified.");		

		av_register_all(); 

		// Allocate the output media context
		_formatCtx = avformat_alloc_context();
		if (!_formatCtx) 
			throw Exception("Unable to allocate format context!");

		if (!_params.ofile.empty())
			_snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _params.ofile.data());
		
		// Set the container codec
		//string 
		_formatCtx->oformat = av_guess_format(
			NULL, 
			_params.ofile.empty() ? 
				(string(".") + _params.oformat.name()).data() : 
				_params.ofile.data(), NULL);	
			//_params.ofile.empty() ? NULL : _params.ofile.data(), NULL);	
		if (!_formatCtx->oformat)
			throw Exception("Unable to find suitable output format.");

		// Set the encoder codec
		if (_params.oformat.video.enabled)
			_formatCtx->oformat->video_codec = static_cast<CodecID>(_params.oformat.video.id);
		if (_params.oformat.audio.enabled)
			_formatCtx->oformat->audio_codec = static_cast<CodecID>(_params.oformat.audio.id);

		// Set the output parameters
		if (av_set_parameters(_formatCtx, NULL) < 0)
			throw Exception("Invalid output format parameters!");

		// Initialize the video codec (if required)
		if (_params.oformat.video.enabled && _formatCtx->oformat->video_codec != CODEC_ID_NONE)
			openVideo(); 

		// Initialize the audio codec (if required)
		if (_params.oformat.audio.enabled && _formatCtx->oformat->audio_codec != CODEC_ID_NONE)
			openAudio();

		if (_params.ofile.empty()) {

			// Operating in streaming mode. Generated packets can be
			// obtained by connecting to the PacketEncoded signal.
			// Setup the output IO context for our output stream.
			_outIO = new ByteIOContext();
			_outIOBuffer = new unsigned char[_outIOBufferSize];
			init_put_byte(_outIO, _outIOBuffer, _outIOBufferSize, 0, this, 0, WriteOutputPacket, 0);
			_formatCtx->pb = _outIO;
		}
		else {

			// Operating in file mode.  
			// Open the output file...
			if (!(_formatCtx->oformat->flags & AVFMT_NOFILE)) {
				if (url_fopen(&_formatCtx->pb, _params.ofile.data(), URL_WRONLY) < 0) {
					throw Exception("AVWriter: Unable to open the output file!");
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
    if (_videoStream) closeVideo();	
    if (_audioStream) closeAudio();
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

	_videoPTS = 0;
	_videoLastPTS = -1;
	_videoTime = 0;
	_audioTime = 0;

	Log("debug") << "[AVEncoder" << this << "] Cleanup: OK" << endl;
}


void AVEncoder::process(IPacket& packet)
{	
	if (!isReady()) {
		Log("debug") << "[AVEncoder" << this << "] Encoder not ready: Dropping Packet: " << packet.className() << endl;
		return;
	}

	Log("trace") << "[AVEncoder" << this << "] Processing Packet: " << &packet << ": " << packet.className() << endl;

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
	
	Log("error") << "[AVEncoder" << this << "] Failed to encode packet." << endl;
}

					
void AVEncoder::onStreamStateChange(const PacketStreamState& state) 
{ 
	Log("debug") << "[AVEncoder" << this << "] Stream State Changed: " << state.toString() << endl;

	switch (state.id()) {
	//	break;
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

	PacketEmitter::onStreamStateChange(state);
}

// -------------------------------------------------------------------
//
// Video Stuff
//
// -------------------------------------------------------------------
void AVEncoder::openVideo()
{	
	assert(_params.oformat.video.enabled);

	// Add a video stream that uses the format's default video 
	// codec to the format context's streams[] array.
	_videoStream = av_new_stream(_formatCtx, 0);
	if (!_videoStream)
		throw Exception("Unable to initialize the video stream!");

	// Set a few optimal pixel formats for lossless codecs of interest.
	//int bitrateScale = 64;
	switch (_formatCtx->oformat->video_codec) {
	case CODEC_ID_JPEGLS:
		// PixelFormat::BGR24 or GRAY8 depending on is_color...
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
	_videoStream->codec->codec_type = AVMEDIA_TYPE_VIDEO;

	// Put sample parameters
	_videoStream->codec->bit_rate = _params.oformat.video.bitRate; 
	_videoStream->codec->bit_rate_tolerance = _params.oformat.video.bitRate * 100;
	//mpa_vidctx->bit_rate = usebitrate;
   // mpa_vidctx->bit_rate_tolerance = usebitrate * 100;
	//_videoStream->codec->bit_rate         = 400000;      // TODO: BITRATE SETTINGS!
	//_videoStream->codec->bit_rate = 1000 * bitrateScale;
	//_videoStream->codec->bit_rate = _params.oformat.video.width * _params.oformat.video.height * bitrateScale; 

	// Resolution must be a multiple of two
	_videoStream->codec->width = _params.oformat.video.width;
	_videoStream->codec->height = _params.oformat.video.height;

	// time base: this is the fundamental unit of time (in seconds) in terms
	// of which frame timestamps are represented. for fixed-fps content,
	// timebase should be 1/framerate and timestamp increments should be
	// identically 1.
	_videoStream->codec->time_base.num = 1;
	_videoStream->codec->time_base.den = _params.oformat.video.fps;

	// Emit one intra frame every twelve frames at most
	//_videoStream->codec->gop_size = 12; 
	_videoStream->codec->gop_size = _params.oformat.video.fps;

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

	/*
	// libx264-medium.ffpreset preset
	_videoStream->codec->coder_type = 1;  // coder = 1
	_videoStream->codec->flags|=CODEC_FLAG_LOOP_FILTER;   // flags=+loop
	_videoStream->codec->me_cmp|= 1;  // cmp=+chroma, where CHROMA = 1
	_videoStream->codec->partitions|=X264_PART_I8X8+X264_PART_I4X4+X264_PART_P8X8+X264_PART_B8X8; // partitions=+parti8x8+parti4x4+partp8x8+partb8x8
	_videoStream->codec->me_method=ME_HEX;    // me_method=hex
	_videoStream->codec->me_subpel_quality = 7;   // subq=7
	_videoStream->codec->me_range = 16;   // me_range=16
	_videoStream->codec->gop_size = 250;  // g=250
	_videoStream->codec->keyint_min = 25; // keyint_min=25
	_videoStream->codec->scenechange_threshold = 40;  // sc_threshold=40
	_videoStream->codec->i_quant_factor = 0.71; // i_qfactor=0.71
	_videoStream->codec->b_frame_strategy = 1;  // b_strategy=1
	_videoStream->codec->qcompress = 0.6; // qcomp=0.6
	_videoStream->codec->qmin = 10;   // qmin=10
	_videoStream->codec->qmax = 51;   // qmax=51
	_videoStream->codec->max_qdiff = 4;   // qdiff=4
	_videoStream->codec->max_b_frames = 3;    // bf=3
	_videoStream->codec->refs = 3;    // refs=3
	_videoStream->codec->directpred = 1;  // directpred=1
	_videoStream->codec->trellis = 1; // trellis=1
	_videoStream->codec->flags2|=CODEC_FLAG2_BPYRAMID+CODEC_FLAG2_MIXED_REFS+CODEC_FLAG2_WPRED+CODEC_FLAG2_8X8DCT+CODEC_FLAG2_FASTPSKIP;  // flags2=+bpyramid+mixed_refs+wpred+dct8x8+fastpskip
	_videoStream->codec->weighted_p_pred = 2; // wpredp=2

	// libx264-main.ffpreset preset
	_videoStream->codec->flags2|=CODEC_FLAG2_8X8DCT;
	//_videoStream->codec->flags2^=CODEC_FLAG2_8X8DCT;    // flags2=-dct8x8
	*/

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
   		throw Exception("Video codec not found!");

	// Open the video codec
	if (avcodec_open(_videoStream->codec, codec) < 0)
   		throw Exception("Unable to open the video codec!");
		
	// Initialize the video buffer
	_videoBufferSize = _videoStream->codec->width * _videoStream->codec->height * 3;
	_videoBuffer = new unsigned char[_videoBufferSize];

	// Allocate the input frame
	//_videoInFrame = createVideoFrame(static_cast<::PixelFormat>(_params.iformat.video.pixfmt), 
	_videoInFrame = createVideoFrame((::PixelFormat)_params.iformat.video.pixfmt, 
		_params.iformat.video.width, _params.iformat.video.height);
	if (!_videoInFrame)
		throw Exception("Unable to allocate input frame!");

	// Allocate the output frame
	_videoOutFrame = createVideoFrame(_videoStream->codec->pix_fmt, 
		_videoStream->codec->width, _videoStream->codec->height);
	if (!_videoOutFrame)
		throw Exception("Unable to allocate output frame!");
}


void AVEncoder::closeVideo()
{
	Log("debug") << "[AVEncoder" << this << "] Closing Video" << endl;

	if (_videoStream) {
		avcodec_close(_videoStream->codec);
		_videoStream = NULL;
	}
    if (_videoInFrame) {
		//if (_videoInFrame->data)
		//	av_free(_videoInFrame->data[0]);
		av_free(_videoInFrame);
		_videoInFrame = NULL;
	}
    if (_videoOutFrame) {
		if (_videoOutFrame->data)
			av_free(_videoOutFrame->data[0]);
        av_free(_videoOutFrame);
		_videoOutFrame = NULL;
    }
	if (_videoBuffer) {
		delete _videoBuffer;
		_videoBuffer = NULL;
	}
	if (_videoConvCtx) {
		sws_freeContext(_videoConvCtx);
		_videoConvCtx = NULL;
	}
}


bool AVEncoder::encodeVideo(unsigned char* buffer, int bufferSize, int width, int height)
{
	//Log("trace") << "[AVEncoder" << this << "] Encoding Video Packet" << endl;

	// Lock the mutex while encoding
	FastMutex::ScopedLock lock(_mutex);

	assert(_videoInFrame);
	assert(_videoOutFrame);

	if (!isReady())
		throw Exception("The encoder is not initialized.");

	if (_params.iformat.video.width != width || 
		_params.iformat.video.height != height)
		throw Exception("The input video frame is the wrong size.");

	if (!buffer || !bufferSize || !width || !height)
		throw Exception("Failed to encode video frame.");

	AVStream* stream = _videoStream;
	AVCodecContext* codec = stream->codec;

	// This is a hack to speed the things up a bit. We just assign the
	// pointer buffer to _videoInFrame->data[0] without using a memcpy.
	_videoInFrame->data[0] = (UInt8*)buffer;
	//memcpy(_videoInFrame->data[0], buffer, bufferSize);

	// Initialize scale conversion context if unitinitialized or if the
	// video input size has changed.
	if (_videoConvCtx == NULL) {
		_videoConvCtx = sws_getContext(
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
	
	if (sws_scale(_videoConvCtx,
		_videoInFrame->data, _videoInFrame->linesize, 0, height,
		_videoOutFrame->data, _videoOutFrame->linesize) < 0)
		throw Exception("Pixel format conversion not supported.");

	if (_formatCtx->oformat->flags & AVFMT_RAWPICTURE) {
		AVPacket packet;
		av_init_packet(&packet);
		packet.flags |= AV_PKT_FLAG_KEY;
		packet.stream_index = stream->index;
		packet.data = (UInt8 *)_videoOutFrame;
		packet.size = sizeof(AVPicture);

		int result = av_interleaved_write_frame(_formatCtx, &packet);
		if (result < 0) {
			Log("error") << "[AVEncoder" << this << "] Failed to write video frame." << endl;
			//throw Exception("Failed to write video frame.");
			return false;
		}
	} 
	else {
		// PTS value will increment by 1 for input each frame at defined FPS value.
		// PTS value will need to be dynamically generated for variable FPS rates.
		_fpsCounter.tick();
		if (_videoOutFrame->pts == AV_NOPTS_VALUE)
			_videoOutFrame->pts = 0;
		else {
			double fpsDiff = (codec->time_base.den / _fpsCounter.fps);
			_videoPTS = _videoPTS + fpsDiff;
			_videoOutFrame->pts = _videoPTS;
		
		/*
			Log("debug") << "[AVEncoder" << this << "] FPS Difference: " << fpsDiff << endl;
			Log("debug") << "[AVEncoder" << this << "] FPS Difference 1: " << (1.0 / fpsDiff) << endl;
			Log("debug") << "[AVEncoder" << this << "] Calculated PTS: " << _videoPTS << endl;
			Log("debug") << "[AVEncoder" << this << "] Using PTS: " << _videoOutFrame->pts << endl;
			Log("debug") << "[AVEncoder" << this << "] Current FPS: " << _fpsCounter.fps << endl;
		*/
		}
		
 		// Encode the frame 
		int size = avcodec_encode_video(codec, _videoBuffer, _videoBufferSize, _videoOutFrame);	
		if (size > 0) 
		{
			AVPacket packet;
			av_init_packet(&packet);	
	
			if (codec->coded_frame &&
				codec->coded_frame->pts != AV_NOPTS_VALUE) {
				packet.pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);
				/*
				Log("debug") << "[AVEncoder" << this << "] FFmpeg PTS: " << packet.pts << endl;
				Log("debug") << "[AVEncoder" << this << "] Stream Time Base Num: " << stream->time_base.num << endl;
				Log("debug") << "[AVEncoder" << this << "] Stream Time Base Den: " << stream->time_base.den << endl;
				*/
			}

            if (codec->coded_frame->key_frame)
                packet.flags |= AV_PKT_FLAG_KEY;

			packet.stream_index = stream->index;
			packet.data = _videoBuffer;
			packet.size = size;	

	 		// Write the encoded frame to the output file
			//int result = av_write_frame(_formatCtx, &packet);
			int result = av_interleaved_write_frame(_formatCtx, &packet);
			if (result < 0) {
				Log("error") << "[AVEncoder" << this << "] Failed to write video frame." << endl;
				return false;
			}
		}
		else {
			Log("debug") << "[AVEncoder" << this << "] Encoded video frame size is NULL." << endl;
			return false;
		}
	}

	return true;
}


AVFrame* AVEncoder::createVideoFrame(::PixelFormat pixfmt, int width, int height)
{
    AVFrame* picture;
    UInt8 *pictureBuf;
    int pictureSize;
    
    picture = avcodec_alloc_frame();
    if (!picture)
        return 0;
	
    pictureSize = avpicture_get_size(pixfmt, width, height);
    pictureBuf = (UInt8*)av_malloc(pictureSize);
    if (!pictureBuf) {
        av_free(picture);
        return 0;
	}
		
 	// Lets fill picture with the pictureBuf just created
	avpicture_fill((AVPicture *)picture, pictureBuf, pixfmt, width, height);

    return picture;
}


// -------------------------------------------------------------------
//
// Audio Stuff
//
// -------------------------------------------------------------------
void AVEncoder::openAudio()
{
	// Initializes the audio buffers
	_audioBuffer = new unsigned char[MAX_AUDIO_PACKET_SIZE];

	// Add an audio stream that uses the format's default video codec 
	// to the format context's streams[] array.
	_audioStream = av_new_stream(_formatCtx, 0);
	if (!_audioStream)
		throw Exception("Could not allocate audio stream");

	/*	
    audioCodec = avcodec_alloc_context();
    audioCodec->bit_rate = audioBitrate;
    audioCodec->sample_fmt = SAMPLE_FMT_S16;
    audioCodec->sample_rate = sampleRate;
    audioCodec->channels = channels;
    audioCodec->profile = FF_PROFILE_AAC_MAIN;
    audioCodec->time_base = (AVRational){1, sampleRate};
    audioCodec->codec_type = CODEC_TYPE_AUDIO;
    if (avcodec_open(audioCodec, codec) < 0) return;
	*/

	// Now we'll setup the parameters of AVCodecContext
	avcodec_get_context_defaults2(_audioStream->codec, AVMEDIA_TYPE_AUDIO);
	_audioStream->codec->codec_id = _formatCtx->oformat->audio_codec;
	_audioStream->codec->codec_type = AVMEDIA_TYPE_AUDIO;
	_audioStream->codec->bit_rate = _params.oformat.audio.bitRate;			// 64000
	_audioStream->codec->sample_rate = _params.oformat.audio.sampleRate;		// 44100
	_audioStream->codec->sample_fmt = AV_SAMPLE_FMT_S16;	
	_audioStream->codec->channels = _params.oformat.audio.channels;	 		// 2
	_audioStream->codec->time_base.num = 1;
	_audioStream->codec->time_base.den = _params.oformat.audio.sampleRate;

	if (_audioStream->codec->channels == 6)
		_audioStream->codec->channel_layout = AV_CH_LAYOUT_5POINT1;

	if (_audioStream->codec->codec_id == CODEC_ID_AAC) {		
        //_audioStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER; 
		//_audioStream->codec->profile = FF_PROFILE_AAC_MAIN;
		_audioStream->codec->profile = FF_PROFILE_AAC_LOW;		
	}
	else if (_audioStream->codec->codec_id == CODEC_ID_AMR_NB) {
		_audioStream->codec->sample_rate = 8000;
		_audioStream->codec->channels = 1;
	}

	// Some formats want stream headers to be separate
	if (_formatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
		_audioStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}

	// Find the encoder
	_audioCodec = avcodec_find_encoder(_audioStream->codec->codec_id);
	if (!_audioCodec)
		throw Exception("Audio codec not found.");

	// Open the codec
	if (avcodec_open(_audioStream->codec, _audioCodec) < 0)
		throw Exception("Unable to open the audio codec.");

	// Set the frame size
	if (_audioStream->codec->frame_size <= 1) {
		_audioStream->codec->frame_size = MAX_AUDIO_PACKET_SIZE / _audioStream->codec->channels;
		switch (_audioStream->codec->codec_id) {
		case CODEC_ID_PCM_S16LE:
		case CODEC_ID_PCM_S16BE:
		case CODEC_ID_PCM_U16LE:
		case CODEC_ID_PCM_U16BE:
			_audioStream->codec->frame_size >>= 1;
			break;
		default:
			break;
		}
	}

	/*
	// Set the encoded output frame size
	//int outSize = av_get_bits_per_sample_format(_audioStream->codec->sample_fmt)/8;
	_audioFrameSize = _audioStream->codec->frame_size * _audioStream->codec->channels * 2;

	// The encoder may require a minimum number of raw audio samples for each encoding but we can't
	// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
	// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
	_audioFifo = av_fifo_alloc(2 * MAX_AUDIO_PACKET_SIZE);

	// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
	if ((_audioFifoOutBuffer = (UInt8*)av_malloc(2 * MAX_AUDIO_PACKET_SIZE)) == 0)
		throw Exception("Can't allocate buffer to read into from audio FIFO.");
		*/
}


void AVEncoder::closeAudio()
{
	Log("debug") << "[AVEncoder" << this << "] Closing Audio" << endl;

	if (_audioStream) {
		avcodec_close(_audioStream->codec);
		_audioStream = NULL;
	}
	if (_audioBuffer) {
		delete _audioBuffer;
		_audioBuffer = NULL;
	}
   // if (_params.oformat.audio.enabled)			delete _audioParams;
	if (_audioFifoOutBuffer) {
		av_free(_audioFifoOutBuffer);
		_audioFifoOutBuffer = NULL;
	}
	if (_audioFifo) {	
		av_fifo_free(_audioFifo);
		_audioFifo = NULL;
	}
}


bool AVEncoder::encodeAudio(unsigned char* buffer, int bufferSize)
{
	// Lock the mutex while encoding
	FastMutex::ScopedLock lock(_mutex);	

	if (!isReady())
		throw Exception("The encoder is not initialized.");

	assert(buffer);
	assert(bufferSize);
	assert(_audioStream);

	Log("debug") << "[AVEncoder" << this << "] Input audio frame:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << _videoBufferSize << "\n"
		//<< "Duration: " << frameDuration << "\n" 
		<< endl;
	/*
		*/

	if (!buffer || !bufferSize) 
		throw Exception("Unable to encode audio frame.");

	if (bufferSize > _videoBufferSize) 
		throw Exception("Audio frame too big.");

	av_fifo_generic_write(_audioFifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audioFifo) >= _audioFrameSize) 
	{
		av_fifo_generic_read(_audioFifo, _audioFifoOutBuffer, _audioFrameSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		int size = avcodec_encode_audio(_audioStream->codec, _audioBuffer, _audioFrameSize, (short*)_audioFifoOutBuffer);
		if (size) {
			AVPacket packet;
			av_init_packet(&packet);
			if (_audioStream->codec->coded_frame && 
				_audioStream->codec->coded_frame->pts != AV_NOPTS_VALUE) 
				packet.pts = av_rescale_q(_audioStream->codec->coded_frame->pts, _audioStream->codec->time_base, _audioStream->time_base);
			
			if (_audioStream->codec->coded_frame &&
				_audioStream->codec->coded_frame->key_frame) 
                packet.flags |= AV_PKT_FLAG_KEY;

				/*
				Log("debug") << "[AVEncoder" << this << "] Encoded audio frame:" 
					<< "\n\tFrame Size: " << size << "\n"
					<< "\n\tCoded Frame: " << _audioStream->codec->coded_frame << "\n"
					<< "\n\tKey Frame: " << (packet.flags & AV_PKT_FLAG_KEY) << "\n"
					<< endl;
					*/
            //if (codec->coded_frame->key_frame)
            //    packet.flags |= PKT_FLAG_KEY;
			//}
			//packet.flags |= AV_PKT_FLAG_KEY;
			packet.stream_index = _audioStream->index;
			packet.data = _audioBuffer;
			packet.size = size;

	 		// Write the encoded frame to the output file
			int result = av_interleaved_write_frame(_formatCtx, &packet);
			if (result != 0) {
				Log("error") << "[AVEncoder" << this << "] Failed to write audio frame." << endl;
				return false;
			}
		} 
		else {
			Log("warn") << "[AVEncoder" << this << "] Encoded video frame size is NULL; it may have been buffered." << endl;
			return false;
		}
	}

	return true;
}


} } // namespace Sourcey::Media




			/*
			else {

				// PTS value will increment by 1 for input each frame at defined FPS value.
				// PTS value will need to be dynamically generated for variable FPS rates.
				_fpsCounter.tick();
				double fpsDiff = (codec->time_base.den / _fpsCounter.fps);
				_videoPTS += 1.0 / fpsDiff;
				packet.pts = _videoPTS; //codec->frame_number * (40 * 90); //// 
				//packet.dts = packet.pts; //AV_NOPTS_VALUE;
				if (packet.pts == _videoLastPTS) {
					Log("debug") << "[AVEncoder" << this << "] Skipping frame at duplicate PTS: " << packet.pts << endl;
					return false;
				}
				_videoLastPTS = _videoPTS;
			
				Log("debug") << "[AVEncoder" << this << "] FFmpeg PTS: " << packet.pts << endl;
				//Log("debug") << "[AVEncoder" << this << "] FPS Difference: " << fpsDiff << endl;
				Log("debug") << "[AVEncoder" << this << "] Calculated PTS: " << _videoPTS << endl;
				Log("debug") << "[AVEncoder" << this << "] Current FPS: " << _fpsCounter.fps << endl;

			}
			*/

			/*
			
				//unsigned frameDuration = _videoTime ? (clock() - _videoTime) : 1;
				//_videoTime = clock();				
				//double fpsDiff = (codec->time_base.den / _fpsCounter.fps);
				 //(codec->time_base.den * _fpsCounter.fps) / 1000.0;				
				//sum = sum + ( 1.0 / i );

				//Log("debug") << "[AVEncoder" << this << "] Time Base Num: " << codec->time_base.num << endl;
				//Log("debug") << "[AVEncoder" << this << "] Frame Duration: " << frameDuration << endl;
				//Log("debug") << "[AVEncoder" << this << "] PTS Modifier: " << (codec->time_base.den / _fpsCounter.fps) << endl;
			
				//Log("debug") << "[AVEncoder" << this << "] AVFMT_VARIABLE_FPS: " << (_formatCtx->oformat->flags & AVFMT_VARIABLE_FPS) << endl;
				//Log("debug") << "[AVEncoder" << this << "] AV_NOPTS_VALUE: " << (codec->coded_frame->pts == AV_NOPTS_VALUE) << endl;			
				//Log("debug") << "[AVEncoder" << this << "] FFmpeg Frame Number: " << codec->frame_number << endl;
				//Log("debug") << "[AVEncoder" << this << "] FFmpeg Coded Frame PTS: " << codec->coded_frame->pts << endl;


			//_videoTime = clock();
			//packet.pts = _videoPTS;			
						
			//Log("debug") << "[AVEncoder" << this << "] Frame MS: " << frameMS << endl;
			Log("debug") << "[AVEncoder" << this << "] Frame PTS: " << packet.pts << endl;

			/*
			av_log(NULL, AV_LOG_DEBUG, "encoder -> %"PRId64"/%"PRId64"\n",
			   pkt.pts != AV_NOPTS_VALUE ? av_rescale(pkt.pts, encodec->time_base.den, AV_TIME_BASE*(int64_t)encodec->time_base.num) : -1,
			   pkt.dts != AV_NOPTS_VALUE ? av_rescale(pkt.dts, encodec->time_base.den, AV_TIME_BASE*(int64_t)encodec->time_base.num) : -1);
			
mmh. Which framerate do you want to have? 10 fps? If yes, then your settings are ok. But this is unusual. Normaly are 24, 25 or 30 fps are used. This is a important decision and direct conected with the calculation of a pts! Anyway. Here an example for you:

codec->time_base.den = 25;
codec->time_base.num=1;

(and for h.264 then)
codec->gop_size=25;

A framerate of 25 frames per second means, that one frame plays 40ms. Because a second are 1000 milli sec. If you play 25 frames per seconds, then one frame needs 1000/25 = 40 ms. To get a correct PTS you have to know, that a PTS is scaled with 90 kHz. A valid sequence of PTS are:

Frame: 1. 2. 3. 4. 5. 6. 7. 8.
Time: 40 ms 80 ms 120 ms 160 ms 200 ms 240 ms 280 ms 320 ms ...
PTS (time*90) 3600 7200 10800 ...

So. You have to give every AVFrame before encoding a correct PTS like above. After the encoding process, don't wonder, the order is a different one. Thats because every frames will saved in decoding order. 









	 		// Calculate the frame PTS based on the frame duration
			//if (codec->codec_id == CODEC_ID_FLV1)
			//	_videoPTS += 1000/(1/(frameDuration/1000.0));
			//else
			//_videoPTS += codec->time_base.den/(1/(frameDuration/1000.0));
			UInt64 frameDuration = _videoTime ? (clock() - _videoTime) : 1;
			_videoTime = clock();
			_videoPTS += codec->time_base.den/(codec->time_base.num/(frameDuration/1000.0));

			Log("debug") << "[AVEncoder" << this << "] Current Video PTS:" << _videoPTS << endl;	
			Log("debug") << "[AVEncoder" << this << "] Frame Duration:" << frameDuration << endl;	
			Log("debug") << "[AVEncoder" << this << "] Prev Video PTS:" << _videoLastPTS << endl;	

			packet.pts = _videoPTS;

			if (_videoLastPTS == packet.pts) {
				Log("debug") << "[AVEncoder" << this << "] Skipping frame at duplicate PTS." << endl;
				return false;
			}

			_videoLastPTS = packet.pts;
			//if(codec->coded_frame->key_frame)
			//	packet.flags |= PKT_FLAG_KEY;
			*/
			