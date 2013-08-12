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


#include "Sourcey/Media/DeviceManager.h"

/*
#include "talk/base/fileutils.h"
#include "talk/base/logging.h"
#include "talk/base/pathutils.h"
#include "talk/base/stringutils.h"
#include "talk/base/thread.h"
#include "talk/session/phone/filevideocapturer.h"
#include "talk/session/phone/mediacommon.h"
*/

using namespace std;


namespace scy {
namespace av {


// Initialize to empty string
const char IDeviceManager::kDefaultDeviceName[] = "";


// ----------------------------------------------------------------------------
//
Device::Device() : 
	id(-1) 
{
}


Device::Device(const string& type, int id, const string& name, const string& guid, bool isDefault) : 
	type(type), id(id), name(name), guid(guid), isDefault(isDefault)
{
}

	
void Device::print(ostream& os) 
{		
	os << "Device[" 
		<< type << ": " 
		<< id << ": " 
		<< name << ": " 
		<< isDefault << "]" << endl;
}


// ----------------------------------------------------------------------------
//
DeviceManager::DeviceManager() : 
	_watcher(nil), 
	_initialized(false) 
{
}


DeviceManager::~DeviceManager()
{
	if (initialized())
		uninitialize();
}


bool DeviceManager::initialize() 
{
	if (!initialized()) {
		if (watcher() && !watcher()->start())
			return false;
		setInitialized(true);
	}
	return true;
}


void DeviceManager::uninitialize() 
{
	if (initialized()) {
		if (watcher())
			watcher()->stop();
		setInitialized(false);
	}
}


int DeviceManager::getCapabilities() 
{
	std::vector<Device> devices;
	int caps = VIDEO_RECV;
	if (getAudioInputDevices(devices) && !devices.empty()) {
		caps |= AUDIO_SEND;
	}
	if (getAudioOutputDevices(devices) && !devices.empty()) {
		caps |= AUDIO_RECV;
	}
	if (getVideoCaptureDevices(devices) && !devices.empty()) {
		caps |= VIDEO_SEND;
	}
	return caps;
}


bool DeviceManager::getAudioInputDevices(vector<Device>& devices) 
{
	return getAudioDevices(true, devices);
}


bool DeviceManager::getAudioOutputDevices(vector<Device>& devices) 
{
	return getAudioDevices(false, devices);
}


bool DeviceManager::getAudioInputDevice(Device& out, const string& name, int id) 
{
	return getAudioDevice(true, out, name, id);
}


bool DeviceManager::getAudioInputDevice(Device& out, int id) 
{
	return getAudioDevice(true, out, id);
}


bool DeviceManager::getAudioOutputDevice(Device& out, const string& name, int id) 
{
	return getAudioDevice(false, out, name, id);
}


bool DeviceManager::getAudioOutputDevice(Device& out, int id) 
{
	return getAudioDevice(false, out, id);
}


bool DeviceManager::getVideoCaptureDevices(vector<Device>& devices) 
{
	devices.clear();
#if defined(ANDROID) || defined(IOS)
	// TODO: Incomplete. Use ANDROID implementation for IOS
	// to quiet compiler.
	// On Android, we treat the camera(s) as a single device. Even if there are
	// multiple cameras, that's abstracted away at a higher level.
	Device dev("camera", "1");    // name and ID
	devices.push_back(dev);
#else
	return false;
#endif
}


/*
bool DeviceManager::getAudioInputDevice(Device& out, const string& name, int id) 
{
	// If the name is empty, return the default device.
	if (name.empty())
		return getDefaultAudioInputDevice(out);

	std::vector<Device> devices;
	getAudioInputDevices(devices);
	return matchNameAndID(devices, out, name, id);
}


bool DeviceManager::getAudioOutputDevice(Device& out, const string& name, int id) 
{
	// If the name is empty, return the default device.
	if (name.empty())
		return getDefaultAudioOutputDevice(out);

	std::vector<Device> devices;
	return getAudioOutputDevices(devices) && 
		matchNameAndID(devices, name, id);
}
*/


bool DeviceManager::getVideoCaptureDevice(Device& out, int id) 
{
	std::vector<Device> devices;
	return getVideoCaptureDevices(devices) && 
		matchID(devices, out, id);
}


bool DeviceManager::getVideoCaptureDevice(Device& out, const string& name, int id) 
{
	// If the name is empty, return the default device.
	if (name.empty() || name == kDefaultDeviceName) {
		//infoL() << "Creating default VideoCapturer" << endl;
		return getDefaultVideoCaptureDevice(out);
	}

	std::vector<Device> devices;
	return getVideoCaptureDevices(devices) && 
		matchNameAndID(devices, out, name, id);
	
	/*
	for (vector<Device>::const_iterator it = devices.begin(); it != devices.end(); ++it) {
		if (name == it->name) {
			infoL() << "Creating VideoCapturer for " << name << endl;
			out = *it;
			return true;
		}
	}

	// If the name is a valid path to a file, then we'll create a simulated device
	// with the filename. The LmiMediaEngine will know to use a FileVideoCapturer
	// for these devices.
	if (talk_base::Filesystem::IsFile(name)) {
		infoL() << "Creating FileVideoCapturer" << endl;
		*out = FileVideoCapturer::CreateFileVideoCapturerDevice(name);
		return true;
	}
	*/
}


bool DeviceManager::getAudioDevices(bool input, vector<Device>& devs) 
{
	devs.clear();

#ifdef ANDROID
	// Under Android, we don't access the device file directly.
	// Arbitrary use 0 for the mic and 1 for the output.
	// These ids are used in MediaEngine::SetSoundDevices(in, out);
	// The strings are for human consumption.
	if (input) {
		devs.push_back(Device("audioin", "audiorecord", 0));
	} else {
		devs.push_back(Device("audioout", "audiotrack", 1));
	}
	return true;
#else
	return false;
#endif
}


bool DeviceManager::getDefaultVideoCaptureDevice(Device& device) 
{
	bool ret = false;
	// We just return the first device.
	std::vector<Device> devices;
	ret = (getVideoCaptureDevices(devices) && !devices.empty());
	if (ret) {
		device = devices[0];
	}
	return ret;
}


bool DeviceManager::getAudioDevice(bool input, Device& out, const string& name, int id) 
{
	// If the name is empty, return the default device id.
	if (name.empty() || name == kDefaultDeviceName) {
		out = Device(input ? "audioin" : "audioout", -1, name);
		return true;
	}

	std::vector<Device> devices;
	input ? getAudioInputDevices(devices) : getAudioOutputDevices(devices);
	return matchNameAndID(devices, out, name, id);
}


bool DeviceManager::getAudioDevice(bool input, Device& out, int id) 
{
	std::vector<Device> devices;
	input ? getAudioInputDevices(devices) : getAudioOutputDevices(devices);	
	return matchID(devices, out, id);
}


/*
bool DeviceManager::getDefaultAudioInputDevice(Device& device) 
{
	bool ret = false;
	// We just return the first device.
	std::vector<Device> devices;
	ret = (getAudioInputDevices(devices) && !devices.empty());
	if (ret) {
		device = devices[0];
	}
	return ret;
}


bool DeviceManager::getDefaultAudioOutputDevice(Device& device) 
{
	bool ret = false;
	// We just return the first device.
	std::vector<Device> devices;
	ret = (getAudioOutputDevices(devices) && !devices.empty());
	if (ret) {
		device = devices[0];
	}
	return ret;
}
*/


bool DeviceManager::getDefaultAudioInputDevice(Device& device) 
{
	device = Device("audioin", -1, "None");
	return true;
}


bool DeviceManager::getDefaultAudioOutputDevice(Device& device) 
{
	device = Device("audioout", -1, "None");
	return true;
}


bool DeviceManager::shouldDeviceBeIgnored(const string& deviceName, const char* const exclusionList[]) 
{
	// If exclusionList is empty return directly.
	if (!exclusionList)
		return false;

	int i = 0;
	while (exclusionList[i]) {
		if (util::icompare(deviceName, exclusionList[i]) == 0) {
			traceL() << "Ignoring device " << deviceName << endl;
			return true;
		}
		++i;
	}
	return false;
}


bool DeviceManager::filterDevices(vector<Device>& devices, const char* const exclusionList[]) 
{
	//if (!devices) {
	//	return false;
	//}

	for (vector<Device>::iterator it = devices.begin();
		it != devices.end(); ) {
			if (shouldDeviceBeIgnored(it->name, exclusionList)) {
				it = devices.erase(it);
			} else {
				++it;
			}
	}
	return true;
}


bool DeviceManager::matchID(vector<Device>& devices, Device& out, int id)
{
	if (id >= 0 && !devices.empty() && devices.size() >= (id + 1)) {
		out = devices[id];
		return true;
	}
	return false;
}


bool DeviceManager::matchNameAndID(vector<Device>& devices, Device& out, const string& name, int id)
{
	bool ret = false;
	for (size_t i = 0; i < devices.size(); ++i) {
		if (devices[i].name == name) {
			// The first device matching the given name will be returned,
			// but we will try and match the given ID as well.
			//if (out.id == -1)
			out = devices[i];
			ret = true;
			if (id == -1 || id == i)
				break;
		}
	}

	return ret;
}


void DeviceManager::setWatcher(DeviceWatcher* watcher) 
{ 
	if (_watcher)
		delete _watcher;
	_watcher = watcher;
	//_watcher.reset(watcher); 
}


DeviceWatcher* DeviceManager::watcher() 
{ 
	return _watcher; //_watcher.get();
}


void DeviceManager::setInitialized(bool initialized) 
{ 
	_initialized = initialized; 
}


void DeviceManager::print(ostream& ost) 
{
	std::vector<Device> devs;
	getAudioInputDevices(devs);
	ost << "Audio input devices:" << endl;
	for (size_t i = 0; i < devs.size(); ++i)
		devs[i].print(ost);

	getAudioOutputDevices(devs);
	ost << "Audio output devices:" << endl;
	for (size_t i = 0; i < devs.size(); ++i)
		devs[i].print(ost);

	getVideoCaptureDevices(devs);
	ost << "Video capture devices:" << endl;
	for (size_t i = 0; i < devs.size(); ++i)
		devs[i].print(ost);
}


} } // namespace scy::av



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