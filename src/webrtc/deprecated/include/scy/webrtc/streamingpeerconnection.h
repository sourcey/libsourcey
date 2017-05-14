///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_WebRTC_StreamingPeerConnection_H
#define SCY_WebRTC_StreamingPeerConnection_H


#include "scy/av/mediacapture.h"
#include "scy/webrtc/multiplexmediacapturer.h"
#include "scy/webrtc/peerconnection.h"


namespace scy {
namespace wrtc {


class StreamingPeerConnection : public PeerConnection
{
public:
    /// Create the streaming peer connection.
    StreamingPeerConnection(PeerConnectionManager* manager,
                            const std::string& peerid,
                            const std::string& token,
                            const std::string& file);
    virtual ~StreamingPeerConnection();
    virtual rtc::scoped_refptr<webrtc::MediaStreamInterface>
    createMediaStream();

    virtual void OnIcePeerChange(
        webrtc::PeerConnectionInterface::IcePeerState new_state);

protected:
    std::string _file;
    MultiplexMediaCapturer _capturer;

    std::unique_ptr<rtc::Thread> _networkThread;
    std::unique_ptr<rtc::Thread> _workerThread;
    // std::unique_ptr<rtc::Thread> _signalingThread;
};


} } // namespace scy::wrtc


#endif
