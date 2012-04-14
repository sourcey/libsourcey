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


#ifndef SOURCEY_Symple_Message_H
#define SOURCEY_Symple_Message_H


#include "Sourcey/Base.h"
#include "Sourcey/IPacket.h"
#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Symple/Address.h"


namespace Sourcey {
namespace Symple {


class Message: public JSON::Value, public IPacket
{
public:		
	Message();
	Message(const JSON::Value& root);
	Message(const Message& root);
	virtual ~Message();

	virtual IPacket* clone() const;
	
	virtual bool valid() const;
	virtual void clear();
	virtual void clearData();
	virtual void clearNotes();
	
	std::string type() const;	
	std::string id() const;	
	Address to() const;
	Address from() const;
	int status() const;	
	
	void setType(const std::string& type);
	void setTo(const Address& to);	
	//void setTo(const std::string& to);	
	void setFrom(const Address& from);
	//void setFrom(const std::string& from);

	void setStatus(int code);
		/// HTTP status codes are used to describe the message response.
		/// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html

	JSON::Value& notes();
	void setNote(const std::string& type, const std::string& text);
	void addNote(const std::string& type, const std::string& text);
		/// Possible "type" values: info, warn, error
		
	JSON::Value data(const std::string& name) const;
	JSON::Value& data(const std::string& name);
	JSON::Value& setData(const std::string& name);
	void setData(const std::string& name, const std::string& data);
	void setData(const std::string& name, int data);
	void removeData(const std::string& name);
	
	virtual bool read(Buffer& buf);
	virtual void write(Buffer& buf) const;
	
	bool isRequest() const;	
	virtual size_t size() const;

	void print(std::ostream& os) const;
	
	virtual const char* className() const { return "Symple::Message"; }
};


} // namespace Symple 
} // namespace Sourcey


#endif // SOURCEY_Symple_Message_H
