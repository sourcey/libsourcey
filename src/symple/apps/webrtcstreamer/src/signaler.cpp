#include "signaler.h"
#include "videostreamcapturer.h"
#include "peerconnectionclient.h"

#include <iostream>
#include <string>


using std::endl;


namespace scy {


// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";


Signaler::Signaler(const smpl::Client::Options& options) :
    client(options)
{
    client.StateChange += sdelegate(this, &Signaler::onClientStateChange);
    client.roster().ItemAdded += sdelegate(this, &Signaler::onPeerConnected);
    client.roster().ItemRemoved += sdelegate(this, &Signaler::onPeerDiconnected);
    client += smpl::messageDelegate(this, &Signaler::onPeerMessage);
    client.connect();
}


Signaler::~Signaler()
{
}


void Signaler::broadcastSDP(const std::string& type, const std::string sdp)
{
    assert(type == "offer" || type == "answer");
    smpl::Message m;
    Json::Value desc;
    desc[kSessionDescriptionTypeName] = type;
    desc[kSessionDescriptionSdpName] = sdp;
    m[type] = desc;

    postMessage(m);
}


void Signaler::broadcastCandidate(const std::string& mid, int mlineindex, const std::string sdp)
{
    smpl::Message m;
    Json::Value desc;
    desc[kCandidateSdpMidName] = mid;
    desc[kCandidateSdpMlineIndexName] = mlineindex;
    desc[kCandidateSdpName] = sdp;
    m["candidate"] = desc;

    postMessage(m);
}


void Signaler::postMessage(const smpl::Message& m)
{
    ipc.push(new ipc::Action(std::bind(&Signaler::syncMessage, this, std::placeholders::_1), m.clone()));
}


void Signaler::syncMessage(const ipc::Action& action)
{
    auto m = reinterpret_cast<smpl::Message*>(action.arg);
    client.send(*m);
    delete m;
}


void Signaler::onPeerConnected(void*, smpl::Peer& peer)
{
    if (peer.id() == client.ourID()) return;
    DebugL << "Peer connected: " << peer.id()  << endl;

    if (manager.exists(peer.id())) {
        DebugL << "Peer already has session: " << peer.id() << endl;
        return;
    }

    auto conn = new PeerConnectionClient(*this, peer.id());
    if (!conn->initConnection()) {
        ErrorL << "Cannot initialize peer connection: " << peer.id() << endl;
        assert(0);
        return;
    }

    // Uncomment to always initiate a video session with the connecting peer.
    conn->createOffer();

    manager.add(peer.id(), conn);
}


void Signaler::onPeerMessage(void*, smpl::Message& m)
{
    DebugL << "Peer message: " << m.from().toString() << endl;

    if (m.isMember("offer")) {
        assert(0 && "offer not supported");
    }

    else if (m.isMember("answer")) {
        auto conn = manager.get(m.from().id, false);
        if (!conn) {
            assert(0 && "peer mismath");
            return;
        }

        std::string type = m["answer"].get(kSessionDescriptionTypeName, "").asString();
        std::string sdp = m["answer"].get(kSessionDescriptionSdpName, "").asString();
        if (sdp.empty() || type != "answer") {
            ErrorL << "Received bad answer: " << sdp << endl;
            assert(0 && "bad sdp");
            return;
        }

        if (!conn->handleRemoteAnswer(type, sdp)) {
            ErrorL << "Failed to parse answer" << endl;
            assert(0 && "cannot parse sdp");
            return;
        }

        DebugL << "Received answer: " << sdp << endl;
    }

    else if (m.isMember("candidate")) {
        auto conn = manager.get(m.from().id, false);
        if (!conn) {
            assert(0 && "peer mismath");
            return;
        }

        std::string mid = m["candidate"].get(kCandidateSdpMidName, "").asString();
        int mlineindex = m["candidate"].get(kCandidateSdpMlineIndexName, -1).asInt();
        std::string sdpCandidate = m["candidate"].get(kCandidateSdpName, "").asString();
        if (mlineindex == -1 || mid.empty() || sdpCandidate.empty()) {
            ErrorL << "Invalid candidate format" << endl;
            assert(0 && "bad candiate");
            return;
        }

        DebugL << "Received candidate:" << sdpCandidate << endl;

        if (!conn->handleRemoteCandidate(mid, mlineindex, sdpCandidate)) {
            ErrorL << "Failed to apply the received candidate" << endl;
            assert(0 && "cannot parse candiate");
            return;
        }
    }

    // else assert(0 && "unknown event");
}


void Signaler::onPeerDiconnected(void*, const smpl::Peer& peer)
{
    DebugL << "Peer disconnected" << endl;

    auto conn = manager.remove(peer.id());
    if (conn) {
        DebugL << "Deleting peer connection: " << peer.id() << endl;
        delete conn;
    }
}


void Signaler::onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState)
{
    DebugL << "Client state changed from " << oldState << " to " << state << endl;

    switch (state.id()) {
    case sockio::ClientState::Connecting:
        break;
    case sockio::ClientState::Connected:
        break;
    case sockio::ClientState::Online:
        break;
    case sockio::ClientState::Error:
        throw std::runtime_error("Cannot connect to Symple server. " \
            "Did you start the demo app and the Symple server is running on port 4500?");
    }
}


} // namespace scy
