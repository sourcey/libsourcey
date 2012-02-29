/* util.h
** strophe XMPP client library -- various utility functions
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
 *  Internally used utility functions.
 */

#ifndef __LIBSTROPHE_UTIL_H__
#define __LIBSTROPHE_UTIL_H__

#ifndef _WIN32
#include <stdint.h>
#else
#include "ostypes.h"
#endif

/* timing functions */
uint64_t time_stamp(void);
uint64_t time_elapsed(uint64_t t1, uint64_t t2);

#endif /* __LIBSTROPHE_UTIL_H__ */
