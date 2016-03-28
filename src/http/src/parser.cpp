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


#include "scy/http/parser.h"
#include "scy/http/connection.h"
#include "scy/logger.h"
#include "scy/crypto/crypto.h"
#include "scy/http/util.h"
#include <stdexcept>


using std::endl;


namespace scy {
namespace http {


Parser::Parser(http::Response* response) :
    _observer(nullptr),
    _request(nullptr),
    _response(response),
    _error(nullptr)
{
    init(HTTP_RESPONSE);
}


Parser::Parser(http::Request* request) :
    _observer(nullptr),
    _request(request),
    _response(nullptr),
    _error(nullptr)
{
    init(HTTP_REQUEST);
}


Parser::Parser(http_parser_type type) :
    _observer(nullptr),
    _request(nullptr),
    _response(nullptr),
    _error(nullptr)
{
    init(type);
}


Parser::~Parser()
{
    TraceS(this) << "Destroy" << endl;
    reset();
}


void Parser::init(http_parser_type type)
{
    TraceS(this) << "Init: " << type << endl;

    assert(_parser.data != this);

    ::http_parser_init(&_parser, type);
    _parser.data = this;
    _settings.on_message_begin = on_message_begin;
    _settings.on_url = on_url;
    _settings.on_status = on_status;
    _settings.on_header_field = on_header_field;
    _settings.on_header_value = on_header_value;
    _settings.on_headers_complete = on_headers_complete;
    _settings.on_body = on_body;
    _settings.on_message_complete = on_message_complete;

    reset();
}


bool Parser::parse(const char* data, std::size_t len)
{
    // TraceS(this) << "Parse: " << len << endl;
    TraceS(this) << "Parse: " << std::string(data, len) << endl;

    assert(!complete());
    assert(_parser.data == this);

    if (complete()) {
        setParserError("Parsing already complete");
    }

    // Parse and handle errors
    std::size_t nparsed = ::http_parser_execute(&_parser, &_settings, data, len);
    if (nparsed != len && !_parser.upgrade) { //_parser.http_errno != HPE_OK
        setParserError();
    }

    return complete();
}


void Parser::reset()
{
    _complete = false;
    if (_error) {
        delete _error;
        _error = nullptr;
    }

    // TODO: Reset parser internal state?
}


void Parser::setParserError(const std::string& message) //bool throwException,
{
    assert(_parser.http_errno != HPE_OK);
    ParserError err;
    err.code = HTTP_PARSER_ERRNO(&_parser);
    err.message = message.empty() ? http_errno_name(err.code) : message;
    onError(err);

    //if (throwException)
    //    throw std::runtime_error(err.message);
}


void Parser::setRequest(http::Request* request)
{
    assert(!_request);
    assert(!_response);
    assert(_parser.type == HTTP_REQUEST);
    _request = request;
}


void Parser::setResponse(http::Response* response)
{
    assert(!_request);
    assert(!_response);
    assert(_parser.type == HTTP_RESPONSE);
    _response = response;
}


void Parser::setObserver(ParserObserver* observer)
{
    _observer = observer;
}


http::Message* Parser::message()
{
    return _request ? reinterpret_cast<http::Message*>(_request)
        : _response ? reinterpret_cast<http::Message*>(_response)
        : nullptr;
}


ParserObserver* Parser::observer() const
{
    return _observer;
}


bool Parser::complete() const
{
    return _complete;
}


bool Parser::upgrade() const
{
    return _parser.upgrade > 0;
}


bool Parser::shouldKeepAlive() const
{
    return http_should_keep_alive(&_parser) > 0;
}


//
// Events
//

void Parser::onURL(const std::string& value)
{
    TraceS(this) << "onURL: " << value << endl;

    if (_request)
        _request->setURI(value);
}


void Parser::onHeader(const std::string& name, const std::string& value)
{
    // TraceS(this) << "onHeader: " << name << ":" << value << endl;

    if (message())
        message()->add(name, value);
    if (_observer)
        _observer->onParserHeader(name, value);
}


void Parser::onHeadersEnd()
{
    /// HTTP version
    //start_line_.version(parser_.http_major, parser_.http_minor);

    /// KeepAlive
    //headers->setKeepAlive(http_should_keep_alive(parser) > 0);

    /// Request HTTP method
    if (_request)
        _request->setMethod(http_method_str(static_cast<http_method>(_parser.method)));

    if (_observer)
        _observer->onParserHeadersEnd();
}


void Parser::onBody(const char* buf, std::size_t len) //std::size_t off,
{
    TraceS(this) << "onBody" << endl;
    if (_observer)
        _observer->onParserChunk(buf, len); //Buffer(buf+off,len) + off
}


void Parser::onMessageEnd()
{
    TraceS(this) << "onMessageEnd" << endl;
    _complete = true;
    if (_observer)
        _observer->onParserEnd();
}


void Parser::onError(const ParserError& err)
{
    TraceS(this) << "On error: " << err.code << ": " << err.message << endl;
    _complete = true;
    _error = new ParserError;
    _error->code = err.code;
    _error->message = err.message;
    if (_observer)
        _observer->onParserError(err);
}



//
// http_parser callbacks
//

int Parser::on_message_begin(http_parser* parser)
{
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);

    self->reset();
    return 0;
}


int Parser::on_url(http_parser* parser, const char *at, std::size_t len)
{
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);
    assert(at && len);

    self->onURL(std::string(at, len));
    return 0;
}


int Parser::on_status(http_parser* parser, const char *at, size_t length)
{
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);

    /// Handle response status line
    if (self->_response)
        self->_response->setStatus((http::StatusCode)parser->status_code);

    return 0;
}


int Parser::on_header_field(http_parser* parser, const char* at, std::size_t len)
{
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);

    if (self->_wasHeaderValue) {
        if (!self->_lastHeaderField.empty()) {
            self->onHeader(self->_lastHeaderField, self->_lastHeaderValue);
            self->_lastHeaderValue.clear();
        }
        self->_lastHeaderField = std::string(at, len);
        self->_wasHeaderValue = false;
    }
    else {
        self->_lastHeaderField += std::string(at, len);
    }

    return 0;
}


int Parser::on_header_value(http_parser* parser, const char* at, std::size_t len)
{
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);

    if (!self->_wasHeaderValue) {
        self->_lastHeaderValue = std::string(at, len);
        self->_wasHeaderValue = true;
    }
    else {
        self->_lastHeaderValue += std::string(at, len);
    }

    return 0;
}


int Parser::on_headers_complete(http_parser* parser)
{
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);
    assert(&self->_parser == parser);

    // Add last entry if any
    if (!self->_lastHeaderField.empty()) {
        self->onHeader(self->_lastHeaderField, self->_lastHeaderValue);
    }

    self->onHeadersEnd();
    return 0;
}


int Parser::on_body(http_parser* parser, const char* at, std::size_t len)
{
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);

    self->onBody(at, len);
    return 0;
}


int Parser::on_message_complete(http_parser* parser)
{
    // When http_parser finished receiving a message, signal message complete
    auto self = reinterpret_cast<Parser*>(parser->data);
    assert(self);

    self->onMessageEnd();
    return 0;
}



} } // namespace scy::http
