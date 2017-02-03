///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup symple
/// @{


#ifndef SCY_Symple_Command_H
#define SCY_Symple_Command_H


#include "scy/base.h"
#include "scy/json/json.h"
#include "scy/symple/message.h"


namespace scy {
namespace smpl {


class Command : public Message
{
public:
    Command();
    Command(const json::Value& root);
    Command(const Command& root);
    virtual ~Command();

    std::string node() const;
    std::string action() const;

    void setNode(const std::string& node);
    void setAction(const std::string& action);

    bool valid() const;

    std::string param(int n) const;
    std::vector<std::string> params();
    bool matches(const std::string& xnode) const;
};


} // namespace smpl
} // namespace scy


#endif // SCY_Symple_Command_H


/// @\}
