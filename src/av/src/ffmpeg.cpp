///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/ffmpeg.h"
#include "scy/logger.h"
#include "scy/util.h"

#ifdef HAVE_FFMPEG

#include <mutex>
#include <iostream>
#include <stdexcept>


namespace scy {
namespace av {
namespace internal {


static int LockManagerOperation(void** lock, enum AVLockOp op)
{
    switch (op) {
        case AV_LOCK_CREATE:
            *lock = new std::mutex();
            if (!*lock)
                return 1;
            return 0;

        case AV_LOCK_OBTAIN:
            static_cast<std::mutex*>(*lock)->lock();
            return 0;

        case AV_LOCK_RELEASE:
            static_cast<std::mutex*>(*lock)->unlock();
            return 0;

        case AV_LOCK_DESTROY:
            delete static_cast<std::mutex*>(*lock);
            *lock = NULL;
            return 0;
    }
    return 1;
}


static std::mutex _mutex;
static int _refCount(0);


static void logCallback(void *ptr, int level, const char *fmt, va_list vl)
{
    char logbuf[256];
    vsnprintf(logbuf, sizeof(logbuf), fmt, vl);
    logbuf[sizeof(logbuf) - 1] = '\0';

    switch (level) {
    case AV_LOG_PANIC:
    case AV_LOG_FATAL:
    case AV_LOG_ERROR:
        LError("FFmpeg: ", util::trimRight<std::string>(logbuf))
        break;
    case AV_LOG_WARNING:
        LWarn("FFmpeg: ", util::trimRight<std::string>(logbuf))
        break;
    case AV_LOG_INFO:
        LDebug("FFmpeg: ", util::trimRight<std::string>(logbuf))
        break;
    default:
    case AV_LOG_VERBOSE:
    case AV_LOG_DEBUG:
        // LTrace("FFmpeg: ", util::trimRight<std::string>(logbuf))
        break;
    }
}


void init()
{
    std::lock_guard<std::mutex> guard(_mutex);

    if (++_refCount == 1) {
        // Use an internal log callback.
        // av_log_set_callback(logCallback);
        // av_log_set_level(AV_LOG_INFO);

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


void uninit()
{
    std::lock_guard<std::mutex> guard(_mutex);

    if (--_refCount == 0) {
        av_lockmgr_register(NULL);
    }
}


} // namespace internal


void initializeFFmpeg()
{
    internal::init();
}


void uninitializeFFmpeg()
{
    internal::uninit();
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
    AVInputFormat* p = av_iformat_next(nullptr);
    while (p) {
        ost << p->name << delim;
        p = av_iformat_next(p);
    }
    uninitializeFFmpeg();
}


void printOutputFormats(std::ostream& ost, const char* delim)
{
    initializeFFmpeg(); // init here so reference is not held
    AVOutputFormat* p = av_oformat_next(nullptr);
    while (p) {
        ost << p->name << delim;
        p = av_oformat_next(p);
    }
    uninitializeFFmpeg();
}


void printEncoders(std::ostream& ost, const char* delim)
{
    initializeFFmpeg(); // init here so reference is not held
    AVCodec* p = av_codec_next(nullptr);
    while (p) {
        if (av_codec_is_encoder(p))
            ost << p->name << delim;
        p = p->next;
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
