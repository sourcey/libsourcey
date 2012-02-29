/* test_base64.c
** libstrophe XMPP client library -- test routines for the jid utils
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
#include <string.h>

#include "strophe.h"
#include "common.h"

static const char jid1[] = "foo@bar.com";
static const char jid2[] = "anyone@example.com/hullo";
static const char jid3[] = "manic.porter@xyz.net/frob";
static const char jid4[] = "domain.tld";

int test_jid(xmpp_ctx_t *ctx)
{
    char *node;
    char *domain;
    char *resource;

    node = xmpp_jid_node(ctx, jid1);
    domain = xmpp_jid_domain(ctx, jid1);
    resource = xmpp_jid_resource(ctx, jid1);
    printf("jid '%s' parsed to %s, %s, %s\n",
	jid1, node, domain, resource);
    if (strcmp(node, "foo")) return 1;
    if (strcmp(domain, "bar.com")) return 1;
    if (resource != NULL) return 1;
    if (node) xmpp_free(ctx, node);
    if (domain) xmpp_free(ctx, domain);
    if (resource) xmpp_free(ctx, resource);

    node = xmpp_jid_node(ctx, jid2);
    domain = xmpp_jid_domain(ctx, jid2);
    resource = xmpp_jid_resource(ctx, jid2);
    printf("jid '%s' parsed to %s, %s, %s\n",
	jid2, node, domain, resource);
    if (strcmp(node, "anyone")) return 1;
    if (strcmp(domain, "example.com")) return 1;
    if (strcmp(resource, "hullo")) return 1;
    if (node) xmpp_free(ctx, node);
    if (domain) xmpp_free(ctx, domain);
    if (resource) xmpp_free(ctx, resource);

    node = xmpp_jid_node(ctx, jid3);
    domain = xmpp_jid_domain(ctx, jid3);
    resource = xmpp_jid_resource(ctx, jid3);
    printf("jid '%s' parsed to %s, %s, %s\n",
	jid3, node, domain, resource);
    if (strcmp(node, "manic.porter")) return 1;
    if (strcmp(domain, "xyz.net")) return 1;
    if (strcmp(resource, "frob")) return 1;
    if (node) xmpp_free(ctx, node);
    if (domain) xmpp_free(ctx, domain);
    if (resource) xmpp_free(ctx, resource);

    node = xmpp_jid_node(ctx, jid4);
    domain = xmpp_jid_domain(ctx, jid4);
    resource = xmpp_jid_resource(ctx, jid4);
    printf("jid '%s' parsed to %s, %s, %s\n",
	jid4, node, domain, resource);
    if (node != NULL) return 1;
    if (strcmp(domain, "domain.tld")) return 1;
    if (resource != NULL) return 1;
    if (node) xmpp_free(ctx, node);
    if (domain) xmpp_free(ctx, domain);
    if (resource) xmpp_free(ctx, resource);

    return 0;
}

int test_jid_new(xmpp_ctx_t *ctx)
{
    char *jid;

    jid = xmpp_jid_new(ctx, "node", "domain", "resource");
    printf("new jid: '%s'\n", jid);
    if (strcmp(jid, "node@domain/resource")) return 1;
    xmpp_free(ctx, jid);

    jid = xmpp_jid_new(ctx, "foo", "bar.com", NULL);
    printf("new jid: '%s'\n", jid);
    if (strcmp(jid, "foo@bar.com")) return 1;
    xmpp_free(ctx, jid);

    return 0;
}

int main(int argc, char *argv[])
{
    xmpp_ctx_t *ctx;
    int ret;

    printf("allocating context... ");   
    ctx = xmpp_ctx_new(NULL, NULL);
    if (ctx == NULL) printf("failed to create context\n");
    if (ctx == NULL) return -1;
    printf("ok.\n");

    printf("testing jid routines... ");   
    ret = test_jid(ctx);
    if (ret) printf("failed!\n");
    if (ret) return ret;
    printf("ok.\n");

    printf("testing jid new routines... ");   
    ret = test_jid_new(ctx);
    if (ret) printf("failed!\n");
    if (ret) return ret;
    printf("ok.\n");

    printf("freeing context... ");
    xmpp_ctx_free(ctx);
    printf("ok.\n");

    return ret;
}
