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


#ifndef SOURCEY_Symple_Form_H
#define SOURCEY_Symple_Form_H


#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Symple/Command.h"


namespace Scy {
namespace Symple {

	
class Form;
class FormField; 
class FormElement
	/// Base implementation for form pages, sections and fields.
{
public:
	FormElement();
	FormElement(JSON::Value& root, const std::string& type = "", const std::string& id = "", const std::string& label = "");
	FormElement(const FormElement& r);
	FormElement& operator = (const FormElement& r);
	virtual ~FormElement();
	
	std::string type() const;
	std::string id() const;
	std::string label() const;
	
	void setType(const std::string& type);
		/// Possible "type" values
		///	  page, section, text, text-multi,
		///	  list, list-multi, checkbox, media, custom
	
	void setId(const std::string& id);
	void setLabel(const std::string& text);
	void setHint(const std::string& text);
	void setError(const std::string& error);
		/// Sets and optional validation error message.

	FormElement addPage(const std::string& id = "", const std::string& label = "");
	FormElement addSection(const std::string& id = "", const std::string& label = "");
	FormField addField(const std::string& type, const std::string& id = "", const std::string& label = "");
	
	FormField getField(const std::string& id, bool partial = false);
	bool getField(const std::string& id, FormField& field, bool partial = false);
	bool hasField(const std::string& id, bool partial = false);
		/// Returns true if the given Address matches any of the
		/// internal form element IDs.
		/// If the partial flag is set then substring matches
		/// will be counted.

	void setLive(bool flag);
		/// Live fields or elements are used to submit partial
		/// sections a form, without sending the entire form.

	bool live() const;
		/// Returns true if this field is live, meaning the
		/// form-processing entity should auto-update this
		/// field's value whenever it changes.		

	bool clearElements(const std::string& id, bool partial = false);
		/// Clears child elements matching the given ID.
	
	void clear();
		/// Clear the entire form.

	bool valid() const;
		/// Returns true if the form element is valid.
		
	int numElements();
		/// Returns the number of child elements.

	bool hasErrors();
		/// Returns true if any fields have errors.

	bool hasPages();
		/// Returns true if the form has multiple pages.

	JSON::Value& root() const;

protected:
	JSON::Value* _root;
		/// The root pointer is just a reference to
		/// the externally managed JSON value memory.
};


class Form: public FormElement
{
public:	
	Form();
	Form(JSON::Value& root);
	Form(Command& root);
	virtual ~Form();
	
	bool valid();

	std::string action() const;
	bool partial() const;

	void setAction(const std::string& action);
		/// Possible "action" values
		/// form 		The form-processing entity is asking the form-submitting entity to complete a form.
		/// submit 		The form-submitting entity is submitting data to the form-processing entity.
		/// cancel 		The form-submitting entity has cancelled submission of data to the form-processing entity.
		/// result 		The form-processing entity is returning data to the form-submitting entity.

	void setPartial(bool flag);
		/// Notifies the form is a partial section of the form.
		/// This is used for transmitting and updating live
		/// fields such as for auto-complete.
};


class FormField: public FormElement
{
public:
	FormField();
	FormField(JSON::Value& root, const std::string& type = "", const std::string& id = "", const std::string& label = "");
	virtual ~FormField();
	
	void addOption(const std::string& key, const std::string& value);	
	void addOption(const std::string& value);
		/// Adds an option for list based fields.

	void setValue(const std::string& value);
	void setValue(int value);
	void setValue(double value);
	void setValue(bool value);
		/// Sets the value clearing all other values.	

	void addValue(const std::string& value);
	void addValue(int value);
	void addValue(double value);
	void addValue(bool value);
		/// Appends a value to the value array.		

	JSON::Value& values();
		/// Returns a JSON array of all values.

	std::string value() const;
	int intValue() const;
	double doubleValue() const;
	bool boolValue() const;
		/// Returns the first value.
		/// Most formats (except multi) only
		/// require single value.
};


} // namespace Symple 
} // namespace Scy


#endif // SOURCEY_Symple_Form_H
