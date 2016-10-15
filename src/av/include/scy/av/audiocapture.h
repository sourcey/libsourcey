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

#ifndef SCY_AV_AudioCapture_H
#define SCY_AV_AudioCapture_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/mediacapture.h"


namespace scy {
namespace av {


class AudioCapture: public MediaCapture
    /// This class implements a cross platform audio capturer.
{
public:
    typedef std::shared_ptr<AudioCapture> Ptr;

    AudioCapture();
    AudioCapture(const std::string& device, int channels = -1, int sampleRate = -1);
    virtual ~AudioCapture();

    virtual void open(const std::string& device, int channels = -1, int sampleRate = -1);
};


} } // namespace scy::av


#endif
#endif // SCY_AV_AudioCapture_H
