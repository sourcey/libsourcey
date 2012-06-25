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


#ifndef SOURCEY_XMPP_Stanza_H
#define SOURCEY_XMPP_Stanza_H


#include "Sourcey/Base.h"
#include "Sourcey/XML/XML.h"


namespace Sourcey {
namespace XMPP {


struct Error;


struct Stanza: public XML::Document
{	
	Stanza(const char* data = NULL);
	Stanza(const XML::Document& src);
	virtual ~Stanza() {};
	
	static Stanza* create(const XML::Document& src);
	static Stanza* create(const char* data);
		// Creates an IQ, Presence, or Message stanza
		// from the provided data.

	virtual Stanza* clone() const;

	virtual void clear();
		// Removes the contents of the stanza.

	virtual void setName(const std::string& name);
	virtual void setId(const std::string& id);
	virtual void setType(const std::string& type);
	virtual void setTo(const std::string& to);
	virtual void setFrom(const std::string& from);

	virtual Error setError(int code, const std::string& cond, const std::string& type, const std::string& text = "");

	virtual std::string name() const;
	virtual std::string id() const;
	virtual std::string type() const;
	virtual std::string to() const;
	virtual std::string from() const;

	virtual Error error() const;
};


struct IQ: public Stanza 
{
	IQ(const char* data);
	IQ(const std::string& type = "set",
	   const std::string& to = "",
	   const std::string& from = "",
	   const std::string& id = "");
	IQ(const XML::Document& src);
	virtual ~IQ() {};

	Stanza* clone() const;
	
	template<typename T>
	T query() {	
		// Returns the first child in the <iq/>.
		XML::Node node = first_child().first_child();
		return T(node);
	}
};


struct Message: public Stanza 
{
	Message(const char* data = NULL);
	Message(const XML::Document& src);
	virtual ~Message() {};

	Stanza* clone() const;

	void setSubject(const std::string& subject);
	void setBody(const std::string& body);
};


struct Presence: public Stanza 
{
	Presence(const char* data = NULL);
	Presence(const XML::Document& src);
	virtual ~Presence() {};

	Stanza* clone() const;

	void setStatus(const std::string& status);
	void setShow(const std::string& show);
};


struct Error: public XML::Node 
{
	Error();
	Error(XML::Node& node);
	Error(Stanza& stanza, int code, const std::string& cond = "", const std::string& type = "", const std::string& text = "");
	virtual ~Error();

	int code() const;
	std::string condition() const;
	std::string type() const;

	Error set(int code, const std::string& cond, const std::string& type, const std::string& text = "");
	Error setCode(int code);
	Error setCondition(const std::string& cond);
	Error setType(const std::string& type);
	Error setText(const std::string& text);
};


} // namespace Sourcey 
} // namespace XMPP


#endif // SOURCEY_XMPP_Stanza_H