//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SCY_Process_H
#define SCY_Process_H


#include "scy/uv/uvpp.h"
#include <functional>
#include <vector>


namespace scy {
    

typedef uv_process_options_t ProcessOptions;


class Process: public uv::Handle
{
public:
    Process(uv::Loop* loop = uv::defaultLoop());

    void spawn();
        // Spawns the process.
        // Options must be properly set.
        // Throws and exception on error.

    bool kill(int signum = 0);
        // Kills the process

    int pid() const;
        // Returns the process PID

    std::vector<std::string> args;
        // Command line args.
        // STL proxy for options.args

    std::function<void(Int64)> onexit;
        // Exit callback; returns the exit status.

    ProcessOptions options;
        // Process options

protected:
    uv_process_t _proc;
};


} // namespace scy


#endif // SCY_Process_H