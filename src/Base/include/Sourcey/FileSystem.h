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


#ifndef SOURCEY_Timeout_H
#define SOURCEY_Timeout_H


#include "Sourcey/Types.h"


#ifdef _WIN32
  #define ALLOWABLE_DIRECTORY_DELIMITERS "/\\"
  #define DIRECTORY_DELIMITER '\\'
  #define DIRECTORY_DELIMITER_STRING "\\"
#else
  #define ALLOWABLE_DIRECTORY_DELIMITERS "/"
  #define DIRECTORY_DELIMITER '/'
  #define DIRECTORY_DELIMITER_STRING "/"
#endif


namespace scy {
namespace fs {


// TODO: Implement libuv fs_* types

std::string getFileName(const std::string& path);
	// Returns the file name and extension part of the given path.

std::string getFileBaseName(std::string path);
	// Returns the file name sans extension.

std::string getPathName(const std::string& path);
	// Returns the directory part of the given path.

std::string getExtension(const std::string& path, bool includeDot = false);
	// Returns the file extension part of the given path.

bool fileExists(const std::string& path);
	// Returns true if the file exists.


} } // namespace scy::fs


#endif
