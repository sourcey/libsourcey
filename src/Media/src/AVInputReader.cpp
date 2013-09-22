#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Logger.h"
//#include "Poco/Format.h"


using namespace std;
using namespace scy;


namespace scy {
namespace av {


AVInputReader::AVInputReader(const Options& options)  : 
	_thread(),
	_options(options),
	_formatCtx(nullptr),
	_video(nullptr),
	_audio(nullptr),
	_stopping(false)
{		
	traceL("AVInputReader", this) << "Create" << endl;
}


AVInputReader::~AVInputReader() 
{
	traceL("AVInputReader", this) << "Destroy" << endl;

	close();
}


void AVInputReader::openFile(const std::string& file)
{
	traceL("AVInputReader", this) << "Opening: " << file << endl;	
	openStream(file.c_str(), nullptr, nullptr);
}


#ifdef LIBAVDEVICE_VERSION
void AVInputReader::openDevice(int deviceID, int width, int height, double framerate)
{
	std::string device;

#ifdef WIN32
	// Only vfwcap supports index based input, 
	// dshow requires full device name.
	// TODO: Determine device name for for given 
	// index using DeviceManager.
	if (_options.deviceEngine != "vfwcap")
		throw "Cannot open index based device";
		
	// Video capture on windows only through 
	// Video For Windows driver
	device = Poco::format("%d", deviceID);
#else
	if (_options.deviceEngine == "dv1394") {
		device = Poco::format("/dev/dv1394/%d", deviceID);
	} 
	else {
		device = Poco::format("/dev/video%d", deviceID);
	}
#endif

	openDevice(device, width, height, framerate);
}


void AVInputReader::openDevice(const std::string& device, int width, int height, double framerate) //int deviceID, 
{        
	traceL("AVInputReader", this) << "Opening Device: " << device << endl;	

	avdevice_register_all();

	AVInputFormat* iformat;
	AVDictionary*  iparams = nullptr;
        
#ifdef WIN32
    iformat = av_find_input_format(_options.deviceEngine.c_str());
#else
	if (_options.deviceEngine == "dv1394") {
        iformat = av_find_input_format("dv1394");
	} 
	else {
		const char* formats[] = {"video4linux2,v4l2", "video4linux2", "video4linux"};
		int i, formatsCount = sizeof(formats) / sizeof(char*);
		for (i = 0; i < formatsCount; i++) {
			iformat = av_find_input_format(formats[i]);
			if (iformat)
				break;
		}
	}	
#endif
	
    if (!iformat)
		throw std::runtime_error("Couldn't find input format.");
	
	// frame rate
	if (framerate)
		av_dict_set(&iparams, "framerate", Poco::format("%f", framerate).c_str(), 0);
	
	// video size
	if (width && height)
		av_dict_set(&iparams, "video_size", Poco::format("%dx%d", width, height).c_str(), 0);
	
	// video standard
	if (!_options.deviceStandard.empty())
		av_dict_set(&iparams, "standard", _options.deviceStandard.c_str(), 0);

	openStream(device.c_str(), iformat, &iparams);
	
	// for video capture it is important to do non blocking read
	//_formatCtx->flags |= AVFMT_FLAG_NONBLOCK;

	av_dict_free(&iparams);
}
#endif


void AVInputReader::openStream(const char* filename, AVInputFormat* inputFormat, AVDictionary** formatParams)
{
	traceL("AVInputReader", this) << "Opening Stream: " << string(filename) << endl;

	av_register_all();

	if (avformat_open_input(&_formatCtx, filename, inputFormat, formatParams) != 0)
		throw std::runtime_error("Could not open the media source.");

	if (av_find_stream_info(_formatCtx) < 0)
		throw std::runtime_error("Could not find stream information.");
	
  	av_dump_format(_formatCtx, 0, filename, 0);
	
	for (unsigned i = 0; i < _formatCtx->nb_streams; i++) {
		if (_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && 
			_video == nullptr && !_options.disableVideo) {
			_video = new VideoDecoderContext();
			_video->create(_formatCtx, i);
			_video->open();
		}
		else if (_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO &&
			_audio == nullptr && !_options.disableAudio) {
			_audio = new AudioDecoderContext();
			_audio->create(_formatCtx, i);
			_audio->open();
		}
	}

	if (_video == nullptr && 
		_audio == nullptr)
		throw std::runtime_error("Could not find a valid media stream.");
}


void AVInputReader::close()
{
	traceL("AVInputReader", this) << "Closing" << endl;

	if (_video) {
		delete _video;
		_video = nullptr;
	}

	if (_audio) {
		delete _audio;
		_audio = nullptr;
	}

	if (_formatCtx) {
  		av_close_input_file(_formatCtx);
		_formatCtx = nullptr;
  	}

	traceL("AVInputReader", this) << "Closing: OK" << endl;
}


void AVInputReader::start() 
{
	traceL("AVInputReader", this) << "Starting" << endl;

	Mutex::ScopedLock lock(_mutex);
	assert(_video || _audio);

	if (_video || _audio &&
		!_thread.running()) {
		traceL("AVInputReader", this) << "Initializing Thread" << endl;
		_stopping = false;
		_thread.start(*this);
	}

	traceL("AVInputReader", this) << "Starting: OK" << endl;
}


void AVInputReader::stop() 
{
	traceL("AVInputReader", this) << "Stopping" << endl;

	//Mutex::ScopedLock lock(_mutex);	

	if (_thread.running()) {
		traceL("AVInputReader", this) << "Terminating Thread" << endl;		
		_stopping = true;
		_thread.join();
	}

	traceL("AVInputReader", this) << "Stopping: OK" << endl;
}


void AVInputReader::run() 
{
	traceL("AVInputReader", this) << "Running" << endl;
	
	try 
	{
		int res;
		int videoFrames = 0;
		int audioFrames = 0;
		while (!_stopping) {
			
			AVPacket ipacket;
			AVPacket opacket;
			av_init_packet(&ipacket);

			while ((res = av_read_frame(_formatCtx, &ipacket)) >= 0) {
				if (_video && ipacket.stream_index == _video->stream->index) {
					if ((!_options.processVideoXFrame || (videoFrames % _options.processVideoXFrame) == 0) &&
						(!_options.processVideoXSecs || !_video->pts || ((ipacket.pts * av_q2d(_video->stream->time_base)) - _video->pts) > _options.processVideoXSecs) &&
						(!_options.iFramesOnly || (ipacket.flags & AV_PKT_FLAG_KEY))) {
 						if (_video->decode(ipacket, opacket)) {
							//traceL("AVInputReader", this) << "Decoded video: " << _video->pts << endl;
							VideoPacket video((char*)opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
							video.source = &opacket;
							emit(this, video);
						}
					}
					//else
					//	traceL("AVInputReader", this) << "Skipping Video Frame: " << videoFrames << endl;
					videoFrames++;
				}
				else if (_audio && ipacket.stream_index == _audio->stream->index) {	
					if ((!_options.processAudioXFrame || (audioFrames % _options.processAudioXFrame) == 0) &&
						(!_options.processAudioXSecs || !_audio->pts || ((ipacket.pts * av_q2d(_audio->stream->time_base)) - _audio->pts) > _options.processAudioXSecs)) {
						if (_audio->decode(ipacket, opacket)) {			
							//traceL("AVInputReader", this) << "Decoded Audio: " << _audio->pts << endl;
							AudioPacket audio((char*)opacket.data, opacket.size, _audio->pts);
							audio.source = &opacket;
							emit(this, audio);
						}	
					}
					//else
					//	traceL("AVInputReader", this) << "Skipping Audio Frame: " << audioFrames << endl;
					audioFrames++;

					/*
					while (ipacket.size > 0) {
						if ((len = _audio->decode(ipacket)) <= 0)
							break;

						//ipacket.data += len;
						//ipacket.size -= len;
						
						//traceL("AVInputReader", this) << "Broadcasting Audio: " << _video->pts << endl;
						AudioPacket audio(_audio->buffer, len, _audio->pts);
						audio.opaque = &ipacket; //_audio;
						emit(this, audio);
					}
					break;
					*/
				} 
				/*
				else {
					warnL() << "[AVInputReader: " << this << "] Dropping frame from unknown stream:"		
						<< "\n\tStream ID: " << ipacket.stream_index
						<< "\n\tVideo ID: " << _video->stream->index
						<< "\n\tAudio ID: " << _audio->stream->index
						<< endl;
				}
				*/

				av_free_packet(&ipacket);
			}

			if (res < 0) {
				bool gotFrame = false;
				
				// Flush video
				while (_video && true) {
					AVPacket opacket;
					gotFrame = _video->flush(opacket);
					if (gotFrame) {
						VideoPacket video((char*)opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
						video.source = &opacket;
						emit(this, video);
					} 					
					av_free_packet(&opacket);
					if (!gotFrame)
						break;
				}
				
				// Flush audio
				while (_audio && true) {
					AVPacket opacket;
					gotFrame = _audio->flush(opacket);
					if (gotFrame) {
						AudioPacket audio((char*)opacket.data, opacket.size, _audio->pts);
						audio.source = &opacket;
						emit(this, audio);
					}					
					av_free_packet(&opacket);
					if (!gotFrame)
						break;
				}

				// End of file or error.
				traceL() << "[AVInputReader: " << this << "] Decoding: EOF" << endl;
				break;
			}

			scy::sleep(10);
		};
	} 
	catch (std::exception& exc) 
	{
		_error = exc.what();
		errorL() << "[AVInputReader: " << this << "] Decoder Error: " << _error << endl;
	}
	catch (...) 
	{
		_error = "Unknown Error";
		errorL() << "[AVInputReader: " << this << "] Unknown Error" << endl;
	}

	traceL("AVInputReader", this) << "Exiting" << endl;
	ReadComplete.emit(this);
}


AVInputReader::Options& AVInputReader::options()
{ 
	Mutex::ScopedLock lock(_mutex);
	return _options; 
}
	

AVFormatContext* AVInputReader::formatCtx() const
{
	Mutex::ScopedLock lock(_mutex);	
	return _formatCtx;
}
	

VideoDecoderContext* AVInputReader::video() const
{
	Mutex::ScopedLock lock(_mutex);	
	return _video;
}
	

AudioDecoderContext* AVInputReader::audio() const
{
	Mutex::ScopedLock lock(_mutex);	
	return _audio;
}


string AVInputReader::error() const
{
	Mutex::ScopedLock lock(_mutex);	
	return _error;
}


} } // namespace scy::av