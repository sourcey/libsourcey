///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/process.h"
#include "scy/filesystem.h"


namespace scy {


Process::Process(uv::Loop* loop)
    : uv::Handle(loop, new uv_process_t)
{
    ptr()->data = this;
    options.env = nullptr;
    options.cwd = nullptr;
    options.flags = 0;
    options.stdio_count = 0;
    // options.uid = 0;
    // options.gid = 0;
    options.exit_cb = [](uv_process_t* req, int64_t exitStatus,
                         int /*termSignal*/) {
        auto self = reinterpret_cast<Process*>(req->data);
        if (self->onexit)
            self->onexit(exitStatus);
        // We could call close() here to free the uv_process_t content
    };
}


void Process::spawn()
{
    // Sanity checks; note that libuv does not return
    // an error if the target doesn't exist, but we will.
    if (options.file == nullptr)
        throw std::runtime_error(
            "Cannot spawn process: File path must be set.");
    if (args.size() > 10)
        throw std::runtime_error(
            "Cannot spawn process: Only 10 command line arguments supported.");
    if (!fs::exists(options.file))
        throw std::runtime_error("Cannot spawn process: File does not exist: " +
                                 std::string(options.file));

    // Override c style args if vector has items.
    char* uvargs[10];
    if (!args.empty()) {
        uvargs[0] =
            const_cast<char*>(options.file); // first arg is always exe path
        unsigned i;
        for (i = 0; i < args.size(); i++) {
            uvargs[i + 1] = const_cast<char*>(args[i].data());
        }
        uvargs[i + 1] = nullptr;
        options.args = uvargs;
    }

    int r = uv_spawn(loop(), ptr<uv_process_t>(), &options);
    if (r < 0)
        setAndThrowError("Cannot spawn process", r);
}


bool Process::kill(int signum)
{
    assert(pid() > 0);
    return uv_kill(pid(), signum) == 0;
}


int Process::pid() const
{
    return ptr<uv_process_t>()->pid;
}


} // namespace scy


/// @\}
