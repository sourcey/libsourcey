#include "AVFileReader.h"
#include "Sourcey/Base/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


AVFileReader::AVFileReader()  : 
	_thread("AVFileReader"),
	_formatCtx(NULL),
	_video(NULL),
	_audio(NULL)
{		
	Log("trace") << "[AVFileReader" << this << "] Creating" << endl;
}


AVFileReader::~AVFileReader() 
{
	Log("trace") << "[AVFileReader" << this << "] Destroying" << endl;

	close();
}


void AVFileReader::start() 
{
	Log("trace") << "[AVFileReader:" << this << "] Starting" << endl;
	FastMutex::ScopedLock lock(_mutex);
	assert(_video || _audio);

	if (_video || _audio &&
		!_thread.isRunning()) {
		Log("trace") << "[AVFileReader:" << this << "] Initializing Thread" << endl;
		_stop = false;
		_thread.start(*this);
	}
	Log("trace") << "[AVFileReader:" << this << "] Starting: OK" << endl;
}


void AVFileReader::stop() 
{
	Log("trace") << "[AVFileReader:" << this << "] Stopping" << endl;
	FastMutex::ScopedLock lock(_mutex);	

	if (_thread.isRunning()) {
		Log("trace") << "[AVFileReader:" << this << "] Terminating Thread" << endl;		
		_stop = true;
		_thread.join();
	}

	Log("trace") << "[AVFileReader:" << this << "] Stopping: OK" << endl;
}


void AVFileReader::close()
{
	// Close the video file.
	if (_formatCtx) {
  		av_close_input_file(_formatCtx);
		_formatCtx = NULL;
  	}

	if (_video) {
		delete _video;
		_video = NULL;
	}

	if (_video) {
		delete _video;
		_video = NULL;
	}
	
	/*
	if (_formatCtx) {		
	 	// Free the format context
		av_free(_formatCtx);
		_formatCtx = NULL;
	}
	*/
}


void AVFileReader::open(const std::string& file)
{
	av_register_all();
		
	if (av_open_input_file(&_formatCtx, file.data(), NULL, 0, NULL) != 0)
		throw Exception("Could not open the media file.");

	if (av_find_stream_info(_formatCtx) < 0)
		throw Exception("Could not find stream information.");
	
  	dump_format(_formatCtx, 0, file.data(), 0);
	
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
		throw Exception("Could not find a valid media stream.");
}


void AVFileReader::run() 
{
	Log("trace") << "[AVFileReader:" << this << "] Running:"		
		<< "\n\tFilename: " << _file
		<< endl;
	
	try 
	{
		int len;
		int res;	
		for (;;) {
			AVPacket packet;
			av_init_packet(&packet);

			while ((res = av_read_frame(_formatCtx, &packet)) >= 0) {
				if (_video && packet.stream_index == _video->stream->index) {					
					while (packet.size > 0) {
						if ((len = _video->decode(packet)) <= 0)
							break;

						packet.data += len;
						packet.size -= len;

						VideoPacket video(_video->buffer, len, _video->pts);
						video.opaque = _video;
						broadcast(video);
					}

					/*
					VideoPacket video(&mat, _video->pts);
					if ((len = decodeVideo(packet)) > 0) {
						//Log("trace") << "[AVFileReader:" << this << "] Broadcasting Video: " << _video->pts() << endl;
						//cv::Mat mat(_video->codec->height, _video->codec->width, CV_8UC(3));
						//populateMatrix(mat);
						VideoPacket video(&mat, _video->pts);
						video.opaque = _video;
						broadcast(video);
					}
					*/
					break;
				}
				else if (_audio && packet.stream_index == _audio->stream->index) {		
					while (packet.size > 0) {
						if ((len = _audio->decode(packet)) <= 0)
							break;

						packet.data += len;
						packet.size -= len;

						AudioPacket audio(_audio->buffer, len, _audio->pts);
						audio.opaque = _audio;
						broadcast(audio);
					}
					/*
					if ((len = decodeAudio(packet)) > 0) {
						//Log("trace") << "[AVFileReader:" << this << "] Broadcasting Audio: " << _audio->pts() << endl;
						AudioPacket audio(_audio->buffer, len, _audio->pts); //_audio->packet->size
						audio.opaque = _audio;
						broadcast(audio);
					}
					*/
					break;
				} 

				Log("debug") << "[AVFileReader:" << this << "] Dropping frame from unknown stream:"		
					<< "\n\tStream ID: " << packet.stream_index
					<< "\n\tVideo ID: " << _video->stream->index
					<< "\n\tAudio ID: " << _audio->stream->index
					<< endl;

				av_free_packet(&packet);
			}

			if (res < 0) {
				// End of file or error.
				Log("info") << "[AVFileReader:" << this << "] Decoding: EOF" << endl;
				break;
				//return 0;
			}

			Thread::sleep(10);
		}
	} 
	catch (Exception& exc) 
	{
		Log("error") << "[AVFileReader:" << this << "] Decoder Error: " << exc.displayText() << endl;
	}
	catch (cv::Exception& exc) 
	{
		Log("error") << "[AVFileReader:" << this << "] OpenCV Error: " << exc.err << endl;
	}

	Log("trace") << "[AVFileReader:" << this << "] Exiting" << endl;
}


} } // namespace Sourcey::Media





/*
	//int _video->streamID = -1;
	//_audio->streamID = -1;
			//openAudio(i);
void AVFileReader::openVideo(int streamID)
{
	Log("info") << "[AVFileReader:" << this << "] Opening Video: " << streamID << endl;

	assert(_formatCtx);
	assert(streamID >= 0);
	
	_video = new VideoContext();
	_video->stream = _formatCtx->streams[streamID];
	_video->codec = _video->stream->codec;
	_video->codec = avcodec_find_decoder(_video->codec->codec_id);	
	if (_video->codec == NULL)
		throw Exception("The video codec is missing or unsupported.");
	
	if (avcodec_open(_video->codec, _video->codec)<0)
		throw Exception("Could not open the video codec.");
	
	_video->iframe = avcodec_alloc_frame();
	if (_video->iframe == NULL)
		throw Exception("Could not allocate the input frame.");

	_video->oframe = avcodec_alloc_frame();	
	if (_video->oframe == NULL)
		throw Exception("Could not allocate the output frame.");
	
	_video->bufferSize = avpicture_get_size(PIX_FMT_BGR24, _video->codec->width, _video->codec->height);
	_video->buffer = (uint8_t*)av_malloc(_video->bufferSize * sizeof(uint8_t));	
	avpicture_fill((AVPicture*)_video->oframe, _video->buffer, PIX_FMT_BGR24, _video->codec->width, _video->codec->height);
	
    _video->packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    av_init_packet(_video->packet);
    //av_init_packet(&_video->packet);

	// Allocate the OpenCV video matrix
	//_cvMatrix.create(_video->codec->height, _video->codec->width, CV_8UC(3));
}


void AVFileReader::openAudio(int streamID)
{	
	Log("info") << "[AVFileReader:" << this << "] Opening Audio: " << streamID << endl;

	assert(_formatCtx);
	assert(streamID >= 0);
	
	_audio = new AudioContext();
    _audio->stream = _formatCtx->streams[streamID];
    _audio->codec = _audio->stream->codec;
    _audio->codec = avcodec_find_decoder (_audio->codec->codec_id);
    if (_audio->codec == NULL)
		throw Exception("The audio codec is missing or unsupported.");

	_audio->populate();

    if (avcodec_open(_audio->codec, _audio->codec) < 0)
		throw Exception("Could not open the video codec.");

    switch (_audio->codec->sample_fmt) {
    case SAMPLE_FMT_S16:
        _audio->width = 16;
        _audio->fp = false;
        break;
    case SAMPLE_FMT_S32:
        _audio->width = 32;
        _audio->fp = false;
        break;
    case SAMPLE_FMT_FLT:
        _audio->width = 32;
        _audio->fp = true;
        break;
    case SAMPLE_FMT_DBL:
        _audio->width = 64;
        _audio->fp = true;
        break;
    default:
		throw Exception("Unsupported audio sample format.");
    }

    _audio->bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE; // * 3) / 2;
    _audio->buffer = (uint8_t*)av_malloc(_audio->bufferSize);
    //_audio->packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(_audio->packet);
    //_audio->offset = 0;
    //return codec;
}


double AVFileReader::fps() 
{
	return _video->codec ? 
		 _video->codec ->time_base.den / _video->codec->time_base.num : 0.0;
}

double AVFileReader::duration() 
{
	return _video->stream ? 
		((double)_video->stream->duration) * av_q2d(_video->stream->time_base) : 0.0;
}
*/


/*
int AVFileReader::readNextFrame()
{	
    AVPacket* packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(_audio->packet);

    while ((res = av_read_frame(_formatCtx, packet)) >= 0) {
        if (_video->packet.stream_index == _video->stream->id) {
            break;
        }
		Log("trace") << "[AVFileReader:" << this << "] Dropping frame from other video stream." << endl;
        av_free_packet(&_video->packet);
    }
    if (res < 0) {
        // End of file or error.
		Log("info") << "[AVFileReader:" << this << "] Decoding Video: EOF" << endl;
        return 0;
    }
}


int AVFileReader::decodeAudio(AVPacket& packet) //AudioPacket& packet
{
	int bufferSize;
    int len;
    //int res;

    if (!_audio || !_audio->error.empty()) {
		Log("warning") << "[AVFileReader:" << this << "] Decoding Audio: Not initialized." << endl;
		return -1;
    }

    //for (;;) {
    while (packet.size > 0) {

		_audio->packet = &packet;
        bufferSize = _audio->bufferSize;
        len = avcodec_decode_audio3(_audio->codec, (int16_t*)_audio->buffer, &bufferSize, _audio->packet);
        if (len < 0) {
            // Error, skip the frame.
            _audio->packet->size = 0;			
			Log("error") << "[AVFileReader:" << this << "] Decoding Audio: Frame size: " << len << endl;
            break;
        }
    while (packet.size > 0) {
        _audio->packet->data += len;
        _audio->packet->size -= len;
        _audio->offset += len;
        if (bufferSize <= 0) {
            // No data yet, get more frames
			Log("info") << "[AVFileReader:" << this << "] Decoding Audio: Decode beffer returned empty." << endl;
            continue;
        }

        // We have data, return it and come back for more later
		//_audio->packet->data = _audio->packet->data;
		//_audio->packet->size = _audio->packet->size;
		
		Log("trace") << "[AVFileReader:" << this << "] Decoding Audio:"		
			<< "\n\tPacket Size: " << _audio->packet->size
			<< "\n\tPacket Ptr: " << _audio->packet->data
			<< "\n\tRead Len: " << len
			<< "\n\tPacket Offset: " << _audio->offset
			<< "\n\tBuffer Size: " << bufferSize
			//<< "\n\tPTS: " << pts //_video->_audio->packet->pts
			//<< "\n\tDuration: " << _video->stream->duration
			//<< "\n\tCurrent: " << _video->_audio->packet->dts
			<< endl;

        return _audio->offset; //bufferSize; //_audio->packet->size;
    }
	
	Log("error") << "[AVFileReader:" << this << "] Decoding Audio:  Unable to read frame" << endl;

    if (_audio->packet->data) {
        _audio->packet->data -= _audio->offset;
        _audio->packet->size += _audio->offset;
        _audio->offset = 0;
        av_free__audio->packet(&_audio->packet);
    }

	return 0;
}


int AVFileReader::decodeVideo(AVPacket& packet) //VideoPacket& packet
{	
	//int bufferSize;
    // int res;
    //int len;

    if (!_video || !_video->error.empty()) {
		Log("warning") << "[AVFileReader:" << this << "] Decoding Video: Not initialized." << endl;
		return -1;
    }
	
	_video->packet = &packet;
	int frameDecoded = -1;
	int len = avcodec_decode_video2(_video->codec, _video->iframe, &frameDecoded, _video->packet);
	if (len < 0) {
        // Error, skip the frame.
       _video->packet->size = 0;			
		Log("error") << "[AVFileReader:" << this << "] Decoding Video: Frame size: " << len << endl;
		return 0;
    }
		
	// If frameDecoded == 0, then no frame was produced.
	if (frameDecoded == 0) {
		Log("warning") << "[AVFileReader:" << this << "] Decoding Video: No frame was encoded." << endl;
		return 0;
	}
		
	// Convert the image from its native format to BGR.
	if (_video->convertCtx == NULL)
		_video->convertCtx = sws_getContext(
			_video->codec->width, _video->codec->height, _video->codec->pix_fmt, 
			_video->codec->width, _video->codec->height, PIX_FMT_BGR24, 
			SWS_BICUBIC, NULL, NULL, NULL);
	if (_video->convertCtx == NULL)
		throw Exception("Cannot initialize the conversion context!");	
			
	// Scales the data in src according to our settings in our SwsContext.
	sws_scale(
		_video->convertCtx, _video->iframe->data, _video->iframe->linesize, 
		0, _video->codec->height, _video->oframe->data, _video->oframe->linesize);

	return len;
};
*/


	/*
int AVFileReader::populateMatrix(cv::Mat& mat)
{
    if (!_video || !_video->error.empty()) {
		Log("warning") << "[AVFileReader:" << this << "] Decoding Video: Not initialized." << endl;
		return -1;
    }
	
	// Convert the image from its native format to BGR.
	if (_video->convertCtx == NULL)
		_video->convertCtx = sws_getContext(
			_video->codec->width, _video->codec->height, _video->codec->pix_fmt, 
			_video->codec->width, _video->codec->height, PIX_FMT_BGR24, 
			SWS_BICUBIC, NULL, NULL, NULL);
	if (_video->convertCtx == NULL)
		throw Exception("Cannot initialize the conversion context!");	
			
	// Scales the data in src according to our settings in our SwsContext.
	if (sws_scale(convCtx,
		iframe->data, iframe->linesize, 0, height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported.");

	sws_scale(
		_video->convertCtx, _video->iframe->data, _video->iframe->linesize, 
		0, _video->codec->height, _video->oframe->data, _video->oframe->linesize);

	// Populate the OpenCV Matrix.
	// TODO: Make condiditonal.
	for (int y = 0; y < _video->codec->height; y++) {
        for (int x = 0; x < _video->codec->width; x++) {
			mat.at<cv::Vec3b>(y,x)[0]=_video->oframe->data[0][y * _video->oframe->linesize[0] + x * 3 + 0];
			mat.at<cv::Vec3b>(y,x)[1]=_video->oframe->data[0][y * _video->oframe->linesize[0] + x * 3 + 1];
			mat.at<cv::Vec3b>(y,x)[2]=_video->oframe->data[0][y * _video->oframe->linesize[0] + x * 3 + 2];
		}
	}
}
		*/


	/*
	*/

		/*
    //for (;;) {
        while (_audio->packet->size > 0) {
            bufferSize = _audio->bufferSize;
            len = avcodec_decode_audio3(_audio->codec, (int16_t*)_audio->buffer, &bufferSize, _audio->packet);
            if (len < 0) {
                // Error, skip the frame.
                _audio->packet->size = 0;			
				Log("error") << "[AVFileReader:" << this << "] Decoding Audio: Frame size: " << len << endl;
                break;
            }
            _audio->packet->data += len;
            _audio->packet->size -= len;
            _audio->offset += len;
            if (bufferSize <= 0) {
                // No data yet, get more frames
				Log("info") << "[AVFileReader:" << this << "] Decoding Audio: Decode beffer returned empty." << endl;
                continue;
            }

            // We have data, return it and come back for more later
			packet->data = _audio->packet->data;
			packet->size = _audio->packet->size;
			
			Log("trace") << "[AVFileReader:" << this << "] Decoding Audio:"		
				<< "\n\tPacket Size: " << packet->size
				<< "\n\tPacket Offset: " << _audio->offset
				<< "\n\tBuffer Size: " << bufferSize
				//<< "\n\tPTS: " << pts //_video->packet->pts
				//<< "\n\tDuration: " << _video->stream->duration
				//<< "\n\tCurrent: " << _video->packet->dts
				<< endl;

            return bufferSize;
        }
        if (_audio->packet->data) {
            _audio->packet->data -= _audio->offset;
            _audio->packet->size += _audio->offset;
            _audio->offset = 0;
            av_free_packet(_audio->packet);
        }
        while ((res = av_read_frame(_formatCtx, _audio->packet)) >= 0) {
            if (_audio->packet->stream_index == _audio->stream->index) {
				break;
            }
			Log("trace") << "[AVFileReader:" << this << "] Dropping frame from other audio stream:"		
				<< "\n\tWanted: " << _audio->stream->index
				<< "\n\tGot: " << _audio->packet->stream_index
				<< endl;
            av_free_packet(_audio->packet);
        }
        if (res < 0) {
            // End of file or error.
			Log("info") << "[AVFileReader:" << this << "] Decoding Audio: EOF" << endl;
            return 0;
        }
		*/
    //}
	


	/*
    _audio->codec_name = g_strdup (_audio->codec->long_name);
    _audio->bit_rate = _audio->codec->bit_rate;
    _audio->sample_rate = _audio->codec->sample_rate;
    _audio->bits_per_sample = _audio->codec->bits_per_raw_sample;
    if (!_audio->bits_per_sample) {
         _audio->bits_per_sample = _audio->codec->bits_per_coded_sample;
    }
    _audio->channels = _audio->codec->channels;
    _audio->duration = _audio->stream->duration * av_q2d (_audio->stream->time_base);
    if (_audio->channels <= 0) {
            _audio->error = _("No audio channels");
            return codec;
    }
	
		*/
	/*
	// Return the next frame of a stream. The information is stored as a _video->packet in pkt.
	if (av_read_frame(_formatCtx, &_video->packet) < 0)
		throw Exception("Could not Read Frame!");
	
	// Check if the _video->packet belonged to the video stream.
	if (_video->packet.stream_index != _video->stream->id) 
		return false;
		*/


	/*
	bool videoOK = true;
	bool audioOK = true;
	while (!_stop && (videoOK && audioOK)) 
	{
		if (videoOK) {
			try 
			{	
				VideoPacket video;
				if (decodeVideo(video)) {
					//Log("trace") << "[AVFileReader:" << this << "] Broadcasting: " << video.className() << endl;
					broadcast(video);
				}
			} 
			catch (Exception& exc) 
			{
				videoOK = false;
				Log("error") << "[AVFileReader:" << this << "] Video Error: " << exc.displayText() << endl;
			}
		}

		if (audioOK) {
		}
		
		//cv::waitKey(10);
	}
	*/
	
	
	/*
	// Set the _video->packet's data pointer to our BGR image.
	_video->packet.data = _video->oframe->data[0];
					
	// Set the _video->packet's OpenCV image pointer to our image.
	_video->packet.mat = &_cvMatrix;
	*/
	/*
		
	//double pts = (_video->packet.dts != AV_NOPTS_VALUE) ? _video->packet.dts : 0.0;
	//pts *= av_q2d(_video->stream->time_base);
	//_video->packet.time = pts;
	Log("trace") << "[AVFileReader:" << this << "] Decoding Video:"		
		<< "\n\tGot Ptr: " << frameDecoded
		<< "\n\tSize: " << _video->packet.size
		//<< "\n\tFPS: " << fps
		<< "\n\tPTS: " << pts //_video->packet.pts
		<< "\n\tDuration: " << _video->stream->duration
		<< "\n\tCurrent: " << _video->packet.dts
		<< endl;
		*/
	// Throw an exception if we can't decode a video frame and exit early.
	//if (len < 0)
	//	throw Exception("Failed to decode video frame.");