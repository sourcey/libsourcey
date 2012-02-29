/* parser.h
** strophe XMPP client library -- parser structures and functions
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
 *  Internally used functions and structures.
 */

#ifndef __LIBSTROPHE_PARSER_H__
#define __LIBSTROPHE_PARSER_H__

#include "strophe.h"

typedef struct _parser_t parser_t;

typedef void (*parser_start_callback)(char *name,
                                      char **attrs,
                                      void * const userdata);
typedef void (*parser_end_callback)(char *name, void * const userdata);
typedef void (*parser_stanza_callback)(xmpp_stanza_t *stanza,
                                       void * const userdata);


parser_t *parser_new(xmpp_ctx_t *ctx, 
                     parser_start_callback startcb,
                     parser_end_callback endcb,
                     parser_stanza_callback stanzacb,
                     void *userdata);
void parser_free(parser_t * const parser);
int parser_reset(parser_t *parser);
int parser_feed(parser_t *parser, char *chunk, int len);

#endif /* __LIBSTROPHE_PARSER_H__ */
