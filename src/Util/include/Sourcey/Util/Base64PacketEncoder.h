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


#ifndef SOURCEY_HTTP_Base64PacketEncoder_H
#define SOURCEY_HTTP_Base64PacketEncoder_H


#include "Sourcey/PacketStream.h"
#include "Sourcey/Signal.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


namespace scy { 


class Base64PacketEncoder: public PacketProcessor
{
public:
	Base64PacketEncoder() :
		PacketProcessor(Emitter)
	{
	}

	virtual void process(IPacket& packet)
	{		
		// TODO: Use own non stream based Base64 encoder
		std::ostringstream ostr;
		Poco::Base64Encoder encoder(ostr);
		const char* data = NULL;
		size_t size = 0;

		// If the packet is a RawPacket we can access
		// the data pointer directly.
		// TODO: Reference cast and catch bad_cast to copy data
		RawPacket* p = dynamic_cast<RawPacket*>(&packet);
		if (p)
			encoder.write((const char*)p->data(), p->size());
		
		// Otherwise we need to copy the packet data.
		else {
			size_t contentLength = packet.size();
			Buffer buf(contentLength > 0 ? contentLength : 1500);
			packet.write(buf);
			encoder.write((const char*)buf.data(), buf.available());
		}

		encoder.close();

		emit(ostr.str());
	}

	PacketSignal Emitter;
};


} // namespace scy


#endif