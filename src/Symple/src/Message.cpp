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


#include "Sourcey/Symple/Message.h"
#include "Sourcey/Util.h"
#include "Sourcey/Crypto.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace scy {
namespace smple {


Message::Message() :
	JSON::Value(Json::objectValue)
{
	(*this)["id"] = crypt::randomKey(16);
	(*this)["type"] = "message";
}


Message::Message(const Message& root) :
	JSON::Value(root)
{
	if (!isMember("id"))
		(*this)["id"] = crypt::randomKey(16);
	if (!isMember("type"))
		(*this)["type"] = "message";
}


Message::Message(const JSON::Value& root) :
	JSON::Value(root)
{
	if (!isMember("id"))
		(*this)["id"] = crypt::randomKey(16);
	if (!isMember("type"))
		(*this)["type"] = "message";
}


Message::~Message() 
{
}


IPacket* Message::clone() const
{
	return new Message(*this);
}


bool Message::read(Buffer& buf) 
{
	string root;
	buf.read(root, buf.remaining());	
	return read(root);
}


bool Message::read(const std::string& root)
{
	JSON::Reader reader;
	return reader.parse(root, *this);
}


void Message::write(Buffer& buf) const 
{
	buf.write(JSON::stringify(*this));
}


size_t Message::size() const
{
	// KLUDGE: is there a better way?
	return JSON::stringify(*this).size();
}

	
void Message::print(ostream& os) const
{
	os << JSON::stringify(*this, true);
}


bool Message::valid() const
{
	return isMember("type") 
		&& isMember("id") 
		&& isMember("from");
}


void Message::clear() 
{
    JSON::Value::clear();
}


void Message::clearData() 
{
    (*this)["data"].clear();
}


void Message::clearNotes() 
{
    (*this)["notes"].clear();
}


string Message::type() const
{
	return get("type", "message").asString();
}


string Message::id() const 
{
	return get("id", "").asString();
}


Address Message::to() const 
{
	//return Address(get("to", ""));
	return Address((*this)["to"]); //get("from", ""));
}


Address Message::from() const 
{
	return Address((*this)["from"]); //get("from", ""));
}


int Message::status() const 
{
	return isMember("status") ? (*this)["status"].asInt() : -1;
}


bool Message::isRequest() const 
{
	return status() == -1;
}


JSON::Value& Message::notes()
{
	return (*this)["notes"];
}


JSON::Value Message::data(const string& name) const
{
	return (*this)["data"][name];
}


JSON::Value& Message::data(const string& name) 
{
	return (*this)["data"][name];
}


void Message::setType(const string& type) 
{
	(*this)["type"] = type;
}

	
void Message::setTo(const Address& to) 
{
	(*this)["to"] = to; //.toString();
}
	

/*
void Message::setTo(const string& to) 
{
	(*this)["to"] = to;
}
*/


void Message::setFrom(const Address& from) 
{
	(*this)["from"] = from; //.toString();
}


/*
void Message::setFrom(const string& from) 
{
	(*this)["from"] = from;
}
*/


void Message::setStatus(int code) 
{
	assert(code > 100 && code < 505);
	(*this)["status"] = code;
}


void Message::setNote(const string& type, const string& text)
{
	clearNotes();
	addNote(type, text);
}

void Message::addNote(const string& type, const string& text) 
{
	assert(
		type == "info" ||
		type == "warn" ||
		type == "error"
	);

	JSON::Value note;
	note["type"] = type;
	note["text"] = text;
	(*this)["notes"].append(note);
}


JSON::Value& Message::setData(const string& name) 
{
	return (*this)["data"][name] = name;
}


void Message::setData(const string& name, const char* data) 
{
	(*this)["data"][name] = data;
}


void Message::setData(const string& name, const string& data) 
{
	(*this)["data"][name] = data;
}


void Message::setData(const string& name, const JSON::Value& data) 
{
	(*this)["data"][name] = data;
}


void Message::setData(const string& name, int data) 
{
	(*this)["data"][name] = data;
}


void Message::removeData(const string& name) 
{
	(*this)["data"].removeMember(name);
}


bool Message::hasData(const string& name)
{
	return (*this)["data"].isMember(name);
}


} // namespace smple 
} // namespace scy