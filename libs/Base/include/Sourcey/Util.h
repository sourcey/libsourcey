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


#ifndef SOURCEY_Util_H
#define SOURCEY_Util_H


#include "Sourcey/Base.h"

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <deque>
	

#define MINVALUE(X,Y) ((X)<(Y)) ? (X) : (Y)
#define MAXVALUE(X,Y) ((X)>(Y)) ? (X) : (Y)

#define ARRAY_SIZE(x) (static_cast<int>((sizeof(x)/sizeof(x[0]))))


namespace Sourcey {
namespace Util {


UInt32 getTime();

std::string itoa(unsigned i);
unsigned atoi(const std::string& str);
std::string dtoa(double d);
double atod(const std::string& str);

template<typename T>
std::string tostr(const T &t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template<typename T>
T strto(const std::string& s) {
    std::istringstream iss(s);
    T t;
    iss >> t;
    return t;
}

bool isNumber(const std::string& str);

void removeSpecialCharacters(std::string& str, bool allowSpaces = false);
void replaceSpecialCharacters(std::string& str, char with = '_', bool allowSpaces = false);
void underscore(std::string& str);

int icompare(const char* s1, const char* s2);
int icompare(const char* s1, const std::string& s2);

bool endsWith(const std::string& str, const std::string& suffix);

std::string getPID(void* ptr);

StringList &split(const std::string& s, const std::string& delim, StringList &elems, int limit = -1);
StringList split(const std::string& s, const std::string& delim, int limit = -1);

StringList &split(const std::string& s, char delim, StringList &elems, int limit = -1);
StringList split(const std::string& s, char delim, int limit = -1);

std::string trim(const std::string& str);
	///
	/// Removes whitespace from beginning and end of Strings.
	/// @param s	String from which to remove whitespace (or rather copy and remove whitespace from)
	///

std::string escape(const std::string& str);
	///
	/// Does some fancy escaping. (& --> &amp;amp;, etc).
	/// @param str A string to escape.
	///

bool compareVersion(const std::string& l, const std::string& r);
	/// This function compares two version strings ie. 3.7.8.0 > 3.2.1.0
	/// If L is greater than R the function returns true.
	/// If L is equal or less than R the function returns false.

bool matchNodes(const std::string& node, const std::string& xnode, const std::string& delim = "\r\n");
bool matchNodes(const StringList& params, const StringList& xparams);


template<typename T>
inline void ClearList(std::list<T*>& L)
	//
	/// Delete all elements from a list of pointers.
	/// @param L List of pointers to delete.
	//
{	
	typename std::list<T*>::iterator it = L.begin();
	while (it != L.end())
	{
		delete *it;
		it = L.erase(it);
	}
}

template<typename T>
inline void ClearDeque(std::deque<T*>& D)
	///
	/// Delete all elements from a list of pointers.
	/// @param L List of pointers to delete.
	///
{
	typename std::deque<T*>::iterator it = D.begin();
	while (it != D.end())
	{
		delete *it;
		it = D.erase(it);
	}
}

template<typename T>
inline void ClearVector(std::vector<T*>& V)
	///
	/// Delete all elements from a vector of pointers.
	/// @param L Vector of pointers to delete.
	///
{
	typename std::vector<T*>::iterator it = V.begin();
	while (it != V.end())
	{
		delete *it;
		it = V.erase(it);
	}
}

template<typename Key, typename T>
inline void ClearMap(std::map<Key, T*>& M)
	///
	/// Delete all associated values from a map (not the key elements).
	/// @param M Map of pointer values to delete.
	///
{
	typename std::map<Key, T*>::iterator it = M.begin();
	typename std::map<Key, T*>::iterator it2;
	while (it != M.end())
	{
		it2 = it++;
		delete (*it2).second;
		M.erase(it2);
	}
}

template<typename Key, typename T>
inline void ClearMap(std::map<const Key, T*>& M)
	///
	/// Delete all associated values from a map (not the key elements).
	/// Const key type version.
	/// @param M Map of pointer values to delete.
	///
{
	typename std::map<const Key, T*>::iterator it = M.begin();
	typename std::map<const Key, T*>::iterator it2;
	while (it != M.end())
	{
		it2 = it++;
		delete (*it2).second;
		M.erase(it2);
	}
}


//
// Windows Helpers
//

#if WIN32
enum WindowsMajorVersions {
	kWindows2000 = 5,
	kWindowsVista = 6,
};

bool getOsVersion(int* major, int* minor, int* build);

inline bool isWindowsVistaOrLater() {
	int major;
	return (getOsVersion(&major, NULL, NULL) && major >= kWindowsVista);
}

inline bool isWindowsXpOrLater() {
	int major, minor;
	return (getOsVersion(&major, &minor, NULL) &&
		(major >= kWindowsVista ||
		(major == kWindows2000 && minor >= 1)));
}
#endif


} // namespace Util
} // namespace Sourcey


#endif // SOURCEY_Util_H