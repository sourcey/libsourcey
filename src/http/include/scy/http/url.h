///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#ifndef SCY_HTTP_URL_H
#define SCY_HTTP_URL_H


#include "scy/http/http.h"
#include "scy/collection.h"
#include <http_parser.h>


namespace scy {
namespace http {


/// An RFC 3986 based URL which uses an external c
/// library to do the heavy lifting.
/// Constructors and assignment operators will throw
/// a SyntaxException if the URL is invalid.
class HTTP_API URL
{
public:
    URL();
    URL(const char* url);
    URL(const std::string& url);
    URL(const std::string& scheme, const std::string& authority);
    URL(const std::string& scheme, const std::string& authority,
        const std::string& pathEtc);
    URL(const std::string& scheme, const std::string& authority,
        const std::string& path, const std::string& query,
        const std::string& fragment = "");
    ~URL();

    URL& operator=(const URL& uri);
    URL& operator=(const std::string& uri);
    URL& operator=(const char* uri);

    /// Parses and assigns an URI from the given std::string.
    /// Throws a SyntaxException if whiny is set and the
    /// given url is invalid.
    bool parse(const std::string& url, bool whiny = true);

    /// RFC 3986 based URL encoding based on JavaScript's
    /// encodeURIComponent()
    static std::string encode(const std::string& str);

    /// RFC 3986 based URL decoding based on JavaScript's
    /// decodeURIComponent()
    static std::string decode(const std::string& str);

public:
    std::string scheme() const;
    std::string userInfo() const;
    std::string host() const;
    uint16_t port() const;
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

    friend std::ostream& operator<<(std::ostream& stream, const URL& url)
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


/// @\}
