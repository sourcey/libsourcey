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
#include "Sourcey/Random.h"
#include "Sourcey/Base64.h"

#include <memory>

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <cstdarg>


using namespace std;


namespace scy {
namespace util {


std::string string_vprintf(const char* fmt, va_list args) 
{
	size_t size = 500;
	char* buf = (char*)malloc(size);
	// Grow the buffer size until the output is no longer truncated
	while (true) {
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
       if (!::isdigit(str[i]))
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
		throw std::runtime_error("Syntax error: Not a valid hexadecimal integer: " + str);
}


std::string memAddress(const void* ptr)
{
	return itostr<const void*>(ptr);
}


std::string randomBinaryString(int size, bool doBase64)
{
	std::string res;
	Random rnd;
	rnd.seed();
	for (int i = 0; i < size; ++i)
		res.push_back(rnd.nextChar());

	if (doBase64) {
		std::string out;
		base64::Encoder enc;
		enc.encode(res, out);
		res = out;
	}
	return res;
}


std::string randomString(int size)
{
	return randomBinaryString(size, true).substr(0, size);
}


UInt32 randomNumber()
{
	Random rnd;
	rnd.seed();
	return rnd.next();
}


void split(const std::string& s, const std::string& delim, std::vector<std::string>& elems, int limit) 
{
	bool final = false;
	std::string::size_type prev = 0, pos = 0;
    while ((pos = s.find(delim, pos)) != std::string::npos) 
	{
		final = static_cast<int>(elems.size() + 1) == limit;
		elems.push_back(s.substr(prev, final ? (s.size() - prev) : (pos - prev)));
        prev = ++pos;
		if (final)
			break;
    }
	if (prev != std::string::npos)
		elems.push_back(s.substr(prev, pos-prev));
}


std::vector<std::string> split(const std::string& s, const std::string& delim, int limit) 
{
    std::vector<std::string> elems;
    split(s, delim, elems, limit);
	return elems;
}


void split(const std::string& s, char delim, std::vector<std::string>& elems, int limit) 
{
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
		if (static_cast<int>(elems.size() + 1) == limit)
			break;
    }
	if (ss.tellg() > 0)
		elems.push_back(ss.str().substr(
		static_cast<unsigned int>(ss.tellg()), 
		static_cast<unsigned int>(s.size() - ss.tellg())));
}


std::vector<std::string> split(const std::string& s, char delim, int limit) 
{
    std::vector<std::string> elems;
    split(s, (char)delim, elems, limit);
    return elems;
}


bool endsWith(const std::string& str, const std::string& suffix)
{
	return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}


double intToDouble(Int64 v) 
{
	if (v+v > 0xFFEULL<<52)
		return 0;
	return ldexp((double)((v&((1LL<<52)-1)) + (1LL<<52)) * (v>>63|1), (int)(v>>52&0x7FF)-1075);
}


float intToFloat(Int32 v)
{
	if (v+v > 0xFF000000U)
		return 0;
	return ldexp((float)((v&0x7FFFFF) + (1<<23)) * (v>>31|1), (int)(v>>23&0xFF)-150);
}


Int64 doubleToInt(double d) 
{
	int e;
	if     ( !d) return 0;
	else if(d-d) return 0x7FF0000000000000LL + ((Int64)(d<0)<<63) + (d!=d);
	d = frexp(d, &e);
	return (Int64)(d<0)<<63 | (e+1022LL)<<52 | (Int64)((fabs(d)-0.5)*(1LL<<53));
}


std::string dumpbin(const char* data, std::size_t len)
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
	std::vector<std::string> lnums = split(l, ".");
	std::vector<std::string> rnums = split(r, ".");
	for (unsigned i = 0; i < lnums.size(); i++) {			
		if (rnums.size() < i + 1)
			break;		
		int ln = util::strtoi<UInt32>(lnums[i]);
		int rn = util::strtoi<UInt32>(rnums[i]);
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
		if (!::isalnum(str[i]) && (!allowSpaces || !::isspace(str[i])) && str[i] != '.')
			str.erase(i, 1);
}


void replaceSpecialCharacters(std::string& str, char with, bool allowSpaces) 
{    
	for (size_t i = 0; i < str.length(); ++i)
		if (!::isalnum(str[i]) && (!allowSpaces || !::isspace(str[i])) && str[i] != '.')
			str[i] = with;
}


void toUnderscore(std::string& str) 
{
	replaceSpecialCharacters(str, '_', false);	
	toLower(str);
}


bool matchNodes(const std::string& node, const std::string& xnode, const std::string& delim)
{
	std::vector<std::string> params = util::split(node, delim);
	std::vector<std::string> xparams = util::split(xnode, delim);
	return matchNodes(params, xparams);
}


bool matchNodes(const std::vector<std::string>& params, const std::vector<std::string>& xparams)
{
	// xparams is a simple matcher pattern with nodes and
	// * as wildcard.
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


std::streamsize copyStream(std::istream& istr, std::ostream& ostr, std::size_t bufferSize)
{
	assert(bufferSize > 0);
	
	std::unique_ptr<char[]> buffer(new char[bufferSize]);
	std::streamsize len = 0;
	istr.read(buffer.get(), bufferSize);
	std::streamsize n = istr.gcount();
	while (n > 0)
	{
		len += n;
		ostr.write(buffer.get(), n);
		if (istr && ostr)
		{
			istr.read(buffer.get(), bufferSize);
			n = istr.gcount();
		}
		else n = 0;
	}
	return len;
}


std::streamsize copyStreamUnbuffered(std::istream& istr, std::ostream& ostr)
{
    char c;
    std::streamsize len = 0;
    istr.get(c);
    while (istr && ostr)
    {
        ++len;
        ostr.put(c);
        istr.get(c);
    }
    return len;
}


std::streamsize copyToString(std::istream& istr, std::string& str, std::size_t bufferSize)
{
	assert(bufferSize > 0);
	
	std::unique_ptr<char[]> buffer(new char[bufferSize]);
	std::streamsize len = 0;
	istr.read(buffer.get(), bufferSize);
	std::streamsize n = istr.gcount();
	while (n > 0)
	{
		len += n;
		str.append(buffer.get(), static_cast<std::string::size_type>(n));
		if (istr)
		{
			istr.read(buffer.get(), bufferSize);
			n = istr.gcount();
		}
		else n = 0;
	}
	return len;
}


} // namespace util
} // namespace scy




/*
void trim(std::string& str) 
{	
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1);
}
*/

/*
void toLowerInPlace(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


std::string toLower(const std::string& str)
{
	std::string res(str);
	toLowerInPlace(res);
	return res;
}


void toUpperInPlace(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}


std::string toUpper(const std::string& str)
{
	std::string res(str);
	toUpperInPlace(res);
	return res;
}


inline int toLower(int ch)
{
	if (::isupper(ch))
		return ch + 32;
	else
		return ch;
}


inline int toUpper(int ch)
{
	if (::islower(ch))
		return ch - 32;
	else
		return ch;
}
*/


	/*
int icompare(const std::string& str, std::string::size_type pos, std::string::size_type n, std::string::const_iterator it2, std::string::const_iterator end2)
{
	std::string::size_type sz = str.size();
	if (pos > sz) pos = sz;
	if (pos + n > sz) n = sz - pos;
	std::string::const_iterator it1  = str.begin() + pos; 
	std::string::const_iterator end1 = str.begin() + pos + n;
	while (it1 != end1 && it2 != end2) {
        std::string::value_type c1 = static_cast<char>(::tolower(*it1)); //util::toLower(*it1);
        std::string::value_type c2 = static_cast<char>(::tolower(*it1)); //util::toLower(*it2);
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


int icompare(const std::string& str1, const std::string& str2)
{
	return icompare(str1, 0, str1.size(), str2.begin(), str2.end());
#ifdef WIN32
	return strnicmp(s1.c_str(), s2.c_str(), s1.length());
#else 
	return stricmp(s1.c_str(), s2.c_str(), s1.length());
#endif 
}
	*/


/*
bool replace(std::string& str, const std::string& from, const std::string& to) 
{
    std::size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;
    std::size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
*/



/*
std::string replace(const std::string& str, const std::string& from, const std::string& to, std::string::size_type start)
{
	std::string res(str);
	replaceInPlace(res, from, to, start);
	return res;
}

	
std::string& replaceInPlace(std::string& str, const std::string& from, const std::string& to, std::string::size_type start)
{
	assert(from.size() > 0);	
	std::string res;
	std::string::size_type pos = 0;
	res.append(str, 0, start);
	do {
		pos = str.find(from, start);
		if (pos != std::string::npos) {
			res.append(str, start, pos - start);
			res.append(to);
			start = pos + from.length();
		}
		else res.append(str, start, str.size() - start);
	}
	while (pos != std::string::npos);
	str.swap(res);
	return str;
}
*/



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