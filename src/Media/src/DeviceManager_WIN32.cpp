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


#include "Sourcey/Media/DeviceManager_WIN32.h"

#include "Poco/UnicodeConverter.h"

#include "RtAudio.h"

#include <atlbase.h>
#include <dbt.h>
#include <strmif.h>  // must come before ks.h
#include <ks.h>
#include <ksmedia.h>
#define INITGUID  // For PKEY_AudioEndpoint_GUID
#include <mmdeviceapi.h>
#include <mmsystem.h>
#include <functiondiscoverykeys_devpkey.h>
#include <uuids.h>

/*
#include "talk/base/win32.h"  // ToUtf8
#include "talk/base/win32window.h"
#include "talk/base/logging.h"
#include "talk/base/stringutils.h"
#include "talk/base/thread.h"
#include "Sourcey/Media/mediacommon.h"
#ifdef HAVE_LOGITECH_HEADERS
#include "third_party/logitech/files/logitechquickcam.h"
#endif
*/


using namespace std; 


namespace Sourcey {
namespace Media {
	

IDeviceManager* DeviceManagerFactory::create() 
{
	return new Win32DeviceManager();
}


static const char* kFilteredAudioDevicesName[] = {
	NULL,
};
static const char* const kFilteredVideoDevicesName[] =  {
	"Google Camera Adapter",   // Google magiccams
	"Asus virtual Camera",     // Bad Asus desktop virtual cam
	"Bluetooth Video",         // Bad Sony viao bluetooth sharing driver
	NULL,
};
static const char kUsbDevicePathPrefix[] = "\\\\?\\usb";
static bool getDevices(const CLSID& catid, vector<Device>& out);


// ---------------------------------------------------------------------
// Win32 Device Manager
//
Win32DeviceManager::Win32DeviceManager() : 
	_needCoUninitialize(false) 
{
	LogTrace("DeviceManager") << "Creating" << endl;
	//setWatcher(new Win32DeviceWatcher(this));
}


Win32DeviceManager::~Win32DeviceManager() 
{
	LogTrace("DeviceManager") << "Destroying" << endl;
	if (initialized()) {
		uninitialize();
	}
}


bool Win32DeviceManager::initialize() 
{
	LogTrace("DeviceManager") << "Initializing" << endl;
	if (!initialized()) {
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		//HRESULT hr = CoInitialize(NULL);
		_needCoUninitialize = SUCCEEDED(hr);
		if (FAILED(hr)) {
			if (hr != RPC_E_CHANGED_MODE) {
				LogError("DeviceManager") << "CoInitialize failed, hr=" << hr << endl;
				return false;
			}
			else
				LogWarn("DeviceManager") << "CoInitialize Changed Mode" << endl;
		}
		if (watcher() && !watcher()->start()) {
			LogError("DeviceManager") << "Cannot start watcher" << endl;
			return false;
		}
		setInitialized(true);
	}
	LogTrace("DeviceManager") << "Initializing: OK" << endl;
	return true;
}


void Win32DeviceManager::uninitialize() 
{
	LogTrace("DeviceManager") << "Uninitializing" << endl;

	if (initialized()) {
		if (watcher())
			watcher()->stop();
		if (_needCoUninitialize) {
			CoUninitialize();
			_needCoUninitialize = false;
		}
		setInitialized(false);
	}
	LogTrace("DeviceManager") << "Uninitializing: OK" << endl;
}


bool Win32DeviceManager::getAudioDevices(bool input, vector<Device>& devs) 
{
	devs.clear();

	// Since we are using RtAudio for device capture, we should
	// use RtAudio to enumerate devices to ensure indexes match.	
	RtAudio audio;
	try 
	{
		// Determine the number of devices available
		unsigned int devices = audio.getDeviceCount();

		// Scan through devices for various capabilities
		RtAudio::DeviceInfo info;
		for (unsigned int i = 0; i <= devices; i++) {
			info = audio.getDeviceInfo(i);
			if (info.probed == true &&
				(input && info.inputChannels > 0) || 
				(!input && info.outputChannels > 0)) {	
				/*
					*/
				LogTrace("Win32DeviceManager", this) << "Device:" 
					<< "\n\tName: " << info.name
					<< "\n\tOutput Channels: " << info.outputChannels
					<< "\n\tInput Channels: " << info.inputChannels
					<< "\n\tDuplex Channels: " << info.duplexChannels
					<< "\n\tDefault Output: " << info.isDefaultOutput
					<< "\n\tDefault Input: " << info.isDefaultInput
					<< endl;
				
				Device dev((input ? "audioin" : "audioout"), i, info.name, "", 
					(input ? info.isDefaultInput : info.isDefaultOutput));
				devs.push_back(dev);
			}
		}
	} 
	catch(...) {}

	return filterDevices(devs, kFilteredAudioDevicesName);
}


bool Win32DeviceManager::getDefaultAudioDevice(bool input, Device& device) 
{
	bool ret = false;
	vector<Device> devices;
	ret = (getAudioDevices(input, devices) && !devices.empty());
	if (ret) {
		// Use the first device by default
		device = devices[0];

		// Loop through devices to check if any are explicitly 
		// set as default
		for (size_t i = 0; i < devices.size(); ++i) {
			if (devices[i].isDefault) {
				device = devices[i];
			}
		}
	}
	return ret;
}


bool Win32DeviceManager::getDefaultAudioInputDevice(Device& device) 
{	
	return getDefaultAudioDevice(true, device);
}


bool Win32DeviceManager::getDefaultAudioOutputDevice(Device& device) 
{
	return getDefaultAudioDevice(false, device);
}


bool Win32DeviceManager::getVideoCaptureDevices(vector<Device>& devices) 
{
	devices.clear();
	if (!getDevices(CLSID_VideoInputDeviceCategory, devices)) {
		return false;
	}
	return filterDevices(devices, kFilteredVideoDevicesName);
}


bool Win32DeviceManager::getDefaultVideoCaptureDevice(Device& device) 
{
	bool ret = false;
	// If there are multiple capture devices, we want the first USB device.
	// This avoids issues with defaulting to virtual cameras or grabber cards.
	vector<Device> devices;
	ret = (getVideoCaptureDevices(devices) && !devices.empty());
	if (ret) {
		device = devices[0];
		for (size_t i = 0; i < devices.size(); ++i) {
			if (strnicmp(devices[i].guid.c_str(), kUsbDevicePathPrefix,
			//if (strnicmp(devices[i].id.c_str(), kUsbDevicePathPrefix,
				ARRAY_SIZE(kUsbDevicePathPrefix) - 1) == 0) {
					device = devices[i];
					break;
			}
		}
	}
	return ret;
}


bool getDevices(const CLSID& catid, vector<Device>& devices) 
{
	HRESULT hr;

	// CComPtr is a scoped pointer that will be auto released when going
	// out of scope. CoUninitialize must not be called before the
	// release.
	CComPtr<ICreateDevEnum> sys_dev_enum;
	CComPtr<IEnumMoniker> cam_enum;
	if (FAILED(hr = sys_dev_enum.CoCreateInstance(CLSID_SystemDeviceEnum)) ||
		FAILED(hr = sys_dev_enum->CreateClassEnumerator(catid, &cam_enum, 0))) {
			LogError("DeviceManager") << "Cannot create device enumerator, hr="  << hr << endl;
			return false;
	}

	// Only enum devices if CreateClassEnumerator returns S_OK. If there are no
	// devices available, S_FALSE will be returned, but enumMk will be NULL.
	if (hr == S_OK) {
		CComPtr<IMoniker> mk;
		while (cam_enum->Next(1, &mk, NULL) == S_OK) {
			CComPtr<IPropertyBag> bag;
			if (SUCCEEDED(mk->BindToStorage(NULL, NULL,
				__uuidof(bag), reinterpret_cast<void**>(&bag)))) {
					CComVariant name, path;
					string type_str, name_str, path_str;
					if (SUCCEEDED(bag->Read(L"FriendlyName", &name, 0)) &&
						name.vt == VT_BSTR) {
							Poco::UnicodeConverter::toUTF8(name.bstrVal, name_str);
							//name_str = talk_base::ToUtf8(name.bstrVal);
							// Get the device id if one exists.
							if (SUCCEEDED(bag->Read(L"DevicePath", &path, 0)) &&
								path.vt == VT_BSTR) {
									Poco::UnicodeConverter::toUTF8(path.bstrVal, path_str);
									//path_str = talk_base::ToUtf8(path.bstrVal);
							}
							if (catid == CLSID_VideoInputDeviceCategory)
								type_str = "video";
							else 
								assert(0 && "unknown type");
							devices.push_back(Device(type_str, (int)devices.size(), name_str, path_str));
					}
			}
			mk = NULL;
		}
	}

	return true;
}


/*
static const wchar_t kFriendlyName[] = L"FriendlyName";
static const wchar_t kDevicePath[] = L"DevicePath";
static bool getCoreAudioDevices(bool input, vector<Device>& devs);
static bool getWaveDevices(bool input, vector<Device>& devs);


bool Win32DeviceManager::getAudioDevices(bool input, vector<Device>& devs) 
{
	devs.clear();

	//if (talk_base::IsWindowsVistaOrLater()) {
	if (Util::isWindowsVistaOrLater()) {
		if (!getCoreAudioDevices(input, devs))
			return false;
	} else {
		if (!getWaveDevices(input, devs))
			return false;
	}
	return filterDevices(devs, kFilteredAudioDevicesName);
}


HRESULT getStringProp(IPropertyStore* bag, PROPERTYKEY key, string* out) {
	out->clear();
	PROPVARIANT var;
	PropVariantInit(&var);

	HRESULT hr = bag->GetValue(key, &var);
	if (SUCCEEDED(hr)) {
		if (var.pwszVal)
			Poco::UnicodeConverter::toUTF8(var.pwszVal, *out);
			//*out = talk_base::ToUtf8(var.pwszVal);
		else
			hr = E_FAIL;
	}

	PropVariantClear(&var);
	return hr;
}


// Adapted from http://msdn.microsoft.com/en-us/library/dd370812(v=VS.85).aspx
HRESULT getDeviceFromImmDevice(IMMDevice* device, Device& out) {
	CComPtr<IPropertyStore> props;

	HRESULT hr = device->OpenPropertyStore(STGM_READ, &props);
	if (FAILED(hr)) {
		return hr;
	}

	// Get the endpoint's name and guid.
	string name, guid;
	hr = getStringProp(props, PKEY_Device_FriendlyName, &name);
	if (SUCCEEDED(hr)) {
		hr = getStringProp(props, PKEY_AudioEndpoint_GUID, &guid);

		if (SUCCEEDED(hr)) {
			out.name = name;
			out.guid = guid;
		}
	}
	return hr;
}


bool getCoreAudioDevices(bool input, vector<Device>& devs) 
{
	HRESULT hr = S_OK;
	CComPtr<IMMDeviceEnumerator> enumerator;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));
	if (SUCCEEDED(hr)) {
		CComPtr<IMMDeviceCollection> devices;

		// Enumerate ALL devices, so we can get the correct system index
		hr = enumerator->EnumAudioEndpoints(eAll, //(input ? eCapture : eRender),
			DEVICE_STATE_ACTIVE, &devices);
		if (SUCCEEDED(hr)) {
			unsigned int count;
			hr = devices->GetCount(&count);

			if (SUCCEEDED(hr)) {
				for (unsigned int i = 0; i < count; i++) {
					CComPtr<IMMDevice> device;

					// Get pointer to endpoint
					hr = devices->Item(i, &device);
					if (FAILED(hr))
						break;
					
					// Get an endpoint interface for the current device
					IMMEndpoint* endpoint = NULL;
					hr = device->QueryInterface(__uuidof(IMMEndpoint), (void**)&endpoint);
					if (FAILED(hr))
						break;

					// Get the data flow for the current device
					EDataFlow dataFlow;
					hr = endpoint->GetDataFlow(&dataFlow);
					if (FAILED(hr))
						break;

					// Get next device if data flow is not as specified 
					if (dataFlow != (input ? eCapture : eRender))
						continue;

					//device->QueryInterface(__uuidof(IMMEndpoint))
					// You can get the flow (render or capture) by calling IMMDevice::QueryInterface(__uuidof(IMMEndpoint)) and then IMMEndpoint::GetDataFlow.
					// http://social.msdn.microsoft.com/Forums/en-US/windowspro-audiodevelopment/thread/50889257-7200-4137-851d-6cc690234863/
					//device
					
					Device dev(input ? "audioin" : "audioout", "", i);
					
					LogTrace("DeviceManager") << "Enumerating Device: " << i << endl;
					hr = getDeviceFromImmDevice(device, dev);
					if (SUCCEEDED(hr)) {
						devs.push_back(dev);
					} else {
						LogWarn("DeviceManager") << "Cannot query IMM Device, skipping.  HR=" << hr << endl;
						hr = S_FALSE;
					}
				}
			}
		}
	}

	if (FAILED(hr)) {
		LogWarn("DeviceManager") << "getCoreAudioDevices failed with hr " << hr << endl;
		return false;
	}
	return true;
}


bool getWaveDevices(bool input, vector<Device>& devs) 
{
	// Note, we don't use the System Device Enumerator interface here since it
	// adds lots of pseudo-devices to the list, such as DirectSound and Wave
	// variants of the same device.
	if (input) {
		int numDevs = waveInGetNumDevs();
		for (int i = 0; i < numDevs; ++i) {
			WAVEINCAPS caps;
			if (waveInGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR &&
				caps.wChannels > 0) {					
					string name;
					Poco::UnicodeConverter::toUTF8(caps.szPname, name);
					devs.push_back(Device("audioin", name, i));
			}
		}
	} 
	else {
		int numDevs = waveOutGetNumDevs();
		for (int i = 0; i < numDevs; ++i) {
			WAVEOUTCAPS caps;
			if (waveOutGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR &&
				caps.wChannels > 0) {			
					string name;
					devs.push_back(Device("audioout", name, i));
					//devs.push_back(Device(talk_base::ToUtf8(caps.szPname), i));
			}
		}
	}
	return true;
}
*/


/*
Win32DeviceWatcher::Win32DeviceWatcher(Win32DeviceManager* manager)
	: DeviceWatcher(manager),
	manager_(manager),
	audio_notify_(NULL),
	video_notify_(NULL) {
}

Win32DeviceWatcher::~Win32DeviceWatcher() {
}

bool Win32DeviceWatcher::start() {
	if (!Create(NULL, _T("libjingle Win32DeviceWatcher Window"),
		0, 0, 0, 0, 0, 0)) {
			return false;
	}

	audio_notify_ = Register(KSCATEGORY_AUDIO);
	if (!audio_notify_) {
		stop();
		return false;
	}

	video_notify_ = Register(KSCATEGORY_VIDEO);
	if (!video_notify_) {
		stop();
		return false;
	}

	return true;
}

void Win32DeviceWatcher::stop() {
	UnregisterDeviceNotification(video_notify_);
	video_notify_ = NULL;
	UnregisterDeviceNotification(audio_notify_);
	audio_notify_ = NULL;
	Destroy();
}

HDEVNOTIFY Win32DeviceWatcher::Register(REFGUID guid) {
	DEV_BROADCAST_DEVICEINTERFACE dbdi;
	dbdi.dbcc_size = sizeof(dbdi);
	dbdi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	dbdi.dbcc_classguid = guid;
	dbdi.dbcc_name[0] = '\0';
	return RegisterDeviceNotification(handle(), &dbdi,
		DEVICE_NOTIFY_WINDOW_HANDLE);
}

void Win32DeviceWatcher::Unregister(HDEVNOTIFY handle) {
	UnregisterDeviceNotification(handle);
}

bool Win32DeviceWatcher::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
	LRESULT& result) {
		if (uMsg == WM_DEVICECHANGE) {
			if (wParam == DBT_DEVICEARRIVAL ||
				wParam == DBT_DEVICEREMOVECOMPLETE) {
					DEV_BROADCAST_DEVICEINTERFACE* dbdi =
						reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(lParam);
					if (dbdi->dbcc_classguid == KSCATEGORY_AUDIO ||
						dbdi->dbcc_classguid == KSCATEGORY_VIDEO) {
							manager_->SignalDevicesChange();
					}
			}
			result = 0;
			return true;
		}

		return false;
}
*/


} } // namespace Sourcey::Media


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