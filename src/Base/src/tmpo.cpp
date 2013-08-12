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


/*
#include "Sourcey/Crypto.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Sourcey/Random.h"
#include "Sourcey/Base64.h"

#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/DigestEngine.h"
#include "Poco/SHA1Engine.h"
#include "Poco/MD5Engine.h"	
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
using namespace Poco::Crypto;

#include <iostream>
#include <sstream>
#include <assert.h>


using namespace std;
*/


namespace scy {
	/*
namespace crypto {


std::string hash(const std::string& algorithm, const std::string& data)
{
	std::string hash;
	if (algorithm == "md5") {
		Poco::MD5Engine engine;
		engine.update(data);
		hash = Poco::crypto::digestToHex(engine.digest());
	} 
	else if (algorithm == "sha1") {
		Poco::SHA1Engine engine;
		engine.update(data);
		hash = Poco::crypto::digestToHex(engine.digest());
	}
	else assert(0 && "not implemented");
	return hash;
}
	*/


	/*
std::string encrypt(const std::string& algorithm, const std::string& data, const std::string& key, const std::string& iv, bool doBase64, bool doPrependIV)
{
	std::string out;
	CipherKey cipherKey(algorithm);
	
	CipherKey::ByteVec keyVec;	
	keyVec.reserve(key.length());
	for (int i = 0; i < key.length(); ++i)
		keyVec.push_back(static_cast<unsigned char>(key[i]));
	cipherKey.setKey(keyVec);

	if (iv.size())
	{
		CipherKey::ByteVec ivVec;	
		ivVec.reserve(iv.length());
		for (int i = 0; i < iv.length(); ++i)
			ivVec.push_back(static_cast<unsigned char>(iv[i]));
		cipherKey.setIV(ivVec);
	}

	Cipher::Ptr pCipher1 = CipherFactory::defaultFactory().createCipher(cipherKey);

	if (doPrependIV)
	{
		out = pCipher1->encryptString(data, Cipher::Binary);
		if (doBase64) {
			std::ostringstream str;
			Poco::Base64Encoder encoder(str);
			encoder << iv;
			encoder << out;
			encoder.close();
			out = str.str();
		}
	}	
	else
	{
		out = pCipher1->encryptString(
			data, 
			doBase64 ? Cipher::Base64 : Cipher::Binary
		);
	}
	return out;
}

	*/
	
	/*
std::string decrypt(const std::string& algorithm, const std::string& data, const std::string& key, const std::string& iv, bool isBase64, bool isPrependIV)
{
	std::string out;
	CipherKey cipherKey(algorithm);
	std::string ivFinal = iv;
	std::istringstream source(data);
	std::ostringstream sink;
	
	CipherKey::ByteVec keyVec;	
	keyVec.reserve(key.length());
	for (int i = 0; i < key.length(); ++i)
		keyVec.push_back(static_cast<unsigned char>(key[i]));
	cipherKey.setKey(keyVec);

	if (isPrependIV && iv.empty())
	{
		ivFinal = "";
		if (isBase64)
		{
			std::istringstream b64strm(data);
			Poco::Base64Decoder decoder(b64strm);
			while (decoder.rdbuf()->sgetc() != EOF && 
				ivFinal.size() < cipherKey.ivSize()) {
				char ch = decoder.rdbuf()->sbumpc();
				ivFinal.push_back(ch);
			}
		}
		else
		{	
			std::string::const_iterator it = data.begin();
			while (it != data.end() && 
				ivFinal.size() < cipherKey.ivSize()) {
				ivFinal.push_back(*it);
				it++;        
			}
		}
	}

	CipherKey::ByteVec ivVec;	
	ivVec.reserve(ivFinal.length());
	for (int i = 0; i < ivFinal.length(); ++i)
		ivVec.push_back(static_cast<unsigned char>(ivFinal[i]));
	cipherKey.setIV(ivVec);	

	Cipher::Ptr pCipher1 = CipherFactory::defaultFactory().createCipher(cipherKey);
	pCipher1->decrypt(source, sink, isBase64 ? Cipher::Base64 : Cipher::Binary);
	
	// If the IV was prepended remove cipherKey.ivSize() characters from the front of the output.
	out = sink.str();
	if (isPrependIV)
		out.erase(0, cipherKey.ivSize());
		*/

	/*
	Logger::send("debug") << "Decrypt:\n" 
		<< "Algorithm: " << algorithm << "\n" 
		<< "Output: " << sink.str() << "\n" 
		<< "Data: " << data << "\n" 
		<< "IV Size: " << cipherKey.ivSize() << "\n" 
		<< endl;

	return out;

}


std::string computeHMAC(const std::string& input, const std::string& key) 
{	
	assert(0 && "use crypto module");
	return "";
}


void encryptFile(const std::string& password, const std::string& inputFileName, 
                 const std::string& outputFileName)
{
	assert(0 && "not implemented");
}


void decryptFile(const std::string& password, const std::string& inputFileName, 
                 const std::string& outputFileName)
{
	assert(0 && "not implemented");
}


} // namespace crypto
		*/
} // namespace scy