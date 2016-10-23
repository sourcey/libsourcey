#include "signaler.h"
#include "scy/logger.h"
#include "scy/idler.h"
// #include "scy/av/mediafactory.h"

#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"


using std::endl;
using namespace scy;


// Test this demo with: https://github.com/sourcey/symple-webrtc-native-streaming-demo


#define SERVER_HOST "localhost"
#define USE_SSL     0 // 1
#if USE_SSL
#define SERVER_PORT 443
#else
#define SERVER_PORT 4500
#endif


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace)); //LTrace

#if USE_SSL
    SSLManager::initNoVerifyClient();
#endif

    // Pre-initialize video captures in the main thread
    // av::MediaFactory::instance().loadVideoCaptures();

    // Setup WebRTC environment
    rtc::LogMessage::LogToDebug(rtc::LERROR); //LERROR (rtc::LoggingSeverity)
    rtc::LogMessage::LogTimestamps();
    rtc::LogMessage::LogThreads();

    rtc::InitializeSSL();

    {
        smpl::Client::Options options;
        options.host = SERVER_HOST;
        options.port = SERVER_PORT;
        options.name = "Video Server";
        options.user = "videoserver";

        // NOTE: The server must enable anonymous
        // authentication for this test.
        // options.token = ""; token based authentication

        Signaler app(options);

        Idler rtc(app.loop, [](void* arg) {
            // DebugL << "Running WebRTC loop" << endl;
            auto thread = reinterpret_cast<rtc::Thread*>(arg);
            thread->ProcessMessages(10);
        }, rtc::Thread::Current());

        app.waitForShutdown();
        // app.finalize();
    }

    // Shutdown the media factory and release devices
    // av::MediaFactory::instance().unloadVideoCaptures();
    // av::MediaFactory::shutdown();

#if USE_SSL
    net::SSLManager::destroy();
#endif
    rtc::CleanupSSL();
    Logger::destroy();

    return 0;
}
