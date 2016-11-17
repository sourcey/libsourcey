///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//

#ifndef SCY_WebRTC_FilePeerConnection_H
#define SCY_WebRTC_FilePeerConnection_H

#include "scy/av/mediacapture.h"
#include "scy/webrtc/multiplexmediacapturer.h"
#include "scy/webrtc/peerconnection.h"

namespace scy {

class FilePeerConnection : public PeerConnection
{
public:
    FilePeerConnection(PeerConnectionManager* manager,
                       const std::string& peerid, Mode mode);
    virtual ~FilePeerConnection(); /// Create the local media stream.
    virtual rtc::scoped_refptr<webrtc::MediaStreamInterface>
    createMediaStream();

    virtual void OnIceConnectionChange(
        webrtc::PeerConnectionInterface::IceConnectionState new_state);

protected:
    MultiplexMediaCapturer _capturer;
};

} // namespace scy

#endif
