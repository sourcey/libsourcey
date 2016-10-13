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


#include "scy/av/videoencoder.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace av {


VideoEncoderContext::VideoEncoderContext(AVFormatContext* format) :
    format(format),
    conv(nullptr),
    buffer(nullptr),
    bufferSize(0)
{
}


VideoEncoderContext::~VideoEncoderContext()
{
    close();
}


void VideoEncoderContext::create() //, const VideoCodec& params
{
    TraceS(this) << "Create: "
        << "\n\tInput: " << iparams.toString()
        << "\n\tOutput: " << oparams.toString()
        << endl;

    VideoContext::create();

    // Find the video encoder
    codec = avcodec_find_encoder_by_name(oparams.encoder.c_str());
    if (!codec) {
        if (format)
            codec = avcodec_find_encoder(format->oformat->video_codec);
        if (!codec)
            throw std::runtime_error("Video encoder not found: " + oparams.encoder);
    }

    // Allocate stream and AVCodecContext from the AVFormatContext if available
    if (format) {
        format->oformat->video_codec = codec->id;

        // Add a video stream that uses the format's default video
        // codec to the format context's streams[] array.
        stream = avformat_new_stream(format, codec);
        if (!stream)
            throw std::runtime_error("Cannot create video stream.");

        // // fixme: testing realtime streams
        // // http://stackoverflow.com/questions/16768794/muxing-from-audio-and-video-files-with-ffmpeg
        // stream->time_base.den = 1000; //realtime_ ? 1000 : fps_.num;
        // stream->time_base.num = 1; //realtime_ ? 1: fps_.den;
        //
        // stream->r_frame_rate.num = oparams.fps;
        // stream->r_frame_rate.den = 1;
        // stream->avg_frame_rate.den = 1;
        // stream->avg_frame_rate.num = oparams.fps;

        ctx = stream->codec;
    }

    // Otherwise allocate the standalone AVCodecContext
    else {
        ctx = avcodec_alloc_context3(codec);
        if (!ctx)
            throw std::runtime_error("Cannot allocate encoder context.");
    }

    initVideoEncoderContext(ctx, codec, oparams);

    // Create the video conversion context if needed
    createConverter();

    // Some formats want stream headers to be separate
    if (format && format->oformat->flags & AVFMT_GLOBALHEADER)
        ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    // Allocate the input frame
    frame = createVideoFrame(av_get_pix_fmt(iparams.pixelFmt.c_str()), iparams.width, iparams.height);
    if (!frame)
        throw std::runtime_error("Cannot allocate input frame.");

    // Allocate the encode buffer
    // XXX: Disabling in favor of encoder manged buffer
    // bufferSize = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    // buffer = (std::uint8_t*)av_malloc(bufferSize);
}


void VideoEncoderContext::createConverter()
{
    if (conv)
        throw std::runtime_error("Conversion context already exists.");

    // Create the conversion context
    if (iparams.width != oparams.width ||
        iparams.height != oparams.height ||
        iparams.pixelFmt != oparams.pixelFmt) {
        conv = new VideoConversionContext();
        conv->iparams = iparams;
        conv->oparams = oparams;
        conv->create();
    }
}


void VideoEncoderContext::freeConverter()
{
    if (conv) {
        delete conv;
        conv = nullptr;
    }

    //if (frame) {
    //    av_free(frame);
    //    frame = nullptr;
    //}
}


void VideoEncoderContext::close()
{
    TraceS(this) << "Closing" << endl;

    VideoContext::close();

    freeConverter();

#if 0
    if (buffer) {
        av_free(buffer);
        buffer = nullptr;
    }

    // Free the stream
    if (stream && format && format->nb_streams) {
        for (unsigned int i = 0; i < format->nb_streams; i++) {
            if (format->streams[i] == stream) {
                TraceS(this) << "Closing: Removing stream: " << stream << endl;
                av_freep(&format->streams[i]->codec);
                av_freep(&format->streams[i]);
                stream = nullptr;
                format->nb_streams--;
            }
        }
    }
#endif
}


bool encodeVideoPacket(VideoEncoderContext* enc, AVFrame* iframe, AVPacket& opacket)
{
    // assert(iframe);
    // assert(iframe->data[0]);
    assert(enc->codec);

    AVFrame* cframe = nullptr;
    if (iframe) {

        // Recreate the video conversion context on the fly
        // if the input resolution changes.
        if (iframe->width != enc->iparams.width ||
            iframe->height != enc->iparams.height) {
            // _options.iformat.video.width = iframe->width;
            // _options.iformat.video.height = iframe->height;
            enc->iparams.width = iframe->width;
            enc->iparams.height = iframe->height;
            DebugL << "Recreating video conversion context" << endl;
            enc->freeConverter();
            enc->createConverter();
        }

        // Convert the input frame if required
        cframe = enc->conv ? enc->conv->convert(iframe) : iframe;
        cframe->format = enc->ctx->pix_fmt;
        cframe->width  = iframe->width;
        cframe->height = iframe->height;

        // Set the input PTS or a monotonic value to keep the encoder happy.
        // The actual setting of the PTS is outside the scope of this encoder.
        cframe->pts = iframe->pts != AV_NOPTS_VALUE ? iframe->pts : enc->ctx->frame_number;
    }

    av_init_packet(&opacket);
    opacket.data = nullptr; // using encoder assigned buffer
    opacket.size = 0;
    // opacket.data = this->buffer;
    // opacket.size = this->bufferSize;
    if (enc->stream)
        opacket.stream_index = enc->stream->index;

    int frameEncoded = 0;
    if (avcodec_encode_video2(enc->ctx, &opacket, cframe, &frameEncoded) < 0) {
        // TODO: Use av_strerror
        enc->error = "Fatal encoder error";
        ErrorL << enc->error << endl; //S(this)
        throw std::runtime_error(enc->error);
    }

    return frameEncoded > 0;
}


bool VideoEncoderContext::encode(unsigned char* data, int size, std::int64_t pts, AVPacket& opacket)
{
    assert(data);
    assert(size);
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = data;
    ipacket.size = size;
    ipacket.pts = pts;
    if (stream)
        ipacket.stream_index = stream->index;
    return encode(ipacket, opacket);
}


bool VideoEncoderContext::encode(AVPacket& ipacket, AVPacket& opacket)
{
    assert(ipacket.data);
    assert(frame);
    assert(codec);

    // Populate the input frame with date from the given buffer.
    // NOTE: This only works with contiguous buffers
    frame->data[0] = reinterpret_cast<std::uint8_t*>(ipacket.data);

    // TODO: Correctly set the input frame PTS
    // http://thompsonng.blogspot.com.au/2011/09/ffmpeg-avinterleavedwriteframe-return.html
    // http://stackoverflow.com/questions/6603979/ffmpegavcodec-encode-video-setting-pts-h264
    // (1 / oparams.fps) * sample rate * frame number
    frame->pts = ipacket.pts;

    // _options.iformat.video.width = iframe->width;
    // _options.iformat.video.height = iframe->height;
    // enc->iparams.width = frame->width;
    // enc->iparams.height = frame->height;

    // avpicture_fill((AVPicture *)frame, (std::uint8_t*)ipacket.data,
    //    av_get_pix_fmt(iparams.pixelFmt), iparams.width, iparams.height);

    return encode(frame, opacket);
}


bool VideoEncoderContext::encode(AVFrame* iframe, AVPacket& opacket)
{
    if (encodeVideoPacket(this, iframe, opacket)) {
        fps.tick();
        if (ctx->coded_frame->key_frame)
            opacket.flags |= AV_PKT_FLAG_KEY;
        if (stream) {
            // if (opacket.pts != AV_NOPTS_VALUE)
            //     opacket.pts = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
            // if (opacket.dts != AV_NOPTS_VALUE)
            //     opacket.dts = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);
            // if (opacket.duration > 0)
            //     opacket.duration = (int)av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);

            TraceS(this) << "Encoded Frame:"
                << "\n\tScaled PTS: " << opacket.pts
                << "\n\tScaled DTS: " << opacket.dts
                << "\n\tScaled Duration: " << opacket.duration
                << endl;
        }

        return true;
    }

    return false;
}


bool VideoEncoderContext::flush(AVPacket& opacket)
{
    return encode(nullptr, opacket);

    // av_init_packet(&opacket);
    // opacket.data = nullptr;
    // opacket.size = 0;
    //
    // int frameEncoded = 0;
    // if (avcodec_encode_video2(ctx, &opacket, nullptr, &frameEncoded) < 0) {
    //     // TODO: Use av_strerror
    //     error = "Fatal encoder error";
    //     ErrorS(this) << error << endl;
    //     throw std::runtime_error(error);
    // }
    //
    // if (frameEncoded) {
    //     if (ctx->coded_frame->key_frame)
    //         opacket.flags |= AV_PKT_FLAG_KEY;
    //     if (stream) {
    //         if (opacket.pts != AV_NOPTS_VALUE)
    //             opacket.pts = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
    //         if (opacket.dts != AV_NOPTS_VALUE)
    //             opacket.dts = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);
    //         if (opacket.duration > 0)
    //             opacket.duration = (int)av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);
    //         TraceS(this) << "Flushed video frame: " << opacket.pts << endl;
    //     }
    //     return true;
    // }
    // return false;
}


} } // namespace scy::av


#endif
