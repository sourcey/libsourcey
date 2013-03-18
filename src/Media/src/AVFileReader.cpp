#include "Sourcey/Media/AVFileReader.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


AVFileReader::AVFileReader(const Options& options)  : 
	_thread("AVFileReader"),
	_options(options),
	_formatCtx(NULL),
	_video(NULL),
	_audio(NULL),
	_stop(false)
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
			_video == NULL && !_options.disableVideo) {
			_video = new VideoDecoderContext();
			_video->create(_formatCtx, i);
			_video->open();
		}
		else if (_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO &&
			_audio == NULL && !_options.disableAudio) {
			_audio = new AudioDecoderContext();
			_audio->create(_formatCtx, i);
			_audio->open();
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

	//FastMutex::ScopedLock lock(_mutex);	

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
		int res;
		int videoFrames = 0;
		int audioFrames = 0;
		while (!_stop) {
			
			AVPacket ipacket;
			AVPacket opacket;
			av_init_packet(&ipacket);

			while ((res = av_read_frame(_formatCtx, &ipacket)) >= 0) {
				if (_video && ipacket.stream_index == _video->stream->index) {
					if ((!_options.processVideoXFrame || (videoFrames % _options.processVideoXFrame) == 0) &&
						(!_options.processVideoXSecs || !_video->pts || ((ipacket.pts * av_q2d(_video->stream->time_base)) - _video->pts) > _options.processVideoXSecs) &&
						(!_options.iFramesOnly || (ipacket.flags & AV_PKT_FLAG_KEY))) {
 						if (_video->decode(ipacket, opacket)) {
							//Log("trace") << "[AVFileReader:" << this << "] Decoded Video: " << _video->pts << endl;
							VideoPacket video(opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
							video.opaque = &opacket;
							dispatch(this, video);
						}
					}
					//else
					//	Log("trace") << "[AVFileReader:" << this << "] Skipping Video Frame: " << videoFrames << endl;
					videoFrames++;
				}
				else if (_audio && ipacket.stream_index == _audio->stream->index) {	
					if ((!_options.processAudioXFrame || (audioFrames % _options.processAudioXFrame) == 0) &&
						(!_options.processAudioXSecs || !_audio->pts || ((ipacket.pts * av_q2d(_audio->stream->time_base)) - _audio->pts) > _options.processAudioXSecs) &&
						(!_options.iFramesOnly || (ipacket.flags & AV_PKT_FLAG_KEY))) {
						if (_audio->decode(ipacket, opacket)) {			
							//Log("trace") << "[AVFileReader:" << this << "] Decoded Audio: " << _audio->pts << endl;
							AudioPacket audio(opacket.data, opacket.size, _audio->pts);
							audio.opaque = &opacket;
							dispatch(this, audio);
						}	
					}
					//else
					//	Log("trace") << "[AVFileReader:" << this << "] Skipping Audio Frame: " << audioFrames << endl;
					audioFrames++;

					/*
					while (ipacket.size > 0) {
						if ((len = _audio->decode(ipacket)) <= 0)
							break;

						//ipacket.data += len;
						//ipacket.size -= len;
						
						//Log("trace") << "[AVFileReader:" << this << "] Broadcasting Audio: " << _video->pts << endl;
						AudioPacket audio(_audio->buffer, len, _audio->pts);
						audio.opaque = &ipacket; //_audio;
						dispatch(this, audio);
					}
					break;
					*/
				} 
				/*
				else {
					Log("warn") << "[AVFileReader:" << this << "] Dropping frame from unknown stream:"		
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
						VideoPacket video(opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
						video.opaque = &opacket;
						dispatch(this, video);
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
						AudioPacket audio(opacket.data, opacket.size, _audio->pts);
						audio.opaque = &opacket;
						dispatch(this, audio);
					}					
					av_free_packet(&opacket);
					if (!gotFrame)
						break;
				}

				// End of file or error.
				Log("debug") << "[AVFileReader:" << this << "] Decoding: EOF" << endl;
				break;
			}

			Thread::sleep(10);
		};
	} 
	catch (Exception& exc) 
	{
		_error = exc.displayText();
		Log("error") << "[AVFileReader:" << this << "] Decoder Error: " << _error << endl;
	}
	catch (...) 
	{
		_error = "Unknown Error";
		Log("error") << "[AVFileReader:" << this << "] Unknown Error" << endl;
	}

	Log("trace") << "[AVFileReader:" << this << "] Exiting" << endl;
	ReadComplete.dispatch(this);
}


AVFileReader::Options& AVFileReader::options()
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _options; 
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