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


#ifndef SOURCEY_Worker_H
#define SOURCEY_Worker_H


namespace scy { 


// ----------------------------------------------------------------------------
//
class Worker 
{
public:
	Worker(uv::Loop& loop) : 
		loop(loop)
	{
	}

	virtual ~Worker()
	{
	}
	
	void start()
	{		
		req.data = this;
        uv_queue_work(&loop, &req, _run, _afterRun);
	}
	
	void stop()
	{	
		// TODO: Can use uv_cancel on linux, 
		// need a synced boolean on windows
	}

	virtual void run() = 0;
	
	virtual void afterRun()
	{	
	}

	static void _run(uv_work_t *req)
	{			
		reinterpret_cast<Worker*>(req->data)->run();
	}

	static void _afterRun(uv_work_t *req, int status)
	{		
		// TODO: Do something with status?
		reinterpret_cast<Worker*>(req->data)->afterRun();
	}

protected:
	uv::Loop& loop;
	uv_work_t req;
};


} // namespace scy


#endif // SOURCEY_Worker_H