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


#ifndef SOURCEY_Application_H
#define SOURCEY_Application_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Exception.h"
#include "Sourcey/Singleton.h"


namespace scy {

		
class Application
	// A simple event based application which runs until the
	// internal event loop is terminated.
	//
	// The Application class also provides shutdown handling (Ctrl-C).
	//
	// TODO: Cross platform getopts
{
public:
	static Application& getDefault();
		// Returns the default Application singleton, although
		// Application instances may be initialized individually.
		// The default runner should be kept for short running
		// tasks such as timers in order to maintain performance.

	uv::Loop& loop;
		// The active event loop.
		// May be assigned at construction, otherwise the default
		// event loop is used. Should only be accessed 
	
	Application(uv::Loop& loop = uv::defaultLoop());
	~Application();
	
	void run();
	void stop();
	void finalize();		
	
	//
	// Shutdown handling
	//

	struct ShutdownCommand 
	{
		typedef void (*Fn)(void*);
		Fn callback;
		void* opaque;
	};
	
	void waitForShutdown(ShutdownCommand::Fn callback, void* opaque = nil);
			
	static void onShutdownSignal(uv_signal_t *req, int signum);		
	static void onPrintHandle(uv_handle_t* handle, void* arg);
};


} // namespace scy


#endif // SOURCEY_Application_H