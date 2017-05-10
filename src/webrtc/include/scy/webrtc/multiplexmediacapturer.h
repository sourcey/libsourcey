///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_WebRTC_MultiplexMediaCapturer_H
#define SCY_WebRTC_MultiplexMediaCapturer_H


#include "scy/av/av.h"

#ifdef HAVE_FFMPEG

#include "scy/av/mediacapture.h"
#include "scy/webrtc/audiopacketmodule.h"
#include "scy/webrtc/videopacketsource.h"

#include "webrtc/api/peerconnectioninterface.h"


namespace scy {


class MultiplexMediaCapturer
{
public:
    MultiplexMediaCapturer();
    virtual ~MultiplexMediaCapturer();

    virtual void openFile(const std::string& file, bool loop = true);

    virtual void addMediaTracks(webrtc::PeerConnectionFactoryInterface* factory,
                                webrtc::MediaStreamInterface* stream);

    virtual void start();
    virtual void stop();

    virtual rtc::scoped_refptr<AudioPacketModule> getAudioModule();
    virtual VideoPacketSource* createVideoSource();

protected:
    PacketStream _stream;
    av::MediaCapture::Ptr _videoCapture;
    rtc::scoped_refptr<AudioPacketModule> _audioModule;
};


} // namespace scy


#endif // HAVE_FFMPEG
#endif


/// @\}
