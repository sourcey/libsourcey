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


#ifndef SOURCEY_STUN_Transaction_H
#define SOURCEY_STUN_Transaction_H


#include "Sourcey/Net/Transaction.h"
#include "Sourcey/STUN/Message.h"


namespace Scy {
namespace STUN {


struct Transaction: public Net::Transaction<Message>
{
	Transaction(Runner& runner, 
				Net::IPacketSocket* socket, 
				const Net::Address& localAddress, 
				const Net::Address& peerAddress, 
				long timeout = 10000, 
				int retries = 1);
	
	virtual bool match(const Message& message);
	virtual void onResponse();

protected:
	virtual ~Transaction();
};


} } // namespace Scy::STUN


#endif // SOURCEY_STUN_Transaction_H