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


#ifndef SOURCEY_MEDIA_FormatRegistry_H
#define SOURCEY_MEDIA_FormatRegistry_H


#include "Sourcey/Media/Format.h"
#include "Poco/Mutex.h"

#include <vector>


namespace Sourcey {
namespace Media {


class FormatRegistry 
{
public:
	static FormatRegistry& instance();

	FormatRegistry();
	virtual ~FormatRegistry();
	
    virtual Format& get(const std::string& label);
    virtual Format& getByID(const std::string& id);
    virtual Format& getOrDefault(const std::string& label);
    virtual Format& getDefault();
		/// Returns the default media format.
		/// If a default has been specified it will be
		/// returned, other the format with the highest
		/// priority will take precedence.
	
    virtual void registerFormat(const Format& format);
		/// Registers the given media format overriding 
		/// existing media formats of the same label.

    virtual bool unregisterFormat(const std::string& label);
		/// Unregisters the media format matching the
		/// given label. 

    virtual void setDefault(const std::string& label);
		/// Sets the default fallback media format.

    virtual bool exists(const std::string& label);
    virtual void clear();

    virtual FormatList formats() const;

private:
	FormatRegistry(FormatRegistry const&) {};
	FormatRegistry& operator=(FormatRegistry const&) {};
	
    FormatList _formats;
    std::string _default;
	mutable Poco::FastMutex _mutex;
};


} } // namespace Sourcey::Media


#endif // SOURCEY_MEDIA_FormatRegistry_H