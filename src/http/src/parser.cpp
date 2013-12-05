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
	TraceLS(this) << "Destroy" << endl;	
	reset();
}


void Parser::init(http_parser_type type)
{
	TraceLS(this) << "Init: " << type << endl;	

	assert(_parser.data != this);

	::http_parser_init(&_parser, type);
	_parser.data = this;
	_settings.on_message_begin = on_message_begin;
	_settings.on_url = on_url;
	_settings.on_status_complete = on_status_complete;
	_settings.on_header_field = on_header_field;
	_settings.on_header_value = on_header_value;
	_settings.on_headers_complete = on_headers_complete;
	_settings.on_body = on_body;
	_settings.on_message_complete = on_message_complete;

	reset();
}


bool Parser::parse(const char* data, std::size_t len)
{
	TraceLS(this) << "Parse: " << len << endl;	
	
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
	//	throw std::runtime_error(err.message);
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
	return _request ? static_cast<http::Message*>(_request) 
		: _response ? static_cast<http::Message*>(_response) 
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
	if (_request)
		_request->setURI(value);
}


void Parser::onHeader(const std::string& name, const std::string& value)
{
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


void Parser::onBody(const char* buf, std::size_t len) //size_t off, 
{
	TraceLS(this) << "onBody" << endl;	
	if (_observer)
		_observer->onParserChunk(buf, len); //Buffer(buf+off,len) + off
}


void Parser::onMessageEnd() 
{
	TraceLS(this) << "onMessageEnd" << endl;		
	_complete = true;
	if (_observer)
		_observer->onParserEnd();
}


void Parser::onError(const ParserError& err)
{
	TraceLS(this) << "On error: " << err.code << ": " << err.message << endl;	
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


int Parser::on_status_complete(http_parser* parser)
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
	/// When http_parser finished receiving a message, signal message complete
	auto self = reinterpret_cast<Parser*>(parser->data);
	assert(self);

	self->onMessageEnd();
	return 0;
}



} } // namespace scy::http




/*
	_observer = nullptr;
Parser::Parser(http_parser_type type) : ///, http::Message* headers // ParserObserver& observer, 
	_observer(nullptr), 
	//_headers(headers), 
	_error(nullptr),
	_complete(false)//, 
	//_upgrade(true), 
	//_wasHeaderValue(true)
{	
	TraceLS(this) << "Create" << endl;
}
*/
	/*
	setParserError();
	enum http_errno err = HTTP_PARSER_ERRNO(&_parser);

	Local<Value> e = Exception::Error(String::NewSymbol("Parse Error"));
	Local<Object> obj = e->ToObject();
	obj->Set(String::NewSymbol("bytesParsed"), nparsed_obj);
	obj->Set(String::NewSymbol("code"), String::New(http_errno_name(err)));
	*/
	
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
	/// Handle parser error
	if (_parser.http_errno != HPE_OK) {
		setParserError(); //resval(_parser.http_errno)"HTTP parser error"
		return true;		
	} 

	/// Finished, should have invoked onMessageEnd already
	else if (!parsing()) {
		assert(0 && "already complete");
		return true;		
	}

	/// Need more data...
	else return false;
	*/


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
	if (_observer) _observer->onParserError(e);
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
      if(parse(buf.base(), 0, buf.available()))
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
      _observer->onParserError(std::runtime_error("socket end before message complete"));
      // socket will next emit close
    } else { // Haven't created response so must emit on request
      // Started parsing headers but not yet complete
      onError_(std::runtime_error("socket end before headers complete"));
    }
  });

  socket_->on<native::event::error>([=](const scy::Exception& exc){
    if (_observer) {
      _observer->onParserError(e);
    } else {
      onError_(e);
    }
  });

  socket_->on<native::event::close>([=](){
    // EOF
    if (_observer && !parsing()) return;
    if (_observer) {
      // TODO: add members to Connection so events are emitted there
      _observer->emit<native::event::close>();
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

	/// TODO: Write to request

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
  //  std::string host("");
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
#include "scy/http/authenticator.h"
#include "scy/logger.h"
#include "scy/crypto/crypto.h"
#include "scy/http/util.h"

#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/String.h"
#include "Poco/Format.h"


using std::endl;



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
//	HTTP/1.1 401 NotAuthorized
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
DigestAuthenticator::DigestAuthenticator(const std::string& realm, const std::string& version, bool usingRFC2617) : 
	Authenticator(),
	_realm(realm),
	_version(version),
	_usingRFC2617(usingRFC2617),
	_opaque(crypto::hash("md5", realm))
{
	TraceL << "[DigestAuthenticator] Creating" << endl;
}


DigestAuthenticator::~DigestAuthenticator() 
{
	TraceL << "[DigestAuthenticator] Destroying" << endl;
}


string DigestAuthenticator::parseHeaderSegment(const std::string& key) 
{
	std::string value = "";
	string::size_type start, end = 0;
	start = _lastRequest.find(" "+key+" = ",0);
	if (start != std::string::npos) {
		start += key.length()+2;
		end = _lastRequest.find(",", start);
		if (end == std::string::npos) end = _lastRequest.find("\r", start);
		if (end == std::string::npos) end = _lastRequest.find("\n", start);
		if (end == std::string::npos) 
			return "";
		value = _lastRequest.substr(start, end-start);
		replaceInPlace(value,"\"", "");
	}
	//TraceL << format("Parse: Key: %s, Value: %s, start: %d, end: %d", key, value, start, end));
	return value;
}


bool DigestAuthenticator::validateRequest(UserManager* authenticator, const std::string& request) 
{
	TraceL << "[DigestAuthenticator] Validating Request: " + request << endl;

	_lastRequest = request;
	std::string hash;
	std::string httpMethod = request.substr(0, request.find_first_of(' '));
	std::string username = parseHeaderSegment("username");
	std::string uri = parseHeaderSegment("uri");
	std::string nonce = parseHeaderSegment("nonce");
	std::string nc = parseHeaderSegment("nc");
	std::string cnonce = parseHeaderSegment("cnonce");
	std::string qop = parseHeaderSegment("qop");
	std::string response = parseHeaderSegment("response");

	if (!httpMethod.size() || !username.size() || !uri.size() || !response.size())
		return false;

	/// Get the user details for the supplied username from our session object
	const IUser* user = authenticator->get(username);
	if (!user) 
		return false;

	std::string ha1 = crypto::hash("md5", format("%s:%s:%s", username, _realm, user->password()));
	std::string ha2 = crypto::hash("md5", format("%s:%s", httpMethod, uri));

	if (_usingRFC2617 && qop.size()) {
		/// Using advanced digest authentication
		/// Hash = md5(HA1:nonce:nc:cnonce:qop:HA2) 
		hash = crypto::hash("md5", format("%s:%s:%s:%s:%s:%s", ha1, nonce, nc, cnonce, qop, ha2));
	} else {
		/// Using standard digest authentication
		/// Hash = md5(HA1:nonce:HA2) 
		hash = crypto::hash("md5", format("%s:%s:%s", ha1, nonce, ha2));
	}

	return hash == response;
}


string DigestAuthenticator::prepare401Header(const std::string& extra) 
{
	_noonce = util::randomString(32);
	if (_usingRFC2617) {
		return format(
			"%s 401 NotAuthorized\r\n"
			"%s"
			"WWW-Authenticate: Digest realm=\"%s\", qop=\"auth\", nonce=\"%s\", opaque=\"%s\"\r\n\r\n",
			_version, extra, _realm, _noonce, _opaque
		);
	} else {
		return format(
			"%s 401 NotAuthorized\r\n"
			"%s"
			"WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\"\r\n\r\n",
			_version, extra, _realm, _noonce
		);
	}
}


} } // namespace scy::http

*/


	/*
	TraceL << "Digest Authenticator:"
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