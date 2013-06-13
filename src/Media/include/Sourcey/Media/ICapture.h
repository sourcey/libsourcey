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


#include "Sourcey/PacketEmitter.h"
#include "Sourcey/IStartable.h"

#include <list>


namespace Scy {
namespace Media {


class ICapture: public PacketEmitter, public IStartable
{
public:
	ICapture() {};
	virtual ~ICapture() {};

	virtual void start() = 0;
	virtual void stop() = 0;
};


typedef std::list<ICapture*> ICaptureList;


} // namespace Media 
} // namespace Scy


#endif // SOURCEY_MEDIA_ICapture_H