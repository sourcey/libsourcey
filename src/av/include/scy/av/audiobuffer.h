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


#ifndef SCY_AV_AudioBuffer_H
#define SCY_AV_AudioBuffer_H


#include <string>

#include "scy/base.h"

#ifdef HAVE_FFMPEG

extern "C" {
#include <libavutil/audio_fifo.h>
}


namespace scy {
namespace av {


struct AudioBuffer
{
    AudioBuffer();
    ~AudioBuffer();

    void alloc(const std::string& sampleFmt, int channels, int numSamples = 1024);
    void reset();
    void close();

    void write(void** samples, int numSamples);
    bool read(void** samples, int numSamples);

    int available() const;

    AVAudioFifo* fifo;
};


} } // namespace scy::av


#endif
#endif // SCY_AV_AudioBuffer_H
