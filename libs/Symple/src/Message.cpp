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


#include "Sourcey/Symple/Message.h"
#include "Sourcey/Util.h"
#include "Sourcey/CryptoProvider.h"
#include "Poco/Format.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Symple {


Message::Message() 
{
	(*this)["id"] = CryptoProvider::generateRandomKey(32);
	(*this)["type"] = "message";
}


Message::Message(const Message& root) :
	JSON::Value(root)
{
	if (!isMember("id"))
		(*this)["id"] = CryptoProvider::generateRandomKey(32);
	if (!isMember("type"))
		(*this)["type"] = "message";
}


Message::Message(const JSON::Value& root) :
	JSON::Value(root)
{
	if (!isMember("id"))
		(*this)["id"] = CryptoProvider::generateRandomKey(32);
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
	string document;
	buf.readString(document, buf.remaining());	
	JSON::Reader reader;
	return reader.parse(document, *this);
}


void Message::write(Buffer& buf) const 
{
	buf.writeString(JSON::stringify(*this));
}


size_t Message::size() const
{
	return JSON::stringify(*this).size();
}

	
void Message::print(ostream& os) const
{
	JSON::StyledWriter writer;
	os << writer.write(*this);
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


string Message::to() const 
{
	return get("to", "").asString();
}


string Message::from() const 
{
	return get("from", "").asString();
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

	
void Message::setTo(const ID& to) 
{
	(*this)["to"] = to.toString();
}
	

void Message::setTo(const string& to) 
{
	(*this)["to"] = to;
}


void Message::setFrom(const ID& from) 
{
	(*this)["from"] = from.toString();
}


void Message::setFrom(const string& from) 
{
	(*this)["from"] = from;
}


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


void Message::setData(const string& name, const string& data) 
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


} // namespace Symple 
} // namespace Sourcey