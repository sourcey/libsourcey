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


#ifndef SOURCEY_NET_StatefulSocketBase_H
#define SOURCEY_NET_StatefulSocketBase_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/PacketSocketBase.h"

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SecureStreamSocket.h"


namespace scy {
namespace Net {


struct SocketState: public State 
{
	enum Type 
	{
		None				= 0x00,
		Connecting			= 0x01,
		Connected			= 0x04,
		//Handshaking			= 0x08,
		//Online				= 0x10,
		Disconnected		= 0x20
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Connecting:	return "Connecting";
		case Connected:		return "Connected";
		//case Handshaking:	return "Handshaking";
		//case Online:		return "Online";
		case Disconnected:	return "Disconnected";
		}
		return "undefined"; 
	};
};


template <class SocketBaseT>
class StatefulSocketBase: public SocketBaseT, public StatefulSignal<SocketState>
{
public:
	StatefulSocketBase(Reactor& reactor) : 
		SocketBaseT(reactor)
	{
	}


	StatefulSocketBase(const StatefulSocketBase& r) : 
		SocketBaseT(r)
	{
	}
	
	
	virtual ~StatefulSocketBase()
	{
	}


	virtual void connect(const Address& peerAddress, int timeout = 0) 
	{
		setState(static_cast<SocketBaseT::InterfaceT*>(this), SocketState::Connecting);
		SocketBaseT::connect(peerAddress, timeout);
	}


	virtual bool isActive()
	{
		return stateEquals(SocketState::Connecting)
			//|| stateEquals(SocketState::Handshaking)
			|| stateEquals(SocketState::Connected)
			//|| stateEquals(SocketState::Online)
			;
	}


	/*
	virtual bool isOnline()
	{
		return stateEquals(SocketState::Online);
	}
	*/
	

	virtual const char* className() const { return "StatefulSocketBase"; }

	
protected:
	virtual void onConnect()
	{
		assert(stateEquals(SocketState::Connecting));
		SocketBaseT::onConnect(); // must call base before setting state
		setState(static_cast<SocketBaseT::InterfaceT*>(this), SocketState::Connected);
	}


	/*
	virtual void onHandshake()
	{
		assert(stateEquals(SocketState::Connected));
		setState(static_cast<SocketBaseT::InterfaceT*>(this), SocketState::Handshaking);
	}


	virtual void onOnline()
	{
		assert(stateEquals(SocketState::Handshaking)
			|| stateEquals(SocketState::Connected));
		setState(static_cast<SocketBaseT::InterfaceT*>(this), SocketState::Online);
	}
	*/


	virtual void onClose()
	{
		LogTrace() << "[StatefulSocketBase:" << this << "] On Close" << std::endl;
		setState(static_cast<SocketBaseT::InterfaceT*>(this), SocketState::Disconnected, _error);
		SocketBaseT::onClose(); // may result in destructon
	}
};


typedef StatefulSocketBase< SocketBase<Poco::Net::StreamSocket, TCP> > TCPStatefulSocket;
typedef StatefulSocketBase< SocketBase<Poco::Net::SecureStreamSocket, SSLTCP> > SSLStatefulSocket;
typedef StatefulSocketBase< PacketSocketBase<Poco::Net::StreamSocket, TCP> > TCPStatefulPacketSocket;
typedef StatefulSocketBase< PacketSocketBase<Poco::Net::SecureStreamSocket, SSLTCP> > SSLStatefulPacketSocket;


} } // namespace scy::Net


#endif // SOURCEY_NET_StatefulSocketBase_H





	//StatefulSocketBase(Reactor& reactor/* = Reactor::getDefault()*/);
	//StatefulSocketBase(const StatefulSocketBase& r);
	//virtual ~StatefulSocketBase();
	//
	//virtual void connect(const Address& peerAddress, int timeout = 0);

	////virtual void send(IPacket& packet);

	//bool isActive();
	//bool isOnline();

//protected:
	//virtual void onConnect();
	//virtual void onHandshake();		// never called in default impl
	//virtual void onOnline();		// never called in default impl
	//virtual void onClose();