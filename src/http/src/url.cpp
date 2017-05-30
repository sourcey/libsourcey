///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#include "scy/http/url.h"
#include "scy/util.h"


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


URL::URL(const std::string& scheme, const std::string& authority)
{
    parse(scheme + "://" + authority);
}


URL::URL(const std::string& scheme, const std::string& authority,
         const std::string& pathEtc)
{
    parse(scheme + "://" + authority + pathEtc);
}


URL::URL(const std::string& scheme, const std::string& authority,
         const std::string& path, const std::string& query,
         const std::string& fragment)
{
    parse(scheme + "://" + authority + path + "?" + query + "#" + fragment);
}


URL::~URL()
{
}


URL& URL::operator=(const URL& uri)
{
    if (&uri != this)
        parse(uri.str());
    return *this;
}


URL& URL::operator=(const std::string& uri)
{
    parse(uri);
    return *this;
}


URL& URL::operator=(const char* uri)
{
    parse(uri);
    return *this;
}


bool URL::parse(const std::string& url, bool whiny)
{
    LTrace("Parsing: ", url)
    std::string src(util::trim(url));
    _buf = src;
    if (http_parser_parse_url(src.c_str(), src.length(), 0, &_parser) == 0)
        return true;
    _buf.clear();
    if (whiny)
        throw std::runtime_error("Syntax error: Cannot parse invalid URL: " +
                                 src);
    return false;
}


std::string URL::scheme() const
{
    std::string res;
    if (hasSchema()) {
        res.assign(_buf.substr(_parser.field_data[UF_SCHEMA].off,
                               _parser.field_data[UF_SCHEMA].len));
        util::toLowerInPlace(res); // always return as lowercase
    }
    return res;
}


std::string URL::host() const
{
    if (hasHost())
        return _buf.substr(_parser.field_data[UF_HOST].off,
                           _parser.field_data[UF_HOST].len);
    return std::string();
}


uint16_t URL::port() const
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
        res.append(util::itostr<uint16_t>(port()));
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
        return _buf.substr(_parser.field_data[UF_PATH].off,
                           _parser.field_data[UF_PATH].len);
    return std::string();
}


std::string URL::query() const
{
    if (hasQuery())
        return _buf.substr(_parser.field_data[UF_QUERY].off,
                           _parser.field_data[UF_QUERY].len);
    return std::string();
}


std::string URL::fragment() const
{
    if (hasFragment())
        return _buf.substr(_parser.field_data[UF_FRAGMENT].off,
                           _parser.field_data[UF_FRAGMENT].len);
    return std::string();
}


std::string URL::userInfo() const
{
    if (hasUserInfo())
        return _buf.substr(_parser.field_data[UF_USERINFO].off,
                           _parser.field_data[UF_USERINFO].len);
    return std::string();
}


#if 0
void URL::updateSchema(const std::string& scheme)
{
    if (!hasSchema())
        throw std::runtime_error("Cannot update invalid URL");

    std::string tmp(str());
    util::replaceInPlace(tmp, this->scheme(), scheme);
    parse(tmp);
}


void URL::updateHost(const std::string& host)
{
    if (!hasHost())
        throw std::runtime_error("Cannot update invalid URL");

    std::string tmp(str());
    util::replaceInPlace(tmp, this->host(), host);
    parse(tmp);
}


void URL::updatePort(uint16_t port)
{
    if (!hasPort())
        throw std::runtime_error("Cannot update invalid URL");

    std::string tmp(str());
    util::replaceInPlace(tmp,
        util::itostr<uint16_t>(this->port()),
        util::itostr<uint16_t>(port));
    parse(tmp);
}


void URL::updatePath(const std::string& path)
{
    if (!hasPath())
        throw std::runtime_error("Cannot update invalid URL");

    std::string tmp(str());
    util::replaceInPlace(tmp, this->path(), path);
    parse(tmp);
}


void URL::updateQuery(const std::string& query)
{
    if (!hasQuery())
        throw std::runtime_error("Cannot update invalid URL");

    std::string tmp(str());
    util::replaceInPlace(tmp, this->query(), query);
    parse(tmp);
}


void URL::updateFragment(const std::string& fragment)
{
    if (!hasFragment())
        throw std::runtime_error("Cannot update invalid URL");

    std::string tmp(str());
    util::replaceInPlace(tmp, this->fragment(), fragment);
    parse(tmp);
}


void URL::updateUserInfo(const std::string& info)
{
    if (!hasUserInfo())
        throw std::runtime_error("Cannot update invalid URL");

    std::string tmp(str());
    util::replaceInPlace(tmp, this->userInfo(), info);
    parse(tmp);
}
#endif


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
    return (_parser.field_set & (1 << UF_SCHEMA)) == (1 << UF_SCHEMA);
}


bool URL::hasHost() const
{
    return (_parser.field_set & (1 << UF_HOST)) == (1 << UF_HOST);
}


bool URL::hasPort() const
{
    return (_parser.field_set & (1 << UF_PORT)) == (1 << UF_PORT);
}


bool URL::hasPath() const
{
    return (_parser.field_set & (1 << UF_PATH)) == (1 << UF_PATH);
}


bool URL::hasQuery() const
{
    return (_parser.field_set & (1 << UF_QUERY)) == (1 << UF_QUERY);
}


bool URL::hasFragment() const
{
    return (_parser.field_set & (1 << UF_FRAGMENT)) == (1 << UF_FRAGMENT);
}


bool URL::hasUserInfo() const
{
    return (_parser.field_set & (1 << UF_USERINFO)) == (1 << UF_USERINFO);
}


std::string URL::encode(const std::string& str)
{
    const std::string unreserved =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";

    std::string escaped = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (unreserved.find_first_of(str[i]) != std::string::npos) {
            escaped.push_back(str[i]);
        } else {
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
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%') {
            const std::string digits = "0123456789ABCDEF";
            clean +=
                (char)(digits.find(str[i + 1]) * 16 + digits.find(str[i + 2]));
            i += 2;
        } else {
            clean += str[i];
        }
    }
    return clean;
}


} // namespace http
} // namespace scy


/// @\}
