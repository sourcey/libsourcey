//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_SDP_LINE_S_H
#define SOURCEY_SDP_LINE_S_H


#include "Sourcey/SDP/Line.h"

#include "Sourcey/Util.h"
#include <assert.h>


namespace Sourcey {
namespace SDP { 


class S: public Line {
public:
	S(const std::string& src);
	virtual ~S();

	std::string sessionName();
	void setSessionName(const std::string& s);

	virtual std::string toString();

private:
	std::string _sessionName;

};


} // namespace Sourcey
} // namespace SDP 


#endif
