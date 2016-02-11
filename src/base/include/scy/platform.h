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


#ifndef SCY_Platform_H
#define SCY_Platform_H


#include <cstdint>
#include <string>


namespace scy {
    
        
//
/// Cross-platform utilities
//
    
std::string getExePath();
    // Returns the current executable path.

std::string getCwd();
    // Return the current working directory.

std::uint64_t getFreeMemory();
    // Returns the current amount of free memory.

std::uint64_t getTotalMemory();
    // Returns the current amount of used memory.

void sleep(int ms);
    // Pause the current thread for the given ms duration.

void pause();
    // Pause the current thread until enter is pressed.



//
/// Windows helpers
//

#ifdef WIN32

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
