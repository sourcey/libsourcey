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


#ifndef SOURCEY_Crypto_Cipher_H
#define SOURCEY_Crypto_Cipher_H


#include "Sourcey/Crypto/Crypto.h"
#include "Sourcey/Random.h"
#include "Sourcey/Types.h"

#include <openssl/evp.h>


namespace scy {
namespace crypto {
	

class Cipher
	/// Provides symmetric algorithms for encryption and decryption. 
	/// The algorithms that are available depend on the particular 
	/// version of OpenSSL that is installed.
{
public:	
	Cipher(const std::string& name);
		// Creates a new Cipher object. Auto initializes the key
		// and initialization vector with random bytes.
	
	Cipher(const std::string& name, const ByteVec& key, const ByteVec& iv);
		// Creates a new Cipher object, using the given cipher name, 
		// key and initialization vector.
	
	Cipher(const std::string& name, const std::string& passphrase,
		const std::string& salt, int iterationCount);
		// Creates a new Cipher object, using the given cipher name,
		// passphrase, salt value and iteration count.
	
	~Cipher();
		// Destroys the Cipher.
	
	void initEncryptor();
		// Initializes the Cipher for encryption.
	
	void initDecryptor();
		// Initializes the Cipher for decryption.
	
	int update(
		const unsigned char* input, int inputLength,
		unsigned char* output, int outputLength);
		// Encrypts data in a streaming fashion. 
		// Hand consecutive blocks of data to the update method in order to encrypt it. 
		// Returns the encrypted data chunk. When done, the output of final() should be
		// additionally added to the result.

	template <typename I, typename O>
	int update(const I &input, O &output)
		// Alias for update() which accepts a range of buffer types.
	{
		internal::Raw<const unsigned char*> in(input);
		internal::Raw<unsigned char*> out(output);
		return update(in.ptr, in.len, out.ptr, out.len);
	}
		
	int final(unsigned char* output, int length);
		// Returns the remaining data held in the cipher object. 
		// Further calls to update() or final() will return garbage.	
		//
		// See EVP_CipherFinal_ex for further information.
	
	template <typename O>
	int final(O &output)
		// Alias for final() which accepts a range of buffer types.
	{
		internal::Raw<unsigned char*> out(output);
		return final(out.ptr, out.len);
	}

	enum Encoding
		// Transport encoding to use for encrypt() and decrypt().
	{
		Binary      = 0x00, // Plain binary output
		Base64      = 0x01, // Base64-encoded output
		BinHex      = 0x02, // BinHex-encoded output
		Base64_NoLF = 0x81, // Base64-encoded output, no linefeeds
		BinHex_NoLF = 0x82, // BinHex-encoded output, no linefeeds		
	};
	
	int encrypt(
		const unsigned char* inbuf, std::size_t inlen, 
		unsigned char* outbuf, std::size_t outlen, 
		Encoding encoding = Binary);
		// Encrypts a buffer and encode it using the given encoding.
		// This method performs the encryption, and calls final() internally.
	
	template <typename I, typename O>
	int encrypt(const I& input, O& output, Encoding encoding = Binary)
		// Alias for encrypt() which accepts a range of buffer types.
	{
		internal::Raw<const unsigned char*> in(input);
		internal::Raw<unsigned char*> out(output);
		return encrypt(in.ptr, in.len, out.ptr, out.len, encoding);
	}

	virtual std::string encryptString(const std::string& str, Encoding encoding = Binary);
		// Encrypts a string and encodes it using the given encoding.
	
	virtual std::string decryptString(const std::string& str, Encoding encoding = Binary);
		// Decrypts a string that is encoded with the given encoding.
	
	virtual void encryptStream(std::istream& source, std::ostream& sink, Encoding encoding = Binary);
		// Encrypts an input stream and encodes it using the given encoding.
	
	virtual void decryptStream(std::istream& source, std::ostream& sink, Encoding encoding = Binary);	
		// Decrypts an input stream that is encoded with the given encoding.	
	
	template<typename T>
	void setKey(const T& key) 
		// Sets the key for the Cipher.
	{
		assert(int(key.size()) == keySize());
		_key.clear();
		for (typename T::const_iterator it = key.begin(); it != key.end(); ++it)
			_key.push_back(static_cast<unsigned char>(*it));
	}
	
	template<typename T>
	void setIV(const T& iv) 
		// Sets the initialization vector (IV) for the Cipher.
	{
		assert(int(iv.size()) == ivSize());
		_iv.clear();
		for (typename T::const_iterator it = iv.begin(); it != iv.end(); ++it)
			_iv.push_back(static_cast<unsigned char>(*it));
	}

	int setPadding(int padding);
		// Enables or disables padding. By default encryption operations are padded using 
		// standard block padding and the padding is checked and removed when decrypting. 
		// If the pad parameter is zero then no padding is performed, the total amount of
		// data encrypted or decrypted must then be a multiple of the block size or an
		// error will occur.
		//
		// See EVP_CIPHER_CTX_set_padding for further information.
	
	const ByteVec& getKey() const;
		// Returns the key for the Cipher.
	
	const ByteVec& getIV() const;
		// Returns the initialization vector (IV) for the Cipher.
	
	const std::string& name() const;
		// Returns the name of the Cipher.
	
	int blockSize() const;
		// Returns the block size of the Cipher.
	
	int keySize() const;
		// Returns the key size of the Cipher.
	
	int ivSize() const;
		// Returns the IV size of the Cipher.
	
	const EVP_CIPHER* cipher();
		// Returns the cipher object.
		
protected:
	Cipher();
	Cipher(const Cipher&);
	Cipher& operator = (const Cipher&);
	
	void generateKey(const std::string& passphrase,
		const std::string& salt,
		int iterationCount);
		// Generates and sets the key and IV from a password and optional salt string.
	
	void setRandomKey();
		// Generates and sets key from random data.

	void setRandomIV();
		// Generates and sets IV from random data.

	void initialize(bool encrypt);
		// Initializes the Cipher using the given direction.
	
	bool              _initialized;
	bool              _encrypt;
	const EVP_CIPHER* _cipher;
	std::string       _name;
	EVP_CIPHER_CTX    _ctx;
	ByteVec           _key;
	ByteVec           _iv;
};	


template<typename K, typename I>
std::string encryptString(const std::string& algorithm, const std::string& data, const K& key, const I& iv, Cipher::Encoding encoding = Cipher::Binary)
{
	Cipher ciph(algorithm);
	
	if (key.size())
		ciph.setKey(key);
	if (iv.size())
		ciph.setIV(iv);
	
	return ciph.encryptString(data, encoding);
}


template<typename K, typename I>
std::string decryptString(const std::string& algorithm, const std::string& data, const K& key, const I& iv, Cipher::Encoding encoding = Cipher::Binary)
{
	Cipher ciph(algorithm);
	
	if (key.size())
		ciph.setKey(key);
	if (iv.size())
		ciph.setIV(iv);
	
	return ciph.decryptString(data, encoding);
}


} } // namespace scy::crypto


#endif // SOURCEY_Crypto_Cipher_H




	
/*
	struct CipherOptions
	{
		Encoding encoding;

		CipherOptions() {
			encoding = Cipher::Binary;
		}
	};
	//void generateKey();	
	//void setKey(const ByteVec& key);
	//void setIV(const ByteVec& iv);

*/
