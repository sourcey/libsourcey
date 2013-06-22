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


#include "Sourcey/IConfiguration.h"

#include "Poco/Exception.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"


using namespace std;
using namespace Poco;


namespace scy {


IConfiguration::IConfiguration()
{	
}


IConfiguration::~IConfiguration()
{
}


bool IConfiguration::exists(const std::string& key) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	return getRaw(key, value);
}

	
std::string IConfiguration::getString(const std::string& key) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		throw NotFoundException(key);
}

	
std::string IConfiguration::getString(const std::string& key, const std::string& defaultValue) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		return defaultValue;
}


std::string IConfiguration::getRawString(const std::string& key) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		throw NotFoundException(key);
}

	
std::string IConfiguration::getRawString(const std::string& key, const std::string& defaultValue) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		return defaultValue;
}

	
int IConfiguration::getInt(const std::string& key) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(value);
	else
		throw NotFoundException(key);
}

	
int IConfiguration::getInt(const std::string& key, int defaultValue) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(value);
	else
		return defaultValue;
}


double IConfiguration::getDouble(const std::string& key) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return NumberParser::parseFloat(value);
	else
		throw NotFoundException(key);
}

	
double IConfiguration::getDouble(const std::string& key, double defaultValue) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return NumberParser::parseFloat(value);
	else
		return defaultValue;
}


bool IConfiguration::getBool(const std::string& key) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseBool(value);
	else
		throw NotFoundException(key);
}


bool IConfiguration::getBool(const std::string& key, bool defaultValue) const
{
	FastMutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseBool(value);
	else
		return defaultValue;
}


void IConfiguration::setString(const std::string& key, const std::string& value)
{
	FastMutex::ScopedLock lock(_mutex);

	setRaw(key, value);
}

	
void IConfiguration::setInt(const std::string& key, int value)
{
	FastMutex::ScopedLock lock(_mutex);

	setRaw(key, NumberFormatter::format(value));
}


void IConfiguration::setDouble(const std::string& key, double value)
{
	FastMutex::ScopedLock lock(_mutex);

	setRaw(key, NumberFormatter::format(value));
}


void IConfiguration::setBool(const std::string& key, bool value)
{
	FastMutex::ScopedLock lock(_mutex);

	setRaw(key, value ? "true" : "false");
}


int IConfiguration::parseInt(const std::string& value)
{
	if (value.compare(0, 2, "0x") == 0)
		return NumberParser::parseHex(value.substr(2));
	else
		return NumberParser::parse(value);
}


bool IConfiguration::parseBool(const std::string& value)
{
	int n;
	if (NumberParser::tryParse(value, n))
		return n != 0;
	else if (icompare(value, "true") == 0)
		return true;
	else if (icompare(value, "yes") == 0)
		return true;
	else if (icompare(value, "on") == 0)
		return true;
	else if (icompare(value, "false") == 0)
		return false;
	else if (icompare(value, "no") == 0)
		return false;
	else if (icompare(value, "off") == 0)
		return false;
	else 
		throw SyntaxException("Cannot convert to boolean", value);
}


} // namespace scy