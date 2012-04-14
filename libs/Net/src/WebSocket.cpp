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


#include "Sourcey/Net/WebSocket.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace Net {


WebSocket::WebSocket(Reactor& reactor) :
	TCPClientSocket(reactor)
{  
}


WebSocket::WebSocket(Reactor& reactor, const URI& uri) :
	TCPClientSocket(reactor),
	_headerState(0),
	_uri(uri)
{
}


WebSocket::~WebSocket()
{
}


void WebSocket::connect(const Poco::URI& uri)
{
	_uri = uri;
	connect();
}


void WebSocket::connect()
{
	_headerState = 0;
	
	Log("debug") << "[WebSocket: " << this << "] Connecting to " << _uri.toString() << endl;	

	// Will throw on error
	TCPClientSocket::connect(Address(_uri.getHost(), _uri.getPort()));
}


void WebSocket::close()
{
	Log("debug") << "[WebSocket] Closing" << endl;
	_headerState = 0;
	
	if (stateEquals(ClientState::Connected)) {
		SocketStream ss(*this);
		ss << 0xff;
		ss << 0x00;
		ss.flush();
	}

	TCPClientSocket::close();
}


void WebSocket::onConnect()
{
	Log("debug") << "[WebSocket] Connected" << endl;
	TCPClientSocket::onConnect();
	sendHandshake();
}


void WebSocket::sendHandshake()
{
	Log("debug") << "[WebSocket] Sending Handshake" << endl;

	SocketStream ss(*this);
	
#define WS_KEY_ONE		"18x 6]8vM;54 *(5:  {   U1]8  z [  8"
#define WS_KEY_TWO		"1_ tx7X d  <  nw  334J702) 7]o}` 0"
#define WS_KEY_THREE	" Tm[K T2u"

	ss << "GET " << _uri.getPathAndQuery() << " HTTP/1.1" << "\r\n";
	ss << "Upgrade: WebSocket" << "\r\n";
	ss << "Connection: Upgrade" << "\r\n";
	ss << "Host: " << _uri.getUserInfo() << "\r\n";
	ss << "Origin: " << _uri.toString() << "\r\n";
	ss << "Sec-WebSocket-Key1:" << WS_KEY_ONE << "\r\n";
	ss << "Sec-WebSocket-Key2:" << WS_KEY_TWO << "\r\n";
	if (!_protocol.empty()) {
		ss << "Sec-WebSocket-Protocol: " << _protocol << "\r\n";
	}
	if (!_cookie.empty()) {
		ss << "Cookie: " << _cookie << "\r\n";
	}
	ss << "\r\n";
	ss << WS_KEY_THREE;
	ss.flush();

	TCPClientSocket::onHandshake();
}

	
void WebSocket::setProtocol(const std::string& proto)
{
	_protocol = proto;
}


void WebSocket::setCookie(const std::string& cookie)
{
	_cookie = cookie;
}


int WebSocket::send(const char* data, int size)
{
	//Log("trace") << "[WebSocket] Send: " << (string(data, size)) << endl;	
	SocketStream ss(*this);
	ss.put(0x00);
	ss.write(data, size);
	ss.put(0xff);
	ss.flush();
	return size;
}

int WebSocket::send(const IPacket& packet)
{
	Buffer buf;
	packet.write(buf);
	return send(buf.data(), buf.size());
}


void WebSocket::recv(Buffer& buffer) 
{
	Log("trace") << "[WebSocket: " << this << "] On Data: " << buffer.size() << endl;
	
	if (buffer.size() == 0 || (
		buffer.size() == 2 && buffer.data()[0] == 0xff && buffer.data()[1] == 0x00)) {
		Log("debug") << "[WebSocket] Recv Close" << endl;
		close();
		return;
	}
	
	// Read Messages
	else if (_headerState == 2) {
		Log("trace") << "[WebSocket: " << this << "] Parsing Messages: " << buffer.size() << endl;
		
		while (!buffer.eof()) {
			string message;
			UInt8 start;
			buffer.readUInt8(start);

			if (start != 0x00) {
				Log("warn") << "[WebSocket] Message contains bad start code" << endl;
				return;
			}

			if (buffer.readToNext(message, 0xff) == 0) {
				buffer.readString(message, buffer.remaining() - 1);
			}
			
			Log("debug") << "[WebSocket] Parsed Message: " << message << endl;
			Buffer msgBuf(message.data(), message.size());
			packetize(msgBuf);
			buffer++;
		}
		return;
	}

	// Read Initial HTTP Header
	else if (_headerState == 0) { //stateEquals(ClientState::Connected)

		string request(buffer.data(), buffer.size());
		Log("debug") << "[WebSocket: " << this << "] Parsing Header: " << request << endl;			
		if (strncmp(request.data(), "HTTP/1.1 101 ", 13) == 0) {
			Log("debug") << "[WebSocket] Received HTTP Response" << endl;	
			size_t pos = pos = request.find("\r\n\r\n");
			if (pos != string::npos) {
				buffer.setPosition(pos + 4);
				_headerState = 1;
			}
			else {
				setError("Invalid response header");
				return;
			}
		}
	}

	// Read the Digest
	// NOTE: Occasionally the digest is received after the header
	// so we need to handle this here.
	if (_headerState == 1 && buffer.remaining() >= 16) {
		string replyDigest;
		buffer.readString(replyDigest, 16);
		Log("debug") << "[WebSocket] Reply Digest: " << replyDigest << endl;
		_headerState = 2;

		// TODO: Validate Digest

		onOnline();
	}
}




/*
void WebSocket::onConnect() 
{
	//setState(this, ClientState::Handshaking);
	//	TCPSocket::onConnect();
	//TCPClientSocket::onConnected(status);
}
*/
/*
void WebSocket::onError(SocketEvent& event) 
{
	Log("warn") << "[TCPClientSocket: " << this << "] On Error" << endl;
	//nf->release();
	setState(this, ClientState::Disconnected, "Connection error");
}


bool WebSocket::setState(this, unsigned int id, const std::string& message) 
{ 	
	Log("trace") << "[WebSocket] setState: " << id << endl;
	if (stateEquals(id))
		return false;

	switch(id) {
	case ClientState::None:			
		break;
	case ClientState::Connecting:	
		break;
	case ClientState::Connected:
		break;
	case ClientState::Disconnected:	
		break;
	}

	return StatefulSignal<ClientState>::setState(this, id, message); //TCPClientSocket
}
*/


} } // namespace Sourcey::Net
