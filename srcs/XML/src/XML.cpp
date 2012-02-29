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


#include "Sourcey/XML/XML.h"


using namespace std; 


namespace Sourcey {
namespace XML {


// ---------------------------------------------------------------------
//	
Node::Node() 
{
}


Node::Node(const Node& r)
{
	*this = r;
}


Node::Node(const pugi::xml_node& r) 
{ 
	static_cast<pugi::xml_node&>(*this) = r;
}

	
Node::~Node() 
{
}


Node& Node::operator = (const Node& r) 
{
	static_cast<pugi::xml_node&>(*this) = static_cast<const pugi::xml_node&>(r);
	return *this;
}


Node Node::append(const string& source, bool whiny) 
{			
	Document temp;
	bool result = temp.load(source.data(), whiny);
	if (!result)
		return Node();
	append_copy(temp.first_child());
	return last_child();
}

	
Node Node::appendChild(const string& name) 
{ 
	XML::Node node = append_child();
	if (!name.empty())
		node.set_name(name.data());
	return node; 
};


// ---------------------------------------------------------------------
//	
Document::Document() 
{
}


Document::Document(const Document& src) 
{ 
	stringstream ss;
	src.print(ss);
	load(ss);
}


Document::~Document() 
{
}


/*
bool Document::load(const char* data, bool whiny) 
{
	pugi::xml_parse_result result = static_cast<pugi::xml_document*>(this)->load(data); 
	if (!result) {
		if (whiny)
			throw Exception(result.description()); 
		return false;
	}
	return true;
}
*/


bool Document::read(Buffer& buf) 
{
	string data;
	buf.readString(data, buf.size());
	pugi::xml_parse_result result = load(data.data()); //reinterpret_cast<pugi::xml_document*>(this)->
	if (!result) {
		return false;
	}
	return true;
}
	

void Document::write(Buffer& buf) const 
{
	ostringstream s;
	print(s);
	buf.writeString(s.str());
}



} // namespace XML
} // namespace Sourcey