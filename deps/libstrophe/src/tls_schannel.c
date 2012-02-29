/* tls_schannel.c
** strophe XMPP client library -- TLS abstraction schannel impl.
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
 *  TLS implementation with Win32 SChannel.
 */

#include "common.h"
#include "tls.h"
#include "sock.h"

#define SECURITY_WIN32
#include <security.h>
#include <schnlsp.h>

struct _tls {
    xmpp_ctx_t *ctx;
    sock_t sock;

    HANDLE hsec32;
    SecurityFunctionTable *sft;
    CredHandle hcred;
    SecPkgInfo *spi;
    int init;

    CtxtHandle hctxt;
    SecPkgContext_StreamSizes spcss;

    unsigned char *recvbuffer;
    unsigned int recvbuffermaxlen;
    unsigned int recvbufferpos;

    unsigned char *readybuffer;
    unsigned int readybufferpos;
    unsigned int readybufferlen;

    unsigned char *sendbuffer;
    unsigned int sendbuffermaxlen;
    unsigned int sendbufferlen;
    unsigned int sendbufferpos;

    SECURITY_STATUS lasterror;
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
    tls_t *tls;
    PSecurityFunctionTable (*pInitSecurityInterface)(void);
    SCHANNEL_CRED scred;
    int ret;
    ALG_ID algs[1];

    SecPkgCred_SupportedAlgs spc_sa;
    SecPkgCred_CipherStrengths spc_cs;
    SecPkgCred_SupportedProtocols spc_sp;

    OSVERSIONINFO osvi;

    memset(&osvi, 0, sizeof(osvi));
    osvi.dwOSVersionInfoSize = sizeof(osvi);

    GetVersionEx(&osvi);

    /* no TLS support on win9x/me, despite what anyone says */
    if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
	return NULL;
    }

    tls = xmpp_alloc(ctx, sizeof(*tls));

    if (!tls) {
	return NULL;
    }

    memset(tls, 0, sizeof(*tls));
    tls->ctx = ctx;
    tls->sock = sock;

    if (!(tls->hsec32 = LoadLibrary ("secur32.dll"))) {
	tls_free(tls);
	return NULL;
    }

    if (!(pInitSecurityInterface = 
	  (void *)GetProcAddress(tls->hsec32, "InitSecurityInterfaceA"))) {
	tls_free(tls);
	return NULL;
    }

    tls->sft = pInitSecurityInterface();

    if (!tls->sft) {
	tls_free(tls);
	return NULL;
    }

    ret = tls->sft->QuerySecurityPackageInfo(UNISP_NAME, &(tls->spi));

    if (ret != SEC_E_OK)
    {
	tls_free(tls);
	return NULL;
    }

    xmpp_debug(ctx, "TLSS", "QuerySecurityPackageInfo() success");

    memset(&scred, 0, sizeof(scred));
    scred.dwVersion = SCHANNEL_CRED_VERSION;
    /*scred.grbitEnabledProtocols = SP_PROT_TLS1_CLIENT;*/
    /* Something down the line doesn't like AES, so force it to RC4 */
    algs[0] = CALG_RC4;
    scred.cSupportedAlgs = 1;
    scred.palgSupportedAlgs = algs;

    ret = tls->sft->AcquireCredentialsHandleA(NULL, UNISP_NAME,
	SECPKG_CRED_OUTBOUND, NULL, &scred, NULL, NULL, &(tls->hcred), NULL);

    if (ret != SEC_E_OK)
    {
	tls_free(tls);
	return NULL;
    }

    xmpp_debug(ctx, "TLSS", "AcquireCredentialsHandle() success");

    tls->init = 1;

    /* This bunch of queries should trip up wine until someone fixes
     * schannel support there */
    ret = tls->sft->QueryCredentialsAttributes(&(tls->hcred), SECPKG_ATTR_SUPPORTED_ALGS, &spc_sa);
    if (ret != SEC_E_OK)
    {
	tls_free(tls);
	return NULL;
    }

    ret = tls->sft->QueryCredentialsAttributes(&(tls->hcred), SECPKG_ATTR_CIPHER_STRENGTHS, &spc_cs);
    if (ret != SEC_E_OK)
    {
	tls_free(tls);
	return NULL;
    }

    ret = tls->sft->QueryCredentialsAttributes(&(tls->hcred), SECPKG_ATTR_SUPPORTED_PROTOCOLS, &spc_sp);
    if (ret != SEC_E_OK)
    {
	tls_free(tls);
	return NULL;
    }

    return tls;
}

void tls_free(tls_t *tls)
{
    if (tls->recvbuffer) {
	xmpp_free(tls->ctx, tls->recvbuffer);
    }

    if (tls->readybuffer) {
	xmpp_free(tls->ctx, tls->readybuffer);
    }

    if (tls->sendbuffer) {
	xmpp_free(tls->ctx, tls->sendbuffer);
    }

    if (tls->init) {
	tls->sft->FreeCredentialsHandle(&(tls->hcred));
    }

    tls->sft = NULL;

    if (tls->hsec32) {
	FreeLibrary(tls->hsec32);
	tls->hsec32 = NULL;
    }
    
    xmpp_free(tls->ctx, tls);
    return;
}

int tls_set_credentials(tls_t *tls, const char *cafilename)
{
    return -1;
}

int tls_start(tls_t *tls)
{
    ULONG ctxtreq = 0, ctxtattr = 0;
    SecBufferDesc sbdin, sbdout;
    SecBuffer sbin[2], sbout[1];
    SECURITY_STATUS ret;
    int sent;
    char *name = NULL;

    /* search the ctx's conns for our sock, and use the domain there as our
     * name */
    {
	xmpp_connlist_t *listentry = tls->ctx->connlist;

	while (listentry) {
	    xmpp_conn_t *conn = listentry->conn;

	    if (conn->sock == tls->sock) {
		name = strdup(conn->domain);
		listentry = NULL;
	    } else {
		listentry = listentry->next;
	    }
	}
    }

    ctxtreq = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT
	    | ISC_REQ_CONFIDENTIALITY | ISC_RET_EXTENDED_ERROR
	    | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM
	    | ISC_REQ_MANUAL_CRED_VALIDATION | ISC_REQ_INTEGRITY;

    memset(&(sbout[0]), 0, sizeof(sbout[0]));
    sbout[0].BufferType = SECBUFFER_TOKEN;

    memset(&sbdout, 0, sizeof(sbdout));
    sbdout.ulVersion = SECBUFFER_VERSION;
    sbdout.cBuffers = 1;
    sbdout.pBuffers = sbout;

    memset(&(sbin[0]), 0, sizeof(sbin[0]));
    sbin[0].BufferType = SECBUFFER_TOKEN;
    sbin[0].pvBuffer = xmpp_alloc(tls->ctx, tls->spi->cbMaxToken);
    sbin[0].cbBuffer = tls->spi->cbMaxToken;

    memset(&(sbin[1]), 0, sizeof(sbin[1]));
    sbin[1].BufferType = SECBUFFER_EMPTY;

    memset(&sbdin, 0, sizeof(sbdin));
    sbdin.ulVersion = SECBUFFER_VERSION;
    sbdin.cBuffers = 2;
    sbdin.pBuffers = sbin;

    ret = tls->sft->InitializeSecurityContextA(&(tls->hcred), NULL, name, ctxtreq, 0, 0,
					  NULL, 0, &(tls->hctxt), &sbdout,
					  &ctxtattr, NULL);

    while (ret == SEC_I_CONTINUE_NEEDED
	   || ret == SEC_I_INCOMPLETE_CREDENTIALS) {
	unsigned char *p = sbin[0].pvBuffer;
	int len = 0, inbytes = 0;

	if (sbdout.pBuffers[0].cbBuffer) {
	    unsigned char *writebuff = sbdout.pBuffers[0].pvBuffer;
	    unsigned int writelen = sbdout.pBuffers[0].cbBuffer;

	    sent = sock_write(tls->sock, writebuff, writelen);
	    if (sent == -1) {
		tls->lasterror = sock_error();
	    }
	    else
	    {
		writebuff += sent;
		writelen -= sent;
	    }
	    tls->sft->FreeContextBuffer(sbdout.pBuffers[0].pvBuffer);
	    sbdout.pBuffers[0].pvBuffer = NULL;
	    sbdout.pBuffers[0].cbBuffer = 0;
	}

	/* poll for a bit until the remote server stops sending data, ie it
	 * finishes sending the token */
	inbytes = 1;
	{
	    fd_set fds;
	    struct timeval tv;

	    tv.tv_sec = 2;
	    tv.tv_usec = 0;

	    FD_ZERO(&fds);
	    FD_SET(tls->sock, &fds);
    
	    select(tls->sock, &fds, NULL, NULL, &tv);
	}

	while (inbytes > 0) {
	    fd_set fds;
	    struct timeval tv;

	    tv.tv_sec = 0;
	    tv.tv_usec = 1000;

	    FD_ZERO(&fds);
	    FD_SET(tls->sock, &fds);
    
	    select(tls->sock, &fds, NULL, NULL, &tv);

	    inbytes = sock_read(tls->sock, p, tls->spi->cbMaxToken - len);

	    if (inbytes > 0) {
		len += inbytes;
		p += inbytes;
	    }
	    else
	    {
	        tls->lasterror = sock_error();
	    }

	}

	sbin[0].cbBuffer = len;

	ret = tls->sft->InitializeSecurityContextA(&(tls->hcred), &(tls->hctxt), name,
					      ctxtreq, 0, 0, &sbdin, 0,
					      &(tls->hctxt), &sbdout,
					      &ctxtattr, NULL);
    }

    if (ret == SEC_E_OK) {
	if (sbdout.pBuffers[0].cbBuffer) {
	    unsigned char *writebuff = sbdout.pBuffers[0].pvBuffer;
	    unsigned int writelen = sbdout.pBuffers[0].cbBuffer;
	    sent = sock_write(tls->sock, writebuff, writelen);
	    if (sent == -1) {
		tls->lasterror = sock_error();
	    }
	    else
	    {
		writebuff += sent;
		writelen -= sent;
	    }
	    tls->sft->FreeContextBuffer(sbdout.pBuffers[0].pvBuffer);
	    sbdout.pBuffers[0].pvBuffer = NULL;
	    sbdout.pBuffers[0].cbBuffer = 0;
	}
    }

    xmpp_free(tls->ctx, sbin[0].pvBuffer);

    if (ret != SEC_E_OK) {
	tls->lasterror = ret;
	return 0;
    }

    tls->sft->QueryContextAttributes(&(tls->hctxt), SECPKG_ATTR_STREAM_SIZES, 
				&(tls->spcss));

    tls->recvbuffermaxlen = tls->spcss.cbHeader + tls->spcss.cbMaximumMessage
			    + tls->spcss.cbTrailer;
    tls->recvbuffer       = xmpp_alloc(tls->ctx, tls->recvbuffermaxlen);
    tls->recvbufferpos    = 0;

    tls->sendbuffermaxlen = tls->spcss.cbHeader + tls->spcss.cbMaximumMessage
			    + tls->spcss.cbTrailer;
    tls->sendbuffer       = xmpp_alloc(tls->ctx, tls->sendbuffermaxlen);
    tls->sendbufferpos    = 0;
    tls->sendbufferlen    = 0;

    tls->readybuffer      = xmpp_alloc(tls->ctx, tls->spcss.cbMaximumMessage);
    tls->readybufferpos   = 0;
    tls->readybufferlen   = 0;

    return 1;
}

int tls_stop(tls_t *tls)
{
    return -1;
}

int tls_error(tls_t *tls)
{
    return tls->lasterror;
}

int tls_is_recoverable(int error)
{
    return (error == SEC_E_OK || error == SEC_E_INCOMPLETE_MESSAGE
	    || error == WSAEWOULDBLOCK || error == WSAEMSGSIZE
	    || error == WSAEINPROGRESS);
}

int tls_read(tls_t *tls, void * const buff, const size_t len)
{
    int bytes;

    /* first, if we've got some ready data, put that in the buffer */
    if (tls->readybufferpos < tls->readybufferlen)
    {
	if (len < tls->readybufferlen - tls->readybufferpos) {
	    bytes = len;
	} else {
	    bytes = tls->readybufferlen - tls->readybufferpos;
	}

	memcpy(buff, tls->readybuffer + tls->readybufferpos, bytes);

	if (len < tls->readybufferlen - tls->readybufferpos) {
	    tls->readybufferpos += bytes;
	    return bytes;
	} else {
	    unsigned char *newbuff = buff;
	    int read;
	    tls->readybufferpos += bytes;
	    newbuff += bytes;
	    read = tls_read(tls, newbuff, len - bytes);

	    if (read == -1) {
	        if (tls_is_recoverable(tls->lasterror))	{
		    return bytes;
		}

		return -1;
	    }

	    return bytes + read;
	}
    }

    /* next, top up our recv buffer */
    bytes = sock_read(tls->sock, tls->recvbuffer + tls->recvbufferpos,
		      tls->recvbuffermaxlen - tls->recvbufferpos);

    if (bytes == -1) {
	if (!tls_is_recoverable(sock_error())) {
	    tls->lasterror = sock_error();
	    return -1;
	}
    }

    if (bytes > 0) {
	tls->recvbufferpos += bytes;
    }

    /* next, try to decrypt the recv buffer */
    if (tls->recvbufferpos > 0) {
	SecBufferDesc sbddec;
	SecBuffer sbdec[4];
	int ret;

	memset(&sbddec, 0, sizeof(sbddec));
	sbddec.ulVersion = SECBUFFER_VERSION;
	sbddec.cBuffers = 4;
	sbddec.pBuffers = sbdec;

	memset(&(sbdec[0]), 0, sizeof(sbdec[0]));
	sbdec[0].BufferType = SECBUFFER_DATA;
	sbdec[0].pvBuffer = tls->recvbuffer;
	sbdec[0].cbBuffer = tls->recvbufferpos;

	memset(&(sbdec[1]), 0, sizeof(sbdec[1]));
	sbdec[1].BufferType = SECBUFFER_EMPTY;

	memset(&(sbdec[2]), 0, sizeof(sbdec[2]));
	sbdec[2].BufferType = SECBUFFER_EMPTY;

	memset(&(sbdec[3]), 0, sizeof(sbdec[3]));
	sbdec[3].BufferType = SECBUFFER_EMPTY;

	ret = tls->sft->DecryptMessage(&(tls->hctxt), &sbddec, 0, NULL);

	if (ret == SEC_E_OK) {
	    memcpy(tls->readybuffer, sbdec[1].pvBuffer, sbdec[1].cbBuffer); 
	    tls->readybufferpos = 0;
	    tls->readybufferlen = sbdec[1].cbBuffer;
	    /* have we got some data left over?  If so, copy it to the start
	     * of the recv buffer */
	    if (sbdec[3].BufferType == SECBUFFER_EXTRA) {
		memcpy(tls->recvbuffer, sbdec[3].pvBuffer, sbdec[3].cbBuffer);
		tls->recvbufferpos = sbdec[3].cbBuffer;
	    } else {
		tls->recvbufferpos = 0;
	    }

	    return tls_read(tls, buff, len);
	} else if (ret == SEC_E_INCOMPLETE_MESSAGE) {
	    tls->lasterror = SEC_E_INCOMPLETE_MESSAGE;
	    return -1;
	} else if (ret == SEC_I_RENEGOTIATE) {
	    ret = tls_start(tls);
	    if (!ret)
	    {
		return -1;
	    }

	    /* fake an incomplete message so we're called again */
	    tls->lasterror = SEC_E_INCOMPLETE_MESSAGE;
	    return -1;
	}

	/* something bad happened, so we bail */
	tls->lasterror = ret;

	return -1;
    }

    tls->lasterror = SEC_E_INCOMPLETE_MESSAGE;

    return -1;
}

int tls_clear_pending_write(tls_t *tls)
{
    if (tls->sendbufferpos < tls->sendbufferlen)
    {
	int bytes;

	bytes = sock_write(tls->sock, tls->sendbuffer + tls->sendbufferpos,
			   tls->sendbufferlen - tls->sendbufferpos);

	if (bytes == -1) {
	    tls->lasterror = sock_error();
	    return -1;
	} else if (bytes > 0) {
	    tls->sendbufferpos += bytes;
	}

	if (tls->sendbufferpos < tls->sendbufferlen) {
	    return 0;
	}
    }

    return 1;
}

int tls_write(tls_t *tls, const void * const buff, const size_t len)
{
    SecBufferDesc sbdenc;
    SecBuffer sbenc[4];
    unsigned char *sendbuffer;
    const unsigned char *p = buff;
    int sent = 0, ret, remain = len;

    ret = tls_clear_pending_write(tls);
    if (ret <= 0) {
	return ret;
    }

    tls->sendbufferpos = 0;
    tls->sendbufferlen = 0;

    memset(&sbdenc, 0, sizeof(sbdenc));
    sbdenc.ulVersion = SECBUFFER_VERSION;
    sbdenc.cBuffers = 4;
    sbdenc.pBuffers = sbenc;

    memset(&(sbenc[0]), 0, sizeof(sbenc[0]));
    sbenc[0].BufferType = SECBUFFER_STREAM_HEADER;

    memset(&(sbenc[1]), 0, sizeof(sbenc[1]));
    sbenc[1].BufferType = SECBUFFER_DATA;

    memset(&(sbenc[2]), 0, sizeof(sbenc[2]));
    sbenc[2].BufferType = SECBUFFER_STREAM_TRAILER;

    memset(&(sbenc[3]), 0, sizeof(sbenc[3]));
    sbenc[3].BufferType = SECBUFFER_EMPTY;

    sbenc[0].pvBuffer = tls->sendbuffer;
    sbenc[0].cbBuffer = tls->spcss.cbHeader;

    sbenc[1].pvBuffer = tls->sendbuffer + tls->spcss.cbHeader;

    while (remain > 0)
    {
	if (remain > tls->spcss.cbMaximumMessage) {
	    sbenc[1].cbBuffer = tls->spcss.cbMaximumMessage;
	} else {
	    sbenc[1].cbBuffer = remain;
	}

	sbenc[2].pvBuffer = (unsigned char *)sbenc[1].pvBuffer
			    + sbenc[1].cbBuffer;
        sbenc[2].cbBuffer = tls->spcss.cbTrailer;

	memcpy(sbenc[1].pvBuffer, p, sbenc[1].cbBuffer);
	p += tls->spcss.cbMaximumMessage;

	tls->sendbufferlen = sbenc[0].cbBuffer + sbenc[1].cbBuffer
			     + sbenc[2].cbBuffer;

	ret = tls->sft->EncryptMessage(&(tls->hctxt), 0, &sbdenc, 0);

	if (ret != SEC_E_OK) {
	    tls->lasterror = ret;
	    return -1;
	}

	tls->sendbufferpos = 0;

	ret = tls_clear_pending_write(tls);

	if (ret == -1 && !tls_is_recoverable(tls_error(tls))) {
	    return -1;
	} 
	
	if (remain > tls->spcss.cbMaximumMessage) {
	    sent += tls->spcss.cbMaximumMessage;
	    remain -= tls->spcss.cbMaximumMessage;
	} else {
	    sent += remain;
	    remain = 0;
	}

	if (ret == 0 || (ret == -1 && tls_is_recoverable(tls_error(tls)))) {
	    return sent;
	}

    }

    return sent;
}
