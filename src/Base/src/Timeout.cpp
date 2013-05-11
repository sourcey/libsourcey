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


#include "Sourcey/Timeout.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


namespace Scy {


Timeout::Timeout(long delay, bool autoStart) :
	_startAt(0), _delay(delay) 
{
	if (autoStart)
		start();
}


Timeout::Timeout(const Timeout& src) :
	_startAt(src._startAt), _delay(src._delay) 
{
}


Timeout& Timeout::operator = (const Timeout& src) 
{
	_startAt = src._startAt;
	_delay = src._delay;
	return *this;
}


Timeout::~Timeout() 
{
}


bool Timeout::running() const 
{
	return _startAt != 0;
}


void Timeout::start() 
{
	_startAt = Util::getTime();
}


void Timeout::stop() 
{
	_startAt = 0;
}


void Timeout::reset() 
{
	_startAt = Util::getTime();
}


long Timeout::remaining() const 
{
	time_t current = Util::getTime();
	long remaining = static_cast<long>(_delay - (current - _startAt));
	return remaining > 0 ? remaining : 0;
}


bool Timeout::expired() const 
{
	if (_delay == 0) //_startAt == 0 || 
		return false;

	return remaining() == 0;
}


} // namespace Scy