#ifndef TURN_TurnClientTest_H
#define TURN_TurnClientTest_H


#include "scy/net/socket.h"
#include "scy/time.h"
#include "scy/util.h"

#include <iostream>


#define TEST_TCP 1
#define RAISE_LOCAL_SERVER 0

#define TURN_SERVER_IP                                                         \
    "127.0.0.1" //"202.173.167.126" // "58.7.41.244" "127.0.0.1"
                //"122.201.111.134" "74.207.248.97"
#define TURN_SERVER_PORT 3478
#define TURN_SERVER_USERNAME "username"
#define TURN_SERVER_PASSWORD "password"
#define TURN_AUTHORIZE_PEER_IP                                                 \
    "202.173.167.126" //"202.173.167.126" // for CreatePremission

#define TEST_INITIATOR_TO_RESPONDER
//#define TEST_RESPONDER_TO_INITIATOR


namespace scy {
namespace turn {


inline std::string createLatencyCheck(int packetSize= 1024)
{
    std::string payload;
    // payload.append(util::itostr(time::ticks()));
    payload.append(packetSize - payload.length(), 'x');
    return payload;
}
}
} //  namespace scy::turn


#endif // TURN_TurnClientTest_H
