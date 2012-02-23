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


#include "Sourcey/XML/Configuration.h"
#include "Poco/String.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace XML {


Configuration::Configuration()
{	
	Log("debug") << "Creating" << endl;
}


Configuration::~Configuration()
{
	Log("debug") << "Destroying" << endl;
}


void Configuration::load(const std::string& path)
{
	_path = path;
	load();
}


void Configuration::load()
{
	FastMutex::ScopedLock lock(_mutex); 

	assert(!_path.empty());
	Log("debug") << "Loading: " << _path << endl;

	pugi::xml_parse_result result = load_file(_path.data()); 
	if (!result || child("config").empty()) {	
		result = static_cast<pugi::xml_document*>(this)->load("<config />");
		if (!result) {
			throw Exception(result.description()); 
		}
	}
}

bool Configuration::save()
{
	FastMutex::ScopedLock lock(_mutex); 

	Log("debug") << "Saving: " << _path << endl;

	if (!_path.empty())
		return false;
		
	return static_cast<pugi::xml_document*>(this)->save_file(_path.data());
}


void Configuration::print(ostream& ost) 
{
	print(ost);
}


bool Configuration::remove(const string& key) //, bool exactMatch
{
	FastMutex::ScopedLock lock(_mutex); 
	
    XML::Node property = child("config").find_child_by_attribute("property", "name", key.data());
	if (!property.empty()) {
		child("config").remove_child(property);
		return true;
	}
	return false;
}


void Configuration::removeAll(const std::string& baseKey)
{
	Log("debug") << "Removing All: " << baseKey << endl;
	FastMutex::ScopedLock lock(_mutex); 

	pugi::xpath_node_set properties = select_nodes(format("//property[contains(@name, '%s')]", baseKey).data());
    for (pugi::xpath_node_set::const_iterator it = properties.begin(); it != properties.end(); ++it) {
		Log("debug") << "Removing: " << (*it).node().attribute("name").value() << endl;
		child("config").remove_child((*it).node());
	}
}


void Configuration::replace(const string& from, const string& to)
{
	FastMutex::ScopedLock lock(_mutex); 

	stringstream ss;
	print(ss);	
	string data = ss.str();
	Poco::replaceInPlace(data, from, to);
	ss.str(data);
	static_cast<pugi::xml_document*>(this)->load(ss);
}


bool Configuration::getRaw(const string& key, string& value) const
{	
	FastMutex::ScopedLock lock(_mutex); 
	
	//Log("debug") << "Get Raw: " << key << ": " << value << endl;
	
    XML::Node property = child("config").find_child_by_attribute("property", "name", key.data());
	if (!property.empty()) {
		value = property.child_value();
		return true;
	}
	return false;
}


void Configuration::setRaw(const string& key, const string& value)
{	
	Log("debug") << "Set Raw: " << key << ": " << value << endl;
	{
		FastMutex::ScopedLock lock(_mutex); 

		XML::Node property = child("config").find_child_by_attribute("property", "name", key.data());
		if (property.empty()) {
			property = child("config").append_child();
			property.append_attribute("name").set_value(key.data());
			property.set_name("property");
			property.append_child(pugi::node_pcdata).set_value(value.data());
		} 
		else
			property.first_child().set_value(value.data());
	}

	// The base class triggers the ConfigurationChanged callback.
	IConfiguration::setRaw(key, value);
}


void Configuration::keys(StringList& keys, const std::string& baseKey)
{
	FastMutex::ScopedLock lock(_mutex); 

	pugi::xpath_node_set properties = select_nodes(format("//property[contains(@name, '%s')]", baseKey).data());
    for (pugi::xpath_node_set::const_iterator it = properties.begin(); it != properties.end(); ++it) {
		string value = (*it).node().attribute("name").value();
		Log("debug") << "Keys: " << baseKey << ": " << value << endl;
		keys.push_back(value);
	}
}


} } // namespace Sourcey::XML

