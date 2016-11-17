///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/multiplexencoder.h"

#ifdef HAVE_FFMPEG

#include "assert.h"

extern "C" {
#include "libavutil/time.h" // av_gettime (depreciated)
}

#ifdef SCY_WIN
#define snprintf _snprintf
#endif


using std::endl;


namespace scy {
namespace av {


MultiplexEncoder::MultiplexEncoder(const EncoderOptions& options)
    : _options(options)
    , _formatCtx(nullptr)
    , _video(nullptr)
    , _audio(nullptr)
    , _ioCtx(nullptr)
    , _ioBuffer(nullptr)
    , _ioBufferSize(MAX_VIDEO_PACKET_SIZE)
    , _pts(0)
{
    TraceS(this) << "Create" << endl;
    initializeFFmpeg();
}


MultiplexEncoder::~MultiplexEncoder()
{
    TraceS(this) << "Destroy" << endl;
    uninitialize();
    uninitializeFFmpeg();
}


static int dispatchOutputPacket(void* opaque, std::uint8_t* buffer,
                                int bufferSize)
{
    // Callback example at:
    // http://lists.mplayerhq.hu/pipermail/libav-client/2009-May/003034.html
    auto klass= reinterpret_cast<MultiplexEncoder*>(opaque);
    if (klass) {
        TraceL << "Dispatching packet: " << bufferSize << endl;
        if (!klass->isActive()) {
            WarnL << "Dropping packet: " << bufferSize << ": " << klass->state()
                  << endl;
            return bufferSize;
        }
        MediaPacket packet(buffer, bufferSize);
        klass->emitter.emit(/*klass, */ packet);
        TraceL << "Dispatching packet: OK: " << bufferSize << endl;
    }

    return bufferSize;
}


void MultiplexEncoder::initialize()
{
    assert(!isActive());

    TraceS(this) << "Initialize:"
                 << "\n\tInput Format: " << _options.iformat.toString()
                 << "\n\tOutput Format: " << _options.oformat.toString()
                 << "\n\tDuration: " << _options.duration << endl;

    try {
        _options.oformat.video.enabled= _options.iformat.video.enabled;
        _options.oformat.audio.enabled= _options.iformat.audio.enabled;
        if (!_options.oformat.video.enabled && !_options.oformat.audio.enabled)
            throw std::runtime_error(
                "Either video or audio parameters must be specified.");

        if (_options.oformat.id.empty())
            throw std::runtime_error(
                "The output container format must be specified.");

        // Allocate the output media context
        assert(!_formatCtx);
        _formatCtx= avformat_alloc_context();
        if (!_formatCtx)
            throw std::runtime_error("Cannot allocate format context.");

        if (!_options.ofile.empty())
            snprintf(_formatCtx->filename, sizeof(_formatCtx->filename), "%s",
                     _options.ofile.c_str());

        // Set the container codec
        _formatCtx->oformat= av_guess_format(_options.oformat.id.c_str(),
                                             _options.ofile.c_str(), nullptr);
        if (!_formatCtx->oformat)
            throw std::runtime_error(
                "Cannot find suitable encoding format for " +
                _options.oformat.name);

        // Initialize encoder contexts
        if (_options.oformat.video.enabled)
            createVideo();
        if (_options.oformat.audio.enabled)
            createAudio();

        if (_options.ofile.empty()) {

            // Operating in streaming mode. Generated packets can be
            // obtained by connecting to the outgoing PacketSignal.
            // Setup the output IO context for our output stream.
            _ioBuffer= new unsigned char[_ioBufferSize];
            _ioCtx= avio_alloc_context(_ioBuffer, _ioBufferSize,
                                       AVIO_FLAG_WRITE, this, nullptr,
                                       dispatchOutputPacket, nullptr);
            _ioCtx->seekable= 0;
            _formatCtx->flags|= AVFMT_FLAG_CUSTOM_IO;
            _formatCtx->pb= _ioCtx;
        } else {

            // Operating in file mode.
            // Open the output file...
            if (!(_formatCtx->oformat->flags & AVFMT_NOFILE)) {
                // if (url_fopen(&_formatCtx->pb, _options.ofile.c_str(),
                // URL_WRONLY) < 0) {
                if (avio_open(&_formatCtx->pb, _options.ofile.c_str(),
                              AVIO_FLAG_WRITE) < 0) {
                    throw std::runtime_error(
                        "AVWriter: Unable to open the output file");
                }
            }
        }

        // Write the stream header (if any)
        // TODO: After Ready state
        avformat_write_header(_formatCtx, nullptr);

        // Send the format information to sdout
        av_dump_format(_formatCtx, 0, _options.ofile.c_str(), 1);

        // Get realtime presentation timestamp
        _formatCtx->start_time_realtime= av_gettime();

        setState(this, EncoderState::Ready);
    } catch (std::exception& exc) {
        ErrorS(this) << "Error: " << exc.what() << endl;
        setState(this, EncoderState::Error, exc.what());
        cleanup();
        throw exc; //.rethrow()
    }

    TraceS(this) << "Initialize: OK" << endl;
}


void MultiplexEncoder::uninitialize()
{
    TraceS(this) << "Uninitialize" << endl;

    // Write the trailer and dispatch the tail packet if any
    if (_formatCtx && _formatCtx->pb)
        av_write_trailer(_formatCtx);

    TraceS(this) << "Uninitializing: Wrote trailer" << endl;

    // Free memory
    cleanup();
    setState(this, EncoderState::Stopped);

    TraceS(this) << "Uninitialize: OK" << endl;
}


void MultiplexEncoder::cleanup()
{
    TraceS(this) << "Cleanup" << endl;

    // Delete stream encoders
    freeVideo();
    freeAudio();

    // Close the format
    if (_formatCtx) {

        // Free all remaining streams
        for (unsigned int i= 0; i < _formatCtx->nb_streams; i++) {
            av_freep(&_formatCtx->streams[i]->codec);
            av_freep(&_formatCtx->streams[i]);
        }

        // Close the output file (if any)
        if (!_options.ofile.empty() && _formatCtx->pb && _formatCtx->oformat &&
            !(_formatCtx->oformat->flags & AVFMT_NOFILE))
            avio_close(_formatCtx->pb);
        // avio_url_fclose(_formatCtx->pb);

        // Free the format context
        av_free(_formatCtx);
        _formatCtx= nullptr;
    }

    // if (_videoPtsCalc) {
    //    delete _videoPtsCalc;
    //    _videoPtsCalc = nullptr;
    // }

    if (_ioBuffer) {
        delete _ioBuffer;
        _ioBuffer= nullptr;
    }

    TraceS(this) << "Cleanup: OK" << endl;
}


void MultiplexEncoder::flush()
{
    TraceS(this) << "Flushing" << endl;

    if (_video) {
        _video->flush();
    }
    if (_audio) {
        _audio->flush();
    }
}


EncoderOptions& MultiplexEncoder::options()
{
    // Mutex::ScopedLock lock(_mutex);
    return _options;
}


VideoEncoder* MultiplexEncoder::video()
{
    // Mutex::ScopedLock lock(_mutex);
    return _video;
}


AudioEncoder* MultiplexEncoder::audio()
{
    // Mutex::ScopedLock lock(_mutex);
    return _audio;
}


//
// Helpers
//


// Write a packet to the output stream.
bool MultiplexEncoder::writeOutputPacket(AVPacket& packet)
{
    assert(packet.data);
    assert(packet.size);
    assert(packet.pts != AV_NOPTS_VALUE);
    assert(isActive());

    TraceL << "Writing packet:"
           << "\n\tPacket Size: " << packet.size << "\n\tPTS: " << packet.pts
           << "\n\tDTS: " << packet.dts << "\n\tDuration: " << packet.duration
           << endl;

    // Write the encoded frame to the output file
    if (av_interleaved_write_frame(_formatCtx, &packet) != 0) {
        WarnL << "Cannot write packet" << endl;
        return false;
    }
    return true;
}


void MultiplexEncoder::updateStreamPts(AVStream* stream, std::int64_t* pts)
{
    // https://docs.thefoundry.co.uk/products/nuke/developers/63/ndkdevguide/examples/ffmpegReader.cpp
    // https://ffmpeg.org/doxygen/trunk/doc_2examples_2muxing_8c-example.html
    // https://ffmpeg.org/doxygen/trunk/doc_2examples_2transcoding_8c-example.html
    if (*pts == AV_NOPTS_VALUE) {
        // Set a realtime pts value if not specified
        std::int64_t delta(av_gettime() - _formatCtx->start_time_realtime);
        _pts= delta * (float)stream->time_base.den /
              (float)stream->time_base.num / AV_TIME_BASE;
        *pts= _pts;
    } else {
        // Convert from input milliseconds to encoder stream time base
        _pts= *pts * (float)stream->time_base.den /
              (float)stream->time_base.num / AV_TIME_BASE;
        *pts= _pts;
    }
}


//
// Video stuff
//


void MultiplexEncoder::createVideo()
{
    // Mutex::ScopedLock lock(_mutex);
    assert(!_video);
    assert(_options.oformat.video.enabled);
    assert(_formatCtx->oformat->video_codec != AV_CODEC_ID_NONE);
    _video= new VideoEncoder(_formatCtx);
    _video->emitter.attach(packetSlot(this, &MultiplexEncoder::onVideoEncoded));
    _video->iparams= _options.iformat.video;
    _video->oparams= _options.oformat.video;
    _video->create();
    _video->open();
}


void MultiplexEncoder::freeVideo()
{
    // Mutex::ScopedLock lock(_mutex);

    if (_video) {
        delete _video;
        _video= nullptr;
    }
}


bool MultiplexEncoder::encodeVideo(AVFrame* frame)
{
    TraceS(this) << "Encoding video: " << frame->pts << endl;

    assert(isActive());
    assert(_video && _video->frame);

    if (!isActive())
        throw std::runtime_error("The encoder is not initialized");

    if (!_video)
        throw std::runtime_error("No video context");

    if (!frame->data || !frame->width || !frame->height)
        throw std::runtime_error("Invalid video frame");

    if (frame->pts == AV_NOPTS_VALUE)
        updateStreamPts(_video->stream, &frame->pts);

    return _video->encode(frame);

    // // AVPacket opacket;
    // // av_init_packet(&opacket);
    // if (_video->encode(frame)) { //, opacket
    //     // assert(opacket.stream_index == _video->stream->index);
    //     // return writeOutputPacket(opacket);
    // }
    //
    // assert(0);
    //
    // return false;
}


bool MultiplexEncoder::encodeVideo(std::uint8_t* buffer, int bufferSize,
                                   int width, int height, std::int64_t time)
{
    TraceS(this) << "Encoding video: " << time << endl;

    assert(isActive());
    assert(_video && _video->frame);

    if (!isActive())
        throw std::runtime_error("The encoder is not initialized");

    if (!_video)
        throw std::runtime_error("No video context");

    if (!buffer || !bufferSize || !width || !height)
        throw std::runtime_error("Invalid video frame");

    // if (time == AV_NOPTS_VALUE)
    updateStreamPts(_video->stream, &time);

    return _video->encode(buffer, bufferSize, time);
    // // AVPacket opacket;
    // // // av_init_packet(&opacket);
    // // if (_formatCtx->oformat->flags & AVFMT_RAWPICTURE) {
    // //     opacket.flags |= AV_PKT_FLAG_KEY;
    // //     opacket.stream_index = _video->stream->index;
    // //     opacket.data = buffer;
    // //     opacket.size = bufferSize;
    // // }
    // // else {
    //     if (!_video->encode(buffer, bufferSize, /*calc ? calc->tick() :
    //     */time)) { //, opacket
    //         WarnL << "Cannot encode video frame" << endl;
    //         return false;
    //     }
    // // }
    //
    // assert(0);
    // // if (opacket.size > 0) {
    // //     assert(opacket.stream_index == _video->stream->index);
    // //     assert(isActive());
    // //     return writeOutputPacket(opacket);
    // // }
    //
    // return false;
}


void MultiplexEncoder::onVideoEncoded(av::VideoPacket& packet)
{
    writeOutputPacket(*reinterpret_cast<AVPacket*>(packet.source));
}


//
// Audio stuff
//


void MultiplexEncoder::createAudio()
{
    TraceS(this) << "Create Audio" << endl;

    // Mutex::ScopedLock lock(_mutex);
    assert(!_audio);
    assert(_options.oformat.audio.enabled);
    assert(_formatCtx->oformat->audio_codec != AV_CODEC_ID_NONE);

    _audio= new AudioEncoder(_formatCtx);
    _audio->emitter.attach(packetSlot(this, &MultiplexEncoder::onAudioEncoded));
    _audio->iparams= _options.iformat.audio;
    _audio->oparams= _options.oformat.audio;
    _audio->create();
    _audio->open();
}


void MultiplexEncoder::freeAudio()
{
    // Mutex::ScopedLock lock(_mutex);

    if (_audio) {
        delete _audio;
        _audio= nullptr;
    }

    // if (_audioFifo) {
    //     av_fifo_free(_audioFifo);
    //     _audioFifo = nullptr;
    // }

    // if (_audioBuffer) {
    //     av_free(_audioBuffer);
    //     _audioBuffer = nullptr;
    // }
}


bool MultiplexEncoder::encodeAudio(std::uint8_t* buffer, int numSamples,
                                   std::int64_t time)
{
    TraceS(this) << "Encoding audio packet: samples=" << numSamples
                 << ", time=" << time << endl;
    assert(buffer);
    assert(numSamples);

    if (!buffer || !numSamples)
        throw std::runtime_error("Invalid audio input");

    if (!isActive())
        throw std::runtime_error("The encoder is not initialized");

    if (!_audio)
        throw std::runtime_error("No audio context");

    // if (time == AV_NOPTS_VALUE)
    updateStreamPts(_audio->stream, &time);

    return _audio->encode(buffer, numSamples, time);

    // // AVPacket opacket;
    // // av_init_packet(&opacket);
    //
    // //FIXME
    // if (_audio->encode(buffer, numSamples, time)) { //, opacket
    //     // assert(opacket.stream_index == _audio->stream->index);
    //     // return writeOutputPacket(opacket);
    // }
    //
    // assert(0);
    //
    // return false;
}


void MultiplexEncoder::onAudioEncoded(av::AudioPacket& packet)
{
    writeOutputPacket(*reinterpret_cast<AVPacket*>(packet.source));
}


} // namespace av
} // namespace scy


#endif


/// @\}
