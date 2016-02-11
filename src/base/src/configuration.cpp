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


#include "scy/configuration.h"
#include "scy/util.h"
#include "scy/exception.h"


namespace scy {


Configuration::Configuration()
{    
}


Configuration::~Configuration()
{
}


bool Configuration::exists(const std::string& key) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string tmp;
    return getRaw(key, tmp);
}



std::string Configuration::getString(const std::string& key) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return value;
    else
        throw std::invalid_argument("Not found: " + key);
}

    
std::string Configuration::getString(const std::string& key, const std::string& defaultValue) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return value;
    else
        return defaultValue;
}


std::string Configuration::getRawString(const std::string& key) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return value;
    else
        throw std::invalid_argument("Not found: " + key);
}

    
std::string Configuration::getRawString(const std::string& key, const std::string& defaultValue) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return value;
    else
        return defaultValue;
}

    
int Configuration::getInt(const std::string& key) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return parseInt(value);
    else
        throw std::invalid_argument("Not found: " + key);
}

    
int Configuration::getInt(const std::string& key, int defaultValue) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return parseInt(value);
    else
        return defaultValue;
}

    
std::int64_t Configuration::getLargeInt(const std::string& key) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return parseLargeInt(value);
    else
        throw std::invalid_argument("Not found: " + key);
}

    
std::int64_t Configuration::getLargeInt(const std::string& key, std::int64_t defaultValue) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return parseLargeInt(value);
    else
        return defaultValue;
}


double Configuration::getDouble(const std::string& key) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return util::strtoi<double>(value);
    else
        throw std::invalid_argument("Not found: " + key);
}

    
double Configuration::getDouble(const std::string& key, double defaultValue) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return util::strtoi<double>(value);
    else
        return defaultValue;
}


bool Configuration::getBool(const std::string& key) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return parseBool(value);
    else
        throw std::invalid_argument("Not found: " + key);
}


bool Configuration::getBool(const std::string& key, bool defaultValue) const
{
    Mutex::ScopedLock lock(_mutex);

    std::string value;
    if (getRaw(key, value))
        return parseBool(value);
    else
        return defaultValue;
}


void Configuration::setString(const std::string& key, const std::string& value)
{
    Mutex::ScopedLock lock(_mutex);

    setRaw(key, value);
}

    
void Configuration::setInt(const std::string& key, int value)
{
    Mutex::ScopedLock lock(_mutex);

    setRaw(key, util::itostr<int>(value));
}

    
void Configuration::setLargeInt(const std::string& key, std::int64_t value)
{
    Mutex::ScopedLock lock(_mutex);

    setRaw(key, util::itostr<std::int64_t>(value));
}


void Configuration::setDouble(const std::string& key, double value)
{
    Mutex::ScopedLock lock(_mutex);

    setRaw(key, util::itostr<double>(value));
}


void Configuration::setBool(const std::string& key, bool value)
{
    Mutex::ScopedLock lock(_mutex);

    setRaw(key, value ? "true" : "false");
}


int Configuration::parseInt(const std::string& value)
{
    if (value.compare(0, 2, "0x") == 0)
        return util::parseHex(value.substr(2));
    else
        return util::strtoi<int>(value);
}


std::int64_t Configuration::parseLargeInt(const std::string& value)
{
    if (value.compare(0, 2, "0x") == 0)
        return util::parseHex(value.substr(2));
    else
        return util::strtoi<std::int64_t>(value);
}


bool Configuration::parseBool(const std::string& value)
{
    if (value == "0")
        return false;
    else if (value == "1")
        return true;
    else if (util::icompare(value, "true") == 0)
        return true;
    else if (util::icompare(value, "yes") == 0)
        return true;
    else if (util::icompare(value, "on") == 0)
        return true;
    else if (util::icompare(value, "false") == 0)
        return false;
    else if (util::icompare(value, "no") == 0)
        return false;
    else if (util::icompare(value, "off") == 0)
        return false;
    else 
        throw std::runtime_error("Syntax error: Cannot convert to boolean: " + value);
}


//
// Scoped Configuration
//


ScopedConfiguration::ScopedConfiguration(Configuration& config, const std::string& currentScope, const std::string& defaultScope) :
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
    

/*
ScopedConfiguration& ScopedConfiguration::operator = (const ScopedConfiguration& that)
{
    if (&that != this) {
        config = that.config;
        currentScope = that.currentScope;
        defaultScope = that.defaultScope;
    }
    return *this;
}
*/


std::string ScopedConfiguration::getString(const std::string& key, const std::string& defaultValue, bool forceDefaultScope) const 
{
    return forceDefaultScope ? 
        config.getString(getDafaultKey(key), defaultValue) :
            config.getString(getCurrentScope(key), 
                config.getString(getDafaultKey(key), defaultValue));
}


int ScopedConfiguration::getInt(const std::string& key, int defaultValue, bool forceDefaultScope) const 
{
    return forceDefaultScope ?
        config.getInt(getDafaultKey(key), defaultValue) :
            config.getInt(getCurrentScope(key), 
                config.getInt(getDafaultKey(key), defaultValue));
}


double ScopedConfiguration::getDouble(const std::string& key, double defaultValue, bool forceDefaultScope) const 
{
    return forceDefaultScope ? 
        config.getDouble(getDafaultKey(key), defaultValue) :
            config.getDouble(getCurrentScope(key), 
                config.getDouble(getDafaultKey(key), defaultValue));
}


bool ScopedConfiguration::getBool(const std::string& key, bool defaultValue, bool forceDefaultScope) const 
{
    return forceDefaultScope ? 
        config.getBool(getDafaultKey(key), defaultValue) :
            config.getBool(getCurrentScope(key), 
                config.getBool(getDafaultKey(key), defaultValue));
}


void ScopedConfiguration::setString(const std::string& key, const std::string& value, bool defaultScope) 
{
    config.setString(getScopedKey(key, defaultScope), value);
}


void ScopedConfiguration::setInt(const std::string& key, int value, bool defaultScope) 
{    
    config.setInt(getScopedKey(key, defaultScope), value);
}


void ScopedConfiguration::setDouble(const std::string& key, double value, bool defaultScope) 
{
    config.setDouble(getScopedKey(key, defaultScope), value);
}


void ScopedConfiguration::setBool(const std::string& key, bool value, bool defaultScope) 
{
    config.setBool(getScopedKey(key, defaultScope), value);
}


std::string ScopedConfiguration::getCurrentScope(const std::string& key) const
{    
    return currentScope + key;
}


std::string ScopedConfiguration::getDafaultKey(const std::string& key) const
{
    return defaultScope + key;
}


std::string ScopedConfiguration::getScopedKey(const std::string& key, bool defaultScope) const
{
    return defaultScope ? getDafaultKey(key) : getCurrentScope(key);
}


} // namespace scy
