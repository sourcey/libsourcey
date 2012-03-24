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
#include "Poco/URIStreamOpener.h"
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
	_response(HTTPResponse::HTTP_NOT_FOUND),
	_clientData(NULL)
{
	Log("trace") << "[HTTPTransaction] Creating" << endl;
}


Transaction::~Transaction()
{
	Log("trace") << "[HTTPTransaction] Destroying" << endl;
	if (_session) {
		if (_uri.getScheme() == "http")
			delete static_cast<HTTPClientSession*>(_session); // base is protected

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
		
		Log("trace") << "[HTTPTransaction] Running:" 
			<< "\n\tMethod: " << _request->getMethod()
			<< "\n\tHas Credentials: " << _request->hasCredentials()
			<< "\n\tURI: " << _request->getURI()
			<< "\n\tStatus: " << _response.getStatus()
			//<< "\n\tOutput Path: " << _outputPath
			<< endl;
		
		_uri = URI(_request->getURI());
		if (_uri.getScheme() == "http") {
			HTTPClientSession session(_uri.getHost(), _uri.getPort());
			session.setTimeout(Timespan(6, 0));
			ostream &ost = session.sendRequest(*_request);
			processRequest(ost);
			istream& ist = session.receiveResponse(_response);
			processResponse(ist);
		} 
		else if (_uri.getScheme() == "https") {
			HTTPSClientSession session(_uri.getHost(), _uri.getPort());
			session.setTimeout(Timespan(6, 0));
			ostream &ost = session.sendRequest(*_request);
			processRequest(ost);
			istream& ist = session.receiveResponse(_response);
			processResponse(ist);
		} 
		else
			throw Exception("Unable to create HTTP session for scheme: " + _uri.getScheme());

		if (!cancelled())
			setState(this, _response.success() ? 
				TransactionState::Success : TransactionState::Failed, 
				_response.getReason());
	}
	catch (StopPropagation&) {
		Log("trace") << "[HTTPTransaction] Cancelled" << endl;
		return false;
	}
	catch (Exception& exc) {
		Log("error") << "[HTTPTransaction] Failed: " << exc.displayText() << endl;
		_response.error = exc.displayText();
		setState(this, TransactionState::Failed, _response.error);
		//exc.rethrow();
	}

	Log("trace") << "[HTTPTransaction] Response:" 
		<< "\n\tStatus: " << _response.getStatus()
		<< "\n\tReason: " << _response.getReason()
		<< endl;

	dispatchCallbacks();
	return _response.success();
}


void Transaction::cancel()
{
	Log("trace") << "[HTTPTransaction] Cancelling" << endl;
	FastMutex::ScopedLock lock(_mutex);
	setState(this, TransactionState::Cancelled);
}

	
void Transaction::updateUploadState(TransferState::Type state, UInt32 current)
{
	_uploadState.state = state;
	_uploadState.current = current;
	UploadProgress.dispatch(this, _uploadState);
}


void Transaction::updateDownloadState(TransferState::Type state, UInt32 current)
{
	_downloadState.state = state;
	_downloadState.current = current;
	DownloadProgress.dispatch(this, _downloadState);
}


void Transaction::processRequest(ostream &ost)
{
	try 
	{
		if (_request->form) {
			//UInt32 bytesTotal = _request->getContentLength();
			UInt32 bytesUploaded = 0;
			streambuf* pbuf = _request->body.rdbuf(); 
			while (pbuf->sgetc() != EOF) {
				char ch = pbuf->sbumpc();
				ost << ch;
				bytesUploaded++;
				if (bytesUploaded % 32768 == 0) {					
					if (cancelled()) {
						updateUploadState(TransferState::Cancelled, bytesUploaded);
						throw StopPropagation();
					}

					//Log("trace") << "Upload progress: " << bytesUploaded << " of " << bytesTotal << endl;
					updateUploadState(TransferState::Running, bytesUploaded);
				}
			}
		
			updateUploadState(TransferState::Complete, bytesUploaded);
		}
	}
	catch (Exception& exc) 
	{
		Log("trace") << "[HTTPTransaction] Request Error: " << exc.displayText() << endl;
		updateUploadState(TransferState::Failed, 0);
		exc.rethrow();
	}		
}


void Transaction::processResponse(istream &ist)
{		
	try 
	{
		// TODO: Upload progress

		// If no output path is set copy the response data to 
		// our response object.
		if (_outputPath.empty()) {
			StreamCopier::copyStream(ist, _response.body);
			//StreamCopier::copyStreamUnbuffered(rs, _response.data);
		}
		
		// Otherwise save the response to the output file.
		else {
			Log("trace") << "[HTTPTransaction] Saving output file: " << _outputPath << endl;
			Path dir(_outputPath);
			dir.setFileName("");
			File(dir).createDirectories();	
			FileOutputStream fost(_outputPath);
			StreamCopier::copyStream(ist, fost);
			//StreamCopier::copyStreamUnbuffered(ist, fost);
		}

		updateDownloadState(TransferState::Complete, 0);
	}
	catch (Exception& exc) 
	{
		Log("trace") << "[HTTPTransaction] Response Error: " << exc.displayText() << endl;
		updateDownloadState(TransferState::Failed, 0);
		exc.rethrow();
	}
}


void Transaction::dispatchCallbacks()
{	
	if (!cancelled())
		TransactionComplete.dispatch(this, _response);
}


bool Transaction::cancelled()
{
	FastMutex::ScopedLock lock(_mutex);	
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


TransferState& Transaction::uploadState() 
{
	FastMutex::ScopedLock lock(_mutex);
	return _uploadState; 
}


TransferState& Transaction::downloadState() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _uploadState; 
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