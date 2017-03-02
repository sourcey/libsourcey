///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
///


#include "scy/idler.h"
#include "scy/logger.h"
#include "signaler.h"

#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"


using std::endl;
using namespace scy;


// Test this demo with the code in the `client` directory


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace)); // LTrace

#if USE_SSL
    net::SSLManager::initNoVerifyClient();
#endif

    // Pre-initialize video captures in the main thread
    // av::MediaFactory::instance().loadVideoCaptures();

    // Setup WebRTC environment
    rtc::LogMessage::LogToDebug(rtc::LERROR); // LERROR (rtc::LoggingSeverity)
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
            // TraceL << "Running WebRTC loop" << endl;
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
