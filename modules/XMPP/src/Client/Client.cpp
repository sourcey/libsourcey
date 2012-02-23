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


#include "Sourcey/XMPP/client/Client.h"
#include "Sourcey/XMPP/Router.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Base.h"

#include "Poco/String.h"
#include "Poco/Mutex.h"

#include "src/common.h"

#include <assert.h>
#include <iostream>
#include <string>


using namespace std;
using Poco::FastMutex;


namespace Sourcey {
namespace XMPP {


Client::Client(Router& router): 
	_router(router),
	_task(NULL)	
{
}


Client::~Client() 
{
	cleanup();
}


void Client::setCredentials(const string& jid, 
							const string& password, 
							const string& domain, 
							short port) 
{
	Log("debug", this) << "Setting Credentials: " << jid << ": " << password << endl; 
	
	_credentials.jid = jid;
	_credentials.password = password;
	_credentials.domain = domain;
	_credentials.port = port;
}


void Client::login() 
{
	Log("debug", this) << "Logging in as " << _credentials.jid << endl; 

	assert(_credentials.valid());

	// Disconnect if we are connected.
	cleanup();

	// Connected state will be set by the XMPP task
	// on success.
	setState(ClientState::Connecting);

	// The default implementation will use the
	// Router's Task Runner for the receiver task.
	_task = new XMPPTask(*this, _router.runner());
	_task->start();
}

	
void Client::logout() 
{
	Log("debug", this) << "Logging out" << endl; 	
	
	cleanup();

	// The task will set the state to Disconnected
	// on destruction.
}


void Client::cleanup()
{
	if (_task) {
		// Stop receiving outbound Stanzas
		_router.detachAll(_task);

		_task->destroy();
		_task = NULL;
	}
}


void Client::send(const Stanza& stanza) 
{
	ostringstream ost;
	stanza.print(ost);
	send(ost.str());
}


void Client::send(const string& raw) 
{
	if (!isConnected())
		throw Exception("The XMPP client is not connected.");
	
	assert(_task);
	_task->send(raw);
}


Router& Client::router() const
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _router; 
}	


ClientCredentials& Client::credentials()
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _credentials; 
}


const StreamError* Client::error() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _task ? _task->error() : 0;
}


string Client::jid() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _task->jid();
}


bool Client::setState(unsigned int id) 
{ 
	return StatefulSignal<ClientState>::setState(this, id); 
}



//
// Libstrophe C Callbacks
//
int HandleXMPPPresence(xmpp_conn_t* const conn, xmpp_stanza_t* const stanza, void* const clientdata)
{
	XMPPTask* klass = reinterpret_cast<XMPPTask*>(clientdata);
	klass->onRecvStanza(stanza);
	return 1;
}


int HandleXMPPMessage(xmpp_conn_t* const conn, xmpp_stanza_t* const stanza, void* const clientdata)
{	
	XMPPTask* klass = reinterpret_cast<XMPPTask*>(clientdata);
	klass->onRecvStanza(stanza);
	return 1;	
}


int HandleXMPPIQ(xmpp_conn_t* const conn, xmpp_stanza_t* const stanza, void* const clientdata)
{
	XMPPTask* klass = reinterpret_cast<XMPPTask*>(clientdata);	

	// Use the initial session IQ to grab our JID and set us as Connected. 
	// Update our JID with the server provided.
    if (strcmp(xmpp_stanza_get_id(stanza), "_xmpp_session1") == 0 &&
		strcmp(xmpp_stanza_get_name(stanza), "iq") == 0) {

		// Now that we are conneceted we can start receiving outgoing XMPP
		// stanzas from the router. Outgoing stanzas will be automatically
		// sent to the remote client.
		klass->client().router().setJID(xmpp_stanza_get_attribute(stanza, "to"));
		
		//klass->client().router().attach(
		//	xmppDelegate(klass, &XMPPTask::onSendStanza, //XMPPDelegate<XMPPTask, Stanza>
		//		AcceptOutbound// | AcceptPresenceStanzas
		//	)
		//);

		klass->client().setState(ClientState::Connected);
		return 1;
	}

	klass->onRecvStanza(stanza);
	return 1;
}


void HandleXMPPConnection(xmpp_conn_t* const conn, const xmpp_conn_event_t status, 
		  const int error, xmpp_stream_error_t* const stream_error,
		  void* const clientdata)
{
	XMPPTask* klass = reinterpret_cast<XMPPTask*>(clientdata);	
	
    if (status == XMPP_CONN_CONNECT) {	
		Log("debug", klass) << "Connected" << endl;	
		
		xmpp_handler_add(conn, HandleXMPPPresence, NULL, "presence", NULL, clientdata);
		xmpp_handler_add(conn, HandleXMPPIQ, NULL, "iq", NULL, clientdata);
		xmpp_handler_add(conn, HandleXMPPMessage, NULL, "message", NULL, clientdata);
		
		// NOTE: The client is not considered connected until the
		// initial session IQ stanza has been received. 
		// @see HandleXMPPIQ()
    }
    else {
		// NOTE: The stream_error variable is unreliable. One other
		// way to determine error typess is to use the area value of
		// the area log to determine the error type.
		Log("debug", klass) << "Disconnected: " 
			<< (stream_error ? stream_error->text : "No Error") << endl;	
		  
		// The client state will hold the error message.
		klass->client().setState(ClientState::Disconnected); 
		
		// Abort the task...
		klass->client().cleanup();
    }
}


void HandleXMPPLogger(void* const clientdata,
			 const xmpp_log_level_t level,
			 const char* const area,
			 const char* const msg)
{
	//XMPPTask* klass = reinterpret_cast<XMPPTask*>(clientdata);	
	
	string message(msg);

	switch (level) {
	case XMPP_LEVEL_DEBUG:
		//Log("debug") << "Trace: " << message << endl; 
		break;

	case XMPP_LEVEL_INFO:
		Log("debug") << "Info: " << message << endl; 
		break;

	case XMPP_LEVEL_WARN:
		Log("warn") << "Warning: " << message << endl; 
		break;

	case XMPP_LEVEL_ERROR:
		Log("error") << "Error: " << message << endl; 

		//string errorType(area);		
		//errorType[0] = toupper(errorType[0]);
		//klass->client().setStateMessage(errorType + ": " + message); 
		break;
	}    
}


static xmpp_log_t XMPPLogger = { HandleXMPPLogger, NULL };


// ---------------------------------------------------------------------
//
XMPPTask::XMPPTask(Client& client, Runner& runner) :
	ITask(runner),
	_client(client),
	_context(NULL),
    _connection(NULL),
    _log(NULL)
{	
#ifdef _DEBUG
	ostringstream ss;
	ss << "XMPPTask[" << &_client << "]";
	_name = ss.str();
#endif
}
	

XMPPTask::~XMPPTask()
{	
	//Log("debug", this) << "Destroying" << endl;	

	// Release our connection and context
	xmpp_conn_release(_connection);
	xmpp_ctx_free(_context);

	// Shutdown libstrophe
	xmpp_shutdown();
	
	_connection = NULL;
	_context = NULL;
	_log = NULL;
	
	Log("debug", this) << "Destroying XMPP: OK" << endl;		
}


bool XMPPTask::start()
{
	FastMutex::ScopedLock lock(_mutex);
	
	Log("debug", this) << "Starting XMPP" << endl;	

    // Initialize libstrophe
    xmpp_initialize();

    // Create a connection and setup authentication information.
    //_log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG);
	_log = &XMPPLogger;
    _context = xmpp_ctx_new(NULL, _log);
    _connection = xmpp_conn_new(_context);
    xmpp_conn_set_jid(_connection, _client.credentials().jid.data());
    xmpp_conn_set_pass(_connection, _client.credentials().password.data());

    // Initiate the connection.
	// NOTE: This will stall the current thread for timeout x 
	// seconds if we can't connect.
    xmpp_connect_client(_connection, 
		_client.credentials().domain.empty() ? NULL : _client.credentials().domain.data(), 
		_client.credentials().port, HandleXMPPConnection, this);
	
	Log("debug", this) << "Starting XMPP: OK" << endl;		

	return ITask::start();
}


void XMPPTask::send(const Stanza& stanza) 
{
	ostringstream ost;
	stanza.print(ost);
	send(ost.str());
}


void XMPPTask::send(const string& raw)
{
	Log("debug", this) << "SEND: " << raw.length() << ": >>>>>>>>>>>>>>>>>>>>\n" << raw << endl;
	
	FastMutex::ScopedLock lock(_mutex);

	// No exception is thrown because we are
	// most likely inside the runner loop.
	if (!_connection) {
		Log("error", this) << "Dropping stanza. The XMPP client is not connected." << endl;
		return;
	}
	
	xmpp_send_raw(_connection, raw.data(), raw.length());
}


Client& XMPPTask::client() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _client;
}


string XMPPTask::jid() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _connection
		&& _connection->bound_jid 
		 ? _connection->bound_jid : ""; //_jid;
}


const StreamError* XMPPTask::error() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _connection
		&& _connection->stream_error 
		 ? _connection->stream_error : 0;
}


/*
string XMPPTask::errorMessage() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _connection
		&& _connection->stream_error 
		 ? _connection->stream_error->text : "";
}
*/


xmpp_ctx_t*	XMPPTask::context() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _context;
}


xmpp_conn_t* XMPPTask::connection() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _connection;
}


xmpp_log_t*	XMPPTask::log() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _log;
}


void XMPPTask::run()
{	
	FastMutex::ScopedLock lock(_mutex);

    // Run the XMPP event loop once with a 1ms timeout.
	assert(_context);
	xmpp_run_once(_context, 1);
}


void XMPPTask::onSendStanza(void*, XMPP::Stanza& stanza)
{
	send(stanza);
}


void XMPPTask::onRecvStanza(xmpp_stanza_t* const stanza)
{	
	Log("debug", this) << "RECV >>>>>>>>>>>>>>>>>>>>" << endl;
	char* buf;
	size_t len;
	int ret;
	if ((ret = xmpp_stanza_to_text(stanza, &buf, &len)) == 0) {
		Log("debug", this) << string(buf, len) << endl;
		Stanza* s = Stanza::create(buf);
		if (s) {
			FastMutex::ScopedLock lock(_mutex);
			_client.router() >> *s;
			delete s;
		}
		else
			Log("error", this) << "Unable to create stanza." << endl;
		delete buf;
	}
}


} // namespace XMPP 
} // namespace Sourcey