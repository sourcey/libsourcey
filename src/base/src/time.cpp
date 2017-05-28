///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/time.h"
#include "scy/util.h"

#include "uv.h"

#include <ctime>
#include <time.h>
//#include <chrono>
#include <iomanip>
#include <locale>
#include <sstream>


namespace scy {
namespace time {


std::time_t now()
{
#if 0 // no need for chrono here yet
    std::chrono::time_point<std::chrono::system_clock> system_now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(system_now);
#endif
    return std::time(0);
}


double clockSecs()
{
    return clock() / CLOCKS_PER_SEC;
}


std::tm toLocal(const std::time_t& time)
{
    std::tm tm_snapshot;
#if defined(WIN32)
    localtime_s(&tm_snapshot, &time); // thread-safe?
#else
    localtime_r(&time, &tm_snapshot); // POSIX
#endif
    return tm_snapshot;
}


std::tm toUTC(const std::time_t& time)
{
    // TODO: double check thread safety of native methods
    std::tm tm_snapshot;
#if defined(WIN32)
    gmtime_s(&tm_snapshot, &time); // thread-safe?
#else
    gmtime_r(&time, &tm_snapshot);    // POSIX
#endif
    return tm_snapshot;
}


std::string print(const std::tm& dt, const char* fmt)
{
#if defined(WIN32)
    // BOGUS hack done for VS2012: C++11 non-conformant since
    // it SHOULD take a `const struct tm*`
    // ref. C++11 standard: ISO/IEC 14882:2011, � 27.7.1
    std::ostringstream oss;
    oss << std::put_time(const_cast<std::tm*>(&dt), fmt);
    return oss.str();

#else // LINUX
    const size_t size = 1024;
    char buffer[size];
    auto success = std::strftime(buffer, size, fmt, &dt);

    if (0 == success)
        return fmt;

    return buffer;
#endif
}


std::string printLocal(const char* fmt)
{
    return print(toLocal(now()), fmt);
}


std::string printUTC(const char* fmt)
{
    return print(toUTC(now()), fmt);
}


std::string getLocal()
{
    return printLocal(ISO8601Format);
}


std::string getUTC()
{
    return printUTC(ISO8601Format);
}


uint64_t hrtime()
{
    return uv_hrtime();
}


#if 0
std::time_t nowUTC()
{
    std::time_t local = std::time(NULL);
    return std::mktime(std::gmtime(&local)); // UTC time
}

uint64_t ticks()
{
#ifdef SCY_WIN
    return ::GetTickCount();
#else
    struct timespec tval;
    clock_gettime(CLOCK_MONOTONIC, &tval);
    return tval.tv_sec * 1000 + tval.tv_nsec / 1000000;
#endif
}


uint64_t getTimeMS()
{
    return uv_hrtime() / 1000000;
}
#endif


} // namespace time
} // namespace scy


/// @\}
