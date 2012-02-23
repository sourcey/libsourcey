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


#include "Sourcey/XMPP/Stanza.h"
#include "Sourcey/CryptoProvider.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"

#include "assert.h"


using namespace std; 
using namespace Poco;
using namespace Sourcey::XML;


namespace Sourcey {
namespace XMPP {


// ---------------------------------------------------------------------
//
// Stanza
//
// ---------------------------------------------------------------------
Stanza::Stanza(const char* data) 
{
	if (data) {
		load(data);
	}
}


Stanza::Stanza(const XML::Document& src) :
	XML::Document(src)
{
}

	
Stanza* Stanza::create(const XML::Document& src) 
{	
	if (strcmp(src.first_child().name(), "iq") == 0)
		return new IQ(src);

	else if (
		strcmp(src.first_child().name(), "presence") == 0)
		return new Presence(src);

	else if (
		strcmp(src.first_child().name(), "message") == 0)
		return new Message(src);
	
	Log("debug") << "[Stanza] Unable to create XMPP stanza." << endl;
	return NULL;
}


Stanza* Stanza::create(const char* data) 
{	
	//
	// WARNING: Does not take into account <xml tag.
	//
	if (strncmp(data, "<iq", 3) == 0) 
	{
		return new IQ(data);
	}
	else if (strncmp(data, "<presence", 9) == 0)
	{
		return new Presence(data);
	}
	else if (strncmp(data, "<message", 8) == 0)
	{
		return new Message(data);
	}
	
	Log("debug") << "[Stanza] Unable to create XMPP stanza." << endl;
	return NULL;
}


Stanza* Stanza::clone() const 
{
	Log("debug") << "Cloning Stanza: " << id() << endl;
	return new Stanza(*this);
}


void Stanza::clear() 
{
	remove_child(first_child().name());
	//return *this;
}


void Stanza::setName(const string& name) 
{
	first_child().set_name(name.data());
	//return *this;
}


void Stanza::setId(const string& id) 
{
	first_child().remove_attribute("id");
	first_child().append_attribute("id").set_value(id.data());
	//return *this;
}


void Stanza::setType(const string& type) 
{
	first_child().remove_attribute("type");
	first_child().append_attribute("type").set_value(type.data());
	//return *this;
}


void Stanza::setTo(const string& to) 
{
	first_child().remove_attribute("to");
	first_child().append_attribute("to").set_value(to.data());
	//return *this;
}


void Stanza::setFrom(const string& from) 
{
	first_child().remove_attribute("from");
	first_child().append_attribute("from").set_value(from.data());
	//return *this;
}


string Stanza::name() const 
{
	return first_child().name();
}


string Stanza::id() const 
{
	return first_child().attribute("id").value();
}


string Stanza::type() const 
{
	return first_child().attribute("type").value();
}


string Stanza::to() const 
{
	return first_child().attribute("to").value();
}


string Stanza::from() const 
{
	return first_child().attribute("from").value();
}


Error Stanza::error() const 
{
	Node node = first_child().child("error");
	return Error(node);
}


Error Stanza::setError(int code, const string& cond, const string& type, const string& text) 
{
	first_child().remove_child("error");
	Node node = first_child().append_child();
	return Error(*this, code, cond, type, text);
	//node.set_name("error");
	//((Error&)first_child().append_child()).set(code, cond, type, text);
	//return *this;
}


// ---------------------------------------------------------------------
//
// IQ Error
//
// ---------------------------------------------------------------------
Error::Error()
{
}


Error::Error(XML::Node& node) :
	XML::Node(node)
{
	node.set_name("error");
}


Error::Error(Stanza& stanza, int code, const string& cond, const string& type, const string& text) :
	XML::Node(stanza.rootNode().appendChild("error"))
{
	set(code, cond, type, text);
}


Error::~Error()
{
}


int Error::code() const 
{
	return attribute("code").as_int();
}


string Error::condition() const 
{
	return first_child().name();
}


string Error::type() const 
{
	return attribute("type").value();
}


Error Error::set(int code, const string& cond, const string& type, const string& text) 
{
	set_name("error");
	setCode(code);
	setCondition(cond);
	setType(type);
	if (!text.empty())
		setText(text);
	return *this;
}


Error Error::setCode(int code) 
{
	if (code) {
		remove_attribute("code");
		append_attribute("code").set_value(code);
	}
	return *this;
}


Error Error::setCondition(const string& cond) 
{
	Node node = append_child();
	node.set_name(cond.data());
	node.append_attribute("xmlns").set_value("urn:ietf:params:xml:ns:xmpp-stanzas");
	return *this;
}


Error Error::setType(const string& type) 
{
	remove_attribute("type");
	append_attribute("type").set_value(type.data());
	return *this;
}
	

Error Error::setText(const string& text) 
{
	remove_child("text");
	Node node = append_child();
	node.set_name("text");
	node.append_attribute("xmlns").set_value("urn:ietf:params:xml:ns:xmpp-stanzas");
	node.append_child(pugi::node_pcdata).set_value(text.data());
	return *this;
}


// ---------------------------------------------------------------------
//
// IQ Stanza
//
// ---------------------------------------------------------------------
IQ::IQ(const char* data) : 
	Stanza(data ? data : "<iq/>") 
{
	//Log("debug") << "Creating IQ: " << id() << endl;

	if (!data) {
		setId(CryptoProvider::generateRandomKey(16));
	}
}


IQ::IQ(const string& type,
	   const string& to,
	   const string& from,
	   const string& id) 
{
	   load(format("<iq type=\"%s\" to=\"%s\" from=\"%s\" id=\"%s\" />", 
			type, 
			to, 
			from,
			id.empty() ? CryptoProvider::generateRandomKey(8) : id).data());
}


IQ::IQ(const XML::Document& src) :
	Stanza(src)
{
}

	
Stanza* IQ::clone() const
{
	//Log("debug") << "Cloning IQ: " << id() << endl;
	return new IQ(*this);
}


// ---------------------------------------------------------------------
//
// Message Stanza
//
// ---------------------------------------------------------------------
Message::Message(const char* data) : 
	Stanza(data ? data : "<message/>") 
{
	Log("debug") << "Creating Message: " << id() << endl;
	if (!data) {
		setId(CryptoProvider::generateRandomKey(16));
	}
}


Message::Message(const XML::Document& src) :
	Stanza(src)
{
}


Stanza* Message::clone() const
{
	//Log("debug") << "Cloning Message: " << id() << endl;
	return new Message(*this);
}


void Message::setSubject(const string& subject) 
{
	first_child().remove_child("subject");
	Node node = first_child().append_child();
	node.set_name("subject");
	node.append_child(pugi::node_pcdata).set_value(subject.data());
	//return *this;
}


void Message::setBody(const string& body) 
{
	first_child().remove_child("body");
	Node node = first_child().append_child();
	node.set_name("body");
	node.append_child(pugi::node_pcdata).set_value(body.data());
	//return *this;
}


// ---------------------------------------------------------------------
//
// Presence Stanza
//
// ---------------------------------------------------------------------
Presence::Presence(const char* data) : 
	Stanza(data ? data : "<presence/>") 
{
	//Log("debug") << "Creating Presence: " << id() << endl;
	if (!data) {
		setId(CryptoProvider::generateRandomKey(16));
	}
}


Presence::Presence(const XML::Document& src) :
	Stanza(src)
{
}


Stanza* Presence::clone() const
{
	//Log("debug") << "Cloning Presence: " << id() << endl;
	return new Presence(*this);
}


void Presence::setStatus(const string& status) 
{
	first_child().remove_child("status");
	Node node = first_child().append_child();
	node.set_name("status");
	node.append_child(pugi::node_pcdata).set_value(status.data());
	//return *this;
}


void Presence::setShow(const string& show) 
{
	first_child().remove_child("show");
	Node node = first_child().append_child();
	node.set_name("show");
	node.append_child(pugi::node_pcdata).set_value(show.data());
	//return *this;
}


} // namespace XMPP 
} // namespace Sourcey