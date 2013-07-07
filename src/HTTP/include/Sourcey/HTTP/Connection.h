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


#ifndef SOURCEY_HTTP_ServerConnection_H
#define SOURCEY_HTTP_ServerConnection_H


#include "Sourcey/Timeout.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Response.h"
#include "Sourcey/HTTP/Parser.h"
#include "Sourcey/HTTP/Packetizers.h"

	
namespace scy { 
namespace http {

		
class Connection: public ParserObserver, public net::SocketObserver
	/// TODO: 
	///		- extend SocketObserver
	///		- attach output IPacketizer
	///     - 
	//add timeout
{
public:
    Connection(const net::Socket& socket, http_parser_type type);
					
	virtual bool sendHeaders(bool whiny = false);
	virtual bool sendBytes(const char* buf, size_t len, bool whiny = false);

	virtual void close();
	virtual void onClose();

	bool closed();
	bool expired();

	Request& request();	
	Response& response();
	net::Socket& socket();
	Buffer& buffer();
	
	NullSignal Close;	
		/// Close signal fires on connection closure.

protected:

	//
	/// Socket emitter callbacks
	virtual void onSocketConnect() {};
	virtual void onSocketRecv(Buffer& buf, const net::Address& peerAddr);
	virtual void onSocketError(int syserr, const std::string& message) {};
	virtual void onSocketClose();
		
	//
	/// Parser callbacks
    virtual void onParserHeader(const std::string& name, const std::string& value);
	virtual void onParserHeadersDone() = 0;
	virtual void onParserChunk(const char* buf, size_t len) = 0;
    virtual void onParserError(const ParserError& err);
    virtual void onParserDone() = 0;


protected:
    virtual ~Connection();

    Request* _request;
    Response* _response;
    net::Socket _socket;	
	Timeout _timeout;
	//Buffer _buffer;
    Parser _parser;
	bool _closed;
	bool _sentResponseHeaders;
	IPacketizer* _packetizer;
	
	friend class Parser;
	friend class Deleter<Connection>;
};


inline bool isExplicitKeepAlive(Poco::Net::HTTPMessage* message) 
{	
	const std::string& connection = message->get(Poco::Net::HTTPMessage::CONNECTION, Poco::Net::HTTPMessage::EMPTY);
	return !connection.empty() && Poco::icompare(connection, Poco::Net::HTTPMessage::CONNECTION_KEEP_ALIVE) == 0;
}


} } // namespace scy::http


#endif

//#include "Sourcey/Stateful.h"
//#include "Sourcey/ISendable.h"
//#include "Sourcey/Net/ServerConnection.h"
//#include "Poco/Net/HTTPSession.h"
//#include "Poco/URI.h"
//#include "Poco/Thread.h"

		
/*
 //: public net::SocketEmitter
 //Buffer& buf, const net::Address& peerAddr
class ServerResponser
	// TODO: Chunked ServerConnection Handler
{
public:
	virtual void handleRequest(ServerConnection* connection, Request& request, Response& response) = 0;
};


class DefaultServerResponser
	// TODO: Chunked ServerConnection Handler
{
public:
	virtual void handleRequest(ServerConnection* connection, Request& request, Response& response);
};
*/





    /**
	//virtual void onConnect();
    * 
    */
	/*, Parser* parser)
	
protected:
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

    void _emitPending(std::function<void()> callback);
    void _emitData(const Buffer& buf);
    void _emitEnd();
    void _addHeaderLine(const std::string& field, const std::string& value);
	
    // TODO: handle encoding
    //void setEncoding();

    const headers_type& headers();
    bool has_header(const std::string& name);
    const std::string& get_header(const std::string& name);

    const headers_type& trailers() const;
    bool has_trailer(const std::string& name);
    const std::string& get_trailer(const std::string& name);
	*/
	
	/*
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

    void destroy(const ParserError& err);
	*/

/*

// ---------------------------------------------------------------------
//
class ServerConnection: public StatefulSignal<net::ServerConnectionState>, public ISendable, public IPolymorphic
	/// Implements a stateful HTTP request/response 
	/// transaction with progress updates.
{
public:
	ServerConnection(Request* request = NULL);
	virtual ~ServerConnection();

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
	
	virtual const char* className() const { return "ServerConnection"; }
	
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