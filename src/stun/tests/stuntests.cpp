#include "scy/base.h"
#include "scy/platform.h"
#include "scy/filesystem.h"
#include "scy/logger.h"
#include "scy/util.h"
#include "scy/stun/message.h"

#include <assert.h>
#include <algorithm>
#include <stdexcept>


using namespace std;
using namespace scy;


// TODO: Test vectors from http://tools.ietf.org/html/rfc5769


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("Test", LTrace));

    // =========================================================================
    // Message Integrity
    //
    describe("message integrity", []() {
        std::string username("someuser");
        std::string password("somepass");

        stun::Message request(stun::Message::Request, stun::Message::Allocate);
        //request.setType(stun::Message::Allocate);

        auto usernameAttr = new stun::Username;
        usernameAttr->copyBytes(username.c_str(), username.size());
        request.add(usernameAttr);

        auto integrityAttr = new stun::MessageIntegrity;
        integrityAttr->setKey(password);
        request.add(integrityAttr);

        Buffer buf;
        request.write(buf);

        stun::Message response;
        response.read(constBuffer(buf));

        integrityAttr = response.get<stun::MessageIntegrity>();
        assert(integrityAttr->verifyHmac(password));
    });

    // =========================================================================
    // Request Types
    //
    describe("request types", []() {
        std::uint16_t type = stun::Message::Indication | stun::Message::SendIndication;

        //assert(IS_STUN_INDICATION(type));

        std::uint16_t classType = type & 0x0110;
        std::uint16_t methodType = type & 0x000F;

        assert(classType == stun::Message::Indication);
        assert(methodType == stun::Message::SendIndication);

        stun::Message request(stun::Message::Indication, stun::Message::SendIndication);
        //assert(IS_STUN_INDICATION(request.classType() | request.methodType()));

        assert(request.classType() != stun::Message::Request);
        assert(request.classType() == stun::Message::Indication);

        stun::Message request1(stun::Message::Request, stun::Message::Allocate);
        //assert(IS_STUN_REQUEST(request1.classType() | request1.methodType()));
    });

    // =========================================================================
    // Xor Address
    //
    describe("xor address", []() {
        assert(5555 == 0x15B3);
        assert(5555 ^ (kMagicCookie >> 16) == 0x34A1);

        net::Address addr("192.168.1.1", 5555);
        DebugL << "Source Address: " << addr << endl;

        stun::Message request(stun::Message::Request, stun::Message::Allocate);
        //stun::Message request;
        //request.setType(stun::Message::Allocate);

        auto addrAttr = new stun::XorRelayedAddress;
        addrAttr->setAddress(addr);
        request.add(addrAttr);
        DebugL << "Request Address: " << addrAttr->address() << endl;

        Buffer buf;
        request.write(buf);

        stun::Message response;
        response.read(constBuffer(buf));

        addrAttr = response.get<stun::XorRelayedAddress>();

        DebugL << "Response Address: " << addrAttr->address() << endl;
        assert(addrAttr->address() == addr);
    });

    test::runAll();

    Logger::destroy();

    return finalize();
}
