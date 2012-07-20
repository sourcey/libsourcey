#include "Sourcey/Media/AVWriter.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;
using namespace Sourcey::Media;


// ============================================================================
//
// AV Writer
//
// ============================================================================
FastMutex AVWriter::_mutex;


AVWriter::AVWriter(std::string filename, VideoParams* videoParams, AudioParams* audioParams) : 
	_filename(filename),
	_videoParams(videoParams),
	_videoBuffer(NULL),
	_videoInFrame(NULL),
	_videoOutFrame(NULL),
	_videoPTS(0),
	_videoPrevPTS(-1),
	_videoStream(NULL),
	_videoCodecCtx(NULL),
	_audioParams(audioParams),
	_audioOut(NULL),
	_audioOutputBuffer(NULL),
	_audioFifo(NULL),
	_audioFifoOutBuf(NULL),
	_audioStream(NULL),
	_audioCodecCtx(NULL),
	_outputFmt(NULL),
	_formatCtx(NULL),
	_isInitialized(false)
{
	cout << "AVWriter::AVWriter()" << endl;
	assert(_filename.size());

	if (initialize() == true) {
		_isInitialized = true;
	} else {
		close();
	}
}

AVWriter::~AVWriter()
{
	cout << "AVWriter::~AVWriter()" << endl;
	if (_isInitialized)
		close();
}


bool AVWriter::initialize() 
{
	try 
	{
		//bool isInitialized = true;

 		// Lock our mutex during initialization
		FastMutex::ScopedLock lock(_mutex);

 		// Register all codecs
		av_register_all(); 

 		// Set our output container format with defaults
		if (_videoParams)
			_outputFmt = guess_format(/*_videoParams->codec.c_str()*/"", _filename.c_str(), NULL);	
		else if (_audioParams) // We have no video...
			assert(false);
		//	_outputFmt = guess_format(_audioParams->codec.c_str(), _filename.c_str(), NULL);	
		else
			throw Exception("AVWriter: No Audio/Video parameters specified!");

		if (!_outputFmt) {
			cerr << "AVWriter: Could not find suitable output format for '" << _videoParams->codec << "', defaulting to MPEG." << endl;
			_outputFmt = guess_format("mpeg", NULL, NULL);
		}	

		/*
 		// Set our output container format with defaults
		switch (_videoParams->codec) {		
			case CODEC_ID_H264:
				_outputFmt = guess_format("h264", NULL, NULL);		
				if (!_outputFmt) {
					throw Exception("AVWriter: Could not find suitable output format for h264.");
					isInitialized = false;
				}		
				break;
			case CODEC_ID_FLV1:
				_outputFmt = guess_format("flv", NULL, NULL);		
				if (!_outputFmt) {
					throw Exception("AVWriter: Could not find suitable output format for flv.");
					isInitialized = false;
				}	
				break;
			case CODEC_ID_MPEG4:
				_outputFmt = guess_format("mp4", NULL, NULL);		
				if (!_outputFmt) {
					throw Exception("AVWriter: Could not find suitable output format for mp4.");
					isInitialized = false;
				}	
				break;
			default:
		 		// Auto detect the output format from the name, defaulting to mpeg
				_outputFmt = guess_format(NULL, _filename, NULL);
				if (!_outputFmt) {
					_outputFmt = guess_format("mpeg", NULL, NULL);
					throw Exception("AVWriter: Could not find suitable output format, defaulting to mpeg.");
				}
				break;
		}
		*/
		
 		// Set the sub-codecs we want to use if they were specified
		if (_videoParams)
		{
			//AVCodec *oCodec = avcodec_find_encoder(static_cast<CodecID>(_oParams.codec));
			_outputFmt->video_codec = static_cast<CodecID>(_videoParams->codec);
			/*
			if (_videoParams->codec == "h264")
				_outputFmt->video_codec = CODEC_ID_H264;
			else if (_videoParams->codec == "h263p")
				_outputFmt->video_codec = CODEC_ID_H263P;
			else if (_videoParams->codec == "h263")
				_outputFmt->video_codec = CODEC_ID_H263;
			else if (_videoParams->codec == "mpeg1")
				_outputFmt->video_codec = CODEC_ID_MPEG1VIDEO;
			else if (_videoParams->codec == "mpeg2")
				_outputFmt->video_codec = CODEC_ID_MPEG2VIDEO;
			else if (_videoParams->codec == "mpeg4")
				_outputFmt->video_codec = CODEC_ID_MPEG4;
			else if (_videoParams->codec == "flv")
				_outputFmt->video_codec = CODEC_ID_FLV1;
			else if (!_videoParams->codec.empty()) {
				_videoParams->codec = "";
				_outputFmt->video_codec = CODEC_ID_NONE;
				cerr << "AVWriter: The specified video codec is unsupported: " << _outputFmt->video_codec << endl;
			}
			*/
		}


		if (_audioParams)
		{
			assert(false);
			/*
			if (_audioParams->codec == "mp2")
				_outputFmt->audio_codec = CODEC_ID_MP2;
			else if (_audioParams->codec == "mp3")
				_outputFmt->audio_codec = CODEC_ID_MP3;
			else if (_audioParams->codec == "aac")
				_outputFmt->audio_codec = CODEC_ID_AAC;
			else if (_audioParams->codec == "pcm_s16le")
				_outputFmt->audio_codec = CODEC_ID_PCM_S16LE;
			else if (!_videoParams->codec.empty()) {
				_audioParams->codec = "";
				_outputFmt->audio_codec = CODEC_ID_NONE;
				cerr << "AVWriter: The specified audio codec is unsupported: " << _outputFmt->audio_codec << endl;
			}
			*/
		}

		// Allocate the output media context
		_formatCtx = av_alloc_format_context();
		if (!_formatCtx) {
			throw Exception("AVWriter: Unable to allocate format context!");
			//result = false;
		}
		_formatCtx->oformat = _outputFmt;
		_snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s", _filename.c_str());

		// Set the output parameters (must be done even if no parameters)
		if (av_set_parameters(_formatCtx, NULL) < 0) {
			throw Exception("AVWriter: Invalid output format parameters!");
			//result = false;
		}	
		
		// Add the audio and video streams using the default format codecs
		// and initialize the codecs
		if (_audioParams && 
			//_audioParams->codec.size() && 
			_outputFmt->audio_codec != CODEC_ID_NONE) {
	 		// Open codec associated with current audio stream
			if (!openAudio()) {
				throw Exception("AVWriter: Unable to open the audio codec!");
				//result = false;
			}
		}
		if (_videoParams && 
			//_videoParams->codec.size() && 
			_outputFmt->video_codec != CODEC_ID_NONE) {
	 		// Open codec associated with current video stream
			if (!openVideo()) {
				throw Exception("AVWriter: Unable to open the video codec!");
				//result = false;
			}
		}

 		// Open the output file
		if (!(_outputFmt->flags & AVFMT_NOFILE)) {
			if (url_fopen(&_formatCtx->pb, _filename.c_str(), URL_WRONLY) < 0) {
				throw Exception("AVWriter: Unable to open the output file!");
				//result = false;
			}
		}	

		// Write the stream header
		av_write_header(_formatCtx);

 		// Send the format information to sdout
		dump_format(_formatCtx, 0, _filename.c_str(), 1);
	} 
	catch (Exception& exc)
	{
		cout << exc.displayText() << endl;
		return false;
	}

	return true;
}


void AVWriter::close()
{
	cout << "AVWriter::close()" << endl;

 	// Lock our mutex during closure
	FastMutex::ScopedLock lock(_mutex);

 	// Write the trailer
    if (_formatCtx &&
		_formatCtx->pb) 
		av_write_trailer(_formatCtx);

    // Close each codec
    if (_audioStream) closeAudio();
    if (_videoStream) closeVideo();	
	if (_formatCtx) 
	{
	 	// Free the streams
		for (unsigned int i = 0; i < _formatCtx->nb_streams; i++) {
			av_freep(&_formatCtx->streams[i]->codec);
			av_freep(&_formatCtx->streams[i]);
		}

	 	// Close the output file
		if (_formatCtx->pb &&  
			_outputFmt && !(_outputFmt->flags & AVFMT_NOFILE))
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
bool AVWriter::encodeVideoFrame(unsigned char* buffer, int bufferBytes, int width, int height, int durationInMS)
{
	std::cout << "AVWriter::encodeVideoFrame"  << std::endl;
	try 
	{
		std::cout << "AVWriter: Input video frame:\n" 
			<< "Frame Size: " << bufferBytes << "\n"
			<< "Buffer Size: " << MAX_VIDEO_PACKET_SIZE << "\n"
			<< "Duration: " << durationInMS << "\n" << std::endl;
		/*
			*/

		if (!buffer || !bufferBytes || !width || !height || !_videoStream || !_isInitialized || 
			!_videoInFrame || !_videoOutFrame)
			throw Exception("AVWriter: Failed: Unable to encode video frame.");

		if (bufferBytes > MAX_VIDEO_PACKET_SIZE)
			throw Exception("AVWriter: image size too big.");

 		// This is a hack to speed the things up a bit. We just assign the pointer buffer
 		// to _videoInFrame->data[0] without using a memcpy.
 		_videoInFrame->data[0] = (uint8_t*)buffer;
		//memcpy(_videoInFrame->data[0], buffer, bufferBytes);

		_videoImgConvCtx = sws_getContext(width, height, _videoInPixFmt,
					 _videoCodecCtx->width, _videoCodecCtx->height, _videoCodecCtx->pix_fmt,
					 SWS_BICUBIC, NULL, NULL, NULL);

		if (sws_scale(_videoImgConvCtx, _videoInFrame->data,
				 _videoInFrame->linesize, 0,
				 height,
				 _videoOutFrame->data, _videoOutFrame->linesize) < 0)
		{
			throw Exception("AVWriter: Pixel format conversion from BGR24 not supported.");
		}
		sws_freeContext(_videoImgConvCtx);

		if (_formatCtx->oformat->flags & AVFMT_RAWPICTURE) 
		{
			AVPacket packet;
			av_init_packet(&packet);

			packet.flags |= PKT_FLAG_KEY;
			packet.stream_index = _videoStream->index;
			packet.data = (uint8_t *)_videoOutFrame;
			packet.size = sizeof(AVPicture);

			int frameSize = av_interleaved_write_frame(_formatCtx, &packet);
			if (frameSize == 0) 
				throw Exception("AVWriter: Encoded video frame size is NULL; it may have been buffered.");
		} 
		else 
		{
	 		// Encode the frame 
			int frameSize = avcodec_encode_video(_videoCodecCtx, _videoBuffer, MAX_VIDEO_PACKET_SIZE, _videoOutFrame);	
			if (frameSize > 0) 
			{
				AVPacket packet;
				av_init_packet(&packet);	

				/* 
				if (_videoCodecCtx->coded_frame->pts != AV_NOPTS_VALUE)
					packet.pts= av_rescale_q(_videoCodecCtx->coded_frame->pts, _videoCodecCtx->time_base, _videoStream->time_base);
				if (_videoCodecCtx->coded_frame->key_frame)
					packet.flags |= PKT_FLAG_KEY;
				*/		

		 		// Calculate the frame PTS based on the variable durationInMS value
				//if (_videoCodecCtx->codec_id == CODEC_ID_FLV1)
				//	_videoPTS += 1000/(1/(durationInMS/1000.0));
				//else
				//_videoPTS += _videoCodecCtx->time_base.den/(1/(durationInMS/1000.0));

				_videoPTS += _videoCodecCtx->time_base.den/(_videoCodecCtx->time_base.num/(durationInMS/1000.0));

				cout << "AVWriter: _videoCodecCtx->time_base.den: " << _videoCodecCtx->time_base.den << endl;
				cout << "AVWriter: _videoCodecCtx->time_base.num: " << _videoCodecCtx->time_base.num << endl;
				cout << "AVWriter: Actual Video PTS: " << packet.pts << endl;
				cout << "AVWriter: Current Video PTS:" << _videoPTS << endl;	
				cout << "AVWriter: Prev Video PTS:" << _videoPrevPTS << endl;	
				cout << "AVWriter: durationInMS:" << durationInMS << endl;	
				cout << "AVWriter: CALCULATED:" << _videoCodecCtx->time_base.den/(_videoCodecCtx->time_base.num/(durationInMS/1000.0)) << endl;	
				cout << "AVWriter: Ffmpeg Video PTS: " << av_rescale_q(_videoCodecCtx->coded_frame->pts, _videoCodecCtx->time_base, _videoStream->time_base) << endl;	

				/*
				cout << "AVWriter: (1/(durationInMS/1000.0):" << (int)1/(durationInMS/1000.0) << endl;	
				*/

				packet.pts = _videoPTS;
				if (_videoPrevPTS == packet.pts)
					throw Exception("AVWriter: Skipping frame at duplicate PTS");

				_videoPrevPTS = packet.pts;
				if(_videoCodecCtx->coded_frame->key_frame)
					packet.flags |= PKT_FLAG_KEY;

				packet.stream_index = _videoStream->index;
				packet.data = _videoBuffer;
				packet.size = frameSize;	

		 		// Write the encoded frame to the output file
				int result = av_interleaved_write_frame(_formatCtx, &packet);
				if (result != 0)
					throw Exception("AVWriter: Failed while writing video frame.");
			}
			else
				throw Exception("AVWriter: Encoded video frame size is NULL; it may have been buffered.");
		}
	} 
	catch (Exception& exc)
	{
		cout << exc.displayText() << endl;
		return false;
	}
	return true;
}


AVFrame* AVWriter::createVideoFrame(PixelFormat pixfmt)
{
    AVFrame *picture;
    uint8_t *pictureBuf;
    int pictureSize;
    
    picture = avcodec_alloc_frame();
    if (!picture)
        return NULL;
	
    pictureSize = avpicture_get_size(pixfmt, _videoParams->width, _videoParams->height);
    pictureBuf = (uint8_t*)av_malloc(pictureSize);
    if (!pictureBuf) {
        av_free(picture);
        return NULL;
	}
		
 	// Lets fill picture with the pictureBuf just created
	avpicture_fill((AVPicture *)picture, pictureBuf, pixfmt, _videoParams->width, _videoParams->height);

    return picture;
}


bool AVWriter::openVideo()
{
	try 
	{
		int bitrateScale = 64;
		
 		// Initializes the video buffer
		_videoBuffer = new unsigned char[MAX_VIDEO_PACKET_SIZE];

 		// Add a video stream that uses the format's default video 
 		// codec to the format context's streams[] array.
		_videoStream = av_new_stream(_formatCtx, 0);
		if (!_videoStream)
			throw Exception("AVWriter: Unable to initialize the video stream!");
		
 		// Set out video codec context
		_videoCodecCtx = _videoStream->codec;

		// Set a few optimal pixel formats for lossless codecs of interest
		switch (_outputFmt->video_codec) {
		case CODEC_ID_JPEGLS:
			// BGR24 or GRAY8 depending on is_color...
			_videoCodecCtx->pix_fmt = PIX_FMT_BGR24;
			break;
		case CODEC_ID_HUFFYUV:
			_videoCodecCtx->pix_fmt = PIX_FMT_YUV422P;
			break;
		case CODEC_ID_MJPEG:
		case CODEC_ID_LJPEG:
		  _videoCodecCtx->pix_fmt = PIX_FMT_YUVJ420P;
		  bitrateScale = 128;
		  break;
		case CODEC_ID_RAWVIDEO:
		default:
			// Good for lossy formats, MPEG, etc.
			_videoCodecCtx->pix_fmt = PIX_FMT_YUV420P;
			break;
		}
		
 		// Set out input video pixel format
		_videoInPixFmt = static_cast<PixelFormat>(_videoParams->pixfmt);
		/*
		if (_videoParams->pixfmt == "rgb24")
			_videoInPixFmt = PIX_FMT_RGB24;
		else if (_videoParams->pixfmt == "bgr24")
			_videoInPixFmt = PIX_FMT_BGR24;
		else if (_videoParams->pixfmt == "yuv420p")
			_videoInPixFmt = PIX_FMT_YUV420P;
		else if (_videoParams->pixfmt == "yuyv422")
			_videoInPixFmt = PIX_FMT_YUYV422;
		else
			_videoInPixFmt = PIX_FMT_BGR24;
			*/

 		//_videoCodecCtx->codec_id = av_guess_codec(_formatCtx->oformat, NULL, _formatCtx->filename, NULL, CODEC_TYPE_VIDEO);
		_videoCodecCtx->codec_id = _outputFmt->video_codec;	
		_videoCodecCtx->codec_type = CODEC_TYPE_VIDEO;

 		// Put sample parameters
		_videoCodecCtx->bit_rate = 400000;
 		//_videoCodecCtx->bit_rate = _videoParams->width * _videoParams->height * bitrateScale; 

 		// Resolution must be a multiple of two
		_videoCodecCtx->width = _videoParams->width;
		_videoCodecCtx->height = _videoParams->height;

 		// time base: this is the fundamental unit of time (in seconds) in terms
		// of which frame timestamps are represented. for fixed-fps content,
		// timebase should be 1/framerate and timestamp increments should be
		// identically 1.
		_videoCodecCtx->time_base.den = _videoParams->fps;
		if (_videoCodecCtx->codec_id == CODEC_ID_FLV1) {
			_videoCodecCtx->time_base.num = 1000;
		} else {
			_videoCodecCtx->time_base.num = 1;
		}	

 		// Emit one intra frame every twelve frames at most
		_videoCodecCtx->gop_size = 12; 

		if (_videoCodecCtx->codec_id == CODEC_ID_MPEG2VIDEO) {
			_videoCodecCtx->max_b_frames = 2;
		}
		if (_videoCodecCtx->codec_id == CODEC_ID_MPEG1VIDEO || 
			_videoCodecCtx->codec_id == CODEC_ID_MSMPEG4V3) {
			// Needed to avoid using macroblocks in which some coecs overflow
	 		// this doesn't happen with normal video, it just happens here as the
	 		// motion of the chroma plane doesn't match the luma plane 
	 		// avoid Ffmpeg warning 'clipping 1 dct coefficients...'
			_videoCodecCtx->mb_decision=2;
		}

 		// Some formats want stream headers to be separate
		if(_formatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
			_videoCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;
		}
		
		_videoCodec = avcodec_find_encoder(_videoCodecCtx->codec_id);
		if (!_videoCodec)
       		throw Exception("AVWriter: Video codec not found!");

		// Open the video codec
		if (avcodec_open(_videoCodecCtx, _videoCodec) < 0)
       		throw Exception("AVWriter: Unable to open the video codec!");
		
 		// We should now create the frame on which we can store the 
 		// converted bytes ready to be encoded
		_videoOutFrame = createVideoFrame(_videoStream->codec->pix_fmt);
		if (!_videoOutFrame)
			throw Exception("AVWriter: Could not allocate encoded frame!");

 		// This _videoInFrame will be converted to a stream-compatible 
		// AVFrame [see _videoOutFrame]
		_videoInFrame = createVideoFrame(_videoInPixFmt);
		if (!_videoInFrame)
			throw Exception("AVWriter: Could not allocate input frame!");
	} 
	catch (Exception& exc)
	{
		cout << exc.displayText() << endl;
		return false;
	}

	return true;
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
}


// -------------------------------------------------------------------
//
// Audio Stuff
//
// -------------------------------------------------------------------
bool AVWriter::openAudio()
{
	try 
	{
 		// Initializes the audio buffers
		_audioOut = new unsigned char[MAX_AUDIO_PACKET_SIZE];
		_audioOutputBuffer = new unsigned char[MAX_AUDIO_PACKET_SIZE];

 		// Add an audio stream that uses the format's default video codec 
 		// to the format context's streams[] array.
		_audioStream = av_new_stream(_formatCtx, 0);
		if (!_audioStream)
			throw Exception("AVWriter: Could not alloc stream");

 		// Now we'll setup the parameters of AVCodecContext
		_audioCodecCtx = _audioStream->codec;
		avcodec_get_context_defaults2(_audioCodecCtx, CODEC_TYPE_AUDIO);
		_audioCodecCtx->codec_id = _outputFmt->audio_codec;
		_audioCodecCtx->codec_type = CODEC_TYPE_AUDIO;
		_audioCodecCtx->bit_rate = _audioParams->bitRate;			 // 64000
		if (_audioCodecCtx->codec_id == CODEC_ID_AMR_NB) {
			_audioCodecCtx->sample_rate = 8000;
			_audioCodecCtx->channels = 1;
		} else {
			_audioCodecCtx->sample_rate = _audioParams->sampleRate; 	// 44100
			_audioCodecCtx->channels = _audioParams->channels;	 	// 2
		}

 		// Find the encoder
		_audioCodec = avcodec_find_encoder(_audioCodecCtx->codec_id);
		if (!_audioCodec)
			throw Exception("AVWriter: Audio codec not found.");

 		// Open the codec
		if (avcodec_open(_audioCodecCtx, _audioCodec) < 0)
			throw Exception("AVWriter: Unable to open the audio codec.");

		// Set the frame size
		if (_audioCodecCtx->frame_size <= 1) {
			_audioCodecCtx->frame_size = MAX_AUDIO_PACKET_SIZE / _audioCodecCtx->channels;
			switch(_audioCodecCtx->codec_id) {
			case CODEC_ID_PCM_S16LE:
			case CODEC_ID_PCM_S16BE:
			case CODEC_ID_PCM_U16LE:
			case CODEC_ID_PCM_U16BE:
				_audioCodecCtx->frame_size >>= 1;
				break;
			default:
				break;
			}
		} else {
			_audioCodecCtx->frame_size = _audioCodecCtx->frame_size;
		}

 		// Set the encoded output frame size
		//int outSize = av_get_bits_per_sample_format(_audioCodecCtx->sample_fmt)/8;
		int outSize = 2;
		_audioOutSize = _audioCodecCtx->frame_size * outSize * _audioCodecCtx->channels;

 		// The encoder may require a minimum number of raw audio samples for each encoding but we can't
 		// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
 		// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
		_audioFifo = av_fifo_alloc(2 * MAX_AUDIO_PACKET_SIZE);

 		// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
		if ((_audioFifoOutBuf = (uint8_t*)av_malloc(2 * MAX_AUDIO_PACKET_SIZE)) == 0)
			throw Exception("AVWriter: Can't allocate buffer to read into from audio FIFO.");
	} 
	catch (Exception& exc)
	{
		cout << exc.displayText() << endl;
		return false;
	}

	return true;
}


bool AVWriter::encodeAudioFrame(unsigned char* buffer, int bufferBytes, int durationInMS)
{
	try 
	{
		//cout << "encodeAudioFrame" << endl;
		/*
		cout << "AVWriter: Write audio frame:\n" 
			<< "Frame Size: " << bufferBytes << "\n"
			<< "Buffer Size: " << MAX_VIDEO_PACKET_SIZE << "\n"
			<< "Duration: " << durationInMS << "\n" << std::endl;
			*/

		if (!buffer || !bufferBytes || !_audioStream || !_isInitialized) 
			throw Exception("AVWriter: Unable to encode audio frame.");

		if (bufferBytes > MAX_VIDEO_PACKET_SIZE) 
			throw Exception("AVWriter: Audio frame too big.");

		//cout << "encodeAudioFrame 1" << endl;

		av_fifo_generic_write(_audioFifo, (uint8_t *)buffer, bufferBytes, NULL);
		while (av_fifo_size(_audioFifo) >= _audioOutSize) 
		{

			//cout << "av_fifo_size(_audioFifo): " << av_fifo_size(_audioFifo) << endl;
			//cout << "_audioOutSize: " << _audioOutSize << endl;

			av_fifo_generic_read(_audioFifo, _audioFifoOutBuf, _audioOutSize, NULL);

	 		// Encode a frame of AudioOutSize bytes
			int frameSize = avcodec_encode_audio(_audioCodecCtx, _audioOut, _audioOutSize, (short*)_audioFifoOutBuf);
			
			//cout << "frameSize: " << frameSize << endl;

			if (frameSize) 
			{
				//cout << "encodeAudioFrame 3" << endl;

				AVPacket packet;
				av_init_packet(&packet);
				if (_audioCodecCtx->coded_frame && _audioCodecCtx->coded_frame->pts != AV_NOPTS_VALUE) 
					packet.pts = av_rescale_q(_audioCodecCtx->coded_frame->pts, _audioCodecCtx->time_base, _audioStream->time_base);
				packet.flags |= PKT_FLAG_KEY;
				packet.stream_index = _audioStream->index;
				packet.data = _audioOut;
				packet.size = frameSize;

				//cout << "encodeAudioFrame 4" << endl;

		 		// Write the encoded frame to the output file
				int result = av_interleaved_write_frame(_formatCtx, &packet);
				if (result != 0) 
					throw Exception("AVWriter: Failed while writing audio frame.");
			} 
			else 
				throw Exception("AVWriter: Encoded audio frame size is NULL; it may have been buffered.");
		}

	} 
	catch (Exception& exc)
	{
		cerr << exc.displayText() << endl;
		return false;
	}

	return true;
}


void AVWriter::closeAudio()
{
    avcodec_close(_audioStream->codec);
	if (_audioOut)			delete _audioOut;
	if (_audioOutputBuffer)	delete _audioOutputBuffer;	
	if (_audioFifoOutBuf)	av_free(_audioFifoOutBuf);
	if (_audioFifo)			av_fifo_free(_audioFifo);
}