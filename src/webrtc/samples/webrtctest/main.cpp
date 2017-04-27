#include "webrtc/base/thread.h"
#include "webrtc/p2p/base/basicpacketsocketfactory.h"
#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/test/fakeconstraints.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/base/ssladapter.h"


#include "json.hpp" // include nlohmann json

// #include "scy/idler.h"
// #include "scy/idler.h"
// #include "scy/logger.h"
using std::endl;
// using namespace scy;

int main(int argc, char* argv[]) {


  // Setup WebRTC environment
  rtc::LogMessage::LogToDebug(rtc::LS_VERBOSE); // LS_VERBOSE, LS_INFO, LERROR
  // rtc::LogMessage::LogTimestamps();
  // rtc::LogMessage::LogThreads();

  rtc::InitializeSSL();

  // something from base
  rtc::Thread* thread = rtc::Thread::Current();

  // something from p2p
  std::unique_ptr<rtc::BasicPacketSocketFactory> socket_factory(
    new rtc::BasicPacketSocketFactory());

  // something from api
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
    peer_connection_factory = webrtc::CreatePeerConnectionFactory();

  // something from api/test
  webrtc::FakeConstraints constraints;

  // something from media/engine
  cricket::WebRtcVideoDeviceCapturerFactory factory;
  auto capturer = factory.Create(cricket::Device("", 0));
  // cricket::VideoCapturer* capturer = factory.Create(cricket::Device("", 0));

    {
        nlohmann::json j;
        // j["data"] = "z";
        // nlohmann::json::value desc({{ "kSessionDescriptionTypeName", "type" },
        // { "kSessionDescriptionSdpName", "sdp"}});


    }

  return 0;
}
