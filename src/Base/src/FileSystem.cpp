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


#include "Sourcey/FileSystem.h"
#include <fstream>


namespace scy {
namespace fs {


std::string getFileName(const std::string& path)
{
	size_t dirp = path.find_last_of(ALLOWABLE_DIRECTORY_DELIMITERS);
	if (dirp == std::string::npos) return path;
	return path.substr(dirp + 1);
}


std::string getPathName(const std::string& path)
{
	size_t dirp = path.find_last_of(ALLOWABLE_DIRECTORY_DELIMITERS);
	if (dirp == std::string::npos) return "";
	return path.substr(0, dirp);
}


std::string getFileBaseName(const std::string& path)
{
	size_t dotp = path.find_last_of(".");
	if (dotp == std::string::npos) 
		return path;

	size_t dirp = path.find_last_of(ALLOWABLE_DIRECTORY_DELIMITERS);
	if (dirp != std::string::npos && dotp < dirp)
		return path;

	return path.substr(0, dotp);
}


std::string getExtension(const std::string& path, bool includeDot)
{
	size_t dotp = path.find_last_of(".");
	if (dotp == std::string::npos) 
		return "";

	// Ensure the dot was not part of the pathname
	size_t dirp = path.find_last_of(ALLOWABLE_DIRECTORY_DELIMITERS);
	if (dirp != std::string::npos && dotp < dirp)
		return "";

	return path.substr(dotp + includeDot ? 0 : 1);
}


bool fileExists(const std::string& path)
{
	std::ifstream ifs;
	ifs.open(path.c_str(), std::ios::in);
	bool res = ifs.is_open();
	ifs.close();
	return res;
}


} } // namespace scy::fs