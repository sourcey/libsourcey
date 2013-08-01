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


#include "Sourcey/Util.h"


#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
//#include <cctype>
#include <assert.h>
#include <cstdarg>


using namespace std;


namespace scy {
namespace util {


//
/// String utilities
//


std::string string_vprintf(const char* fmt, va_list args) 
{
	size_t size = 500;
	char* buf = (char*)malloc(size);
	// Grow the buffer size until the output is no longer truncated
	while (1) {
		va_list args_copy;
#if defined(_WIN32)
		args_copy = args;
		size_t nwritten = _vsnprintf(buf, size-1, fmt, args_copy);
#else
		va_copy(args_copy, args);
		size_t nwritten = vsnprintf(buf, size-1, fmt, args_copy);
#endif
		// Some c libraries return -1 for overflow, 
		// some return a number larger than size-1
		if (nwritten < size-2) {
			buf[nwritten+1] = 0;
			std::string ret(buf);
			free(buf);
			return ret;
		}
		size *= 2;
		buf = (char* )realloc(buf, size);
	}
}


std::string format(const char* fmt, ...) 
{
	va_list args;
	va_start(args, fmt);
	std::string ret = string_vprintf(fmt, args);
	va_end(args);
	return ret;
}


bool isNumber(const std::string& str)
{
   for (size_t i = 0; i < str.length(); i++) {
       if (!isdigit(str[i]))
           return false;
   }
   return true;
}


bool tryParseHex(const std::string& str, unsigned& value)
{
	char temp;
	return std::sscanf(str.c_str(), "%x%c", &value, &temp) == 1;
}


unsigned parseHex(const std::string& str)
{
	unsigned result;
	if (tryParseHex(str, result))
		return result;
	else
		throw SyntaxException("Not a valid hexadecimal integer", str);
}


int icompare(const std::string& s1, const std::string& s2)
{
	if (s2.length() > s1.length())
		return -1;
	return strncasecmp(s1.c_str(), s2.c_str(), s1.length());
}


std::string memAddress(const void* ptr)
{
	return toString<const void*>(ptr);
}


void trim(std::string& str) 
{	
	str.erase(0, str.find_first_not_of(' '));	// prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);	// surfixing spaces
}


StringVec &split(const std::string& s, const std::string& delim, StringVec &elems, int limit) 
{
	bool final = false;
	std::string::size_type prev = 0, pos = 0;
    while((pos = s.find(delim, pos)) != std::string::npos) {
		final = static_cast<int>(elems.size() + 1) == limit;
		elems.push_back(s.substr(prev, final ? (s.size() - prev) : (pos - prev)));
        prev = ++pos;
		if (final)
			break;
    }
	if (prev != std::string::npos)
		elems.push_back(s.substr(prev, pos-prev));
	return elems;
}


StringVec split(const std::string& s, const std::string& delim, int limit) 
{
    StringVec elems;
    return split(s, delim, elems, limit);
};


StringVec &split(const std::string& s, char delim, StringVec &elems, int limit) 
{
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
		if (static_cast<int>(elems.size() + 1) == limit)
			break;
    }
	if (ss.tellg() > 0)
		elems.push_back(ss.str().substr(ss.tellg(), s.size() - ss.tellg()));
    return elems;
}


StringVec split(const std::string& s, char delim, int limit) 
{
    StringVec elems;
    return split(s, (char)delim, elems, limit);
}


bool endsWith(const std::string& str, const std::string& suffix)
{
	return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}


std::string replace(const std::string& str, const std::string& from, const std::string& to, std::string::size_type start)
{
	std::string result(str);
	replaceInPlace(result, from, to, start);
	return result;
}


std::string replace(const std::string& str, const std::string::value_type* from, const std::string::value_type* to, std::string::size_type start)
{
	std::string result(str);
	replaceInPlace(result, from, to, start);
	return result;
}

	
std::string& replaceInPlace(std::string& str, const std::string& from, const std::string& to, std::string::size_type start)
{
	assert(from.size() > 0);	
	std::string result;
	std::string::size_type pos = 0;
	result.append(str, 0, start);
	do {
		pos = str.find(from, start);
		if (pos != std::string::npos) {
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + from.length();
		}
		else result.append(str, start, str.size() - start);
	}
	while (pos != std::string::npos);
	str.swap(result);
	return str;
}

std::string& replaceInPlace(std::string& str, const std::string::value_type* from, const std::string::value_type* to, std::string::size_type start)
{
	assert(*from);
	std::string result;
	std::string::size_type pos = 0;
	std::string::size_type fromLen = std::strlen(from);
	result.append(str, 0, start);
	do {
		pos = str.find(from, start);
		if (pos != std::string::npos) {
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + fromLen;
		}
		else result.append(str, start, str.size() - start);
	}
	while (pos != std::string::npos);
	str.swap(result);
	return str;
}


std::string dumpbin(const char* data, size_t len)
{
	std::string output;
	for (size_t i = 0; i < len; i++) {
		char byte = data[i];
		for (size_t mask = 0x80; mask > 0; mask >>= 1) {
			output.push_back(byte & mask ? '1' : '0');
		}
		if (i % 4 == 3)
			output.push_back('\n');
		else
			output.push_back(' ');
	}
	return output;
}


bool compareVersion(const std::string& l, const std::string& r) 
{
	if (l.empty())
		return false;
	if (r.empty())
		return true;

	bool isEqual = true;
	StringVec lnums = split(l, ".");
	StringVec rnums = split(r, ".");
	for (unsigned i = 0; i < lnums.size(); i++) {			
		if (rnums.size() < i + 1)
			break;		
		int ln = util::fromString<UInt32>(lnums[i]);
		int rn = util::fromString<UInt32>(rnums[i]);
		if (ln < rn)
			return false;
		else if (ln > rn)
			isEqual = false;
	}
	return !isEqual;
}


void removeSpecialCharacters(std::string& str, bool allowSpaces) 
{    
	for (size_t i = 0; i < str.length(); ++i)
		if (!::isalnum(str[i]) || (!allowSpaces && ::isspace(str[i])))
			str.erase(i, 1);
}


void replaceSpecialCharacters(std::string& str, char with, bool allowSpaces) 
{    
	for (size_t i = 0; i < str.length(); ++i)
		if (!::isalnum(str[i]) || (!allowSpaces && ::isspace(str[i])))
			str[i] = with;
}


void toLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


void toUpper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}


void toUnderscore(std::string& str) 
{
	replaceSpecialCharacters(str, '_', false);	
	toLower(str);
}

/*
bool replace(std::string& str, const std::string& from, const std::string& to) 
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
*/


bool matchNodes(const std::string& node, const std::string& xnode, const std::string& delim)
{
	StringVec params = util::split(node, delim);
	StringVec xparams = util::split(xnode, delim);
	return matchNodes(params, xparams);
}


bool matchNodes(const StringVec& params, const StringVec& xparams)
{
	// xparams is a simple matcher pattern with nodes and
	// * as windcard.
	// No match if xparams are greater than the params.
	if (xparams.size() > params.size())
		return false;
	
	// If params is longer the last xparam the last xparam
	// must be a *.
	if (params.size() > xparams.size() && 
		xparams[xparams.size() - 1] != "*")
		return false;

	for (size_t i = 0; i < xparams.size(); ++i) {

		// Wildcard * matches anything.
		if (xparams[i] == "*") 
			continue;
		
		if (xparams[i] != params[i])
			return false;
	}

	return true;
}


} // namespace util
} // namespace scy





/*
#ifdef WIN32
UInt64 getTimeHR() {
	return ::GetTickCount();
}
#else
static int ClocksPerSec = sysconf(_SC_CLK_TCK);
UInt64 getTimeHR() {
	tms t;
	clock_t result = times(&t);
	return (UInt64)result;
}
#endif


void pause()
{
	//cin.ignore(1024, '\n');
	cout << "Press enter to continue...";
	cin.get();
}
*/