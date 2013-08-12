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
#include "Sourcey/Exception.h"

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <map>


namespace scy {
namespace util {


//
/// String utilities
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

std::string format(const char* fmt, ...);
	// Printf style string formatting for POD types.

std::string randomBinaryString(int size, bool doBase64 = false);
	// Generates a random (optionally base64 encoded) binary key

std::string randomString(int size);
	// Generates a random string

UInt64 randomNumber(int size);
	// Generates a random number

int icompare(const std::string& str1, const std::string& str2);
	// Case insensitive string comparison.

std::string replace(const std::string& str, const std::string& from, const std::string& to, std::string::size_type start = 0);
std::string& replaceInPlace(std::string& str, const std::string& from, const std::string& to, std::string::size_type start = 0);
	// Functions for replacing string content.

std::string toLower(const std::string& str);
void toLowerInPlace(std::string& str);
	// Transforms the string to lowercase.

std::string toUpper(const std::string& str);
void toUpperInPlace(std::string& str);
	// Transforms the string to uppercase.

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

std::string dumpbin(const char* data, size_t len);
	// Dumps the binary representation of the 
	// given buffer to the output string.

StringVec &split(const std::string& str, const std::string& delim, StringVec &elems, int limit = -1);
StringVec split(const std::string& str, const std::string& delim, int limit = -1);
	// Splits the given string at the delimiter string

StringVec &split(const std::string& str, char delim, StringVec &elems, int limit = -1);
StringVec split(const std::string& str, char delim, int limit = -1);
	// Splits the given string at the delimiter character

void trim(std::string& str);
	// Removes whitespace from beginning and end of the given string.

bool compareVersion(const std::string& l, const std::string& r);
	// Compares two version strings ie. 3.7.8.0 > 3.2.1.0
	// If L is greater than R the function returns true.
	// If L is equal or less than R the function returns false.

bool matchNodes(const std::string& node, const std::string& xnode, const std::string& delim = "\r\n");
bool matchNodes(const StringVec& params, const StringVec& xparams);
	// Matches two node lists against each other.

std::string memAddress(const void* ptr);
	// Returns the object PID as a string.


//
/// Stream copiers
//

std::streamsize copyStreamUnbuffered(std::istream& istr, std::ostream& ostr);
std::streamsize copyStream(std::istream& istr, std::ostream& ostr, std::size_t bufferSize = 8192);
std::streamsize copyToString(std::istream& istr, std::string& str, std::size_t bufferSize = 8192);


//
/// Integer conversions
//

inline double intToDouble(Int64 v) {
	if (v+v > 0xFFEULL<<52)
		return 0;
	return ldexp((double)((v&((1LL<<52)-1)) + (1LL<<52)) * (v>>63|1), (int)(v>>52&0x7FF)-1075);
}


inline float intToFloat(Int32 v) {
	if (v+v > 0xFF000000U)
		return 0;
	return ldexp((float)((v&0x7FFFFF) + (1<<23)) * (v>>31|1), (int)(v>>23&0xFF)-150);
}


inline Int64 doubleToInt(double d) {
	int e;
	if     ( !d) return 0;
	else if(d-d) return 0x7FF0000000000000LL + ((Int64)(d<0)<<63) + (d!=d);
	d = frexp(d, &e);
	return (Int64)(d<0)<<63 | (e+1022LL)<<52 | (Int64)((fabs(d)-0.5)*(1LL<<53));
}


//
/// Container helpers
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

template<class Deleter, typename Key, typename Val>
inline void clearMap(std::map<Key, Val*>& M)
	// Delete all associated values from a map (not the key elements)
	// using the given deleter method.
	// @param M Map of pointer values to delete.
{
	typename std::map<Key, Val*>::iterator it = M.begin();
	typename std::map<Key, Val*>::iterator it2;
	while (it != M.end()) {
		it2 = it++;
		Deleter::func((*it2).second);
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
/// System Utils
//

//void pause();
	// Pause the current thread until a key is pressed.