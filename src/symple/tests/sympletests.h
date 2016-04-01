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


#ifndef SCY_Symple_Tests_H
#define SCY_Symple_Tests_H


#include "scy/base.h"
#include "scy/test.h"
#include "scy/logger.h"
#include "scy/symple/client.h"
#include "scy/net/sslmanager.h"

using std::cout;
using std::cerr;
using std::endl;
using scy::test::Test;


namespace scy {


// =============================================================================
// Test Client
//
class TestClient
{
public:
    bool gotOnline;
    bool gotRemotePresence;
    std::string user;

#if USE_SSL
    smpl::SSLClient client;
#else
    smpl::TCPClient client;
#endif

    TestClient(const smpl::Client::Options& options) :
        gotOnline(false),
        gotRemotePresence(false)
    {
        user = options.user;
        client.options() = options;
        // client += messageDelegate(this, &TestClient::onRecvMessage);
        client += presenceDelegate(this, &TestClient::onRecvPresence);
        client.Announce += delegate(this, &TestClient::onClientAnnounce);
        client.StateChange += delegate(this, &TestClient::onClientStateChange);
        client.CreatePresence += delegate(this, &TestClient::onCreatePresence);
    }

    ~TestClient()
    {
        client.close();
    }

    void connect()
    {
        InfoL << user << ": connect" << std::endl;
        client.connect();
    }

    bool completed()
    {
        InfoL << user << ": completed: "
          // << "user=" << client.options().user << ", "
          // << "name=" << client.options().name << ", "
          << "gotOnline=" << gotOnline << ", "
          << "gotRemotePresence=" << gotRemotePresence
          << std::endl;
        return gotOnline && gotRemotePresence;
    }

    void check()
    {
        InfoL << user << ": check: "
          // << "user=" << client.options().user << ", "
          // << "name=" << client.options().name << ", "
          << "gotOnline=" << gotOnline << ", "
          << "gotRemotePresence=" << gotRemotePresence
          << std::endl;
        expect(gotOnline);
        expect(gotRemotePresence);
    }

    // void onRecvMessage(void* sender, smpl::Message& message)
    // {
    //     InfoL << user << ": ############################### On message: " << message.toStyledString() << endl;
    //
    //     // Handle incoming Symple messages here
    // }

    void onRecvPresence(void* sender, smpl::Presence& presence)
    {
        InfoL << user << ": On presence: " << presence.toStyledString() << endl;

        expect(presence.data("version").asString() == "1.0.1");

        // Handle incoming Symple messages here
        if (user == "l") {
            expect(presence.from().user == "r");
        }
        else if (user == "r") {
            expect(presence.from().user == "l");
        }
        else {
            expect(!"user should be 'l' or 'r'");
        }

        gotRemotePresence = true;
    }

    void onClientAnnounce(const int& status)
    {
        assert(status == 200);
    }

    void onClientStateChange(sockio::ClientState& state, const sockio::ClientState& oldState)
    {
        InfoL << user << ": Client state changed: " << state << ": " << client.ws().socket->address() << endl;

        switch (state.id()) {
        case sockio::ClientState::Connecting:
            break;
        case sockio::ClientState::Connected:
            break;
        case sockio::ClientState::Online:
            {
                gotOnline = true;

                // Join the test room when online
                client.joinRoom("test");

                // Send a message when online
                // smpl::Message m;
                // m.setData("olay");
                // client.send(m, true);
            }
            break;
        case sockio::ClientState::Error:
            assert(0);
            break;
        }
    }

    void onCreatePresence(smpl::Peer& peer)
    {
        InfoL << user << ": Updating Client Data" << endl;

        // Update the peer object to be broadcast with presence.
        // Any arbitrary data can be broadcast with presence.
        peer["agent"] = "Spot";
        peer["version"] = "1.0.1";
    }
};


} // namespace scy


#endif // SCY_Symple_Tests_H
