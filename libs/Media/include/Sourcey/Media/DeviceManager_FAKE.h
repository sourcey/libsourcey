//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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
// Implemented from libjingle r116 Feb 16, 2012


#ifndef SOURCEY_MEDIA_DeviceManager_FAKE_H
#define SOURCEY_MEDIA_DeviceManager_FAKE_H


#include <string>
#include <vector>

#include "Sourcey/Media/DeviceManager.h"
//#include "talk/session/phone/devicemanager.h"
//#include "talk/session/phone/mediacommon.h"


namespace Sourcey {
namespace Media {


class FakeDeviceManager: public IDeviceManager 
{
public:
	FakeDeviceManager() {}

	virtual bool initialize() 
	{
		return true;
	}

	virtual void uninitialize() {}

	virtual int getCapabilities() 
	{
		std::vector<Device> devices;
		int caps = VIDEO_RECV;
		if (!input_devices_.empty()) {
			caps |= AUDIO_SEND;
		}
		if (!output_devices_.empty()) {
			caps |= AUDIO_RECV;
		}
		if (!vidcap_devices_.empty()) {
			caps |= VIDEO_SEND;
		}
		return caps;
	}

	virtual bool getAudioInputDevices(std::vector<Device>& devs) {
		devs = input_devices_;
		return true;
	}

	virtual bool getAudioOutputDevices(std::vector<Device>& devs) {
		devs = output_devices_;
		return true;
	}

	virtual bool getAudioInputDevice(const std::string& name, Device& out) 
	{
		return getAudioDevice(true, name, out);
	}

	virtual bool getAudioOutputDevice(const std::string& name, Device& out) 
	{
		return getAudioDevice(false, name, out);
	}

	virtual bool getVideoCaptureDevices(std::vector<Device>& devs) 
	{
		devs = vidcap_devices_;
		return true;
	}

	virtual bool getDefaultVideoCaptureDevice(Device& device) 
	{
		if (vidcap_devices_.empty()) {
			return false;
		}
		device = vidcap_devices_[0];
		return true;
	}

#ifdef OSX
	bool QtKitToSgDevice(const std::string& qtkit_name, Device& out)
	{
		out->name = qtkit_name;
		out->id = "sg:" + qtkit_name;
		return true;
	}
#endif

	void setAudioInputDevices(const std::vector<Device>& devices) 
	{
		//input_devices_.clear();
		//for (size_t i = 0; i < devices.size(); ++i) {
		//	input_devices_.push_back(Device(devices[i], i));
		//}
		input_devices_ = devices;
		//SignalDevicesChange();
	}

	void setAudioOutputDevices(const std::vector<Device>& devices) 
	{
		//output_devices_.clear();
		//for (size_t i = 0; i < devices.size(); ++i) {
		//	output_devices_.push_back(Device(devices[i], i));
		//}		
		output_devices_ = devices;
		//SignalDevicesChange();
	}

	void setVideoCaptureDevices(const std::vector<Device>& devices) 
	{
		//vidcap_devices_.clear();
		//for (size_t i = 0; i < devices.size(); ++i) {
		//	vidcap_devices_.push_back(Device(devices[i], i));
		//}		
		vidcap_devices_ = devices;
		//SignalDevicesChange();
	}


	virtual bool getVideoCaptureDevice(const std::string& name, Device& out) 
	{
			if (vidcap_devices_.empty())
				return false;

			// If the name is empty, return the default device.
			if (name.empty() || name == kDefaultDeviceName) {
				out = vidcap_devices_[0];
				return true;
			}

			return findDeviceByName(vidcap_devices_, name, out);
	}

	bool getAudioDevice(bool input, const std::string& name, Device& out) 
	{
			// If the name is empty, return the default device.
			//if (name.empty() || name == kDefaultDeviceName) {
			//	out = Device(name, -1);
			//	return true;
			//}

			return findDeviceByName((input ? input_devices_ : output_devices_),
				name, out);
	}

	static bool findDeviceByName(const std::vector<Device>& devices, const std::string& name, Device& out) 
	{
			for (std::vector<Device>::const_iterator it = devices.begin();
				it != devices.end(); ++it) {
					if (name == it->name) {
						out = *it;
						return true;
					}
			}
			return false;
	}
private:
	std::vector<Device> input_devices_;
	std::vector<Device> output_devices_;
	std::vector<Device> vidcap_devices_;
};


} } // namespace Sourcey::Media


#endif  // SOURCEY_MEDIA_DeviceManager_FAKE_H


/*
 * libjingle
 * Copyright 2008--2011, Google Inc.
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