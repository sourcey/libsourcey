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


#ifndef SCY_Net_SocketAdapter_H
#define SCY_Net_SocketAdapter_H


#include "scy/base.h"
#include "scy/memory.h"
#include "scy/signal.h"
#include "scy/packetstream.h"
#include "scy/net/types.h"
#include "scy/net/address.h"
#include "scy/net/network.h"


namespace scy {
namespace net {


class SocketAdapter
	/// SocketAdapter is the abstract interface for all socket classes.
	/// A SocketAdapter can also be attached to a Socket in order to 
	/// override default Socket callbacks and behaviour, while still
	/// maintaining the default Socket interface (see Socket::setAdapter).
	/// 
	/// This class also be extended to implement custom processing 
	/// for received socket data before it is dispatched to the application
	/// (see PacketSocketAdapter and Transaction classes).
{
public:
	SocketAdapter(SocketAdapter* sender = nullptr, SocketAdapter* receiver = nullptr);
		// Creates the SocketAdapter.
	
	virtual ~SocketAdapter();
		// Destroys the SocketAdapter.
			
	virtual int send(const char* data, std::size_t len, int flags = 0);
	virtual int send(const char* data, std::size_t len, const Address& peerAddress, int flags = 0); 
		// Sends the given data buffer to the connected peer.
		// Returns the number of bytes sent or -1 on error.
		// No exception will be thrown.
		// For TCP sockets the given peer address must match the
		// connected peer address.

	virtual int sendPacket(const IPacket& packet, int flags = 0);
	virtual int sendPacket(const IPacket& packet, const Address& peerAddress, int flags = 0);
		// Sends the given packet to the connected peer.
		// Returns the number of bytes sent or -1 on error.
		// No exception will be thrown.
		// For TCP sockets the given peer address must match the
		// connected peer address.

	virtual void sendPacket(IPacket& packet);
		// Sends the given packet to the connected peer.
		// This method provides delegate compatability, and unlike
		// other send methods throws an exception if the underlying 
		// socket is closed.

	virtual void onSocketConnect();
	virtual void onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress);
	virtual void onSocketError(const Error& error);
	virtual void onSocketClose();
		// These virtual methods can be overridden as necessary
		// to intercept socket events before they hit the application.

	void setSender(SocketAdapter* adapter, bool freeExisting = false);
		// A pointer to the adapter for handling outgoing data.
		// Send methods proxy data to this adapter by default. 
		// Note that we only keep a simple pointer so
		// as to avoid circular references preventing destruction.	

	SocketAdapter* sender();
		// Returns the output SocketAdapter pointer
	
	void addReceiver(SocketAdapter* adapter, int priority = 0);
		// Adds an input SocketAdapter for receiving socket callbacks.

	void removeReceiver(SocketAdapter* adapter);
		// Removes an input SocketAdapter.

	void* opaque;
		// Optional client data pointer.
		//
		// The pointer is not initialized or managed
		// by the socket base.
		
	NullSignal Connect;
		// Signals that the socket is connected.

	Signal2<const MutableBuffer&, const Address&> Recv; //SocketPacket&
		// Signals when data is received by the socket.

	Signal<const scy::Error&> Error;
		// Signals that the socket is closed in error.
		// This signal will be sent just before the 
		// Closed signal.

	NullSignal Close;
		// Signals that the underlying socket is closed,
		// maybe in error.
	
protected:
	virtual void* self() { return this; };
		// Returns the polymorphic instance pointer 
		// for signal delegate callbacks.
	
	SocketAdapter* _sender;
};


} } // namespace scy::net


#endif // SCY_Net_SocketAdapter_H