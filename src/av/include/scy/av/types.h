///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_Types_H
#define SCY_AV_Types_H


#include "scy/av/av.h"
#include "scy/packet.h"
#include "scy/time.h"


namespace scy {
namespace av {


struct MediaPacket : public RawPacket
{
    int64_t time;

    MediaPacket(uint8_t* data = nullptr, size_t size = 0,
                int64_t time = 0)
        : RawPacket(reinterpret_cast<char*>(data), size)
        , time(time)
    {
    }

    MediaPacket(const MediaPacket& r)
        : RawPacket(r)
        , time(r.time)
    {
    }

    virtual ~MediaPacket() {}

    virtual IPacket* clone() const { return new MediaPacket(*this); }

    virtual const char* className() const { return "MediaPacket"; }
};


struct VideoPacket : public MediaPacket
{
    int width;
    int height;
    bool iframe;

    VideoPacket(uint8_t* data = nullptr, size_t size = 0,
                int width = 0, int height = 0,
                int64_t time = 0)
        : MediaPacket(data, size, time)
        , width(width)
        , height(height)
        , iframe(false)
    {
    }

    VideoPacket(const VideoPacket& r)
        : MediaPacket(r)
        , width(r.width)
        , height(r.height)
        , iframe(r.iframe)
    {
    }

    virtual ~VideoPacket() {};

    virtual IPacket* clone() const { return new VideoPacket(*this); }

    virtual const char* className() const { return "VideoPacket"; }
};


struct PlanarVideoPacket : public VideoPacket
{
    std::vector<uint8_t> ydata;
    std::vector<uint8_t> udata;
    std::vector<uint8_t> vdata;

    size_t ystride;
    size_t ustride;
    size_t vstride;

    PlanarVideoPacket(uint8_t* y = nullptr, uint8_t* u = nullptr, uint8_t* v = nullptr,
                      size_t y_stride = 0, size_t u_stride = 0, size_t v_stride = 0,
                      int width = 0, int height = 0, int64_t time = 0)
        : VideoPacket(nullptr, 0, width, height, time)
        , ydata(y, y + (y_stride * height))
        , udata(u, u + (u_stride * height / 2))
        , vdata(v, v + (v_stride * height / 2))
        , ystride(y_stride)
        , ustride(u_stride)
        , vstride(v_stride)
    {
        //memcpy(frame->data[0], inputBufferY, frame->linesize[0] * frame->height);
        //memcpy(frame->data[1], inputBufferU, frame->linesize[1] * frame->height / 2);
        //memcpy(frame->data[2], inputBufferV, frame->linesize[2] * frame->height / 2);
    }

    PlanarVideoPacket(const PlanarVideoPacket& r)
        : VideoPacket(r)
        , ydata(r.ydata)
        , udata(r.udata)
        , vdata(r.vdata)
        , ystride(r.ystride)
        , ustride(r.ustride)
        , vstride(r.vstride)
    {
    }

    virtual ~PlanarVideoPacket() {};

    virtual IPacket* clone() const { return new PlanarVideoPacket(*this); }

    virtual const char* className() const { return "PlanarVideoPacket"; }
};


struct AudioPacket : public MediaPacket
{
    size_t numSamples; // number of samples per channel

    AudioPacket(uint8_t* data = nullptr, size_t size = 0,
                size_t numSamples = 0,
                int64_t time = 0)
        : MediaPacket(data, size, time)
        , numSamples(numSamples)
    {
    }

    virtual ~AudioPacket() {};

    virtual IPacket* clone() const { return new AudioPacket(*this); }

    virtual uint8_t* samples() const
    {
        return reinterpret_cast<uint8_t*>(_data);
    }

    virtual const char* className() const { return "AudioPacket"; }
};


} // namespace av
} // namespace scy


#endif // SCY_AV_Types_H


/// @\}
