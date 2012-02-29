/* hash.h
** strophe XMPP client library -- hash table interface
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
 *  Hash table API.
 */

#ifndef __LIBSTROPHE_HASH_H__
#define __LIBSTROPHE_HASH_H__

typedef struct _hash_t hash_t;

typedef void (*hash_free_func)(const xmpp_ctx_t * const ctx, void *p);

/** allocate and initialize a new hash table */
hash_t *hash_new(xmpp_ctx_t * const ctx, const int size,
		 hash_free_func free);

/** allocate a new reference to an existing hash table */
hash_t *hash_clone(hash_t * const table);

/** release a hash table when no longer needed */
void hash_release(hash_t * const table);

/** add a key, value pair to a hash table.
 *  each key can appear only once; the value of any
 *  identical key will be replaced
 */
int hash_add(hash_t *table, const char * const key, void *data);

/** look up a key in a hash table */
void *hash_get(hash_t *table, const char *key);

/** delete a key from a hash table */
int hash_drop(hash_t *table, const char *key);

/** return the number of keys in a hash */
int hash_num_keys(hash_t *table);

/** hash key iterator functions */
typedef struct _hash_iterator_t hash_iterator_t;

/** allocate and initialize a new iterator */
hash_iterator_t *hash_iter_new(hash_t *table);

/** release an iterator that is no longer needed */
void hash_iter_release(hash_iterator_t *iter);

/** return the next hash table key from the iterator.
    the returned key should not be freed */
const char * hash_iter_next(hash_iterator_t *iter);

#endif /* __LIBXMPPP_HASH_H__ */
