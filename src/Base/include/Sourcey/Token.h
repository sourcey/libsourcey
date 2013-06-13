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


#ifndef SOURCEY_Token_H
#define SOURCEY_Token_H


#include "Sourcey/Types.h"
#include "Sourcey/Timeout.h"
#include "Sourcey/CryptoProvider.h"


namespace Scy {


class Token: public Timeout
	/// Implements a token that expires 
	/// after the specified duration.
{
public:
	Token(long duration) : 
		Timeout(duration), _id(CryptoProvider::generateRandomKey(32)) {}

	Token(const std::string& id = CryptoProvider::generateRandomKey(32), long duration = 10000) : 
		Timeout(duration), _id(id) {}
	
	std::string id() const { return _id; }
	
	bool operator == (const Token& r) const {
		return id()  == r.id();
	}
	
	bool operator == (const std::string& r) const {
		return id() == r;
	}

protected:
	std::string _id;
};


typedef std::vector<Token> TokenList;


} // namespace Scy


#endif
