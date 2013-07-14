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


#ifndef SOURCEY_Util_H
#define SOURCEY_Util_H


#include "Sourcey/Types.h"
//#include "Sourcey/Memory.h"

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <map>


namespace scy {
namespace util {


//
/// String Utils
//
	
template<typename T>
std::string toString(const T &t) 
	/// Converts any interger at string
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template<typename T>
T fromString(const std::string& s) 
	/// Converts any string to interger.
	/// Ensure the interger type has sufficient storage capacity.
{
    std::istringstream iss(s);
    T x;
	if (!(iss >> x))
		return 0;
    return x;
}

UInt64 getTime();
	/// Returns the current time

std::string getPID(const void* ptr);
	/// Returns the object PID and a string

void removeSpecialCharacters(std::string& str, bool allowSpaces = false);
void replaceSpecialCharacters(std::string& str, char with = '_', bool allowSpaces = false);
void underscore(std::string& str);

std::string dumpbin(const char* data, size_t len);

int icompare(const char* s1, const char* s2);
int icompare(const char* s1, const std::string& s2);

bool isNumber(const std::string& str);
	/// Checks if the string is a number

bool endsWith(const std::string& str, const std::string& suffix);

StringVec &split(const std::string& s, const std::string& delim, StringVec &elems, int limit = -1);
StringVec split(const std::string& s, const std::string& delim, int limit = -1);

StringVec &split(const std::string& s, char delim, StringVec &elems, int limit = -1);
StringVec split(const std::string& s, char delim, int limit = -1);

void trim(std::string& str);
	/// Removes whitespace from beginning and end of Strings.
	/// @param str String from which to remove whitespace

void escape(std::string& str);
	/// Does some fancy escaping. (& --> &amp;amp;, etc).
	/// @param str A string to escape.

bool compareVersion(const std::string& l, const std::string& r);
	/// This function compares two version strings ie. 3.7.8.0 > 3.2.1.0
	/// If L is greater than R the function returns true.
	/// If L is equal or less than R the function returns false.

bool matchNodes(const std::string& node, const std::string& xnode, const std::string& delim = "\r\n");
bool matchNodes(const StringVec& params, const StringVec& xparams);


//
/// System Tools
//

void pause();
	/// Pause the current thread until a key is pressed.


/// Windows Helpers

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


//
/// Container methods
//

template<typename Val>
inline void clearList(std::list<Val*>& L)
	/// Delete all elements from a list of pointers.
	/// @param L List of pointers to delete.
{	
	typename std::list<Val*>::iterator it = L.begin();
	while (it != L.end()) {
		delete *it;
		//Deleter::func(*it);
		it = L.erase(it);
	}
}

template<typename Val>
inline void clearDeque(std::deque<Val*>& D)
	/// Delete all elements from a list of pointers.
	/// @param D List of pointers to delete.
{
	typename std::deque<Val*>::iterator it = D.begin();
	while (it != D.end()) {
		delete *it;
		//Deleter::func(*it);
		it = D.erase(it);
	}
}

template<typename Val>
inline void clearVector(std::vector<Val*>& V)
	/// Delete all elements from a vector of pointers.
	/// @param V Vector of pointers to delete.
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
	/// Delete all associated values from a map (not the key elements).
	/// @param M Map of pointer values to delete.
{
	typename std::map<Key, Val*>::iterator it = M.begin();
	typename std::map<Key, Val*>::iterator it2;
	while (it != M.end()) {
		it2 = it++;
		delete (*it2).second;
		//Deleter::func((*it2).second);
		M.erase(it2);
	}
}

template<class Deleter, typename Key, typename Val>
inline void clearMap(std::map<Key, Val*>& M)
	/// Delete all associated values from a map (not the key elements)
	/// using the given deleter method.
	/// @param M Map of pointer values to delete.
{
	typename std::map<Key, Val*>::iterator it = M.begin();
	typename std::map<Key, Val*>::iterator it2;
	while (it != M.end()) {
		it2 = it++;
		//delete (*it2).second;
		Deleter::func((*it2).second);
		M.erase(it2);
	}
}

template<typename Key, typename Val>
inline void clearMap(std::map<const Key, Val*>& M)
	/// Delete all associated values from a map (not the key elements).
	/// Const key type version.
	/// @param M Map of pointer values to delete.
{
	typename std::map<const Key, Val*>::iterator it = M.begin();
	typename std::map<const Key, Val*>::iterator it2;
	while (it != M.end()) {
		it2 = it++;
		delete (*it2).second;
		//Deleter::func((*it2).second);
		M.erase(it2);
	}
}



} // namespace util
} // namespace scy


#endif // SOURCEY_Util_H