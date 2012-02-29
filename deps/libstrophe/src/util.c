/* util.c
** strophe XMPP client library -- various utility functions
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
 *  Utility functions.
 */

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#endif

#include "strophe.h"
#include "common.h"
#include "util.h"

/** implement our own strdup that uses the ctx allocator */
/** Duplicate a string.
 *  This function replaces the standard strdup library call with a version
 *  that uses the Strophe context object's allocator.
 *
 *  @param ctx a Strophe context object
 *  @param s a string
 *
 *  @return a new allocates string with the same data as s or NULL on error
 */
char *xmpp_strdup(const xmpp_ctx_t * const ctx, const char * const s)
{
    size_t len;
    char *copy;

    len = strlen(s);
    copy = xmpp_alloc(ctx, len + 1);
    if (!copy) {
        xmpp_error(ctx, "xmpp", "failed to allocate required memory");
        return NULL;
    }

    memcpy(copy, s, len + 1);

    return copy;
}

/** Return an integer based time stamp.
 *  This function uses gettimeofday or timeGetTime (on Win32 platforms) to
 *  compute an integer based time stamp.  This is used internally by the
 *  event loop and timed handlers.
 *
 *  @return an integer time stamp
 */
uint64_t time_stamp(void)
{
#ifdef _WIN32
    return timeGetTime();
#else
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
#endif
}

/** Get the time elapsed between two time stamps.
 *  This function returns the time elapsed between t1 and t2 by subtracting
 *  t1 from t2.  If t2 happened before t1, the result will be negative.  This
 *  function is used internally by the event loop and timed handlers.
 *
 *  @param t1 first time stamp
 *  @param t2 second time stamp
 *
 *  @return number of milliseconds between the stamps
 */
uint64_t time_elapsed(uint64_t t1, uint64_t t2)
{
    return (uint64_t)(t2 - t1);
}

/** Disconnect the stream with a memory error.
 *  This is a convenience function used internally by various parts of
 *  the Strophe library for terminating the connection because of a 
 *  memory error.
 *
 *  @param conn a Strophe connection object
 */
void disconnect_mem_error(xmpp_conn_t * const conn)
{
    xmpp_error(conn->ctx, "xmpp", "Memory allocation error");
    xmpp_disconnect(conn);
}
