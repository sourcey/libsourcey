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


#include "Sourcey/Crypto/Hash.h"
#include "Sourcey/Exception.h"
#include <assert.h>
#include <iostream>


using namespace std; 


namespace scy {
namespace crypto {
	

Hash::Hash(const std::string& algorithm) :	
	_algorithm(algorithm)
{
	crypto::initializeEngine();

	_md = EVP_get_digestbyname(algorithm.data());
	if (!_md)
		throw std::runtime_error("Algorithm not supported: " + algorithm);

	EVP_DigestInit(&_ctx, _md);
}


Hash::~Hash() 
{
	crypto::uninitializeEngine();

	EVP_MD_CTX_cleanup(&_ctx);
}


void Hash::reset()
{
	internal::api(EVP_MD_CTX_cleanup(&_ctx));
	internal::api(EVP_DigestInit(&_ctx, _md));
	_digest.clear();
}


void Hash::update(const void* data, unsigned length) 
{
	internal::api(EVP_DigestUpdate(&_ctx, data, length));
}


void Hash::update(const std::string& data)
{
	update(data.c_str(), data.length());
}


void Hash::update(char data)
{
	update(&data, 1);
}


const ByteVec& Hash::digest() 
{
	// Compute the first time
	if (_digest.size() == 0) {
		_digest.resize(EVP_MAX_MD_SIZE); // TODO: Get actual algorithm size
		unsigned int len = 0;	
		internal::api(EVP_DigestFinal(&_ctx, &_digest[0], &len));
		_digest.resize(len);
	}
	return _digest;
}


const std::string& Hash::algorithm(void) const
{
	return _algorithm;
}


} } // namespace scy::crypto