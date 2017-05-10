///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/application.h"
#include "scy/memory.h"
#include "scy/logger.h"
#include "scy/error.h"
#include "scy/singleton.h"


namespace scy {


namespace internal {

    static Singleton<Application> singleton;

    struct ShutdownCmd
    {
        Application* self;
        void* opaque;
        std::function<void(void*)> callback;
    };

}


Application& Application::getDefault()
{
    return *internal::singleton.get();
}


Application::Application(uv::Loop* loop) :
    loop(loop)
{
    DebugS(this) << "Create" << std::endl;
}


Application::~Application()
{
    DebugS(this) << "Destroy" << std::endl;
}


void Application::run()
{
    uv_run(loop, UV_RUN_DEFAULT);
}


void Application::stop()
{
    uv_stop(loop);
}


void Application::finalize()
{
    DebugS(this) << "Finalizing" << std::endl;

#ifdef _DEBUG
    // Print active handles
    uv_walk(loop, Application::onPrintHandle, nullptr);
#endif

    // Shutdown the garbage collector to safely free memory before the app exists
    GarbageCollector::instance().finalize();

    // Run until handles are closed
    run();
    assert(loop->active_handles == 0);
    //assert(loop->active_reqs == 0);

    DebugS(this) << "Finalization complete" << std::endl;
}


void Application::bindShutdownSignal(std::function<void(void*)> callback, void* opaque)
{
    auto cmd = new internal::ShutdownCmd;
    cmd->self = this;
    cmd->opaque = opaque;
    cmd->callback = callback;

    auto sig = new uv_signal_t;
    sig->data = cmd;
    uv_signal_init(loop, sig);
    uv_signal_start(sig, Application::onShutdownSignal, SIGINT);
}


void Application::waitForShutdown(std::function<void(void*)> callback, void* opaque)
{
    DebugS(this) << "Wait for shutdown" << std::endl;
    bindShutdownSignal(callback, opaque);
    run();
}


void Application::onShutdownSignal(uv_signal_t* req, int /* signum */)
{
    auto cmd = reinterpret_cast<internal::ShutdownCmd*>(req->data);
    DebugS(cmd->self) << "Got shutdown signal" << std::endl;

    uv_close((uv_handle_t*)req, [](uv_handle_t* handle) {
        delete handle;
    });
    if (cmd->callback)
        cmd->callback(cmd->opaque);
    delete cmd;
}


void Application::onPrintHandle(uv_handle_t* handle, void* /* arg */)
{
    DebugA("Active handle: ", handle, ": ", handle->type)
}


//
// Command-line option parser
//

OptionParser::OptionParser(int argc, char* argv[], const char* delim)
{
    char* lastkey = 0;
    auto dlen = strlen(delim);
    for (int i = 0; i < argc; i++) {

        // Get the application exe path
        if (i == 0) {
            exepath.assign(argv[i]);
            continue;
        }

        // Get option keys
        if (strncmp(argv[i], delim, dlen) == 0) {
            lastkey = (&argv[i][dlen]);
            args[lastkey] = "";
        }

        // Get value for current key
        else if (lastkey) {
            args[lastkey] = argv[i];
            lastkey = 0;
        }

        else {
            DebugA("Unrecognized option:", argv[i]);
        }
    }
}


} // namespace scy


/// @\}
