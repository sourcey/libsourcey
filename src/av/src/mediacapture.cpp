///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


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

    stop();

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
//     auto iformat = DeviceManager::instance().findVideoInputFormat();
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
//     auto iformat = DeviceManager::instance().findAudioInputFormat();
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
        auto stream = _formatCtx->streams[i];
        auto codec = stream->codec;
        if (!_video && codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            _video = new VideoDecoder(stream);
            _video->emitter.attach(packetSlot(this, &MediaCapture::emit)); // proxy packets
            _video->create();
            _video->open();
        }
        else if (!_audio && codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            _audio = new AudioDecoder(stream);
            _audio->emitter.attach(packetSlot(this, &MediaCapture::emit)); // proxy packets
            _audio->create();
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

    Mutex::ScopedLock lock(_mutex);

    _stopping = true;
    if (_thread.running()) {
        TraceS(this) << "Terminating Thread" << endl;
        _thread.join();
    }

    TraceS(this) << "Stopping: OK" << endl;
}


void MediaCapture::emit(IPacket& packet)
{
    TraceS(this) << "Emit: " << packet.size() << endl;

    emitter.emit(/*this, */packet);
}


void MediaCapture::run()
{
    TraceS(this) << "Running" << endl;

    try {
        int res;
        AVPacket ipacket;
        av_init_packet(&ipacket);
        while ((res = av_read_frame(_formatCtx, &ipacket)) >= 0) {
            TraceS(this) << "Read frame: pts=" << ipacket.pts << ", dts=" << ipacket.dts << endl;
            if (_stopping) break;
            if (_video && ipacket.stream_index == _video->stream->index) {
                if (_video->decode(ipacket)) {
                    TraceS(this) << "Decoded video: "
                        << "time=" << _video->time //<< ", "
                        // << "pts=" << opacket.pts << ", "
                        // << "dts=" << opacket.dts
                        << endl;
                }
            }
            else if (_audio && ipacket.stream_index == _audio->stream->index) {
                if (_audio->decode(ipacket)) { //, opacket
                    TraceS(this) << "Decoded Audio: "
                        << "time=" << _audio->time //<< ", "
                        // << "pts=" << opacket.pts << ", "
                        // << "dts=" << opacket.dts
                        << endl;
                }
            }

            av_packet_unref(&ipacket);
        }

        if (!_stopping && res < 0) {

            // Flush video
            if (_video) {
                _video->flush();
            }

            // Flush audio
            if (_audio) {
                _audio->flush();
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
    _stopping = true;
    Closing.emit(/*this*/);
}


void MediaCapture::getEncoderFormat(Format& format)
{
    format.name = "Capture";
    getEncoderVideoCodec(format.video);
    getEncoderAudioCodec(format.audio);
}


void MediaCapture::getEncoderAudioCodec(AudioCodec& params)
{
    Mutex::ScopedLock lock(_mutex);
    if (_audio) {
        params = _audio->oparams;
    }
}


void MediaCapture::getEncoderVideoCodec(VideoCodec& params)
{
    Mutex::ScopedLock lock(_mutex);
    if (_video) {
        params = _video->oparams;
    }
}


AVFormatContext* MediaCapture::formatCtx() const
{
    Mutex::ScopedLock lock(_mutex);
    return _formatCtx;
}


VideoDecoder* MediaCapture::video() const
{
    Mutex::ScopedLock lock(_mutex);
    return _video;
}


AudioDecoder* MediaCapture::audio() const
{
    Mutex::ScopedLock lock(_mutex);
    return _audio;
}


bool MediaCapture::stopping() const
{
    Mutex::ScopedLock lock(_mutex);
    return _stopping;
}


std::string MediaCapture::error() const
{
    Mutex::ScopedLock lock(_mutex);
    return _error;
}


} } // namespace scy::av


#endif

/// @\}
