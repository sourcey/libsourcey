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


#ifndef SOURCEY_MEDIA_DeviceManager_H
#define SOURCEY_MEDIA_DeviceManager_H

#include <string>
#include <vector>

#include "Sourcey/Base.h"
#include "Sourcey/Signal.h"

//#include "talk/base/scoped_ptr.h"
//#include "talk/base/sigslot.h"
//#include "talk/base/stringencode.h"


namespace Scy {
namespace Media {


struct Device 
	/// Represents a system audio, video or render device.
{
	Device();
	Device(const std::string& type, int id, 
		const std::string& name, const std::string& guid = "", 
		bool isDefault = false);
	
	void print(std::ostream& os);

	int id;
	std::string type; // audioin, audioout, video
	std::string name;
	std::string guid;
	bool isDefault;
};

	
enum MediaCapabilities 
{
	AUDIO_RECV = 1 << 0,
	AUDIO_SEND = 1 << 1,
	VIDEO_RECV = 1 << 2,
	VIDEO_SEND = 1 << 3,
};


class IDeviceManager
	/// A platform independent interface to manage the audio
	/// and video devices on the system.
{
public:
	virtual ~IDeviceManager() { }

	// Initialization
	virtual bool initialize() = 0;
	virtual void uninitialize() = 0;

	// Device enumeration
	virtual bool getAudioInputDevices(std::vector<Device>& devices) = 0;
	virtual bool getAudioOutputDevices(std::vector<Device>& devices) = 0;
	
	virtual bool getAudioInputDevice(Device& out, const std::string& name, int id = -1) = 0;
	virtual bool getAudioInputDevice(Device& out, int id) = 0;
	
	virtual bool getAudioOutputDevice(Device& out, const std::string& name, int id = -1) = 0;
	virtual bool getAudioOutputDevice(Device& out, int id) = 0;

	//virtual bool getAudioInputDevice(const std::string& name, Device& out) = 0;
	//virtual bool getAudioOutputDevice(const std::string& name, Device& out) = 0;

	virtual bool getVideoCaptureDevices(std::vector<Device>& devs) = 0;
	virtual bool getVideoCaptureDevice(Device& out, int id) = 0;
	virtual bool getVideoCaptureDevice(Device& out, const std::string& name, int id = -1) = 0;
	
	virtual bool getDefaultAudioInputDevice(Device& device) = 0;
	virtual bool getDefaultAudioOutputDevice(Device& device) = 0;
	virtual bool getDefaultVideoCaptureDevice(Device& device) = 0;

	// Capabilities
	virtual int getCapabilities() = 0;

	virtual void print(std::ostream& ost) = 0;

	NullSignal SignalDevicesChange;

	static const char kDefaultDeviceName[];
};


class DeviceWatcher 
{
public:
	explicit DeviceWatcher(IDeviceManager* dm) {}
	virtual ~DeviceWatcher() {}
	virtual bool start() { return true; }
	virtual void stop() {}
};


class DeviceManagerFactory 
{
public:
	static IDeviceManager* create();
private:
	DeviceManagerFactory();
};


class DeviceManager: public IDeviceManager 
{
public:
	DeviceManager();
	virtual ~DeviceManager();

	// Initialization
	virtual bool initialize();
	virtual void uninitialize();

	// Capabilities
	virtual int getCapabilities();

	// Device enumeration
	virtual bool getAudioInputDevices(std::vector<Device>& devices);
	virtual bool getAudioOutputDevices(std::vector<Device>& devices);

	//virtual bool getAudioInputDevice(const std::string& name, Device& out);
	//virtual bool getAudioOutputDevice(const std::string& name, Device& out);
	
	virtual bool getAudioInputDevice(Device& out, const std::string& name, int id = -1);
	virtual bool getAudioInputDevice(Device& out, int id);
	
	virtual bool getAudioOutputDevice(Device& out, const std::string& name, int id = -1);
	virtual bool getAudioOutputDevice(Device& out, int id);

	virtual bool getVideoCaptureDevices(std::vector<Device>& devs);
	virtual bool getVideoCaptureDevice(Device& out, const std::string& name, int id = -1);

	virtual bool getVideoCaptureDevice(Device& out, int id);
		// Returns the video capture device at the given system index.
	
	virtual bool getDefaultAudioInputDevice(Device& device);
	virtual bool getDefaultAudioOutputDevice(Device& device);
	virtual bool getDefaultVideoCaptureDevice(Device& device);

	static bool filterDevices(std::vector<Device>& devices, const char* const exclusionList[]);
		// The exclusionList MUST be a NULL terminated list.
	
	static bool matchID(std::vector<Device>& devices, Device& out, int id);
		// Returns a device matching the given ID.

	static bool matchNameAndID(std::vector<Device>& devices, Device& out, const std::string& name, int id = -1);
		// Returns a device matching the given name and ID.
		// If the device name is not available at the given ID then first
		// device of that name will be returned.
		// If the ID should not be matched the given ID should be -1.

	bool initialized() const { return _initialized; }

	void print(std::ostream& ost);

protected:
	virtual bool getAudioDevices(bool input, std::vector<Device>& devs);
	virtual bool getAudioDevice(bool input, Device& out, const std::string& name, int id = -1);
	virtual bool getAudioDevice(bool input, Device& out, int id);

	void setInitialized(bool initialized);

	void setWatcher(DeviceWatcher* watcher);
	DeviceWatcher* watcher();

private:
	// The exclusionList MUST be a NULL terminated list.
	static bool shouldDeviceBeIgnored(const std::string& deviceName, 
		const char* const exclusionList[]);

	bool _initialized;
	DeviceWatcher* _watcher;
};


} } // namespace Scy::Media


#endif  // SOURCEY_MEDIA_DeviceManager_H


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