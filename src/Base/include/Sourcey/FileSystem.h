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


#ifndef SOURCEY_FileSystem_H
#define SOURCEY_FileSystem_H


#include "Sourcey/Types.h"


/*
#ifdef _WIN32
  #define ALLOWABLE_DIRECTORY_DELIMITERS "/\\"
  #define DIRECTORY_DELIMITER '\\'
  #define DIRECTORY_DELIMITER_STRING "\\"
#else
  #define ALLOWABLE_DIRECTORY_DELIMITERS "/"
  #define DIRECTORY_DELIMITER '/'
  #define DIRECTORY_DELIMITER_STRING "/"
#endif
  */


namespace scy {
namespace fs {

	
extern char separator;
	// The path separator used by the current platform:
	// '/' on unix and '\\' on windows.

std::string filename(const std::string& path);
	// Returns the file name and extension part of the given path.

std::string basename(const std::string& path);
	// Returns the file name sans extension.

std::string dirname(const std::string& path);
	// Returns the directory part of the path.

std::string extname(const std::string& path, bool includeDot = false);
	// Returns the file extension part of the path.

bool exists(const std::string& path);
	// Returns true if the file exists.

bool readdir(const std::string& dir, std::vector<std::string>& res);
	// Returns a list of all files and folders in the directory. 

bool isdir(const std::string& path);
	// Returns true if the path is a directory.
		
std::string transcode(const std::string& path);
	/// If LibSourcey was compiled with Unicode support (UNICODE) this 
	/// function converts a UTF-8 encoded file path into windows native format,
	/// otherwise the path string is returned unchanged.

// TODO: Implement more libuv fs_* types

/*
class StatWatcher: public uv::Handle 
{
public:
	StatWatcher();
	virtual ~StatWatcher();

	void start(const std::string& path, uint32_t interval, bool persistent = true);
	void stop();

private:
	static void onCallback(uv_fs_poll_t* handle,
						int status,
						const uv_stat_t* prev,
						const uv_stat_t* curr);
	void stop();
};
*/


} } // namespace scy::fs


#endif
