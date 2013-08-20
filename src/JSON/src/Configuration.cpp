//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "Sourcey/JSON/Configuration.h"
#include "Sourcey/Logger.h"
//#include "Poco/String.h"
//#include "Poco/File.h"


using namespace std;



namespace scy { 
namespace json {


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


void Configuration::load(bool /* create */)
{
	Mutex::ScopedLock lock(_mutex); 

	if (_path.empty())
		throw std::runtime_error("Cannot load configuration: File path not set.");

	debugL("JSONConfiguration") << " Loading: " << _path << endl;
	
	try {	
		//if (create && !fs::exists(_path))
		//	fs::createFile(_path);

		json::loadFile(*this, _path);
	}
    catch (...) {
		// The config file may be empty,
		// but the path is set so we can save.
    }

	_loaded = true;
}


void Configuration::save()
{
	Mutex::ScopedLock lock(_mutex); 
	
	if (_path.empty())
		throw std::runtime_error("Cannot save: Configuration file path must be set.");

	debugL("JSONConfiguration") << " Saving: " << _path << endl;
	
	// Will throw on error
	json::saveFile(*this, _path);
}


std::string Configuration::path()
{
	Mutex::ScopedLock lock(_mutex); 
	return _path;
}


bool Configuration::loaded()
{
	Mutex::ScopedLock lock(_mutex); 
	return _loaded;
}


void Configuration::print(ostream& ost) 
{
	json::StyledWriter writer;
	ost << writer.write(*this);
}


bool Configuration::remove(const std::string& key)
{
	Mutex::ScopedLock lock(_mutex); 
	
	return removeMember(key) != Json::nullValue;
}


void Configuration::removeAll(const std::string& baseKey)
{
	traceL() << "Removing All: " << baseKey << endl;
	Mutex::ScopedLock lock(_mutex); 	
	
    Members members = this->getMemberNames();
	for (unsigned i = 0; i < members.size(); i++) {
		if (members[i].find(baseKey) != std::string::npos)
			removeMember(members[i]);
	}
}


void Configuration::replace(const std::string& from, const std::string& to)
{
	Mutex::ScopedLock lock(_mutex); 

	std::stringstream ss;
	json::StyledWriter writer;
	std::string data = writer.write(*this);
	util::replaceInPlace(data, from, to);
	ss.str(data);

	json::Reader reader;
	reader.parse(data, *this);
}


bool Configuration::getRaw(const std::string& key, std::string& value) const
{	
	Mutex::ScopedLock lock(_mutex); 
	
	if (!isMember(key))
		return false;

	value = (*this)[key].asString();
	return true;
}


void Configuration::setRaw(const std::string& key, const std::string& value)
{	
	{
		Mutex::ScopedLock lock(_mutex); 
		(*this)[key] = value;
	}
	PropertyChanged.emit(this, key, value);
}


void Configuration::keys(std::vector<std::string>& keys, const std::string& baseKey)
{
	Mutex::ScopedLock lock(_mutex); 
		
    Members members = this->getMemberNames();
	for (unsigned i = 0; i < members.size(); i++) {
		if (members[i].find(baseKey) != std::string::npos)
			keys.push_back(members[i]);
	}
}


} } // namespace scy::json