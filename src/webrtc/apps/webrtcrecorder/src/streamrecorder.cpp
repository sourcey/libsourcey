#include "streamrecorder.h"

#include "scy/logger.h"
#include "scy/filesystem.h"
#include "scy/av/ffmpeg.h"

#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"


namespace scy {


StreamRecorder::StreamRecorder() //webrtc::VideoTrackInterface* track_to_render, const std::string& basename
    : // _videoTrack(track_to_render),
    //   _basename(basename),
    //   _count(0),
    //   _width(0),
    //   _height(0)
    _awaitingVideo(false),
    _awaitingAudio(false),
    in_frame(nullptr),
    raw_in(nullptr)
{
    av::initializeFFmpeg();

    // _encoder.options().ofile = "test.webm";
    _encoder.options().ofile = "test.mp4";
    // _encoder.options().oformat.name = "MP4";
    _encoder.options().oformat.id = "mp4";
}


StreamRecorder::~StreamRecorder()
{
    av::uninitializeFFmpeg();

    if (_videoTrack)
        _videoTrack->RemoveSink(this);
        // delete _videoTrack;

    if (_audioTrack)
        _audioTrack->RemoveSink(this);
        // delete _audioTrack;
}


void StreamRecorder::setVideoTrack(webrtc::VideoTrackInterface* track_to_render)
{
    assert(!_videoTrack);
    _videoTrack = track_to_render;
    _videoTrack->AddOrUpdateSink(this, rtc::VideoSinkWants());
    _awaitingVideo = true;
    // _videoTrack = new StreamRecorderVideoTrack(this, track_to_render);
}


void StreamRecorder::setAudioTrack(webrtc::AudioTrackInterface* track_to_render)
{
    assert(!_audioTrack);
    _audioTrack = track_to_render;
    _audioTrack->AddSink(this);
    _awaitingAudio = true;
    // _audioTrack = new StreamRecorderAudioTrack(this, track_to_render);
}

static int frameNumber = 0;
static int maxFrames = 200;

void StreamRecorder::OnFrame(const cricket::VideoFrame& yuvframe)
{
    frameNumber++;
    if (frameNumber == maxFrames) {
        _encoder.uninitialize();
        // _encoder.close();
        // _output.close();
    }
    if (frameNumber >= maxFrames) {
        DebugL << "Done recroding" << std::endl;
        return;
    }

    DebugL << "On video frame: " << yuvframe.width() << 'x' << yuvframe.height() << std::endl;

    // ivideo.width = yuvframe.width();
    // ivideo.height = yuvframe.height();

    if (_awaitingVideo) {
        _awaitingVideo = false;

        auto& ivideo = _encoder.options().iformat.video;
        auto& ovideo = _encoder.options().oformat.video;
        ivideo.pixelFmt = "yuv420p";
        ivideo.width = yuvframe.width();
        ivideo.height = yuvframe.height();
        ivideo.fps = 25;

        // H.264
        ovideo.encoder = "libx264";
        ovideo.pixelFmt = "yuv420p";

        // H.265
        // ovideo.encoder = "libx265";
        // ovideo.pixelFmt = "yuv420p";

        // MJPEG
        // ovideo.encoder = "mjpeg";
        // ovideo.pixelFmt = "yuvj420p";

        // WebM
        // ovideo.encoder = "libvpx-vp9";
        // ovideo.encoder = "libvpx";
        // ovideo.pixelFmt = "yuv420p";

        ovideo.width = yuvframe.width();
        ovideo.height = yuvframe.height();
        ovideo.fps = 25;
        ovideo.bitRate = 500000;
        ovideo.enabled = true;

        // _output.open("example.mp4", std::ios::out | std::ios::binary);

        // _encoder.emitter.attach(packetDelegate(this, &StreamRecorder::onPacketEncoded));

        if (!_awaitingAudio)
            _encoder.initialize();

        // _encoder.iparams.width = width; //yuvframe.width();
        // _encoder.iparams.height = height; //yuvframe.height();
        // _encoder.iparams.pixelFmt = "yuv420p";
        //
        // _encoder.oparams.encoder = "libx264"; // "png"
        // _encoder.oparams.pixelFmt = "yuv420p";
        // _encoder.oparams.width = width; //yuvframe.width();
        // _encoder.oparams.height = height; //yuvframe.height();
        // _encoder.oparams.fps = 25;
        // _encoder.oparams.bitRate = 500000;
        // _encoder.oparams.enabled = true;
        //
        // _encoder.create();
        // _encoder.open();
        //
        // _output.open("example.h264", std::ios::out | std::ios::binary);
    }

    // Set avframe->data pointers manually

    // /* prepare a dummy image */
    // int width = yuvframe.width();
    // int height = yuvframe.height();
    // auto frame = frame; //_encoder.frame;
    // int x, y;
    // /* Y */
    // for (y = 0; y < height; y++) {
    //     for (x = 0; x < width; x++) {
    //         frame->data[0][y * frame->linesize[0] + x] = x + y + frameNumber * 3;
    //     }
    // }
    // /* Cb and Cr */
    // for (y = 0; y < height/2; y++) {
    //     for (x = 0; x < width/2; x++) {
    //         frame->data[1][y * frame->linesize[1] + x] = 128 + y + frameNumber * 2;
    //         frame->data[2][y * frame->linesize[2] + x] = 64 + x + frameNumber * 5;
    //     }
    // }
    // frame->width = width;
    // frame->height = height;
    // frame->pts = frameNumber; // * 10000;

    // _encoder.frame->data[0] = (uint8_t*)yuvframe.video_frame_buffer()->DataY();
    // _encoder.frame->data[1] = (uint8_t*)yuvframe.video_frame_buffer()->DataU();
    // _encoder.frame->data[2] = (uint8_t*)yuvframe.video_frame_buffer()->DataV();
    // _encoder.frame->width = width; //yuvframe.width();
    // _encoder.frame->height = height; //yuvframe.height();
    // _encoder.frame->pts = AV_NOPTS_VALUE;

    // Feed the raw frame into the encoder
    // AVPacket opacket;
    // if (_encoder.encode(_encoder.frame, opacket)) {
    //
    //     // Write the output file
    //     _output.write(reinterpret_cast<const char*>(opacket.data), opacket.size);
    // }

    // Set avframe->data pointers manually
    auto frame = _encoder.video()->frame; //_encoder.frame;
    frame->data[0] = (uint8_t*)yuvframe.video_frame_buffer()->DataY();
    frame->data[1] = (uint8_t*)yuvframe.video_frame_buffer()->DataU();
    frame->data[2] = (uint8_t*)yuvframe.video_frame_buffer()->DataV();
    frame->width = yuvframe.width();
    frame->height = yuvframe.height();
    frame->pts = AV_NOPTS_VALUE;

    // Encode the video frame
    _encoder.encodeVideo(frame);
}


void StreamRecorder::onPacketEncoded(av::MediaPacket& packet)
{
    _output.write(reinterpret_cast<const char*>(packet.data()), packet.size());
}


// Generate i-th frame of test audio
static int generate_raw_frame(uint16_t *frame_data, int i, int sample_rate,
                             int channels, int frame_size)
{
   int j, k;

   for (j = 0; j < frame_size; j++) {
       frame_data[channels * j] = 10000 * ((j / 10 * i) % 2);
       for (k = 1; k < channels; k++)
           frame_data[channels * j + k] = frame_data[channels * j] * (k + 1);
   }
   return 0;
    //
    // int t = 0;
    // int tincr = 2 * M_PI * 440.0 / sample_rate;
    // // for (i = 0; i < 200; i++) {
    // //     av_init_packet(&pkt);
    //     // pkt.data = NULL; // packet data will be allocated by the encoder
    //     // pkt.size = 0;
    //     for (j = 0; j < frame_size; j++) {
    //         frame_data[2*j] = (int)(sin(t) * 10000);
    //         for (k = 1; k < channels; k++)
    //             frame_data[2*j + k] = frame_data[2*j];
    //         t += tincr;
    //     }
    // //     /* encode the samples */
    // //     ret = avcodec_encode_audio2(c, &pkt, frame, &got_output);
    // //     if (ret < 0) {
    // //         fprintf(stderr, "Error encoding audio frame\n");
    // //         exit(1);
    // //     }
    // //     if (got_output) {
    // //         fwrite(pkt.data, 1, pkt.size, f);
    // //         av_packet_unref(&pkt);
    // //     }
    // // }
    //  return 0;
}


void StreamRecorder::OnData(const void* audio_data,
                            int bits_per_sample,
                            int sample_rate,
                            size_t number_of_channels,
                            size_t number_of_frames)
{
    DebugL << "On audio frame: " << number_of_frames << std::endl;

    frameNumber++;
    if (frameNumber == maxFrames) {
        _encoder.uninitialize();
        // _encoder.close();
        // _output.close();
    }
    if (frameNumber >= maxFrames) {
        DebugL << "Done recroding" << std::endl;
        return;
    }

    if (_awaitingAudio) {
        _awaitingAudio = false;

        auto& iaudio = _encoder.options().iformat.audio;
        auto& oaudio = _encoder.options().oformat.audio;

        // AudioCodec(const std::string& name,
        //     int channels = DEFAULT_AUDIO_CHANNELS,
        //     int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE,
        //     int bitRate = DEFAULT_AUDIO_BIT_RATE,
        //     const char* sampleFmt = DEFAULT_AUDIO_SAMPLE_FMT);

        // iaudio.bitRate = 64000; //bits_per_sample;
        iaudio.sampleRate = sample_rate;
        iaudio.channels = number_of_channels;
        iaudio.sampleFmt = "s16"; // ???

        oaudio.encoder = "libfdk_aac";
        oaudio.bitRate = 64000; //DEFAULT_AUDIO_BIT_RATE; //bits_per_sample;
        oaudio.sampleRate = sample_rate; //DEFAULT_AUDIO_SAMPLE_RATE; //
        oaudio.channels = number_of_channels; //DEFAULT_AUDIO_CHANNELS; //
        oaudio.sampleFmt = "s16"; // ???
        oaudio.enabled = true;

        if (!_awaitingVideo)
            _encoder.initialize();
    }


    auto audio = _encoder.audio();
                                // int bits_per_sample,
                                // int sample_rate,
                                // size_t number_of_channels,


    if (!in_frame) {
        in_frame = av_frame_alloc();
        if (!in_frame) {
            throw std::runtime_error("Can't allocate input frame\n");

        }

        in_frame->nb_samples = audio->ctx->frame_size;
        in_frame->format = audio->ctx->sample_fmt;
        in_frame->channel_layout = audio->ctx->channel_layout;
        if (av_frame_get_buffer(in_frame, 32) != 0) {
            throw std::runtime_error("Can't allocate a buffer for input frame\n");

        }

        // raw_in = (uint8_t*)av_malloc(in_frame->linesize[0] * maxFrames);
        // if (!raw_in) {
        //     throw std::runtime_error("Can't allocate memory for raw_in\n");
        //
        // }
        //
        // in_offset = 0;
    }

    // uint8_t *raw_in = NULL;

    generate_raw_frame((uint16_t*)(in_frame->data[0]), frameNumber, audio->ctx->sample_rate,
                                   audio->ctx->channels, audio->ctx->frame_size);

    int in_frame_bytes = in_frame->nb_samples * av_frame_get_channels(in_frame) * sizeof(uint16_t);
    if (in_frame_bytes > in_frame->linesize[0]) {
        throw std::runtime_error("Incorrect value of input frame linesize\n");
        return;
    }

    // memcpy(raw_in + in_offset, in_frame->data[0], in_frame_bytes);
    // in_offset += in_frame_bytes;

    // bool AudioEncoderContext::encode(const std::uint8_t* samples, const int frameSize, const std::int64_t pts, AVPacket& opacket)

    // nBufferFrames * self->_channels * size
    // std::size_t frameSize = number_of_frames * number_of_channels * 2;
    _encoder.encodeAudio(in_frame->data[0], in_frame->nb_samples, in_frame->nb_samples);
    // , std::uint64_t time = AV_NOPTS_VALUE


    // // Set avframe->data pointers manually
    // auto frame = _encoder.video()->frame; //_encoder.frame;
    // frame->data[0] = (uint8_t*)yuvframe.video_frame_buffer()->DataY();
    // frame->data[1] = (uint8_t*)yuvframe.video_frame_buffer()->DataU();
    // frame->data[2] = (uint8_t*)yuvframe.video_frame_buffer()->DataV();
    // frame->width = yuvframe.width();
    // frame->height = yuvframe.height();
    // frame->pts = AV_NOPTS_VALUE;
    //
    // // Encode the video frame
    // _encoder.encodeVideo(frame);
}


// //
// // Stream Recorder Video Track
// //
//
// StreamRecorderVideoTrack::StreamRecorderVideoTrack(StreamRecorder* recorder, webrtc::VideoTrackInterface* track_to_render) //, const std::string& basename
//     : _recorder(recorder),
//       _track(track_to_render),
//       // _basename(basename),
//       _count(0),
//       _width(0),
//       _height(0)
// {
//     _track->AddOrUpdateSink(this, rtc::VideoSinkWants());
// }
//
//
// StreamRecorderVideoTrack::~StreamRecorderVideoTrack()
// {
//     _track->RemoveSink(this);
//
//     // // Cleanup the frame
//     // if (_avframe)
//     //     av_free(_avframe);
// }
//
//
// // std::string StreamRecorderVideoTrack::getNextFilename()
// // {
// //     std::stringstream filename;
// //     filename << _basename;
// //     if (++_count > 1)
// //         filename << '-' << _count;
// //     filename << '_' << _width << 'x' << _height << ".jpg";
// //     return filename.str();
// // }
//
//
// void StreamRecorderVideoTrack::OnFrame(const cricket::VideoFrame& yuvframe)
// {
//     TraceL << "On video frame: " << yuvframe.width() << 'x' << yuvframe.height() << std::endl;
//
//     // if (_width != yuvframe.width() ||
//     //     _height != yuvframe.height()) {
//     //     _width = yuvframe.width();
//     //     _height = yuvframe.height();
//     //
//     //     _encoder.iparams.width = yuvframe.width();
//     //     _encoder.iparams.height = yuvframe.height();
//     //     _encoder.iparams.pixelFmt = "yuv420p";
//     //
//     //     _encoder.oparams.encoder = "mjpeg"; // "png"
//     //     _encoder.oparams.pixelFmt = "yuvj420p";
//     //     _encoder.oparams.width = yuvframe.width();
//     //     _encoder.oparams.height = yuvframe.height();
//     //     _encoder.oparams.fps = 25; // avoid FFmpeg 0 fps bitrate tolerance error
//     //     _encoder.oparams.enabled = true;
//     //
//     //     _encoder.create();
//     //     _encoder.open();
//     //
//     //     // Cleanup the previous frame (if any)
//     //     if (_avframe)
//     //         av_free(_avframe);
//     //
//     //     // Initialize the AVFrame
//     //     _avframe = av_frame_alloc();
//     //     _avframe->width = yuvframe.width();
//     //     _avframe->height = yuvframe.height();
//     //     _avframe->format = AV_PIX_FMT_YUV420P;
//     //
//     //     // Initialize avframe->linesize
//     //     avpicture_fill((AVPicture*)_avframe, NULL, /*_avframe->format*/AV_PIX_FMT_YUV420P, _avframe->width, _avframe->height);
//     // }
//     //
//     // // Set avframe->data pointers manually
//     // _avframe->data[0] = (uint8_t*)yuvframe.video_frame_buffer()->DataY();
//     // _avframe->data[1] = (uint8_t*)yuvframe.video_frame_buffer()->DataU();
//     // _avframe->data[2] = (uint8_t*)yuvframe.video_frame_buffer()->DataV();
//     //
//     // // Feed the raw frame into the encoder
//     // AVPacket opacket;
//     // if (_encoder.encode(_avframe, opacket)) {
//     //
//     //     // Write the output file
//     //     std::string filename(getNextFilename());
//     //     TraceL << "Write video frame: " << filename << std::endl;
//     //     fs::savefile(filename, (const char*)opacket.data, (std::streamsize)opacket.size);
//     // }
// }
//
//
// //
// // Stream Recorder Audio Track
// //
//
//
// StreamRecorderAudioTrack::StreamRecorderAudioTrack(StreamRecorder* recorder, webrtc::AudioTrackInterface* track_to_render) //, const std::string& basename
//     : _recorder(recorder),
//       _track(track_to_render)
// {
//     _track->AddSink(this);
// }
//
//
// StreamRecorderAudioTrack::~StreamRecorderAudioTrack()
// {
//     _track->RemoveSink(this);
// }
//
//
// void StreamRecorderAudioTrack::OnData(const void* audio_data,
//                                      int bits_per_sample,
//                                      int sample_rate,
//                                      size_t number_of_channels,
//                                      size_t number_of_frames)
// {
// }
//
// // if(encoded_) {
// // if (audio_record_sample_hz_ != audio.sample_rate || audio.channels != audio_record_channels_) {
// //   int16_t temp_output[kMaxDataSizeSamples];
// //   int samples_per_channel_int = resampler_record_.Resample10Msec((int16_t*)audio.data, audio.sample_rate * audio.channels,
// //     audio_record_sample_hz_ * audio_record_channels_, 1, kMaxDataSizeSamples, temp_output);
// //   Encode(temp_output, audio_record_sample_hz_ / 100, 2, audio_record_channels_, audio_record_sample_hz_, 0);
// // }
// // else {
// //   Encode((int16_t*)audio.data, audio.samples_per_channel, 2, audio_record_channels_, audio_record_sample_hz_, 0);
// // }
// // }

} // namespace scy
