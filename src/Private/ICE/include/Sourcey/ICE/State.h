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


#ifndef SOURCEY_ICE_TYPES_H
#define SOURCEY_ICE_TYPES_H


#include "Sourcey/Stateful.h"

#include <string>


namespace Sourcey {
namespace ICE {


struct State: public StateT 
{
	enum Type 
	{
		Waiting						= 0x00, 
		GatheringCandidates			= 0x01, 
		SendingSDP					= 0x02, 
		CheckingConnectivity		= 0x04, 
		Success						= 0x08, 
		Failed						= 0x10, 
		Terminating					= 0x20
	};

	std::string str(unsigned int id) const 
	{ 
		switch (id) {
		case Waiting:				return "Waiting";
		case GatheringCandidates:	return "GatheringCandidates";
		case SendingSDP:			return "SendingSDP";
		case CheckingConnectivity:	return "CheckingConnectivity";
		case Success:				return "Success";
		case Failed:				return "Failed";
		case Terminating:			return "Terminating";
		}
		return "undefined"; 
	};
};


} // namespace ICE 
} // namespace Sourcey


#endif // SOURCEY_ICE_TYPES_H
