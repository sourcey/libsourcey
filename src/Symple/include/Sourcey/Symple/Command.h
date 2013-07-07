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


#ifndef SOURCEY_Symple_Command_H
#define SOURCEY_Symple_Command_H


#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Base.h"
#include "Sourcey/Symple/Message.h"


namespace scy {
namespace smple {


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
	StringVec params();
	bool matches(const std::string& xnode) const;
};


} // namespace smple 
} // namespace scy


#endif // SOURCEY_Symple_Command_H
