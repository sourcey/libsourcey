//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_Flaggable_H
#define SOURCEY_Flaggable_H


#include <string>


namespace Scy {


struct Flags 
{
	Flags(unsigned flags = 0) : _data(flags) {}
	virtual void reset() { _data = 0; };
	virtual void set(unsigned flag) { if (!has(flag)) _data |= flag; };
	virtual void add(unsigned flag) { _data |= flag; };
	virtual void remove(unsigned flag) { _data &= ~flag; };
	virtual void toggle(unsigned flag) { _data ^= flag; };
	virtual bool has(unsigned flag) const { return (_data & flag) == flag; };
	virtual unsigned data() const { return _data; };

private:
	unsigned _data;
};


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


} // namespace Scy


#endif // SOURCEY_Flaggable_H