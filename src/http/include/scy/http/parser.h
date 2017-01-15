///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup http
/// @{


#include "scy/http/request.h"
#include "scy/http/response.h"
#include "scy/net/socket.h"
#include <http_parser.h>


#ifndef SCY_HTTP_Parser_H
#define SCY_HTTP_Parser_H


namespace scy {
namespace http {


struct ParserError
{
    http_errno code;
    std::string message;
};


class SCY_EXTERN ParserObserver
{
public:
    virtual void onParserHeader(const std::string& name,
                                const std::string& value) = 0;
    virtual void onParserHeadersEnd() = 0;
    virtual void onParserChunk(const char* data, std::size_t len) = 0;
    virtual void onParserEnd() = 0;

    virtual void onParserError(const ParserError& err) = 0;
};


class SCY_EXTERN Parser
{
public:
    Parser(http::Response* response);
    Parser(http::Request* request);
    Parser(http_parser_type type);
    ~Parser();

    void init(http_parser_type type);

    std::size_t parse(const char* data,
                      std::size_t length); //, bool expectComplete = false

    /// Feed data read from socket into the http_parser.
    ///
    /// Returns true of the message is complete, false if incomplete.
    /// Reset the parser state for a new message
    void reset();

    /// Returns true if parsing is complete, either
    /// in success or error.
    bool complete() const;

    void setParserError(
        const std::string& message = ""); // bool throwException = true,

    void setRequest(http::Request* request);
    void setResponse(http::Response* response);
    void setObserver(ParserObserver* observer);

    http::Message* message();
    ParserObserver* observer() const;

    bool upgrade() const;
    bool shouldKeepAlive() const;

    ///// Callbacks
    void onURL(const std::string& value);
    void onHeader(const std::string& name, const std::string& value);
    void onHeadersEnd();
    void onBody(const char* buf, std::size_t len);
    void onMessageEnd();
    void onError(const ParserError& err);

public:
    ///// http_parser callbacks
    static int on_message_begin(http_parser* parser);
    static int on_url(http_parser* parser, const char* at, std::size_t len);
    static int on_status(http_parser* parser, const char* at, std::size_t len);
    static int on_header_field(http_parser* parser, const char* at,
                               std::size_t len);
    static int on_header_value(http_parser* parser, const char* at,
                               std::size_t len);
    static int on_headers_complete(http_parser* parser);
    static int on_body(http_parser* parser, const char* at, std::size_t len);
    static int on_message_complete(http_parser* parser);

public:
    ParserObserver* _observer;
    http::Request* _request;
    http::Response* _response;
    http::Message* _message;

    http_parser _parser;
    http_parser_settings _settings;

    bool _wasHeaderValue;
    std::string _lastHeaderField;
    std::string _lastHeaderValue;

    bool _complete;

    ParserError* _error;
};


} // namespace http
} // namespace scy


#endif // SCY_HTTP_Parser_H


/// @\}
