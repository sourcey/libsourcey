///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_Types_H
#define SCY_AV_Types_H


#include "scy/av/config.h"
#include "scy/packet.h"
#include "scy/time.h"


namespace scy {
namespace av {


struct MediaPacket : public RawPacket
{
    std::int64_t time;

    MediaPacket(std::uint8_t* data = nullptr, std::size_t size = 0,
                std::int64_t time = 0)
        : // = time::clockSecs()
        RawPacket(reinterpret_cast<char*>(data), size)
        , time(time){};

    MediaPacket(const MediaPacket& r)
        : RawPacket(r)
        , time(r.time)
    {
    }

    virtual ~MediaPacket(){};

    virtual IPacket* clone() const { return new MediaPacket(*this); }

    virtual const char* className() const { return "MediaPacket"; }
};


struct VideoPacket : public MediaPacket
{
    int width;
    int height;
    bool iframe;

    VideoPacket(std::uint8_t* data = nullptr, std::size_t size = 0,
                int width = 0, int height = 0,
                std::int64_t time = 0)
        : // = time::clockSecs()
        MediaPacket(data, size, time)
        , width(width)
        , height(height)
        , iframe(false){};

    VideoPacket(const VideoPacket& r)
        : MediaPacket(r)
        , width(r.width)
        , height(r.height)
        , iframe(r.iframe)
    {
    }

    virtual ~VideoPacket(){};

    virtual IPacket* clone() const { return new VideoPacket(*this); }

    virtual const char* className() const { return "VideoPacket"; }
};


struct AudioPacket : public MediaPacket
{
    std::size_t numSamples; // number of samples per channel

    AudioPacket(std::uint8_t* data = nullptr, std::size_t size = 0,
                std::size_t numSamples = 0,
                std::int64_t time = 0)
        : // = time::clockSecs()
        MediaPacket(data, size, time)
        , numSamples(numSamples){};

    /// AudioPacket(const AudioPacket& r) :    ///     MediaPacket(r) {}

    virtual ~AudioPacket(){};

    virtual IPacket* clone() const { return new AudioPacket(*this); }

    virtual std::uint8_t* samples() const
    {
        return reinterpret_cast<std::uint8_t*>(_data);
    }

    virtual const char* className() const { return "AudioPacket"; }
};


} // namespace av
} // namespace scy


#endif // SCY_AV_Types_H


/// @\}
