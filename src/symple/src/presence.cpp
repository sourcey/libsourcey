///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup symple
/// @{


#include "scy/symple/presence.h"
#include "assert.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace smpl {


Presence::Presence()
{
    (*this)["type"]= "presence";
}


Presence::Presence(const Presence& root)
    : Message(root)
{
    if (!isMember("type"))
        (*this)["type"]= "presence";
}


Presence::Presence(const json::Value& root)
    : Message(root)
{
    if (!isMember("type"))
        (*this)["type"]= "presence";
}


Presence::~Presence()
{
}


bool Presence::isProbe()
{
    return (*this)["probe"].asBool();
}


void Presence::setProbe(bool flag)
{
    (*this)["probe"]= flag;
}


} // namespace smpl
} // namespace scy


/// @\}
