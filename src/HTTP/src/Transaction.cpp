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


#include "Sourcey/HTTP/Transaction.h"
#include "Sourcey/Logger.h"

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

#include <assert.h>


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace scy { 
namespace http {



	/*
Transaction::Transaction(Request* request) : 
	_request(request),
	_session(NULL),
	_response(HTTPResponse::HTTP_SERVICE_UNAVAILABLE),
	_clientData(NULL)
{
	log("trace") << "Creating" << endl;
}


Transaction::~Transaction()
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


bool Transaction::send() 
{
	assert(_request);
	try 
	{
		setState(this, net::TransactionState::Running);
	
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
				net::TransactionState::Success : net::TransactionState::Failed, 
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
		setState(this, net::TransactionState::Failed, _response.error);
	}

	onComplete();
	return _response.success();
}


void Transaction::cancel()
{
	log("trace") << "Cancelling" << endl;
	assert(!cancelled());
	setState(this, net::TransactionState::Cancelled);
}


void Transaction::processRequest(ostream& ostr)
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


void Transaction::setRequestState(TransferProgress::Type state)
{
	_requestState.state = state;
	UploadProgress.emit(this, _requestState);
}


void Transaction::processResponse(istream& istr)
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

void Transaction::setResponseState(TransferProgress::Type state)
{
	_responseState.state = state;
	DownloadProgress.emit(this, _responseState);
}


void Transaction::onComplete()
{	
	assert(!cancelled());
	Complete.emit(this, _response);
}


bool Transaction::cancelled()
{
	return stateEquals(net::TransactionState::Cancelled);
}


Request& Transaction::request() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return *_request;
}


Response& Transaction::response() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _response; 
}


TransferProgress& Transaction::requestState() 
{
	FastMutex::ScopedLock lock(_mutex);
	return _requestState; 
}


TransferProgress& Transaction::responseState() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _requestState; 
}	


void Transaction::setRequest(Request* request)
{ 
	FastMutex::ScopedLock lock(_mutex);
	_request = request;
}


void Transaction::setOutputPath(const string& path)
{ 
	FastMutex::ScopedLock lock(_mutex);
	_outputPath = path;
}


std::string& Transaction::outputPath() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _outputPath; 
}


void Transaction::setClientData(void* clientData)
{ 
	FastMutex::ScopedLock lock(_mutex);
	_clientData = clientData;
}


void* Transaction::clientData() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _clientData;
}
*/


} } // namespace scy::http