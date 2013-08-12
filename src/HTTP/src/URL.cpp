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


#include "Sourcey/HTTP/URL.h"
#include "Sourcey/Util.h"



namespace scy {
namespace http {
	

URL::URL()
{
	parse("");
}


URL::URL(const char* url)
{
	parse(url);
}


URL::URL(const std::string& url)
{
	parse(url);
}


/*
URL::URL(const std::string& scheme, const std::string& pathEtc)
{
	parse(scheme + "://" + pathEtc);
}
*/


URL::URL(const std::string& scheme, const std::string& host, const std::string& pathEtc)
{
	parse(scheme + "://" + host + pathEtc);
}


URL::URL(const std::string& scheme, const std::string& host, const std::string& path, const std::string& query, const std::string& fragment)
{
	parse(scheme + "://" + host + path + "?" + query + "#" + fragment);
}


URL::~URL() 
{
}


URL& URL::operator = (const URL& uri)
{
	if (&uri != this)
		parse(uri.str());
	return *this;
}

	
URL& URL::operator = (const std::string& uri)
{
	parse(uri);
	return *this;
}


URL& URL::operator = (const char* uri)
{
	parse(std::string(uri));
	return *this;
}


bool URL::parse(const std::string& url, bool whiny)
{
	debugL("URL") << "Parsing: " << url << std::endl;
    _buf = url;
    if (http_parser_parse_url(url.c_str(), url.length(), 0, &_parser) == 0)
		return true;
	_buf.clear();
	if (whiny)
		throw SyntaxException("Cannot parse invalid URL: " + url);
	return false;
}


std::string URL::scheme() const
{
	std::string res;
    if (hasSchema()) {
		res.assign(_buf.substr(_parser.field_data[UF_SCHEMA].off, _parser.field_data[UF_SCHEMA].len));
		util::toLower(res); // always returned as lowercase
	}
    return res;
}


std::string URL::host() const
{
    if (hasHost())
		return _buf.substr(_parser.field_data[UF_HOST].off, _parser.field_data[UF_HOST].len);
    return std::string();
}


UInt16 URL::port() const
{
    if (hasPort())
		return _parser.port;
	std::string sc = scheme();
	if (sc == "http")
		return 80;
	else if (sc == "https")
		return 443;
    return 0;
}


std::string URL::authority() const
{
	std::string res;	
    if (hasUserInfo()) {
		res.append(userInfo());
		res.append("@");
	}
	res.append(host());
    if (hasPort()) {
		res.append(":");
		res.append(util::itostr<UInt16>(port()));
	}
	return res;
}


std::string URL::pathEtc() const
{
	std::string res;	
	res.append(path());	
    if (hasQuery()) {
		res.append("?");
		res.append(query());
	}
    if (hasFragment()) {
		res.append("#");
		res.append(fragment());
	}
	return res;
}


std::string URL::path() const
{
    if (hasPath())
		return _buf.substr(_parser.field_data[UF_PATH].off, _parser.field_data[UF_PATH].len);
    return std::string();
}


std::string URL::query() const
{
    if (hasQuery()) 
		return _buf.substr(_parser.field_data[UF_QUERY].off, _parser.field_data[UF_QUERY].len);
    return std::string();
}


std::string URL::fragment() const
{
    if (hasFragment()) 
		return _buf.substr(_parser.field_data[UF_FRAGMENT].off, _parser.field_data[UF_FRAGMENT].len);
    return std::string();
}


std::string URL::userInfo() const
{
    if (hasUserInfo()) 
		return _buf.substr(_parser.field_data[UF_USERINFO].off, _parser.field_data[UF_USERINFO].len);
    return std::string();
}

	
/*
void URL::updateSchema(const std::string& scheme)
{
	if (!hasSchema())
		throw Exception("Cannot update invalid URL");		
	
	std::string tmp(str());
	util::replaceInPlace(tmp, this->scheme(), scheme);
	parse(tmp);
}


void URL::updateHost(const std::string& host)
{
	if (!hasHost())
		throw Exception("Cannot update invalid URL");		

	std::string tmp(str());
	util::replaceInPlace(tmp, this->host(), host);
	parse(tmp);
}


void URL::updatePort(UInt16 port)
{	
	if (!hasPort())
		throw Exception("Cannot update invalid URL");		

	std::string tmp(str());
	util::replaceInPlace(tmp, 
		util::itostr<UInt16>(this->port()), 
		util::itostr<UInt16>(port));
	parse(tmp);
}


void URL::updatePath(const std::string& path)
{
	if (!hasPath())
		throw Exception("Cannot update invalid URL");		

	std::string tmp(str());
	util::replaceInPlace(tmp, this->path(), path);
	parse(tmp);
}


void URL::updateQuery(const std::string& query)
{
	if (!hasQuery())
		throw Exception("Cannot update invalid URL");		

	std::string tmp(str());
	util::replaceInPlace(tmp, this->query(), query);
	parse(tmp);
}


void URL::updateFragment(const std::string& fragment)
{
	if (!hasFragment())
		throw Exception("Cannot update invalid URL");		

	std::string tmp(str());
	util::replaceInPlace(tmp, this->fragment(), fragment);
	parse(tmp);
}


void URL::updateUserInfo(const std::string& info)
{
	if (!hasUserInfo())
		throw Exception("Cannot update invalid URL");		

	std::string tmp(str());
	util::replaceInPlace(tmp, this->userInfo(), info);
	parse(tmp);
}
*/
	

bool URL::valid() const
{
	return !_buf.empty();
}


std::string URL::str() const
{
    return _buf;
}


bool URL::hasSchema() const 
{ 
	return (_parser.field_set & (1<<UF_SCHEMA)) == (1<<UF_SCHEMA); 
}


bool URL::hasHost() const 
{ 
	return (_parser.field_set & (1<<UF_HOST)) == (1<<UF_HOST);
}


bool URL::hasPort() const
{ 
	return (_parser.field_set & (1<<UF_PORT)) == (1<<UF_PORT);
}


bool URL::hasPath() const 
{ 
	return (_parser.field_set & (1<<UF_PATH)) == (1<<UF_PATH);
}


bool URL::hasQuery() const 
{ 
	return (_parser.field_set & (1<<UF_QUERY)) == (1<<UF_QUERY);
}


bool URL::hasFragment() const 
{ 
	return (_parser.field_set & (1<<UF_FRAGMENT)) == (1<<UF_FRAGMENT);
}


bool URL::hasUserInfo() const 
{ 
	return (_parser.field_set & (1<<UF_USERINFO)) == (1<<UF_USERINFO);
}


std::string URL::encode(const std::string &str)
{
    const std::string unreserved = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";

    std::string escaped = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (unreserved.find_first_of(str[i]) != std::string::npos) {
            escaped.push_back(str[i]);
        }
        else {
            escaped.append("%");
            char buf[3];
            sprintf(buf, "%.2X", str[i]);
            escaped.append(buf);
        }
    }
    return escaped;
}


std::string URL::decode(const std::string& str)
{
	std::string clean = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '%') {
			const std::string digits = "0123456789ABCDEF";
			clean += (char)(digits.find(str[i+1])*16 + digits.find(str[i+2]));
			i += 2;
		} 
		else {
			clean += str[i];
		}
	}
	return clean;
}


} // namespace http
} // namespace scy