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
// Implemented from libjingle r116 Feb 16, 2012


#ifndef SOURCEY_MEDIA_DeviceManager_WIN32_H
#define SOURCEY_MEDIA_DeviceManager_WIN32_H

#ifndef __WINDOWS_DS__
#define __WINDOWS_DS__
#endif


#include <string>
#include <vector>

#include "Sourcey/Media/DeviceManager.h"


namespace Scy {
namespace Media {


class Win32DeviceManager: public DeviceManager 
{
public:
	Win32DeviceManager();
	virtual ~Win32DeviceManager();

	virtual bool initialize();
	virtual void uninitialize();

	virtual bool getVideoCaptureDevices(std::vector<Device>& devs);
	virtual bool getDefaultAudioOutputDevice(Device& device);
	virtual bool getDefaultAudioInputDevice(Device& device);
	virtual bool getDefaultVideoCaptureDevice(Device& device);

private:
	virtual bool getAudioDevices(bool input, std::vector<Device>& devs);
	virtual bool getDefaultAudioDevice(bool input, Device& device);

	bool _needCoUninitialize;
	static Poco::FastMutex _mutex;
};


/*
class Win32DeviceWatcher : public DeviceWatcher, public talk_base::Win32Window 
{
public:
	explicit Win32DeviceWatcher(Win32DeviceManager* dm);
	virtual ~Win32DeviceWatcher();
	virtual bool start();
	virtual void stop();

private:
	HDEVNOTIFY Register(REFGUID guid);
	void Unregister(HDEVNOTIFY notify);
	virtual bool OnMessage(UINT msg, WPARAM wp, LPARAM lp, LRESULT& result);

	Win32DeviceManager* manager_;
	HDEVNOTIFY audio_notify_;
	HDEVNOTIFY video_notify_;
};
*/


} } // namespace Scy::Media


#endif  // SOURCEY_MEDIA_DeviceManager_WIN32_H


/*
 * libjingle
 * Copyright 2004 Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */