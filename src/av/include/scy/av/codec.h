///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_Codec_H
#define SCY_AV_Codec_H


#include "scy/av/av.h"

#include <iostream>
#include <list>
#include <string>


namespace scy {
namespace av {


// ---------------------------------------------------------------------
///// Defines a codec for encoding/decoding media.
struct Codec
{
    //
    // Base Codec Variables

    std::string name;    ///< The display name for this codec.
    std::string encoder; ///< The encoder name for FFmpeg.
    int sampleRate;      ///< The sampling rate or RTP clock rate.
    int bitRate;         ///< The bit rate to encode at.
    int quality; ///< Optional quality value, variable range depending on codec.
    bool enabled; ///< Weather or not the codec is available for use.

    //
    // Ctors/Dtors

    Codec();
    Codec(const std::string& name, int sampleRate = 0, int bitRate = 0,
          bool enabled = true);
    Codec(const std::string& name, const std::string& encoder,
          int sampleRate = 0, int bitRate = 0,
          bool enabled = true); /// Codec(const Codec& r);
    virtual ~Codec();

    //
    // Methods

    virtual std::string toString() const;
    virtual void print(std::ostream& ost);

    //
    // Operators
    // Codec& operator=(const Codec& r);
};


// ---------------------------------------------------------------------
//
#define DEFAULT_AUDIO_SAMPLE_RATE 44100
#define DEFAULT_AUDIO_BIT_RATE 64000
#define DEFAULT_AUDIO_CHANNELS 2
#define DEFAULT_AUDIO_SAMPLE_FMT "s16"


struct AudioCodec : public Codec
{
    int channels;
    std::string sampleFmt; ///< One of: u8, s16, s32, flt, dbl, u8p, s16p, s32p,
                           ///fltp, dblp
    /// int frameSize;  // Frame size (optional value set by encoder/decoder)
    /// /// int bufferSize; // Buffer size (optional value set by
    /// encoder/decoder)

    AudioCodec();
    AudioCodec(const std::string& name,
               int channels = 0,   // = DEFAULT_AUDIO_CHANNELS
               int sampleRate = 0, // = DEFAULT_AUDIO_SAMPLE_RATE
               int bitRate = 0,    // = DEFAULT_AUDIO_BIT_RATE
               const std::string& sampleFmt = ""); // = DEFAULT_AUDIO_SAMPLE_FMT
    AudioCodec(const std::string& name, const std::string& encoder,
               int channels = 0,   // = DEFAULT_AUDIO_CHANNELS
               int sampleRate = 0, // = DEFAULT_AUDIO_SAMPLE_RATE
               int bitRate = 0,    // = DEFAULT_AUDIO_BIT_RATE
               const std::string& sampleFmt = ""); // = DEFAULT_AUDIO_SAMPLE_FMT
    /// AudioCodec(const AudioCodec& r);
    virtual ~AudioCodec();

    virtual std::string toString() const;
    virtual void print(std::ostream& ost);

    /// AudioCodec& operator==(const AudioCodec& that);
};


// ---------------------------------------------------------------------
//
#define DEFAULT_VIDEO_SAMPLE_RATE                                              \
    384000 // 128-384 kbit/s for business-oriented videoconferencing quality
           // using video compression
#define DEFAULT_VIDEO_BIT_RATE 400000
#define DEFAULT_VIDEO_PIXEL_FMT "yuv420p"


struct VideoCodec : public Codec
{
    int width;
    int height;
    double fps;
    std::string pixelFmt;

    VideoCodec();
    VideoCodec(const std::string& name, int width = 0, int height = 0,
               double fps = 20, int bitRate = DEFAULT_VIDEO_BIT_RATE,
               int sampleRate = DEFAULT_VIDEO_SAMPLE_RATE,
               const std::string& pixelFmt = DEFAULT_VIDEO_PIXEL_FMT);
    VideoCodec(const std::string& name, const std::string& encoder,
               int width = 0, int height = 0, double fps = 20,
               int bitRate = DEFAULT_VIDEO_BIT_RATE,
               int sampleRate = DEFAULT_VIDEO_SAMPLE_RATE,
               const std::string& pixelFmt = DEFAULT_VIDEO_PIXEL_FMT);
    VideoCodec(const VideoCodec& r);
    virtual ~VideoCodec();

    virtual std::string toString() const;
    virtual void print(std::ostream& ost);
};


typedef std::list<Codec> CodecList;
typedef std::list<Codec*> CodecPList;


} // namespace av
} // namespace scy


#endif
