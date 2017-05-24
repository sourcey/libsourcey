///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup crypto
/// @{


#include "scy/crypto/crypto.h"
#include "scy/random.h"
#include "scy/thread.h"

#include <mutex>
#include <stdexcept>

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>


extern "C" {
struct CRYPTO_dynlock_value
{
    std::mutex _mutex;
};
}


namespace scy {
namespace crypto {
namespace internal {


void throwError()
{
    unsigned long err;
    std::string msg;

    while ((err = ERR_get_error())) {
        if (!msg.empty())
            msg.append("; ");
        msg.append(ERR_error_string(err, 0));
    }

    throw std::runtime_error(msg);
}


void api(int ret, const char* error)
{
    if (ret == 0) {
        if (error)
            throw std::runtime_error(error);
        else
            throwError();
    }
}


//
// Private internal methods
//


const int SEEDSIZE = 256;
static std::mutex* _mutexes(0);
static std::mutex _mutex;
static int _refCount(0);


void lock(int mode, int n, const char* /* file */, int /* line */)
{
    if (mode & CRYPTO_LOCK)
        _mutexes[n].lock();
    else
        _mutexes[n].unlock();
}


// unsigned long id()
// {
//     return Thread::currentID();
// }


struct CRYPTO_dynlock_value* dynlockCreate(const char* /* file */,
                                           int /* line */)
{
    return new CRYPTO_dynlock_value;
}


void dynlock(int mode, struct CRYPTO_dynlock_value* lock,
             const char* /* file */, int /* line */)
{
    assert(lock);

    if (mode & CRYPTO_LOCK)
        lock->_mutex.lock();
    else
        lock->_mutex.unlock();
}


void dynlockDestroy(struct CRYPTO_dynlock_value* lock,
                    const char* /* file */, int /* line */)
{
    delete lock;
}


void init()
{
    std::lock_guard<std::mutex> guard(_mutex);

    if (++_refCount == 1) {
#if OPENSSL_VERSION_NUMBER >= 0x0907000L
        OPENSSL_config(NULL);
#endif
        SSL_library_init();
        SSL_load_error_strings();

        OpenSSL_add_all_algorithms();
#ifdef OPENSSL_IS_BORINGSSL
        CRYPTO_library_init();
#endif

        char seed[SEEDSIZE];
        Random::getSeed(seed, sizeof(seed));
        RAND_seed(seed, SEEDSIZE);

        int nMutexes = CRYPTO_num_locks();
        _mutexes = new std::mutex[nMutexes];
        CRYPTO_set_locking_callback(&internal::lock);
// #ifndef WIN32 // SF# 1828231: random unhandled exceptions when linking with ssl
//         CRYPTO_set_id_callback(&internal::id);
// #endif
        CRYPTO_set_dynlock_create_callback(&internal::dynlockCreate);
        CRYPTO_set_dynlock_lock_callback(&internal::dynlock);
        CRYPTO_set_dynlock_destroy_callback(&internal::dynlockDestroy);
    }
}


void uninit()
{
    // NOTE: crypto::uninit() should be called before the app exists
    // to endure the mutex is still in memory.
    std::lock_guard<std::mutex> guard(_mutex);

    if (--_refCount == 0) {
        EVP_cleanup();
        ERR_free_strings();
        CRYPTO_set_locking_callback(0);
        delete[] _mutexes;
    }
}


} // namespace internal


/// Initializes the OpenSSL library.
void initializeEngine()
{
    internal::init();
}

/// Shuts down the OpenSSL library.
void uninitializeEngine()
{
    internal::uninit();
}


} // namespace crypto
} // namespace scy


/// @\}
