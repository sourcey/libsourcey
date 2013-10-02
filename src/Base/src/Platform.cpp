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


using namespace std;


#define PATHMAX 1024


namespace scy {

	
std::string getExePath() 
{	
	char buf[PATHMAX];
	size_t size = PATHMAX;
	if (uv_exepath(buf, &size) != 0)
		throw std::runtime_error("System error: Cannot resolve executable path");
	return std::string(buf, size);
}


std::string getCwd() 
{	
	char buf[PATHMAX];
	size_t size = PATHMAX;
	if (uv_cwd(buf, size) != 0)
		throw std::runtime_error("System error: Cannot resolve working directory");
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



//
/// Windows helpers
//

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
	return (getOsVersion(&major, nullptr, nullptr) && major >= kWindowsVista);
}

bool isWindowsXpOrLater() 
{
	int major, minor;
	return (getOsVersion(&major, &minor, nullptr) &&
		(major >= kWindowsVista ||
		(major == kWindows2000 && minor >= 1)));
}


#define STACK_ARRAY(TYPE, LEN) static_cast<TYPE*>(::alloca((LEN)*sizeof(TYPE)))


std::wstring toUtf16(const char* utf8, std::size_t len)
{
	int len16 = ::MultiByteToWideChar(CP_UTF8, 0, utf8, len, NULL, 0);
	wchar_t* ws = STACK_ARRAY(wchar_t, len16);
	::MultiByteToWideChar(CP_UTF8, 0, utf8, len, ws, len16);
	return std::wstring(ws, len16);
}

std::wstring toUtf16(const std::string& str) 
{
	return toUtf16(str.data(), str.length());
}

std::string toUtf8(const wchar_t* wide, std::size_t len) 
{
	int len8 = ::WideCharToMultiByte(CP_UTF8, 0, wide, len, NULL, 0, NULL, NULL);
	char* ns = STACK_ARRAY(char, len8);
	::WideCharToMultiByte(CP_UTF8, 0, wide, len, ns, len8, NULL, NULL);
	return std::string(ns, len8);
}

std::string toUtf8(const std::wstring& wstr) 
{
	return toUtf8(wstr.data(), wstr.length());
}

#endif


} // namespace scy::uv