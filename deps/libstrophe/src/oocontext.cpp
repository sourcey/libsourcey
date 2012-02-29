/* oocontext.cpp
** strophe XMPP client library -- C++ context implementation
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

#include "strophe.h"
#include "strophepp.h"

XMPP::Context::Context() 
{
    m_mem.alloc = callAlloc;
    m_mem.realloc = callRealloc;
    m_mem.free = callFree;
    m_mem.userdata = (void *)this;

    m_log.handler = callLog;
    m_log.userdata = (void *)this;

    m_ctx = ::xmpp_ctx_new(&m_mem, &m_log);
}

XMPP::Context::~Context()
{
    ::xmpp_ctx_free(m_ctx);
}

void *XMPP::Context::alloc(const size_t size)
{
    return ::malloc(size);
}

void *XMPP::Context::realloc(void *p, const size_t size)
{
    return ::realloc(p, size);
}

void XMPP::Context::free(void *p)
{
    ::free(p);
}

void XMPP::Context::log(const xmpp_log_level_t level,
			const char * const area,
			const char * const msg)
{
    /* do nothing by default */
}

xmpp_ctx_t *XMPP::Context::getContext()
{
    return m_ctx;
}

void *XMPP::Context::callAlloc(const size_t size, void * const userdata)
{
    return reinterpret_cast<Context *>(userdata)->alloc(size);
}

void *XMPP::Context::callRealloc(void *p, const size_t size,
				 void * const userdata)
{
    return reinterpret_cast<Context *>(userdata)->realloc(p, size);
}

void XMPP::Context::callFree(void *p, void * const userdata)
{
    reinterpret_cast<Context *>(userdata)->free(p);
}

void XMPP::Context::callLog(void * const userdata,
			    const xmpp_log_level_t level,
			    const char * const area,
			    const char * const msg)
{
    reinterpret_cast<Context *>(userdata)->log(level, area, msg);
}

