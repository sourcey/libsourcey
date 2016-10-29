//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SCY_WebRTC_WebRTCStreamer_Signaler_H
#define SCY_WebRTC_WebRTCStreamer_Signaler_H


#include "scy/webrtc/peerconnectionmanager.h"
#include "scy/application.h"
#include "scy/symple/client.h"
#include "scy/net/sslsocket.h"
#include "scy/net/sslmanager.h"
#include "scy/ipc.h"
#include "scy/util.h"


namespace scy {


class Signaler:
    public PeerConnectionManager,
    public Application
{
public:
    Signaler(const smpl::Client::Options& options);
    ~Signaler();

protected:
    // PeerConnectionManager interface
    void sendSDP(PeerConnection* conn, const std::string& type, const std::string& sdp);
    void sendCandidate(PeerConnection* conn, const std::string& mid, int mlineindex, const std::string& sdp);
    void onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);
    void onRemoveRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);

    void postMessage(const smpl::Message& m);
    void syncMessage(const ipc::Action& action);

    void onPeerConnected(void*, smpl::Peer& peer);
    void onPeerMessage(void*, smpl::Message& m);
    void onPeerDiconnected(void*, const smpl::Peer& peer);

    void onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState);

protected:
    ipc::SyncQueue<> _ipc;
#if USE_SSL
    smpl::SSLClient _client;
#else
    smpl::TCPClient _client;
#endif
};


} // namespace scy


#endif
