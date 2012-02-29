/* jid.c
** strophe XMPP client library -- helper functions for parsing JIDs
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
 *  JID creation and parsing.
 */

#include <string.h>

#include "strophe.h"
#include "common.h"

/** Create a JID string from component parts node, domain, and resource.
 *
 *  @param ctx the Strophe context object
 *  @param node a string representing the node
 *  @param domain a string representing the domain.  Required.
 *  @param resource a string representing the resource
 *
 *  @return an allocated string with the full JID or NULL if no domain
 *      is specified
 */
char *xmpp_jid_new(xmpp_ctx_t *ctx, const char *node,
				    const char *domain,
				    const char *resource)
{
    char *result;
    int len,nlen,dlen,rlen;

    /* jid must at least have a domain */
    if (domain == NULL) return NULL;

    /* accumulate lengths */
    dlen = strlen(domain);
    nlen = (node) ? strlen(node) + 1 : 0;
    rlen = (resource) ? strlen(resource) + 1 : 0;
    len = nlen + dlen + rlen;

    /* concat components */
    result = xmpp_alloc(ctx, len + 1);
    if (result != NULL) {
	if (node != NULL) {
	    memcpy(result, node, nlen - 1);
	    result[nlen-1] = '@';
	}
        memcpy(result + nlen, domain, dlen);
	if (resource != NULL) {
	    result[nlen+dlen] = '/';
	    memcpy(result+nlen+dlen+1, resource, rlen - 1);
	}
	result[nlen+dlen+rlen] = '\0';
    }

    return result;
}

/** Create a bare JID from a JID.
 *  
 *  @param ctx the Strophe context object
 *  @param jid the JID
 *
 *  @return an allocated string with the bare JID or NULL on an error
 */
char *xmpp_jid_bare(xmpp_ctx_t *ctx, const char *jid)
{
    char *result;
    const char *c;

    c = strchr(jid, '/');
    if (c == NULL) return xmpp_strdup(ctx, jid);

    result = xmpp_alloc(ctx, c-jid+1);
    if (result != NULL) {
	memcpy(result, jid, c-jid);
	result[c-jid] = '\0';
    }

    return result;
}

/** Create a node string from a JID.
 *  
 *  @param ctx a Strophe context object
 *  @param jid the JID
 *
 *  @return an allocated string with the node or NULL if no node is found
 *      or an error occurs
 */
char *xmpp_jid_node(xmpp_ctx_t *ctx, const char *jid)
{
    char *result = NULL;
    const char *c;

    c = strchr(jid, '@');
    if (c != NULL) {
	result = xmpp_alloc(ctx, (c-jid) + 1);
	if (result != NULL) {
	    memcpy(result, jid, (c-jid));
	    result[c-jid] = '\0';
	}
    }

    return result;
}

/** Create a domain string from a JID.
 *
 *  @param ctx the Strophe context object
 *  @param jid the JID
 *
 *  @return an allocated string with the domain or NULL on an error
 */
char *xmpp_jid_domain(xmpp_ctx_t *ctx, const char *jid)
{
    char *result = NULL;
    const char *c,*s;

    c = strchr(jid, '@');
    if (c == NULL) {
	/* no node, assume domain */
	c = jid;
    } else {
	/* advance past the separator */
	c++;
    }
    s = strchr(c, '/');
    if (s == NULL) {
	/* no resource */
	s = c + strlen(c);
    }
    result = xmpp_alloc(ctx, (s-c) + 1);
    if (result != NULL) {
	memcpy(result, c, (s-c));
	result[s-c] = '\0';
    }

    return result;
}

/** Create a resource string from a JID.
 *
 *  @param ctx a Strophe context object
 *  @param jid the JID
 *
 *  @return an allocated string with the resource or NULL if no resource 
 *      is found or an error occurs
 */
char *xmpp_jid_resource(xmpp_ctx_t *ctx, const char *jid)
{
    char *result = NULL;
    const char *c;
    int len;

    c = strchr(jid, '/');
    if (c != NULL)  {
	c++;
	len = strlen(c);
	result = xmpp_alloc(ctx, len + 1);
	if (result != NULL) {
	    memcpy(result, c, len);
	    result[len] = '\0';
	}
    }

    return result;
}
