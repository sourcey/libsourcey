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


#include "scy/crypto/crypto.h"
#include "scy/random.h"
#include "scy/mutex.h"
#include "scy/thread.h"
#include <stdexcept>

#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
//#include <openssl/opensslconf.h>
#if OPENSSL_VERSION_NUMBER >= 0x0907000L
#include <openssl/conf.h>
#endif


extern "C"
{
    struct CRYPTO_dynlock_value
    {
        scy::Mutex _mutex;
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
static Mutex* _mutexes(0);
static Mutex _mutex;
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


struct CRYPTO_dynlock_value* dynlockCreate(const char* /* file */, int /* line */)
{
    return new CRYPTO_dynlock_value;
}


void dynlock(int mode, struct CRYPTO_dynlock_value* lock, const char* /* file */, int /* line */)
{
    assert(lock);

    if (mode & CRYPTO_LOCK)
        lock->_mutex.lock();
    else
        lock->_mutex.unlock();
}


void dynlockDestroy(struct CRYPTO_dynlock_value* lock, const char* /* file */, int /* line */)
{
    delete lock;
}


void initialize()
{
    Mutex::ScopedLock lock(_mutex);

    if (++_refCount == 1)
    {
#if OPENSSL_VERSION_NUMBER >= 0x0907000L
        OPENSSL_config(NULL);
#endif
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();

        char seed[SEEDSIZE];
        Random::getSeed(seed, sizeof(seed));
        RAND_seed(seed, SEEDSIZE);

        int nMutexes = CRYPTO_num_locks();
        _mutexes = new Mutex[nMutexes];
        CRYPTO_set_locking_callback(&internal::lock);
// #ifndef WIN32 // SF# 1828231: random unhandled exceptions when linking with ssl
//         CRYPTO_set_id_callback(&internal::id);
// #endif
        CRYPTO_set_dynlock_create_callback(&internal::dynlockCreate);
        CRYPTO_set_dynlock_lock_callback(&internal::dynlock);
        CRYPTO_set_dynlock_destroy_callback(&internal::dynlockDestroy);
    }
}


void uninitialize()
{
    // NOTE: crypto::uninitialize() should be called before the app exists
    // to endure the mutex is still in memory.
    Mutex::ScopedLock lock(_mutex);

    if (--_refCount == 0) {
        EVP_cleanup();
        ERR_free_strings();
        CRYPTO_set_locking_callback(0);
        delete [] _mutexes;
    }
}


} // namespace internal


/// Initializes the OpenSSL library.
void initializeEngine()
{
    internal::initialize();
}

/// Shuts down the OpenSSL library.
void uninitializeEngine()
{
    internal::uninitialize();
}


} } // namespace scy::crypto
