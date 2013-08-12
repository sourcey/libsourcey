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


using namespace std;


namespace scy {
	


Application& Application::getDefault() 
{
	static Singleton<Application> sh;
	return *sh.get();
}


Application::Application(uv::Loop& loop) :
	loop(loop)
{
	traceL("Application", this) << "Creating" << endl;
}

	
Application::~Application() 
{	
	traceL("Application", this) << "Destroying" << endl;
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
	debugL("Application") << "Finalizing" << std::endl;

#ifdef _DEBUG
	// Print active handles
	uv_walk(&loop, Application::onPrintHandle, nil);
#endif

	// Run until handles are closed 
	run(); 
			
	// Shutdown the garbage collector to free memory
	//GarbageCollector::instance().shutdown();

	// Run once more to clear garbage collector handle
	//run(); 

	debugL("Application") << "Finalized" << std::endl;
}		

	
void Application::waitForShutdown(ShutdownCommand::Fn callback, void* opaque)
{ 
	ShutdownCommand* cmd = new ShutdownCommand;
	cmd->opaque = opaque;
	cmd->callback = callback;

	uv_signal_t* sig = new uv_signal_t;
	sig->data = cmd;
	uv_signal_init(&loop, sig);
	uv_signal_start(sig, Application::onShutdownSignal, SIGINT);
		
	debugL("Application") << "Wait for kill" << std::endl;
	run();
}

			
void Application::onShutdownSignal(uv_signal_t *req, int signum)
{
	debugL("Application") << "On shutdown signal" << std::endl;
	ShutdownCommand* cmd = reinterpret_cast<ShutdownCommand*>(req->data);
	uv_close((uv_handle_t*)req, uv::afterClose);
	cmd->callback(cmd->opaque);
	delete cmd;
}
		

void Application::onPrintHandle(uv_handle_t* handle, void* arg) 
{
	debugL("Application") << "#### Active handle: " << handle << ": " << handle->type << std::endl;
}


} // namespace scy