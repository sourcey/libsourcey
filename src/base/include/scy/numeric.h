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
// This file uses functions from POCO C++ Libraries (license below)
//


#ifndef SCY_Numeric_H
#define SCY_Numeric_H


#include "scy/types.h"
#include <string>


#if defined(_MSC_VER) || defined(__MINGW32__)
	#define I64_FMT "I64"
#elif defined(__APPLE__) 
	#define I64_FMT "q"
#else
	#define I64_FMT "ll"
#endif


namespace scy {
namespace numeric {


//
// Integer To String Formatting
//
	

void format(std::string& str, int value);
	// Formats an integer value in decimal notation.

void format(std::string& str, int value, int width);
	// Formats an integer value in decimal notation,
	// right justified in a field having at least
	// the specified width.

void format0(std::string& str, int value, int width);
	// Formats an integer value in decimal notation, 
	// right justified and zero-padded in a field
	// having at least the specified width.

void formatHex(std::string& str, int value);
	// Formats an int value in hexadecimal notation.
	// The value is treated as unsigned.

void formatHex(std::string& str, int value, int width);
	// Formats a int value in hexadecimal notation,
	// right justified and zero-padded in
	// a field having at least the specified width.
	// The value is treated as unsigned.

void format(std::string& str, unsigned value);
	// Formats an unsigned int value in decimal notation.

void format(std::string& str, unsigned value, int width);
	// Formats an unsigned long int in decimal notation,
	// right justified in a field having at least the
	// specified width.

void format0(std::string& str, unsigned int value, int width);
	// Formats an unsigned int value in decimal notation, 
	// right justified and zero-padded in a field having at 
	// least the specified width.

void formatHex(std::string& str, unsigned value);
	// Formats an unsigned int value in hexadecimal notation.

void formatHex(std::string& str, unsigned value, int width);
	// Formats a int value in hexadecimal notation,
	// right justified and zero-padded in
	// a field having at least the specified width.

void format(std::string& str, long value);
	// Formats a long value in decimal notation.

void format(std::string& str, long value, int width);
	// Formats a long value in decimal notation,
	// right justified in a field having at least the 
	// specified width.

void format0(std::string& str, long value, int width);
	// Formats a long value in decimal notation, 
	// right justified and zero-padded in a field
	// having at least the specified width.

void formatHex(std::string& str, long value);
	// Formats an unsigned long value in hexadecimal notation.
	// The value is treated as unsigned.

void formatHex(std::string& str, long value, int width);
	// Formats an unsigned long value in hexadecimal notation,
	// right justified and zero-padded in a field having at least the 
	// specified width.
	// The value is treated as unsigned.

void format(std::string& str, unsigned long value);
	// Formats an unsigned long value in decimal notation.

void format(std::string& str, unsigned long value, int width);
	// Formats an unsigned long value in decimal notation,
	// right justified in a field having at least the specified 
	// width.

void format0(std::string& str, unsigned long value, int width);
	// Formats an unsigned long value in decimal notation, 
	// right justified and zero-padded
	// in a field having at least the specified width.

void formatHex(std::string& str, unsigned long value);
	// Formats an unsigned long value in hexadecimal notation.

void formatHex(std::string& str, unsigned long value, int width);
	// Formats an unsigned long value in hexadecimal notation,
	// right justified and zero-padded in a field having at least the 
	// specified width.

void format(std::string& str, Int64 value);
	// Formats a 64-bit integer value in decimal notation.

void format(std::string& str, Int64 value, int width);
	// Formats a 64-bit integer value in decimal notation,
	// right justified in a field having at least the specified width.

void format0(std::string& str, Int64 value, int width);
	// Formats a 64-bit integer value in decimal notation, 
	// right justified and zero-padded in a field having at least 
	// the specified width.

void formatHex(std::string& str, Int64 value);
	// Formats a 64-bit integer value in hexadecimal notation.
	// The value is treated as unsigned.

void formatHex(std::string& str, Int64 value, int width);
	// Formats a 64-bit integer value in hexadecimal notation,
	// right justified and zero-padded in a field having at least 
	// the specified width.
	// The value is treated as unsigned.

void format(std::string& str, UInt64 value);
	// Formats an unsigned 64-bit integer value in decimal notation.

void format(std::string& str, UInt64 value, int width);
	// Formats an unsigned 64-bit integer value in decimal notation,
	// right justified in a field having at least the specified width.

void format0(std::string& str, UInt64 value, int width);
	// Formats an unsigned 64-bit integer value in decimal notation, 
	// right justified and zero-padded in a field having at least the 
	// specified width.

void formatHex(std::string& str, UInt64 value);
	// Formats a 64-bit integer value in hexadecimal notation.

void formatHex(std::string& str, UInt64 value, int width);
	// Formats a 64-bit integer value in hexadecimal notation,
	// right justified and zero-padded in a field having at least 
	// the specified width.

#if 0
void format(std::string& str, float value);
	// Formats a float value in decimal floating-point notation,
	// according to std::printf's %g format with a precision of 8 fractional digits.

void format(std::string& str, double value);
	// Formats a double value in decimal floating-point notation,
	// according to std::printf's %g format with a precision of 16 fractional digits.

void format(std::string& str, double value, int precision);
	// Formats a double value in decimal floating-point notation,
	// according to std::printf's %f format with the given precision.

void format(std::string& str, double value, int width, int precision);
	// Formats a double value in decimal floating-point notation,
	// right justified in a field of the specified width,
	// with the number of fractional digits given in precision.

void format(std::string& str, const void* ptr);
	// Formats a pointer in an eight (32-bit architectures) or
	// sixteen (64-bit architectures) characters wide
	// field in hexadecimal notation.
#endif


} } // namespace scy::numeric


#endif // SCY_Numeric_H


//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//