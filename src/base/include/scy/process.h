///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Process_H
#define SCY_Process_H


#include "scy/base.h"
#include "scy/pipe.h"
#include "scy/handle.h"
#include <functional>
#include <vector>
#include <initializer_list>


namespace scy {


typedef uv_process_options_t ProcessOptions;


class Base_API Process
{
public:
    /// Default constructor.
    Process(uv::Loop* loop = uv::defaultLoop());

    /// Constructor with command line arguments.
    Process(std::initializer_list<std::string> args, uv::Loop* loop = uv::defaultLoop());

    /// Destructor.
    ~Process();

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
    std::vector<std::string> args;

    /// Spawns the process.
    /// Options must be properly set.
    /// Throws and exception on error.
    void spawn();

    /// Kills the process
    bool kill(int signum = SIGKILL);

    /// Returns the process PID
    int pid() const;

    /// Returns the stdin pipe.
    Pipe& in();

    /// Returns the stdout pipe.
    Pipe& out();

    /// Stdout signal.
    /// Signals when a line has been output from the process.
    std::function<void(std::string)> onstdout;

    /// Exit stgnals.
    /// Signals process exit status code.
    std::function<void(std::int64_t)> onexit;

    /// LibUV C options.
    /// Available for advanced use cases.
    ProcessOptions options;

protected:
    void init();

    uv::Handle<uv_process_t> _handle;
    Pipe _stdin;
    Pipe _stdout;
    uv_stdio_container_t _stdio[2];
    std::vector<char*> _cargs;
};


} // namespace scy


#endif // SCY_Process_H


/// @\}
