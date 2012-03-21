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


#include "Sourcey/Media/FormatRegistry.h"

#include "Poco/SingletonHolder.h"
#include "Poco/Format.h"


using namespace std;
using Poco::SingletonHolder;
using Poco::FastMutex;
using Poco::format;


namespace Sourcey {
namespace Media {


FormatRegistry::FormatRegistry()
{
}


FormatRegistry::~FormatRegistry()
{
}


FormatRegistry& FormatRegistry::instance() 
{
	static SingletonHolder<FormatRegistry> sh;
	return *sh.get();
}


Format& FormatRegistry::get(const string& label) 
{
	FastMutex::ScopedLock lock(_mutex);
	for (unsigned int i = 0; i < _formats.size(); i++) {
		if (_formats[i].label == label) {
			return _formats[i];
		}
	}
           
	throw Poco::NotFoundException("No media format for: " + label);
}


Format& FormatRegistry::get(UInt32 id) 
{
	FastMutex::ScopedLock lock(_mutex);
	for (unsigned int i = 0; i < _formats.size(); i++) {
		if (_formats[i].id == id) {
			return _formats[i];
		}
	}
           
	throw Poco::NotFoundException(Poco::format("No media format type: %d", (int)id));
}


Format& FormatRegistry::getOrDefault(const string& label) 
{
	{
		FastMutex::ScopedLock lock(_mutex);
		for (unsigned int i = 0; i < _formats.size(); i++) {
			if (_formats[i].label == label) {
				return _formats[i];
			}
		}
	}
           
	return getDefault();
}


Format& FormatRegistry::getDefault() 
{
	FastMutex::ScopedLock lock(_mutex);
	if (!_default.empty()) {
		return get(_default);
	}
	else if (!_formats.empty()) {
		return *_formats.begin();
	}
         
	throw Poco::NotFoundException("No default media format.");
}


bool FormatRegistry::exists(const string& label)
{
	FastMutex::ScopedLock lock(_mutex);
	for (unsigned int i = 0; i < _formats.size(); i++) {
		if (_formats[i].label == label) {
			return true;
		}
	}
           
	return false;
}


FormatList FormatRegistry::formats() const
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _formats; 
}


void FormatRegistry::registerFormat(const Format& format)	
{ 
	FastMutex::ScopedLock lock(_mutex);
    _formats.push_back(format);
}


void FormatRegistry::unregisterFormat(const string& label)	
{ 
	FastMutex::ScopedLock lock(_mutex);
	for (FormatList::iterator it = _formats.begin(); it != _formats.end(); ++it) {
		if ((*it).label == label) {
			_formats.erase(it);
			if (_default == label)
				_default = "";
			return;
		}
	}
}


void FormatRegistry::setDefault(const string& label)
{
	FastMutex::ScopedLock lock(_mutex);
	_default = label;
}


} } // namespace Sourcey::Media
