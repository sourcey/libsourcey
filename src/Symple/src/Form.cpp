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


#include "Sourcey/Symple/Form.h"
#include "Sourcey/Util.h"
#include "Sourcey/CryptoProvider.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Symple {


Form::Form()
{
}


Form::Form(Command& root) :
	FormElement(root["form"])
{
	root["form"]["type"] = "form";
}


Form::Form(JSON::Value& root) :
	FormElement(root)
{
	root["type"] = "form";
}


Form::~Form() 
{
}


string Form::action() const 
{
	return root().get("action", "create").asString();
}

	
void Form::setAction(const string& action) 
{
	assert(
		action == "create" ||
		action == "submit" ||
		action == "result"
	);
	root()["action"] = action;
}


bool Form::valid()
{
	return FormElement::valid() 
		&& root().size() > 0 
		&& !hasErrors();
}


// ---------------------------------------------------------------------
//
// Form Element
//
// ---------------------------------------------------------------------
FormElement::FormElement() :
	_root(NULL)
{
}


FormElement::FormElement(JSON::Value& root, const string& type, const string& id, const string& label) :
	_root(&root)
{
	if (!type.empty())
		root["type"] = type;
	if (!id.empty())
		root["id"] = id;
	if (!label.empty())
		root["label"] = label;
}
	

FormElement::FormElement(const FormElement& r) :
	_root(r._root)
{
}


FormElement& FormElement::operator = (const FormElement& r)
{
	_root = r._root;
	return *this;
}
	

FormElement::~FormElement() 
{
}


string FormElement::type() const
{
	return root()["type"].asString();
}


string FormElement::id() const
{
	return root()["id"].asString();
}


string FormElement::label() const
{
	return root()["label"].asString();
}


bool FormElement::live() const
{
	return root()["live"].asBool();
}


void FormElement::setType(const string& type)
{
	root()["type"] = type;
}


void FormElement::setId(const string& id)
{
	root()["id"] = id;
}


void FormElement::setLabel(const string& text)
{
	root()["label"] = text;
}


void FormElement::setHint(const string& text)
{	
	root()["hint"] = text;
}


void FormElement::setError(const string& error)
{
	root()["error"] = error;
}



void FormElement::setLive(bool flag)
{	
	root()["live"] = flag;
}


FormElement FormElement::addPage(const string& label) 
{
	return FormElement(root()["elements"][root()["elements"].size()], "page", "", label);
}


FormElement FormElement::addSection(const string& label) 
{
	return FormElement(root()["elements"][root()["elements"].size()], "section", "", label);
}


FormField FormElement::addField(const string& type, const string& id, const string& label)
{
	assert(
		type == "text" ||
		type == "text-multi" ||
		type == "list" ||
		type == "list-multi" ||
		type == "boolean" ||
		type == "media" ||
		type == "custom"
	);

	return FormField(root()["elements"][root()["elements"].size()], type, id, label);
}


void FormElement::clear()
{
	root().clear();
}


bool FormElement::valid() const
{
	return _root != NULL; 
}

	
bool FormElement::hasErrors()
{
	return JSON::hasNestedKey(root(), "error");
}


bool FormElement::getField(const std::string& id, FormField& field, bool partial)
{	
	return JSON::findNestedObjectWithProperty(root(), field._root, "id", id, partial);
}


FormField FormElement::getField(const string& id, bool partial)
{
	FormField field;
	JSON::findNestedObjectWithProperty(root(), field._root, "id", id, partial);
	return field;
}


bool FormElement::hasField(const string& id, bool partial)
{
	JSON::Value* tmp = NULL;
	return JSON::findNestedObjectWithProperty(root(), tmp, "id", id, partial);
}


bool FormElement::hasPages()
{
	JSON::Value* tmp = NULL;
	return JSON::findNestedObjectWithProperty(root(), tmp, "type", "page");
}


JSON::Value& FormElement::root() const 
{
	if (_root == NULL)
		throw Exception("Form root is unassigned");
	return *_root;
}


// ---------------------------------------------------------------------
//
// Form Field
//
// ---------------------------------------------------------------------
FormField::FormField()
{
}


FormField::FormField(JSON::Value& root, const string& type, const string& id, const string& label) :
	FormElement(root, type, id, label)
{
}


FormField::~FormField()
{
}

	
JSON::Value& FormField::values()
{
	return root()["values"];
}

	
std::string FormField::value() const
{
	return root()["values"][(size_t)0].asString();
}

	
int FormField::intValue() const
{
	return Util::atoi(value());
}

	
double FormField::doubleValue() const
{
	return Util::atod(value());
}

	
bool FormField::boolValue() const
{
	string val = value();
	return val == "1" || val == "true" ? true : false;
}


void FormField::addOption(const string& value)
{
	root()["options"][value] = value;
}


void FormField::addOption(const string& key, const string& value)
{
	root()["options"][key] = value;
}


void FormField::setValue(const string& value)
{
	root()["values"].clear();
	root()["values"].append(value);
}


void FormField::setValue(int value)
{
	setValue(Util::tostr<int>(value));
}


void FormField::setValue(double value)
{
	setValue(Util::dtoa(value));
}


void FormField::setValue(bool value)
{
	setValue(string(value ? "1" : "0"));
}


void FormField::addValue(const string& value)
{
	root()["values"].append(value);
}


void FormField::addValue(int value)
{
	addValue(Util::tostr<int>(value));
}


void FormField::addValue(double value)
{
	addValue(Util::dtoa(value));
}


void FormField::addValue(bool value)
{
	addValue(string(value ? "1" : "0"));
}


} // namespace Symple 
} // namespace Sourcey