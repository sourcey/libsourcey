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


#ifndef SOURCEY_Crypto_Crypto_H
#define SOURCEY_Crypto_Crypto_H


#include "Sourcey/Base64.h"
#include <vector>


namespace scy {
namespace crypto {

	
void initializeEngine();
	// Initialize the Crypto library, as well as the underlying OpenSSL
	// libraries.
	//
	// OpenSSL must be initialized before using any classes from the
	// Crypto library. OpenSSL will be initialized automatically
	// through OpenSSL instances held by various Crypto classes
	// (Cipher, Hash, X509Certificate), however it is recommended to
	// call initializeEngine() in any case at application startup.
	//
	// The Crypto library can be called multiple times; however,
	// for every call to initializeEngine(), a matching call to 
	// uninitializeEngine() must be performed.
	

void uninitializeEngine();
	// Uninitializes the Crypto library.


typedef std::vector<unsigned char> ByteVec;	
	// Generic storage container for storing cryptographic binary data.


namespace internal {
	
	void api(int ret, const char* error = nullptr);
		// Check return values from OpenSSL and throw an exception if it failed.
	
	void throwError();
		// Throws the last OpenSSL error.
	
	template<typename T> struct Raw
		// Allows template functions to accept a wide range of buffer types.
		// See constructor definitions below for all compatible types.
		// The class uses const_cast for maximum flexibility, so use with care.
		// Also ensure that std::string is contiguous on your platform
		// before using the std::string constructors (C++11 guarantees it).
	{
		T ptr;
		std::size_t len;
	
		Raw(T ptr, std::size_t len) {
			ptr = ptr;
			len = len;
		}
	
		Raw(const char* ptr, std::size_t len) {
			ptr = reinterpret_cast<T>(const_cast<char*>(ptr));
			len = len;
		}

		Raw(std::string& str) {
			ptr = reinterpret_cast<T>(&str[0]);
			len = str.length();
		}

		Raw(const std::string& str) {
			ptr = reinterpret_cast<T>(const_cast<char*>(&str[0]));
			len = str.length();
		}

		Raw(const std::vector<char>& vec) {
			ptr = reinterpret_cast<T>(const_cast<char*>(&vec[0]));
			len = vec.size();
		}

		Raw(const ByteVec& vec) {
			ptr = reinterpret_cast<T>(const_cast<unsigned char*>(&vec[0]));
			len = vec.size();
		}
	};

} // namespace internal
	

} } // namespace scy::crypto


#endif // SOURCEY_Crypto_Crypto_H