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
#include "Sourcey/Util.h"
#include <functional>
#include <vector>
#include <map>


namespace scy {

		
class Application
	/// A simple event based application which runs until the
	/// internal event loop is terminated.
	///
	/// The Application class provides shutdown handling (Ctrl-C).
	///
	/// TODO: Cross platform getopts
{
public:
	static Application& getDefault();
		// Returns the default Application singleton, although
		// Application instances may be initialized individually.
		// The default runner should be kept for short running
		// tasks such as timers in order to maintain performance.

	uv::Loop* loop;
		// The active event loop.
		// May be assigned at construction, otherwise the default
		// event loop is used.
	
	Application(uv::Loop* loop = uv::defaultLoop());
	~Application();
	
	void run();
	void stop();
	void finalize();

	
	//
	// Shutdown handling
	//

	void waitForShutdown(std::function<void(void*)> callback = nullptr, void* opaque = nullptr);
			
	static void onShutdownSignal(uv_signal_t *req, int signum);		
	static void onPrintHandle(uv_handle_t* handle, void* arg);

protected:
	Application(const Application&); // = delete;
	Application(Application&&); // = delete;
	Application& operator=(const Application&); // = delete;
	Application& operator=(Application&&); // = delete;
};


//
// Command Line Option Parser
//

typedef std::map<std::string, std::string> OptionMap;

struct OptionParser 
{	
	std::string exepath; // TODO: UTF8
	OptionMap args;
	
	OptionParser(int argc, char* argv[], char* delim); // "--"

	bool has(const char* key) {
		return args.find(key) != args.end();
	}

	std::string get(const char* key) {
		OptionMap::const_iterator it = args.find(key);
		if (it != args.end())
			return it->second;
		return std::string();
	}

	template<typename NumericType>
	NumericType get(const char* key) {
		OptionMap::const_iterator it = args.find(key);
		if (it != args.end())
			return util::strtoi<NumericType>(it->second);
	}
};


} // namespace scy


#endif // SOURCEY_Application_H
