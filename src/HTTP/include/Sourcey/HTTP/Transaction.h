//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_HTTP_Transaction_H
#define SOURCEY_HTTP_Transaction_H


#include "Sourcey/Signal.h"
#include "Sourcey/ISendable.h"
#include "Sourcey/PacketTransaction.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Response.h"

#include "Poco/Net/HTTPSession.h"
#include "Poco/URI.h"
#include "Poco/Thread.h"

	
namespace Scy { 
namespace HTTP {


struct TransferState 
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

	TransferState() :	
		current(0), total(0), state(None) {}

	double progress() {
		return (current / (total * 1.0)) * 100;
	}
};


// ---------------------------------------------------------------------
class Transaction: public StatefulSignal<TransactionState>, public ISendable, public IPolymorphic
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
	virtual TransferState& requestState();
	virtual TransferState& responseState();
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
	
	Signal<TransferState&> RequestProgress;  /// upload progress
	Signal<TransferState&> ResponseProgress; /// download progress
	Signal<Response&> Complete;
	
	virtual const char* className() const { return "HTTPTransaction"; }
	
protected:
	virtual void processRequest(std::ostream &ost);
	virtual void processResponse(std::istream &ist);
	virtual void onComplete();
	
	virtual void setRequestState(TransferState::Type state);
	virtual void setResponseState(TransferState::Type state);

protected:
	Request*		_request;
	Response		_response;
	Poco::URI		_uri;
	TransferState	_requestState;
	TransferState	_responseState;
	std::string		_outputPath;
	void*			_clientData;
	Poco::Net::HTTPSession*	_session;
	mutable Poco::FastMutex	_mutex;
};


} } // namespace Scy::HTTP


#endif