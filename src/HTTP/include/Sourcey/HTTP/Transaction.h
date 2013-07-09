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


#ifndef SOURCEY_HTTP_Transaction_H
#define SOURCEY_HTTP_Transaction_H


#include "Sourcey/Stateful.h"
#include "Sourcey/ISendable.h"
#include "Sourcey/Net/Transaction.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Response.h"
#include "Sourcey/HTTP/Parser.h"

#include "Poco/Net/HTTPSession.h"
#include "Poco/URI.h"
#include "Poco/Thread.h"

	
namespace scy { 
namespace http {


struct TransferProgress 
{
	enum Type
	{
		None,
		Running,
		Complete,
		Cancelled,	/// cancelled as result of user intervention
		Failed,
	};
	
	Type state;
	std::streamsize current;
	std::streamsize total;

	TransferProgress() :	
		current(0), total(0), state(None) {}

	double progress() {
		return (current / (total * 1.0)) * 100;
	}
};





/**
    * This is the base class for incoming http messages (requests or responses 
    * for servers or clients respectively). These will be created by a Parser 
    * after parsing headers and will be passed to a request/response handler 
    * for further processing and registering event listeners. It implements 
    * the ReadableStream interface and read-only access to encapsulated
    * instance of detail::http_message.
    *
    * Emits the following events:
    *   data  - when a chunk of the body is received
    *   end   - the body is finished and no more data events to come
    *   close - the connection was terminated before finishing
    *   error - there was an error with the connection or HTTP parsing
    */
	/*
class Transaction : public net::Transaction<http::Request>
{
protected:
    Parser* parser_;

    net::Socket* socket_;

    bool complete_;
    bool readable_;
    bool paused_;
    std::vector<Buffer> pendings_;
    bool endEmitted_;
    http_start_line message_;
    headers_type headers_;
    headers_type trailers_;
    bool body_;


    bool readable();
    void pause();
    void resume();

    void destroy(const Exception& e);

public:
    Transaction(net::Socket* socket, Parser* parser);

    virtual ~Transaction() {}

    Parser* parser();

    net::Socket* socket();

    // Read-only HTTP message
    int statusCode();
    const http_method& method();
    const http_version& httpVersion();
    std::string httpVersionString();
    int httpVersionMajor();
    int httpVersionMinor();
    const URL& url();

    bool shouldKeepAlive();
    bool upgrade();

    void end();

    // TODO: handle encoding
    //void setEncoding();

    const headers_type& headers() const;
    bool has_header(const std::string& name);
    const std::string& get_header(const std::string& name);

    const headers_type& trailers() const;
    bool has_trailer(const std::string& name);
    const std::string& get_trailer(const std::string& name);

    void onParserHeader(const std::string& name, const std::string& value);
    void onParserError(const Exception& e);
    void onParserChunk(const Buffer& body);
    virtual void onParserHeadersEnd();

protected:

};
	*/
	
    /**
    * Called from parser to signal end of message
    */
		
	/*
    void _emitPending(std::function<void()> callback);
    void _emitData(const Buffer& buf);
    void _emitEnd();
    void _addHeaderLine(const std::string& field, const std::string& value);
	*/

/*

// ---------------------------------------------------------------------
//
class Transaction: public StatefulSignal<net::TransactionState>, public ISendable, public IPolymorphic
	/// Implements a stateful HTTP request/response 
	/// transaction with progress updates.
{
public:
	Transaction(Request* request = NULL);
	virtual ~Transaction();

	virtual bool send();
	virtual void cancel();
		/// Cancels the transaction.
		/// onComplete() will never be called.
	
	virtual Request& request();
	virtual Response& response();
	virtual TransferProgress& requestState();
	virtual TransferProgress& responseState();
	virtual bool cancelled();

	virtual void setRequest(Request* request);
		/// Sets the Request pointer if it wasn't set
		/// via the constructor.
		/// The transaction takes ownership of the pointer.
		
	virtual std::string& outputPath();
	virtual void setOutputPath(const std::string& path);
		/// If the path is set the response data will be
		/// saved to this location on the file system.
	
	virtual void* clientData() const;
	virtual void setClientData(void* clientData);
		/// Sets an arbitrary pointer to associate with
		/// this transaction.
	
	Signal<TransferProgress&> UploadProgress;
	Signal<TransferProgress&> DownloadProgress;
	Signal<Response&> Complete;
	
	virtual const char* className() const { return "HTTPTransaction"; }
	
protected:
	virtual void processRequest(std::ostream &ost);
	virtual void processResponse(std::istream &ist);
	virtual void onComplete();
	
	virtual void setRequestState(TransferProgress::Type state);
	virtual void setResponseState(TransferProgress::Type state);

protected:
	Request*		_request;
	Response		_response;
	Poco::URI		_uri;
	TransferProgress _requestState;
	TransferProgress _responseState;
	std::string		_outputPath;
	void*			_clientData;
	Poco::Net::HTTPSession*	_session;
	mutable Poco::FastMutex	_mutex;
};
*/


} } // namespace scy::http


#endif