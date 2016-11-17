///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/ffmpeg.h"

#ifdef HAVE_FFMPEG

#include "scy/mutex.h"

#include <iostream>
#include <stdexcept>


extern "C" {
#include <libavformat/avformat.h>
#ifdef HAVE_FFMPEG_AVDEVICE
#include <libavdevice/avdevice.h>
#endif
}


namespace scy {
namespace av {
namespace internal {


static int LockManagerOperation(void** lock, enum AVLockOp op)
{
    switch (op) {
        case AV_LOCK_CREATE:
            *lock= new Mutex();
            if (!*lock)
                return 1;
            return 0;

        case AV_LOCK_OBTAIN:
            static_cast<Mutex*>(*lock)->lock();
            return 0;

        case AV_LOCK_RELEASE:
            static_cast<Mutex*>(*lock)->unlock();
            return 0;

        case AV_LOCK_DESTROY:
            delete static_cast<Mutex*>(*lock);
            *lock= NULL;
            return 0;
    }
    return 1;
}


static Mutex _mutex;
static int _refCount(0);


void initialize()
{
    Mutex::ScopedLock lock(_mutex);

    if (++_refCount == 1) {
        // Optionally disable logging.
        // av_log_set_level(AV_LOG_QUIET);

        // Register our protocol glue code with FFmpeg.
        av_lockmgr_register(&LockManagerOperation);

        // Now register the rest of FFmpeg.
        av_register_all();

// And devices if available.
#ifdef HAVE_FFMPEG_AVDEVICE
        avdevice_register_all();
#endif
    }
}


void uninitialize()
{
    Mutex::ScopedLock lock(_mutex);

    if (--_refCount == 0) {
        av_lockmgr_register(NULL);
    }
}


} // namespace internal


void initializeFFmpeg()
{
    internal::initialize();
}


void uninitializeFFmpeg()
{
    internal::uninitialize();
}


std::string averror(const int error)
{
    static char error_buffer[255];
    av_strerror(error, error_buffer, sizeof(error_buffer));
    return error_buffer;
}


void printInputFormats(std::ostream& ost, const char* delim)
{
    initializeFFmpeg(); // init here so reference is not held
    AVInputFormat* p= av_iformat_next(nullptr);
    while (p) {
        ost << p->name << delim;
        p= av_iformat_next(p);
    }
    uninitializeFFmpeg();
}


void printOutputFormats(std::ostream& ost, const char* delim)
{
    initializeFFmpeg(); // init here so reference is not held
    AVOutputFormat* p= av_oformat_next(nullptr);
    while (p) {
        ost << p->name << delim;
        p= av_oformat_next(p);
    }
    uninitializeFFmpeg();
}


void printEncoders(std::ostream& ost, const char* delim)
{
    initializeFFmpeg(); // init here so reference is not held
    AVCodec* p= av_codec_next(nullptr);
    while (p) {
        if (av_codec_is_encoder(p))
            ost << p->name << delim;
        p= p->next;
    }
    uninitializeFFmpeg();
}


} // namespace av
} // namespace scy


#else

namespace scy {
namespace av {


void initializeFFmpeg()
{
}


void uninitializeFFmpeg()
{
}


} // namespace av
} // namespace scy


#endif


/// @\}
