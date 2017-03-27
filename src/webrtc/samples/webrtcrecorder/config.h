#ifndef SCY_WebRTC_WebRTCRecorder_Config_H
#define SCY_WebRTC_WebRTCRecorder_Config_H


#include "scy/base.h"
#include "scy/av/codec.h"
#include "scy/av/format.h"


#define SERVER_HOST "localhost"
#define USE_SSL 0 // 1
#if USE_SSL
#define SERVER_PORT 443
#else
#define SERVER_PORT 4500
#endif


#define OUTPUT_FILENAME "webrtcrecorder.mp4"
#define OUTPUT_FORMAT av::Format("MP4", "mp4",                                             \
               av::VideoCodec("H.264", "libx264", 400, 300, 25, 48000, 128000, "yuv420p"), \
               av::AudioCodec("AAC", "libfdk_aac", 2, 44100, 64000, "s16"));


#endif
