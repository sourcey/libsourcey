///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup crypto
/// @{


#include "scy/crypto/hash.h"
#include "scy/error.h"
#include <assert.h>
#include <iostream>


using std::endl;


namespace scy {
namespace crypto {


Hash::Hash(const std::string& algorithm)
    : _algorithm(algorithm)
    //, _ctx(EVP_MD_CTX_new())
{
    crypto::initializeEngine();

    _md = EVP_get_digestbyname(algorithm.data());
    if (!_md)
        throw std::runtime_error("Algorithm not supported: " + algorithm);

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_DigestInit(&_ctx, _md);
#else
    EVP_DigestInit(_ctxPtr, _md);
#endif
}


Hash::~Hash()
{
    crypto::uninitializeEngine();

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_MD_CTX_cleanup(&_ctx);
#else
    EVP_MD_CTX_free(_ctxPtr);
#endif
    //EVP_MD_CTX_free(_ctx);
}


void Hash::reset()
{
    //EVP_MD_CTX_free(_ctx);
    //_ctx = EVP_MD_CTX_new();
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    internal::api(EVP_MD_CTX_cleanup(&_ctx));
    internal::api(EVP_DigestInit(&_ctx, _md));
#else
    internal::api(EVP_MD_CTX_cleanup(_ctxPtr));
    internal::api(EVP_DigestInit(_ctxPtr, _md));
#endif

    _digest.clear();
}


void Hash::update(const void* data, size_t length)
{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    internal::api(EVP_DigestUpdate(&_ctx, data, length));
#else
    internal::api(EVP_DigestUpdate(_ctxPtr, data, length));
#endif

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
        #if OPENSSL_VERSION_NUMBER < 0x10100000L
        internal::api(EVP_DigestFinal(&_ctx, &_digest[0], &len));
        #else
        internal::api(EVP_DigestFinal(_ctxPtr, &_digest[0], &len));
        #endif
        _digest.resize(len);
    }
    return _digest;
}


std::string Hash::digestStr()
{
    const ByteVec& vec = digest();
    return std::string((const char*)vec.data(), vec.size());
}


const std::string& Hash::algorithm(void) const
{
    return _algorithm;
}


} // namespace crypto
} // namespace scy


/// @\}
