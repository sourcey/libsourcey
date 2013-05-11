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


#ifndef SOURCEY_ScopedConfiguration_H
#define SOURCEY_ScopedConfiguration_H


#include "Sourcey/IConfiguration.h"


namespace Scy {
	

class ScopedConfiguration 
	/// ScopedConfiguration provides multiple levels of configuration for a module.
	/// Multiple levels means that there is a module level scope, and a default scope.
	/// When a property is accessed, the module scope value will be used if available,
	/// otherwise the default scope value will be used.
	///
	/// Example scoping:
	///		Module: channels.[name].modes.[name].[value]
	///		Default: modes.[name].[value]
{	
public:
	ScopedConfiguration(IConfiguration& config, const std::string& moduleScope, const std::string& defaultScope);
	ScopedConfiguration(const ScopedConfiguration& r);

	std::string getString(const std::string& key, const std::string& defaultValue, bool forceDefaultScope = false) const;
	int getInt(const std::string& key, int defaultValue, bool forceDefaultScope = false) const;
	double getDouble(const std::string& key, double defaultValue, bool forceDefaultScope = false) const;
	bool getBool(const std::string& key, bool defaultValue, bool forceDefaultScope = false) const;

	void setString(const std::string& key, const std::string& value, bool defaultScope = false);
	void setInt(const std::string& key, int value, bool defaultScope = false);
	void setDouble(const std::string& key, double value, bool defaultScope = false);
	void setBool(const std::string& key, bool value, bool defaultScope = false);
	
	std::string getModuleScope(const std::string& key) const;
	std::string getDafaultKey(const std::string& key) const;
	std::string getScopedKey(const std::string& key, bool defaultScope = false) const;

	IConfiguration& config;
	std::string moduleScope;
	std::string defaultScope;

private:	
	ScopedConfiguration& operator=(const ScopedConfiguration&) {};
};


} // namespace Scy


#endif // SOURCEY_ScopedConfiguration_H