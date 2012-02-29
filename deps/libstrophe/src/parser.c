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

#include <expat.h>

#include <strophe.h>
#include "common.h"
#include "parser.h"

struct _parser_t {
    xmpp_ctx_t *ctx;
    XML_Parser expat;
    parser_start_callback startcb;
    parser_end_callback endcb;
    parser_stanza_callback stanzacb;
    void *clientdata;
    int depth;
    xmpp_stanza_t *stanza;
};

static void _set_attributes(xmpp_stanza_t *stanza, const XML_Char **attrs)
{
    int i;

    if (!attrs) return;

    for (i = 0; attrs[i]; i += 2) {
        xmpp_stanza_set_attribute(stanza, attrs[i], attrs[i+1]);
    }
}

static void _start_element(void *clientdata,
                           const XML_Char *name,
                           const XML_Char **attrs)
{
    parser_t *parser = (parser_t *)clientdata;
    xmpp_stanza_t *child;

    if (parser->depth == 0) {
        /* notify the owner */
        if (parser->startcb)
            parser->startcb((char *)name, (char **)attrs, 
                            parser->clientdata);
    } else {
	/* build stanzas at depth 1 */
	if (!parser->stanza && parser->depth != 1) {
	    /* something terrible happened */
	    /* FIXME: shutdown disconnect */
	    xmpp_error(parser->ctx, "parser", "oops, where did our stanza go?");
	} else if (!parser->stanza) {
	    /* starting a new toplevel stanza */
	    parser->stanza = xmpp_stanza_new(parser->ctx);
	    if (!parser->stanza) {
		/* FIXME: can't allocate, disconnect */
	    }
	    xmpp_stanza_set_name(parser->stanza, name);
	    _set_attributes(parser->stanza, attrs);
	} else {
	    /* starting a child of parser->stanza */
	    child = xmpp_stanza_new(parser->ctx);
	    if (!child) {
		/* FIXME: can't allocate, disconnect */
	    }
	    xmpp_stanza_set_name(child, name);
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

static void _end_element(void *clientdata, const XML_Char *name)
{
    parser_t *parser = (parser_t *)clientdata;

    parser->depth--;

    if (parser->depth == 0) {
        /* notify the owner */
        if (parser->endcb)
            parser->endcb((char *)name, parser->clientdata);
    } else {
	if (parser->stanza->parent) {
	    /* we're finishing a child stanza, so set current to the parent */
	    parser->stanza = parser->stanza->parent;
	} else {
            if (parser->stanzacb)
                parser->stanzacb(parser->stanza,
                                 parser->clientdata);
	    xmpp_stanza_release(parser->stanza);
	    parser->stanza = NULL;
	}
    }
}

static void _characters(void *clientdata, const XML_Char *s, int len)
{
    parser_t *parser = (parser_t *)clientdata;
    xmpp_stanza_t *stanza;

    if (parser->depth < 2) return;

    /* create and populate stanza */
    stanza = xmpp_stanza_new(parser->ctx);
    if (!stanza) {
	/* FIXME: allocation error, disconnect */
	return;
    }
    xmpp_stanza_set_text_with_size(stanza, s, len);

    xmpp_stanza_add_child(parser->stanza, stanza);
    xmpp_stanza_release(stanza);
}

parser_t *parser_new(xmpp_ctx_t *ctx,
                     parser_start_callback startcb,
                     parser_end_callback endcb,
                     parser_stanza_callback stanzacb,
                     void *clientdata)
{
    parser_t *parser;

    parser = xmpp_alloc(ctx, sizeof(parser_t));
    if (parser != NULL) {
        parser->ctx = ctx;
        parser->expat = NULL;
        parser->startcb = startcb;
        parser->endcb = endcb;
        parser->stanzacb = stanzacb;
        parser->clientdata = clientdata;
        parser->depth = 0;
        parser->stanza = NULL;

        parser_reset(parser);
    }

    return parser;
}

/* free a parser */
void parser_free(parser_t *parser)
{
    if (parser->expat)
        XML_ParserFree(parser->expat);

    xmpp_free(parser->ctx, parser);
}

/* shuts down and restarts XML parser.  true on success */
int parser_reset(parser_t *parser)
{
    if (parser->expat)
	XML_ParserFree(parser->expat);

    if (parser->stanza) 
	xmpp_stanza_release(parser->stanza);

    parser->expat = XML_ParserCreate(NULL);
    if (!parser->expat) return 0;

    parser->depth = 0;
    parser->stanza = NULL;

    XML_SetUserData(parser->expat, parser);
    XML_SetElementHandler(parser->expat, _start_element, _end_element);
    XML_SetCharacterDataHandler(parser->expat, _characters);

    return 1;
}


int parser_feed(parser_t *parser, char *chunk, int len)
{
    return XML_Parse(parser->expat, chunk, len, 0);
}
