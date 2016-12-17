///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_WebRTC_FilePeerConnection_H
#define SCY_WebRTC_FilePeerConnection_H


#include "scy/av/mediacapture.h"
#include "scy/webrtc/multiplexmediacapturer.h"
#include "scy/webrtc/peerconnection.h"


namespace scy {


class FilePeerConnection : public PeerConnection
{
public:
    /// Create the local file media stream.
    FilePeerConnection(PeerConnectionManager* manager,
                       const std::string& peerid,
                       const std::string& file);
    virtual ~FilePeerConnection();
    virtual rtc::scoped_refptr<webrtc::MediaStreamInterface>
    createMediaStream();

    virtual void OnIceConnectionChange(
        webrtc::PeerConnectionInterface::IceConnectionState new_state);

protected:
    std::string _file;
    MultiplexMediaCapturer _capturer;
};


} // namespace scy


#endif
