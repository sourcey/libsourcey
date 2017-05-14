///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/ffmpegvideobuffer.h"
//#include "scy/av/ffmpeg.h"
//#include "scy/av/videocontext.h"


using std::endl;


namespace scy {
namespace wrtc {


FFmpeg420Buffer::FFmpeg420Buffer(int width, int height,
    const uint8_t* y_plane, int y_stride,
    const uint8_t* u_plane, int u_stride,
    const uint8_t* v_plane, int v_stride)
    : width_(width)
    , height_(height)
    //, ydata(y_plane, y_plane + y_stride)
    //, udata(u_plane, u_plane + u_stride)
    //, vdata(v_plane, v_plane + v_stride)
    , ydata(y_plane, y_plane + (y_stride * height))
    , udata(u_plane, u_plane + (u_stride * (height / 2)))
    , vdata(v_plane, v_plane + (v_stride * (height / 2)))
    //, y_plane_(y_plane)
    //, u_plane_(u_plane)
    //, v_plane_(v_plane)
    //, y_stride_(y_stride)
    //, u_stride_(u_stride)
    //, v_stride_(v_stride)
{
}

FFmpeg420Buffer::FFmpeg420Buffer(const av::PlanarVideoPacket& packet)
    : width_(packet.width)
    , height_(packet.height)
    , ydata(packet.ydata)
    , udata(packet.udata)
    , vdata(packet.vdata)
{
}

FFmpeg420Buffer::~FFmpeg420Buffer() 
{
    //no_longer_used_cb_();
}


int FFmpeg420Buffer::width() const 
{
    return width_;
}


int FFmpeg420Buffer::height() const
{
    return height_;
}


const uint8_t* FFmpeg420Buffer::DataY() const 
{
    return ydata.data();
}

const uint8_t* FFmpeg420Buffer::DataU() const
{
    return udata.data();
}

const uint8_t* FFmpeg420Buffer::DataV() const 
{
    return vdata.data();
}

int FFmpeg420Buffer::StrideY() const 
{
    return 640; //ydata.size();
}

int FFmpeg420Buffer::StrideU() const 
{
    return 320; //udata.size();
}

int FFmpeg420Buffer::StrideV() const 
{
    return 320; // vdata.size();
}

void* FFmpeg420Buffer::native_handle() const 
{
    return nullptr;
}

rtc::scoped_refptr<webrtc::VideoFrameBuffer> FFmpeg420Buffer::NativeToI420Buffer()
{
    RTC_NOTREACHED();
    return nullptr;
}


} } // namespace scy::wrtc


/// @\}
