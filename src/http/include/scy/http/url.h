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


#ifndef SCY_HTTP_URL_H
#define SCY_HTTP_URL_H


#include "scy/base.h"
#include "scy/collection.h"
#include <http_parser.h>


namespace scy {
namespace http {


class URL
	/// An RFC 3986 based URL which uses an external c 
	/// library to do the heavy lifting.
	/// Constructors and assignment operators will throw
	/// a SyntaxException if the URL is invalid.
{
public:
    URL();
    URL(const char* url);
    URL(const std::string& url);
    URL(const std::string& scheme, const std::string& authority);
    URL(const std::string& scheme, const std::string& authority, const std::string& pathEtc);
    URL(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment = "");
    ~URL();
	
	URL& operator = (const URL& uri);
	URL& operator = (const std::string& uri);
	URL& operator = (const char* uri);
	
	// Parses and assigns an URI from the given std::string.
	// Throws a SyntaxException if whiny is set and the
	// given url is invalid.
	bool parse(const std::string& url, bool whiny = true);
	
	// RFC 3986 based URL encoding based on JavaScript's 
	// encodeURIComponent()
	static std::string encode(const std::string &str);
	
	// RFC 3986 based URL decoding based on JavaScript's 
	// decodeURIComponent()
	static std::string decode(const std::string& str);

public:
    std::string scheme() const;
    std::string userInfo() const;
    std::string host() const;
    UInt16 port() const;
    std::string authority() const;
    std::string path() const;
    std::string pathEtc() const;
    std::string query() const;
    std::string fragment() const;

    bool hasSchema() const;
    bool hasUserInfo() const;
    bool hasHost() const;
    bool hasPort() const;
    bool hasPath() const;
    bool hasQuery() const;
    bool hasFragment() const;
	
    bool valid() const;

    std::string str() const;
	
    friend std::ostream& operator << (std::ostream& stream, const URL& url) 
	{
		stream << url.str();
		return stream;
    }

protected:
    http_parser_url _parser;
    std::string _buf;
};


} // namespace http
} // namespace scy


#endif // SCY_HTTP_URL_H



		
	/*
	void updateSchema(const std::string& scheme);
	void updatePathEtc(const std::string& pathEtc);
	void updateSchema(const std::string& scheme);
	void updateSchema(const std::string& scheme);
	void updateHost(const std::string& host);
	void updatePort(UInt16 port);
	void updatePath(const std::string& path);	
	void updateQuery(const std::string& query);	
	void updateFragment(const std::string& fragment);	
	void updateUserInfo(const std::string& info);
	*/

//std::string parseHeader(const std::string& request, const std::string& name);
	/// Parses a Header from a HTTP request.
//bool parseRestfulQuery(const std::string& request, std::vector<std::string>& out);
