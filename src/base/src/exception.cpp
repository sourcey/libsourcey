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


#include "scy/exception.h"


namespace scy {


#if 0
Exception::Exception(int code): 
	_code(code)
{
}


Exception::Exception(const std::string& msg, int code) : 
	_msg(msg), _code(code)
{
}


Exception::Exception(const std::string& msg, const std::string& ext, int code) : 
	_msg(msg), _code(code)
{
	if (!ext.empty()) {
		_msg.append(": ");
		_msg.append(ext);
	}
}

	
Exception::~Exception() throw()
{
}


const char* Exception::name() const throw()
{
	return "Error";
}

	
const char* Exception::what() const throw()
{
	return name();
}

	
std::string Exception::message() const
{
	if (!_msg.empty())
		return _msg;
	return name();
}


void Exception::entendMessage(const std::string& ext)
{
	if (!ext.empty()) {
		if (!_msg.empty()) 
			_msg.append(": ");
		_msg.append(ext);
	}
}


void Exception::rethrow() const
{
	throw *this;
}
#endif


} // namespace scy




/*


const char* Exception::className() const throw()
{
	return typeid(*this).name();
}


Exception* Exception::clone() const
{
	return new Exception(*this);
}
Exception::Exception(const Exception& exc) :
	std::exception(exc),
	_msg(exc._msg),
	_code(exc._code)
{
}


Exception& Exception::operator = (const Exception& exc)
{
	if (&exc != this) {
		_msg = exc._msg;
		_code = exc._code;
	}
	return *this;
}
*/