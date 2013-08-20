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


#include "Sourcey/Application.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Exception.h"
#include "Sourcey/Singleton.h"


namespace scy {
	
	
namespace internal {
	static Singleton<Application> singleton;

	struct ShutdownCmd 
	{
		std::function<void(void*)> callback;
		void* opaque;
	};
}


Application& Application::getDefault() 
{
	return *internal::singleton.get();
}


Application::Application(uv::Loop& loop) :
	loop(loop)
{
	traceL("Application", this) << "ctor" << std::endl;
}

	
Application::~Application() 
{	
	traceL("Application", this) << "dtor" << std::endl;
}

	
void Application::run() 
{ 
	uv_run(&loop, UV_RUN_DEFAULT);
}


void Application::stop() 
{ 
	uv_stop(&loop); 
}


void Application::finalize() 
{ 
	debugL("Application") << "finalizing" << std::endl;

#ifdef _DEBUG
	// Print active handles
	uv_walk(&loop, Application::onPrintHandle, nullptr);
#endif

	// Run until handles are closed 
	run(); 
			
	// Shutdown the garbage collector to free memory
	//GarbageCollector::instance().shutdown();

	// Run once more to clear garbage collector handle
	//run(); 

	debugL("Application") << "finalization complete" << std::endl;
}		
	
	
void Application::waitForShutdown(std::function<void(void*)> callback, void* opaque)
{ 
	internal::ShutdownCmd* cmd = new internal::ShutdownCmd;
	cmd->opaque = opaque;
	cmd->callback = callback;

	uv_signal_t* sig = new uv_signal_t;
	sig->data = cmd;
	uv_signal_init(&loop, sig);
	uv_signal_start(sig, Application::onShutdownSignal, SIGINT);
		
	debugL("Application") << "waiting for shutdown" << std::endl;
	run();
}

			
void Application::onShutdownSignal(uv_signal_t* req, int /* signum */)
{
	debugL("Application") << "got shutdown signal" << std::endl;
	internal::ShutdownCmd* cmd = reinterpret_cast<internal::ShutdownCmd*>(req->data);
	uv_close((uv_handle_t*)req, uv::afterClose);
	cmd->callback(cmd->opaque);
	delete cmd;
}
		

void Application::onPrintHandle(uv_handle_t* handle, void* /* arg */) 
{
	debugL("Application") << "#### active handle: " << handle << ": " << handle->type << std::endl;
}

//
// Command-line option parser
//
	
OptionParser::OptionParser(int argc, char* argv[], char delim)
{
	char* lastkey = 0;		
	for (int i = 0; i < argc; i++) {
		if (i == 0) {
			exepath.assign(argv[i]);
			continue;
		}
		if (argv[i][0] == delim) {
			args[&argv[i][1]] = ""; // create empty entry
			lastkey = argv[i];
		}
		else if (lastkey) {
			lastkey = 0;
			args[argv[i - 1]] = argv[i];
		}
		else assert(0);
	}
}


} // namespace scy