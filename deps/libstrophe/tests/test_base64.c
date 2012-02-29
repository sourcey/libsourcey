/* test_base64.c
** libstrophe XMPP client library -- test routines for the base64 codec
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
#include "sasl.h"

static const unsigned char text_1[] = "How now brown cow?";
static const char base64_1[] = "SG93IG5vdyBicm93biBjb3c/";
static const unsigned char text_2[] = {0x14,0xfb,0x9c,0x03,0xd9,0x7e,0x00};
static const char base64_2[] = "FPucA9l+";
static const unsigned char text_3[] = 
    "From rest and sleep, which but thy pictures be, "
    "Much pleasure; then from thee much more must flow, "
    "And soonest our best men with thee do go, "
    "Rest of their bones, and soul's delivery.";
static const char base64_3[] =
    "RnJvbSByZXN0IGFuZCBzbGVlcCwgd2hpY2ggYnV0IHRoeSBwaWN0dXJl"
    "cyBiZSwgTXVjaCBwbGVhc3VyZTsgdGhlbiBmcm9tIHRoZWUgbXVjaCBt"
    "b3JlIG11c3QgZmxvdywgQW5kIHNvb25lc3Qgb3VyIGJlc3QgbWVuIHdp"
    "dGggdGhlZSBkbyBnbywgUmVzdCBvZiB0aGVpciBib25lcywgYW5kIHNv"
    "dWwncyBkZWxpdmVyeS4=";
static const unsigned char text_4[] = 
    {0xd6, 0x2f, 0x27, 0x49,  0x7e, 0xdd, 0xf3, 0xd5, 
     0x41, 0xbc, 0x1b, 0xe9,  0xdf, 0xe9, 0xb3, 0x08, 0x00};
static const char base64_4[] = "1i8nSX7d89VBvBvp3+mzCA==";
static const char text_5[] = 
    "realm=\"chesspark.com\",nonce=\"b243c0d663257a9149999cef2f83a22116559e93\",qop=\"auth\",charset=utf-8,algorithm=md5-sess";
static const char base64_5[] = 
    "cmVhbG09ImNoZXNzcGFyay5jb20iLG5vbmNlPSJiMjQzYzBkNjYzMjU3"
    "YTkxNDk5OTljZWYyZjgzYTIyMTE2NTU5ZTkzIixxb3A9ImF1dGgiLGNo"
    "YXJzZXQ9dXRmLTgsYWxnb3JpdGhtPW1kNS1zZXNz";

int test_encode(xmpp_ctx_t *ctx)
{
    char *result;
    result = base64_encode(ctx, text_1, strlen(text_1));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(base64_1)) return 1;
    if (strncmp(base64_1, result, strlen(base64_1))) return 1;
    xmpp_free(ctx,result);

    result = base64_encode(ctx, text_2, strlen(text_2));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(base64_2)) return 1;
    if (strncmp(base64_2, result, strlen(base64_2))) return 1;
    xmpp_free(ctx,result);

    result = base64_encode(ctx, text_3, strlen(text_3));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(base64_3)) return 1;
    if (strncmp(base64_3, result, strlen(base64_3))) return 1;
    xmpp_free(ctx,result);

    result = base64_encode(ctx, text_4, strlen(text_4));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(base64_4)) return 1;
    if (strncmp(base64_4, result, strlen(base64_4))) return 1;
    xmpp_free(ctx,result);

    result = base64_encode(ctx, text_5, strlen(text_5));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(base64_5)) return 1;
    if (strncmp(base64_5, result, strlen(base64_5))) return 1;
    xmpp_free(ctx,result);

    return 0;
}


int test_decode(xmpp_ctx_t *ctx)
{
    unsigned char *result;

    result = base64_decode(ctx, base64_1, strlen(base64_1));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(text_1)) return 1;
    if (strncmp(text_1, result, strlen(text_1))) return 1;
    xmpp_free(ctx,result);

    result = base64_decode(ctx, base64_2, strlen(base64_2));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(text_2)) return 1;
    if (strncmp(text_2, result, strlen(text_2))) return 1;
    xmpp_free(ctx,result);

    result = base64_decode(ctx, base64_3, strlen(base64_3));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(text_3)) return 1;
    if (strncmp(text_3, result, strlen(text_3))) return 1;
    xmpp_free(ctx,result);

    result = base64_decode(ctx, base64_4, strlen(base64_4));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(text_4)) return 1;
    if (strncmp(text_4, result, strlen(text_4))) return 1;
    xmpp_free(ctx,result);

    result = base64_decode(ctx, base64_5, strlen(base64_5));
    if (result == NULL) return 2;
    if (strlen(result) != strlen(text_5)) return 1;
    if (strncmp(text_5, result, strlen(text_5))) return 1;
    xmpp_free(ctx,result);

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

    printf("testing encode... ");   
    ret = test_encode(ctx);
    if (ret) printf("base64_encode failed!\n");
    if (ret) return ret;
    printf("ok.\n");

    printf("testing decode... ");   
    ret = test_decode(ctx);
    if (ret) printf("base64_decode failed!\n");
    if (ret) return ret;
    printf("ok.\n");

    printf("freeing context... ");
    xmpp_ctx_free(ctx);
    printf("ok.\n");

    return ret;
}
