///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup symple
/// @{


#include "scy/symple/form.h"
#include "scy/util.h"
#include "assert.h"


using std::endl;



namespace scy {
namespace smpl {


Form::Form()
{
}


Form::Form(Command& root) :
    FormElement(root["form"])
{
    root["form"]["type"] = "form";
}


Form::Form(json::Value& root) :
    FormElement(root)
{
    root["type"] = "form";
}


Form::~Form()
{
}


std::string Form::action() const
{
    return root().get("action", "form").asString();
}


bool Form::partial() const
{
    return root()["partial"].asBool();
}


void Form::setPartial(bool flag)
{
    root()["partial"] = flag;
}


void Form::setAction(const std::string& action)
{
    assert(
        action == "form" ||
        action == "submit" ||
        action == "cancel" ||
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
// Form Element
//
FormElement::FormElement() :
    _root(nullptr)
{
}


FormElement::FormElement(json::Value& root, const std::string& type, const std::string& id, const std::string& label) :
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


std::string FormElement::type() const
{
    return root()["type"].asString();
}


std::string FormElement::id() const
{
    return root()["id"].asString();
}


std::string FormElement::label() const
{
    return root()["label"].asString();
}


void FormElement::setType(const std::string& type)
{
    root()["type"] = type;
}


void FormElement::setId(const std::string& id)
{
    root()["id"] = id;
}


void FormElement::setLabel(const std::string& text)
{
    root()["label"] = text;
}


void FormElement::setHint(const std::string& text)
{
    root()["hint"] = text;
}


void FormElement::setError(const std::string& error)
{
    root()["error"] = error;
}


FormElement FormElement::addPage(const std::string& id, const std::string& label)
{
    return FormElement(root()["elements"][root()["elements"].size()], "page", id, label);
}


FormElement FormElement::addSection(const std::string& id, const std::string& label)
{
    return FormElement(root()["elements"][root()["elements"].size()], "section", id, label);
}


FormField FormElement::addField(const std::string& type, const std::string& id, const std::string& label)
{
    assert(
        type == "text" ||
        type == "text-multi" ||
        type == "list" ||
        type == "list-multi" ||
        type == "boolean" ||
        type == "number" ||
        type == "media" ||
        type == "date" ||
        type == "time" ||
        type == "datetime" ||
        type == "horizontal-set" ||
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


int FormElement::numElements()
{
    return root()["elements"].size();
}


bool FormElement::hasErrors()
{
    return json::hasNestedKey(root(), "error");
}


bool FormElement::live() const
{
    return root()["live"].asBool();
}


void FormElement::setLive(bool flag)
{
    root()["live"] = flag;
}



bool FormElement::clearElements(const std::string& id, bool partial)
{
    //json::Value& root() = section.root()();
    bool match = false;
    json::Value result;
    json::Value::Members members = root().getMemberNames();
    for (unsigned i = 0; i < members.size(); i++) {

        // Filter elements
        if (members[i] == "elements") {
            for (unsigned i = 0; i < root()["elements"].size(); i++) {
                json::Value& element =  root()["elements"][i];
                std::string curID = element["id"].asString();
                if (//element.isObject() &&
                    //element.isMember("id") &&
                    partial ?
                        curID.find(id) != std::string::npos :
                        curID == id) {
                    TraceL << "Symple form: Removing redundant: " << curID << endl;
                    match = true;
                }
                else {
                    TraceL << "Symple form: Keeping: " << curID << endl;
                    result["elements"].append(element);
                }
            }
        }

        // Keep other members
        else
            result[members[i]] = root()[members[i]];
    }

    *_root = result;
    return match;
}


bool FormElement::getField(const std::string& id, FormField& field, bool partial)
{
    return json::findNestedObjectWithProperty(root(), field._root, "id", id, partial);
}


FormField FormElement::getField(const std::string& id, bool partial)
{
    FormField field;
    json::findNestedObjectWithProperty(root(), field._root, "id", id, partial);
    return field;
}


bool FormElement::hasField(const std::string& id, bool partial)
{
    json::Value* tmp = NULL;
    return json::findNestedObjectWithProperty(root(), tmp, "id", id, partial);
}


bool FormElement::hasPages()
{
    json::Value* tmp = NULL;
    return json::findNestedObjectWithProperty(root(), tmp, "type", "page");
}


json::Value& FormElement::root() const
{
    if (_root == NULL)
        throw std::runtime_error("Form root is unassigned");
    return *_root;
}


// ---------------------------------------------------------------------
// Form Field
//
FormField::FormField()
{
}


FormField::FormField(json::Value& root, const std::string& type, const std::string& id, const std::string& label) :
    FormElement(root, type, id, label)
{
}


FormField::~FormField()
{
}


json::Value& FormField::values()
{
    return root()["values"];
}


std::string FormField::value() const
{
    return root()["values"][(unsigned)0].asString();
}


int FormField::intValue() const
{
    return util::strtoi<std::uint32_t>(value());
}


double FormField::doubleValue() const
{
    return util::strtoi<double>(value());
}


bool FormField::boolValue() const
{
    std::string val = value();
    return val == "1" || val == "true" || val == "on" ? true : false;
}


void FormField::addOption(const std::string& value)
{
    root()["options"][value] = value;
}


void FormField::addOption(const std::string& key, const std::string& value)
{
    root()["options"][key] = value;
}


void FormField::setValue(const std::string& value)
{
    root()["values"].clear();
    root()["values"].append(value);
}


void FormField::setValue(int value)
{
    setValue(util::itostr<int>(value));
}


void FormField::setValue(double value)
{
    setValue(util::itostr<double>(value));
}


void FormField::setValue(bool value)
{
    setValue(std::string(value ? "1" : "0"));
}


void FormField::addValue(const std::string& value)
{
    root()["values"].append(value);
}


void FormField::addValue(int value)
{
    addValue(util::itostr<int>(value));
}


void FormField::addValue(double value)
{
    addValue(util::itostr<double>(value));
}


void FormField::addValue(bool value)
{
    addValue(std::string(value ? "1" : "0"));
}


} // namespace smpl
} // namespace scy

/// @\}
