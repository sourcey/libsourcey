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


#include "Sourcey/Platform.h"
#include "Sourcey/Exception.h"
#include "Sourcey/UV/UVPP.h"


using namespace std;


#define PATHMAX 1024


namespace scy {
	

UInt64 getTime() 
{
	return uv_hrtime();
}

	
std::string getExePath() 
{	
	char buf[PATHMAX];
	size_t size = PATHMAX;
	if (uv_exepath(buf, &size) != 0)
		throw SystemException("Cannot resolve executable path");
	return std::string(buf, size);
}


std::string getCWD() 
{	
	char buf[PATHMAX];
	size_t size = PATHMAX;
	if (uv_cwd(buf, size).code != UV_OK)
		throw SystemException("Cannot resolve working directory");
	return std::string(buf);
}


UInt64 getFreeMemory()
{
	return uv_get_free_memory();
}


UInt64 getTotalMemory()
{
	return uv_get_total_memory();
}


#if WIN32

enum WindowsMajorVersions 
{
	kWindows2000 = 5,
	kWindowsVista = 6,
};


bool getOsVersion(int* major, int* minor, int* build) 
{
	OSVERSIONINFO info = {0};
	info.dwOSVersionInfoSize = sizeof(info);
	if (GetVersionEx(&info)) {
		if (major) *major = info.dwMajorVersion;
		if (minor) *minor = info.dwMinorVersion;
		if (build) *build = info.dwBuildNumber;
		return true;
	}
	return false;
}

bool isWindowsVistaOrLater() 
{
	int major;
	return (getOsVersion(&major, NULL, NULL) && major >= kWindowsVista);
}

bool isWindowsXpOrLater() 
{
	int major, minor;
	return (getOsVersion(&major, &minor, NULL) &&
		(major >= kWindowsVista ||
		(major == kWindows2000 && minor >= 1)));
}

#endif


} // namespace scy::uv