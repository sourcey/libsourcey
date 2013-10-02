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

#include "Sourcey/Media/FFmpeg.h"
#include "Sourcey/Mutex.h"

#include <stdexcept>
/*
#include "Sourcey/Random.h"
#include "Sourcey/Mutex.h"
#include "Sourcey/Thread.h"

#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
//#include <openssl/opensslconf.h>
#if OPENSSL_VERSION_NUMBER >= 0x0907000L
#include <openssl/conf.h>
#endif
*/


	/*
extern "C"
{
	struct CRYPTO_dynlock_value
	{
		scy::Mutex _mutex;
	};
}
	*/

extern "C" {
#include <libavformat/avformat.h>
//#include <libavcodec/avcodec.h>
//#include <libavutil/fifo.h>
//#include <libswscale/swscale.h>
}


namespace scy {
namespace av {
namespace internal {


static int LockManagerOperation(void** lock, enum AVLockOp op) {
  switch (op) {
    case AV_LOCK_CREATE:
      *lock = new Mutex();
      if (!*lock)
        return 1;
      return 0;

    case AV_LOCK_OBTAIN:
      static_cast<Mutex*>(*lock)->lock();
      return 0;

    case AV_LOCK_RELEASE:
      static_cast<Mutex*>(*lock)->unlock();
      return 0;

    case AV_LOCK_DESTROY:
      delete static_cast<Mutex*>(*lock);
      *lock = NULL;
      return 0;
  }
  return 1;
}
	

static Mutex _mutex;
static int _refCount(0);


void initialize()
{
	Mutex::ScopedLock lock(_mutex);
	
	if (++_refCount == 1)
	{ 
		// Before doing anything disable logging as it interferes with layout tests.
		//av_log_set_level(AV_LOG_QUIET);

		// Register our protocol glue code with FFmpeg.
	    av_lockmgr_register(&LockManagerOperation);

		// Now register the rest of FFmpeg.
		av_register_all();
	}
}


void uninitialize()
{
	Mutex::ScopedLock lock(_mutex);

	if (--_refCount == 0) {
		av_lockmgr_register(NULL);
	}
}


} // namespace internal


void initializeFFmpeg()
{
	internal::initialize();
}
	

void uninitializeFFmpeg()
{
	internal::uninitialize();
}


} } // namespace scy::av




/*
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
*/
		/*
		EVP_cleanup();
		ERR_free_strings();
		CRYPTO_set_locking_callback(0);
		delete [] _mutexes;
		*/

		/*
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
#ifndef WIN32 // SF# 1828231: random unhandled exceptions when linking with ssl
		CRYPTO_set_id_callback(&internal::id);
#endif
		CRYPTO_set_dynlock_create_callback(&internal::dynlockCreate);
		CRYPTO_set_dynlock_lock_callback(&internal::dynlock);
		CRYPTO_set_dynlock_destroy_callback(&internal::dynlockDestroy);
		*/


/*
const int SEEDSIZE = 256;
static Mutex* _mutexes(0);
static Mutex _mutex;


void lock(int mode, int n, const char* file, int line)
{
	if (mode & CRYPTO_LOCK)
		_mutexes[n].lock();
	else
		_mutexes[n].unlock();
}


unsigned long id()
{
	return Thread::currentID();
}
*/

/*
//private:
Mutex* _mutexes(0);
Mutex _mutex;
int _refCount(0);

internal::OpenSSL()
{
	initialize();
}


internal::~OpenSSL()
{
	uninitialize();
}
*/
	
	/*
/// OpenSSL multithreading support
void lock(int mode, int n, const char* file, int line);
unsigned long id();
struct CRYPTO_dynlock_value* dynlockCreate(const char* file, int line);
void dynlock(int mode, struct CRYPTO_dynlock_value* lock, const char* file, int line);
void dynlockDestroy(struct CRYPTO_dynlock_value* lock, const char* file, int line);
*/