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


#ifndef SCY_AV_Types_H
#define SCY_AV_Types_H


#include "scy/av/config.h"
#include "scy/packet.h"
#include "scy/time.h"


namespace scy {
namespace av {


struct MediaPacket: public RawPacket
{
    std::int64_t time;

    MediaPacket(std::uint8_t* data = nullptr,
                std::size_t size = 0,
                std::int64_t time = 0) : // = time::clockSecs()
        RawPacket(reinterpret_cast<char*>(data), size),
        time(time) {};

    MediaPacket(const MediaPacket& r) :
        RawPacket(r),
        time(r.time) {}

    virtual ~MediaPacket() {};

    virtual IPacket* clone() const {
        return new MediaPacket(*this);
    }

    virtual const char* className() const { return "MediaPacket"; }
};


struct VideoPacket: public MediaPacket
{
    int width;
    int height;
    bool iframe;

    VideoPacket(std::uint8_t* data = nullptr,
                std::size_t size = 0,
                int width = 0,
                int height = 0,
                std::int64_t time = 0) : // = time::clockSecs()
        MediaPacket(data, size, time),
        width(width),
        height(height),
        iframe(false) {};

    VideoPacket(const VideoPacket& r) :
        MediaPacket(r),
        width(r.width),
        height(r.height),
        iframe(r.iframe) {}

    virtual ~VideoPacket() {};

    virtual IPacket* clone() const {
        return new VideoPacket(*this);
    }

    virtual const char* className() const { return "VideoPacket"; }
};


struct AudioPacket: public MediaPacket
{
    std::size_t numSamples; // number of samples per channel

    AudioPacket(std::uint8_t* data = nullptr,
                std::size_t size = 0,
                std::size_t numSamples = 0,
                std::int64_t time = 0) : // = time::clockSecs()
        MediaPacket(data, size, time), numSamples(numSamples) {};

    // AudioPacket(const AudioPacket& r) :
    //     MediaPacket(r) {}

    virtual ~AudioPacket() {};

    virtual IPacket* clone() const {
        return new AudioPacket(*this);
    }

    virtual std::uint8_t* samples() const {
        return reinterpret_cast<std::uint8_t*>(_data);
    }

    virtual const char* className() const { return "AudioPacket"; }
};


} } // namespace scy::av


#endif // SCY_AV_Types_H
