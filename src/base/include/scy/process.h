///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Process_H
#define SCY_Process_H


#include "scy/uv/uvpp.h"
#include <functional>
#include <vector>


namespace scy {


typedef uv_process_options_t ProcessOptions;


class Process : public uv::Handle
{
public:
    Process(uv::Loop* loop= uv::defaultLoop());

    /// Spawns the process.
    /// Options must be properly set.
    /// Throws and exception on error.
    void spawn();

    /// Kills the process
    bool kill(int signum= 0);

    /// Returns the process PID
    int pid() const;

    /// Command line args.
    /// STL proxy for options.args
    std::vector<std::string> args;

    /// Exit callback; returns the exit status.
    std::function<void(std::int64_t)> onexit;

    /// Process options
    ProcessOptions options;

protected:
    uv_process_t _proc;
};


} // namespace scy


#endif // SCY_Process_H


/// @\}
