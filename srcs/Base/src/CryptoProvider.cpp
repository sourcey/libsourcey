//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#include "Sourcey/CryptoProvider.h"
#include "Sourcey/Logger.h"

#include "Poco/DigestEngine.h"
#include "Poco/SHA1Engine.h"
#include "Poco/MD5Engine.h"	
#include "Poco/Random.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"

#include <iostream>
#include <sstream>


using namespace std;
using namespace Poco;
using namespace Poco::Crypto;


namespace Sourcey {
namespace CryptoProvider {


// Generates a random binary key
string generateRandomBinaryKey(int size, bool doBase64)
{
	string res;
	ostringstream ostr;
	try
	{
		Random rnd;
		rnd.seed();
		for (int i = 0; i < size; ++i)
			res.push_back(rnd.nextChar());

		if (doBase64) {
			Base64Encoder encoder(ostr);
			encoder << res;
			encoder.close();
			res = ostr.str();
		}
	} 
	catch (...) 
	{
		Log("error") << "CryptoProvider: Unknown Error" << endl;
	}
	return res;
}


// Generates a random key
string generateRandomKey(int size)
{
	string res;
	try
	{	
		res = hash("md5", generateRandomBinaryKey(size)).substr(0, size);
	}
	catch (...)
	{
		Log("error") << "CryptoProvider: Unknown Error" << endl;
	}
	return res;
}


UInt64 generateRandomNumber(int size)
{
	UInt64 res = 0;
	stringstream strm;
	string str;
	try
	{	
		Random rnd;
		rnd.seed();
		while (strm.str().length() < size)
		{
			strm << rnd.next();
		}
		str = strm.str().substr(0, size);
		res = atoi(str.data());
	}
	catch (...)
	{
		Log("error") << "CryptoProvider: Unknown Error" << endl;
	}
	return res;
}


string hash(const string& algorithm, const string& data)
{
	string hash;
	if (algorithm == "md5") {
		MD5Engine engine;
		engine.update(data);
		hash = DigestEngine::digestToHex(engine.digest());
	} else if (algorithm == "sha1") {
		SHA1Engine engine;
		engine.update(data);
		hash = DigestEngine::digestToHex(engine.digest());
	}
	return hash;
}


string encrypt(const string& algorithm, const string& data, const string& key, const string& iv, bool doBase64, bool doPrependIV)
{
	CipherKey cipherKey(algorithm);
	string out;
	
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
		out = pCipher1->encryptString(data, Cipher::ENC_NONE);
		if (doBase64) {
			ostringstream str;
			Base64Encoder encoder(str);
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
			doBase64 ? Cipher::ENC_BASE64 : Cipher::ENC_NONE
		);
	}

	//Logger::send("debug") << "Encrypt: " << algorithm << ": Output: " << out << endl;

	return out;
}


string decrypt(const string& algorithm, const string& data, const string& key, const string& iv, bool isBase64, bool isPrependIV)
{
	CipherKey cipherKey(algorithm);
	string ivFinal = iv;
	istringstream source(data);
	ostringstream sink;
	string out;
	
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
			istringstream b64strm(data);
			Base64Decoder decoder(b64strm);
			while (decoder.rdbuf()->sgetc() != EOF && 
				ivFinal.size() < cipherKey.ivSize()) {
				char ch = decoder.rdbuf()->sbumpc();
				ivFinal.push_back(ch);
			}
		}
		else
		{	
			string::const_iterator it = data.begin();
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
	pCipher1->decrypt(source, sink, isBase64 ? Cipher::ENC_BASE64 : Cipher::ENC_NONE);
	
	// If the IV was prepended remove cipherKey.ivSize() characters from the front of the output.
	out = sink.str();
	if (isPrependIV)
		out.erase(0, cipherKey.ivSize());

	/*
	Logger::send("debug") << "Decrypt:\n" 
		<< "Algorithm: " << algorithm << "\n" 
		<< "Output: " << sink.str() << "\n" 
		<< "Data: " << data << "\n" 
		<< "IV Size: " << cipherKey.ivSize() << "\n" 
		<< endl;
		*/

	return out;

}


#ifdef WIN32
//hack for name collision of OCSP_RESPONSE and wincrypt.h in latest openssl release 0.9.8h
//http://www.google.com/search?q=OCSP%5fRESPONSE+wincrypt%2eh
//continue to watch this issue for a real fix.
#undef OCSP_RESPONSE
#endif
#include <openssl/hmac.h>
#include <assert.h>

std::string computeHMAC(const std::string& input, const std::string& key) {	
	/*
	Log("debug") << "CryptoProvider: Computing HMAC:\n"
		<< "\tInput: " << input << "\n"
		<< "\tInput Length: " << input.length() << "\n"
		<< "\tKey: " << key << "\n"
		<< "\tKey Length: " << key.length() << "\n"
		<< std::endl;
		*/
	unsigned int resultSize=0;
	char* buffer = new char[20];
	HMAC(EVP_sha1(), 
		key.data(), key.length(), 
        reinterpret_cast<const unsigned char*>(input.data()), input.length(), 
        reinterpret_cast<unsigned char*>(buffer), &resultSize);
	assert(resultSize == 20);
	std::string hmac(buffer, resultSize);
	delete buffer;
	return hmac;
}


void encryptFile(const string& password, const string& inputFileName, 
                 const string& outputFileName)
{
	assert(false);
}


void decryptFile(const string& password, const string& inputFileName, 
                 const string& outputFileName)
{
	assert(false);
}


} // namespace CryptoProvider
} // namespace Sourcey