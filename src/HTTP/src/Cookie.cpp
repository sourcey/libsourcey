#include "Sourcey/HTTP/Cookie.h"


namespace scy {
namespace http {


Cookie::Cookie():
	_version(0),
	_secure(false),
	_maxAge(-1),
	_httpOnly(false)
{
}

	
Cookie::Cookie(const std::string& name):
	_version(0),
	_name(name),
	_secure(false),
	_maxAge(-1),
	_httpOnly(false)
{
}


Cookie::Cookie(const NVCollection& nvc):
	_version(0),
	_secure(false),
	_maxAge(-1),
	_httpOnly(false)
{
	for (NVCollection::ConstIterator it = nvc.begin(); it != nvc.end(); ++it)
	{
		const std::string& name  = it->first;
		const std::string& value = it->second;
		if (util::icompare(name, "comment") == 0)
		{
			setComment(value);
		}
		else if (util::icompare(name, "domain") == 0)
		{
			setDomain(value);
		}
		else if (util::icompare(name, "path") == 0)
		{
			setPath(value);
		}
		else if (util::icompare(name, "max-age") == 0)
		{
			assert(0);
			//setMaxAge(NumberParser::parse(value));
		}
		else if (util::icompare(name, "secure") == 0)
		{
			setSecure(true);
		}
		else if (util::icompare(name, "expires") == 0)
		{
			assert(0);
			/*
			int tzd;
			DateTime exp = DateTimeParser::parse(value, tzd);
			Timestamp now;
			setMaxAge((int) ((exp.timestamp() - now)/Timestamp::resolution()));
			*/
		}
		else if (util::icompare(name, "version") == 0)
		{
			assert(0);
			//setVersion(NumberParser::parse(value));
		}
		else if (util::icompare(name, "HttpOnly") == 0)
		{
			setHttpOnly(true);
		}
		else
		{
			setName(name);
			setValue(value);
		}
	}
}

	
Cookie::Cookie(const std::string& name, const std::string& value):
	_version(0),
	_name(name),
	_value(value),
	_secure(false),
	_maxAge(-1),
	_httpOnly(false)
{
}

	
Cookie::Cookie(const Cookie& cookie):
	_version(cookie._version),
	_name(cookie._name),
	_value(cookie._value),
	_comment(cookie._comment),
	_domain(cookie._domain),
	_path(cookie._path),
	_secure(cookie._secure),
	_maxAge(cookie._maxAge),
	_httpOnly(cookie._httpOnly)
{
}


Cookie::~Cookie()
{
}

	
Cookie& Cookie::operator = (const Cookie& cookie)
{
	if (&cookie != this)
	{
		_version  = cookie._version;
		_name     = cookie._name;
		_value    = cookie._value;
		_comment  = cookie._comment;
		_domain   = cookie._domain;
		_path     = cookie._path;
		_secure   = cookie._secure;
		_maxAge   = cookie._maxAge;
		_httpOnly = cookie._httpOnly;
	}
	return *this;
}

	
void Cookie::setVersion(int version)
{
	_version = version;
}

	
void Cookie::setName(const std::string& name)
{
	_name = name;
}

	
void Cookie::setValue(const std::string& value)
{
	_value = value;
}

	
void Cookie::setComment(const std::string& comment)
{
	_comment = comment;
}


void Cookie::setDomain(const std::string& domain)
{
	_domain = domain;
}


void Cookie::setPath(const std::string& path)
{
	_path = path;
}


void Cookie::setSecure(bool secure)
{
	_secure = secure;
}


void Cookie::setMaxAge(int maxAge) 
{
	_maxAge = maxAge;
}


void Cookie::setHttpOnly(bool flag)
{
	_httpOnly = flag;
}


std::string Cookie::toString() const
{
	std::string result;
	result.reserve(256);
	result.append(_name);
	result.append("=");
	if (_version == 0)
	{
		// Netscape cookie
		result.append(_value);
		if (!_domain.empty())
		{
			result.append("; domain=");
			result.append(_domain);
		}
		if (!_path.empty())
		{
			result.append("; path=");
			result.append(_path);
		}
		if (_maxAge >= 0)
		{
			assert(0);
			/*
			Timestamp ts;
			ts += _maxAge*Timestamp::resolution();
			result.append("; expires=");
			DateTimeFormatter::append(result, ts, DateTimeFormat::HTTP_FORMAT);
			*/
		}
		if (_secure)
		{
			result.append("; secure");
		}
		if (_httpOnly)
		{
			result.append("; HttpOnly");
		}
	}
	else
	{
		// RFC 2109 cookie
		result.append("\"");
		result.append(_value);
		result.append("\"");
		if (!_comment.empty())
		{
			result.append("; Comment=\"");
			result.append(_comment);
			result.append("\"");
		}
		if (!_domain.empty())
		{
			result.append("; Domain=\"");
			result.append(_domain);
			result.append("\"");
		}
		if (!_path.empty())
		{
			result.append("; Path=\"");
			result.append(_path);
			result.append("\"");
		}
		if (_maxAge >= 0)
		{
			result.append("; Max-Age=\"");
			assert(0); //NumberFormatter::append(result, _maxAge);
			result.append("\"");
		}
		if (_secure)
		{
			result.append("; secure");
		}
		if (_httpOnly)
		{
			result.append("; HttpOnly");
		}
		result.append("; Version=\"1\"");
	}
	return result;
}


namespace
{
	static const std::string ILLEGAL_CHARS("()[]/|\\',;");
}


std::string Cookie::escape(const std::string& str)
{
	std::string result;
	assert(0); //http::URL::encode(str, ILLEGAL_CHARS, result);
	return result;
}


std::string Cookie::unescape(const std::string& str)
{
	std::string result;
	assert(0); //http::URL::decode(str, result);
	return result;
}


} } // namespace scy::http


//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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
// all derivative works of the Software, unless such copies or derivative
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
