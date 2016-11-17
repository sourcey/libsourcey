///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


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


AudioCapture::AudioCapture(const std::string& device, int channels,
                           int sampleRate)
{
    open(device, channels, sampleRate);
}


AudioCapture::~AudioCapture()
{
}


void AudioCapture::open(const std::string& device, int channels, int sampleRate)
{
    TraceS(this) << "Opening microphone: "
                 << "device=" << device << ", "
                 << "channels=" << channels << ", "
                 << "sampleRate=" << sampleRate << endl;

    DeviceManager devman;
    auto iformat= devman.findAudioInputFormat();
    if (!iformat)
        throw std::runtime_error("Couldn't find microphone input format.");

    AVDictionary* iparams= nullptr;
    if (sampleRate > 0)
        av_dict_set_int(&iparams, "sample_rate", sampleRate, 0);
    if (channels > 0)
        av_dict_set_int(&iparams, "channels", channels, 0);

    openStream(device.c_str(), iformat, &iparams);

    av_dict_free(&iparams); // FIXME: possible memory leak if exception thrown
}


} // namespace av
} // namespace scy


#endif


/// @\}
