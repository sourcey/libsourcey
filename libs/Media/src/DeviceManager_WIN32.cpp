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


static const char* kFilteredAudioDevicesName[] = {
	NULL,
};
static const char* const kFilteredVideoDevicesName[] =  {
	"Google Camera Adapter",   // Our own magiccams
	"Asus virtual Camera",     // Bad Asus desktop virtual cam
	"Bluetooth Video",         // Bad Sony viao bluetooth sharing driver
	NULL,
};
static const wchar_t kFriendlyName[] = L"FriendlyName";
static const wchar_t kDevicePath[] = L"DevicePath";
static const char kUsbDevicePathPrefix[] = "\\\\?\\usb";
static bool getDevices(const CLSID& catid, vector<Device>& out);
static bool getCoreAudioDevices(bool input, vector<Device>& devs);
static bool getWaveDevices(bool input, vector<Device>& devs);


Win32DeviceManager::Win32DeviceManager() : 
	_need_couninitialize(false) 
{
	Log("trace") << "[DeviceManager] Creating" << endl;
	//setWatcher(new Win32DeviceWatcher(this));
}


Win32DeviceManager::~Win32DeviceManager() 
{
	Log("trace") << "[DeviceManager] Destroying" << endl;
	if (initialized()) {
		uninitialize();
	}
}


bool Win32DeviceManager::initialize() 
{
	Log("trace") << "[DeviceManager] Initializing" << endl;
	if (!initialized()) {
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		//HRESULT hr = CoInitialize(NULL);
		_need_couninitialize = SUCCEEDED(hr);
		if (FAILED(hr)) {
			if (hr != RPC_E_CHANGED_MODE) {
				Log("error") << "[DeviceManager] CoInitialize failed, hr=" << hr << endl;
				return false;
			}
			else
				Log("warn") << "[DeviceManager] CoInitialize Changed Mode" << endl;
		}
		if (watcher() && !watcher()->start()) {
			return false;
		}
		setInitialized(true);
	}
	return true;
}


void Win32DeviceManager::uninitialize() 
{
	Log("trace") << "[DeviceManager] Uninitializing" << endl;

	if (initialized()) {
		if (watcher())
			watcher()->stop();
		if (_need_couninitialize) {
			CoUninitialize();
			_need_couninitialize = false;
		}
		setInitialized(false);
	}
}


bool Win32DeviceManager::getDefaultVideoCaptureDevice(Device& device) 
{
	bool ret = false;
	// If there are multiple capture devices, we want the first USB one.
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


bool Win32DeviceManager::getVideoCaptureDevices(vector<Device>& devices) 
{
	devices.clear();
	if (!getDevices(CLSID_VideoInputDeviceCategory, devices)) {
		return false;
	}
	return filterDevices(devices, kFilteredVideoDevicesName);
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
			Log("error") << "[DeviceManager] Failed to create device enumerator, hr="  << hr << endl;
			return false;
	}

	// Only enum devices if CreateClassEnumerator returns S_OK. If there are no
	// devices available, S_FALSE will be returned, but enumMk will be NULL.
	if (hr == S_OK) {
		CComPtr<IMoniker> mk;
		while (cam_enum->Next(1, &mk, NULL) == S_OK) {
/*
#ifdef HAVE_LOGITECH_HEADERS
			// Initialize Logitech device if applicable
			MaybeLogitechDeviceReset(mk);
#endif
*/
			CComPtr<IPropertyBag> bag;
			if (SUCCEEDED(mk->BindToStorage(NULL, NULL,
				__uuidof(bag), reinterpret_cast<void**>(&bag)))) {
					CComVariant name, path;
					string type_str, name_str, path_str;
					if (SUCCEEDED(bag->Read(kFriendlyName, &name, 0)) &&
						name.vt == VT_BSTR) {
							Poco::UnicodeConverter::toUTF8(name.bstrVal, name_str);
							//name_str = talk_base::ToUtf8(name.bstrVal);
							// Get the device id if one exists.
							if (SUCCEEDED(bag->Read(kDevicePath, &path, 0)) &&
								path.vt == VT_BSTR) {
									Poco::UnicodeConverter::toUTF8(path.bstrVal, path_str);
									//path_str = talk_base::ToUtf8(path.bstrVal);
							}
							if (catid == CLSID_VideoInputDeviceCategory)
								type_str = "video";
							else 
								assert(0 && "unknown type");
							devices.push_back(Device(type_str, name_str, (int)devices.size(), path_str));
					}
			}
			mk = NULL;
		}
	}

	return true;
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

	// Get the endpoint's name and id.
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
		hr = enumerator->EnumAudioEndpoints((input ? eCapture : eRender),
			DEVICE_STATE_ACTIVE, &devices);
		if (SUCCEEDED(hr)) {
			unsigned int count;
			hr = devices->GetCount(&count);

			if (SUCCEEDED(hr)) {
				for (unsigned int i = 0; i < count; i++) {
					CComPtr<IMMDevice> device;

					// Get pointer to endpoint number i.
					hr = devices->Item(i, &device);
					if (FAILED(hr)) {
						break;
					}

					Device dev(input ? "audioin" : "audioout", "", i);
					hr = getDeviceFromImmDevice(device, dev);
					if (SUCCEEDED(hr)) {
						devs.push_back(dev);
					} else {
						Log("warn") << "[DeviceManager] Cannot query IMM Device, skipping.  HR=" << hr << endl;
						hr = S_FALSE;
					}
				}
			}
		}
	}

	if (FAILED(hr)) {
		Log("warn") << "[DeviceManager] getCoreAudioDevices failed with hr " << hr << endl;
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