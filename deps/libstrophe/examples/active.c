/* active.c
** libstrophe XMPP client library -- basic usage example
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

/* This example demonstrates basic handler functions by printing out
** active resources on a jabberd 2.x server.  This program requires
** an admin account on a jabberd 2.x account in order to run.
*/

#include <stdio.h>
#include <string.h>

#include <strophe.h>

int handle_reply(xmpp_conn_t * const conn,
		 xmpp_stanza_t * const stanza,
		 void * const userdata)
{
    xmpp_stanza_t *query, *item;
    char *type;

    type = xmpp_stanza_get_type(stanza);
    if (strcmp(type, "error") == 0)
	fprintf(stderr, "ERROR: query failed\n");
    else {
	query = xmpp_stanza_get_child_by_name(stanza, "query");
	printf("Active Sessions:\n");
	for (item = xmpp_stanza_get_children(query); item; 
	     item = xmpp_stanza_get_next(item))
	    printf("\t %s\n", xmpp_stanza_get_attribute(item, "jid"));
	printf("END OF LIST\n");
    }

    /* disconnect */
    xmpp_disconnect(conn);

    return 0;
}

void conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
		  const int error, xmpp_stream_error_t * const stream_error,
		  void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
    xmpp_stanza_t *iq, *query;

    if (status == XMPP_CONN_CONNECT) {
	fprintf(stderr, "DEBUG: connected\n");
	
	/* create iq stanza for request */
	iq = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(iq, "iq");
	xmpp_stanza_set_type(iq, "get");
	xmpp_stanza_set_id(iq, "active1");
	xmpp_stanza_set_attribute(iq, "to", "xxxxxxxxx.com");

	query = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(query, "query");
	xmpp_stanza_set_ns(query, XMPP_NS_DISCO_ITEMS);
	xmpp_stanza_set_attribute(query, "node", "sessions");

	xmpp_stanza_add_child(iq, query);

	/* we can release the stanza since it belongs to iq now */
	xmpp_stanza_release(query);

	/* set up reply handler */
	xmpp_id_handler_add(conn, handle_reply, "active1", ctx);

	/* send out the stanza */
	xmpp_send(conn, iq);

	/* release the stanza */
	xmpp_stanza_release(iq);
    } else {
	fprintf(stderr, "DEBUG: disconnected\n");
	xmpp_stop(ctx);
    }
}

int main(int argc, char **argv)
{
    xmpp_ctx_t *ctx;
    xmpp_conn_t *conn;
    xmpp_log_t *log;

    //if (argc != 3) {
	//fprintf(stderr, "Usage: active <jid> <pass>\n\n");
	//return 1;
    //}

    /* initialize lib */
    xmpp_initialize();

    /* create a context */

    /* create a context */
    log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* pass NULL instead to silence output */
    ctx = xmpp_ctx_new(NULL, log);

    /* create a connection */
    conn = xmpp_conn_new(ctx);

    /* setup authentication information */
    //xmpp_conn_set_jid(conn, argv[1]);
    //xmpp_conn_set_pass(conn, argv[2]);
    xmpp_conn_set_jid(conn, "admin@gorillas-e376e6");
    xmpp_conn_set_pass(conn, "tigase");

	
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
 *  @param userdata an opaque data pointer that will be passed to the callback
 *
 *  @return 0 on success and -1 on an error
 *
 *  @ingroup Connections
int xmpp_connect_client(xmpp_conn_t * const conn, 
			  const char * const altdomain,
			  unsigned short altport,
			  xmpp_conn_handler callback,
			  void * const userdata)
 */



    /* initiate connection */
    xmpp_connect_client(conn, "localhost", 5222, conn_handler, ctx);

    /* start the event loop */
    xmpp_run(ctx);

    /* release our connection and context */
    xmpp_conn_release(conn);
    xmpp_ctx_free(ctx);

    /* shutdown lib */
    xmpp_shutdown();

    return 0;
}
