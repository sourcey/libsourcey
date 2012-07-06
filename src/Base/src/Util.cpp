//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
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
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdio.h>


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Util {


#ifdef _WIN32
UInt32 getTime() {
	return ::GetTickCount();
}
#else
static int ClocksPerSec = sysconf(_SC_CLK_TCK);

UInt32 getTime() {
	tms t;
	clock_t result = times(&t);
	return result & 0xFFFFFFFF;
}
#endif


double atod(const string& str)
{
	istringstream i(str);
	double x;
	if (!(i >> x))
		return 0;
	return x;
} 


bool isNumber(const string& str)
{
   for (int i = 0; i < str.length(); i++) {
       if (!isdigit(str[i]))
           return false;
   }
   return true;
}


unsigned atoi(const string& str)
{
	istringstream i(str);
	unsigned x;
	if (!(i >> x)) x = 0;
	return x;
}


string itoa(unsigned i)
{
	ostringstream out;
	out << i;
	return out.str();
}


string dtoa(double d)
{
	stringstream out;
	out << d;
	return out.str();
}


string getPID(void* ptr)
{
	stringstream out;
	out << ptr;
	return out.str();
}


string trim(const string& str) 
{	
	string what = str;
	what.erase(0, what.find_first_not_of(' '));       //prefixing spaces
	what.erase(what.find_last_not_of(' ')+1);         //surfixing spaces
	return what;
}


StringList &split(const string& s, const string& delim, StringList &elems, int limit) 
{
	bool final = false;
	std::string::size_type prev = 0, pos = 0;
    while((pos = s.find(delim, pos)) != std::string::npos) {
		final = elems.size() + 1 == limit;
		elems.push_back(s.substr(prev, final ? (s.size() - prev) : (pos - prev)));
        prev = ++pos;
		if (final)
			break;
    }
	if (prev != std::string::npos)
		elems.push_back(s.substr(prev, pos-prev));
	return elems;
}


StringList split(const string& s, const string& delim, int limit) 
{
    StringList elems;
    return split(s, delim, elems, limit);
};



StringList &split(const string& s, char delim, StringList &elems, int limit) 
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
		if (elems.size() + 1 == limit)
			break;
    }
	if (ss.tellg() > 0)
		elems.push_back(ss.str().substr(ss.tellg(), s.size() - ss.tellg()));
    return elems;
}


StringList split(const string& s, char delim, int limit) 
{
    StringList elems;
    return split(s, (char)delim, elems, limit);
}


string escape(const string& str) 
{
	string what = str;
	const char escape_chars[] = { '&', '<', '>', '\'', '"' };
	const string escape_seqs[] = { "amp;", "lt;", "gt;", "apos;", "quot;" };
	const unsigned escape_size = 5;
	for (size_t val, i = 0; i < what.length(); ++i) {
		for (val = 0; val < escape_size; ++val) {
			if (what[i] == escape_chars[val]) {
				what[i] = '&';
				what.insert(i+1, escape_seqs[val]);
				i += escape_seqs[val].length();
				break;
			}
		}
	}
	return what;
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


bool compareVersion(const string& l, const string& r) 
{
	if (l.empty())
		return false;
	if (r.empty())
		return true;

	bool isEqual = true;
	StringList lnums = split(l, ".");
	StringList rnums = split(r, ".");
	for (unsigned i = 0; i < lnums.size(); i++) {			
		if (rnums.size() < i + 1)
			break;		
		unsigned ln = atoi(lnums[i]);
		unsigned rn = atoi(rnums[i]);
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
	StringList params = Util::split(node, delim);
	StringList xparams = Util::split(xnode, delim);
	return matchNodes(params, xparams);
}


bool matchNodes(const StringList& params, const StringList& xparams)
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


} // namespace Util
} // namespace Sourcey