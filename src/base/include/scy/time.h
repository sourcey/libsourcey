///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Time_H
#define SCY_Time_H


#include <cstdint>
#include <ctime>
#include <string>


namespace scy {


/// Classes and functions for handling time
namespace time {


/// The date/time format defined in the ISO 8601 standard.
/// This is the default format used throughout the library for consistency.
///
/// Examples:
///   2005-01-01T12:00:00+01:00
///   2005-01-01T11:00:00Z
static const char* ISO8601Format = "%Y-%m-%dT%H:%M:%SZ";

/// Return the number of UTC milliseconds since epoch.
std::time_t now();

/// Return the current process time in decimal seconds.
double clockSecs();

/// Cross-platform time formatting.
std::string print(const std::tm& dt, const char* fmt = ISO8601Format);

/// Print the current local time using the given format.
std::string printLocal(const char* fmt = ISO8601Format);

/// Print the current UTC time using the given format.
std::string printUTC(const char* fmt = ISO8601Format);

/// Convert the given time value to local time.
/// Uses thread-safe native functions.
std::tm toLocal(const std::time_t& time);

/// Convert the given time value to UTC time.
/// Uses thread-safe native functions.
std::tm toUTC(const std::time_t& time);

/// Return a local ISO8601 formatted date time string.
std::string getLocal();

/// Return a UTC ISO8601 formatted date time string.
std::string getUTC();

/// Return the current high-resolution real time in nanoseconds.
std::uint64_t hrtime();

#if 0
/// Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.
std::uint64_t ticks();

/// Return the current real time in milliseconds.
std::uint64_t getTimeMS();
#endif


} // namespace time
} // namespace scy


#endif // SCY_Time_H


/// @\}
