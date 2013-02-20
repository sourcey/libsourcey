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


namespace Sourcey { 
namespace HTTP {


Transaction::Transaction(Request* request) : 
	_request(request),
	_session(NULL),
	_response(HTTPResponse::HTTP_SERVICE_UNAVAILABLE),
	_clientData(NULL)
{
	Log("trace", this) << "Creating" << endl;
}


Transaction::~Transaction()
{
	Log("trace", this) << "Destroying" << endl;
	// Free HTTP client session like so as base is protected
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
		setState(this, TransactionState::Running);
	
		_request->prepare();
		_uri = URI(_request->getURI());
		_request->setURI(_uri.getPathAndQuery()); // ensure URI only in request

		Log("trace", this) << "Running:" 
			<< "\n\tMethod: " << _request->getMethod()
			<< "\n\tHas Credentials: " << _request->hasCredentials()
			<< "\n\tURL: " << _uri.toString()
			<< "\n\tURI: " << _request->getURI()
			<< "\n\tHost: " << _uri.getHost()
			<< "\n\tPort: " << _uri.getPort()
			<< "\n\tOutput Path: " << _outputPath
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
				TransactionState::Success : TransactionState::Failed, 
				_response.getReason());
	}
	catch (StopPropagation&) {
		Log("trace", this) << "Cancelled" << endl;
		return false;
	}
	catch (Exception& exc) {
		Log("error", this) << "Failed: " << exc.displayText() << endl;
		_response.error = exc.displayText();
		setState(this, TransactionState::Failed, _response.error);
		//exc.rethrow();
	}

	Log("trace", this) << "Response: " 
		<< _response.getStatus() << ": " 
		<< _response.getReason() << endl;

	dispatchCallbacks();
	return _response.success();
}


void Transaction::cancel()
{
	Log("trace", this) << "Cancelling" << endl;
	setState(this, TransactionState::Cancelled);
}


void Transaction::processRequest(ostream& ostr)
{
	//stringstream ss;
	//_request->write(ss);	
	//Log("trace", this) << "Request Headers: " << ss.str() << endl;

	try 
	{
		TransferState& st = _requestState;
		st.total = _request->getContentLength();
		setRequestState(TransferState::Running);
		if (_request->form) 
		{
			//Log("trace", this) << "Request Body: " << string(ss.str().data(), 100) << endl;
			
			char c;
			streambuf* pbuf = _request->body.rdbuf(); 
			while (pbuf->sgetc() != EOF) {				
				c = pbuf->sbumpc();
				ostr << c;
				st.current++;
				if (st.current % 32768 == 0) {					
					if (cancelled()) {
						setRequestState(TransferState::Cancelled);
						throw StopPropagation();
					}
					
					Log("trace", this) << "Upload progress: " << 
						st.current << " of " << 
						st.total << endl;

					setRequestState(TransferState::Running);
				}
			}		
		}
		setRequestState(TransferState::Complete);
	}
	catch (Exception& exc) 
	{
		Log("error", this) << "Request Error: " << exc.displayText() << endl;
		setRequestState(TransferState::Failed);
		exc.rethrow();
	}	
}


void Transaction::setRequestState(TransferState::Type state)
{
	_requestState.state = state;
	RequestProgress.dispatch(this, _requestState);
}


void Transaction::processResponse(istream& istr)
{	
	//stringstream ss;
	//_response.write(ss);	
	//Log("trace", this) << "Response Headers: " << ss.str() << endl;

	try 
	{	
		char c;
		ofstream* fstr = _outputPath.empty() ? NULL : 
			new ofstream(_outputPath.data(), ios_base::out | ios_base::binary);
		ostream& ostr = fstr ? 
			static_cast<ostream&>(*fstr) : 
			static_cast<ostream&>(_response.body);
		TransferState& st = _responseState;
		st.total = _response.getContentLength();
		setResponseState(TransferState::Running);
		istr.get(c);
		while (istr && ostr)
		{
			st.current++;
			ostr.put(c);
			istr.get(c);
			if (st.current % 32768 == 0) {					
				if (cancelled()) {
					setResponseState(TransferState::Cancelled);
					throw StopPropagation();
				}
				
				Log("trace", this) << "Download progress: " 
					<< st.current << " of " 
					<< st.total << endl;
	
				setResponseState(TransferState::Running);
			}
		}
				
		if (fstr)
			fstr->close();
		
		setResponseState(TransferState::Complete);
	}
	catch (Exception& exc) 
	{
		Log("error", this) << "Response Error: " << exc.displayText() << endl;
		setResponseState(TransferState::Failed);
		exc.rethrow();
	}
}

void Transaction::setResponseState(TransferState::Type state)
{
	_responseState.state = state;
	ResponseProgress.dispatch(this, _responseState);
}


void Transaction::dispatchCallbacks()
{	
	if (!cancelled())
		Complete.dispatch(this, _response);
}


bool Transaction::cancelled()
{
	return stateEquals(TransactionState::Cancelled);
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


TransferState& Transaction::requestState() 
{
	FastMutex::ScopedLock lock(_mutex);
	return _requestState; 
}


TransferState& Transaction::responseState() 
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


} } // namespace Sourcey::HTTP


	
	
				//ch = pbuf->sbumpc();
				//ostr << ch;
			//std::streamsize _requestState.current = 0;

	/*
	char c;
				//ch = pbuf->sbumpc();
				//ostr << ch;
	char c;
	std::streamsize _requestState.current = 0;
	std::streamsize total = _request->getContentLength();
	ostream& istr =_request->body;
	ostr.get(c);
	while (ostr && istr)
	{
		++_requestState.current;
		istr.put(c);
		ostr.get(c);
		if (_requestState.current % 32768 == 0) {					
			if (cancelled()) {
				setResponseState(TransferState::Cancelled, _requestState.current);
				throw StopPropagation();
			}
				
			Log("trace", this) << "Upload progress: " << _requestState.current << " of " << total << endl;
			//Log("trace", this) << "Upload progress: " << _requestState.current << endl;
			setResponseState(TransferState::Running, _requestState.current);
		}
	}

	
	*/
			
	//char c;

	

	/*
	ofstream* ofstr = NULL;	
	if (!_outputPath.empty()) {
		ofstr = new ofstream(_outputPath.data());
		if (!ofstr->is_open())
			throw Exception("Cannot open output file: " + _outputPath);
	}
	ostream& ostr = ofstr ? reinterpret_cast<ostream&>(*ofstr) : _response.body;
	*/
			
	/*
	try 
	{
		// If no output path is set copy the response data to 
		// our response object.
		if (_outputPath.empty()) {
			StreamCopier::copyStream(ist, _response.body);
			//StreamCopier::copyStreamUnbuffered(rs, _response.data);
		}
		
		// Otherwise save the response to the output file.
		else {
			Log("trace", this) << "Saving output file: " << _outputPath << endl;
			Path dir(_outputPath);
			dir.setFileName("");
			File(dir).createDirectories();	
			FileOutputStream ofstr(_outputPath);
			StreamCopier::copyStream(ist, ofstr);
			//StreamCopier::copyStreamUnbuffered(ist, ofstr);
		}

		setResponseState(TransferState::Complete, 0);
	}
	catch (Exception& exc) 
	{
		Log("trace", this) << "Response Error: " << exc.displayText() << endl;
		setResponseState(TransferState::Failed, 0);
		exc.rethrow();
	}
	*/