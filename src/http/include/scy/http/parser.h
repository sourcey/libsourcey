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

//#include "scy/http/connection.h"
#include "scy/net/socket.h"
#include "scy/http/request.h"
#include "scy/http/response.h"
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


class ParserObserver
{
public:
    virtual void onParserHeader(const std::string& name, const std::string& value) = 0;
    virtual void onParserHeadersEnd() = 0;
    virtual void onParserChunk(const char* data, std::size_t len) = 0;
    virtual void onParserEnd() = 0;

    virtual void onParserError(const ParserError& err) = 0;
};


class Parser
{
public:
    Parser(http::Response* response); 
    Parser(http::Request* request); 
    Parser(http_parser_type type);
    ~Parser();

    void init(http_parser_type type);
	
    bool parse(const char* data, std::size_t length); //, bool expectComplete = false
		// Feed data read from socket into the http_parser.
		//
		// Returns true of the message is complete, false if incomplete.

    void reset();
		// Reset the parser state for a new message
	
    bool complete() const;
		// Returns true if parsing is complete, either  
		// in success or error.

	void setParserError(const std::string& message = ""); //bool throwException = true, 

    void setRequest(http::Request* request);
    void setResponse(http::Response* response);
    void setObserver(ParserObserver* observer);
	
    http::Message* message();
    ParserObserver* observer() const;

    bool upgrade() const;
    bool shouldKeepAlive() const;
	
	//
	/// Callbacks
    void onURL(const std::string& value);
    void onHeader(const std::string& name, const std::string& value);
    void onHeadersEnd();
    void onBody(const char* buf, std::size_t len);
    void onMessageEnd();
    void onError(const ParserError& err);

public:

	//
    /// http_parser callbacks
    static int on_message_begin(http_parser* parser);
    static int on_url(http_parser* parser, const char *at, std::size_t len);
    static int on_status_complete(http_parser* parser);
    static int on_header_field(http_parser* parser, const char* at, std::size_t len);
    static int on_header_value(http_parser* parser, const char* at, std::size_t len);
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


} } // namespace scy::http


#endif // SCY_HTTP_Parser_H



	
		//, or throws an exception on error.
		//
		// The expectComplete flag can be set for parsing headers only. 
		// If the message is not complete after calling the parser
		// an exception will be thrown.
	//bool _failed;
    //bool _parsing;
    //bool _upgrade;
    //bool _shouldKeepAlive;

	//
	/// State
	//
    //bool failed() const;
    /// Accessors	

	/*	/// std::size_t offset, 
    //bool parsing() const;
    http::Message* headers()
	{
		return _headers;
	};
	*/


/*
class Connection;


typedef http_method method;
typedef http_parser_url_fields url_fields;
typedef http_errno error;

inline const char* get_error_name(error err)
{
	return http_errno_name(err);
}

inline const char* get_error_description(error err)
{
	return http_errno_description(err);
}

inline const char* get_method_name(method m)
{
	return http_method_str(m);
}
*/



/*
#define HTTP_CB(name)                                                         \
  static int name(http_parser* p_) {                                          \
    Parser* self = container_of(p_, Parser, parser_);                         \
    return self->name##_();                                                   \
  }                                                                           \
  int name##_()


#define HTTP_DATA_CB(name)                                                    \
  static int name(http_parser* p_, const char* at, std::size_t length) {           \
    Parser* self = container_of(p_, Parser, parser_);                         \
    return self->name##_(at, length);                                         \
  }                                                                           \
  int name##_(const char* at, std::size_t length)
  */
/**
    //detail::resval error_;
	
	//const std::string& message = ""
	//void setError(UInt32 code, const std::string& message);
enum http_version {
  HTTP_1_0, HTTP_1_1, HTTP_UNKNOWN_VERSION
};

typedef std::map<std::string, std::string> headers_type; //, util::text::ci_less

std::string http_status_text(int status_code);

 * The http_start_line encapsulates the fields in an HTTP request or status line.
class http_start_line {
private:
  // Common parameters
  http_version version_;

  // Request Line
  http_method method_;
  URL url_;

  // Response Line
  unsigned short status_;

public:
  http_start_line();
  http_start_line(const http_start_line& c);
  http_start_line(http_start_line&& c);
  ~http_start_line();

  const http_version& version() const;
  int version_major() const;
  int version_minor() const;
  std::string version_string() const;
  void version(const http_version& v);
  bool version(unsigned short major, unsigned short minor);

  const URL& url() const;
  bool url(const std::string& u, bool isConnect=false);
  bool url(const char* at, std::size_t len, bool isConnect=false);
  void url(const URL& u);

  const http_method& method() const;
  void method(const http_method& m);

  unsigned short status() const;
  void status(const unsigned short& s);

  void reset();
};
 */


//, net::Socket::Ptr socket

    //void registerSocketEvents();

    //http_start_line start_line_;

    //const http_start_line& start_line() const;

	//bool _error;


//private:

    //net::Socket::Ptr socket_; // Passed to constructor
    //on__observertype on__observer;
    //onError_type onError_;
    //on_close_type on_close_;
    //on_end_type on_end_;

   // void prepare_incoming(); // Create an Connection to collect headers and other info
   // void validate_incoming(); // Check for missing required headers or other invalid info

	/*
    static Parser* create(
        http_parser_type type,
        net::Socket::Ptr socket,
        on__observertype _observercb = nullptr);

    * Callback that must be registered to allow construction of classes
    * derived from Connection. This is necessary because we want to
    * emit events on the derived type.

    typedef std::function<Connection*(net::Socket*,
        Parser*)> on__observertype;

    typedef std::function<void(const Exception&)> onError_type;
    typedef std::function<void()> on_close_type;
    typedef std::function<void()> on_end_type;

    void register_on_incoming(on__observertype callback);
    void register_onError(onError_type callback);
    void register_on_close(on_close_type callback);
    void register_on_end(on_end_type callback);

	NullSignal Recv; 
	NullSignal Close; 
	Signal<const Exception&> Error; 
	*/







/*


#include "scy/util/usercollection.h"
#include <string>


// ---------------------------------------------------------------------
// Abstract Authenticator
//
class Authenticator
{
public:
	virtual ~Authenticator() {};

public:
	virtual bool validateRequest(UserManager* authenticator, const std::string& request) = 0;
	virtual std::string prepare401Header(const std::string& extra = "") = 0;
};


// ---------------------------------------------------------------------
// Digest Authenticator
//
class DigestAuthenticator: public Authenticator
{
public:
	DigestAuthenticator(const std::string& realm = "Spot", const std::string& version = "HTTP/1.1", bool usingRFC2617 = false);
	virtual ~DigestAuthenticator();

public:
	std::string prepare401Header(const std::string& extra = "");
	std::string parseHeaderSegment(const std::string& key);
	bool validateRequest(UserManager* authenticator, const std::string& request);
	std::string version() { return _version; };

protected:
	std::string _lastRequest;
	std::string _protocol;
	std::string _version;
	std::string _realm;
	std::string _noonce;
	std::string _opaque;
	bool _usingRFC2617;
};
*/
