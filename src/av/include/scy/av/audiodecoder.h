///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_AudioDecoder_H
#define SCY_AV_AudioDecoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/packet.h"
#include "scy/av/audiocontext.h"


namespace scy {
namespace av {


struct AudioDecoder : public AudioContext
{
    AudioDecoder(AVStream* stream);
    virtual ~AudioDecoder();

    virtual void create() override;
    virtual void close() override;

    /// Decodes a the given input packet.
    /// Returns true an output packet was created, false otherwise.
    //virtual bool decode(uint8_t* data, int size);
    virtual bool decode(AVPacket& ipacket);

    /// Flushes buffered frames.
    /// This method should be called once after decoding.
    virtual void flush();
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_AudioDecoder_H


/// @\}
