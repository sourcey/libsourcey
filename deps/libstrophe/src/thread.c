/* thread.c
** strophe XMPP client library -- thread abstraction
**
** Copyright (C) 2005-2009 Collecta, Inc. 
**
**  This software is provided AS-IS with no warranty, either express
**  or implied.
**
**  This software is distributed under license and may not be copied,
**  modified or distributed except as expressly authorized under the
**  terms of the license contained in the file LICENSE.txt in this
**  distribution.
*/

/** @file
 *  Thread absraction.
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "strophe.h"
#include "common.h"
#include "thread.h"

struct _mutex_t {
    const xmpp_ctx_t *ctx;

#ifdef _WIN32
    HANDLE mutex;
#else
    pthread_mutex_t *mutex;
#endif
};

/* mutex functions */

mutex_t *mutex_create(const xmpp_ctx_t * ctx)
{
    mutex_t *mutex;

    mutex = xmpp_alloc(ctx, sizeof(mutex_t));
    if (mutex) {
	mutex->ctx = ctx;
#ifdef _WIN32
	mutex->mutex = CreateMutex(NULL, FALSE, NULL);
#else
	mutex->mutex = xmpp_alloc(ctx, sizeof(pthread_mutex_t));
	if (mutex->mutex)
	    if (pthread_mutex_init(mutex->mutex, NULL) != 0) {
		xmpp_free(ctx, mutex->mutex);
		mutex->mutex = NULL;
	    }
#endif
	if (!mutex->mutex) {
	    xmpp_free(ctx, mutex);
	    mutex = NULL;
	}
    }

    return mutex;
}

int mutex_destroy(mutex_t *mutex)
{
    int ret = 1;
    const xmpp_ctx_t *ctx;

#ifdef _WIN32
    if (mutex->mutex)
	ret = CloseHandle(mutex->mutex);
#else
    if (mutex->mutex)
	ret = pthread_mutex_destroy(mutex->mutex) == 0;
#endif
    ctx = mutex->ctx;
    xmpp_free(ctx, mutex);

    return ret;
}

int mutex_lock(mutex_t *mutex)
{
    int ret;

#ifdef _WIN32
    ret = WaitForSingleObject(mutex->mutex, INFINITE) == 0;
#else
    ret = pthread_mutex_lock(mutex->mutex) == 0;
#endif

    return ret;
}

int mutex_trylock(mutex_t *mutex)
{
    /* TODO */
    return 0;
}

int mutex_unlock(mutex_t *mutex)
{
    int ret;

#ifdef _WIN32
    ret = ReleaseMutex(mutex->mutex);
#else
    ret = pthread_mutex_unlock(mutex->mutex) == 0;
#endif

    return ret;
}
