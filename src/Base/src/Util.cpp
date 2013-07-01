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
#include "Poco/String.h"

#ifdef _WIN32
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#else
#include <time.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#endif

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdio.h>


using namespace std;
using namespace Poco;


namespace scy {
namespace util {


bool isNumber(const string& str)
{
   for (int i = 0; i < str.length(); i++) {
       if (!isdigit(str[i]))
           return false;
   }
   return true;
}


string getPID(const void* ptr)
{
	return toString<const void*>(ptr);
}


void trim(string& str) 
{	
	str.erase(0, str.find_first_not_of(' '));	// prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);	// surfixing spaces
}


StringVec &split(const string& s, const string& delim, StringVec &elems, int limit) 
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


StringVec split(const string& s, const string& delim, int limit) 
{
    StringVec elems;
    return split(s, delim, elems, limit);
};


StringVec &split(const string& s, char delim, StringVec &elems, int limit) 
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
		if (static_cast<int>(elems.size() + 1) == limit)
			break;
    }
	if (ss.tellg() > 0)
		elems.push_back(ss.str().substr(ss.tellg(), s.size() - ss.tellg()));
    return elems;
}


StringVec split(const string& s, char delim, int limit) 
{
    StringVec elems;
    return split(s, (char)delim, elems, limit);
}


void escape(string& str) 
{
	const char escape_chars[] = { '&', '<', '>', '\'', '"' };
	const string escape_seqs[] = { "amp;", "lt;", "gt;", "apos;", "quot;" };
	const unsigned escape_size = 5;
	for (size_t val, i = 0; i < str.length(); ++i) {
		for (val = 0; val < escape_size; ++val) {
			if (str[i] == escape_chars[val]) {
				str[i] = '&';
				str.insert(i + 1, escape_seqs[val]);
				i += escape_seqs[val].length();
				break;
			}
		}
	}
}


bool endsWith(const string& str, const string& suffix)
{
	return equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}


void pause()
{
	//cin.ignore(1024, '\n');
	cout << "Press enter to continue...";
	cin.get();
}


string dumpbin(const char* data, size_t len)
{
	string output;
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


bool compareVersion(const string& l, const string& r) 
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


void removeSpecialCharacters(string& str, bool allowSpaces) 
{    
	for (size_t i = 0; i < str.length(); ++i)
		if (!isalnum(str[i]) || (!allowSpaces && isspace(str[i])))
			str.erase(i, 1);
}


void replaceSpecialCharacters(string& str, char with, bool allowSpaces) 
{    
	for (size_t i = 0; i < str.length(); ++i)
		if (!isalnum(str[i]) || (!allowSpaces && isspace(str[i])))
			str[i] = with;
}


void underscore(string& str) 
{
	replaceSpecialCharacters(str, '_', false);	
	transform(str.begin(), str.end(), str.begin(), ::tolower);
}


bool matchNodes(const string& node, const string& xnode, const string& delim)
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
		
		//if (xparams.size() == i + 1)
		//	return false;

		// Wildcard * matches anything.
		if (xparams[i] == "*") 
			continue;
		
		if (xparams[i] != params[i])
			return false;
	}

	return true;
}


#if WIN32
UInt64 getTime() {
	return ::GetTickCount();
}
#else
static int ClocksPerSec = sysconf(_SC_CLK_TCK);
UInt64 getTime() {
	tms t;
	clock_t result = times(&t);
	return (UInt64)result;
}
#endif


#if WIN32
bool getOsVersion(int* major, int* minor, int* build) {
	OSVERSIONINFO info = {0};
	info.dwOSVersionInfoSize = sizeof(info);
	if (GetVersionEx(&info)) {
		if (major) *major = info.dwMajorVersion;
		if (minor) *minor = info.dwMinorVersion;
		if (build) *build = info.dwBuildNumber;
		return true;
	}
	return false;
}
#endif


} // namespace util
} // namespace scy