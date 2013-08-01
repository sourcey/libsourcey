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


#include "Sourcey/HTTP/Util.h"
#include "Sourcey/Util.h"
#include "Poco/String.h"


using namespace std;
//using namespace Poco;


namespace scy {
namespace util {
	

string parseURI(const string& request) 
{
	std::string req = request;
	std::string value = "";
	std::string::size_type start, end = 0;
	util::toUpper(req);
	start = req.find(" ");
	if (start != string::npos) {
		start++;
		end = req.find(" HTTP", start);
		if (end == string::npos) end = req.find(" RTSP", start);
		if (end == string::npos) return "";
		value = request.substr(start, end-start);
	}
	return value;
}


bool matchURL(const std::string& uri, const std::string& expression) 
{
	string::size_type index = uri.find("?");
	return util::matchNodes(uri.substr(0, index), expression, "/");
}


/*
string parseHeader(const string& request, const string& name) 
{
	string req = request;
	toLower(req);
	toLower(name);
	string value = "";
	string::size_type start, end = 0;
	start = req.find(name+": ");
	if (start != string::npos) {
		start += name.length() + 2;
		end = req.find("\r\n", start);
		if (end == string::npos) return "";
		value = request.substr(start, end-start);
		replaceInPlace(value,"\"","");
		replaceInPlace(value,"\r","");
		replaceInPlace(value,"\n","");
	}
	return value;
}
*/


std::string parseCookieItem(const std::string& cookie, const std::string& item)
{
	string::size_type start, end = 0;
	start = cookie.find(item + "=");
	if (start != string::npos) {
		start += item.size() + 1;
		end = cookie.find(";", start);
		return cookie.substr(start, end-start);
	}
	return "";
}


bool splitURIParameters(const string& uri, NVHash& out)
{
	size_t len = uri.length();
	size_t i = 0;

	// Parse REST parameters
	//i++;
	while (i < len && uri[i] != '?') {	
		i++; 

		string value = "";	
		while (uri[i] != '/' && uri[i] != '?' && i < len)
			value += uri[i++];

		// REST parameters are referenced by index
		if (!value.empty())
			out.set(util::toString(out.size()), value);		
	}
	
	// Parse query parameters
	if (uri[i] == '?') i++;
	while (i < len)
	{
		string name = "";
		while (uri[i] != '=' && i < len)
			name += uri[i++];
		i++;
		string value = "";
		while (uri[i] != '&' && i < len)
			value += uri[i++];
		i++;
		
		if (!name.empty() && !value.empty())
			out.set(name, value);
	}

	return out.size() > 0;
}


void splitParameters(const std::string& s, std::string& value, NVHash& parameters)
{
	value.clear();
	parameters.clear();
	std::string::const_iterator it  = s.begin();
	std::string::const_iterator end = s.end();
	while (it != end && ::isspace(*it)) ++it;
	while (it != end && *it != ';') value += *it++;
	Poco::trimRightInPlace(value);
	if (it != end) ++it;
	splitParameters(it, end, parameters);
}


void splitParameters(const std::string::const_iterator& begin, const std::string::const_iterator& end, NVHash& parameters)
{
	std::string pname;
	std::string pvalue;
	pname.reserve(32);
	pvalue.reserve(64);
	std::string::const_iterator it = begin;
	while (it != end)
	{
		pname.clear();
		pvalue.clear();
		while (it != end && ::isspace(*it)) ++it;
		while (it != end && *it != '=' && *it != ';') pname += *it++;
		Poco::trimRightInPlace(pname);
		if (it != end && *it != ';') ++it;
		while (it != end && ::isspace(*it)) ++it;
		while (it != end && *it != ';')
		{
			if (*it == '"')
			{
				++it;
				while (it != end && *it != '"')
				{
					if (*it == '\\')
					{
						++it;
						if (it != end) pvalue += *it++;
					}
					else pvalue += *it++;
				}
				if (it != end) ++it;
			}
			else if (*it == '\\')
			{
				++it;
				if (it != end) pvalue += *it++;
			}
			else pvalue += *it++;
		}
		Poco::trimRightInPlace(pvalue);
		if (!pname.empty()) parameters.add(pname, pvalue);
		if (it != end) ++it;
	}
}


} // namespace util
} // namespace scy