///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{

#include "scy/webrtc/peerconnectionmanager.h"
#include "scy/memory.h"

//#include "webrtc/api/test/fakeaudiocapturemodule.h"


using std::endl;


namespace scy {


PeerConnectionManager::PeerConnectionManager(rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory)
    : _factory(factory) // ? factory : webrtc::CreatePeerConnectionFactory()
{
}


PeerConnectionManager::~PeerConnectionManager()
{
}


void PeerConnectionManager::sendSDP(PeerConnection* conn, const std::string& type, const std::string& sdp)
{
    assert(0 && "virtual");
}


void PeerConnectionManager::sendCandidate(PeerConnection* conn, const std::string& mid, int mlineindex, const std::string& sdp)
{
    assert(0 && "virtual");
}


void PeerConnectionManager::recvSDP(const std::string& token, const json::value& message)
{
    auto conn = PeerConnectionManager::get(token, false);
    if (!conn) {
        assert(0 && "peer mismath");
        return;
    }

    std::string type = message.value(kSessionDescriptionTypeName, "");
    std::string sdp = message.value(kSessionDescriptionSdpName, "");
    if (sdp.empty() || (type != "offer" && type != "answer")) {
        ErrorL << "Received bad sdp: " << type << ": " << sdp << endl;
        assert(0 && "bad sdp");
        return;
    }

    conn->recvSDP(type, sdp);

    DebugL << "Received " << type << ": " << sdp << endl;
}


void PeerConnectionManager::recvCandidate(const std::string& token, const json::value& message)
{
    auto conn = PeerConnectionManager::get(token, false);
    if (!conn) {
        assert(0 && "peer mismath");
        return;
    }

    std::string mid = message.value(kCandidateSdpMidName, "");
    int mlineindex = message.value(kCandidateSdpMlineIndexName, -1);
    std::string sdp = message.value(kCandidateSdpName, "");
    if (mlineindex == -1 || mid.empty() || sdp.empty()) {
        ErrorL << "Invalid candidate format" << endl;
        assert(0 && "bad candiate");
        return;
    }

    DebugL << "Received candidate: " << sdp << endl;

    conn->recvCandidate(mid, mlineindex, sdp);
}


void PeerConnectionManager::onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream)
{
    assert(0 && "virtual");
}


void PeerConnectionManager::onRemoveRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream)
{
    assert(0 && "virtual");
}


void PeerConnectionManager::onStable(PeerConnection* conn)
{
}


void PeerConnectionManager::onClosed(PeerConnection* conn)
{
    DebugL << "Deleting peer connection: " << conn->peerid() << endl;

    if (remove(conn))
        deleteLater<PeerConnection>(conn); // async delete
}


void PeerConnectionManager::onFailure(PeerConnection* conn, const std::string& error)
{
    DebugL << "Deleting peer connection: " << conn->peerid() << endl;

    if (remove(conn))
        deleteLater<PeerConnection>(conn); // async delete
}


void PeerConnectionManager::setFactory(rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory)
{
    assert(!_factory);
    _factory = factory;
}


webrtc::PeerConnectionFactoryInterface* PeerConnectionManager::factory() const
{
    return _factory.get();
}


} // namespace scy


/// @\}
