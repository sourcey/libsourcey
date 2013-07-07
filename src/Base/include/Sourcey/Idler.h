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


#ifndef SOURCEY_UV_Idler_H
#define SOURCEY_UV_Idler_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/Base.h"


namespace scy {
namespace uv {
	

class Idler: public uv::Base<>
	/// https://github.com/AlloSphere-Research-Group/alive/blob/master/archived/av/uv_utils.h
{
public:
	Idler();
	virtual ~Idler();	
	
	virtual void init();	
	virtual void start();	
	virtual void stop();

	void onIdle(int status);

protected:
	uv_idle_t _handle;
};


//
// UV Callbacks
//

UVStatusCallback(Idler, onIdle, uv_idle_t);


} } // namespace scy::uv


#endif // SOURCEY_UV_Idler_H
