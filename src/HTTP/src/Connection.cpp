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


#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/Server.h"
#include "Sourcey/HTTP/Client.h"
#include "Sourcey/Logger.h"

#include <assert.h>


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace scy { 
namespace http {


Connection::Connection(const net::Socket& socket, http_parser_type type) : 
	_parser(*this, type),
	_request(new Request),
	_response(new Response),
	_socket(socket), 
	_closed(false),
	_sentResponseHeaders(false),
	_timeout(30 * 60 * 1000) // 30 secs
{	
	traceL("Connection", this) << "Creating: " << this << endl;
	_socket.base().addObserver(*this, true);
}

	
Connection::~Connection() 
{	
	traceL("Connection", this) << "Destroying: " << this << endl;
	// Must be close()'d
	assert(_closed);
		
	delete _request;
	delete _response;

	traceL("Connection", this) << "Destroying: OK" << endl;
}


bool Connection::sendHeaders(bool whiny)
{
	assert(!_sentResponseHeaders);
	assert(outgoingHeaders());
	
	ostringstream os;
	outgoingHeaders()->write(os);
	string headers(os.str().data(), os.str().length());

	traceL("Connection", this) << "Send Headers >>>>: " << headers << endl; // remove me

	_timeout.start();
	_sentResponseHeaders = true;

	return _socket.send(headers.data(), headers.length()) > 0;
}


bool Connection::sendBytes(const char* buf, size_t len, bool whiny)
{
	// TODO: Assert request read complete
	// TODO: Binary data / file response
	// TODO: Send mutex in the future - not needed 
	// now because we are running in single thread mode.

	// Send headers
	if (!_sentResponseHeaders) 
		sendHeaders(whiny);

	//traceL("Connection", this) << "SEND >>>>\n" << string(buf, len) << endl; // remove me

	// Send body / chunk
	return _socket.send(buf, len) > 0;
}


void Connection::close()
{
	traceL("Connection", this) << "Close" << endl;	
	assert(!_closed);
	assert(_socket.base().refCount() == 2);
	
	_closed = true;
	_socket.base().removeObserver(*this);
	_socket.close();

	onClose();
		
	deleteLater<Connection>(this); // destroy it
}


void Connection::onClose()
{
	Close.emit(this);
}


Request& Connection::request()
{
	return *_request;
}

	
Response& Connection::response()
{
	return *_response;
}

	
net::Socket& Connection::socket()
{
	return _socket;
}
	

Buffer& Connection::buffer()
{
	return static_cast<net::TCPBase&>(_socket.base()).buffer();
}

	
bool Connection::closed()
{
	return _closed;
}

	
bool Connection::expired()
{
	return _timeout.expired();
}


//
// Socket callbacks
//

void Connection::onSocketRecv(Buffer& buf, const net::Address& peerAddr)
{
	traceL("Connection", this) << "On socket recv" << endl;	
	_timeout.stop();
	_parser.parse(buf.data(), 0, buf.size());
}


void Connection::onSocketClose()
{
	traceL("Connection", this) << "On socket close" << endl;	

	// Close the connection when the socket closes
	close();
}


//
// Parser callbacks
//

void Connection::onParserHeader(const string& name, const string& value) 
{
	//traceL("Connection", this) << "On header: " << name << ": " << value << endl;	
	// TODO: handle onStatus and do request status line handling there
	//incomingHeaders()->add(name, value);
}


void Connection::onParserError(const ParserError& err)
{
	traceL("Connection", this) << "Parser error: " << err.message << endl;	
	close();
}

	




	
	/*
	bool res = _socket.send(buf, len) > 0;

	// Close unless keepalive
	if (!_response->getKeepAlive()) {
		traceL("Client", this) << "Closing: No keepalive" << endl; 
		_socket.close();
	}
	*/	






	// TODO: Write to request body for now, but we should
	// enable partial reads in future for large files etc.
	//_request->body.write(body.data(), body.size());
	/*
	
	//assert(0);
	DefaultServerResponser* handler = new DefaultServerResponser();
	handler->handleRequest(this, *request, *response);
	*/

	/*
//
// ServerConnection Handler
//

void DefaultServerResponser::handleRequest(ServerConnection* connection, Request& request, Response& response)
{
	traceL("DefaultServerResponser", this) << "Handle Request" << endl;
	response.body << "hello";
	connection->respond();
}




	body_ = true;
	if (paused_) {
		// If paused socket, store pending buffer
		pendings_.push_back(body);
	} else {
		// Otherwise emit data event
		//emit<native::event::data>(body);
	}
	*/
/*



	// TODO: Might close while still parsing
	//delete this;
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"
*/




 //Buffer& buf, const net::Address& peerAddr


	
	//server.addServerConnection(this);
	//assert(socket);
	//SocketEmitter::socket = &this->socket;
	//this->_socket.base().addObserver(*this, false);
	//this->_socket.base().removeObserver(*this);
	//server.removeServerConnection(this);




  //emit<native::event::error>(e);
	/*
	if (haveListener<native::event::end>()) {
	emit<native::event::end>();
	}
	*/

	/*
void ServerConnection::onParserHeader(const string& name, const string& value) 
  bool append = false; // TODO: check headers to append by default
  if (!body_) {
    headers_[name] =
        (has_header(name) && append) ? headers_[name] + "," + value : value;
  } else {
    trailers_[name] =
        (has_trailer(name) && append) ? trailers_[name] + "," + value : value;
  }
  */
	


	/*
	// ReadableStream events
	registerEvent<native::event::data>();
	registerEvent<native::event::end>();
	registerEvent<native::event::error>();
	registerEvent<native::event::close>();

	// ServerConnection events
	registerEvent<native::event::http::headers>();
	registerEvent<native::event::http::trailers>();
	*/

/**
 * True by default, but turns false after an 'error' occurred, the stream came
 * to an 'end', or destroy() was called.
 * @return
bool ServerConnection::readable() {
  return readable_;
}

void ServerConnection::pause() {
  socket_->pause();
  paused_ = true;
}
void ServerConnection::resume() {
  assert(socket_);
  socket_->resume();

  _emitPending(nullptr);
}

void ServerConnection::destroy(const ParserError& err) {
  socket_->destroy(e);
}
 */



/**
 * Emit data events that were queued after pausing socket
 * @param callback
void ServerConnection::_emitPending(function<void()> callback) {
  if (pendings_.size() > 0) {
    for (vector<Buffer>::iterator it = pendings_.begin();
        it != pendings_.end(); ++it) {
      // TODO: do this on next tick
      emit<native::event::data>(*it);
    }
  }
}
void ServerConnection::_emitData(const Buffer& buf) {
  emit<native::event::data>(buf);
}
void ServerConnection::_emitEnd() {
  if (!endEmitted_) emit<native::event::end>();
  endEmitted_ = true;
}
void ServerConnection::_addHeaderLine(const string& field,
    const string& value) {
}
 */
/*
 * Accessors

 

// Read-only HTTP message
int ServerConnection::statusCode() {
  return parser_->start_line().status();
}
const http_method& ServerConnection::method() {
  return parser_->start_line().method();
}
const http_version& ServerConnection::httpVersion() {
  return parser_->start_line().version();
}
string ServerConnection::httpVersionString() {
  return parser_->start_line().version_string();
}
int ServerConnection::httpVersionMajor() {
  return parser_->start_line().version_major();
}
int ServerConnection::httpVersionMinor() {
  return parser_->start_line().version_minor();
}
const URL& ServerConnection::url() {
  return parser_->start_line().url();
}

bool ServerConnection::shouldKeepAlive() {
  return parser_->shouldKeepAlive();
}
bool ServerConnection::upgrade() {
  return parser_->upgrade();
}


Parser* ServerConnection::parser() {
  return parser_;
}

net::Socket* ServerConnection::socket() {
  return socket_;
}
 */

/*



const headers_type& ServerConnection::headers() const {
  return headers_;
}

bool ServerConnection::has_header(const string& name) {
  return headers_.count(name) > 0;
}

const string& ServerConnection::get_header(const string& name) {
  headers_type::iterator it = headers_.find(name);
  if (it == headers_.end()) {
    // TODO: throw proper error
//    throw Exception("header not found");
  }
  return it->second;
}

const headers_type& ServerConnection::trailers() const {
  return headers_;
}

bool ServerConnection::has_trailer(const string& name) {
  return trailers_.count(name) > 0;
}

const string& ServerConnection::get_trailer(const string& name) {
  headers_type::iterator it = trailers_.find(name);
  if (it == trailers_.end()) {
    // TODO: throw proper error
//    throw Exception("header not found");
  }
  return it->second;
}

*/






	/*
ServerConnection::ServerConnection(Request* request) : 
	_request(request),
	_session(NULL),
	_response(HTTPResponse::HTTP_SERVICE_UNAVAILABLE),
	_clientData(NULL)
{
	log("trace") << "Creating" << endl;
}


ServerConnection::~ServerConnection()
{
	log("trace") << "Destroying" << endl;
	// Beed to cast HTTP client session to free it since base is protected
	if (_session) {
		if (_uri.getScheme() == "http")
			delete static_cast<HTTPClientSession*>(_session);

		else if (_uri.getScheme() == "https")
			delete static_cast<HTTPSClientSession*>(_session);
	}
	if (_request)
		delete _request;
}


bool ServerConnection::send() 
{
	assert(_request);
	try 
	{
		setState(this, net::ServerConnectionState::Running);
	
		_request->prepare();
		_uri = URI(_request->getURI());
		_request->setURI(_uri.getPathAndQuery()); // ensure URI only in request

		log("trace") << "Sending:" 
			<< "\n\tMethod: " << _request->getMethod()
			<< "\n\tURL: " << _uri.toString()
			<< "\n\tIsAuthenticated: " << _request->hasCredentials()
			//<< "\n\tURI: " << _request->getURI()
			//<< "\n\tHost: " << _uri.getHost()
			//<< "\n\tPort: " << _uri.getPort()
			//<< "\n\tOutput Path: " << _outputPath
			<< endl;
		
		assert(!_uri.getScheme().empty());
		if (_uri.getScheme() == "http") {
			HTTPClientSession session(_uri.getHost(), _uri.getPort());
			session.setTimeout(Timespan(10, 0));
			ostream& ost = session.sendRequest(*_request);
			processRequest(ost);
			istream& ist = session.receiveResponse(_response);
			processResponse(ist);
		} 
		else if (_uri.getScheme() == "https") {
			HTTPSClientSession session(_uri.getHost(), _uri.getPort());
			session.setTimeout(Timespan(10, 0));
			ostream& ost = session.sendRequest(*_request);
			processRequest(ost);
			istream& ist = session.receiveResponse(_response);
			processResponse(ist);
		} 
		else
			throw Exception("Unknown HTTP scheme: " + _uri.getScheme());

		if (!cancelled())
			setState(this, _response.success() ? 
				net::ServerConnectionState::Success : net::ServerConnectionState::Failed, 
				_response.getReason());
	}
	catch (StopPropagation&) {
		log("trace") << "Cancelled" << endl;

		// In the transaction was cancelled we return
		// false here. onComplete() will not be called.
		return false;
	}
	catch (Exception& exc) {
		log("error") << "Failed: " << exc.displayText() << endl;
		_response.error = exc.displayText();
		setState(this, net::ServerConnectionState::Failed, _response.error);
	}

	onComplete();
	return _response.success();
}


void ServerConnection::cancel()
{
	log("trace") << "Cancelling" << endl;
	assert(!cancelled());
	setState(this, net::ServerConnectionState::Cancelled);
}


void ServerConnection::processRequest(ostream& ostr)
{
	try 
	{
		TransferProgress& st = _requestState;
		st.total = _request->getContentLength();
		setRequestState(TransferProgress::Running);
		if (_request->form) {	
			char c;
			streambuf* pbuf = _request->body.rdbuf(); 
			while (pbuf->sgetc() != EOF) {				
				c = pbuf->sbumpc();
				ostr << c;
				st.current++;
				if (st.current % 32768 == 0) {					
					if (cancelled()) {
						setRequestState(TransferProgress::Cancelled);
						throw StopPropagation();
					}					
					//log("trace") << "Upload progress: " << 
					//	st.current << " of " << st.total << endl;
					setRequestState(TransferProgress::Running);
				}
			}		
		}
		setRequestState(TransferProgress::Complete);
	}
	catch (Exception& exc) 
	{
		log("error") << "Request Error: " << exc.displayText() << endl;
		setRequestState(TransferProgress::Failed);
		exc.rethrow();
	}	
}


void ServerConnection::setRequestState(TransferProgress::Type state)
{
	_requestState.state = state;
	UploadProgress.emit(this, _requestState);
}


void ServerConnection::processResponse(istream& istr)
{	
	try 
	{	
		char c;
		ofstream* fstr = _outputPath.empty() ? NULL : 
			new ofstream(_outputPath.data(), ios_base::out | ios_base::binary);
		ostream& ostr = fstr ? 
			static_cast<ostream&>(*fstr) : 
			static_cast<ostream&>(_response.body);
		TransferProgress& st = _responseState;
		st.total = _response.getContentLength();
		setResponseState(TransferProgress::Running);
		istr.get(c);
		while (istr && ostr) {
			st.current++;
			ostr.put(c);
			istr.get(c);
			if (st.current % 32768 == 0) {					
				if (cancelled()) {
					setResponseState(TransferProgress::Cancelled);
					throw StopPropagation();
				}				
				//log("trace") << "Download progress: " 
				//	<< st.current << " of " << st.total << endl;	
				setResponseState(TransferProgress::Running);
			}
		}
				
		if (fstr)
			fstr->close();
		
		setResponseState(TransferProgress::Complete);
	}
	catch (Exception& exc) 
	{
		log("error") << "Response Error: " << exc.displayText() << endl;
		setResponseState(TransferProgress::Failed);
		exc.rethrow();
	}
}

void ServerConnection::setResponseState(TransferProgress::Type state)
{
	_responseState.state = state;
	DownloadProgress.emit(this, _responseState);
}


void ServerConnection::onComplete()
{	
	assert(!cancelled());
	Complete.emit(this, _response);
}


bool ServerConnection::cancelled()
{
	return stateEquals(net::ServerConnectionState::Cancelled);
}


Request& ServerConnection::request() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return *_request;
}


Response& ServerConnection::response() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _response; 
}


TransferProgress& ServerConnection::requestState() 
{
	FastMutex::ScopedLock lock(_mutex);
	return _requestState; 
}


TransferProgress& ServerConnection::responseState() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _requestState; 
}	


void ServerConnection::setRequest(Request* request)
{ 
	FastMutex::ScopedLock lock(_mutex);
	_request = request;
}


void ServerConnection::setOutputPath(const string& path)
{ 
	FastMutex::ScopedLock lock(_mutex);
	_outputPath = path;
}


string& ServerConnection::outputPath() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _outputPath; 
}


void ServerConnection::setClientData(void* clientData)
{ 
	FastMutex::ScopedLock lock(_mutex);
	_clientData = clientData;
}


void* ServerConnection::clientData() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _clientData;
}
*/


} } // namespace scy::http