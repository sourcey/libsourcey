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


#ifndef SOURCEY_XML_Configuration_H
#define SOURCEY_XML_Configuration_H


#include "Sourcey/IConfiguration.h"
#include "Sourcey/XML/XML.h"


namespace Scy { 
namespace XML {


class Configuration: public IConfiguration, public XML::Document
{
public:
	Configuration();
	virtual ~Configuration();
	
	virtual void load(const std::string& path);
	virtual void load();
	virtual bool save();
	
	virtual bool remove(const std::string& key);
	virtual void removeAll(const std::string& baseKey);
	virtual void replace(const std::string& from, const std::string& to);
	virtual void keys(StringVec& keys, const std::string& baseKey = "");
	virtual void print(std::ostream& ost);

	// See IConfiguration for data accesors

protected:
	virtual bool getRaw(const std::string& key, std::string& value) const;
	virtual void setRaw(const std::string& key, const std::string& value);

	std::string _path;
	mutable Poco::FastMutex _mutex;
};


} } // namespace Scy::XML


#endif // SOURCEY_XML_Configuration_H