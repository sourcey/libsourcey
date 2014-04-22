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


#ifndef SCY_Crypto_Hash_H
#define SCY_Crypto_Hash_H


#include "scy/crypto/crypto.h"
#include "scy/types.h"
#include "scy/hex.h"

#include <openssl/evp.h>


namespace scy {
namespace crypto {


class Hash
{
public:
	Hash(const std::string& algorithm);
	~Hash();
		// Re-initializes the digest structure.
	
	void update(char data);
	void update(const std::string& data);
	void update(const void* data, unsigned length);
		// Hash the given data.
		// This function may (and normally will) be called
		// many times for large blocks of data.
	
	const ByteVec& digest();
		// Finish up the digest operation and return the result.
	
	std::string digestStr();
		// Finish up the digest operation and return the result as a string.

	void reset();
		// Resets the engine and digest state ready for the next computation.

	const std::string& algorithm(void) const;
		// Returns the hash algorithm being used.

protected:
	Hash& operator=(Hash const&);

	EVP_MD_CTX	_ctx;
	const EVP_MD*	_md;
	crypto::ByteVec	_digest;
	std::string	_algorithm;
};


inline std::string computeHash(const std::string& algorithm, const std::string& data)
{
	Hash engine(algorithm);
	engine.update(data);
	return hex::encode(engine.digest());
}
	

} } // namespace scy::crypto


#endif // SCY_Crypto_Hash_H