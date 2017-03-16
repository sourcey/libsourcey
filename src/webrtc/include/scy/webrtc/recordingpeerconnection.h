///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_WebRTC_RecordingPeerConnection_H
#define SCY_WebRTC_RecordingPeerConnection_H


#include "scy/av/mediacapture.h"
#include "scy/webrtc/multiplexmediacapturer.h"
#include "scy/webrtc/peerconnection.h"
#include "scy/webrtc/streamrecorder.h"


namespace scy {


/// Peer connection class for recording the remote peer video.
class RecordingPeerConnection : public PeerConnection
{
public:
    /// Create the recording peer connection.
    RecordingPeerConnection(PeerConnectionManager* manager,
                            const std::string& peerid,
                            const std::string& token,
                            const av::EncoderOptions options = av::EncoderOptions());
    virtual ~RecordingPeerConnection();

    /// inherited from PeerConnectionObserver
    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state);

protected:
    std::unique_ptr<StreamRecorder> _recorder;
};


} // namespace scy


#endif
