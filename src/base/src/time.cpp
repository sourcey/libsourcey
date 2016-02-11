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


#include "scy/time.h"
#include "scy/uv/uvpp.h"
#include "scy/util.h"
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
    gmtime_r(&time, &tm_snapshot); // POSIX  
#endif
    return tm_snapshot;
}


std::string print(const std::tm& dt, const char* fmt)
{
#if defined(WIN32)     
    // BOGUS hack done for VS2012: C++11 non-conformant since it SHOULD take a "const struct tm* "
    // ref. C++11 standard: ISO/IEC 14882:2011, � 27.7.1, 
    std::ostringstream oss;
    oss << std::put_time(const_cast<std::tm*>(&dt), fmt); 
    return oss.str();

#else    // LINUX
    const std::size_t size = 1024;
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
    

#if 0
std::time_t nowUTC()
{
    std::time_t local = std::time(NULL);
    return std::mktime(std::gmtime(&local)); // UTC time
}

std::uint64_t ticks()
{
#ifdef WIN32
    return ::GetTickCount();
#else
    struct timespec tval;
    clock_gettime(CLOCK_MONOTONIC, &tval);
    return tval.tv_sec * 1000 + tval.tv_nsec / 1000000;
#endif
}

std::uint64_t getTimeHR() 
{
    return uv_hrtime();
}
    

std::uint64_t getTimeMS() 
{
    return uv_hrtime() / 1000000;
}
#endif



} } // namespace scy::time