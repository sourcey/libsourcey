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


#ifndef SOURCEY_Timeout_H
#define SOURCEY_Timeout_H


#include "Sourcey/Types.h"


namespace Scy {


class Timeout 
{
public:
	Timeout(long delay = 0, bool autoStart = false);
	Timeout(const Timeout& src);
	~Timeout();
	
	bool running() const;
	void start();
	void stop();
	void reset();
	long remaining() const;
	bool expired() const;

	void setDelay(long delay) { _delay = delay; };

	time_t startAt() const { return _startAt; };
	long delay() const { return _delay; };

	Timeout& operator = (const Timeout& src);

protected:
	time_t	_startAt;
	long	_delay;
};



} // namespace Scy


#endif
