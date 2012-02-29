/* strophe.h
** strophe XMPP client library C API
**
** Copyright (C) 2005-2009 Collecta, Inc.
**
**  This software is provided AS-IS with no warranty, either express or
**  implied.
**
**  This software is distributed under license and may not be copied,
**  modified or distributed except as expressly authorized under the
**  terms of the license contained in the file LICENSE.txt in this
**  distribution.
*/

/** @file
 *  Strophe public C API definitions.
 */

#ifndef __LIBSTROPHE_STROPHE_H__
#define __LIBSTROPHE_STROPHE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* namespace defines */
/** @def XMPP_NS_CLIENT
 *  Namespace definition for 'jabber:client'.
 */
#define XMPP_NS_CLIENT "jabber:client"
/** @def XMPP_NS_COMPONENT
 *  Namespace definition for 'jabber:component:accept'.
 */
#define XMPP_NS_COMPONENT "jabber:component:accept"
/** @def XMPP_NS_STREAMS
 *  Namespace definition for 'http://etherx.jabber.org/streams'.
 */
#define XMPP_NS_STREAMS "http://etherx.jabber.org/streams"
/** @def XMPP_NS_STREAMS_IETF
 *  Namespace definition for 'urn:ietf:params:xml:ns:xmpp-streams'.
 */
#define XMPP_NS_STREAMS_IETF "urn:ietf:params:xml:ns:xmpp-streams"
/** @def XMPP_NS_TLS
 *  Namespace definition for 'url:ietf:params:xml:ns:xmpp-tls'.
 */
#define XMPP_NS_TLS "urn:ietf:params:xml:ns:xmpp-tls"
/** @def XMPP_NS_SASL
 *  Namespace definition for 'urn:ietf:params:xml:ns:xmpp-sasl'.
 */
#define XMPP_NS_SASL "urn:ietf:params:xml:ns:xmpp-sasl"
/** @def XMPP_NS_BIND
 *  Namespace definition for 'urn:ietf:params:xml:ns:xmpp-bind'.
 */
#define XMPP_NS_BIND "urn:ietf:params:xml:ns:xmpp-bind"
/** @def XMPP_NS_SESSION
 *  Namespace definition for 'urn:ietf:params:xml:ns:xmpp-session'.
 */
#define XMPP_NS_SESSION "urn:ietf:params:xml:ns:xmpp-session"
/** @def XMPP_NS_AUTH
 *  Namespace definition for 'jabber:iq:auth'.
 */
#define XMPP_NS_AUTH "jabber:iq:auth"
/** @def XMPP_NS_DISCO_INFO
 *  Namespace definition for 'http://jabber.org/protocol/disco#info'.
 */
#define XMPP_NS_DISCO_INFO "http://jabber.org/protocol/disco#info"
/** @def XMPP_NS_DISCO_ITEMS
 *  Namespace definition for 'http://jabber.org/protocol/disco#items'.
 */
#define XMPP_NS_DISCO_ITEMS "http://jabber.org/protocol/disco#items"
/** @def XMPP_NS_ROSTER
 *  Namespace definition for 'jabber:iq:roster'.
 */
#define XMPP_NS_ROSTER "jabber:iq:roster"

/* error defines */
/** @def XMPP_EOK
 *  Success error code.
 */
#define XMPP_EOK 0
/** @def XMPP_EMEM
 *  Memory related failure error code.
 *  
 *  This is returned on allocation errors and signals that the host may
 *  be out of memory.
 */
#define XMPP_EMEM -1
/** @def XMPP_EINVOP
 *  Invalid operation error code.
 *
 *  This error code is returned when the operation was invalid and signals
 *  that the Strophe API is being used incorrectly.
 */
#define XMPP_EINVOP -2
/** @def XMPP_EINT
 *  Internal failure error code.
 */
#define XMPP_EINT -3

/* initialization and shutdown */
void xmpp_initialize(void);
void xmpp_shutdown(void);

/* version */
int xmpp_version_check(int major, int minor);

/* run-time contexts */

/* client-replaceable memory allocator */
typedef struct _xmpp_mem_t xmpp_mem_t;

/* client-replaceable log object */
typedef struct _xmpp_log_t xmpp_log_t;

/* opaque run time context containing the above hooks */
typedef struct _xmpp_ctx_t xmpp_ctx_t;

xmpp_ctx_t *xmpp_ctx_new(const xmpp_mem_t * const mem, 
			     const xmpp_log_t * const log);
void xmpp_ctx_free(xmpp_ctx_t * const ctx);

struct _xmpp_mem_t {
    void *(*alloc)(const size_t size, void * const clientdata);
    void (*free)(void *p, void * const clientdata);
    void *(*realloc)(void *p, const size_t size, void * const clientdata);
    void *clientdata;
};

typedef enum {
    XMPP_LEVEL_DEBUG,
    XMPP_LEVEL_INFO,
    XMPP_LEVEL_WARN,
    XMPP_LEVEL_ERROR
} xmpp_log_level_t;

typedef enum {
    XMPP_UNKNOWN,
    XMPP_CLIENT,
    XMPP_COMPONENT
} xmpp_conn_type_t;

typedef void (*xmpp_log_handler)(void * const clientdata, 
				 const xmpp_log_level_t level,
				 const char * const area,
				 const char * const msg);

struct _xmpp_log_t {
    xmpp_log_handler handler;
    void *clientdata;
    /* mutex_t lock; */
};

/* return a default logger filtering at a given level */
xmpp_log_t *xmpp_get_default_logger(xmpp_log_level_t level);

/* connection */

/* opaque connection object */
typedef struct _xmpp_conn_t xmpp_conn_t;
typedef struct _xmpp_stanza_t xmpp_stanza_t;

/* connect callback */
typedef enum {
    XMPP_CONN_CONNECT,
    XMPP_CONN_DISCONNECT,
    XMPP_CONN_FAIL
} xmpp_conn_event_t;

typedef enum {
    XMPP_SE_BAD_FORMAT,
    XMPP_SE_BAD_NS_PREFIX,
    XMPP_SE_CONFLICT,
    XMPP_SE_CONN_TIMEOUT,
    XMPP_SE_HOST_GONE,
    XMPP_SE_HOST_UNKNOWN,
    XMPP_SE_IMPROPER_ADDR,
    XMPP_SE_INTERNAL_SERVER_ERROR,
    XMPP_SE_INVALID_FROM,
    XMPP_SE_INVALID_ID,
    XMPP_SE_INVALID_NS,
    XMPP_SE_INVALID_XML,
    XMPP_SE_NOT_AUTHORIZED,
    XMPP_SE_POLICY_VIOLATION,
    XMPP_SE_REMOTE_CONN_FAILED,
    XMPP_SE_RESOURCE_CONSTRAINT,
    XMPP_SE_RESTRICTED_XML,
    XMPP_SE_SEE_OTHER_HOST,
    XMPP_SE_SYSTEM_SHUTDOWN,
    XMPP_SE_UNDEFINED_CONDITION,
    XMPP_SE_UNSUPPORTED_ENCODING,
    XMPP_SE_UNSUPPORTED_STANZA_TYPE,
    XMPP_SE_UNSUPPORTED_VERSION,
    XMPP_SE_XML_NOT_WELL_FORMED
} xmpp_error_type_t;

typedef struct {
    xmpp_error_type_t type;
    char *text;
    xmpp_stanza_t *stanza;
} xmpp_stream_error_t;

typedef void (*xmpp_conn_handler)(xmpp_conn_t * const conn, 
				  const xmpp_conn_event_t event,
				  const int error,
				  xmpp_stream_error_t * const stream_error,
				  void * const clientdata);

xmpp_conn_t *xmpp_conn_new(xmpp_ctx_t * const ctx);
xmpp_conn_t * xmpp_conn_clone(xmpp_conn_t * const conn);
int xmpp_conn_release(xmpp_conn_t * const conn);

const char *xmpp_conn_get_jid(const xmpp_conn_t * const conn);
const char *xmpp_conn_get_bound_jid(const xmpp_conn_t * const conn);
void xmpp_conn_set_jid(xmpp_conn_t * const conn, const char * const jid);
const char *xmpp_conn_get_pass(const xmpp_conn_t * const conn);
void xmpp_conn_set_pass(xmpp_conn_t * const conn, const char * const pass);
xmpp_ctx_t* xmpp_conn_get_context(xmpp_conn_t * const conn);

int xmpp_connect_client(xmpp_conn_t * const conn, 
			  const char * const altdomain,
			  unsigned short altport,
			  xmpp_conn_handler callback,
			  void * const clientdata);

/*
int xmpp_connect_component(conn, name)
*/
void xmpp_disconnect(xmpp_conn_t * const conn);

void xmpp_send(xmpp_conn_t * const conn,
	       xmpp_stanza_t * const stanza);

void xmpp_send_raw_string(xmpp_conn_t * const conn, 
			  const char * const fmt, ...);
void xmpp_send_raw(xmpp_conn_t * const conn, 
		   const char * const data, const size_t len);


/* handlers */

/* if the handle returns false it is removed */
typedef int (*xmpp_timed_handler)(xmpp_conn_t * const conn, 
				  void * const clientdata);

void xmpp_timed_handler_add(xmpp_conn_t * const conn,
			    xmpp_timed_handler handler,
			    const unsigned long period,
			    void * const clientdata);
void xmpp_timed_handler_delete(xmpp_conn_t * const conn,
			       xmpp_timed_handler handler);


/* if the handler returns false it is removed */
typedef int (*xmpp_handler)(xmpp_conn_t * const conn,
			     xmpp_stanza_t * const stanza,
			     void * const clientdata);

void xmpp_handler_add(xmpp_conn_t * const conn,
		      xmpp_handler handler,
		      const char * const ns,
		      const char * const name,
		      const char * const type,
		      void * const clientdata);
void xmpp_handler_delete(xmpp_conn_t * const conn,
			 xmpp_handler handler);

void xmpp_id_handler_add(xmpp_conn_t * const conn,
			 xmpp_handler handler,
			 const char * const id,
			 void * const clientdata);
void xmpp_id_handler_delete(xmpp_conn_t * const conn,
			    xmpp_handler handler,
			    const char * const id);

/*
void xmpp_register_stanza_handler(conn, stanza, xmlns, type, handler)
*/

/** stanzas **/

/** allocate an initialize a blank stanza */
xmpp_stanza_t *xmpp_stanza_new(xmpp_ctx_t *ctx);

/** clone a stanza */
xmpp_stanza_t *xmpp_stanza_clone(xmpp_stanza_t * const stanza);

/** copies a stanza and all children */
xmpp_stanza_t * xmpp_stanza_copy(const xmpp_stanza_t * const stanza);

/** free a stanza object and it's contents */
int xmpp_stanza_release(xmpp_stanza_t * const stanza);

/** free some blocks returned by other APIs, for example the
    buffer you get from xmpp_stanza_to_text **/
void xmpp_free(const xmpp_ctx_t * const ctx, void *p);

int xmpp_stanza_is_text(xmpp_stanza_t * const stanza);
int xmpp_stanza_is_tag(xmpp_stanza_t * const stanza);

/** marshall a stanza into text for transmission or display **/
int xmpp_stanza_to_text(xmpp_stanza_t *stanza, 
			char ** const buf, size_t * const buflen);

xmpp_stanza_t *xmpp_stanza_get_children(xmpp_stanza_t * const stanza);
xmpp_stanza_t *xmpp_stanza_get_child_by_name(xmpp_stanza_t * const stanza, 
					     const char * const name);
xmpp_stanza_t *xmpp_stanza_get_child_by_ns(xmpp_stanza_t * const stanza,
					   const char * const ns);
xmpp_stanza_t *xmpp_stanza_get_next(xmpp_stanza_t * const stanza);
char *xmpp_stanza_get_attribute(xmpp_stanza_t * const stanza,
				const char * const name);
char * xmpp_stanza_get_ns(xmpp_stanza_t * const stanza);
/* concatenate all child text nodes.  this function
 * returns a string that must be freed by the caller */

char *xmpp_stanza_get_text(xmpp_stanza_t * const stanza);
char *xmpp_stanza_get_text_ptr(xmpp_stanza_t * const stanza);
char *xmpp_stanza_get_name(xmpp_stanza_t * const stanza);

int xmpp_stanza_add_child(xmpp_stanza_t *stanza, xmpp_stanza_t *child);
int xmpp_stanza_set_ns(xmpp_stanza_t * const stanza, const char * const ns);
/* set_attribute adds/replaces attributes */
int xmpp_stanza_set_attribute(xmpp_stanza_t * const stanza, 
			      const char * const key,
			      const char * const value);
int xmpp_stanza_set_name(xmpp_stanza_t *stanza,
			 const char * const name);
int xmpp_stanza_set_text(xmpp_stanza_t *stanza,
			 const char * const text);
int xmpp_stanza_set_text_with_size(xmpp_stanza_t *stanza,
				   const char * const text, 
				   const size_t size);

/* common stanza helpers */
char *xmpp_stanza_get_type(xmpp_stanza_t * const stanza);
char *xmpp_stanza_get_id(xmpp_stanza_t * const stanza);
int xmpp_stanza_set_id(xmpp_stanza_t * const stanza, 
		       const char * const id);
int xmpp_stanza_set_type(xmpp_stanza_t * const stanza, 
			 const char * const type);

/* unimplemented
int xmpp_stanza_set_to();
int xmpp_stanza_set_from();
*/

/* allocate and initialize a stanza in reply to another */
/* unimplemented
xmpp_stanza_t *xmpp_stanza_reply(const xmpp_stanza_t *stanza);
*/

/* stanza subclasses */
/* unimplemented
void xmpp_message_new();
void xmpp_message_get_body();
void xmpp_message_set_body();

void xmpp_iq_new();
void xmpp_presence_new();
*/

/** event loop **/
void xmpp_run_once(xmpp_ctx_t *ctx, const unsigned long  timeout);
void xmpp_run(xmpp_ctx_t *ctx);
void xmpp_stop(xmpp_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* __LIBSTROPHE_STROPHE_H__ */
