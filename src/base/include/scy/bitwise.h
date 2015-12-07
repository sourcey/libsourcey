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

#ifndef SCY_Flaggable_H
#define SCY_Flaggable_H


#include <string>


namespace scy {

    
struct Bitwise 
{
    unsigned data; // storage integer

    Bitwise(unsigned flags = 0) : data(flags) {}
    virtual void reset() { data = 0; };
    virtual void set(unsigned flag) { if (!has(flag)) data |= flag; };
    virtual void add(unsigned flag) { data |= flag; };
    virtual void remove(unsigned flag) { data &= ~flag; };
    virtual void toggle(unsigned flag) { data ^= flag; };
    virtual bool has(unsigned flag) const { return (data & flag) == flag; };
};


#if 0
class Flaggable 
{
public:
    Flaggable(unsigned flags = 0) : _flags(flags) {}
    Flaggable(const Flaggable& r) : _flags(r._flags) {}
    Flaggable(const Flags& r) : _flags(r) {}

    virtual void setFlag(unsigned flag) { 
        _flags.set(flag); 
    };
    virtual void addFlag(unsigned flag) { 
        _flags.add(flag); 
    };
    virtual void removeFlag(unsigned flag) { 
        _flags.remove(flag); 
    };
    virtual void toggleFlag(unsigned flag) { 
        _flags.toggle(flag); 
    };
    virtual bool hasFlag(unsigned flag) const { 
        return _flags.has(flag); 
    };
    virtual void setFlags(Flags flags) { 
        _flags = flags; 
    };
    virtual Flags flags() const { return _flags; };

protected:
    Flags _flags;
};
#endif


} // namespace scy


#endif // SCY_Flaggable_H