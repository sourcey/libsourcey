///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup symple
/// @{


#ifndef SCY_Symple_Event_H
#define SCY_Symple_Event_H


#include "scy/symple/symple.h"
#include "scy/symple/message.h"
#include <ctime>


namespace scy {
namespace smpl {


class Symple_API Event : public Message
{
public:
    Event();
    Event(const json::value& root);
    Event(const Event& root);
    virtual ~Event();

    bool valid() const;

    std::string name() const;
    // std::string message() const;
    std::time_t time() const;

    void setName(const std::string& name);
    // void setMessage(const std::string& message);
    void setTime(std::time_t time);

    // Sets the time from a UTC time string.
    // void setTime(const std::string& time);
};


} // namespace smpl
} // namespace scy


#endif // SCY_Symple_Event_H


/// @\}
