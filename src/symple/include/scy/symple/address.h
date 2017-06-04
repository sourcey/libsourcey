///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup symple
/// @{


#ifndef SCY_Symple_Address_H
#define SCY_Symple_Address_H


#include "scy/symple/symple.h"
#include <cstdint>
#include <string>


namespace scy {
namespace smpl {


/// The Address structure is an endpoint identifier for a
/// peer on the network.
/// The format is like so: user|id
struct Symple_API Address
{
public:
    Address();
    Address(const std::string& addr);
    Address(const std::string& user,
            const std::string& id);
            // const std::string& group,
    virtual ~Address();

    bool parse(const std::string& addr);

    bool valid() const;
    void print(std::ostream& os) const;
    std::string toString() const;

    bool operator==(const Address& r);
    bool operator==(std::string& r);

    friend std::ostream& operator<<(std::ostream& os, const Address& addr)
    {
        addr.print(os);
        return os;
    }

    std::string user;
    // std::string group;
    std::string id;
};


} // namespace scy
} // namespace smpl


#endif // SCY_Symple_Address_H


/// @\}
