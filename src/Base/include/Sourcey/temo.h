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


#ifndef SOURCEY_CryptoProvider_H
#define SOURCEY_CryptoProvider_H


#include "Sourcey/Types.h"
#include <string>


namespace scy {
namespace crypto {


/// Wrapper for some Poco::Crypto objects which will soon become obsolete.
	/*
std::string hash(const std::string& algorithm, const std::string& data);
std::string encrypt(const std::string& algorithm, const std::string& data, const std::string& key, const std::string& iv = "", bool doBase64 = false, bool doPrependIV = false);
std::string decrypt(const std::string& algorithm, const std::string& data, const std::string& key, const std::string& iv = "", bool isBase64 = false, bool isPrependIV = false);
void encryptFile(const std::string& password, const std::string& inputFileName, const std::string& outputFileName);
void decryptFile(const std::string& password, const std::string& inputFileName, const std::string& outputFileName);
std::string computeHMAC(const std::string& input, const std::string& key);
*/


} // namespace crypto
} // namespace scy


#endif // SOURCEY_CryptoProvider_H