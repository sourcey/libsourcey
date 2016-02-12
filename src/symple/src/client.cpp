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


#include "scy/symple/client.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/sslsocket.h"


using std::endl;


namespace scy {
namespace smpl {


//
// TCP Client
//


Client* createTCPClient(const Client::Options& options, uv::Loop* loop)
{
    return new Client(std::make_shared<net::TCPSocket>(loop), options); //, loop
}


TCPClient::TCPClient(const Client::Options& options, uv::Loop* loop) :
    Client(std::make_shared<net::TCPSocket>(loop), options) //, loop
{
}


//
// SSL Client
//


Client* createSSLClient(const Client::Options& options, uv::Loop* loop)
{
    return new Client(std::make_shared<net::SSLSocket>(loop), options); //, loop);
}


SSLClient::SSLClient(const Client::Options& options, uv::Loop* loop) :
    Client(std::make_shared<net::SSLSocket>(loop), options) //, loop)
{
}


//
// Client implementation
//


Client::Client(const net::Socket::Ptr& socket, const Client::Options& options) : //, uv::Loop* loop
    sockio::Client(socket), //, loop
    _options(options),
    _announceStatus(500)
{
    TraceL << "Create" << endl;
}


Client::~Client()
{
    TraceL << "Destroy" << endl;
}


void Client::connect()
{
    TraceL << "Connecting" << endl;
    assert(!_options.user.empty());
    _host = _options.host;
    _port = _options.port;
    sockio::Client::connect();
}


void Client::close()
{
    TraceL << "Closing" << endl;
    sockio::Client::close();
}


int Client::send(Message& m, bool ack)
{
    if (!isOnline()) {
        //assert(0); // may be announcing
        throw std::runtime_error("Cannot send message while offline.");
    }

    m.setFrom(ourPeer()->address());
    //assert(isOnline()); // may be announcing

      TraceL << "Sending message 11111111111111: " << json::stringify(m, true) << endl;

            TraceL << "Sending message FRTOM: " << ourPeer()->address() << endl;
                  TraceL << "Sending message FRTOM: " << m.from().id << endl;
                        TraceL << "Sending message TO: " << m.to().id << endl;

    if (m.to().id == m.from().id) {
        assert(0);
        throw std::runtime_error("Cannot send message with matching sender and recipient.");
    }

    if (!m.valid()) {
        assert(0);
        throw std::runtime_error("Cannot send invalid message.");
    }
    // #ifdef _DEBUG
        TraceL << "Sending message: " << json::stringify(m, true) << endl;
    // #endif

    return sockio::Client::send(m, ack);
}


int Client::send(const std::string& data, bool ack)
{
    Message m;
    if (!m.read(data))
        throw std::runtime_error("Cannot send malformed message.");
    return send(m, ack);
}


int Client::respond(Message& m, bool ack)
{
    m.setTo(m.from());
    return send(m, ack);
}


void Client::createPresence(Presence& p)
{
    TraceL << "Create presence" << endl;

    auto peer = ourPeer();
    if (peer) {
        CreatePresence.emit(this, *peer);
        p["data"] = *peer;
    }
    else assert(0 && "no session");
}


int Client::sendPresence(bool probe)
{
    TraceL << "Broadcasting presence" << endl;

    Presence p;
    createPresence(p);
    p.setProbe(probe);
    return send(p);
}


int Client::sendPresence(const Address& to, bool probe)
{
    TraceL << "Sending presence" << endl;

    Presence p;
    createPresence(p);
    p.setProbe(probe);
    p.setTo(to);
    return send(p);
}


int Client::announce()
{
    TraceL << "Announcing" << endl;

    json::Value data;
    data["name"] = _options.name;
    data["user"] = _options.user;
    data["type"] = _options.type;
    data["token"] = _options.token;
    sockio::Packet pkt("announce", data, true);
    auto txn = createTransaction(pkt);
    txn->StateChange += sdelegate(this, &Client::onAnnounce);
    return txn->send();
}


void Client::onAnnounce(void* sender, TransactionState& state, const TransactionState&)
{
    TraceL << "On announce response: " << state << endl;

    auto transaction = reinterpret_cast<sockio::Transaction*>(sender);
    switch (state.id()) {
    case TransactionState::Success:
        try {
            json::Value data = transaction->response().json(); //[(unsigned)0];
            _announceStatus = data["status"].asInt();

            if (_announceStatus != 200)
                throw std::runtime_error(data["message"].asString());

            _ourID = data["data"]["id"].asString(); //Address();
            if (_ourID.empty())
                throw std::runtime_error("Invalid server response.");

            // Set our local peer data from the response or fail.
            onPresenceData(data["data"], true);

            // Notify the outside application of the response
            // status before we transition the client state.
            Announce.emit(this, _announceStatus);

            // Transition to Online state.
            sockio::Client::onOnline();

            // Broadcast a presence probe to our network.
            sendPresence(true);
        }
        catch (std::exception& exc) {
            // Set the error message and close the connection.
            setError("Announce failed: " + std::string(exc.what()));
        }
        break;

    case TransactionState::Failed:
        Announce.emit(this, _announceStatus);
        setError(state.message());
        break;
    }
}


void Client::onOnline()
{
    // // Start the socket.io timers etc
    // sockio::Client::onOnline();

    // NOTE: Do not transition the Socket.IO client to Online state here
    // It will be done via the Announce callback

    // Announce the Symple Client
    announce();
}


void Client::emit(IPacket& raw)
{
    auto packet = reinterpret_cast<sockio::Packet&>(raw);

    // Parse Symple messages from SocketIO packets
    if (packet.type() == sockio::Packet::Type::Event) {
        TraceL << "JSON packet: " << packet.toString() << endl;

        json::Value data = packet.json();
        // json::Reader reader;
        if (data.isMember("type")) {
            std::string type(data["type"].asString());
#ifdef _DEBUG
            TraceL << "Received " << type << ": " << json::stringify(data, true) << endl;
#endif

            // KLUDGE: Note we are currently creating the JSON object
            // twice with these polymorphic message classes. Perhaps
            // free functions are a better for working with messages.
            if (type == "message") {
                Message m(data);
                if (!m.valid()) {
                    WarnL << "Dropping invalid message: " << json::stringify(data, false) << endl;
                    return;
                }
                PacketSignal::emit(this, m);
            }
            else if (type == "event") {
                Event e(data);
                if (!e.valid()) {
                    WarnL << "Dropping invalid event: " << json::stringify(data, false) << endl;
                    return;
                }
                PacketSignal::emit(this, e);
            }
            else if (type == "presence") {
                Presence p(data);
                if (!p.valid()) {
                    WarnL << "Dropping invalid presence: " << json::stringify(data, false) << endl;
                    return;
                }
                if (p.isMember("data")) {
                    onPresenceData(p["data"], false);
                    if (p.isProbe())
                        sendPresence(p.from());
                }
            }
            else if (type == "command") {
                Command c(data);
                if (!c.valid()) {
                    WarnL << "Dropping invalid command: " << json::stringify(data, false) << endl;
                    return;
                }
                PacketSignal::emit(this, c);
                if (c.isRequest()) {
                    c.setStatus(404);
                    WarnL << "Command not handled: " << c.id() << ": " << c.node() << endl;
                    respond(c);
                }
            }
            else
                WarnL << "Received non-standard message: " << type << endl;
        }
        else {
          assert(0 && "invalid packet");
          WarnL << "Invalid Symple message" << endl;
        }
    }

    // Other packet types are proxied directly
    else {
        TraceL << "Proxying packet" << endl;
        PacketSignal::emit(this, packet);
    }
}


void Client::onPresenceData(const json::Value& data, bool whiny)
{
    TraceL << "Updating: " << json::stringify(data, true) << endl;

    if (data.isObject() &&
        data.isMember("id") &&
        data.isMember("user") &&
        data.isMember("name") &&
        data.isMember("online") //&&
        //data.isMember("type")
        ) {
        std::string id = data["id"].asString();
        bool online = data["online"].asBool();
        Peer* peer = _roster.get(id, false);
        if (online) {
            if (!peer) {
                peer = new Peer(data);
                _roster.add(id, peer);
                InfoL << "Peer connected: " << peer->address().toString() << endl;
                PeerConnected.emit(this, *peer);
            }
            else
                static_cast<json::Value&>(*peer) = data;
        }
        else {
            if (peer) {
                InfoL << "Peer disconnected: " << peer->address().toString() << endl;
                PeerDisconnected.emit(this, *peer);
                _roster.free(id);
            }
            else {
                WarnL << "Got peer disconnected for unknown peer: " << id << endl;
            }
        }
    }
    else {
        std::string error("Bad presence data: " + json::stringify(data));
        ErrorL << error << endl;
        if (whiny)
            throw std::runtime_error(error);
    }

#if 0
    if (data.isObject() &&
        data.isMember("id") &&
        data.isMember("user") &&
        data.isMember("name") //&&
        //data.isMember("type")
        ) {
        TraceL << "Updating: " << json::stringify(data, true) << endl;
        std::string id = data["id"].asString();
        Peer* peer = get(id, false);
        if (!peer) {
            peer = new Peer(data);
            add(id, peer);
        } else
            static_cast<json::Value&>(*peer) = data;
    }
    else if (data.isArray()) {
        for (auto it = data.begin(); it != data.end(); it++) {
            onPresenceData(*it, whiny);
        }
    }
    else {
        std::string error("Bad presence data: " + json::stringify(data));
        ErrorL << error << endl;
        if (whiny)
            throw std::runtime_error(error);
    }
#endif
}


void Client::reset()
{
    // Note: Not clearing persisted messages just in case
    // they are still in use by the outside application
    //_persistence.clear();

    _roster.clear();
    _announceStatus = -1;
    _ourID = "";
    sockio::Client::reset();
}


Roster& Client::roster()
{
    //Mutex::ScopedLock lock(_mutex);
    return _roster;
}


PersistenceT& Client::persistence()
{
    //Mutex::ScopedLock lock(_mutex);
    return _persistence;
}


Client::Options& Client::options()
{
    //Mutex::ScopedLock lock(_mutex);
    return _options;
}


std::string Client::ourID() const
{
    //Mutex::ScopedLock lock(_mutex);
    return _ourID;
}


Peer* Client::ourPeer()
{
    //Mutex::ScopedLock lock(_mutex);
    if (_ourID.empty())
        return nullptr;
        //throw std::runtime_error("No active peer session is available.");
    return _roster.get(_ourID, false);
}


Client& Client::operator >> (Message& message)
{
    send(message);
    return *this;
}


} } // namespace scy::smpl
