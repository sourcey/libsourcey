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
    Logger::instance().add(new ConsoleChannel("debug", LInfo)); // LTrace, LDebug
    Logger::instance().setWriter(new AsyncLogWriter);

//#if USE_SSL
    //net::SSLManager::initNoVerifyClient();
//#endif

    // Pre-initialize video captures in the main thread
    // av::MediaFactory::instance().loadVideoCaptures();

    // Setup WebRTC environment
    rtc::LogMessage::LogToDebug(rtc::LS_INFO); // LS_VERBOSE, LS_INFO, LERROR
    rtc::LogMessage::LogTimestamps();
    rtc::LogMessage::LogThreads();

    rtc::InitializeSSL();

    {
        // Video source file
        std::string sourceFile(SOURCE_FILE);

        // Symple signalling client options
        smpl::Client::Options options;
        options.host = SERVER_HOST;
        options.port = SERVER_PORT;
        options.name = "Video Server";
        options.user = "videoserver";

        // NOTE: The server must enable anonymous
        // authentication for this test.
        // options.token = ""; token based authentication

        // Parse command line args
        OptionParser optparse(argc, argv, "-");
        for (auto& kv : optparse.args) {
            const std::string& key = kv.first;
            const std::string& value = kv.second;
            DebugL << "Setting option: " << key << ": " << value << std::endl;
            if (key == "file") {
                sourceFile = value;
            }
            else {
                std::cerr << "Unrecognized command: " << key << "=" << value << endl;
            }
        }

        Signaler app(options);
        app.startStreaming(sourceFile, true);

        // Process WebRTC threads on the main loop.
        Idler rtc(app.loop, [](void* arg) {
            auto thread = reinterpret_cast<rtc::Thread*>(arg);
            thread->ProcessMessages(10);
        }, rtc::Thread::Current());

        app.waitForShutdown();
        // app.finalize();
    }

    // Shutdown the media factory and release devices
    // av::MediaFactory::instance().unloadVideoCaptures();
    // av::MediaFactory::shutdown();

//#if USE_SSL
    //net::SSLManager::destroy();
//#endif
    rtc::CleanupSSL();
    Logger::destroy();
    return 0;
}
