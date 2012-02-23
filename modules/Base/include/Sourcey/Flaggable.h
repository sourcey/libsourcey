//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


namespace Sourcey {


struct FlagT 
{
	FlagT(unsigned flags = 0) : _data(flags) {}
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
	Flaggable(const FlagT& r) : _flags(r) {}

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
	virtual void setFlags(FlagT flags) { 
		_flags = flags; 
	};
	virtual FlagT flags() const { return _flags; };

protected:
	FlagT _flags;
};


} // namespace Sourcey


#endif // SOURCEY_Flaggable_H