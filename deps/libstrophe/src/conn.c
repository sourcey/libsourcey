/* conn.c
** strophe XMPP client library -- connection object functions
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
 *  Connection management.
 */

/** @defgroup Connections Connection management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <strophe.h>

#include "common.h"
#include "util.h"
#include "parser.h"

#ifndef DEFAULT_SEND_QUEUE_MAX
/** @def DEFAULT_SEND_QUEUE_MAX
 *  The default maximum send queue size.  This is currently unused.
 */
#define DEFAULT_SEND_QUEUE_MAX 64
#endif
#ifndef DISCONNECT_TIMEOUT
/** @def DISCONNECT_TIMEOUT 
 *  The time to wait (in milliseconds) for graceful disconnection to
 *  complete before the connection is reset.  The default is 2 seconds.
 */
#define DISCONNECT_TIMEOUT 2000 /* 2 seconds */
#endif
#ifndef CONNECT_TIMEOUT
/** @def CONNECT_TIMEOUT
 *  The time to wait (in milliseconds) for a connection attempt to succeed 
 * or error.  The default is 5 seconds.
 */
#define CONNECT_TIMEOUT 5000 /* 5 seconds */
#endif

static int _disconnect_cleanup(xmpp_conn_t * const conn, 
			       void * const clientdata);

static void _handle_stream_start(char *name, char **attrs, 
                                 void * const clientdata);
static void _handle_stream_end(char *name,
                               void * const clientdata);
static void _handle_stream_stanza(xmpp_stanza_t *stanza,
                                  void * const clientdata);

/** Create a new Strophe connection object.
 *
 *  @param ctx a Strophe context object
 *
 *  @return a Strophe connection object or NULL on an error
 *
 *  @ingroup Connections
 */
xmpp_conn_t *xmpp_conn_new(xmpp_ctx_t * const ctx)
{
    xmpp_conn_t *conn = NULL;
    xmpp_connlist_t *tail, *item;

    if (ctx == NULL) return NULL;
	conn = xmpp_alloc(ctx, sizeof(xmpp_conn_t));
    
    if (conn != NULL) {
	conn->ctx = ctx;

	conn->type = XMPP_UNKNOWN;
	conn->sock = -1;
	conn->tls = NULL;
	conn->timeout_stamp = 0;
	conn->error = 0;
	conn->stream_error = NULL;

	/* default send parameters */
	conn->blocking_send = 0;
	conn->send_queue_max = DEFAULT_SEND_QUEUE_MAX;
	conn->send_queue_len = 0;
	conn->send_queue_head = NULL;
	conn->send_queue_tail = NULL;

	/* default timeouts */
	conn->connect_timeout = CONNECT_TIMEOUT;

	conn->lang = xmpp_strdup(conn->ctx, "en");
	if (!conn->lang) {
	    xmpp_free(conn->ctx, conn);
	    return NULL;
	}
	conn->domain = NULL;
	conn->jid = NULL;
	conn->pass = NULL;
	conn->stream_id = NULL;
        conn->bound_jid = NULL;

	conn->tls_support = 0;
	conn->tls_failed = 0;
	conn->sasl_support = 0;
        conn->secured = 0;

	conn->bind_required = 0;
	conn->session_required = 0;

	conn->parser = parser_new(conn->ctx, 
                                  _handle_stream_start,
                                  _handle_stream_end,
                                  _handle_stream_stanza,
                                  conn);
        conn->reset_parser = 0;
        conn_prepare_reset(conn, auth_handle_open);

	conn->authenticated = 0;
	conn->conn_handler = NULL;
	conn->clientdata = NULL;
	conn->timed_handlers = NULL;
	/* we own (and will free) the hash values */
	conn->id_handlers = hash_new(conn->ctx, 32, NULL);
	conn->handlers = NULL;

	/* give the caller a reference to connection */
	conn->ref = 1;

	/* add connection to ctx->connlist */
	tail = conn->ctx->connlist;
	while (tail && tail->next) tail = tail->next;

	item = xmpp_alloc(conn->ctx, sizeof(xmpp_connlist_t));
	if (!item) {
	    xmpp_error(conn->ctx, "xmpp", "failed to allocate memory");
	    xmpp_free(conn->ctx, conn->lang);
            parser_free(conn->parser);
	    xmpp_free(conn->ctx, conn);
	    conn = NULL;
	} else {
	    item->conn = conn;
	    item->next = NULL;

	    if (tail) tail->next = item;
	    else conn->ctx->connlist = item;
	}
    }
    
    return conn;
}

/** Clone a Strophe connection object.
 *  
 *  @param conn a Strophe connection object
 *
 *  @return the same conn object passed in with its reference count
 *      incremented by 1
 *
 *  @ingroup Connections
 */
xmpp_conn_t *xmpp_conn_clone(xmpp_conn_t * const conn)
{
    conn->ref++;
    return conn;
}

/** Release a Strophe connection object.
 *  Decrement the reference count by one for a connection, freeing the 
 *  connection object if the count reaches 0.
 *
 *  @param conn a Strophe connection object
 *
 *  @return TRUE if the connection object was freed and FALSE otherwise
 *
 *  @ingroup Connections
 */
int xmpp_conn_release(xmpp_conn_t * const conn)
{
    xmpp_ctx_t *ctx;
    xmpp_connlist_t *item, *prev;
    xmpp_handlist_t *hlitem, *thli;
    hash_iterator_t *iter;
    const char *key;
    int released = 0;

    if (conn->ref > 1) 
	conn->ref--;
    else {
	ctx = conn->ctx;

	/* remove connection from context's connlist */
	if (ctx->connlist->conn == conn) {
	    item = ctx->connlist;
	    ctx->connlist = item->next;
	    xmpp_free(ctx, item);
	} else {
	    prev = NULL;
	    item = ctx->connlist;
	    while (item && item->conn != conn) {
		prev = item;
		item = item->next;
	    }

	    if (!item) {
		xmpp_error(ctx, "xmpp", "Connection not in context's list\n");
	    } else {
		prev->next = item->next;
		xmpp_free(ctx, item);
	    }
	}

	/* free handler stuff
	 * note that clientdata is the responsibility of the client
	 * and the handler pointers don't need to be freed since they
	 * are pointers to functions */

	hlitem = conn->timed_handlers;
	while (hlitem) {
	    thli = hlitem;
	    hlitem = hlitem->next;

	    xmpp_free(ctx, thli);
	}

	/* id handlers
	 * we have to traverse the hash table freeing list elements 
	 * then release the hash table */
	iter = hash_iter_new(conn->id_handlers);
	while ((key = hash_iter_next(iter))) {
	    hlitem = (xmpp_handlist_t *)hash_get(conn->id_handlers, key);
	    while (hlitem) {
		thli = hlitem;
		hlitem = hlitem->next;
		xmpp_free(conn->ctx, thli->id);
		xmpp_free(conn->ctx, thli);
	    }
	}
	hash_iter_release(iter);
	hash_release(conn->id_handlers);

	hlitem = conn->handlers;
	while (hlitem) {
	    thli = hlitem;
	    hlitem = hlitem->next;

	    if (thli->ns) xmpp_free(ctx, thli->ns);
	    if (thli->name) xmpp_free(ctx, thli->name);
	    if (thli->type) xmpp_free(ctx, thli->type);
	    xmpp_free(ctx, thli);
	}

	if (conn->stream_error) {
	    xmpp_stanza_release(conn->stream_error->stanza);
	    if (conn->stream_error->text)
		xmpp_free(ctx, conn->stream_error->text);
	    xmpp_free(ctx, conn->stream_error);
	}

        parser_free(conn->parser);
	
	if (conn->domain) xmpp_free(ctx, conn->domain);
	if (conn->jid) xmpp_free(ctx, conn->jid);
    if (conn->bound_jid) xmpp_free(ctx, conn->bound_jid);
	if (conn->pass) xmpp_free(ctx, conn->pass);
	if (conn->stream_id) xmpp_free(ctx, conn->stream_id);
	if (conn->lang) xmpp_free(ctx, conn->lang);
	xmpp_free(ctx, conn);
	released = 1;
    }

    return released;
}

/** Get the JID which is or will be bound to the connection.
 *  
 *  @param conn a Strophe connection object
 *
 *  @return a string containing the full JID or NULL if it has not been set
 *
 *  @ingroup Connections
 */
const char *xmpp_conn_get_jid(const xmpp_conn_t * const conn)
{
    return conn->jid;
}

/**
 * Get the JID discovered during binding time.
 *
 * This JID will contain the resource used by the current connection.
 * This is useful in the case where a resource was not specified for
 * binding.
 *
 * @param conn a Strophe connection object.
 *
 * @return a string containing the full JID or NULL if it's not been discovered
 *
 * @ingroup Connections
 */
const char *xmpp_conn_get_bound_jid(const xmpp_conn_t * const conn)
{
    return conn->bound_jid;
}

/** Set the JID of the user that will be bound to the connection.
 *  If any JID was previously set, it will be discarded.  This should not be 
 *  be used after a connection is created.  The function will make a copy of
 *  the JID string.  If the supllied JID is missing the node, SASL
 *  ANONYMOUS authentication will be used.
 *
 *  @param conn a Strophe connection object
 *  @param jid a full or bare JID
 *
 *  @ingroup Connections
 */
void xmpp_conn_set_jid(xmpp_conn_t * const conn, const char * const jid)
{
    if (conn->jid) xmpp_free(conn->ctx, conn->jid);
    conn->jid = xmpp_strdup(conn->ctx, jid);
}

/** Get the password used for authentication of a connection.
 *
 *  @param conn a Strophe connection object
 *
 *  @return a string containing the password or NULL if it has not been set
 *
 *  @ingroup Connections
 */
const char *xmpp_conn_get_pass(const xmpp_conn_t * const conn)
{
    return conn->pass;
}

/** Set the password used to authenticate the connection.
 *  If any password was previously set, it will be discarded.  The function
 *  will make a copy of the password string.
 * 
 *  @param conn a Strophe connection object
 *  @param pass the password
 *
 *  @ingroup Connections
 */
void xmpp_conn_set_pass(xmpp_conn_t * const conn, const char * const pass)
{
    if (conn->pass) xmpp_free(conn->ctx, conn->pass);
    conn->pass = xmpp_strdup(conn->ctx, pass);
}

/** Get the strophe context that the connection is associated with.
*  @param conn a Strophe connection object
* 
*  @return a Strophe context
* 
*  @ingroup Connections
*/
xmpp_ctx_t* xmpp_conn_get_context(xmpp_conn_t * const conn)
{
	return conn->ctx;
}

/** Initiate a connection to the XMPP server.
 *  This function returns immediately after starting the connection
 *  process to the XMPP server, and notifiations of connection state changes
 *  will be sent to the callback function.  The domain and port to connect to
 *  are usually determined by an SRV lookup for the xmpp-client service at
 *  the domain specified in the JID.  If SRV lookup fails, altdomain and 
 *  altport will be used instead if specified.
 *
 *  @param conn a Strophe connection object
 *  @param altdomain a string with domain to use if SRV lookup fails.  If this
 *      is NULL, the domain from the JID will be used.
 *  @param altport an integer port number to use if SRV lookup fails.  If this
 *      is 0, the default port (5222) will be assumed.
 *  @param callback a xmpp_conn_handler callback function that will receive
 *      notifications of connection status
 *  @param clientdata an opaque data pointer that will be passed to the callback
 *
 *  @return 0 on success and -1 on an error
 *
 *  @ingroup Connections
 */
int xmpp_connect_client(xmpp_conn_t * const conn, 
			  const char * const altdomain,
			  unsigned short altport,
			  xmpp_conn_handler callback,
			  void * const clientdata)
{
    char connectdomain[2048];
    int connectport;
    const char * domain;

    conn->type = XMPP_CLIENT;

    conn->domain = xmpp_jid_domain(conn->ctx, conn->jid);
    if (!conn->domain) return -1;

    if (altdomain) {
        xmpp_debug(conn->ctx, "xmpp", "Connecting via altdomain.");
        strcpy(connectdomain, altdomain);
        connectport = altport ? altport : 5222;
    } else if (!sock_srv_lookup("xmpp-client", "tcp", conn->domain,
                                connectdomain, 2048, &connectport)) {
	    xmpp_debug(conn->ctx, "xmpp", "SRV lookup failed.");
	    if (!altdomain)
		    domain = conn->domain;
	    else
		    domain = altdomain;
	    xmpp_debug(conn->ctx, "xmpp", "Using alternate domain %s, port %d",
                   altdomain, altport);
	    strcpy(connectdomain, domain);
	    connectport = altport ? altport : 5222;
    }
    conn->sock = sock_connect(connectdomain, connectport);
    xmpp_debug(conn->ctx, "xmpp", "sock_connect to %s:%d returned %d",
               connectdomain, connectport, conn->sock);
    if (conn->sock == -1) return -1;

    /* setup handler */
    conn->conn_handler = callback;
    conn->clientdata = clientdata;

    /* FIXME: it could happen that the connect returns immediately as
     * successful, though this is pretty unlikely.  This would be a little
     * hard to fix, since we'd have to detect and fire off the callback
     * from within the event loop */

    conn->state = XMPP_STATE_CONNECTING;
    conn->timeout_stamp = time_stamp();
    xmpp_debug(conn->ctx, "xmpp", "attempting to connect to %s", connectdomain);

    return 0;
}

/** Cleanly disconnect the connection.
 *  This function is only called by the stream parser when </stream:stream>
 *  is received, and it not intended to be called by code outside of Strophe.
 *
 *  @param conn a Strophe connection object
 */
void conn_disconnect_clean(xmpp_conn_t * const conn)
{
    /* remove the timed handler */
    xmpp_timed_handler_delete(conn, _disconnect_cleanup);

    conn_disconnect(conn);
}

/** Disconnect from the XMPP server.
 *  This function immediately disconnects from the XMPP server, and should
 *  not be used outside of the Strophe library.
 *
 *  @param conn a Strophe connection object
 */
void conn_disconnect(xmpp_conn_t * const conn) 
{
    xmpp_debug(conn->ctx, "xmpp", "Closing socket.");
    conn->state = XMPP_STATE_DISCONNECTED;
    if (conn->tls) {
	tls_stop(conn->tls);
	tls_free(conn->tls);
	conn->tls = NULL;
    }
    sock_close(conn->sock);

    /* fire off connection handler */
    conn->conn_handler(conn, XMPP_CONN_DISCONNECT, conn->error,
		       conn->stream_error, conn->clientdata);
}

/* prepares a parser reset.  this is called from handlers. we can't
 * reset the parser immediately as it is not re-entrant. */
void conn_prepare_reset(xmpp_conn_t * const conn, xmpp_open_handler handler)
{
    conn->reset_parser = 1;
    conn->open_handler = handler;
}

/* reset the parser */
void conn_parser_reset(xmpp_conn_t * const conn)
{
    conn->reset_parser = 0;
    parser_reset(conn->parser);
}

/* timed handler for cleanup if normal disconnect procedure takes too long */
static int _disconnect_cleanup(xmpp_conn_t * const conn, 
			       void * const clientdata)
{
    xmpp_debug(conn->ctx, "xmpp",
	       "disconnection forced by cleanup timeout");

    conn_disconnect(conn);

    return 0;
}

/** Initiate termination of the connection to the XMPP server.
 *  This function starts the disconnection sequence by sending
 *  </stream:stream> to the XMPP server.  This function will do nothing
 *  if the connection state is CONNECTING or CONNECTED.
 *
 *  @param conn a Strophe connection object
 *
 *  @ingroup Connections
 */
void xmpp_disconnect(xmpp_conn_t * const conn)
{
    if (conn->state != XMPP_STATE_CONNECTING &&
	conn->state != XMPP_STATE_CONNECTED)
	return;

    /* close the stream */
    xmpp_send_raw_string(conn, "</stream:stream>");

    /* setup timed handler in case disconnect takes too long */
    handler_add_timed(conn, _disconnect_cleanup,
		      DISCONNECT_TIMEOUT, NULL);
}

/** Send a raw string to the XMPP server.
 *  This function is a convenience function to send raw string data to the 
 *  XMPP server.  It is used by Strophe to send short messages instead of
 *  building up an XML stanza with DOM methods.  This should be used with care
 *  as it does not validate the data; invalid data may result in immediate
 *  stream termination by the XMPP server.
 *
 *  @param conn a Strophe connection object
 *  @param fmt a printf-style format string followed by a variable list of
 *      arguments to format
 */
void xmpp_send_raw_string(xmpp_conn_t * const conn, 
			  const char * const fmt, ...)
{
    va_list ap;
    size_t len;
    char buf[1024]; /* small buffer for common case */
    char *bigbuf;

    va_start(ap, fmt);
    len = xmpp_vsnprintf(buf, 1024, fmt, ap);
    va_end(ap);

    if (len >= 1024) {
	/* we need more space for this data, so we allocate a big 
	 * enough buffer and print to that */
	len++; /* account for trailing \0 */
	bigbuf = xmpp_alloc(conn->ctx, len);
	if (!bigbuf) {
	    xmpp_debug(conn->ctx, "xmpp", "Could not allocate memory for send_raw_string");
	    return;
	}
	va_start(ap, fmt);
	xmpp_vsnprintf(bigbuf, len, fmt, ap);
	va_end(ap);

	xmpp_debug(conn->ctx, "conn", "SENT: %s", bigbuf);

	/* len - 1 so we don't send trailing \0 */
	xmpp_send_raw(conn, bigbuf, len - 1);

	xmpp_free(conn->ctx, bigbuf);
    } else {
	xmpp_debug(conn->ctx, "conn", "SENT: %s", buf);

	xmpp_send_raw(conn, buf, len);
    }
}

/** Send raw bytes to the XMPP server.
 *  This function is a convenience function to send raw bytes to the 
 *  XMPP server.  It is usedly primarly by xmpp_send_raw_string.  This 
 *  function should be used with care as it does not validate the bytes and
 *  invalid data may result in stream termination by the XMPP server.
 *
 *  @param conn a Strophe connection object
 *  @param data a buffer of raw bytes
 *  @param len the length of the data in the buffer
 */
void xmpp_send_raw(xmpp_conn_t * const conn,
		   const char * const data, const size_t len)
{
    xmpp_send_queue_t *item;

    if (conn->state != XMPP_STATE_CONNECTED) return;

    /* create send queue item for queue */
    item = xmpp_alloc(conn->ctx, sizeof(xmpp_send_queue_t));
    if (!item) return;

    item->data = xmpp_alloc(conn->ctx, len);
    if (!item->data) {
	xmpp_free(conn->ctx, item);
	return;
    }
    memcpy(item->data, data, len);
    item->len = len;
    item->next = NULL;
    item->written = 0;

    /* add item to the send queue */
    if (!conn->send_queue_tail) {
	/* first item, set head and tail */
	conn->send_queue_head = item;
	conn->send_queue_tail = item;
    } else {
	/* add to the tail */
	conn->send_queue_tail->next = item;
	conn->send_queue_tail = item;
    }
    conn->send_queue_len++;
}

/** Send an XML stanza to the XMPP server.
 *  This is the main way to send data to the XMPP server.  The function will
 *  terminate without action if the connection state is not CONNECTED.
 *
 *  @param conn a Strophe connection object
 *  @param stanza a Strophe stanza object
 *
 *  @ingroup Connections
 */
void xmpp_send(xmpp_conn_t * const conn,
	       xmpp_stanza_t * const stanza)
{
    char *buf;
    size_t len;
    int ret;

    if (conn->state == XMPP_STATE_CONNECTED) {
	if ((ret = xmpp_stanza_to_text(stanza, &buf, &len)) == 0) {
	    xmpp_send_raw(conn, buf, len);
	    xmpp_debug(conn->ctx, "conn", "SENT: %s", buf);
	    xmpp_free(conn->ctx, buf);
	}
    }
}

/** Send the opening &lt;stream:stream&gt; tag to the server.
 *  This function is used by Strophe to begin an XMPP stream.  It should
 *  not be used outside of the library.
 *
 *  @param conn a Strophe connection object
 */
void conn_open_stream(xmpp_conn_t * const conn)
{
    xmpp_send_raw_string(conn, 
			 "<?xml version=\"1.0\"?>"			\
			 "<stream:stream to=\"%s\" "			\
			 "xml:lang=\"%s\" "				\
			 "version=\"1.0\" "				\
			 "xmlns=\"%s\" "				\
			 "xmlns:stream=\"%s\">", 
			 conn->domain,
			 conn->lang,
			 conn->type == XMPP_CLIENT ? XMPP_NS_CLIENT : XMPP_NS_COMPONENT,
			 XMPP_NS_STREAMS);
}

static void _log_open_tag(xmpp_conn_t *conn, char **attrs)
{
    char buf[4096];
    size_t len, pos;
    int i;
    
    if (!attrs) return;

    pos = 0;
    len = xmpp_snprintf(buf, 4096, "<stream:stream");
    if (len < 0) return;
    
    pos += len;
    
    for (i = 0; attrs[i]; i += 2) {
        len = xmpp_snprintf(&buf[pos], 4096 - pos, " %s='%s'",
                            attrs[i], attrs[i+1]);
        if (len < 0) return;
        pos += len;
    }

    len = xmpp_snprintf(&buf[pos], 4096 - pos, ">");
    if (len < 0) return;

    xmpp_debug(conn->ctx, "xmpp", "RECV: %s", buf);
}

static char *_get_stream_attribute(char **attrs, char *name)
{
    int i;

    if (!attrs) return NULL;

    for (i = 0; attrs[i]; i += 2)
        if (strcmp(name, attrs[i]) == 0)
            return attrs[i+1];

    return NULL;
}

static void _handle_stream_start(char *name, char **attrs, 
                                 void * const clientdata)
{
    xmpp_conn_t *conn = (xmpp_conn_t *)clientdata;
    char *id;

    if (strcmp(name, "stream:stream") != 0) {
        printf("name = %s\n", name);
        xmpp_error(conn->ctx, "conn", "Server did not open valid stream.");
        conn_disconnect(conn);
    } else {
        _log_open_tag(conn, attrs);
        
        if (conn->stream_id) xmpp_free(conn->ctx, conn->stream_id);

        id = _get_stream_attribute(attrs, "id");
        if (id)
            conn->stream_id = xmpp_strdup(conn->ctx, id);

        if (!conn->stream_id) {
            xmpp_error(conn->ctx, "conn", "Memory allocation failed.");
            conn_disconnect(conn);
        }
    }
    
    /* call stream open handler */
    conn->open_handler(conn);
}

static void _handle_stream_end(char *name,
                               void * const clientdata)
{
    xmpp_conn_t *conn = (xmpp_conn_t *)clientdata;

    /* stream is over */
    xmpp_debug(conn->ctx, "xmpp", "RECV: </stream:stream>");
    conn_disconnect_clean(conn);
}

static void _handle_stream_stanza(xmpp_stanza_t *stanza,
                                  void * const clientdata)
{
    xmpp_conn_t *conn = (xmpp_conn_t *)clientdata;
    char *buf;
    size_t len;

    if (xmpp_stanza_to_text(stanza, &buf, &len) == 0) {
        xmpp_debug(conn->ctx, "xmpp", "RECV: %s", buf);
        xmpp_free(conn->ctx, buf);
    }

    handler_fire_stanza(conn, stanza);
}
