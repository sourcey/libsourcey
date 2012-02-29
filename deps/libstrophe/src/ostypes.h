/* ostypes.h
** strophe XMPP client library -- type definitions for platforms 
**     without stdint.h
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
 *  Type definitions for platforms without stdint.h.
 */

#ifndef __LIBSTROPHE_OSTYPES_H__
#define __LIBSTROPHE_OSTYPES_H__

#ifdef _WIN32
typedef unsigned __int64 uint64_t; 
#endif

#endif /* __LIBSTROPHE_OSTYPES_H__ */
