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


#ifndef SOURCEY_XML_Configuration_H
#define SOURCEY_XML_Configuration_H


#include "Sourcey/IConfiguration.h"
#include "Sourcey/XML/XML.h"


namespace Sourcey { 
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
	virtual void keys(StringList& keys, const std::string& baseKey = "");
	virtual void print(std::ostream& ost);

	// See IConfiguration for data accesors

protected:
	virtual bool getRaw(const std::string& key, std::string& value) const;
	virtual void setRaw(const std::string& key, const std::string& value);

	std::string _path;
	mutable Poco::FastMutex _mutex;
};


} } // namespace Sourcey::XML


#endif // SOURCEY_XML_Configuration_H