/* parser.c
** strophe XMPP client library -- xml parser handlers and utility functions
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
 *  XML parser handlers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <strophe.h>
#include "common.h"
#include "parser.h"

struct _parser_t {
    xmpp_ctx_t *ctx;
    xmlParserCtxtPtr xmlctx;
    xmlSAXHandler handlers;
    parser_start_callback startcb;
    parser_end_callback endcb;
    parser_stanza_callback stanzacb;
    void *userdata;
    int depth;
    xmpp_stanza_t *stanza;
};

static void _set_attributes(xmpp_stanza_t *stanza, const xmlChar **attrs)
{
    int i;

    if (!attrs) return;

    for (i = 0; attrs[i]; i += 2) {
        xmpp_stanza_set_attribute(stanza, attrs[i], attrs[i+1]);
    }
}

static void _start_element(void *userdata, 
                           const xmlChar *name, const xmlChar **attrs)
{
    parser_t *parser = (parser_t *)userdata;
    xmpp_stanza_t *child;

    if (parser->depth == 0) {
        /* notify the owner */
        if (parser->startcb)
            parser->startcb((char *)name, (char **)attrs, 
                            parser->userdata);
    } else {
	/* build stanzas at depth 1 */
	if (!parser->stanza && parser->depth != 1) {
	    /* something terrible happened */
	    /* FIXME: we should probably trigger a disconnect */
	    xmpp_error(parser->ctx, "parser", "oops, where did our stanza go?");
	} else if (!parser->stanza) {
	    /* starting a new toplevel stanza */
	    parser->stanza = xmpp_stanza_new(parser->ctx);
	    if (!parser->stanza) {
		/* FIXME: can't allocate, disconnect */
	    }
	    xmpp_stanza_set_name(parser->stanza, (char *)name);
	    _set_attributes(parser->stanza, attrs);
	} else {
	    /* starting a child of conn->stanza */
	    child = xmpp_stanza_new(parser->ctx);
	    if (!child) {
		/* FIXME: can't allocate, disconnect */
	    }
	    xmpp_stanza_set_name(child, (char *)name);
	    _set_attributes(child, attrs);

	    /* add child to parent */
	    xmpp_stanza_add_child(parser->stanza, child);
	    
	    /* the child is owned by the toplevel stanza now */
	    xmpp_stanza_release(child);

	    /* make child the current stanza */
	    parser->stanza = child;
	}
    }

    parser->depth++;
}

static void _end_element(void *userdata, const xmlChar *name)
{
    parser_t *parser = (parser_t *)userdata;

    parser->depth--;

    if (parser->depth == 0) {
        /* notify owner */
        if (parser->endcb)
            parser->endcb((char *)name, parser->userdata);
    } else {
	if (parser->stanza->parent) {
	    /* we're finishing a child stanza, so set current to the parent */
	    parser->stanza = parser->stanza->parent;
	} else {
            if (parser->stanzacb)
                parser->stanzacb(parser->stanza,
                                 parser->userdata);
            xmpp_stanza_release(parser->stanza);
            parser->stanza = NULL;
	}
    }
}

static void _characters(void *userdata, const xmlChar *chr, int len)
{
    parser_t *parser = (parser_t *)userdata;
    xmpp_stanza_t *stanza;

    /* skip unimportant whitespace, etc */
    if (parser->depth < 2) return;

    /* create and populate stanza */
    stanza = xmpp_stanza_new(parser->ctx);
    if (!stanza) {
	/* FIXME: allocation error, disconnect */
	return;
    }
    xmpp_stanza_set_text_with_size(stanza, (char *)chr, len);

    xmpp_stanza_add_child(parser->stanza, stanza);
    xmpp_stanza_release(stanza);
}

/* create a new parser */
parser_t *parser_new(xmpp_ctx_t *ctx,
                     parser_start_callback startcb,
                     parser_end_callback endcb,
                     parser_stanza_callback stanzacb,
                     void *userdata)
{
    parser_t *parser;

    parser = xmpp_alloc(ctx, sizeof(parser_t));
    if (parser != NULL) {
        parser->ctx = ctx;
        parser->xmlctx = NULL;
        memset(&parser->handlers, 0, sizeof(xmlSAXHandler));
        parser->handlers.startElement = _start_element;
        parser->handlers.endElement = _end_element;
        parser->handlers.characters = _characters;
        parser->startcb = startcb;
        parser->endcb = endcb;
        parser->stanzacb = stanzacb;
        parser->userdata = userdata;
        parser->depth = 0;
        parser->stanza = NULL;

        parser_reset(parser);
    }

    return parser;
}

/* free a parser */
void parser_free(parser_t *parser)
{
    if (parser->xmlctx)
        xmlFreeParserCtxt(parser->xmlctx);
    xmpp_free(parser->ctx, parser);
}

/* shuts down and restarts XML parser.  true on success */
int parser_reset(parser_t *parser)
{
    if (parser->xmlctx)
        xmlFreeParserCtxt(parser->xmlctx);

    if (parser->stanza) 
	xmpp_stanza_release(parser->stanza);

    parser->xmlctx = xmlCreatePushParserCtxt(&parser->handlers, 
                                             parser, NULL, 0, NULL);
    if (!parser->xmlctx) return 0;

    parser->depth = 0;
    parser->stanza = NULL;

    return 1;
}

/* feed a chunk of data to the parser */
int parser_feed(parser_t *parser, char *chunk, int len)
{
     /* xmlParseChunk API returns 0 on success which is opposite logic to
       the status returned by parser_feed */
    if(!xmlParseChunk(parser->xmlctx, chunk, len, 0)) {
        return 1;
    } else {
        return 0;
    }
}
