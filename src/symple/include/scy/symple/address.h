//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SCY_Symple_Address_H
#define SCY_Symple_Address_H


#include <cstdint>
#include <string>


namespace scy {
namespace smpl {


struct Address
    /// The Address structure is an endpoint identifier for a
    /// peer on the network.
    /// The format is like so: user|id
{
public:
    Address();
    Address(const std::string& addr);
    Address(const std::string& user, const std::string& id); //const std::string& group,
    virtual ~Address();

    bool parse(const std::string& addr);

    bool valid() const;
    void print(std::ostream& os) const;
    std::string toString() const;

    bool operator == (const Address& r);
    bool operator == (std::string& r);

    friend std::ostream& operator << (std::ostream& os, const Address& addr)
    {
        addr.print(os);
        return os;
    }

    std::string user;
    // std::string group;
    std::string id;
};


} // namespace symple
} // namespace scy


#endif // SCY_Symple_Address_H
