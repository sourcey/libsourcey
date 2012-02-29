/* check_parser.h
** strophe XMPP client library -- parser tests
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

#include <stdlib.h>

#include <check.h>

#include <strophe.h>
#include "parser.h"

#include "test.h"

START_TEST(create_destroy)
{
    xmpp_ctx_t *ctx;
    parser_t *parser;

    ctx = xmpp_ctx_new(NULL, NULL);
    parser = parser_new(ctx, NULL, NULL, NULL, NULL);
    fail_unless(parser != NULL, "Parser creation failed.");
    parser_free(parser);
    xmpp_ctx_free(ctx);
}
END_TEST

int cbtest_got_start = 0;
void cbtest_handle_start(char *name, char **attrs, void *userdata)
{
    if (strcmp(name, "stream:stream") == 0)
        cbtest_got_start = 1;
}

int cbtest_got_end = 0;
void cbtest_handle_end(char *name, void *userdata)
{
    if (strcmp(name, "stream:stream") == 0)
        cbtest_got_end = 1;
}

int cbtest_got_stanza = 0;
void cbtest_handle_stanza(xmpp_stanza_t *stanza, void *userdata)
{
    if (strcmp(xmpp_stanza_get_name(stanza), "message") == 0)
        cbtest_got_stanza = 1;
}

START_TEST(callbacks)
{
    xmpp_ctx_t *ctx;
    parser_t *parser;
    int ret;

    ctx = xmpp_ctx_new(NULL, NULL);
    parser = parser_new(ctx, 
                        cbtest_handle_start, 
                        cbtest_handle_end,
                        cbtest_handle_stanza, NULL);

    ret = parser_feed(parser, "<stream:stream>", 15);
    ret = parser_feed(parser, "<message/>", 10);
    parser_feed(parser, "</stream:stream>", 16);

    fail_unless(cbtest_got_start == 1);
    fail_unless(cbtest_got_end == 1);
    fail_unless(cbtest_got_stanza == 1);

    parser_free(parser);
    xmpp_ctx_free(ctx);
}
END_TEST

Suite *parser_suite(void)
{
    Suite *s = suite_create("Parser");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, create_destroy);
    tcase_add_test(tc_core, callbacks);
    suite_add_tcase(s, tc_core);
    return s;
}

TEST_MAIN
