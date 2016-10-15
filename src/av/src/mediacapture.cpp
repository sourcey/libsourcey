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


#include "scy/av/mediacapture.h"

#ifdef HAVE_FFMPEG

#include "scy/platform.h"
#include "scy/logger.h"
#include "scy/av/devicemanager.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}


using std::endl;


namespace scy {
namespace av {


MediaCapture::MediaCapture() :
    _formatCtx(nullptr),
    _video(nullptr),
    _audio(nullptr),
    _stopping(false)
{
    TraceS(this) << "Create" << endl;
    initializeFFmpeg();
}


MediaCapture::~MediaCapture()
{
    TraceS(this) << "Destroy" << endl;

    close();
    uninitializeFFmpeg();
}


void MediaCapture::close()
{
    TraceS(this) << "Closing" << endl;

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

    TraceS(this) << "Closing: OK" << endl;
}


void MediaCapture::openFile(const std::string& file)
{
    TraceS(this) << "Opening file: " << file << endl;
    openStream(file, nullptr, nullptr);
}


// // #ifdef HAVE_FFMPEG_AVDEVICE
//
// void MediaCapture::openCamera(const std::string& device, int width, int height, double fps)
// {
//     TraceS(this) << "Opening camera: " << device << endl;
//
//     auto iformat = DeviceManager::instance().findCameraInputFormat();
//     if (!iformat)
//         throw std::runtime_error("Couldn't find camera input format.");
//
//     AVDictionary* iparams = nullptr;
//     if (width > 0 && height > 0)
//         av_dict_set(&iparams, "video_size", util::format("%dx%d", width, height).c_str(), 0);
//     if (fps > 0)
//         av_dict_set(&iparams, "framerate", util::format("%f", fps).c_str(), 0);
//
//     openStream(device.c_str(), iformat, &iparams);
//
//     av_dict_free(&iparams); // FIXME: possible memory leak
// }
//
//
// void MediaCapture::openMicrophone(const std::string& device, int channels, int sampleRate)
// {
//     TraceS(this) << "Opening microphone: " << device << endl;
//
//     auto iformat = DeviceManager::instance().findMicrophoneInputFormat();
//     if (!iformat)
//         throw std::runtime_error("Couldn't find microphone input format.");
//
//     AVDictionary* iparams = nullptr;
//     if (sampleRate > 0)
//         av_dict_set_int(&iparams, "sample_rate", sampleRate, 0);
//     if (channels > 0)
//         av_dict_set_int(&iparams, "channels", channels, 0);
//
//     openStream(device.c_str(), iformat, &iparams);
//
//     av_dict_free(&iparams); // FIXME: possible memory leak
// }
//
// // #endif


void MediaCapture::openStream(const std::string& filename, AVInputFormat* inputFormat, AVDictionary** formatParams)
{
    TraceS(this) << "Opening stream: " << filename << endl;

    if (_formatCtx)
        throw std::runtime_error("Capture has already been initialized");

    if (avformat_open_input(&_formatCtx, filename.c_str(), inputFormat, formatParams) < 0)
        throw std::runtime_error("Cannot open the media source: " + filename);

    if (avformat_find_stream_info(_formatCtx, nullptr) < 0)
        throw std::runtime_error("Cannot find stream information: " + filename);

    av_dump_format(_formatCtx, 0, filename.c_str(), 0);

    for (unsigned i = 0; i < _formatCtx->nb_streams; i++) {
        auto codec = _formatCtx->streams[i]->codec;
        if (!_video && codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            _video = new VideoDecoderContext();
            _video->create(_formatCtx, i);
            _video->open();
        }
        else if (!_audio && codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            _audio = new AudioDecoderContext();
            _audio->create(_formatCtx, i);
            _audio->open();
        }
    }

    if (!_video && !_audio)
        throw std::runtime_error("Cannot find a valid media stream: " + filename);
}


void MediaCapture::start()
{
    TraceS(this) << "Starting" << endl;

    Mutex::ScopedLock lock(_mutex);
    assert(_video || _audio);

    if ((_video || _audio) && !_thread.running()) {
        TraceS(this) << "Initializing Thread" << endl;
        _stopping = false;
        _thread.start(*this);
    }

    TraceS(this) << "Starting: OK" << endl;
}


void MediaCapture::stop()
{
    TraceS(this) << "Stopping" << endl;

    // Mutex::ScopedLock lock(_mutex);

    _stopping = true;
    if (_thread.running()) {
        TraceS(this) << "Terminating Thread" << endl;
        _thread.join();
    }

    TraceS(this) << "Stopping: OK" << endl;
}


void MediaCapture::run()
{
    TraceS(this) << "Running" << endl;

    try {
        int res;
        std::int64_t videoFrames = 0;
        std::int64_t audioFrames = 0;
        AVPacket ipacket;
        AVPacket opacket;
        av_init_packet(&ipacket); // TODO: ensure always uninitialized

        while ((res = av_read_frame(_formatCtx, &ipacket)) >= 0) {
            TraceS(this) << "Read frame: pts=" << ipacket.pts << ": dts=" << ipacket.dts << endl;
            if (_stopping) break;
            if (_video && ipacket.stream_index == _video->stream->index) {
                if (_video->decode(ipacket, opacket)) {
                    TraceS(this) << "Decoded video: " << _video->pts << endl;
                    VideoPacket video(opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
                    video.iframe = ipacket.flags & AV_PKT_FLAG_KEY;
                    video.source = &opacket;
                    video.opaque = _video;
                    emit(video);
                }
                videoFrames++;
            }
            else if (_audio && ipacket.stream_index == _audio->stream->index) {
                if (_audio->decode(ipacket, opacket)) {
                    TraceS(this) << "Decoded Audio: " << _audio->pts << endl;
                    AudioPacket audio(_audio->frame->data[0], opacket.size, _audio->frame->nb_samples, _audio->pts);
                    audio.source = &opacket;
                    audio.opaque = _audio;
                    emit(audio);
                }
                audioFrames++;
            }

            av_packet_unref(&ipacket);
        }

        if (!_stopping && res < 0) {
            bool gotFrame = false;

            // Flush video
            while (_video && true) {
                AVPacket opacket;
                gotFrame = _video->flush(opacket);
                if (gotFrame) {
                    VideoPacket video(opacket.data, opacket.size, _video->ctx->width, _video->ctx->height, _video->pts);
                    video.source = &opacket;
                    video.opaque = _video;
                    emit(video);
                }
                av_packet_unref(&opacket);
                if (!gotFrame)
                    break;
            }

            // Flush audio
            while (_audio && true) {
                AVPacket opacket;
                gotFrame = _audio->flush(opacket);
                if (gotFrame) {
                    AudioPacket audio(opacket.data, opacket.size, _audio->frame->nb_samples, _audio->pts);
                    audio.source = &opacket;
                    audio.opaque = _audio;
                    emit(audio);
                }
                av_packet_unref(&opacket);
                if (!gotFrame)
                    break;
            }

            // End of file or error.
            TraceS(this) << "Decoding: EOF" << endl;
        }
    }
    catch (std::exception& exc) {
        _error = exc.what();
        ErrorS(this) << "Decoder Error: " << _error << endl;
    }
    catch (...) {
        _error = "Unknown Error";
        ErrorS(this) << "Unknown Error" << endl;
    }

    TraceS(this) << "Exiting" << endl;
    Closing.emit(this);
}


void MediaCapture::getEncoderFormat(Format& iformat)
{
    iformat.name = "Capture";
    getVideoCodec(iformat.video);
    getAudioCodec(iformat.audio);
}


void MediaCapture::getAudioCodec(AudioCodec& iparams)
{
    Mutex::ScopedLock lock(_mutex);

    for (unsigned i = 0; i < _formatCtx->nb_streams; i++) {
        auto codec = _formatCtx->streams[i]->codec;
        if (codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            assert(_audio);
            initAudioCodecFromContext(codec, iparams);
        }
    }
}


void MediaCapture::getVideoCodec(VideoCodec& iparams)
{
    Mutex::ScopedLock lock(_mutex);

    for (unsigned i = 0; i < _formatCtx->nb_streams; i++) {
        auto codec = _formatCtx->streams[i]->codec;
        if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            assert(_video);
            initVideoCodecFromContext(codec, iparams);
        }
    }
}


AVFormatContext* MediaCapture::formatCtx() const
{
    Mutex::ScopedLock lock(_mutex);
    return _formatCtx;
}


VideoDecoderContext* MediaCapture::video() const
{
    Mutex::ScopedLock lock(_mutex);
    return _video;
}


AudioDecoderContext* MediaCapture::audio() const
{
    Mutex::ScopedLock lock(_mutex);
    return _audio;
}


std::string MediaCapture::error() const
{
    Mutex::ScopedLock lock(_mutex);
    return _error;
}


} } // namespace scy::av


#endif
