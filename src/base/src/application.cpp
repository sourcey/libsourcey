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


#include "scy/application.h"
#include "scy/memory.h"
#include "scy/logger.h"
#include "scy/exception.h"
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
	DebugLS(this) << "Create" << std::endl;
}

	
Application::~Application() 
{	
	DebugLS(this) << "Destroy" << std::endl;
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
	DebugLS(this) << "Finalizing" << std::endl;

#ifdef _DEBUG
	// Print active handles
	uv_walk(loop, Application::onPrintHandle, nullptr);
#endif
			
	// Shutdown the garbage collector to free memory
	GarbageCollector::instance().finalize();

	// Run until handles are closed
	run(); 	
	assert(loop->active_handles == 0);
	//assert(loop->active_reqs == 0);

	DebugLS(this) << "Finalization complete" << std::endl;
}		
	
	
void Application::waitForShutdown(std::function<void(void*)> callback, void* opaque)
{ 
	auto cmd = new internal::ShutdownCmd;
	cmd->self = this;
	cmd->opaque = opaque;
	cmd->callback = callback;

	auto sig = new uv_signal_t;
	sig->data = cmd;
	uv_signal_init(loop, sig);
	uv_signal_start(sig, Application::onShutdownSignal, SIGINT);
		
	DebugLS(this) << "Wait for shutdown" << std::endl;
	run();
}

			
void Application::onShutdownSignal(uv_signal_t* req, int /* signum */)
{
	auto cmd = reinterpret_cast<internal::ShutdownCmd*>(req->data);
	DebugLS(cmd->self) << "Got shutdown signal" << std::endl;

	uv_close((uv_handle_t*)req, [](uv_handle_t* handle) {
		delete handle;
	});
	if (cmd->callback)
		cmd->callback(cmd->opaque);
	delete cmd;
}
		

void Application::onPrintHandle(uv_handle_t* handle, void* /* arg */) 
{
	DebugL << "#### Active handle: " << handle << ": " << handle->type << std::endl;
}


//
// Command-line option parser
//
	
OptionParser::OptionParser(int argc, char* argv[], char* delim)
{
	char* lastkey = 0;	
	int dlen = strlen(delim);	
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
			TraceL << "Unrecognized option: " << argv[i] << std::endl;	
		}
	}
}


} // namespace scy