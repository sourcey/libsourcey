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


#ifndef SOURCEY_NET_WebSocket_H
#define SOURCEY_NET_WebSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/StatefulSocket.h"

#include "Poco/URI.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SecureStreamSocket.h"


namespace scy {
namespace Net {
	

class IWebSocket: public ISocket
{
public:
	virtual ~IWebSocket() {};
	
	virtual void connect(const std::string& uri) = 0;
	virtual void connect() = 0;
	virtual void close() = 0;
	
	virtual void setProtocol(const std::string& proto) = 0;
	virtual void setCookie(const std::string& cookie) = 0;

	virtual UInt16 port() = 0;

	NullSignal Online;
		/// Signals that the socket is validated
		/// and ready to send data.
};
	

// ---------------------------------------------------------------------
//
class IPacketWebSocket: public IPacketSocket
{
public:
	virtual ~IPacketWebSocket() {};
	
	virtual void connect(const std::string& uri) = 0;
	virtual void connect() = 0;
	virtual void close() = 0;
	
	virtual void setProtocol(const std::string& proto) = 0;
	virtual void setCookie(const std::string& cookie) = 0;

	virtual UInt16 port() = 0;

	NullSignal Online;
		/// Signals that the socket is validated
		/// and ready to send data.
};


// ---------------------------------------------------------------------
//
template <class SocketBaseT>
class WebSocketBase: public SocketBaseT
{
public:
	WebSocketBase(Reactor& reactor) :
		SocketBaseT(reactor)
	{  
	}

	WebSocketBase(Reactor& reactor, const std::string& uri) :
		SocketBaseT(reactor),
		_headerState(0),
		_uri(uri)
	{
	}

	virtual ~WebSocketBase()
	{
	}

	virtual void connect(const std::string& uri)
	{
		_uri = uri;
		connect();
	}

	virtual void connect()
	{
		_headerState = 0;
	
		log("debug") << "Web Socket Connecting to " << _uri.toString() << std::endl;	

		// Will throw on error
		SocketBaseT::connect(Address(_uri.getHost(), port()));
	}

	virtual void close()
	{
		log("debug") << "WebSocket Closing" << std::endl;
		_headerState = 0;
	
		try {
			if (isConnected()) {
				Poco::Net::SocketStream ss(*this);
				ss << 0xff;
				ss << 0x00;
				ss.flush();
			}
		}
		catch (Exception& exc) {
			// Not a fatal error
			log("warn") << "WebSocket Closing Error: " << exc.displayText() << std::endl;
		}

		SocketBaseT::close();
	}

	void sendHandshake()
	{
		log("debug") << "Sending Handshake" << std::endl;

		Poco::Net::SocketStream ss(*this);
	
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
		
		//onHandshake();
	}

	virtual int send(const char* data, int size)
	{
		//log("trace") << "Send: " << (std::string(data, size)) << std::endl;	
		Poco::Net::SocketStream ss(*this);
		ss.put(static_cast<unsigned char>(0x00));
		ss.write(data, size);
		ss.put(static_cast<unsigned char>(0xff));
		ss.flush();
		return size;
	}

	virtual int send(const IPacket& packet)
	{
		Buffer buf;
		packet.write(buf);
		return send(buf.data(), buf.size());
	}

	virtual void recv(Buffer& buffer) 
	{
		log("trace") << "On Data: " << buffer.size() << std::endl;
	
		if (buffer.size() == 0 || (
			buffer.size() == 2 && buffer.data()[0] == 0xff && buffer.data()[1] == 0x00)) {
			log("debug") << "Recv Close" << std::endl;
			close();
			return;
		}
	
		// Read Messages
		else if (_headerState == 2) {
			log("trace") << "Parsing Messages: " << buffer.size() << std::endl;
		
			while (!buffer.eof()) {
				std::string message;
				UInt8 start;
				buffer.readUInt8(start);

				if (start != 0x00) {
					log("warn") << "Message contains bad start code" << std::endl;
					return;
				}

				if (buffer.readToNext(message, static_cast<unsigned char>(0xff)) == 0) {
					buffer.readString(message, buffer.remaining() - 1);
				}
			
				log("trace") << "Parsed Message: " << message << std::endl;
				Buffer msgBuf(message.data(), message.size());
				packetize(msgBuf);
				buffer++;
			}
			return;
		}

		// Read Initial HTTP Header
		else if (_headerState == 0) { //stateEquals(ClientState::Connected)

			std::string request(buffer.data(), buffer.size());
			log("debug") << "Parsing Header: " << request << std::endl;			
			if (strncmp(request.data(), "HTTP/1.1 101 ", 13) == 0) {
				log("debug") << "Received HTTP Response" << std::endl;	
				size_t pos = pos = request.find("\r\n\r\n");
				if (pos != std::string::npos) {
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
			std::string replyDigest;
			buffer.readString(replyDigest, 16);
			log("debug") << "Reply Digest: " << replyDigest << std::endl;
			_headerState = 2;

			// TODO: Validate Digest

			onOnline();
		}
	}
	
	virtual void setProtocol(const std::string& proto)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_protocol = proto;
	}
	
	virtual void setCookie(const std::string& cookie)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_cookie = cookie;
	}
		
	virtual UInt16 port()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _uri.getPort() ? _uri.getPort() : (_uri.getScheme() == "wss" ? 443 : 80);
	}

	virtual const char* className() const { return "WebSocketBase"; }
	

protected:	
	virtual void onConnect()
	{
		log("debug") << "Web Socket Connected" << std::endl;
		SocketBaseT::onConnect();
		sendHandshake();
	}

	virtual void onOnline()
	{
		log("debug") << "Web Socket Online" << std::endl;

		Online.emit(static_cast<SocketBaseT::InterfaceT*>(this));
	}


protected:	
	mutable Poco::FastMutex _mutex;

	Poco::URI _uri;
	std::string _protocol;
	std::string _cookie;
	int _headerState;
};


typedef WebSocketBase< Net::SocketBase< ::Poco::Net::StreamSocket, TCP, Net::IWebSocket > >  WebSocket;
typedef WebSocketBase< Net::SocketBase< ::Poco::Net::SecureStreamSocket, SSLTCP, Net::IWebSocket > >  SSLWebSocket;
typedef WebSocketBase< Net::PacketSocketBase< ::Poco::Net::StreamSocket, TCP, Net::IPacketWebSocket > >  PacketWebSocket;
typedef WebSocketBase< Net::PacketSocketBase< ::Poco::Net::SecureStreamSocket, SSLTCP, Net::IPacketWebSocket > >  SSLPacketWebSocket;


} } // namespace scy::Net


#endif //  SOURCEY_NET_WebSocket_H





/*
class WebSocket: public SocketT
{
public:
	WebSocket(Reactor& reactor);
	WebSocket(Reactor& reactor, const Poco::URI& uri);
	virtual ~WebSocket();
	
	void connect(const Poco::URI& uri);
	void connect();
	void close();
	
	virtual int send(const char* data, int size);
	virtual int send(const IPacket& packet);
	
	void setProtocol(const std::string& proto);
	void setCookie(const std::string& cookie);

protected:
	virtual void sendHandshake();
	virtual void onConnect();

	virtual void recv(Buffer& buffer);

	//virtual void onReadable(SocketEvent& event);
	//virtual void onError(SocketEvent& event);
	
	Poco::URI _uri;
	std::string _protocol;
	std::string _cookie;
	int _headerState;
};
*/