/* test_ctx.c
** libstrophe XMPP client library -- test routines for the library run-time context
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strophe.h"
#include "common.h"

static int log_called = 0;
static int mem_alloc_called = 0;
static int mem_free_called = 0;
static int mem_realloc_called = 0;

void *my_alloc(const size_t size, void * const userdata)
{
    mem_alloc_called++;
    return malloc(size);
}

void my_free(void *p, void * const userdata)
{
    mem_free_called++;
    return free(p);
}

void *my_realloc(void *p, const size_t size, void * const userdata)
{
    mem_realloc_called++;
    return realloc(p, size);
}

void my_logger(void * const userdata, const xmpp_log_level_t level,
	       const char * const area, const char * const msg)
{
    if (strcmp((char *)userdata, "asdf") == 0 && level == XMPP_LEVEL_DEBUG
	&& strcmp(area, "test") == 0 && strcmp(msg, "hello") == 0)
	log_called++;
}

int main(int argc, char **argv)
{
    xmpp_ctx_t *ctx;
    xmpp_mem_t mymem;
    xmpp_log_t mylog;
    char my_str[5] = "asdf";
    void *testptr1, *testptr2;

    ctx = xmpp_ctx_new(NULL, NULL);
    if (ctx == NULL) return 1;

    /* destroy context */
    xmpp_ctx_free(ctx);

    /* setup our memory handler */
    mymem.alloc = my_alloc;
    mymem.free = my_free;
    mymem.realloc = my_realloc;

    /* setup our logger */
    mylog.handler = my_logger;
    mylog.userdata = my_str;

    ctx = xmpp_ctx_new(&mymem, &mylog);
    xmpp_debug(ctx, "test", "hello");

    testptr1 = xmpp_alloc(ctx, 1024);
    if (testptr1 == NULL) {
	xmpp_ctx_free(ctx);
	return 1;
    }

    testptr2 = xmpp_realloc(ctx, testptr1, 2048);
    if (testptr2 == NULL) {
	xmpp_free(ctx, testptr1);
	xmpp_ctx_free(ctx);
	return 1;
    }

    xmpp_free(ctx, testptr2);

    xmpp_ctx_free(ctx);

    /* check for test failure */
    if (!(log_called && mem_alloc_called && mem_realloc_called && 
	  mem_free_called))
	return 1;
    
    return 0;
}
