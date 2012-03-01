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


#include "Sourcey/Symple/Presence.h"
#include "Sourcey/Util.h"
#include "Sourcey/CryptoProvider.h"
//#include "Poco/Format.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Symple {


Presence::Presence() 
{
	(*this)["type"] = "presence";
}


Presence::Presence(const Presence& root) :
	Message(root)
{
	if (!isMember("type"))
		(*this)["type"] = "presence";
}


Presence::Presence(const JSON::Value& root) :
	Message(root)
{
	if (!isMember("type"))
		(*this)["type"] = "presence";
}


Presence::~Presence() 
{
}


bool Presence::isProbe() 
{
	return (*this)["probe"].asBool();
}


void Presence::setProbe(bool flag)
{	
	(*this)["probe"] = flag;
}



} // namespace Symple 
} // namespace Sourcey