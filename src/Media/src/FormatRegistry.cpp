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


#include "Sourcey/Media/FormatRegistry.h"

#include "Sourcey/Singleton.h"
//#include "Poco/Format.h"


using namespace std;


namespace scy {
namespace av {


FormatRegistry::FormatRegistry()
{
}


FormatRegistry::~FormatRegistry()
{
}


FormatRegistry& FormatRegistry::instance() 
{
	static Singleton<FormatRegistry> sh;
	return *sh.get();
}


Format& FormatRegistry::get(const std::string& name) 
{
	Mutex::ScopedLock lock(_mutex);
	for (unsigned int i = 0; i < _formats.size(); i++) {
		if (_formats[i].name == name) {
			return _formats[i];
		}
	}
           
	throw NotFoundException("No media format for: " + name);
}


Format& FormatRegistry::getByID(const std::string& id) 
{
	Mutex::ScopedLock lock(_mutex);
	for (unsigned int i = 0; i < _formats.size(); i++) {
		if (_formats[i].id == id) {
			return _formats[i];
		}
	}
           
	throw NotFoundException("No media format type: " + id);
}


Format& FormatRegistry::getOrDefault(const std::string& name) 
{
	{
		Mutex::ScopedLock lock(_mutex);
		for (unsigned int i = 0; i < _formats.size(); i++) {
			if (_formats[i].name == name) {
				return _formats[i];
			}
		}
	}
           
	return getDefault();
}


Format& FormatRegistry::getDefault() 
{
	Mutex::ScopedLock lock(_mutex);
	if (!_default.empty()) {
		return get(_default);
	}
	else if (!_formats.empty()) {
		return *_formats.begin();
	}
         
	throw NotFoundException("No default media format.");
}


bool FormatRegistry::exists(const std::string& name)
{
	Mutex::ScopedLock lock(_mutex);
	for (unsigned int i = 0; i < _formats.size(); i++) {
		if (_formats[i].name == name) {
			return true;
		}
	}
           
	return false;
}


void FormatRegistry::clear()
{
	Mutex::ScopedLock lock(_mutex);
	_formats.clear();
}


FormatList FormatRegistry::formats() const
{ 
	Mutex::ScopedLock lock(_mutex);
	return _formats; 
}


void FormatRegistry::registerFormat(const Format& format)	
{ 
	unregisterFormat(format.name);
	Mutex::ScopedLock lock(_mutex);
    _formats.push_back(format);
}


bool FormatRegistry::unregisterFormat(const std::string& name)	
{ 
	Mutex::ScopedLock lock(_mutex);
	for (FormatList::iterator it = _formats.begin(); it != _formats.end(); ++it) {
		if ((*it).name == name) {
			_formats.erase(it);
			if (_default == name)
				_default = "";
			return true;
		}
	}
	return false;
}


void FormatRegistry::setDefault(const std::string& name)
{
	Mutex::ScopedLock lock(_mutex);
	_default = name;
}


} } // namespace scy::av
