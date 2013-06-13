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


#ifndef SOURCEY_ICE_HOST_CANDIDATE_H
#define SOURCEY_ICE_HOST_CANDIDATE_H


#include "Sourcey/ICE/LocalCandidate.h"


namespace Sourcey {
namespace ICE { 


class HostCandidate: public LocalCandidate 
{
public:
	HostCandidate(Component& component);		
	
	void resolve();
	void makeRedundant();
	void terminate();

	int send(const char* data, int size, const Net::Address& peerAddress);
	int send(const IPacket& packet, const Net::Address& peerAddress);
	//bool send(STUN::Transaction* transaction);
	//void sendError(const STUN::Message& request, const Net::Address& peerAddress, int errorCode, const char* errorDesc);

	Net::ISocket& socket() const { return *_socket; };
	
	virtual const char* className() const { return "HostCandidate"; }

protected:	
	~HostCandidate();

	void onSTUNPacketReceived(void* sender, STUN::Message& packet
		//Net::ISocket& socket, const STUN::Message& message, const Net::Address& localAddress, const Net::Address& peerAddress
		);
	void onDataPacketReceived(void* sender, DataPacket& packet
		//Net::ISocket& socket, const DataPacket& packet, const Net::Address& localAddress, const Net::Address& peerAddress
		);

private:	
	Net::ISocket* _socket;
};


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_HOST_CANDIDATE_H