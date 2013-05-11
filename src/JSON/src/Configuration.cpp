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


#include "Sourcey/JSON/Configuration.h"
#include "Sourcey/Logger.h"
#include "Poco/String.h"
#include "Poco/File.h"


using namespace std;
using namespace Poco;


namespace Scy { 
namespace JSON {


Configuration::Configuration() :
	_loaded(false)
{	
}


Configuration::~Configuration()
{
}


void Configuration::load(const std::string& path, bool create)
{
	_path = path;
	load(create);
}


void Configuration::load(bool create)
{
	FastMutex::ScopedLock lock(_mutex); 

	if (_path.empty())
		throw Exception("Cannot load: Configuration file path must be set.");

	LogDebug() << "[JSONConfiguration] Loading: " << _path << endl;
	
	if (create && !File(_path).exists())
		File(_path).createFile();
	
	try {	
		JSON::loadFile(*this, _path);
	}
    catch (...) {
		// The config file may be empty,
		// but the path is set so we can save.
    }

	_loaded = true;
}


void Configuration::save()
{
	FastMutex::ScopedLock lock(_mutex); 
	
	if (_path.empty())
		throw Exception("Cannot save: Configuration file path must be set.");

	LogDebug() << "[JSONConfiguration] Saving: " << _path << endl;
	
	// Will throw on error
	JSON::saveFile(*this, _path);
}


std::string Configuration::path()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _path;
}


bool Configuration::loaded()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _loaded;
}


void Configuration::print(ostream& ost) 
{
	JSON::StyledWriter writer;
	ost << writer.write(*this);
}


bool Configuration::remove(const string& key)
{
	FastMutex::ScopedLock lock(_mutex); 
	
	return removeMember(key) != Json::nullValue;
}


void Configuration::removeAll(const std::string& baseKey)
{
	LogTrace() << "Removing All: " << baseKey << endl;
	FastMutex::ScopedLock lock(_mutex); 	
	
    Members members = this->getMemberNames();
	for (unsigned i = 0; i < members.size(); i++) {
		if (members[i].find(baseKey) != std::string::npos)
			removeMember(members[i]);
	}
}


void Configuration::replace(const string& from, const string& to)
{
	FastMutex::ScopedLock lock(_mutex); 

	stringstream ss;
	JSON::StyledWriter writer;
	string data = writer.write(*this);
	Poco::replaceInPlace(data, from, to);
	ss.str(data);

	JSON::Reader reader;
	reader.parse(data, *this);
}


bool Configuration::getRaw(const string& key, string& value) const
{	
	FastMutex::ScopedLock lock(_mutex); 
	
	if (!isMember(key))
		return false;

	value = (*this)[key].asString();
	return true;
}


void Configuration::setRaw(const string& key, const string& value)
{	
	{
		FastMutex::ScopedLock lock(_mutex); 
		(*this)[key] = value;
	}
}


void Configuration::keys(StringVec& keys, const std::string& baseKey)
{
	FastMutex::ScopedLock lock(_mutex); 
		
    Members members = this->getMemberNames();
	for (unsigned i = 0; i < members.size(); i++) {
		if (members[i].find(baseKey) != std::string::npos)
			keys.push_back(members[i]);
	}
}


} } // namespace Scy::JSON