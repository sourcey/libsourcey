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


#ifndef SCY_MEDIA_FormatRegistry_H
#define SCY_MEDIA_FormatRegistry_H


#include "scy/media/format.h"
#include "scy/mutex.h"

#include <vector>


namespace scy {
namespace av {


class FormatRegistry 
{
public:
	static FormatRegistry& instance();

	FormatRegistry();
	virtual ~FormatRegistry();
	
    virtual Format& get(const std::string& name);
    virtual Format& getByID(const std::string& id);
    virtual Format& getOrDefault(const std::string& name);
    virtual Format& getDefault();
		// Returns the default media format.
		// If a default has been specified it will be
		// returned, other the format with the highest
		// priority will take precedence.
	
    virtual void registerFormat(const Format& format);
		// Registers the given media format overriding 
		// existing media formats of the same name.

    virtual bool unregisterFormat(const std::string& name);
		// Unregisters the media format matching the
		// given name. 

    virtual void setDefault(const std::string& name);
		// Sets the default fallback media format.

    virtual bool exists(const std::string& name);
    virtual void clear();

    virtual FormatList formats() const;

private:
	FormatRegistry(FormatRegistry const&) {};
	//FormatRegistry& operator=(FormatRegistry const&) {};
	
    FormatList _formats;
    std::string _default;
	mutable Mutex _mutex;
};


} } // namespace scy::av


#endif // SCY_MEDIA_FormatRegistry_H
