/* tls_dummy.c
** strophe XMPP client library -- TLS abstraction dummy impl.
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
 *  TLS dummy implementation.
 */

#include "common.h"
#include "tls.h"
#include "sock.h"

struct _tls {
    xmpp_ctx_t *ctx; /* do we need this? */
    sock_t sock;
    /* we don't implement anything */
};

void tls_initialize(void)
{
    return;
}

void tls_shutdown(void)
{
    return;
}

tls_t *tls_new(xmpp_ctx_t *ctx, sock_t sock)
{
    /* always fail */
    return NULL;
}

void tls_free(tls_t *tls)
{
    return;
}

int tls_set_credentials(tls_t *tls, const char *cafilename)
{
    return -1;
}

int tls_start(tls_t *tls)
{
    return -1;
}

int tls_stop(tls_t *tls)
{
    return -1;
}

int tls_error(tls_t *tls)
{
    /* todo: some kind of error polling/dump */
    return 0;
}

int tls_read(tls_t *tls, void * const buff, const size_t len)
{
    return -1;
}

int tls_write(tls_t *tls, const void * const buff, const size_t len)
{
    return -1;
}

int tls_clear_pending_write(tls_t *tls)
{
    return -1;
}

int tls_is_recoverable(int error)
{
    return 0;
}
