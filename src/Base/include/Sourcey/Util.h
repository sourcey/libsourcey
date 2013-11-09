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


#ifndef SOURCEY_Util_H
#define SOURCEY_Util_H


#include "Sourcey/Types.h"
#include "Sourcey/Exception.h"

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <map>


namespace scy {
namespace util {


std::string format(const char* fmt, ...);
	// Printf style string formatting for POD types.

void toUnderscore(std::string& str);
	// Replaces special characters in the given string with 
	// underscores and transform to lowercase.

bool isNumber(const std::string& str);
	// Checks if the string is a number

bool endsWith(const std::string& str, const std::string& suffix);
	// Returns true if the string ends with the given substring.

void removeSpecialCharacters(std::string& str, bool allowSpaces = false);
void replaceSpecialCharacters(std::string& str, char with = '_', bool allowSpaces = false);
	// Replaces non-alphanumeric characters.
	
bool tryParseHex(const std::string& s, unsigned& value);
unsigned parseHex(const std::string& s);	
	// String to hex value.

std::string dumpbin(const char* data, std::size_t len);
	// Dumps the binary representation of the 
	// given buffer to the output string.

bool compareVersion(const std::string& l, const std::string& r);
	// Compares two version strings ie. 3.7.8.0 > 3.2.1.0
	// If L is greater than R the function returns true.
	// If L is equal or less than R the function returns false.

bool matchNodes(const std::string& node, const std::string& xnode, const std::string& delim = "\r\n");
bool matchNodes(const std::vector<std::string>& params, const std::vector<std::string>& xparams);
	// Matches two node lists against each other.

std::string memAddress(const void* ptr);
	// Returns the pointer memory address as a string.


//
// Type converters
//
	
template<typename T>
std::string itostr(const T& t) 
	// Converts integer T to string.
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}
	
template<typename T>
T strtoi(const std::string& s) 
	// Converts string to integer T.
	// Ensure the integer type has  
	// sufficient storage capacity.
{
    std::istringstream iss(s);
    T x;
	if (!(iss >> x))
		return 0;
    return x;
}

/*
double intToDouble(Int64 v);
	// Interger to double

float intToFloat(Int32 v);
	// Interger to float

Int64 doubleToInt(double d);
	// Double to interger
*/


//
// Random generators
//

UInt32 randomNumber();
	// Generates a 31-bit pseudo random number.

std::string randomString(int size);
	// Generates a random string.

std::string randomBinaryString(int size, bool doBase64 = false);
	// Generates a random (optionally base64 encoded) binary key.


//
// String splitters
//

void split(const std::string& str, const std::string& delim, std::vector<std::string>& elems, int limit = -1);
std::vector<std::string> split(const std::string& str, const std::string& delim, int limit = -1);
	// Splits the given string at the delimiter string.

void split(const std::string& str, char delim, std::vector<std::string>& elems, int limit = -1);
std::vector<std::string> split(const std::string& str, char delim, int limit = -1);
	// Splits the given string at the delimiter character.


//
// String replace methods (POCO)
//

template <class S>
S& replaceInPlace(S& str, const S& from, const S& to, typename S::size_type start = 0)
{
	assert(from.size() > 0);
	
	S result;
	typename S::size_type pos = 0;
	result.append(str, 0, start);
	do
	{
		pos = str.find(from, start);
		if (pos != S::npos)
		{
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + from.length();
		}
		else result.append(str, start, str.size() - start);
	}
	while (pos != S::npos);
	str.swap(result);
	return str;
}

template <class S>
S& replaceInPlace(S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{
	assert(*from);

	S result;
	typename S::size_type pos = 0;
	typename S::size_type fromLen = std::strlen(from);
	result.append(str, 0, start);
	do {
		pos = str.find(from, start);
		if (pos != S::npos) {
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + fromLen;
		}
		else result.append(str, start, str.size() - start);
	}
	while (pos != S::npos);
	str.swap(result);
	return str;
}

template <class S>
S replace(const S& str, const S& from, const S& to, typename S::size_type start = 0)
	/// Replace all occurences of from (which must not be the empty string)
	/// in str with to, starting at position start.
{
	S result(str);
	replaceInPlace(result, from, to, start);
	return result;
}

template <class S>
S replace(const S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, from, to, start);
	return result;
}


//
// String trimming (POCO)
//

template <class S>
S trimLeft(const S& str)
	/// Returns a copy of str with all leading
	/// whitespace removed.
{
	typename S::const_iterator it  = str.begin();
	typename S::const_iterator end = str.end();
	
	while (it != end && ::isspace(*it)) ++it;
	return S(it, end);
}

template <class S>
S& trimLeftInPlace(S& str)
	/// Removes all leading whitespace in str.
{
	typename S::iterator it  = str.begin();
	typename S::iterator end = str.end();
	
	while (it != end && ::isspace(*it)) ++it;
	str.erase(str.begin(), it);
	return str;
}

template <class S>
S trimRight(const S& str)
	/// Returns a copy of str with all trailing
	/// whitespace removed.
{
	int pos = int(str.size()) - 1;
		
	while (pos >= 0 && ::isspace(str[pos])) --pos;
	return S(str, 0, pos + 1);
}

template <class S>
S& trimRightInPlace(S& str)
	/// Removes all trailing whitespace in str.
{
	int pos = int(str.size()) - 1;
		
	while (pos >= 0 && ::isspace(str[pos])) --pos;
	str.resize(pos + 1);

	return str;
}

template <class S>
S trim(const S& str)
	/// Returns a copy of str with all leading and
	/// trailing whitespace removed.
{
	int first = 0;
	int last  = int(str.size()) - 1;
	
	while (first <= last && ::isspace(str[first])) ++first;
	while (last >= first && ::isspace(str[last])) --last;

	return S(str, first, last - first + 1);
}

template <class S>
S& trimInPlace(S& str)
	/// Removes all leading and trailing whitespace in str.
{
	int first = 0;
	int last  = int(str.size()) - 1;
	
	while (first <= last && ::isspace(str[first])) ++first;
	while (last >= first && ::isspace(str[last])) --last;

	str.resize(last + 1);
	str.erase(0, first);

	return str;
}


//
// String case conversion (POCO)
//

template <class S>
S toUpper(const S& str)
	/// Returns a copy of str containing all upper-case characters.
{
	typename S::const_iterator it  = str.begin();
	typename S::const_iterator end = str.end();

	S result;
	result.reserve(str.size());
	while (it != end) result += static_cast<char>(::toupper(*it++));
	return result;
}

template <class S>
S& toUpperInPlace(S& str)
	/// Replaces all characters in str with their upper-case counterparts.
{
	typename S::iterator it  = str.begin();
	typename S::iterator end = str.end();

	while (it != end) { *it = static_cast<char>(::toupper(*it)); ++it; }
	return str;
}

template <class S>
S toLower(const S& str)
	/// Returns a copy of str containing all lower-case characters.
{
	typename S::const_iterator it  = str.begin();
	typename S::const_iterator end = str.end();

	S result;
	result.reserve(str.size());
	while (it != end) result += static_cast<char>(::tolower(*it++));
	return result;
}

template <class S>
S& toLowerInPlace(S& str)
	/// Replaces all characters in str with their lower-case counterparts.
{
	typename S::iterator it  = str.begin();
	typename S::iterator end = str.end();

	while (it != end) { *it = static_cast<char>(::tolower(*it)); ++it; }
	return str;
}


//
// String case-insensative comparators (POCO)
//

template <class S, class It>
int icompare(
	const S& str,
	typename S::size_type pos, 
	typename S::size_type n,
	It it2, 
	It end2)
	/// Case-insensitive string comparison
{
	typename S::size_type sz = str.size();
	if (pos > sz) pos = sz;
	if (pos + n > sz) n = sz - pos;
	It it1  = str.begin() + pos; 
	It end1 = str.begin() + pos + n;
	while (it1 != end1 && it2 != end2)
	{
        typename S::value_type c1(::tolower(*it1));
        typename S::value_type c2(::tolower(*it2));
        if (c1 < c2)
            return -1;
        else if (c1 > c2)
            return 1;
        ++it1; ++it2;
	}
    
    if (it1 == end1)
		return it2 == end2 ? 0 : -1;
    else
        return 1;
}

template <class S>
int icompare(const S& str1, const S& str2)
{
	typename S::const_iterator it1(str1.begin());
	typename S::const_iterator end1(str1.end());
	typename S::const_iterator it2(str2.begin());
	typename S::const_iterator end2(str2.end());
	while (it1 != end1 && it2 != end2)
	{
        typename S::value_type c1(static_cast<char>(::tolower(*it1)));
        typename S::value_type c2(static_cast<char>(::tolower(*it2)));
        if (c1 < c2)
            return -1;
        else if (c1 > c2)
            return 1;
        ++it1; ++it2;
	}
    
    if (it1 == end1)
		return it2 == end2 ? 0 : -1;
    else
        return 1;
}

template <class S>
int icompare(const S& str1, typename S::size_type n1, const S& str2, typename S::size_type n2)
{
	if (n2 > str2.size()) n2 = str2.size();
	return icompare(str1, 0, n1, str2.begin(), str2.begin() + n2);
}

template <class S>
int icompare(const S& str1, typename S::size_type n, const S& str2)
{
	if (n > str2.size()) n = str2.size();
	return icompare(str1, 0, n, str2.begin(), str2.begin() + n);
}

template <class S>
int icompare(const S& str1, typename S::size_type pos, typename S::size_type n, const S& str2)
{
	return icompare(str1, pos, n, str2.begin(), str2.end());
}

template <class S>
int icompare(
	const S& str1, 
	typename S::size_type pos1, 
	typename S::size_type n1, 
	const S& str2,
	typename S::size_type pos2,
	typename S::size_type n2)
{
	typename S::size_type sz2 = str2.size();
	if (pos2 > sz2) pos2 = sz2;
	if (pos2 + n2 > sz2) n2 = sz2 - pos2;
	return icompare(str1, pos1, n1, str2.begin() + pos2, str2.begin() + pos2 + n2);
}

template <class S>
int icompare(
	const S& str1, 
	typename S::size_type pos1, 
	typename S::size_type n, 
	const S& str2,
	typename S::size_type pos2)
{
	typename S::size_type sz2 = str2.size();
	if (pos2 > sz2) pos2 = sz2;
	if (pos2 + n > sz2) n = sz2 - pos2;
	return icompare(str1, pos1, n, str2.begin() + pos2, str2.begin() + pos2 + n);
}

template <class S>
int icompare(
	const S& str,
	typename S::size_type pos,
	typename S::size_type n,
	const typename S::value_type* ptr)
{
	assert(ptr);
	typename S::size_type sz = str.size();
	if (pos > sz) pos = sz;
	if (pos + n > sz) n = sz - pos;
	typename S::const_iterator it  = str.begin() + pos; 
	typename S::const_iterator end = str.begin() + pos + n;
	while (it != end && *ptr) 
	{
        typename S::value_type c1(static_cast<char>(::tolower(*it)));
        typename S::value_type c2(static_cast<char>(::tolower(*ptr)));
        if (c1 < c2)
            return -1;
        else if (c1 > c2)
            return 1;
        ++it; ++ptr;
	}
    
    if (it == end)
		return *ptr == 0 ? 0 : -1;
    else
        return 1;
}

template <class S>
int icompare(
	const S& str,
	typename S::size_type pos,
	const typename S::value_type* ptr)
{
	return icompare(str, pos, str.size() - pos, ptr);
}

template <class S>
int icompare(
	const S& str,
	const typename S::value_type* ptr)
{
	return icompare(str, 0, str.size(), ptr);
}


//
// Stream copiers (POCO)
//

std::streamsize copyStreamUnbuffered(std::istream& istr, std::ostream& ostr);
std::streamsize copyStream(std::istream& istr, std::ostream& ostr, std::size_t bufferSize = 8192);
std::streamsize copyToString(std::istream& istr, std::string& str, std::size_t bufferSize = 8192);


//
// Container helpers
//

template<typename Val>
inline void clearList(std::list<Val*>& L)
	// Delete all elements from a list of pointers.
	// @param L List of pointers to delete.
{	
	typename std::list<Val*>::iterator it = L.begin();
	while (it != L.end()) {
		delete *it;
		it = L.erase(it);
	}
}

template<typename Val>
inline void clearDeque(std::deque<Val*>& D)
	// Delete all elements from a list of pointers.
	// @param D List of pointers to delete.
{
	typename std::deque<Val*>::iterator it = D.begin();
	while (it != D.end()) {
		delete *it;
		it = D.erase(it);
	}
}

template<typename Val>
inline void clearVector(std::vector<Val*>& V)
	// Delete all elements from a vector of pointers.
	// @param V Vector of pointers to delete.
{
	typename std::vector<Val*>::iterator it = V.begin();
	while (it != V.end()) {
		delete *it;		
		//Deleter::func(*it);
		it = V.erase(it);
	}
}

template<typename Key, typename Val>
inline void clearMap(std::map<Key, Val*>& M)
	// Delete all associated values from a map (not the key elements).
	// @param M Map of pointer values to delete.
{
	typename std::map<Key, Val*>::iterator it = M.begin();
	typename std::map<Key, Val*>::iterator it2;
	while (it != M.end()) {
		it2 = it++;
		delete (*it2).second;
		M.erase(it2);
	}
}

template<typename Deleter, typename Key, typename Val>
inline void clearMap(std::map<Key, Val*>& M)
	// Delete all associated values from a map (not the key elements)
	// using the given deleter method.
	// @param M Map of pointer values to delete.
{
	typename std::map<Key, Val*>::iterator it = M.begin();
	typename std::map<Key, Val*>::iterator it2;
	while (it != M.end()) {
		it2 = it++;
		Deleter func;
		func((*it2).second);
		M.erase(it2);
	}
}

template<typename Key, typename Val>
inline void clearMap(std::map<const Key, Val*>& M)
	// Delete all associated values from a map (not the key elements).
	// Const key type version.
	// @param M Map of pointer values to delete.
{
	typename std::map<const Key, Val*>::iterator it = M.begin();
	typename std::map<const Key, Val*>::iterator it2;
	while (it != M.end()) {
		it2 = it++;
		delete (*it2).second;
		M.erase(it2);
	}
}


} // namespace util
} // namespace scy


#endif // SOURCEY_Util_H


//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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
