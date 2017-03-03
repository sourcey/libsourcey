///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Process_H
#define SCY_Process_H


#include "scy/base.h"
#include "scy/pipe.h"
#include <functional>
#include <vector>


namespace scy {


typedef uv_process_options_t ProcessOptions;





class SCY_EXTERN Process : public uv::Handle
{
public:
    Process(uv::Loop* loop = uv::defaultLoop());

    /// Path to the program to execute.
    /// Cenvenience proxy for options.file.
    /// Must be set before `spawn()`
    std::string file;

    /// Set the current working directory.
    /// Cenvenience proxy for options.cwd.
    /// Must be set before `spawn()`
    std::string cwd;

    /// Command line agruments to pass to the process.
    /// Cenvenience proxy for options.args.
    /// Must be set before `spawn()`
    std::vector<char*> args;

    /// Spawns the process.
    /// Options must be properly set.
    /// Throws and exception on error.
    void spawn();

    /// Kills the process
    bool kill(int signum = 0);

    /// Returns the process PID
    int pid() const;

    /// Stdout signal.
    /// Signals when a line has been output from the process.
    std::function<void(std::string)> sdout;

    /// Exit stgnals. 
    /// Signals process exit status code.
    std::function<void(std::int64_t)> onexit;

    /// LibUV C options.
    /// Available for advanced use cases.
    ProcessOptions options;

protected:
    uv_process_t _proc;
    Pipe _stdin;
    Pipe _stdout;
    uv_stdio_container_t _stdio[2];
};


} // namespace scy


#endif // SCY_Process_H


/// @\}
