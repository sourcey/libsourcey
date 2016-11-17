///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup symple
/// @{

#include "scy/symple/event.h"
#include "scy/util.h"

using std::endl;

namespace scy {
namespace smpl {

Event::Event()
{
    setType("event");
    setTime(::time(0));
}

Event::Event(const Event& root)
    : Message(root)
{
    if (!isMember("type"))
        setType("event");
    if (!isMember("time"))
        setTime(::time(0));
}

Event::Event(const json::Value& root)
    : Message(root)
{
    if (!isMember("type"))
        setType("event");
    if (!isMember("time"))
        setTime(::time(0));
}

Event::~Event()
{
}

bool Event::valid() const
{
    return Message::valid() && isMember("name");
}

std::string Event::name() const
{
    return get("name", "").asString();
}

time_t Event::time() const
{
    return static_cast<time_t>(get("time", 0).asDouble());
}

void Event::setName(const std::string& name)
{
    (*this)["name"] = name;
}

void Event::setTime(time_t time)
{
    (*this)["time"] = static_cast<Json::UInt64>(time);
    // DateTimeFormatter::format(
    //    Timestamp::fromEpochTime(time),
    //    DateTimeFormat::ISO8601_FORMAT);
}

} // namespace smpl
} // namespace scy

/// @\}
