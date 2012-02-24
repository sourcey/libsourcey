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


#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H


#include "Sourcey/Base.h"
#include "Sourcey/Signal.h"

#include <iostream>
#include <string>
#include <vector>


// TODO: Solve conflicts while including the Windows SDK
// when compiling with MinGW/Msys.
#ifdef __MINGW32__
#define NO_DEVICE_MANAGER
#endif

#define ARRAY_SIZE(x) (static_cast<int>((sizeof(x)/sizeof(x[0]))))


namespace Sourcey {
namespace Media {


struct Device 
	/// Represents a system audio, video or render device.
{
	Device() : id(-1) {}
	Device(const std::string& type, const std::string& name, int id, const std::string& guid = "") : 
		type(type), name(name), id(id), guid(guid) {}

	void toXML(std::ostream& ost)
	{
		ost << "<device"
			<< " type='" << type << "'"
			<< " name='" << name << "'"
			<< " guid='" << guid << "'"
			<< " id='" << id << "' />";
	};
	
	int id;
	std::string type;
	std::string name;
	std::string guid;
};


class DeviceManager;


class DeviceWatcher 
	/// The DeviceWatcher monitors device availability for the 
	// DeviceManager. Not implemented.
{
public:
	explicit DeviceWatcher(DeviceManager* manager) {}
	bool start() { return true; }
	void stop() {}
};


class DeviceManager 
	/// DeviceManager manages the audio and video devices on the system.
	/// Methods are virtual to allow for easy stubbing/mocking in tests.
{
public:
	DeviceManager();
	virtual ~DeviceManager();

	virtual bool initialize();
	virtual void uninitialize();
	bool initialized() const { return _isInitialized; }

	void toXML(std::ostream& ost);
	void print(std::ostream& ost);

	virtual bool getAudioInputDevices(std::vector<Device>& devices);
	virtual bool getAudioInputDevice(const std::string& name, int id, Device& out);
	virtual bool getAudioInputDevice(int id, Device& out);
	virtual bool getDefaultAudioInputDevice(Device& device);

	virtual bool getAudioOutputDevices(std::vector<Device>& devices);
	virtual bool getAudioOutputDevice(const std::string& name, int id, Device& out);	
	virtual bool getAudioOutputDevice(int id, Device& out);	

	virtual bool getVideoInputDevices(std::vector<Device>& devs);
	virtual bool getVideoInputDevice(int id, Device& device);
	virtual bool getVideoInputDevice(const std::string& name, int id, Device& device);
	virtual bool getDefaultVideoInputDevice(Device& device);

	Signal<int> DevicesChanged;

protected:
	virtual bool getAudioDevice(bool isInput, const std::string& name, int id, Device& out);

private:
	bool getAudioDevicesByPlatform(bool input, std::vector<Device>& devs);

	bool _isInitialized;
	DeviceWatcher* _watcher;
};


} } // namespace Sourcey::Media


#endif  // DEVICE_MANAGER_H


/*
 * libjingle
 * Copyright 2004--2006, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this vector of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this vector of conditions and the following disclaimer in the documentation
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

