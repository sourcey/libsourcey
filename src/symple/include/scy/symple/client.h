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


#ifndef SCY_Symple_Client_H
#define SCY_Symple_Client_H


#include "scy/bitwise.h"
#include "scy/socketio/client.h"
#include "scy/net/socket.h"
#include "scy/http/websocket.h"
#include "scy/util/timedmanager.h"
#include "scy/symple/roster.h"
#include "scy/symple/message.h"
#include "scy/symple/presence.h"
#include "scy/symple/command.h"
#include "scy/symple/event.h"
#include "scy/symple/form.h"
#include "scy/symple/peer.h"


namespace scy {
namespace smpl {


typedef TimedManager<std::string, Message> PersistenceT;


//
// Symple Client
//


class Client: public sockio::Client
{
public:
    struct Options
    {
        std::string host;
        std::uint16_t port;

        bool reconnection;
            // Weather or not to reconnect if disconnected from the server.

        int reconnectAttempts;
            // The number of times to attempt to reconnect if disconnected
            // from the server. (0 = unlimited)

        std::string user;
        std::string name;
        std::string type;
        std::string token;

        Options() {
            host = "127.0.0.1";
            port = 4500;

            reconnection = true;
            reconnectAttempts = 0;

            user  = "";
            name  = "";
            type  = "";
            token = "";
        }
    };

public:
    Client(const net::Socket::Ptr& socket, const Options& options = Options());
    virtual ~Client();

    void connect();
    void close();

    virtual int send(Message& message, bool ack = false);
        // Sends a message.
        // May be a polymorphic Command, Presence, Event or other ...

    virtual int send(const std::string& message, bool ack = false);
        // Sends a string message.
        // The message must be a valid Symple message otherwise
        // it will net be delivered.

    virtual int respond(Message& message, bool ack = false);
        // Swaps the 'to' and 'from' fields and sends
        // the given message.

    virtual int sendPresence(bool probe = false);
        // Broadcasts presence to the user group scope.
        // The outgoing Presence object may be modified via
        // the CreatePresence signal.

    virtual int sendPresence(const Address& to, bool probe = false);
        // Sends directed presence to the given peer.

    virtual std::string ourID() const;
        // Returns the session ID of our current peer object.
        // Returns an empty string when offline.

    virtual Peer* ourPeer();
        // Returns the peer object for the current session,
        // or throws an exception when offline.

    virtual Roster& roster();
        // Returns the roster which stores all online peers.

    virtual PersistenceT& persistence();
        // Returns the persistence manager which stores
        // long lived messages.

    Client::Options& options();
        // Return a reference to the client options object.

    virtual Client& operator >> (Message& message);
        // Stream operator alias for send().

    virtual void onPresenceData(const json::Value& data, bool whiny = false);
        // Updates the roster from the given client object.

    //
    // Signals
    //

    Signal<const int&> Announce;
        // Notifies the outside application about the
        // response status code of our announce() call.
        // Possible status codes are:
        //   - 200: Authentication success
        //   - 401: Authentication failed
        //   - 400: Bad request data
        //   - 500: Server not found

    Signal<Peer&> PeerConnected;
        // Signals when a peer connects.

    Signal<Peer&> PeerDisconnected;
        // Signals when a peer disconnects.

    Signal<Peer&> CreatePresence;
        // Called by createPresence() so outside classes
        // can modify the outgoing Peer JSON object.

protected:
    virtual int announce();
        // Called when a new connection is established
        // to announce and authenticate the peer on the
        // server.

    virtual void reset();
        // Resets variables and data at the beginning
        // and end of each session.

    virtual void createPresence(Presence& p);
        // Creates a Presence object.

    virtual void emit(IPacket& packet);
        // Override PAcketStream::emit

    virtual void onOnline();
    virtual void onAnnounceState(void* sender, TransactionState& state, const TransactionState&);
    // virtual void onPacket(sockio::Packet& packet);

protected:
    Roster _roster;
    std::string _ourID;
    PersistenceT _persistence;
    Client::Options _options;
    int _announceStatus;
};


//
// TCP Client
//


Client* createTCPClient(const Client::Options& options = Client::Options(), uv::Loop* loop = uv::defaultLoop());


class TCPClient: public Client
{
public:
    TCPClient(const Client::Options& options = Client::Options(), uv::Loop* loop = uv::defaultLoop());
};


//
// SSL Client
//


Client* createSSLClient(const Client::Options& options = Client::Options(), uv::Loop* loop = uv::defaultLoop());


class SSLClient: public Client
{
public:
    SSLClient(const Client::Options& options = Client::Options(), uv::Loop* loop = uv::defaultLoop());
};


//
// Filters
//


enum FilterFlags
{
    AcceptRequests        = 0x01,
    AcceptResponses        = 0x02
};


struct Filter//: public Flaggable
{
    Filter(const std::string& path, unsigned flags = 0) :
        flags(flags), path(path) {}

    Filter(unsigned flags = 0) :
        flags(flags), path("*") {}

    Bitwise flags;
    std::string path;
};


//
// Polymorphic Message Delegates
//


struct MessageDelegate: public PacketDelegateBase
{
    typedef Filter DataT;
    Filter filter;

    MessageDelegate(const Filter& filter = Filter()) : filter(filter) {};
    MessageDelegate(const MessageDelegate& r) : filter(r.filter) {};

    virtual bool accepts(void* /* sender */, IPacket& data, void*, void*, void*)
    {
        auto packet = dynamic_cast<Message*>(&data);
        if (packet &&
            (!filter.flags.has(AcceptRequests) ||
                (filter.flags.has(AcceptRequests) && packet->isRequest())) &&
            (!filter.flags.has(AcceptResponses) ||
                (filter.flags.has(AcceptResponses) && !packet->isRequest()))) {
            return true;
        }
        return false;
    }
};


struct CommandDelegate: public MessageDelegate
{
    CommandDelegate(const Filter& filter = Filter()) : MessageDelegate(filter) {};
    CommandDelegate(const CommandDelegate& r) : MessageDelegate(r) {};

    virtual bool accepts(void* sender, IPacket& data, void* empty2, void* empty3, void* empty4)
    {
        if (MessageDelegate::accepts(sender, data, empty2, empty3, empty4)) {
            auto c = dynamic_cast<Command*>(&data);
            return c && c->matches(filter.path);
        }
        return false;
    }
};


struct PresenceDelegate: public MessageDelegate
{
    PresenceDelegate() : MessageDelegate(AcceptRequests) {};
    PresenceDelegate(const PresenceDelegate& r) : MessageDelegate(r) {};

    virtual bool accepts(void* sender, IPacket& data, void* empty2, void* empty3, void* empty4)
    {
        if (MessageDelegate::accepts(sender, data, empty2, empty3, empty4)) {
            auto p = dynamic_cast<Presence*>(&data);
            return p && p->type() == "presence";
        }
        return false;
    }
};


struct EventDelegate: public MessageDelegate
{
    EventDelegate() : MessageDelegate(AcceptRequests) {};
    EventDelegate(const EventDelegate& r) : MessageDelegate(r) {};

    virtual bool accepts(void* sender, IPacket& data, void* empty2, void* empty3, void* empty4)
    {
        if (MessageDelegate::accepts(sender, data, empty2, empty3, empty4)) {
            auto e = dynamic_cast<Event*>(&data);
            return e && e->type() == "event" && (
                filter.path.empty() || filter.path == "*" ||
                util::matchNodes(e->name(), filter.path, ":"));
        }
        return false;
    }
};


DefinePolymorphicDelegateWithArg(messageDelegate, IPacket, MessageDelegate, const Filter&, Filter())
DefinePolymorphicDelegateWithArg(commandDelegate, IPacket, CommandDelegate, const Filter&, Filter())
DefinePolymorphicDelegate(presenceDelegate, IPacket, PresenceDelegate)
DefinePolymorphicDelegate(eventDelegate, IPacket, EventDelegate)


} } // namespace scy::smpl


#endif //  SCY_Symple_Client_H
