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


#include "scy/media/devicemanager_win32.h"
#include "scy/platform.h"

#ifdef HAVE_RTAUDIO

#include "RtAudio.h"

#include <windows.h>
#include <dshow.h>
#include <dbt.h> // DBT_* & DEV_*
#include <ks.h>
#include <ksmedia.h> // KSCATEGORY_*


using std::endl; 


namespace scy {
namespace av {


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
static bool getDevicesWin32(const CLSID& catid, std::vector<Device>& out);
	

IDeviceManager* DeviceManagerFactory::create() 
{
	return new Win32DeviceManager();
}


//
// Win32 Device Manager
//


Win32DeviceManager::Win32DeviceManager() : 
	_needCoUninitialize(false) 
{
	TraceL << "Create" << endl;
	
	// FIXME: Not receiving WM_DEVICECHANGE in our console applications
	setWatcher(new Win32DeviceWatcher(this));
}


Win32DeviceManager::~Win32DeviceManager() 
{
	TraceL << "Destroy" << endl;
}


bool Win32DeviceManager::initialize() 
{
	TraceL << "Initializing" << endl;
	if (!initialized()) {
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		//HRESULT hr = CoInitialize(nullptr);
		_needCoUninitialize = SUCCEEDED(hr);
		if (FAILED(hr)) {
			if (hr != RPC_E_CHANGED_MODE) {
				ErrorL << "CoInitialize failed, hr=" << hr << endl;
				return false;
			}
			else
				WarnL << "CoInitialize Changed Mode" << endl;
		}
		if (watcher() && !watcher()->start()) {
			ErrorL << "Cannot start watcher" << endl;
			return false;
		}
		setInitialized(true);
	}
	TraceL << "Initializing: OK" << endl;
	return true;
}


void Win32DeviceManager::uninitialize() 
{
	TraceL << "Uninitializing" << endl;

	if (initialized()) {
		if (watcher())
			watcher()->stop();
		if (_needCoUninitialize) {
			CoUninitialize();
			_needCoUninitialize = false;
		}
		setInitialized(false);
	}
	TraceL << "Uninitializing: OK" << endl;
}


bool Win32DeviceManager::getAudioDevices(bool input, std::vector<Device>& devs) 
{
	devs.clear();

	// Since we are using RtAudio for audio capture, we should
	// use RtAudio to enumerate devices to ensure indexes match.	
	RtAudio audio;

	// Determine the number of devices available
	auto ndevices = audio.getDeviceCount();
	TraceLS(this) << "Get audio devices: " << ndevices << endl;

	// Scan through devices for various capabilities
	RtAudio::DeviceInfo info;
	for (unsigned i = 0; i <= ndevices; i++) {
		try {
			info = audio.getDeviceInfo(i);	// may throw RtError

			TraceLS(this) << "Device:" 
				<< "\n\tName: " << info.name
				<< "\n\tOutput Channels: " << info.outputChannels
				<< "\n\tInput Channels: " << info.inputChannels
				<< "\n\tDuplex Channels: " << info.duplexChannels
				<< "\n\tDefault Output: " << info.isDefaultOutput
				<< "\n\tDefault Input: " << info.isDefaultInput
				<< "\n\tProbed: " << info.probed
				<< endl;

			if (info.probed == true &&
				(input && info.inputChannels > 0) || 
				(!input && info.outputChannels > 0)) {	

				TraceLS(this) << "Adding device: " << info.name << endl;
				Device dev((input ? "audioin" : "audioout"), i, info.name, "", 
					(input ? info.isDefaultInput : info.isDefaultOutput));
				devs.push_back(dev);
			}
		} 
		catch (RtError& e) {
			ErrorLS(this) << "Cannot probe audio device: " << e.getMessage() << endl;
		}
	}

	return filterDevices(devs, kFilteredAudioDevicesName);
}


bool Win32DeviceManager::getDefaultAudioDevice(bool input, Device& device) 
{
	bool ret = false;
	std::vector<Device> devices;
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


bool Win32DeviceManager::getVideoCaptureDevices(std::vector<Device>& devices) 
{
	devices.clear();
	if (!getDevicesWin32(CLSID_VideoInputDeviceCategory, devices)) 
		return false;
	return filterDevices(devices, kFilteredVideoDevicesName);
}

#define ARRAY_SIZE(x) (static_cast<int>((sizeof(x)/sizeof(x[0]))))


bool Win32DeviceManager::getDefaultVideoCaptureDevice(Device& device) 
{
	bool ret = false;
	// If there are multiple capture devices, we want the first USB device.
	// This avoids issues with defaulting to virtual cameras or grabber cards.
	std::vector<Device> devices;
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


namespace internal { // Windows API stuff

	#pragma comment(lib, "strmiids")


	HRESULT enumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
	{
		// Create the System Device Enumerator.
		ICreateDevEnum *pDevEnum;
		HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,  
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

		if (SUCCEEDED(hr)) {
			// Create an enumerator for the category.
			hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
			if (hr == S_FALSE) {
				hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
			}
			pDevEnum->Release();
		}
		return hr;
	}


	void getDeviceInformation(IEnumMoniker *pEnum, REFGUID catid, std::vector<Device>& devices)
	{
		IMoniker *pMoniker = NULL;

		while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag *pPropBag;
			HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
			if (FAILED(hr)) {
				pMoniker->Release();
				continue;  
			} 

			VARIANT var;
			VariantInit(&var);
			std::string name_str, path_str;

			// Get description or friendly name.
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
			if (FAILED(hr)) {
				hr = pPropBag->Read(L"Description", &var, 0);
			}
			if (SUCCEEDED(hr)) {
				//printf("%S\n", var.bstrVal);
				name_str = scy::toUtf8(var.bstrVal);
				VariantClear(&var); 
			}

			hr = pPropBag->Read(L"DevicePath", &var, 0);
			if (SUCCEEDED(hr)) {
				// The device path is not intended for display.
				//printf("Device path: %S\n", var.bstrVal);
				path_str = scy::toUtf8(var.bstrVal);
				VariantClear(&var); 
			}
		
			devices.push_back(
				Device(catid == CLSID_VideoInputDeviceCategory ? "video" : "audio",
				(int)devices.size(), name_str, path_str));

			pPropBag->Release();
			pMoniker->Release();
		}
	}

} // namespace internal


bool getDevicesWin32(REFGUID catid, std::vector<Device>& devices) 
{	
	// Selecting a Capture Device
	// http://msdn.microsoft.com/en-us/library/windows/desktop/dd377566(v=vs.85).aspx

	IEnumMoniker *pEnum;
    HRESULT hr = internal::enumerateDevices(catid, &pEnum);
    if (SUCCEEDED(hr)) {
        internal::getDeviceInformation(pEnum, catid, devices);
        pEnum->Release();
		return true;
    }
	return false;
}


//
// Win32 Device Watcher
//


Win32DeviceWatcher::Win32DeviceWatcher(Win32DeviceManager* manager) : 
	DeviceWatcher(manager),
	manager_(manager),
	audio_notify_(NULL),
	video_notify_(NULL) 
{
}


Win32DeviceWatcher::~Win32DeviceWatcher()
{
}


bool Win32DeviceWatcher::start() 
{
	if (!Create(NULL, L"LibSourcey Device Watcher Window", 0, 0, 0, 0, 0, 0)) {
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


void Win32DeviceWatcher::stop()
{
	UnregisterDeviceNotification(video_notify_);
	video_notify_ = NULL;
	UnregisterDeviceNotification(audio_notify_);
	audio_notify_ = NULL;
	Destroy();
}


HDEVNOTIFY Win32DeviceWatcher::Register(REFGUID guid)
{
	//DEV_BROADCAST_DEVICEINTERFACE dbdi;
	//dbdi.dbcc_size = sizeof(dbdi);
	//dbdi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	//dbdi.dbcc_classguid = guid;
	
    DEV_BROADCAST_DEVICEINTERFACE dbdi;

    ZeroMemory( &dbdi, sizeof(dbdi) );
    dbdi.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    dbdi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    dbdi.dbcc_classguid = guid;

	// Add DEVICE_NOTIFY_ALL_INTERFACE_CLASSES to ignore 
	// dbcc_classguid and listen for all GUID types.
	return RegisterDeviceNotification(handle(), &dbdi,
		DEVICE_NOTIFY_WINDOW_HANDLE);
		//DEVICE_NOTIFY_WINDOW_HANDLE);
}


void Win32DeviceWatcher::Unregister(HDEVNOTIFY handle)
{
	UnregisterDeviceNotification(handle);
}


bool Win32DeviceWatcher::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
{
	DebugL << "OnMessage: " << uMsg << endl;

	if (uMsg == WM_DEVICECHANGE) {
		//bool arriving = wParam == DBT_DEVICEARRIVAL;
		//bool removing = wParam == DBT_DEVICEREMOVECOMPLETE;
		if (wParam == DBT_DEVICEARRIVAL ||
			wParam == DBT_DEVICEREMOVECOMPLETE) {
				DEV_BROADCAST_DEVICEINTERFACE* dbdi =
					reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(lParam);
				if (dbdi->dbcc_classguid == KSCATEGORY_AUDIO ||
					dbdi->dbcc_classguid == KSCATEGORY_VIDEO) {
						bool isVideo = dbdi->dbcc_classguid == KSCATEGORY_VIDEO;
						bool isConnect = wParam == DBT_DEVICEARRIVAL;
						DebugL << "Signal Devices changed: " << isVideo << ": " << isConnect << endl;
						manager_->DevicesChanged.emit(manager_, isVideo, isConnect);
				}
		}
		result = 0;
		return true;
	}

	return false;
}


//
// Win32 Window
//

static const wchar_t kWindowBaseClassName[] = L"WindowBaseClass";
HINSTANCE Win32Window::instance_ = NULL;
ATOM Win32Window::window_class_ = 0;

Win32Window::Win32Window() : wnd_(NULL) 
{
}


Win32Window::~Win32Window() 
{
	assert(NULL == wnd_);
}


bool Win32Window::Create(HWND parent, const wchar_t* title, DWORD style,
						 DWORD exstyle, int x, int y, int cx, int cy) 
{
	if (wnd_) {
		// Window already exists.
		return false;
	}

	if (!window_class_) {
		if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			reinterpret_cast<LPCWSTR>(&Win32Window::WndProc),
			&instance_)) {
				ErrorL << "GetModuleHandleEx failed" << endl;
				return false;
		}

		// Class not registered, register it.
		WNDCLASSEX wcex;
		memset(&wcex, 0, sizeof(wcex));
		wcex.cbSize = sizeof(wcex);
		wcex.hInstance = instance_;
		wcex.lpfnWndProc = &Win32Window::WndProc;
		wcex.lpszClassName = kWindowBaseClassName;
		window_class_ = ::RegisterClassEx(&wcex);
		if (!window_class_) {
			ErrorL << "RegisterClassEx failed" << endl;
			return false;
		}
	}
	wnd_ = ::CreateWindowEx(exstyle, kWindowBaseClassName, title, style,
		x, y, cx, cy, parent, NULL, instance_, this);
	return (NULL != wnd_);
}


void Win32Window::Destroy() 
{
	BOOL res = ::DestroyWindow(wnd_);
	assert(res != FALSE);
}


void Win32Window::Shutdown() 
{
	if (window_class_) {
		::UnregisterClass(MAKEINTATOM(window_class_), instance_);
		window_class_ = 0;
	}
}


bool Win32Window::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) 
{
	switch (uMsg) {
	case WM_CLOSE:
		if (!OnClose()) {
			result = 0;
			return true;
		}
		break;
	}
	return false;
}


LRESULT Win32Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	Win32Window* that = reinterpret_cast<Win32Window*>(
		::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (!that && (WM_CREATE == uMsg)) {
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		that = static_cast<Win32Window*>(cs->lpCreateParams);
		that->wnd_ = hwnd;
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(that));
	}
	if (that) {
		LRESULT result;
		bool handled = that->OnMessage(uMsg, wParam, lParam, result);
		if (WM_DESTROY == uMsg) {
			for (HWND child = ::GetWindow(hwnd, GW_CHILD); child;
				child = ::GetWindow(child, GW_HWNDNEXT)) {
					DebugL << "Child window: " << static_cast<void*>(child) << endl;
			}
		}
		if (WM_NCDESTROY == uMsg) {
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
			that->wnd_ = NULL;
			that->OnNcDestroy();
		}
		if (handled) {
			return result;
		}
	}
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}


} } // namespace scy::av


#endif


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


/*
bool getDevices(const CLSID& catid, std::vector<Device>& devices) 
{	
	HRESULT hr;

	// CComPtr is a scoped pointer that will be auto released when going
	// out of scope. CoUninitialize must not be called before the
	// release.
	CComPtr<ICreateDevEnum> sys_dev_enum;
	CComPtr<IEnumMoniker> cam_enum;
	if (FAILED(hr = sys_dev_enum.CoCreateInstance(CLSID_SystemDeviceEnum)) ||
		FAILED(hr = sys_dev_enum->CreateClassEnumerator(catid, &cam_enum, 0))) {
			ErrorL << "Cannot create device enumerator, hr="  << hr << endl;
			return false;
	}

	// Only enum devices if CreateClassEnumerator returns S_OK. If there are no
	// devices available, S_FALSE will be returned, but enumMk will be nullptr.
	if (hr == S_OK) {
		CComPtr<IMoniker> mk;
		while (cam_enum->Next(1, &mk, nullptr) == S_OK) {
			CComPtr<IPropertyBag> bag;
			if (SUCCEEDED(mk->BindToStorage(nullptr, nullptr,
				__uuidof(bag), reinterpret_cast<void**>(&bag)))) {
					CComVariant name, path;
					std::string type_str, name_str, path_str;
					if (SUCCEEDED(bag->Read(L"FriendlyName", &name, 0)) &&
						name.vt == VT_BSTR) {
							name = scy::toUtf8(name.bstrVal);
							// Get the device id if one exists.
							if (SUCCEEDED(bag->Read(L"DevicePath", &path, 0)) &&
								path.vt == VT_BSTR) {
									path = scy::toUtf8(path.bstrVal);
							}
							if (catid == CLSID_VideoInputDeviceCategory)
								type_str = "video";
							else 
								assert(0 && "unknown type");
							devices.push_back(Device(type_str, (int)devices.size(), name_str, path_str));
					}
			}
			mk = nullptr;
		}
	}

	return true;
}
*/


/*
static const wchar_t kFriendlyName[] = L"FriendlyName";
static const wchar_t kDevicePath[] = L"DevicePath";
static bool getCoreAudioDevices(bool input, std::vector<Device>& devs);
static bool getWaveDevices(bool input, std::vector<Device>& devs);


bool Win32DeviceManager::getAudioDevices(bool input, std::vector<Device>& devs) 
{
	devs.clear();

	//if (scy::IsWindowsVistaOrLater()) {
	if (util::isWindowsVistaOrLater()) {
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
			Poco::toUtf::toUTF8(var.pwszVal, *out);
			//*out = scy::toUtf8(var.pwszVal);
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
	std::string name, guid;
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


bool getCoreAudioDevices(bool input, std::vector<Device>& devs) 
{
	HRESULT hr = S_OK;
	CComPtr<IMMDeviceEnumerator> enumerator;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
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
					IMMEndpoint* endpoint = nullptr;
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
					
					TraceL << "Enumerating Device: " << i << endl;
					hr = getDeviceFromImmDevice(device, dev);
					if (SUCCEEDED(hr)) {
						devs.push_back(dev);
					} else {
						WarnL << "Cannot query IMM Device, skipping.  HR=" << hr << endl;
						hr = S_FALSE;
					}
				}
			}
		}
	}

	if (FAILED(hr)) {
		WarnL << "getCoreAudioDevices failed with hr " << hr << endl;
		return false;
	}
	return true;
}


bool getWaveDevices(bool input, std::vector<Device>& devs) 
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
					std::string name;
					Poco::toUtf::toUTF8(caps.szPname, name);
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
					std::string name;
					devs.push_back(Device("audioout", name, i));
					//devs.push_back(Device(scy::toUtf8(caps.szPname), i));
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
	audio_notify_(nullptr),
	video_notify_(nullptr) {
}

Win32DeviceWatcher::~Win32DeviceWatcher() {
}

bool Win32DeviceWatcher::start() {
	if (!Create(nullptr, _T("libjingle Win32DeviceWatcher Window"),
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
	video_notify_ = nullptr;
	UnregisterDeviceNotification(audio_notify_);
	audio_notify_ = nullptr;
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
							manager_->DevicesChanged();
					}
			}
			result = 0;
			return true;
		}

		return false;
}
*/