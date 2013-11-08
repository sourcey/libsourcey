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


#ifndef SOURCEY_Symple_Message_H
#define SOURCEY_Symple_Message_H


#include "Sourcey/Base.h"
#include "Sourcey/Packet.h"
#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Symple/Address.h"


namespace scy {
namespace smpl {


class Message: public json::Value, public IPacket
{
public:		
	Message();
	Message(const json::Value& root);
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
	void setFrom(const Address& from);

	void setStatus(int code);
		// HTTP status codes are used to describe the message response.
		// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html

	json::Value& notes();
	void setNote(const std::string& type, const std::string& text);
	void addNote(const std::string& type, const std::string& text);
		// Possible "type" values: info, warn, error
		
	json::Value data(const std::string& name) const;
	json::Value& data(const std::string& name);
	json::Value& setData(const std::string& name);
	void setData(const std::string& name, const char* data);
	void setData(const std::string& name, const std::string& data);
	void setData(const std::string& name, const json::Value& data);
	void setData(const std::string& name, int data);
	void removeData(const std::string& name);
	bool hasData(const std::string& name);
	
	virtual std::size_t read(const ConstBuffer& buf);
	virtual std::size_t read(const std::string& root);
	virtual void write(Buffer& buf) const;
	
	bool isRequest() const;	
	virtual size_t size() const;

	void print(std::ostream& os) const;
	
	virtual const char* className() const { return "smpl::Message"; }
};


} // namespace symple 
} // namespace scy


#endif // SOURCEY_Symple_Message_H
