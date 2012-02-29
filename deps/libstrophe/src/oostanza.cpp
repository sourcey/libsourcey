/* oostanza.cpp
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

#include "strophe.h"
#include "strophepp.h"

using namespace XMPP;

void *Stanza::operator new(size_t size, Context *ctx)
{
    void *p;

    /* we must allocate extra room for the Context object so that the
    destructor can access it to free the object.  C++ does not allow
    us to access normal members in the destructor, so we have to hide
    it.  This must be prepended as well, since C++ will add stuff to
    the end in subclasses. */

    p = ctx->alloc(size + sizeof(Context *));
    if (!p) return p;

    *reinterpret_cast<Context **>(p) = ctx;
    p = reinterpret_cast<void *>(reinterpret_cast<char *>(p) + 
				 sizeof(Context *));

    return p;
}

void Stanza::operator delete(void *p)
{
    Context *ctx;

    ctx = *reinterpret_cast<Context **>(reinterpret_cast<char *>(p) - 4);
    ctx->free(reinterpret_cast<char *>(p) - 4);
}

Stanza::Stanza(Context *ctx)
{
    m_ctx = ctx;
    m_stanza = ::xmpp_stanza_new(ctx->getContext());
    // TODO: check for errors
}

Stanza::~Stanza()
{
}

Stanza *Stanza::create(Context *ctx)
{
    return new (ctx) Stanza(ctx);
}

void Stanza::release()
{
    if (::xmpp_stanza_release(m_stanza))
	delete this;
}

Stanza *Stanza::clone()
{
    ::xmpp_stanza_clone(m_stanza);
    return this;
}

Stanza *Stanza::copy()
{
    // TODO
    return NULL;
}


