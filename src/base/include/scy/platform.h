///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Platform_H
#define SCY_Platform_H


#include <cstdint>
#include <string>


namespace scy {


//
/// Cross-platform utilities
//

/// Returns the current executable path.
std::string getExePath();

/// Return the current working directory.
std::string getCwd();

/// Returns the current amount of free memory.
std::uint64_t getFreeMemory();

/// Returns the current amount of used memory.
std::uint64_t getTotalMemory();

/// Pause the current thread for the given ms duration.
void sleep(int ms);

/// Pause the current thread until enter is pressed.
void pause();


//
/// Windows helpers
//

#ifdef SCY_WIN

bool getOsVersion(int* major, int* minor, int* build);
bool isWindowsVistaOrLater();
bool isWindowsXpOrLater();

std::wstring toUtf16(const char* utf8, std::size_t len);
std::wstring toUtf16(const std::string& str);
std::string toUtf8(const wchar_t* wide, std::size_t len);
std::string toUtf8(const std::wstring& wstr);

#endif


} // namespace scy


#endif // SCY_Platform_H


/// @\}
