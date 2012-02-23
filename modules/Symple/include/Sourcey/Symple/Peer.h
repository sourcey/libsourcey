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


#ifndef SOURCEY_Symple_Peer_H
#define SOURCEY_Symple_Peer_H


#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Symple/ID.h"


namespace Sourcey {
namespace Symple {


class Peer: public JSON::Value
{
public:		
	Peer();
	Peer(const Peer& r);
	Peer(const JSON::Value& r);
	virtual ~Peer();

	virtual bool valid();
	
	ID id() const;	
	std::string user() const;
	std::string name() const;
	std::string type() const;
	std::string address() const;
	
	void setID(const std::string& id);	
	void setType(const std::string& type);
	void setName(const std::string& name);
	void setUser(const std::string& user);
	void setAddress(const std::string& addr);
	
	void print(std::ostream& os) const;
	
	virtual const char* className() const { return "Symple::Peer"; }
};


} // namespace Symple 
} // namespace Sourcey


#endif // SOURCEY_Symple_Peer_H
