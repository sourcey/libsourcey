///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/platform.h"
#include "scy/error.h"

#include "uv.h"

#include <stdlib.h>
#include <string.h>

#ifdef SCY_WIN
#include <windows.h>
#include <winsock2.h>
#else
#include <unistd.h>
#endif


#define PATHMAX 1024


namespace scy {


std::string getExePath()
{
    char buf[PATHMAX];
    size_t size = PATHMAX;
    if (uv_exepath(buf, &size) != 0)
        throw std::runtime_error(
            "System error: Cannot resolve executable path");
    return std::string(buf, size);
}


std::string getCwd()
{
    char buf[PATHMAX];
    size_t size = PATHMAX;
    if (uv_cwd(buf, &size) != 0)
        throw std::runtime_error(
            "System error: Cannot resolve working directory");
    return std::string(buf);
}


uint64_t getFreeMemory()
{
    return uv_get_free_memory();
}


uint64_t getTotalMemory()
{
    return uv_get_total_memory();
}


int numCpuCores()
{
    uv_cpu_info_t *info;
    int cpu_count;
    uv_cpu_info(&info, &cpu_count);
    uv_free_cpu_info(info, cpu_count);
    return cpu_count;
}


void sleep(int ms)
{
#ifdef SCY_WIN
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}


void pause()
{
    std::puts("Press enter to continue...");
    std::getchar();
}


std::string getHostname()
{
    char name[256];
    gethostname(name, 256);
    return name;
}


std::string getEnv(const std::string& name, const std::string& defaultValue)
{
    const char* value = getenv(name.c_str());
    if (value)
        return value;
    return defaultValue;
}


bool getEnvBool(const std::string& name)
{
    const char* value = getenv(name.c_str());
    return value && (
        strcmp(value, "1") == 0 ||
        strcmp(value, "true") == 0);
}


//
/// Windows helpers
//

#ifdef SCY_WIN

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
        if (major)
            *major = info.dwMajorVersion;
        if (minor)
            *minor = info.dwMinorVersion;
        if (build)
            *build = info.dwBuildNumber;
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
            (major >= kWindowsVista || (major == kWindows2000 && minor >= 1)));
}


#define STACK_ARRAY(TYPE, LEN) static_cast<TYPE*>(::alloca((LEN) * sizeof(TYPE)))


std::wstring toUtf16(const char* utf8, size_t len)
{
    auto len16 = ::MultiByteToWideChar(CP_UTF8, 0, utf8, int(len), NULL, 0);
    wchar_t* ws = STACK_ARRAY(wchar_t, len16);
    ::MultiByteToWideChar(CP_UTF8, 0, utf8, int(len), ws, len16);
    return std::wstring(ws, len16);
}

std::wstring toUtf16(const std::string& str)
{
    return toUtf16(str.data(), str.length());
}

std::string toUtf8(const wchar_t* wide, size_t len)
{
    auto len8 = ::WideCharToMultiByte(CP_UTF8, 0, wide, int(len), NULL, 0, NULL, NULL);
    char* ns = STACK_ARRAY(char, len8);
    ::WideCharToMultiByte(CP_UTF8, 0, wide, int(len), ns, len8, NULL, NULL);
    return std::string(ns, len8);
}

std::string toUtf8(const std::wstring& wstr)
{
    return toUtf8(wstr.data(), wstr.length());
}

#endif


} // namespace scy


/// @\}
