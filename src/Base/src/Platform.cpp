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
#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Exception.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>


using namespace std;


#define PATHMAX 1024


namespace scy {
	

UInt64 getTimeHR() 
{
	return uv_hrtime();
}
	

UInt64 getTimeMS() 
{
	return uv_hrtime() / 1000000;
}


UInt64 getTicks()
{
#ifdef WIN32
	return ::GetTickCount();
#else
	struct timespec tval;
	clock_gettime(CLOCK_MONOTONIC, &tval);
	return tval.tv_sec * 1000 + tval.tv_nsec / 1000000;
#endif
}


double getProcessTime()
{
	return clock() / CLOCKS_PER_SEC;
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


void sleep(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	sleep(ms);
#endif
}


void pause()
{
	std::puts("Press enter to continue...");
	std::getchar();
}



#ifdef WIN32

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