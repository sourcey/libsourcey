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


#include "scy/media/format.h"
#include <sstream>
#include <algorithm>


using std::endl;


namespace scy {
namespace av {


Format::Format() :
    name("Unknown"), priority(0)
{
}


Format::Format(const std::string& name, const char* id, const VideoCodec& video, const AudioCodec& audio, int priority) :
    name(name), id(id), video(video), audio(audio), priority(priority)
{
}


Format::Format(const std::string& name, const char* id, const VideoCodec& video, int priority) :
    name(name), id(id), video(video), priority(priority)
{
}


Format::Format(const std::string& name, const char* id, const AudioCodec& audio, int priority) :
    name(name), id(id), audio(audio), priority(priority)
{
}


Format::Format(const Format& r) :
    name(r.name),
    id(r.id),
    video(r.video),
    audio(r.audio),
    priority(r.priority)
{
}


Format::Type Format::type() const
{
    if (video.enabled && audio.enabled)
        return Format::Multiplex;
    else if (video.enabled)
        return Format::Video;
    else if (audio.enabled)
        return Format::Audio;
    else
        return Format::None;
}


std::string Format::toString() const
{
    std::ostringstream os;
    os << "Format[" << name << ":"  << id;
    if (video.enabled)
        os << "\n\t" << video.toString();
    if (audio.enabled)
        os << "\n\t" << audio.toString();
    os << "]";
    return os.str();
}


void Format::print(std::ostream& ost)
{
    ost << "Format[" << name << ":"  << id;
    if (video.enabled) {
        ost << "\n";
        video.print(ost);
        //ost << "\n";
    }
    if (audio.enabled) {
        ost << "\n";
        audio.print(ost);
        //ost << "\n";
    }
    ost << "\n]";
}


} // namespace av
} // namespace scy
