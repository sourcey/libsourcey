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


#ifndef SOURCEY_Symple_Command_H
#define SOURCEY_Symple_Command_H


#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Base.h"
#include "Sourcey/Symple/Message.h"


namespace Sourcey {
namespace Symple {


class Command: public Message
{
public:	
	Command();
	Command(const JSON::Value& root);
	Command(const Command& root);
	virtual ~Command();
		
	std::string node() const;
	std::string action() const;
		
	void setNode(const std::string& node);
	void setAction(const std::string& action);
	
	bool valid() const;

	std::string param(int n) const;
	StringList params();
	bool matches(const std::string& xnode) const;
};


} // namespace Symple 
} // namespace Sourcey


#endif // SOURCEY_Symple_Command_H
