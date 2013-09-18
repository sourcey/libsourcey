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


#ifndef SOURCEY_Process_H
#define SOURCEY_Process_H


#include "Sourcey/UV/UVPP.h"


namespace scy {


class Process: public uv::Handle
{
public:
	Process(uv::Loop& loop = uv::defaultLoop()) :
		uv::Handle(&loop, new uv_process_t)
	{	
	}

	bool spawn()
		// Spawns the process.
		// Options must be properly set.
	{	
		if (uv_spawn(loop(), handle<uv_process_t>(), options)) {
			setLastError("Cannot spawn process");
			return false;
		}
		return true;
	}

	bool kill(int signum = 0)
		// Kills the process
	{	
		assert(pid() > 0);
		return uv_kill(pid(), signum).code == UV_OK;
	}

	int pid() const
		// Returns the process PID
	{	
		return handle<uv_process_t>()->pid;
	}

	typedef uv_process_options_t Options;
	Options options;

protected:
	uv_process_t _proc;
};


} // namespace scy


#endif // SOURCEY_Process_H




	/*
	bool spawn(const std::string& file, const std::string& args, unsigned int flags = 0)
		// Spawns the process using basic options.
	{	
		options.file = file.c_str();
		options.args = args.c_str();
		options.flags = flags;
		return spawn();
	}
	*/