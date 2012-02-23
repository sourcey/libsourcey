//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef ANIONU_API_CLIENT_H
#define ANIONU_API_CLIENT_H


#include "Sourcey/Base.h"
#include "Sourcey/Signal.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Response.h"
#include "Sourcey/HTTP/Transaction.h"
#include "Sourcey/XML/XML.h"

	
namespace Sourcey { 
namespace Anionu {


#ifdef _DEBUG
#define ANIONU_API_ENDPOINT "http://localhost:3000"
#else
#define ANIONU_API_ENDPOINT "https://anionu.com"
#endif


// ---------------------------------------------------------------------
//
class APIService 
{
public:
	APIService();

	virtual void format(const std::string& format);
		/// Sets the format as .html, .xml, .api

	virtual void interpolate(const StringMap& params);
		/// Interpolates a URI parameter eg. :key => value

public:
	std::string name;
	std::string method;
	std::string contentType;
	Poco::URI uri;
	bool isAnonymous;
};


// ---------------------------------------------------------------------
//
class APIServices: public XML::Document 
	/// Stores and parses the services descriptor from the Sourcey API.
	/// TODO: Use JSON for service description.
	/// The input XML should be in the following format:
	///
	/// <services>
	///		<service>
	///			<name>SomeMethod</name>
	///			<method>GET</method>
	///			<uri>https://anionu.com/...</uri>
	///		</service>
	/// </services>
{
public:
	APIServices();
	virtual ~APIServices();

	virtual void update();
	virtual bool valid();

	virtual APIService get(const std::string& name, const std::string& format = "json", const StringMap& params = StringMap());

private:
	mutable Poco::FastMutex	_mutex;
};


// ---------------------------------------------------------------------
//
struct APICredentials
{	
	std::string username;
	std::string password;

	APICredentials(
		const std::string& username = "", 
		const std::string& password = "") :
		username(username), password(password) {} 
};


// ---------------------------------------------------------------------
//
struct APIRequest: public HTTP::Request
{
	APIRequest() : HTTP::Request(HTTPMessage::HTTP_1_1) {} 	
	APIRequest(const APIService& service, 
			   const APICredentials& credentials) :
		HTTP::Request(HTTPMessage::HTTP_1_1),
		service(service), 
		credentials(credentials) {}
	virtual ~APIRequest() {}

	virtual void prepare();
		// MUST be called after setting all information and
		// credentials before sending the request.

	APIService service;

	APICredentials credentials;
		// HTTP authentication credentials.

private:
	APIRequest(APIRequest& r) {}
		// The copy constructor is private (Poco limitation).
};


// ---------------------------------------------------------------------
//
class APITransaction: public HTTP::Transaction
{
public:
	APITransaction(APIRequest* request = NULL);
	virtual ~APITransaction();
	
	Signal2<APIService&, HTTP::Response&> APITransactionComplete;

protected:
	virtual void processCallbacks();
};

	
typedef AsyncSendable<APITransaction> AsyncTransaction;
typedef std::vector<APITransaction*> APITransactionList;


// ---------------------------------------------------------------------
//
class APIClient
{
public:
	APIClient();
	virtual ~APIClient();
	
	virtual void setCredentials(const std::string& username, const std::string& password);
		// Sets the credentials for authenticating HTTP requests.

	virtual bool loadServices(bool whiny = true);
		// Loads the service descriptions from the Anionu server.
		// This must be called before calling any services.

	virtual bool isOK();
		// Returns true when services descriptions are loaded and
		// the API is avilable.

	virtual APIServices& services();
		// Returns the services descriptions XML.
	
	virtual APIRequest* createRequest(const APIService& service);
	virtual APIRequest* createRequest(const std::string& service, 
									  const std::string& format = "json", 
									  const StringMap& params = StringMap());

	virtual APITransaction* call(APIRequest* request);
	virtual APITransaction* call(const APIService& service);
	virtual APITransaction* call(const std::string& service, 
								 const std::string& format = "json", 
								 const StringMap& params = StringMap());
	
	virtual AsyncTransaction* callAsync(APIRequest* request);
	virtual AsyncTransaction* callAsync(const APIService& service);
	virtual AsyncTransaction* callAsync(const std::string& service, 
										const std::string& format = "json", 
										const StringMap& params = StringMap());
protected:
	void stopWorkers();
	void onTransactionComplete(void* sender, HTTP::Response& response);

private:
	APIServices			_services;
	APICredentials		_credentials;
	APITransactionList	_transactions;
	mutable Poco::FastMutex _mutex;
};



} } // namespace Sourcey::Anionu


#endif