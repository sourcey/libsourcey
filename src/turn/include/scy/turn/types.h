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


#ifndef SCY_TURN_Types_H
#define SCY_TURN_Types_H


#include "scy/net/socket.h"
#include "scy/net/address.h"
#include "scy/stun/message.h"


namespace scy {
namespace turn {


const int CLIENT_SOCK_BUF_SIZE = 65536;
const int SERVER_SOCK_BUF_SIZE = CLIENT_SOCK_BUF_SIZE * 32;
	

// Returns the native socket file descriptor
#if WIN32
#define nativeSocketFd(handle) ((handle)->socket)
#elif defined(__APPLE__)
int nativeSocketFd__(uv_stream_t* handle);
#define nativeSocketFd(handle) (nativeSocketFd__((uv_stream_t*) (handle)))
#else
#define nativeSocketFd(handle) ((handle)->io_watcher.fd)
#endif


template<class NativeT> int getServerSocketSendBufSize(uv::Handle& handle)
{
	int fd = nativeSocketFd(handle.ptr<NativeT>());
	int optval = 0; 
	socklen_t optlen = sizeof(int); 
	int err = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&optval, &optlen);
	if (err < 1) {
		errorL("TURNServer") << "Cannot get snd sock size on fd " << fd << std::endl;
	}
	return optval;
}


template<class NativeT> int getServerSocketRecvBufSize(uv::Handle& handle)
{
	int fd = nativeSocketFd(handle.ptr<NativeT>());
	int optval = 0; 
	socklen_t optlen = sizeof(int); 
	int err = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&optval, &optlen);
	if (err < 1) {
		errorL("TURNServer") << "Cannot get rcv sock size on fd " << fd << std::endl;
	}
	return optval;
}


template<class NativeT> int setServerSocketBufSize(uv::Handle& handle, int size)
{
	int fd = nativeSocketFd(handle.ptr<NativeT>());
	int sz;

	sz = size;
	while (sz > 0) {
		if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&sz, (socklen_t)sizeof(sz)) < 0) {
			sz = sz / 2;
		} else break;
	}

	if (sz < 1) {
		errorL("TURNServer") << "Cannot set rcv sock size " << size << " on fd " << fd << std::endl;
	}

	// Get the value to ensure it has propagated through the OS
	traceL("TURNServer") << "Recv sock size " << getServerSocketRecvBufSize<NativeT>(handle) << " on fd " << fd << std::endl;

	sz = size;
	while (sz > 0) {
		if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&sz, (socklen_t)sizeof(sz)) < 0) {
			sz = sz / 2;
		} else break;
	}

	if (sz < 1) {
		errorL("TURNServer") << "Cannot set snd sock size " << size << " on fd " << fd << std::endl;
	}	

	// Get the value to ensure it has propagated through the OS
	traceL("TURNServer") << "Send sock size " << getServerSocketSendBufSize<NativeT>(handle)<< " on fd " << fd << std::endl;

	return sz;
}


inline bool isChannelData(UInt16 messageType)
{
  // The first two bits of a channel data message are 0b01.
  return ((messageType & 0xC000) == 0x4000);
}


enum AuthenticationState 
{
	Authenticating	= 1,
	Authorized		= 2,
	QuotaReached	= 3,
	NotAuthorized	= 4
};


struct Request: public stun::Message
{
	net::Socket socket;
	net::Address localAddress;
	net::Address remoteAddr;

	Request(const net::Socket& socket, 
			const stun::Message& message, 
			const net::Address& localAddress = net::Address(), 
			const net::Address& remoteAddr = net::Address()) :
		stun::Message(message), 
		socket(socket), 
		localAddress(localAddress), 
		remoteAddr(remoteAddr) {}

	//Request(const Request& r) :
	//	stun::Message(r), 
	//	socket(r.socket), 
	//	localAddress(r.localAddress), 
	//	remoteAddr(r.remoteAddr) {}

//private:
	//Request& operator = (const Request&) {}
};


typedef std::vector<std::string> IPList;


} } // namespace scy::turn


#endif // SCY_TURN_Types_H
