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


#include "scy/av/audiocapture.h"

#ifdef HAVE_FFMPEG

#include "scy/av/devicemanager.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}


using std::endl;


namespace scy {
namespace av {


AudioCapture::AudioCapture()
{
}


AudioCapture::AudioCapture(const std::string& device, int channels, int sampleRate)
{
    open(device, channels, sampleRate);
}


AudioCapture::~AudioCapture()
{
}


void AudioCapture::open(const std::string& device, int channels, int sampleRate)
{
    TraceS(this) << "Opening microphone: " << device << endl;

    DeviceManager devman;
    auto iformat = devman.findAudioInputFormat();
    if (!iformat)
        throw std::runtime_error("Couldn't find microphone input format.");

    AVDictionary* iparams = nullptr;
    if (sampleRate > 0)
        av_dict_set_int(&iparams, "sample_rate", sampleRate, 0);
    if (channels > 0)
        av_dict_set_int(&iparams, "channels", channels, 0);

    openStream(device.c_str(), iformat, &iparams);

    av_dict_free(&iparams); // FIXME: possible memory leak
}


} } // namespace scy::av


#endif
