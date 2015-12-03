//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "scy/media/avinputreader.h"

#ifdef HAVE_FFMPEG

#include "scy/platform.h"
#include "scy/logger.h"


using std::endl;


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
	TraceLS(this) << "Create" << endl;
	initializeFFmpeg();
}


AVInputReader::~AVInputReader()
{
	TraceLS(this) << "Destroy" << endl;

	close();
	uninitializeFFmpeg();
}


void AVInputReader::openFile(const std::string& file)
{
	TraceLS(this) << "Opening: " << file << endl;
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
	TraceLS(this) << "Opening Device: " << device << endl;

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
	TraceLS(this) << "Opening Stream: " << std::string(filename) << endl;

	if (avformat_open_input(&_formatCtx, filename, inputFormat, formatParams) != 0)
		throw std::runtime_error("Cannot open the media source: " + std::string(filename));

	if (avformat_find_stream_info(_formatCtx, NULL) < 0)
		throw std::runtime_error("Cannot find stream information: " + std::string(filename));

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
		throw std::runtime_error("Cannot find a valid media stream: " + std::string(filename));
}


void AVInputReader::close()
{
	TraceLS(this) << "Closing" << endl;

	if (_video) {
		delete _video;
		_video = nullptr;
	}

	if (_audio) {
		delete _audio;
		_audio = nullptr;
	}

	if (_formatCtx) {
		avformat_close_input(&_formatCtx);
	  _formatCtx = nullptr;
	}

	TraceLS(this) << "Closing: OK" << endl;
}


void AVInputReader::start()
{
	TraceLS(this) << "Starting" << endl;

	Mutex::ScopedLock lock(_mutex);
	assert(_video || _audio);

	if ((_video || _audio) && !_thread.running()) {
		TraceLS(this) << "Initializing Thread" << endl;
		_stopping = false;
		_thread.start(*this);
	}

	TraceLS(this) << "Starting: OK" << endl;
}


void AVInputReader::stop()
{
	TraceLS(this) << "Stopping" << endl;

	//Mutex::ScopedLock lock(_mutex);

	_stopping = true;
	if (_thread.running()) {
		TraceLS(this) << "Terminating Thread" << endl;
		_thread.join();
	}

	TraceLS(this) << "Stopping: OK" << endl;
}


void AVInputReader::run()
{
	TraceLS(this) << "Running" << endl;

	try {
		int res;
		int videoFrames = 0;
		int audioFrames = 0;
		AVPacket ipacket;
		AVPacket opacket;
		av_init_packet(&ipacket);

		while ((res = av_read_frame(_formatCtx, &ipacket)) >= 0) {
			TraceLS(this) << "Read video frame: " << _stopping << endl;
			if (_stopping) break;
			if (_video && ipacket.stream_index == _video->stream->index) {
				if ((!_options.processVideoXFrame || (videoFrames % _options.processVideoXFrame) == 0) &&
					(!_options.processVideoXSecs || !_video->pts || ((ipacket.pts * av_q2d(_video->stream->time_base)) - _video->pts) > _options.processVideoXSecs) &&
					(!_options.iFramesOnly || (ipacket.flags & AV_PKT_FLAG_KEY))) {
 					if (_video->decode(ipacket, opacket)) {
						//TraceLS(this) << "Decoded video: " << _video->pts << endl;
						VideoPacket video((char*)opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
						video.source = &opacket;
						emit(video);
					}
				}
				//else
				//	TraceLS(this) << "Skipping video frame: " << videoFrames << endl;
				videoFrames++;
			}
			else if (_audio && ipacket.stream_index == _audio->stream->index) {
				if ((!_options.processAudioXFrame || (audioFrames % _options.processAudioXFrame) == 0) &&
					(!_options.processAudioXSecs || !_audio->pts || ((ipacket.pts * av_q2d(_audio->stream->time_base)) - _audio->pts) > _options.processAudioXSecs)) {
					if (_audio->decode(ipacket, opacket)) {
						//TraceLS(this) << "Decoded Audio: " << _audio->pts << endl;
						AudioPacket audio((char*)opacket.data, opacket.size, _audio->pts);
						audio.source = &opacket;
						emit(audio);
					}
				}
				//else
				//	TraceLS(this) << "Skipping audio frame: " << audioFrames << endl;
				audioFrames++;
			}

			av_free_packet(&ipacket);
		}

		if (!_stopping && res < 0) {
			bool gotFrame = false;

			// Flush video
			while (_video && true) {
				AVPacket opacket;
				gotFrame = _video->flush(opacket);
				if (gotFrame) {
					VideoPacket video((char*)opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
					video.source = &opacket;
					emit(video);
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
					emit(audio);
				}
				av_free_packet(&opacket);
				if (!gotFrame)
					break;
			}

			// End of file or error.
			TraceLS(this) << "Decoding: EOF" << endl;
			//break;
		}

	}
	catch (std::exception& exc) {
		_error = exc.what();
		ErrorLS(this) << "Decoder Error: " << _error << endl;
	}
	catch (...) {
		_error = "Unknown Error";
		ErrorLS(this) << "Unknown Error" << endl;
	}

	TraceLS(this) << "Exiting" << endl;
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


std::string AVInputReader::error() const
{
	Mutex::ScopedLock lock(_mutex);
	return _error;
}


} } // namespace scy::av


#endif
