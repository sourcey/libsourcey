///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup symple
/// @{


#ifndef SCY_Symple_Presence_H
#define SCY_Symple_Presence_H


#include "scy/symple/symple.h"
#include "scy/symple/message.h"


namespace scy {
namespace smpl {


class Symple_API Presence : public Message
{
public:
    Presence();
    Presence(const json::value& root);
    Presence(const Presence& root);
    virtual ~Presence();

    bool isProbe();
    void setProbe(bool flag);
};


} // namespace smpl
} // namespace scy


#endif // SCY_Symple_Presence_H


/// @\}
