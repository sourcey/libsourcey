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


#ifndef SOURCEY_XMPP_DataForm_H
#define SOURCEY_XMPP_DataForm_H

#include "Sourcey/XML/XML.h"


namespace Sourcey {
namespace XMPP {


struct Command;
struct DataForm;
struct FormField;
struct FormLayoutPage;
struct FormLayoutSection;



struct FormDocument: public XML::Document 
	/// This class provides memory management for the creation
	/// and manipulation of a Form.
{
	FormDocument(const std::string& type = "form");
	virtual ~FormDocument() {};

	virtual void load(const XML::Node& xml);
	virtual bool valid() const;
	virtual DataForm form();
};


struct DataForm: public XML::Node
	/// This class subclasses a referenced XML::Node to provide
	/// polymorphic manipulation methods for XMPP-like Forms.
{
public:		
	DataForm();
	DataForm(Command& command);
	virtual ~DataForm();

	//DataForm create(Command& command);

	virtual bool valid() const;
	virtual std::string type() const;

	virtual void clear();

	virtual DataForm setType(const std::string& type);
	virtual DataForm setTitle(const std::string& message);
	virtual DataForm setInstructions(const std::string& instructions);

	virtual FormField addField(const std::string& type, const std::string& var = "", const std::string& label = "");
	virtual FormField getField(const std::string& var);
	virtual FormField matchFields(const std::string& substr);
	
	//
	//XEP-0141: Data Forms Layout
	//
	virtual FormLayoutPage addPage(const std::string& label = "");

	//
	// Adds a non-standard from group for client side
	// styling reference.
	//FormLayoutPage addGroup(const std::string& ns, const std::string& title, const std::string& desc);
};

struct FormField: public XML::Node 
{
	FormField();
	FormField(DataForm& parent, const std::string& type = "", const std::string& var = "", const std::string& label = "");
	FormField(XML::Node& node, const std::string& type = "", const std::string& var = "", const std::string& label = "");
	virtual ~FormField();

	virtual FormField setType(const std::string& type);
	virtual FormField setLabel(const std::string& label);
	virtual FormField setVar(const std::string& var);

	virtual std::string type() const;
	virtual std::string label() const;
	virtual std::string var() const;

	virtual FormField addOption(const std::string& label = "");
	virtual FormField addValue(const char* value);
	virtual FormField addValue(const std::string& value);
	virtual FormField addValue(int value);
	virtual FormField addValue(unsigned int value);
	virtual FormField addValue(double value);
	virtual FormField addValue(bool value);
	virtual FormField setDescription(const std::string& desc);
	virtual void clear();

	virtual std::string getValue();
	virtual int getIntValue();
	virtual bool getBoolValue();

	virtual FormField setError(const std::string& error);
		// NON-STANDARD: Adds a non-standard <error/> element to
		// to the form field for field specific errors.
};


struct FormLayoutContainer: public XML::Node
	/// Base implementation for form layout pages and sections.
{
	//FormLayoutPage(DataForm& form, const std::string& label);
	//FormLayoutContainer(const XML::Node& node, const std::string& label);
	FormLayoutContainer(XML::Node node, const std::string& label);
	virtual ~FormLayoutContainer();

	virtual FormLayoutContainer setLabel(const std::string& label);
	virtual FormLayoutContainer addText(const std::string& text);
	virtual FormLayoutContainer addFieldRef(const std::string& var);
	virtual FormLayoutContainer addFieldRef(const FormField& field);
};


struct FormLayoutPage: public FormLayoutContainer
	/// Implementation of XEP-0141: Data Forms Layout.
	/// <page xmlns='http://jabber.org/protocol/xdata-layout' label='Personal Information'>
{	
	//static FormLayoutPage create(DataForm& form, const std::string& label);
	
	FormLayoutPage(DataForm& form, const std::string& label);
	FormLayoutPage(XML::Node& node, const std::string& label);
	virtual ~FormLayoutPage();
	
	virtual FormLayoutSection addSection(const std::string& label = "");
};


struct FormLayoutSection: public FormLayoutContainer
	/// Implementation of XEP-0141: Data Forms Layout.
	/// <page xmlns='http://jabber.org/protocol/xdata-layout'>
	///		<section label='Personal Information'/>
	/// </page>
{
	//static FormLayoutSection create(FormLayoutContainer& container, const std::string& label);
	
	FormLayoutSection(FormLayoutPage& page, const std::string& label);
	FormLayoutSection(XML::Node& node, const std::string& label);
	virtual ~FormLayoutSection();
};


} // namespace XMPP 
} // namespace Sourcey


#endif // SOURCEY_XMPP_DataForm_H
