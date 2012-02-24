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


#include "Sourcey/XMPP/DataForm.h"
#include "Sourcey/XMPP/Command.h"

#include "Sourcey/Util.h"
#include "Poco/Format.h"
#include "assert.h"


using namespace std;
using namespace Poco;
using namespace Sourcey::XML;


namespace Sourcey {
namespace XMPP {


const char* DEFAULT_FORM = "<x type=\"%s\"/>\n";


// ---------------------------------------------------------------------
//
// Data DataForm Document
//
// ---------------------------------------------------------------------
FormDocument::FormDocument(const string& type)
{
	reinterpret_cast<Document*>(this)->load(
		format(DEFAULT_FORM, type).data()
	); 
}


void FormDocument::load(const Node& xml) 
{
	stringstream s;
	xml.print(s);	
	reinterpret_cast<Document*>(this)->load(s);
	assert(strcmp(xml.name(), "x") == 0);
	assert(!empty());
	assert(valid());
}


bool FormDocument::valid() const 
{
	return !empty();
}


DataForm FormDocument::form() 
{
	return (XMPP::DataForm&)*this;
}


// ---------------------------------------------------------------------
//
// Data Form
//
// ---------------------------------------------------------------------
DataForm::DataForm() 
{
}


DataForm::DataForm(Command& command) :
	Node(command.child("x").empty() ?
		command.appendChild("x") : 
		command.child("x"))
{
	if (attribute("xmlns").empty())
		append_attribute("xmlns").set_value("jabber:x:data");

	if (attribute("type").empty())
		append_attribute("type").set_value("form");
}


DataForm::~DataForm() 
{
}


bool DataForm::valid() const 
{
	// TODO: Improve checking
	return !empty();
}


string DataForm::type() const 
{
	return attribute("type").value();
}


void DataForm::clear() 
{
	for (pugi::xml_node node = first_child(); node; node = node.next_sibling()) {
		remove_child(node);
    }
}


DataForm DataForm::setType(const string& type) 
{
	remove_attribute("type");
	append_attribute("type").set_value(type.data());
	return *this;
}


DataForm DataForm::setTitle(const string& title)
{
	Node node = append_child();
	node.set_name("title");
	node.append_child(pugi::node_pcdata).set_value(title.data());
	return *this;
}


DataForm DataForm::setInstructions(const string& instructions) 
{	
	Node node = append_child();
	node.set_name("instructions");
	node.append_child(pugi::node_pcdata).set_value(instructions.data());
	return *this;
}


FormField DataForm::addField(const string& type, const string& var, const string& label) 
{
	assert(!type.empty());
	//Node node = appendChild("");
	return FormField(*this, type, var, label);
}


FormField DataForm::getField(const string& var) 
{
    Node field = select_single_node(format("//field[@var='%s']", var).data()).node();
	if (!field.empty())
		return FormField(field);
	return FormField();
}


FormField DataForm::matchFields(const string& substr)
{
    Node field = select_single_node(format("//field[contains(@var, '%s')]", substr).data()).node();
	if (!field.empty())
		return FormField(field);
	return FormField();
}


FormLayoutPage DataForm::addPage(const string& label) 
{
	//Node node = appendChild();
	return FormLayoutPage(*this, label);
}


// ---------------------------------------------------------------------
//
// Data Form Layout Container
//
// ---------------------------------------------------------------------
/*
FormLayoutContainer::FormLayoutContainer(DataForm& form, const string& label) :
	Node(form.appendChild)
{
}
*/

FormLayoutContainer::FormLayoutContainer(Node& node, const string& label) :
	Node(node)
{
	if (!label.empty()) {
		remove_attribute("label");
		append_attribute("label").set_value(label.data());
	}
}


FormLayoutContainer::~FormLayoutContainer() 
{
}


FormLayoutContainer FormLayoutContainer::setLabel(const string& label)
{
	remove_attribute("label");
	append_attribute("label").set_value(label.data());
	return *this;
}


FormLayoutContainer FormLayoutContainer::addText(const string& text)
{
	Node node = append_child();
	node.set_name("text");
	node.append_child(pugi::node_pcdata).set_value(text.data());
	return *this;
}


FormLayoutContainer FormLayoutContainer::addFieldRef(const string& var)
{
	Node node = append_child();
	node.set_name("fieldref");
	node.append_attribute("var").set_value(var.data());
	return *this;
}

FormLayoutContainer FormLayoutContainer::addFieldRef(const FormField& field)
{
	Node node = append_child();
	node.set_name("fieldref");
	node.append_attribute("var").set_value(field.var().data());
	return *this;
}


// ---------------------------------------------------------------------
//
// Data Form Layout Page
//
// ---------------------------------------------------------------------
FormLayoutPage::FormLayoutPage(DataForm& form, const string& label) :
	FormLayoutContainer(form.appendChild("page"), label)
{
	remove_attribute("xmlns");
	append_attribute("xmlns").set_value("http://jabber.org/protocol/xdata-layout");
}


FormLayoutPage::FormLayoutPage(Node& node, const string& label) :
	FormLayoutContainer(node, label)
{
	assert(strcmp(name(), "page") == 0);
	//set_name("page");
	//remove_attribute("xmlns");
	//append_attribute("xmlns").set_value("http://jabber.org/protocol/xdata-layout");
}


FormLayoutPage::~FormLayoutPage() 
{
}
	

FormLayoutSection FormLayoutPage::addSection(const string& label)
{
	return FormLayoutSection(*this, label);
}


// ---------------------------------------------------------------------
//
// Data Form Layout Section
//
// ---------------------------------------------------------------------
FormLayoutSection::FormLayoutSection(FormLayoutPage& page, const string& label) :
	FormLayoutContainer(page.appendChild("section"), label)
{
}


FormLayoutSection::FormLayoutSection(Node& node, const string& label) :
	FormLayoutContainer(node, label)
{
	assert(strcmp(name(), "section") == 0);
	//set_name("section");
}


FormLayoutSection::~FormLayoutSection() 
{
}


// ---------------------------------------------------------------------
//
// Data Form Field
//
// ---------------------------------------------------------------------
FormField::FormField()
{
}


FormField::FormField(DataForm& parent, const string& type, const string& var, const string& label) :
	Node(parent.appendChild("field"))
{
	append_attribute("type").set_value(type.data());
	append_attribute("var").set_value(var.data());
	append_attribute("label").set_value(label.data());
}


FormField::FormField(Node& node, const string& type, const string& var, const string& label) :
	Node(node)
{
	assert(strcmp(name(), "field") == 0);
	//set_name("field");
	//append_attribute("type").set_value(type.data());
	//append_attribute("var").set_value(var.data());
	//append_attribute("label").set_value(label.data());
}


FormField::~FormField()
{
}


FormField FormField::setType(const string& type)
{
	remove_attribute("type");
	append_attribute("type").set_value(type.data());
	return *this;
}


FormField FormField::setLabel(const string& label)
{
	remove_attribute("label");
	append_attribute("label").set_value(label.data());
	return *this;
}


FormField FormField::setVar(const string& var)
{
	remove_attribute("var");
	append_attribute("var").set_value(var.data());
	return *this;
}


FormField FormField::addOption(const string& label)
{
	Node node = append_child();
	node.set_name("option");
	if (!label.empty())
		node.append_attribute("label").set_value(label.data());
	// NOTE: Returns the current <option/> element, not the <field/>
	return (FormField&)node; 
}


FormField FormField::addValue(const char* value)
{
	Node node = append_child();
	node.set_name("value");
	node.append_child(pugi::node_pcdata).set_value(value);
	return *this;
}


FormField FormField::addValue(const string& value)
{
	return addValue(value.data()); // (FormField&)node;
}


FormField FormField::addValue(int value)
{
	ostringstream stm;
	stm << value;
	return addValue(stm.str().data()); // (FormField&)node;
}


FormField FormField::addValue(unsigned int value)
{
	ostringstream stm;
	stm << value;
	return addValue(stm.str().data()); // (FormField&)node;
}


FormField FormField::addValue(double value)
{
	ostringstream stm;
	stm << value;
	return addValue(stm.str().data()); // (FormField&)node;
}


FormField FormField::addValue(bool value)
{
	return addValue(value ? "1" : "0"); // (FormField&)node;
}

	
FormField FormField::setDescription(const string& desc)
{
	remove_child("desc");
	Node node = append_child();
	node.set_name("desc");
	node.append_child(pugi::node_pcdata).set_value(desc.data());
	return *this;
}


void FormField::clear()
{
    for (Node node = first_child(); node; node = node.next_sibling())
		remove_child(node);
}


string FormField::getValue()
{
	return child("value").child_value();
}


int FormField::getIntValue()
{
	return atoi(child("value").child_value());
}


bool FormField::getBoolValue()
{
	return getValue() == "1" ? true : false;
}


string FormField::type() const
{
	return attribute("type").value();
}


string FormField::label() const
{
	return attribute("label").value();
}


string FormField::var() const
{
	return attribute("var").value();
}


//
// Non-Standard
//
FormField FormField::setError(const string& error)
{
	remove_child("error");
	Node node = append_child();
	node.set_name("error");
	node.append_child(pugi::node_pcdata).set_value(error.data());
	return (FormField&)node;
}


} // namespace XMPP 
} // namespace Sourcey