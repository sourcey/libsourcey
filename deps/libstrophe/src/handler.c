/* handler.c
** strophe XMPP client library -- event handler management
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
 *  Event handler management.
 */

/** @defgroup Handlers Stanza and timed event handlers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <stdint.h>
#else
#include "ostypes.h"
#endif

#include "strophe.h"
#include "common.h"

/** Fire off all stanza handlers that match.
 *  This function is called internally by the event loop whenever stanzas
 *  are received from the XMPP server.
 *
 *  @param conn a Strophe connection object
 *  @param stanza a Strophe stanza object
 */
void handler_fire_stanza(xmpp_conn_t * const conn,
			 xmpp_stanza_t * const stanza)
{
    xmpp_handlist_t *item, *prev;
    char *id, *ns, *name, *type;
    
    /* call id handlers */
    id = xmpp_stanza_get_id(stanza);
    if (id) {
	prev = NULL;
 	item = (xmpp_handlist_t *)hash_get(conn->id_handlers, id);
	while (item) {
	    xmpp_handlist_t *next = item->next;

	    if (item->user_handler && !conn->authenticated) {
		item = next;
 		continue;
	    }

	    if (!((xmpp_handler)(item->handler))(conn, stanza, item->clientdata)) {
		/* handler is one-shot, so delete it */
		if (prev)
		    prev->next = next;
		else {
		    hash_drop(conn->id_handlers, id);
		    hash_add(conn->id_handlers, id, next);
		}
                xmpp_free(conn->ctx, item->id);
		xmpp_free(conn->ctx, item);
		item = NULL;
	    }
	    if (item)
		prev = item;
	    item = next;
	}
    }
    
    /* call handlers */
    ns = xmpp_stanza_get_ns(stanza);
    name = xmpp_stanza_get_name(stanza);
    type = xmpp_stanza_get_type(stanza);
    
    /* enable all added handlers */
    for (item = conn->handlers; item; item = item->next)
	item->enabled = 1;

    prev = NULL;
    item = conn->handlers;
    while (item) {
	/* skip newly added handlers */
	if (!item->enabled) {
	    prev = item;
	    item = item->next;
	    continue;
	}

	/* don't call user handlers until authentication succeeds */
	if (item->user_handler && !conn->authenticated) {
	    prev = item;
	    item = item->next;
	    continue;
	}

	if ((!item->ns || (ns && strcmp(ns, item->ns) == 0) ||
	     xmpp_stanza_get_child_by_ns(stanza, item->ns)) &&
	    (!item->name || (name && strcmp(name, item->name) == 0)) &&
	    (!item->type || (type && strcmp(type, item->type) == 0)))
	    if (!((xmpp_handler)(item->handler))(conn, stanza, item->clientdata)) {
		/* handler is one-shot, so delete it */
		if (prev)
		    prev->next = item->next;
		else
		    conn->handlers = item->next;
                if (item->ns) xmpp_free(conn->ctx, item->ns);
                if (item->name) xmpp_free(conn->ctx, item->name);
                if (item->type) xmpp_free(conn->ctx, item->type);
		xmpp_free(conn->ctx, item);
		item = NULL;
	    }
	
	if (item) {
	    prev = item;
	    item = item->next;
	} else if (prev)
	    item = prev->next;
	else
	    item = conn->handlers;
    }
}

/** Fire off all timed handlers that are ready.
 *  This function is called internally by the event loop.
 *
 *  @param ctx a Strophe context object
 *
 *  @return the time in milliseconds until the next handler will be ready
 */
uint64_t handler_fire_timed(xmpp_ctx_t * const ctx)
{
    xmpp_connlist_t *connitem;
    xmpp_handlist_t *handitem, *temp;
    int ret, fired;
    uint64_t elapsed, min;

    min = (uint64_t)(-1);

    connitem = ctx->connlist;
    while (connitem) {
	if (connitem->conn->state != XMPP_STATE_CONNECTED) {
	    connitem = connitem->next;
	    continue;
	}
	
	/* enable all handlers that were added */
	for (handitem = connitem->conn->timed_handlers; handitem;
	     handitem = handitem->next)
	    handitem->enabled = 1;

	handitem = connitem->conn->timed_handlers;
	while (handitem) {
	    /* skip newly added handlers */
	    if (!handitem->enabled) {
		handitem = handitem->next;
		continue;
	    }

	    /* only fire user handlers after authentication */
	    if (handitem->user_handler && !connitem->conn->authenticated) {
		handitem = handitem->next;
		continue;
	    }

	    fired = 0;
	    elapsed = time_elapsed(handitem->last_stamp, time_stamp());
	    if (elapsed >= handitem->period) {
		/* fire! */
		fired = 1;
		handitem->last_stamp = time_stamp();
		ret = ((xmpp_timed_handler)handitem->handler)(connitem->conn, handitem->clientdata);
	    } else if (min > (handitem->period - elapsed))
		min = handitem->period - elapsed;
		
	    temp = handitem;
	    handitem = handitem->next;

	    /* delete handler if it returned false */
	    if (fired && !ret)
		xmpp_timed_handler_delete(connitem->conn, temp->handler);
	}

	connitem = connitem->next;
    }

    return min;
}

/** Reset all timed handlers.
 *  This function is called internally when a connection is successful.
 *
 *  @param conn a Strophe connection object
 *  @param user_only whether to reset all handlers or only user ones
 */
void handler_reset_timed(xmpp_conn_t *conn, int user_only)
{
    xmpp_handlist_t *handitem;

    handitem = conn->timed_handlers;
    while (handitem) {
	if ((user_only && handitem->user_handler) || !user_only)
	    handitem->last_stamp = time_stamp();
	
	handitem = handitem->next;
    }
}

static void _timed_handler_add(xmpp_conn_t * const conn,
			       xmpp_timed_handler handler,
			       const unsigned long period,
			       void * const clientdata, 
			       const int user_handler)
{
    xmpp_handlist_t *item, *tail;

    /* check if handler is already in the list */
    for (item = conn->timed_handlers; item; item = item->next) {
	if (item->handler == (void *)handler)
	    break;
    }
    if (item) return;

    /* build new item */
    item = xmpp_alloc(conn->ctx, sizeof(xmpp_handlist_t));
    if (!item) return;

    item->user_handler = user_handler;
    item->handler = (void *)handler;
    item->clientdata = clientdata;
    item->enabled = 0;
    item->next = NULL;

    item->period = period;
    item->last_stamp = time_stamp();

    /* append item to list */
    if (!conn->timed_handlers)
	conn->timed_handlers = item;
    else {
	tail = conn->timed_handlers;
	while (tail->next) 
	    tail = tail->next;
	tail->next = item;
    }
}

/** Delete a timed handler.
 *
 *  @param conn a Strophe connection object
 *  @param handler function pointer to the handler
 *
 *  @ingroup Handlers
 */
void xmpp_timed_handler_delete(xmpp_conn_t * const conn,
			       xmpp_timed_handler handler)
{
    xmpp_handlist_t *item, *prev;

    if (!conn->timed_handlers) return;

    prev = NULL;
    item = conn->timed_handlers;
    while (item) {
	if (item->handler == (void *)handler)
	    break;
	prev = item;
	item = item->next;
    }

    if (item) {
	if (prev)
	    prev->next = item->next;
	else
	    conn->timed_handlers = item->next;
	
	xmpp_free(conn->ctx, item);
    }
}

static void _id_handler_add(xmpp_conn_t * const conn,
			 xmpp_handler handler,
			 const char * const id,
			 void * const clientdata, int user_handler)
{
    xmpp_handlist_t *item, *tail;

    /* check if handler is already in the list */
    item = (xmpp_handlist_t *)hash_get(conn->id_handlers, id);
    while (item) {
	if (item->handler == (void *)handler)
	    break;
	item = item->next;
    }
    if (item) return;

    /* build new item */
    item = xmpp_alloc(conn->ctx, sizeof(xmpp_handlist_t));
    if (!item) return;

    item->user_handler = user_handler;
    item->handler = (void *)handler;
    item->clientdata = clientdata;
    item->enabled = 0;
    item->next = NULL;

    item->id = xmpp_strdup(conn->ctx, id);
    if (!item->id) {
	xmpp_free(conn->ctx, item);
	return;
    }

    /* put on list in hash table */
    tail = (xmpp_handlist_t *)hash_get(conn->id_handlers, id);
    if (!tail)
	hash_add(conn->id_handlers, id, item);
    else {
	while (tail->next) 
	    tail = tail->next;
	tail->next = item;
    }
}

/** Delete an id based stanza handler.
 *
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a stanza handler
 *  @param id a string containing the id the handler is for
 *
 *  @ingroup Handlers
 */
void xmpp_id_handler_delete(xmpp_conn_t * const conn,
			    xmpp_handler handler,
			    const char * const id)
{
    xmpp_handlist_t *item, *prev;

    prev = NULL;
    item = (xmpp_handlist_t *)hash_get(conn->id_handlers, id);
    if (!item) return;

    while (item) {
	if (item->handler == (void *)handler)
	    break;

	prev = item;
	item = item->next;
    }

    if (item) {
	if (prev)
	    prev->next = item->next;
	else {
	    hash_drop(conn->id_handlers, id);
	    hash_add(conn->id_handlers, id, item->next);
	}
	xmpp_free(conn->ctx, item->id);
	xmpp_free(conn->ctx, item);
    }
}

/* add a stanza handler */
static void _handler_add(xmpp_conn_t * const conn,
			 xmpp_handler handler,
			 const char * const ns,
			 const char * const name,
			 const char * const type,
			 void * const clientdata, int user_handler)
{
    xmpp_handlist_t *item, *tail;

    /* check if handler already in list */
    for (item = conn->handlers; item; item = item->next) {
	if (item->handler == (void *)handler)
	    break;
    }
    if (item) return;

    /* build new item */
    item = (xmpp_handlist_t *)xmpp_alloc(conn->ctx, sizeof(xmpp_handlist_t));
    if (!item) return;

    item->user_handler = user_handler;
    item->handler = (void *)handler;
    item->clientdata = clientdata;
    item->enabled = 0;
    item->next = NULL;
    
    if (ns) {
	item->ns = xmpp_strdup(conn->ctx, ns);
	if (!item->ns) {
	    xmpp_free(conn->ctx, item);
	    return;
	}
    } else
	item->ns = NULL;
    if (name) {
	item->name = xmpp_strdup(conn->ctx, name);
	if (!item->name) {
	    if (item->ns) xmpp_free(conn->ctx, item->ns);
	    xmpp_free(conn->ctx, item);
	    return;
	}
    } else
	item->name = NULL;
    if (type) {
	item->type = xmpp_strdup(conn->ctx, type);
	if (!item->type) {
	    if (item->ns) xmpp_free(conn->ctx, item->ns);
	    if (item->name) xmpp_free(conn->ctx, item->name);
	    xmpp_free(conn->ctx, item);
	}
    } else
	item->type = NULL;

    /* append to list */
    if (!conn->handlers)
	conn->handlers = item;
    else {
	tail = conn->handlers;
	while (tail->next) 
	    tail = tail->next;
	tail->next = item;
    }
}

/** Delete a stanza handler.
 *
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a stanza handler
 *
 *  @ingroup Handlers
 */
void xmpp_handler_delete(xmpp_conn_t * const conn,
			 xmpp_handler handler)
{
    xmpp_handlist_t *prev, *item;

    if (!conn->handlers) return;

    prev = NULL;
    item = conn->handlers;
    while (item) {
	if (item->handler == (void *)handler)
	    break;
	
	prev = item;
	item = item->next;
    }

    if (item) {
	if (prev)
	    prev->next = item->next;
	else
	    conn->handlers = item->next;

	if (item->ns) xmpp_free(conn->ctx, item->ns);
	if (item->name) xmpp_free(conn->ctx, item->name);
	if (item->type) xmpp_free(conn->ctx, item->type);
	xmpp_free(conn->ctx, item);
    }
}

/** Add a timed handler.
 *  The handler will fire for the first time once the period has elapsed,
 *  and continue firing regularly after that.  Strophe will try its best
 *  to fire handlers as close to the period times as it can, but accuracy
 *  will vary depending on the resolution of the event loop.
 *   
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a timed handler
 *  @param period the time in milliseconds between firings
 *  @param clientdata an opaque data pointer that will be passed to the handler
 *
 *  @ingroup Handlers
 */
void xmpp_timed_handler_add(xmpp_conn_t * const conn,
			    xmpp_timed_handler handler,
			    const unsigned long period,
			    void * const clientdata)
{
    _timed_handler_add(conn, handler, period, clientdata, 1);
}

/** Add a timed system handler.
 *  This function is used to add internal timed handlers and should not be
 *  used outside of the library.
 *
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a timed handler
 *  @param period the time in milliseconds between firings
 *  @param clientdata an opaque data pointer that will be passed to the handler
 */
void handler_add_timed(xmpp_conn_t * const conn,
		       xmpp_timed_handler handler,
		       const unsigned long period,
		       void * const clientdata)
{
    _timed_handler_add(conn, handler, period, clientdata, 0);
}

/** Add an id based stanza handler.

 *  This function adds a stanza handler for an &lt;iq/&gt; stanza of
 *  type 'result' or 'error' with a specific id attribute.  This can
 *  be used to handle responses to specific &lt;iq/&gt;s.
 *
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a stanza handler
 *  @param id a string with the id
 *  @param clientdata an opaque data pointer that will be passed to the handler
 *
 *  @ingroup Handlers
 */
void xmpp_id_handler_add(xmpp_conn_t * const conn,
			 xmpp_handler handler,
			 const char * const id,
			 void * const clientdata)
{
    _id_handler_add(conn, handler, id, clientdata, 1);
}

/** Add an id based system stanza handler.
 *  This function is used to add internal id based stanza handlers and should
 *  not be used outside of the library.
 *
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a stanza handler
 *  @param id a string with the id
 *  @param clientdata an opaque data pointer that will be passed to the handler
 */
void handler_add_id(xmpp_conn_t * const conn,
		    xmpp_handler handler,
		    const char * const id,
		    void * const clientdata)
{
    _id_handler_add(conn, handler, id, clientdata, 0);
}

/** Add a stanza handler.
 *  This function is used to add a stanza handler to a connection.
 *  The handler will be called when the any of the filters match.  The
 *  name filter matches to the top level stanza name.  The type filter
 *  matches the 'type' attribute of the top level stanza.  The ns
 *  filter matches the namespace ('xmlns' attribute) of either the top
 *  level stanza or any of it's immediate children (this allows you do
 *  handle specific &lt;iq/&gt; stanzas based on the &lt;query/&gt;
 *  child namespace.
 *
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a stanza handler
 *  @param ns a string with the namespace to match
 *  @param name a string with the stanza name to match
 *  @param type a string with the 'type' attribute to match
 *  @param clientdata an opaque data pointer that will be passed to the handler
 *
 *  @ingroup Handlers
 */
void xmpp_handler_add(xmpp_conn_t * const conn,
		      xmpp_handler handler,
		      const char * const ns,
		      const char * const name,
		      const char * const type,
		      void * const clientdata)
{
    _handler_add(conn, handler, ns, name, type, clientdata, 1);
}

/** Add a system stanza handler.
 *  This function is used to add internal stanza handlers and should
 *  not be used outside of the library.
 *
 *  @param conn a Strophe connection object
 *  @param handler a function pointer to a stanza handler
 *  @param ns a string with the namespace to match
 *  @param name a string with the stanza name to match
 *  @param type a string with the 'type' attribute value to match
 *  @param clientdata an opaque data pointer that will be passed to the handler
 */
void handler_add(xmpp_conn_t * const conn,
		 xmpp_handler handler,
		 const char * const ns,
		 const char * const name,
		 const char * const type,
		 void * const clientdata)
{
    _handler_add(conn, handler, ns, name, type, clientdata, 0);
}
