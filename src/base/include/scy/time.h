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

#ifndef SCY_Time_H
#define SCY_Time_H


#include "scy/types.h"
#include <string>
#include <ctime>


namespace scy {
namespace time {


static const char* ISO8601Format = "%Y-%m-%dT%H:%M:%SZ";
		// The date/time format defined in the ISO 8601 standard.
		// This is the default format used throughout the library for consistency.
		//
		// Examples: 
		//   2005-01-01T12:00:00+01:00
		//   2005-01-01T11:00:00Z
		   
std::time_t now();
	// Returns the current system time.

UInt64 ticks();
	// Returns the number of millisecond ticks since epoch.

double clockSecs();
	// Returns the current process time in decimal seconds.

std::string print(const std::tm& dt, const char* fmt = ISO8601Format);
	// Cross-platform time formatting.

std::string printLocal(const char* fmt = ISO8601Format);
	// Prints the current local time using the given format.

std::string printUTC(const char* fmt = ISO8601Format);
	// Prints the current UTC time using the given format.

std::tm toLocal(const std::time_t& time);
	// Converts the given time value to local time.
	// Uses thread-safe native functions.

std::tm toUTC(const std::time_t& time);
	// Converts the given time value to UTC time.
	// Uses thread-safe native functions.

std::string getLocal();
	// Returns a local ISO8601 formatted date time string.

std::string getUTC();
	// Returns a UTC ISO8601 formatted date time string.


#if 0
UInt64 hrtime();
	// Returns the current high-resolution real time in nanoseconds.

UInt64 getTimeMS();
	// Returns the current high-resolution real time in milliseconds.
#endif


} } // namespace scy::time


#endif // SCY_Time_H
