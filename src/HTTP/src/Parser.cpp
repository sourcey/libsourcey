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


#include "Sourcey/HTTP/Parser.h"
#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Crypto.h"
#include "Sourcey/HTTP/Util.h"


using namespace std;
using namespace Poco;


namespace scy { 
namespace http {


Parser::Parser(ParserObserver& observer, http_parser_type type) : ///, Poco::Net::HTTPMessage* headers
	_observer(observer), 
	//_headers(headers), 
	_error(NULL),
	_parsing(false)//, 
	//_upgrade(true), 
	//_wasHeaderValue(true)
{	
	traceL("Parser", this) << "Creating" << endl;

	::http_parser_init(&_parser, type);
	_parser.data = this;
	_settings.on_message_begin = on_message_begin_;
	_settings.on_url = on_url_;
	//_settings.on_status_complete = on_status_complete_;
	_settings.on_header_field = on_header_field_;
	_settings.on_header_value = on_header_value_;
	_settings.on_headers_complete = on_headers_complete_;
	_settings.on_body = on_body_;
	_settings.on_message_complete = on_message_complete_;
}


Parser::~Parser() 
{
	traceL("Parser", this) << "Destroy" << endl;	
	if (_error)
		delete _error;
}


bool Parser::parse(const char* data, size_t offset, size_t len) 
{
	traceL("Parser", this) << "Parse Data: " << string(data, len) << endl;	
	size_t nparsed = ::http_parser_execute(&_parser, &_settings, &data[offset], len);
	
    if (!_parser.upgrade && nparsed != len) {
		setParserError();
		/*
		enum http_errno err = HTTP_PARSER_ERRNO(&_parser);

		Local<Value> e = Exception::Error(String::NewSymbol("Parse Error"));
		Local<Object> obj = e->ToObject();
		obj->Set(String::NewSymbol("bytesParsed"), nparsed_obj);
		obj->Set(String::NewSymbol("code"), String::New(http_errno_name(err)));
		*/
    }


	/*
    // If there was a parse error in one of the callbacks
    // TODO What if there is an error on EOF?
    if (!parser->_parser.upgrade && nparsed != len) {
      enum http_errno err = HTTP_PARSER_ERRNO(&parser->_parser);

      Local<Value> e = Exception::Error(String::NewSymbol("Parse Error"));
      Local<Object> obj = e->ToObject();
      obj->Set(String::NewSymbol("bytesParsed"), nparsed_obj);
      obj->Set(String::NewSymbol("code"), String::New(http_errno_name(err)));
      return scope.Close(e);
    } else {
      return scope.Close(nparsed_obj);
    }
	*/


	/*
	// Handle parser error
	if (_parser.http_errno != HPE_OK) {
		setParserError(); //resval(_parser.http_errno)"HTTP parser error"
		return true;		
	} 

	// Finished, should have invoked onMessageComplete already
	else if (!parsing()) {
		assert(0 && "already complete");
		return true;		
	}

	// Need more data...
	else return false;
	*/
	return true;
}


void Parser::reset() 
{
	// TODO: How to reset parser state?

	_parsing = true;
	if (_error) {
		delete _error;
		_error = NULL;
	}
}


void Parser::setParserError()
{
	assert(_parser.http_errno != HPE_OK);	
	ParserError err;
	err.code = HTTP_PARSER_ERRNO(&_parser);
	err.message = http_errno_name(err.code);
	onError(err);
}


bool Parser::parsing() const 
{
	return _parsing;
}


bool Parser::upgrade() const 
{
	return _parser.upgrade > 0;
}


bool Parser::shouldKeepAlive() const 
{
	return http_should_keep_alive(&_parser);
}


//
// Events
//

// Add a header to the buffer, flushing if MAX_HEADERS_COUNT reached
void Parser::onHeader(const string& name, const string& value)
{
	// If no Connection created yet
	//if (!_observer) {
	//  prepare_incoming();
	//}	
	
	_observer.incomingHeaders()->add(name, value);
	_observer.onParserHeader(name, value);
}


void Parser::onBody(const char* buf, size_t off, size_t len)
{
	traceL("Parser", this) << "onBody" << endl;	
	_observer.onParserChunk(buf + off, len); //Buffer(buf+off,len)
}


void Parser::onMessageComplete() 
{
	traceL("Parser", this) << "onMessageComplete" << endl;	
	_observer.onParserDone();
}


void Parser::onError(const ParserError& err)
{
	traceL("Parser", this) << "On error: " << err.code << ": " << err.message << endl;	
	_parsing = false;
	_error = new ParserError;
	_error->code = err.code;
	_error->message = err.message;
	_observer.onParserError(err);
}



//
// http_parser callbacks
//

int Parser::on_message_begin_(http_parser* parser) 
{	
	// When http_parser has started receiving a new message reset the Parser state
	auto self = reinterpret_cast<Parser*>(parser->data);
	//traceL("Parser", self) << "on_message_begin" << endl;	
	assert(self);
	self->reset();
	return 0;
}


int Parser::on_url_(http_parser* parser, const char *at, size_t len) 
{
	// When http_parser has received an url, set the url on the http_start_line
	auto self = reinterpret_cast<Parser*>(parser->data);
	//traceL("Parser", self) << "on_url_" << endl;	
	assert(self);
	assert(at && len);
	
	// TODO
	http::Request* request = dynamic_cast<http::Request*>(self->_observer.incomingHeaders());
	request->setURI(string(at, len));

	/*
	if (!self->start_line_.url(at, len)) {
	self->error_ = true; //resval(error::http_parser_url_fail);
	return 1;
	}
	*/

	return 0;
}


int Parser::on_header_field_(http_parser* parser, const char* at, size_t len) 
{	
	// When http_parser receives a header field, store it in the headers buffer
	auto self = reinterpret_cast<Parser*>(parser->data);
	assert(self);

	if (self->_wasHeaderValue) {
		// new field started
		if (!self->_lastHeaderField.empty()) {

			// add new entry and callback
			self->onHeader(self->_lastHeaderField, self->_lastHeaderValue);
			self->_lastHeaderValue.clear();
		}
		self->_lastHeaderField = string(at, len);
		self->_wasHeaderValue = false;
	} 
	else {
		// appending
		self->_lastHeaderField += string(at, len);
	}

	return 0;
}

int Parser::on_header_value_(http_parser* parser, const char* at, size_t len) 
{
	// When http_parser receives a header value, 
	// store it in the headers buffer
	auto self = reinterpret_cast<Parser*>(parser->data);
	assert(self);

	if (!self->_wasHeaderValue) {
		self->_lastHeaderValue = string(at, len);
		self->_wasHeaderValue = true;
	} 
	else {
		// appending
		self->_lastHeaderValue += string(at, len);
	}

	return 0;
}

int Parser::on_headers_complete_(http_parser* parser)
{
	// When http_parser finishes receiving headers, 
	// add last header to buffer and
	auto self = reinterpret_cast<Parser*>(parser->data);
	//traceL("Parser", self) << "on_headers_complete_" << endl;	
	assert(self);
	assert(&self->_parser == parser);

	// add last entry if any
	if (!self->_lastHeaderField.empty()) {
		// add new entry
		self->onHeader(self->_lastHeaderField, self->_lastHeaderValue);
	}

	Poco::Net::HTTPMessage* headers = self->_observer.incomingHeaders();
			
	// HTTP version
	//start_line_.version(parser_.http_major, parser_.http_minor);

	// KeepAlive
	headers->setKeepAlive(http_should_keep_alive(parser));

	//
	/// Response
	http::Response* response = dynamic_cast<http::Response*>(headers);
	if (response) {
		
		// HTTP status
		response->setStatus((Poco::Net::HTTPResponse::HTTPStatus)parser->status_code);
	}
	
	//
	/// Request
	http::Request* request = dynamic_cast<http::Request*>(headers);
	if (request) {
		
		// HTTP method
		request->setMethod(http_method_str(static_cast<http_method>(parser->method))); //)
	}




	/*
	if (!self->_observer) {
	// If we're here without an incoming message then we did not receive any headers
	self->prepare_incoming(); // Create an incoming message
	self->validate_incoming(); // Check if required headers missing
	}
	*/

	self->_observer.onParserHeadersDone();
	return 0;
}

// When http_parser receives a body chunk, signal that body received
int Parser::on_body_(http_parser* parser, const char* at, size_t len) 
{		
	auto self = reinterpret_cast<Parser*>(parser->data);
	//traceL("Parser", self) << "on_body_" << endl;	
	assert(self);

	// Call handle_body
	self->onBody(at, 0, len);
	return 0;
}


// When http_parser finished receiving a message, signal message complete
int Parser::on_message_complete_(http_parser* parser) 
{	
	auto self = reinterpret_cast<Parser*>(parser->data);
	assert(self);

	// Call handle_message_complete
	self->onMessageComplete();
	self->_parsing = false;

	return 0;
}






} } // namespace scy::http




/*
void Parser::setError(UInt32 code, const std::string& message)
{
	Error err;
	err.code = code;
	err.message = message;
	onError(err);
}
*/
	/*
	if (_observer) _observer.onParserError(e);
	else onError_(e);
	*/

/**
 * Create a parser of the specified type attached to the given socket
 * which invokes the given callback to create an Connection instance.
 *
 * @param socket
 * @param type
 * @param callback
 * @return
Parser* Parser::create(
    http_parser_type type,
    net::Socket* socket,
    on__observertype _observercb)
{

  Parser* parser(new Parser(type, socket));

  if (_observercb) parser->register_on_incoming(_observercb);
  return parser;
}
 */

/**
 * Set the callback for allocating an incoming message
 * @param callback [description]
void Parser::register_on_incoming(on__observertype callback) {
  on__observer = callback;
}

void Parser::register_onError(onError_type callback) {
  onError_ = callback;
}

void Parser::register_on_close(on_close_type callback) {
  on_close_ = callback;
}

void Parser::register_on_end(on_end_type callback) {
  on_end_ = callback;
}
 */

/* Accessors *****************************************************************

const http_start_line& Parser::start_line() const {
  return start_line_;
}
*/



/* Initialization *************************************************************/

/**
 * Private constructor
 * @param type   http_parser_type value (HTTP_{REQUEST,RESPONSE,BOTH})
 * @param socket net::Socket to be read by parser
 
	//socket_(socket) //, net::Socket* socket
	//, on__observer()
	//, 
	//, parser_()
	//, settings_()
	//, _lastHeaderField()
	//, _lastHeaderValue()

 */






/**
 * Register socket event handlers
 */
	/*
void Parser::registerSocketEvents() {
	//heerree
  // Register net::Socket event handlers
  socket_->on<native::event::data>([=](const Buffer& buf) {
      if(parse(buf.base(), 0, buf.size()))
      {
        // parse end
        socket_->pause();
      } // otherwise more reads required: keep reading!
  });

  socket_->on<native::event::end>([=](){
    // TODO: maybe reuse the Parser::onError(...) method?
    // EOF
    // We can ignore if successfully completed parsing a message.
    // Don't relay it here since the parser emits end on message completion
    // and the socket may be kept open after parsing and no end emitted.
    if (_observer && !parsing()) return;

    if (_observer) {
      // Created response but message not complete
      _observer.onParserError(Exception("socket end before message complete"));
      // socket will next emit close
    } else { // Haven't created response so must emit on request
      // Started parsing headers but not yet complete
      onError_(Exception("socket end before headers complete"));
    }
  });

  socket_->on<native::event::error>([=](const scy::Exception& exc){
    if (_observer) {
      _observer.onParserError(e);
    } else {
      onError_(e);
    }
  });

  socket_->on<native::event::close>([=](){
    // EOF
    if (_observer && !parsing()) return;
    if (_observer) {
      // TODO: add members to Connection so events are emitted there
      _observer.emit<native::event::close>();
    } else {
      on_close_();
    }
  });

//  socket_->on<event::connect>([=]() {
//    DBG("socket connect");
//  });
//  socket_->on<event::timeout>([=](){
//    DBG("socket timeout");
//  });
//  socket_->on<event::drain>([=](){
//    DBG("socket drain");
//  });
}
  */

/* Headers ********************************************************************/

	/*
void Parser::prepare_incoming() {

	// TODO: Write to request

  assert(!_observer); // Shouldn't be called if already have incoming message

  // Prepare http_start_line and parser state
  // Common 

  // HTTP version
  start_line_.version(_parser.http_major, _parser.http_minor);

  // Response

  // HTTP status
  start_line_.status(_parser.status_code);

  // Request

  // TODO: validate host header for incoming HTTP 1.1 requests
  //  string host("");
  //  int port = 80; // default port
  //  // HTTP 1.1 requires "Host" header
  //  if (start_line_.version() == detail::HTTP_1_1) {
  //    // get host and port info from header entry "Host"
  //    if (has_header("host")) {
  //      auto s = get_header("host");
  //      auto colon = s.find_last_of(':');
  //      if (colon == s.npos) {
  //        host = s;
  //      } else {
  //        host = s.substr(0, colon);
  //        port = stoi(s.substr(colon + 1));
  //      }
  //    }
  //  }

  // HTTP method
  start_line_.method(static_cast<http_method>(_parser.method));

  // Headers

  // HTTP keep-alive
  _shouldKeepAlive = !!http_shouldKeepAlive(&_parser);

  // HTTP upgrade
  _upgrade = _parser.upgrade > 0;

  // We must have an Connection factory function registered
  // TODO: replace asserts with errors if Connection factory not registered or fails to create Connection
  assert(on__observer);
  // Create a new Connection
  _observer = on__observer(socket_, this);
  assert(_observer);
}

void Parser::validate_incoming() {
  // TODO: check for missing required headers (like Host) here
}
  */





/*
#include "Sourcey/HTTP/Authenticator.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Crypto.h"
#include "Sourcey/HTTP/Util.h"

#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/String.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;


namespace scy { 
namespace http {


// ---------------------------------------------------------------------
// Digest Authenticator
//
//
//	Client requests uri:
//	-------------------------------------
//
//	GET / HTTP/1.1
//
//
//	Server requires authentication:
//	-------------------------------------
//
//	HTTP/1.1 401 Unauthorized
//	WWW-Authenticate: Digest realm = "The batcave",
//		qop = "auth",
//		nonce = "4993927ba6279",
//		opaque = "d8ea7aa61ha1693024c4cc3a516f49b3c"
//
//
//	Client authenticates:
//	-------------------------------------
//
//	GET / HTTP/1.1
//	Authorization: Digest username = Anionu_API_USERNAME, 
//		realm = "The batcave", 
//		nonce=49938e61ccaa4, 
//		uri = "/", 
//		response = "98ccab4542f284c00a79b5957baaff23", 
//		opaque = "d8ea7aa61ha1693024c4cc3a516f49b3c", 
//		qop=auth, nc=00000001, 
//		cnonce = "8d1b34edb475994b" 
//
DigestAuthenticator::DigestAuthenticator(const string& realm, const string& version, bool usingRFC2617) : 
	Authenticator(),
	_realm(realm),
	_version(version),
	_usingRFC2617(usingRFC2617),
	_opaque(crypt::hash("md5", realm))
{
	traceL() << "[DigestAuthenticator] Creating" << endl;
}


DigestAuthenticator::~DigestAuthenticator() 
{
	traceL() << "[DigestAuthenticator] Destroying" << endl;
}


string DigestAuthenticator::parseHeaderSegment(const string& key) 
{
	string value = "";
	string::size_type start, end = 0;
	start = _lastRequest.find(" "+key+" = ",0);
	if (start != string::npos) {
		start += key.length()+2;
		end = _lastRequest.find(",", start);
		if (end == string::npos) end = _lastRequest.find("\r", start);
		if (end == string::npos) end = _lastRequest.find("\n", start);
		if (end == string::npos) 
			return "";
		value = _lastRequest.substr(start, end-start);
		replaceInPlace(value,"\"", "");
	}
	//traceL() << format("Parse: Key: %s, Value: %s, start: %d, end: %d", key, value, start, end));
	return value;
}


bool DigestAuthenticator::validateRequest(UserManager* authenticator, const string& request) 
{
	traceL() << "[DigestAuthenticator] Validating Request: " + request << endl;

	_lastRequest = request;
	string hash;
	string httpMethod = request.substr(0, request.find_first_of(' '));
	string username = parseHeaderSegment("username");
	string uri = parseHeaderSegment("uri");
	string nonce = parseHeaderSegment("nonce");
	string nc = parseHeaderSegment("nc");
	string cnonce = parseHeaderSegment("cnonce");
	string qop = parseHeaderSegment("qop");
	string response = parseHeaderSegment("response");

	if (!httpMethod.size() || !username.size() || !uri.size() || !response.size())
		return false;

	// Get the user details for the supplied username from our session object
	const IUser* user = authenticator->get(username);
	if (!user) 
		return false;

	string ha1 = crypt::hash("md5", format("%s:%s:%s", username, _realm, user->password()));
	string ha2 = crypt::hash("md5", format("%s:%s", httpMethod, uri));

	if (_usingRFC2617 && qop.size()) {
		// Using advanced digest authentication
		// Hash = md5(HA1:nonce:nc:cnonce:qop:HA2) 
		hash = crypt::hash("md5", format("%s:%s:%s:%s:%s:%s", ha1, nonce, nc, cnonce, qop, ha2));
	} else {
		// Using standard digest authentication
		// Hash = md5(HA1:nonce:HA2) 
		hash = crypt::hash("md5", format("%s:%s:%s", ha1, nonce, ha2));
	}

	return hash == response;
}


string DigestAuthenticator::prepare401Header(const string& extra) 
{
	_noonce = crypt::randomKey(32);
	if (_usingRFC2617) {
		return format(
			"%s 401 Unauthorized\r\n"
			"%s"
			"WWW-Authenticate: Digest realm=\"%s\", qop=\"auth\", nonce=\"%s\", opaque=\"%s\"\r\n\r\n",
			_version, extra, _realm, _noonce, _opaque
		);
	} else {
		return format(
			"%s 401 Unauthorized\r\n"
			"%s"
			"WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\"\r\n\r\n",
			_version, extra, _realm, _noonce
		);
	}
}


} } // namespace scy::http

*/


	/*
	traceL() << "Digest Authenticator:"
		<< "\n\tHTTP Method: " << httpMethod
		<< "\n\tUsername: " << username
		<< "\n\tURI: " << uri
		<< "\n\tNoonce: " << nonce
		<< "\n\tNc: " << nc
		<< "\n\tCNoonce: " << cnonce
		<< "\n\tQOP: " << qop 
		<< "\n\tResponse: " << response 
		<< endl;
		*/