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


#include "Sourcey/Util/ScopedConfiguration.h"


using namespace std;


namespace Scy {


ScopedConfiguration::ScopedConfiguration(IConfiguration& config, const string& currentScope, const string& defaultScope) :
	config(config),
	currentScope(currentScope),
	defaultScope(defaultScope)
{
}


ScopedConfiguration::ScopedConfiguration(const ScopedConfiguration& r) :
	config(r.config),
	currentScope(r.currentScope),
	defaultScope(r.defaultScope)
{
}
	

string ScopedConfiguration::getString(const string& key, const string& defaultValue, bool forceDefaultScope) const 
{
	return forceDefaultScope ? 
		config.getString(getDafaultKey(key), defaultValue) :
			config.getString(getCurrentScope(key), 
				config.getString(getDafaultKey(key), defaultValue));
}


int ScopedConfiguration::getInt(const string& key, int defaultValue, bool forceDefaultScope) const 
{
	return forceDefaultScope ?
		config.getInt(getDafaultKey(key), defaultValue) :
			config.getInt(getCurrentScope(key), 
				config.getInt(getDafaultKey(key), defaultValue));
}


double ScopedConfiguration::getDouble(const string& key, double defaultValue, bool forceDefaultScope) const 
{
	return forceDefaultScope ? 
		config.getDouble(getDafaultKey(key), defaultValue) :
			config.getDouble(getCurrentScope(key), 
				config.getDouble(getDafaultKey(key), defaultValue));
}


bool ScopedConfiguration::getBool(const string& key, bool defaultValue, bool forceDefaultScope) const 
{
	return forceDefaultScope ? 
		config.getBool(getDafaultKey(key), defaultValue) :
			config.getBool(getCurrentScope(key), 
				config.getBool(getDafaultKey(key), defaultValue));
}


void ScopedConfiguration::setString(const string& key, const string& value, bool defaultScope) 
{
	config.setString(getScopedKey(key, defaultScope), value);
}


void ScopedConfiguration::setInt(const string& key, int value, bool defaultScope) 
{	
	config.setInt(getScopedKey(key, defaultScope), value);
}


void ScopedConfiguration::setDouble(const string& key, double value, bool defaultScope) 
{
	config.setDouble(getScopedKey(key, defaultScope), value);
}


void ScopedConfiguration::setBool(const string& key, bool value, bool defaultScope) 
{
	config.setBool(getScopedKey(key, defaultScope), value);
}


string ScopedConfiguration::getCurrentScope(const string& key) const
{	
	return currentScope + key;
}


string ScopedConfiguration::getDafaultKey(const string& key) const
{
	return defaultScope + key;
}


string ScopedConfiguration::getScopedKey(const string& key, bool defaultScope) const
{
	return defaultScope ? getDafaultKey(key) : getCurrentScope(key);
}


} // namespace Scy
