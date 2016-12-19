///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_PeerConnectionManager_H
#define SCY_PeerConnectionManager_H


#include "scy/collection.h"
#include "scy/json/json.h"
#include "scy/webrtc/peerconnection.h"
#include "scy/webrtc/webrtc.h"

#include "webrtc/api/peerconnectioninterface.h"

#include <iostream>
#include <string>


namespace scy {


class PeerConnection;


class PeerConnectionManager
    : public PointerCollection<std::string, PeerConnection>
{
public:
    PeerConnectionManager(rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory = nullptr);
    virtual ~PeerConnectionManager();

    virtual void sendSDP(PeerConnection* conn, const std::string& type, const std::string& sdp);
    virtual void sendCandidate(PeerConnection* conn, const std::string& mid, int mlineindex, const std::string& sdp);

    virtual void recvSDP(const std::string& token, const json::Value& data);
    virtual void recvCandidate(const std::string& token, const json::Value& data);

    virtual void onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);
    virtual void onRemoveRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);

    virtual void onStable(PeerConnection* conn);
    virtual void onClosed(PeerConnection* conn);
    virtual void onFailure(PeerConnection* conn, const std::string& error);

    webrtc::PeerConnectionFactoryInterface* factory() const;

protected:
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _factory;
};


} // namespace scy


#endif


/// @\}
