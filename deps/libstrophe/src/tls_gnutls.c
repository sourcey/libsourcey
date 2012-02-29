/* tls.c
** strophe XMPP client library -- TLS abstraction header
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
 *  TLS implementation with GNUTLS
 */

#include <gnutls/gnutls.h>

#include "common.h"
#include "tls.h"
#include "sock.h"

struct _tls {
    xmpp_ctx_t *ctx; /* do we need this? */
    sock_t sock;
    gnutls_session_t session;
    gnutls_certificate_credentials_t cred;
};

void tls_initialize(void)
{
    /* initialize the GNU TLS global state */
    gnutls_global_init();

    /* TODO: wire in xmpp_ctx_t allocator somehow?
       unfortunately in gnutls it's global, so we can
       only do so much. */
}

void tls_shutdown(void)
{
    /* tear down the GNU TLS global state */
    gnutls_global_deinit();
}

tls_t *tls_new(xmpp_ctx_t *ctx, sock_t sock)
{
    tls_t *tls = xmpp_alloc(ctx, sizeof(tls_t));
    const int cert_type_priority[3] = { GNUTLS_CRT_X509,
        GNUTLS_CRT_OPENPGP, 0 };

    if (tls) {
	tls->ctx = ctx;
	tls->sock = sock;
	gnutls_init(&tls->session, GNUTLS_CLIENT);

	gnutls_certificate_allocate_credentials(&tls->cred);

	gnutls_set_default_priority(tls->session);
	gnutls_certificate_type_set_priority(tls->session, cert_type_priority);

	/* fixme: this may require setting a callback on win32? */
	gnutls_transport_set_ptr(tls->session, 
		(gnutls_transport_ptr_t)sock);
    }

    return tls;
}

void tls_free(tls_t *tls)
{
    gnutls_deinit(tls->session);
    gnutls_certificate_free_credentials(tls->cred);
    xmpp_free(tls->ctx, tls);
}

int tls_set_credentials(tls_t *tls, const char *cafilename)
{
    int err;

    /* set trusted credentials -- takes a .pem filename */
    err = gnutls_certificate_set_x509_trust_file(tls->cred,
	    cafilename, GNUTLS_X509_FMT_PEM);
    if (err < 0) return err;

    err = gnutls_credentials_set(tls->session, GNUTLS_CRD_CERTIFICATE, 
	    tls->cred);

    return err;
}

int tls_start(tls_t *tls)
{
    return gnutls_handshake(tls->session);
}

int tls_stop(tls_t *tls)
{
    return gnutls_bye(tls->session, GNUTLS_SHUT_RDWR);
}

int tls_error(tls_t *tls)
{
    /* todo: some kind of error polling/dump */
    return 0;
}

int tls_read(tls_t *tls, void * const buff, const size_t len)
{
    int ret;

    ret = gnutls_record_recv(tls->session, buff, len);

    return ret;
}

int tls_write(tls_t *tls, const void * const buff, const size_t len)
{
    int ret;

    ret = gnutls_record_send(tls->session, buff, len);

    return ret;
}

