/* stanza.c
** strophe XMPP client library -- XMPP stanza object and utilities
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
 *  Stanza creation and manipulation.
 */

/** @defgroup Stanza Stanza creation and manipulation
 */

#include <stdio.h>
#include <string.h>

#include "strophe.h"
#include "common.h"
#include "hash.h"

#ifdef _WIN32
#define inline __inline
#endif

/** Create a stanza object.
 *  This function allocates and initializes and blank stanza object.
 *  The stanza will have a reference count of one, so the caller does not
 *  need to clone it.
 *
 *  @param ctx a Strophe context object
 *
 *  @return a stanza object
 *
 *  @ingroup Stanza
 */
xmpp_stanza_t *xmpp_stanza_new(xmpp_ctx_t *ctx)
{
    xmpp_stanza_t *stanza;

    stanza = xmpp_alloc(ctx, sizeof(xmpp_stanza_t));
    if (stanza != NULL) {
	stanza->ref = 1;
	stanza->ctx = ctx;
	stanza->type = XMPP_STANZA_UNKNOWN;
	stanza->prev = NULL;
	stanza->next = NULL;
	stanza->children = NULL;
	stanza->parent = NULL;
	stanza->data = NULL;
	stanza->attributes = NULL;
    }

    return stanza; 
}

/** Clone a stanza object.
 *  This function increments the reference count of the stanza object.
 *  
 *  @param stanza a Strophe stanza object
 *
 *  @return the stanza object with it's reference count incremented
 *
 *  @ingroup Stanza
 */
xmpp_stanza_t *xmpp_stanza_clone(xmpp_stanza_t * const stanza)
{
    xmpp_stanza_t *child;

    stanza->ref++;

    return stanza;
}

/** Copy a stanza and its children.
 *  This function copies a stanza along with all its children and returns
 *  the new stanza and children with a reference count of 1.  The returned
 *  stanza will have no parent and no siblings.  This function is useful
 *  for extracting a child stanza for inclusion in another tree.
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return a new Strophe stanza object
 *
 *  @ingroup Stanza
 */
xmpp_stanza_t *xmpp_stanza_copy(const xmpp_stanza_t * const stanza)
{
    xmpp_stanza_t *copy, *child, *copychild, *tail;
    hash_iterator_t *iter;
    const char *key;
    void *val;

    copy = xmpp_stanza_new(stanza->ctx);
    if (!copy) goto copy_error;

    copy->type = stanza->type;

    if (stanza->data) {
	copy->data = xmpp_strdup(stanza->ctx, stanza->data);
	if (!copy->data) goto copy_error;
    }

    if (stanza->attributes) {
	copy->attributes = hash_new(stanza->ctx, 8, xmpp_free);
	if (!copy->attributes) goto copy_error;
	iter = hash_iter_new(stanza->attributes);
	if (!iter) { printf("DEBUG HERE\n"); goto copy_error; }
	while ((key = hash_iter_next(iter))) {
	    val = xmpp_strdup(stanza->ctx,
			      (char *)hash_get(stanza->attributes, key));
	    if (!val) goto copy_error;
	    
	    if (hash_add(copy->attributes, key, val))
		goto copy_error;
	}
	hash_iter_release(iter);
    }

    tail = copy->children;
    for (child = stanza->children; child; child = child->next) {
	copychild = xmpp_stanza_copy(child);
	if (!copychild) goto copy_error;
	copychild->parent = copy;

	if (tail) {
	    copychild->prev = tail;
	    tail->next = copychild;
	} else
	    copy->children = copychild;
	tail = copychild;
    }

    return copy;

copy_error:
    /* release all the hitherto allocated memory */
    if (copy) xmpp_stanza_release(copy);
    return NULL;
}

/** Release a stanza object and all of its children.
 *  This function releases a stanza object and potentially all of its 
 *  children, which may cause the object(s) to be freed.
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return TRUE if the object was freed and FALSE otherwise
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_release(xmpp_stanza_t * const stanza)
{
    int released = 0;
    xmpp_stanza_t *child, *tchild;

    /* release stanza */
    if (stanza->ref > 1)
	stanza->ref--;
    else {
	/* release all children */
	child = stanza->children;
	while (child) {
	    tchild = child;
	    child = child->next;
	    xmpp_stanza_release(tchild);
	}

	if (stanza->attributes) hash_release(stanza->attributes);
	if (stanza->data) xmpp_free(stanza->ctx, stanza->data);
	xmpp_free(stanza->ctx, stanza);
	released = 1;
    }

    return released;
}

/** Determine if a stanza is a text node.
 *  
 *  @param stanza a Strophe stanza object
 *
 *  @return TRUE if the stanza is a text node, FALSE otherwise
 * 
 *  @ingroup Stanza
 */
int xmpp_stanza_is_text(xmpp_stanza_t * const stanza)
{
    return (stanza && stanza->type == XMPP_STANZA_TEXT);
}

/** Determine if a stanza is a tag node.
 *  
 *  @param stanza a Strophe stanza object
 *
 *  @return TRUE if the stanza is a tag node, FALSE otherwise
 * 
 *  @ingroup Stanza
 */
int xmpp_stanza_is_tag(xmpp_stanza_t * const stanza)
{
    return (stanza && stanza->type == XMPP_STANZA_TAG);
}

/* small helper function */
static inline void _render_update(int *written, const int length,
			   const int lastwrite,
			   size_t *left, char **ptr)
{
    *written += lastwrite;

    if (*written > length) {
	*left = 0;
	*ptr = NULL;
    } else {
	*left -= lastwrite;
	*ptr = &(*ptr)[lastwrite];
    }
}

/* always returns number of bytes written or that would have been
 * written if the buffer was large enough
 * return values < 0 indicate some error occured,
 * and return values > buflen indicate buffer was not large enough
 */
static int _render_stanza_recursive(xmpp_stanza_t *stanza,
			     char * const buf, size_t const buflen)
{
    char *ptr = buf;
    size_t left = buflen;
    int ret, written;
    xmpp_stanza_t *child;
    hash_iterator_t *iter;
    const char *key;

    written = 0;

    if (stanza->type == XMPP_STANZA_UNKNOWN) return XMPP_EINVOP;

    if (stanza->type == XMPP_STANZA_TEXT) {
	if (!stanza->data) return XMPP_EINVOP;

	ret = xmpp_snprintf(ptr, left, "%s", stanza->data);
	if (ret < 0) return XMPP_EMEM;
	_render_update(&written, buflen, ret, &left, &ptr);
    } else { /* stanza->type == XMPP_STANZA_TAG */
	if (!stanza->data) return XMPP_EINVOP;

	/* write begining of tag and attributes */
	ret = xmpp_snprintf(ptr, left, "<%s", stanza->data);
	if (ret < 0) return XMPP_EMEM;
	_render_update(&written, buflen, ret, &left, &ptr);

	if (stanza->attributes && hash_num_keys(stanza->attributes) > 0) {
	    iter = hash_iter_new(stanza->attributes);
	    while ((key = hash_iter_next(iter))) {
		ret = xmpp_snprintf(ptr, left, " %s=\"%s\"", key,
			       (char *)hash_get(stanza->attributes, key));
		if (ret < 0) return XMPP_EMEM;
		_render_update(&written, buflen, ret, &left, &ptr);
	    }
	    hash_iter_release(iter);
	}

	if (!stanza->children) {
	    /* write end if singleton tag */
	    ret = xmpp_snprintf(ptr, left, "/>");
	    if (ret < 0) return XMPP_EMEM;
	    _render_update(&written, buflen, ret, &left, &ptr);
	} else {
	    /* this stanza has child stanzas */

	    /* write end of start tag */
	    ret = xmpp_snprintf(ptr, left, ">");
	    if (ret < 0) return XMPP_EMEM;
	    _render_update(&written, buflen, ret, &left, &ptr);
	    
	    /* iterate and recurse over child stanzas */
	    child = stanza->children;
	    while (child) {
		ret = _render_stanza_recursive(child, ptr, left);
		if (ret < 0) return ret;

		_render_update(&written, buflen, ret, &left, &ptr);

		child = child->next;
	    }

	    /* write end tag */
	    ret = xmpp_snprintf(ptr, left, "</%s>", stanza->data);
	    if (ret < 0) return XMPP_EMEM;
	    
	    _render_update(&written, buflen, ret, &left, &ptr);
	}
    }

    return written;
}

/** Render a stanza object to text.
 *  This function renders a given stanza object, along with its
 *  children, to text.  The text is returned in an allocated,
 *  null-terminated buffer.  It starts by allocating a 1024 byte buffer
 *  and reallocates more memory if that is not large enough.
 *
 *  @param stanza a Strophe stanza object
 *  @param buf a reference to a string pointer
 *  @param buflen a reference to a size_t
 *
 *  @return 0 on success (XMPP_EOK), and a number less than 0 on failure
 *      (XMPP_EMEM, XMPP_EINVOP)
 *
 *  @ingroup Stanza
 */
int  xmpp_stanza_to_text(xmpp_stanza_t *stanza,
			 char ** const buf,
			 size_t * const buflen)
{
    char *buffer, *tmp;
    size_t length;
    int ret;

    /* allocate a default sized buffer and attempt to render */
    length = 1024;
    buffer = xmpp_alloc(stanza->ctx, length);
    if (!buffer) {
	*buf = NULL;
	*buflen = 0;
	return XMPP_EMEM;
    }

    ret = _render_stanza_recursive(stanza, buffer, length);
    if (ret < 0) return ret;

    if (ret > length - 1) {
	tmp = xmpp_realloc(stanza->ctx, buffer, ret + 1);
	if (!tmp) {
	    xmpp_free(stanza->ctx, buffer);
	    *buf = NULL;
	    *buflen = 0;
	    return XMPP_EMEM;
	}
	length = ret + 1;
	buffer = tmp;

	ret = _render_stanza_recursive(stanza, buffer, length);
	if (ret > length - 1) return XMPP_EMEM;
    }
    
    buffer[length - 1] = 0;

    *buf = buffer;
    *buflen = ret;

    return XMPP_EOK;
}

/** Set the name of a stanza.
 *  
 *  @param stanza a Strophe stanza object
 *  @param name a string with the name of the stanza
 *
 *  @return XMPP_EOK on success, a number less than 0 on failure (XMPP_EMEM,
 *      XMPP_EINVOP)
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_set_name(xmpp_stanza_t *stanza, 
			 const char * const name)
{
    if (stanza->type == XMPP_STANZA_TEXT) return XMPP_EINVOP;

    if (stanza->data) xmpp_free(stanza->ctx, stanza->data);

    stanza->type = XMPP_STANZA_TAG;
    stanza->data = xmpp_strdup(stanza->ctx, name);

    return XMPP_EOK;
}

/** Get the stanza name.
 *  This function returns a pointer to the stanza name.  If the caller needs
 *  to store this data, it must make a copy.
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return a string with the stanza name
 *
 *  @ingroup Stanza
 */
char *xmpp_stanza_get_name(xmpp_stanza_t * const stanza)
{
    if (stanza->type == XMPP_STANZA_TEXT) return NULL;
    return stanza->data;
}

/** Count the attributes in a stanza object.
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return the number of attributes for the stanza object
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_get_attribute_count(xmpp_stanza_t * const stanza)
{
    if (stanza->attributes == NULL) {
	return 0;
    }

    return hash_num_keys(stanza->attributes);
}

/** Get all attributes for a stanza object.
 *  This function populates the array with attributes from the stanza.  The
 *  attr array will be in the format:  attr[i] = attribute name, 
 *  attr[i+1] = attribute value.
 *
 *  @param stanza a Strophe stanza object
 *  @param attr the string array to populate
 *  @param attrlen the size of the array
 *
 *  @return the number of slots used in the array, which will be 2 times the 
 *      number of attributes in the stanza
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_get_attributes(xmpp_stanza_t * const stanza,
			       const char **attr, int attrlen)
{
    hash_iterator_t *iter;
    const char *key;
    int num = 0;

    if (stanza->attributes == NULL) {
	return 0;
    }

    iter = hash_iter_new(stanza->attributes);
    while ((key = hash_iter_next(iter)) != NULL && attrlen) {
	attr[num++] = key;
	attrlen--;
	if (attrlen == 0) {
	    hash_iter_release(iter);
	    return num;
	}
	attr[num++] = hash_get(stanza->attributes, key);
	attrlen--;
	if (attrlen == 0) {
	    hash_iter_release(iter);
	    return num;
	}
    }

    hash_iter_release(iter);
    return num;
}

/** Set an attribute for a stanza object.
 *  
 *  @param stanza a Strophe stanza object
 *  @param key a string with the attribute name
 *  @param value a string with the attribute value
 *
 *  @return XMPP_EOK (0) on success or a number less than 0 on failure
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_set_attribute(xmpp_stanza_t * const stanza,
			      const char * const key,
			      const char * const value)
{
    char *val;

    if (stanza->type != XMPP_STANZA_TAG) return XMPP_EINVOP;

    if (!stanza->attributes) {
	stanza->attributes = hash_new(stanza->ctx, 8, xmpp_free);
	if (!stanza->attributes) return XMPP_EMEM;
    }

    val = xmpp_strdup(stanza->ctx, value);
    if (!val) return XMPP_EMEM;

    hash_add(stanza->attributes, key, val);

    return XMPP_EOK;
}

/** Set the stanza namespace.
 *  This is a convenience function equivalent to calling:
 *  xmpp_stanza_set_attribute(stanza, "xmlns", ns);
 *
 *  @param stanza a Strophe stanza object
 *  @param ns a string with the namespace
 *
 *  @return XMPP_EOK (0) on success or a number less than 0 on failure
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_set_ns(xmpp_stanza_t * const stanza,
		       const char * const ns)
{
    return xmpp_stanza_set_attribute(stanza, "xmlns", ns);
}

/** Add a child stanza to a stanza object.
 *  This function clones the child and appends it to the stanza object's
 *  children.
 *
 *  @param stanza a Strophe stanza object
 *  @param child the child stanza object
 *
 *  @return XMPP_EOK (0) on success or a number less than 0 on failure
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_add_child(xmpp_stanza_t *stanza, xmpp_stanza_t *child)
{
    xmpp_stanza_t *s;

    /* get a reference to the child */
    xmpp_stanza_clone(child);

    child->parent = stanza;

    if (!stanza->children)
	stanza->children = child;
    else {
	s = stanza->children;
	while (s->next) s = s->next;
	s->next = child;
	child->prev = s;
    }

    return XMPP_EOK;
}

/** Set the text data for a text stanza.
 *  This function copies the text given and sets the stanza object's text to
 *  it.  Attempting to use this function on a stanza that has a name will
 *  fail with XMPP_EINVOP.  This function takes the text as a null-terminated
 *  string.
 *
 *  @param stanza a Strophe stanza object
 *  @param text a string with the text
 *
 *  @return XMPP_EOK (0) on success or a number less than zero on failure
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_set_text(xmpp_stanza_t *stanza,
			 const char * const text)
{
    if (stanza->type == XMPP_STANZA_TAG) return XMPP_EINVOP;
    
    stanza->type = XMPP_STANZA_TEXT;

    if (stanza->data) xmpp_free(stanza->ctx, stanza->data);
    stanza->data = xmpp_strdup(stanza->ctx, text);

    return XMPP_EOK;
}

/** Set the text data for a text stanza.
 *  This function copies the text given and sets teh stanza object's text to
 *  it.  Attempting to use this function on a stanza that has a name will
 *  fail with XMPP_EINVOP.  This function takes the text as buffer and a length
 *  as opposed to a null-terminated string.
 *
 *  @param stanza a Strophe stanza object
 *  @param text a buffer with the text
 *  @param size the length of the text
 *
 *  @return XMPP_EOK (0) on success and a number less than 0 on failure
 * 
 *  @ingroup Stanza
 */
int xmpp_stanza_set_text_with_size(xmpp_stanza_t *stanza,
				   const char * const text,
				   const size_t size)
{
    if (stanza->type == XMPP_STANZA_TAG) return XMPP_EINVOP;

    stanza->type = XMPP_STANZA_TEXT;

    if (stanza->data) xmpp_free(stanza->ctx, stanza->data);
    stanza->data = xmpp_alloc(stanza->ctx, size + 1);
    if (!stanza->data) return XMPP_EMEM;

    memcpy(stanza->data, text, size);
    stanza->data[size] = 0;

    return XMPP_EOK;
}

/** Get the 'id' attribute of the stanza object.
 *  This is a convenience function equivalent to:
 *  xmpp_stanza_get_attribute(stanza, "id");
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return a string with the 'id' attribute value
 *
 *  @ingroup Stanza
 */
char *xmpp_stanza_get_id(xmpp_stanza_t * const stanza)
{
    if (stanza->type != XMPP_STANZA_TAG)
	return NULL;

    if (!stanza->attributes)
	return NULL;

    return (char *)hash_get(stanza->attributes, "id");
}

/** Get the namespace attribute of the stanza object.
 *  This is a convenience function equivalent to:
 *  xmpp_stanza_get_attribute(stanza, "xmlns");
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return a string with the 'xmlns' attribute value
 *
 *  @ingroup Stanza
 */
char *xmpp_stanza_get_ns(xmpp_stanza_t * const stanza)
{
    if (stanza->type != XMPP_STANZA_TAG)
	return NULL;

    if (!stanza->attributes)
	return NULL;

    return (char *)hash_get(stanza->attributes, "xmlns");
}

/** Get the 'type' attribute of the stanza object.
 *  This is a convenience function equivalent to:
 *  xmpp_stanza_get_attribute(stanza, "type");
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return a string with the 'type' attribute value
 *
 *  @ingroup Stanza
 */
char *xmpp_stanza_get_type(xmpp_stanza_t * const stanza)
{
    if (stanza->type != XMPP_STANZA_TAG)
	return NULL;
    
    if (!stanza->attributes)
	return NULL;

    return (char *)hash_get(stanza->attributes, "type");
}

/** Get the first child of stanza with name.
 *  This function searches all the immediate children of stanza for a child
 *  stanza that matches the name.  The first matching child is returned.
 *  
 *  @param stanza a Strophe stanza object
 *  @param name a string with the name to match
 *
 *  @return the matching child stanza object or NULL if no match was found
 *
 *  @ingroup Stanza
 */
xmpp_stanza_t *xmpp_stanza_get_child_by_name(xmpp_stanza_t * const stanza, 
					     const char * const name)
{
    xmpp_stanza_t *child;
    
    for (child = stanza->children; child; child = child->next) {
	if (child->type == XMPP_STANZA_TAG &&
	    (strcmp(name, xmpp_stanza_get_name(child)) == 0))
	    break;
    }

    return child;
}

/** Get the first child of a stanza with a given namespace.
 *  This function searches all the immediate children of a stanza for a child
 *  stanza that matches the namespace provided.  The first matching child
 *  is returned.
 * 
 *  @param stanza a Strophe stanza object
 *  @param ns a string with the namespace to match
 *
 *  @return the matching child stanza object or NULL if no match was found
 *
 *  @ingroup Stanza
 */
xmpp_stanza_t *xmpp_stanza_get_child_by_ns(xmpp_stanza_t * const stanza,
					   const char * const ns)
{
    xmpp_stanza_t *child;

    for (child = stanza->children; child; child = child->next) {
	if (xmpp_stanza_get_ns(child) &&
	    strcmp(ns, xmpp_stanza_get_ns(child)) == 0)
	    break;
    }
    
    return child;
}

/** Get the list of children.
 *  This function returns the first child of the stanza object.  The rest
 *  of the children can be obtained by calling xmpp_stanza_get_next() to
 *  iterate over the siblings.
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return the first child stanza or NULL if there are no children
 *
 *  @ingroup Stanza
 */
xmpp_stanza_t *xmpp_stanza_get_children(xmpp_stanza_t * const stanza) 
{
    return stanza->children;
}

/** Get the next sibling of a stanza.
 *  
 *  @param stanza a Strophe stanza object
 *
 *  @return the next sibling stanza or NULL if there are no more siblings
 *
 *  @ingroup Stanza
 */
xmpp_stanza_t *xmpp_stanza_get_next(xmpp_stanza_t * const stanza)
{
    return stanza->next;
}

/** Get the text data for a text stanza.
 *  This function copies the text data from a stanza and returns the new
 *  allocated string.  The caller is responsible for freeing this string
 *  with xmpp_free().
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return an allocated string with the text data
 *
 *  @ingroup Stanza
 */
char *xmpp_stanza_get_text(xmpp_stanza_t * const stanza)
{
    size_t len, clen;
    xmpp_stanza_t *child;
    char *text;

    if (stanza->type == XMPP_STANZA_TEXT) {
	if (stanza->data)
	    return xmpp_strdup(stanza->ctx, stanza->data);
	else
	    return NULL;
    }

    len = 0;
    for (child = stanza->children; child; child = child->next)
	if (child->type == XMPP_STANZA_TEXT)
	    len += strlen(child->data);

    if (len == 0) return NULL;

    text = (char *)xmpp_alloc(stanza->ctx, len + 1);
    if (!text) return NULL;

    len = 0;
    for (child = stanza->children; child; child = child->next)
	if (child->type == XMPP_STANZA_TEXT) {
	    clen = strlen(child->data);
	    memcpy(&text[len], child->data, clen);
	    len += clen;
	}

    text[len] = 0;

    return text;
}

/** Get the text data pointer for a text stanza.
 *  This function copies returns the raw pointer to the text data in the
 *  stanza.  This should only be used in very special cases where the 
 *  caller needs to translate the datatype as this will save a double
 *  allocation.  The caller should not hold onto this pointer, and is
 *  responsible for allocating a copy if it needs one.
 *
 *  @param stanza a Strophe stanza object
 *
 *  @return an string pointer to the data or NULL
 *
 *  @ingroup Stanza
 */
char *xmpp_stanza_get_text_ptr(xmpp_stanza_t * const stanza)
{
    if (stanza->type == XMPP_STANZA_TEXT)
	return stanza->data;
    return NULL;
}

/** Set the 'id' attribute of a stanza.
 *
 *  This is a convenience function for:
 *  xmpp_stanza_set_attribute(stanza, 'id', id);
 *
 *  @param stanza a Strophe stanza object
 *  @param id a string containing the 'id' value
 *
 *  @return XMPP_EOK (0) on success or a number less than 0 on failure
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_set_id(xmpp_stanza_t * const stanza,
		       const char * const id)
{
    return xmpp_stanza_set_attribute(stanza, "id", id);
}

/** Set the 'type' attribute of a stanza.
 *  This is a convenience function for:
 *  xmpp_stanza_set_attribute(stanza, 'type', type);
 *
 *  @param stanza a Strophe stanza object
 *  @param type a string containing the 'type' value
 *
 *  @return XMPP_EOK (0) on success or a number less than 0 on failure
 *
 *  @ingroup Stanza
 */
int xmpp_stanza_set_type(xmpp_stanza_t * const stanza,
			 const char * const type)
{
    return xmpp_stanza_set_attribute(stanza, "type", type);
}

/** Get an attribute from a stanza.
 *  This function returns a pointer to the attribute value.  If the caller
 *  wishes to save this value it must make its own copy.
 *
 *  @param stanza a Strophe stanza object
 *  @param name a string containing attribute name
 *
 *  @return a string with the attribute value or NULL on an error
 *
 *  @ingroup Stanza
 */
char *xmpp_stanza_get_attribute(xmpp_stanza_t * const stanza,
				const char * const name)
{
    if (stanza->type != XMPP_STANZA_TAG)
	return NULL;
    
    if (!stanza->attributes)
	return NULL;

    return hash_get(stanza->attributes, name);
}
