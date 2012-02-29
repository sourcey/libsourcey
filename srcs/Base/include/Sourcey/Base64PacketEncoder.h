//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_HTTP_Base64PacketEncoder_H
#define SOURCEY_HTTP_Base64PacketEncoder_H


#include "Sourcey/IPacketProcessor.h"
#include "Sourcey/Signal.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


namespace Sourcey { 


class Base64PacketEncoder: public IPacketProcessor
{
public:
	Base64PacketEncoder()
	{
	}

	virtual void process(IPacket& packet)
	{		
		std::ostringstream ostr;
		Poco::Base64Encoder encoder(ostr);
		const char* data = NULL;
		size_t size = 0;

		// If the packet is a DataPacket we can access
		// the data pointer directly.
		DataPacket* p = dynamic_cast<DataPacket*>(&packet);
		if (p)
			encoder.write((const char*)p->data(), p->size());
		
		// Otherwise we need to copy the packet data.
		else {
			int contentLength = packet.size();
			Buffer buf(contentLength > 0 ? contentLength : 1500);
			packet.write(buf);
			encoder.write((const char*)buf.data(), buf.size());
		}

		encoder.close();		
		std::string base64(ostr.str());
		DataPacket opacket((unsigned char*)base64.data(), base64.size());
		dispatch(this, opacket);
	}
};


} // namespace Sourcey


#endif