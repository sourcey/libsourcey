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


#ifndef SOURCEY_XML_H
#define SOURCEY_XML_H


#include "Sourcey/IPacket.h"

#include "pugixml.hpp"

#include <vector>
#include <sstream>


namespace Sourcey {
namespace XML {


struct Template: public pugi::xml_document
{
	std::string data;
};


typedef pugi::xml_attribute Attribute;


// ---------------------------------------------------------------------
//
class Node: public pugi::xml_node 
{
public:
	Node();
	Node(const Node& r);
	Node(const pugi::xml_node& r);
	Node& operator = (const Node& r);

	virtual ~Node();
	
	Node appendChild(const std::string& name = "");

	Node firstChild() { return first_child(); };
	Node append(const std::string& source, bool whiny = true);
};


// ---------------------------------------------------------------------
//
class Document: public pugi::xml_document, public IPacket
{
public:
	Document();
	Document(const Document& src);
	virtual ~Document();

	virtual IPacket* clone() const {
		return new Document(*this);
	}	

	//bool load(const char* data, bool whiny = true);
	
	bool read(Buffer& buf);	
	void write(Buffer& buf) const;

	size_t size() const
	{	
#ifdef PUGIXML_WCHAR_MODE
		return wcslen(first_child().value());
#else
		return strlen(first_child().value());
#endif
	}

	virtual void print(std::ostream& os) const 
	{
		pugi::xml_document::print(os);
	}
	
	Node rootNode() { return first_child(); }
	
	virtual const char* className() const { return "XML Document"; }
};


} // namespace XML
} // namespace Sourcey


#endif // SOURCEY_XML_H
