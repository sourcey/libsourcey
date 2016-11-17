///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup symple
/// @{


#ifndef SCY_Symple_Presence_H
#define SCY_Symple_Presence_H


#include "scy/base.h"
#include "scy/json/json.h"
#include "scy/symple/message.h"


namespace scy {
namespace smpl {


class Presence : public Message
{
public:
    Presence();
    Presence(const json::Value& root);
    Presence(const Presence& root);
    virtual ~Presence();

    bool isProbe();
    void setProbe(bool flag);
};


} // namespace smpl
} // namespace scy


#endif // SCY_Symple_Presence_H


/// @\}
