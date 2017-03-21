///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_FFmpeg_H
#define SCY_AV_FFmpeg_H


#include "scy/base.h"
#include <string>
#include <memory>


#ifdef HAVE_FFMPEG

extern "C" {
#include <libavformat/avformat.h>
#ifdef HAVE_FFMPEG_AVDEVICE
#include <libavdevice/avdevice.h>
#endif
}

#endif


namespace scy {
namespace av {


/// Initialize the FFmpeg library.
void initializeFFmpeg(); 

/// Uninitializes the FFmpeg library.
void uninitializeFFmpeg();

/// Get an error string for the given error code.
std::string averror(const int error); 

void printInputFormats(std::ostream& ost, const char* delim = " ");
void printOutputFormats(std::ostream& ost, const char* delim = " ");
void printEncoders(std::ostream& ost, const char* delim = " ");


#ifdef HAVE_FFMPEG

//
/// Utilites for RAII:

/// Deleter adaptor for functions like av_free that take a pointer
template<typename T, typename R, R(*Fn)(T*)> struct AV_Deleter {
    inline void operator() (T* p) const
    {
        if (p)
            Fn(p);
    }
};

/// Deleter adaptor for functions like av_freep that take a pointer to a pointer
template<typename T, typename R, R(*Fn)(T**)> struct AV_Deleterp {
    inline void operator() (T* p) const
    {
        if (p)
            Fn(&p);
    }
};

using AVFrameHolder = std::unique_ptr<
    AVFrame, AV_Deleterp<AVFrame, void, av_frame_free>
>;

// using AVFifoBufferHolder = std::unique_ptr<
//     AVFifoBuffer, AV_Deleter<AVFifoBuffer, void, av_fifo_free>
// >;

using AVFormatContextHolder = std::unique_ptr<
    AVFormatContext, AV_Deleter<AVFormatContext, void, avformat_free_context>
>;

using AVCodecContextHolder = std::unique_ptr<
    AVCodecContext, AV_Deleter<AVCodecContext, int, avcodec_close>
>;

using AVDictionaryCleanup = std::unique_ptr<
    AVDictionary*, AV_Deleter<AVDictionary*, void, av_dict_free>
>;

// using UFileHolder = std::unique_ptr<
//     AVIOContext, AV_Deleter<AVIOContext, int, ufile_close>
// >;

// template<typename T> using AVMallocHolder = std::unique_ptr<
//     T, AV_Deleter<void, void, av_free>
// >;

#endif


} // namespace av
} // namespace scy


#endif // SCY_Media_FFmpeg_H


/// @\}
