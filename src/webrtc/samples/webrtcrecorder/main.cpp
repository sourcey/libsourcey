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
    Logger::instance().add(new ConsoleChannel("debug", LDebug)); // LTrace

#if USE_SSL
    net::SSLManager::initNoVerifyClient();
#endif

    // Setup WebRTC environment
    rtc::LogMessage::LogToDebug(rtc::LERROR);
    rtc::LogMessage::LogTimestamps();
    rtc::LogMessage::LogThreads();

    rtc::InitializeSSL();

    {
        smpl::Client::Options options;
        options.host = SERVER_HOST;
        options.port = SERVER_PORT;
        options.name = "Video Recorder";
        options.user = "videorecorder";

        // NOTE: The server must enable anonymous authentication for this demo.
        // options.token = ""; token based authentication

        Signaler app(options);

        Idler rtc(app.loop, [](void* arg) {
            // TraceA("Running WebRTC loop")
            auto thread = reinterpret_cast<rtc::Thread*>(arg);
            thread->ProcessMessages(10);
        }, rtc::Thread::Current());

        app.waitForShutdown();
    }

#if USE_SSL
    net::SSLManager::destroy();
#endif
    rtc::CleanupSSL();
    Logger::destroy();
    return 0;
}
