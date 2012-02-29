/* test_hash.c
** libstrophe XMPP client library -- self-test for the hash-table implementation
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

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "strophe.h"
#include "common.h"
#include "hash.h"

#define TABLESIZE 100
#define TESTSIZE 500

/* static test data */
const int nkeys = 5;
const char *keys[] = {
  "foo", "bar", "baz", "quux", "xyzzy"
};
const char *values[] = {
  "wuzzle", "mug", "canonical", "rosebud", "lottery"
};

int main(int argc, char **argv)
{
    xmpp_ctx_t *ctx;
    hash_t *table, *clone;
    hash_iterator_t *iter;
    unsigned int seed;
    const char *key;
    char *result;
    int err = 0;
    int i;

    /* initialize random numbers */
    if (argc > 2) {
	/* use a seed from the command line */
	seed = (unsigned int)atoi(argv[1]);
    } else {
	seed = (unsigned int)clock();
    }
    /* using random seed 'seed' */
    srand(seed);

    /* allocate a default context */
    ctx = xmpp_ctx_new(NULL, NULL);
    if (ctx == NULL) {
	/* ctx allocation failed! */
	return -1;
    }

    /* allocate a hash table */
    table = hash_new(ctx, TABLESIZE, NULL);
    if (table == NULL) {
	/* table allocation failed! */
	return 1;
    }

    /* test insertion */
    for (i = 0; i < nkeys; i++) {
	err = hash_add(table, keys[i], (void*)values[i]);
	if (err) return err;
    }

    /* test key count */
    if (hash_num_keys(table) != nkeys) {
	/* wrong number of keys in table! */
	return 1;
    }

    /* test cloning */
    clone = hash_clone(table);

    /* test lookup */
    for (i = 0; i < nkeys; i++) {
	result = hash_get(clone, keys[i]);
	if (result == NULL) {
	    /* lookup failed! */
	    return 1;
	}
	if (strcmp(values[i], result)) {
	    /* lookup returned incorrect value! */
	    return 1;
	}
    }

    /* test key iterator */
    iter = hash_iter_new(clone);
    if (iter == NULL) {
	/* iterator allocation failed! */
	return 1;
    }
    for (i = 0; i < nkeys; i++) {
	key = hash_iter_next(iter);
	printf("key: '%s'\n", key);
    }
    key = hash_iter_next(iter);
    if (key != NULL) {
	/* extra keys returned! */
	return 1;
    }
    key = hash_iter_next(iter);
    if (key != NULL) {
	/* extra keys returned! */
	return 1;
    }
    hash_iter_release(iter);

    /* release the hash table */
    hash_release(table);

    /* test drops */
    hash_drop(clone, keys[2]);
    if (hash_get(clone, keys[2]) != NULL) return 1;
    hash_drop(clone, keys[1]);
    hash_drop(clone, keys[4]);
    if (hash_get(clone, keys[4]) != NULL) return 1;
    if (hash_get(clone, keys[1]) != NULL) return 1;
    /* keys 0,3 should still be available */
    if (hash_get(clone, keys[0]) == NULL) return 1;
    if (hash_get(clone, keys[3]) == NULL) return 1;

    /* release our clone */
    hash_release(clone);

    /* release our library context */
    xmpp_ctx_free(ctx);

    return err;
}
