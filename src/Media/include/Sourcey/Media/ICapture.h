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


#ifndef SOURCEY_MEDIA_ICapture_H
#define SOURCEY_MEDIA_ICapture_H


#include "Sourcey/PacketStream.h"

#include <list>


namespace scy {
namespace av {
	struct Format;


class ICapture: public PacketSource, public basic::Startable
{
public:
	ICapture() : PacketSource(this->emitter) {};
	virtual ~ICapture() {};

	virtual void start() = 0;
	virtual void stop() = 0;
	
	virtual void getEncoderFormat(Format& iformat) = 0;
		// Sets the input format for encoding with this capture device.
			
	PacketSignal emitter;
};


} // namespace av 
} // namespace scy


#endif // SOURCEY_MEDIA_ICapture_H