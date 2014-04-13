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


#ifndef SCY_Configuration_H
#define SCY_Configuration_H


#include "scy/signal.h"
#include "scy/mutex.h"


namespace scy {


class Configuration
	/// Configuration is an abstract base class for managing 
	/// different kinds of configuration storage back ends such as 
	/// JSON, XML, or database.
	///
	/// Subclasses must override the getRaw() and setRaw() and methods.
	///
	/// This class is safe for multithreaded use.
{
public:
	Configuration();	
		// Creates the Configuration.

	virtual ~Configuration();
		// Destroys the Configuration.

	bool exists(const std::string& key) const;
		// Returns true if the property with the given key exists.
		
	std::string getString(const std::string& key) const;
		// Returns the string value of the property with the given name.
		// Throws a NotFoundException if the key does not exist.
		
	std::string getString(const std::string& key, const std::string& defaultValue) const;
		// If a property with the given key exists, returns the property's string value,
		// otherwise returns the given default value.

	std::string getRawString(const std::string& key) const;
		// Returns the raw string value of the property with the given name.
		// Throws a NotFoundException if the key does not exist.
		// References to other properties are not expanded.
		
	std::string getRawString(const std::string& key, const std::string& defaultValue) const;
		// If a property with the given key exists, returns the property's raw string value,
		// otherwise returns the given default value.
		// References to other properties are not expanded.
		
	int getInt(const std::string& key) const;
		// Returns the int value of the property with the given name.
		// Throws a NotFoundException if the key does not exist.
		// Throws a SyntaxException if the property can not be converted
		// to an int.
		// Numbers starting with 0x are treated as hexadecimal.
		
	int getInt(const std::string& key, int defaultValue) const;
		// If a property with the given key exists, returns the property's int value,
		// otherwise returns the given default value.
		// Throws a SyntaxException if the property can not be converted
		// to an int.
		// Numbers starting with 0x are treated as hexadecimal.
		
	Int64 getLargeInt(const std::string& key) const;
		// Returns the int value of the property with the given name.
		// Throws a NotFoundException if the key does not exist.
		// Throws a SyntaxException if the property can not be converted
		// to an int.
		// Numbers starting with 0x are treated as hexadecimal.
		
	Int64 getLargeInt(const std::string& key, Int64 defaultValue) const;
		// If a property with the given key exists, returns the property's int value,
		// otherwise returns the given default value.
		// Throws a SyntaxException if the property can not be converted
		// to an int.
		// Numbers starting with 0x are treated as hexadecimal.

	double getDouble(const std::string& key) const;
		// Returns the double value of the property with the given name.
		// Throws a NotFoundException if the key does not exist.
		// Throws a SyntaxException if the property can not be converted
		// to a double.
		
	double getDouble(const std::string& key, double defaultValue) const;
		// If a property with the given key exists, returns the property's double value,
		// otherwise returns the given default value.
		// Throws a SyntaxException if the property can not be converted
		// to an double.

	bool getBool(const std::string& key) const;
		// Returns the double value of the property with the given name.
		// Throws a NotFoundException if the key does not exist.
		// Throws a SyntaxException if the property can not be converted
		// to a double.
		
	bool getBool(const std::string& key, bool defaultValue) const;
		// If a property with the given key exists, returns the property's bool value,
		// otherwise returns the given default value.
		// Throws a SyntaxException if the property can not be converted
		// to a boolean.
		// The following string values can be converted into a boolean:
		//   - numerical values: non zero becomes true, zero becomes false
		//   - strings: true, yes, on become true, false, no, off become false
		// Case does not matter.
		
	void setString(const std::string& key, const std::string& value);
		// Sets the property with the given key to the given value.
		// An already existing value for the key is overwritten.
		
	void setInt(const std::string& key, int value);
		// Sets the property with the given key to the given value.
		// An already existing value for the key is overwritten.
		
	void setLargeInt(const std::string& key, Int64 value);
		// Sets the property with the given key to the given value.
		// An already existing value for the key is overwritten.

	void setDouble(const std::string& key, double value);
		// Sets the property with the given key to the given value.
		// An already existing value for the key is overwritten.

	void setBool(const std::string& key, bool value);
		// Sets the property with the given key to the given value.
		// An already existing value for the key is overwritten.

	Signal2<const std::string&, const std::string&> PropertyChanged;
		// The Key and Value of the changed configuration property.

protected:
	virtual bool getRaw(const std::string& key, std::string& value) const = 0;
		// If the property with the given key exists, stores the property's value
		// in value and returns true. Otherwise, returns false.
		//
		// Must be overridden by subclasses.

	virtual void setRaw(const std::string& key, const std::string& value) = 0;
		// Sets the property with the given key to the given value.
		// An already existing value for the key is overwritten.
		//
		// The implementation is responsible for emitting the 
		// PropertyChanged signal.
	
	static int parseInt(const std::string& value);
	static Int64 parseLargeInt(const std::string& value);
	static bool parseBool(const std::string& value);

private:
	Configuration(const Configuration&);
	Configuration& operator = (const Configuration&);

	mutable Mutex _mutex;
};


//
// Scoped Configuration
//
	

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
	ScopedConfiguration(Configuration& config, const std::string& currentScope, const std::string& defaultScope);
	ScopedConfiguration(const ScopedConfiguration& that);

	std::string getString(const std::string& key, const std::string& defaultValue, bool forceDefaultScope = false) const;
	int getInt(const std::string& key, int defaultValue, bool forceDefaultScope = false) const;
	double getDouble(const std::string& key, double defaultValue, bool forceDefaultScope = false) const;
	bool getBool(const std::string& key, bool defaultValue, bool forceDefaultScope = false) const;

	void setString(const std::string& key, const std::string& value, bool defaultScope = false);
	void setInt(const std::string& key, int value, bool defaultScope = false);
	void setDouble(const std::string& key, double value, bool defaultScope = false);
	void setBool(const std::string& key, bool value, bool defaultScope = false);
	
	std::string getCurrentScope(const std::string& key) const;
	std::string getDafaultKey(const std::string& key) const;
	std::string getScopedKey(const std::string& key, bool defaultScope = false) const;

	Configuration& config;
	std::string currentScope;
	std::string defaultScope;

private:
	ScopedConfiguration& operator = (const ScopedConfiguration&);
};



} // namespace scy


#endif // SCY_Configuration_H


// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or subclass
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
