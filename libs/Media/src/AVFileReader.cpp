#include "Sourcey/Media/AVFileReader.h"
#include "Sourcey/Logger.h"


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
	Log("trace") << "[AVFileReader:" << this << "] Creating" << endl;
}


AVFileReader::~AVFileReader() 
{
	Log("trace") << "[AVFileReader:" << this << "] Destroying" << endl;

	close();
}


void AVFileReader::open(const string& ifile)
{
	Log("trace") << "[AVFileReader:" << this << "] Opening: " << ifile << endl;

	av_register_all();

	_ifile = ifile;
		
	if (avformat_open_input(&_formatCtx, ifile.data(), NULL, NULL) != 0)
		throw Exception("Could not open the source media file.");

	if (av_find_stream_info(_formatCtx) < 0)
		throw Exception("Could not find stream information.");
	
  	av_dump_format(_formatCtx, 0, ifile.data(), 0);
	
	for (int i = 0; i < _formatCtx->nb_streams; i++) {
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


void AVFileReader::close()
{
	Log("trace") << "[AVFileReader:" << this << "] Closing" << endl;

	if (_video) {
		delete _video;
		_video = NULL;
	}

	if (_audio) {
		delete _audio;
		_audio = NULL;
	}

	if (_formatCtx) {
  		av_close_input_file(_formatCtx);
		_formatCtx = NULL;
  	}

	Log("trace") << "[AVFileReader:" << this << "] Closing: OK" << endl;
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


void AVFileReader::run() 
{
	Log("trace") << "[AVFileReader:" << this << "] Running: " << _ifile << endl;
	
	try 
	{
		int len;
		int res;
		while (!_stop) {

			AVPacket packet;
			av_init_packet(&packet);

			while ((res = av_read_frame(_formatCtx, &packet)) >= 0) {
				if (_video && packet.stream_index == _video->stream->index) {					
					while (packet.size > 0) {
						if ((len = _video->decode(packet)) <= 0)
							break;

						//packet.data += len;
						//packet.size -= len;
						
						//Log("trace") << "[AVFileReader:" << this << "] Broadcasting Video: " << _video->pts << endl;
						VideoPacket video(_video->buffer, len, _video->codec->width, _video->codec->height, _video->pts);
						video.opaque = &packet; //_video;
						dispatch(this, video);
					}
					break;
				}
				else if (_audio && packet.stream_index == _audio->stream->index) {		
					while (packet.size > 0) {
						if ((len = _audio->decode(packet)) <= 0)
							break;

						//packet.data += len;
						//packet.size -= len;
						
						//Log("trace") << "[AVFileReader:" << this << "] Broadcasting Audio: " << _video->pts << endl;
						AudioPacket audio(_audio->buffer, len, _audio->pts);
						audio.opaque = &packet; //_audio;
						dispatch(this, audio);
					}
					break;
				} 

				Log("warn") << "[AVFileReader:" << this << "] Dropping frame from unknown stream:"		
					<< "\n\tStream ID: " << packet.stream_index
					<< "\n\tVideo ID: " << _video->stream->index
					<< "\n\tAudio ID: " << _audio->stream->index
					<< endl;

				av_free_packet(&packet);
			}

			if (res < 0) {
				// End of file or error.
				Log("debug") << "[AVFileReader:" << this << "] Decoding: EOF" << endl;
				break;
			}

			Thread::sleep(10);
		};
	} 
	catch (Exception& exc) 
	{
		_error = exc.message();
		Log("error") << "[AVFileReader:" << this << "] Decoder Error: " << exc.message() << endl;
	}
	catch (...) 
	{
		_error = "Unknown Error";
		Log("error") << "[AVFileReader:" << this << "] Unknown Error" << endl;
	}

	Log("trace") << "[AVFileReader:" << this << "] Exiting" << endl;
	ReadComplete.dispatch(this);
}
	

AVFormatContext* AVFileReader::formatCtx() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _formatCtx;
}
	

VideoDecoderContext* AVFileReader::video() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _video;
}
	

AudioDecoderContext* AVFileReader::audio() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _audio;
}


string AVFileReader::error() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _error;
}


} } // namespace Sourcey::Media