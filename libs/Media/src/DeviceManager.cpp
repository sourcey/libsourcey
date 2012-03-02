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


#include "Sourcey/Media/DeviceManager.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Poco/UnicodeConverter.h"


#ifndef NO_DEVICE_MANAGER
#if WIN32
#include <atlbase.h>
#include <dbt.h>
#include <strmif.h>  // must come before ks.h
#include <ks.h>
#include <ksmedia.h>
#include <comdef.h>
#ifndef INITGUID
#define INITGUID	// For PKEY_AudioEndpoint_GUID
#endif
#include <mmdeviceapi.h>
#include <mmsystem.h>
#include <functiondiscoverykeys_devpkey.h>
#include <uuids.h>
//#include "Sourcey/Win32.h"
//#include "Sourcey/Win32Window.h"
#elif OSX
#include <CoreAudio/CoreAudio.h>
#include <QuickTime/QuickTime.h>
#elif LINUX
#include <unistd.h>
#endif
#endif // NO_DEVICE_MANAGER


using namespace std;


namespace Sourcey {
namespace Media {


#ifdef NO_DEVICE_MANAGER
static bool getVideoDevices(vector<Device>& out) { return false; };
#else
#ifndef LINUX
static bool ShouldDeviceBeIgnored(const string& deviceName);
#endif
static bool getVideoDevices(vector<Device>& out);
#if WIN32
static const wchar_t kFriendlyName[] = L"FriendlyName";
static const wchar_t kDevicePath[] = L"DevicePath";
static const char kUsbDevicePathPrefix[] = "\\\\?\\usb";
static bool getDevices(const CLSID& catid, vector<Device>& out);
static bool getCoreAudioDevices(bool input, vector<Device>& devs);
static bool getWaveDevices(bool input, vector<Device>& devs);
#elif OSX
static const UInt32 kAudioDeviceNameLength = 64;
static const int kVideoDeviceOpenAttempts = 3;
static bool getAudioDeviceIDs(bool inputs, vector<AudioDeviceID>* out);
static bool getAudioDeviceName(AudioDeviceID id, bool input, string* out);
#endif
#endif // NO_DEVICE_MANAGER


DeviceManager::DeviceManager() : 
	_isInitialized(false), 
	_watcher(new DeviceWatcher(this))
{
}


DeviceManager::~DeviceManager() 
{
	if (_isInitialized) {
		uninitialize();
	}
	delete _watcher;
}


bool DeviceManager::initialize() 
{
	if (!_isInitialized) {
		if (!_watcher->start()) {
			return false;
		}
		_isInitialized = true;
	}
	return true;
}


void DeviceManager::uninitialize() 
{
	if (_isInitialized) {
		_watcher->stop();
		_isInitialized = false;
	}
}


void DeviceManager::toXML(ostream& ost)
{
	ost << "<devices>";	
	vector<Device> devs;
	getAudioInputDevices(devs);
	getVideoInputDevices(devs);
	for (size_t i = 0; i < devs.size(); ++i) {
		ost << "<device"
			<< " type='" << devs[i].type << "'"
			<< " name='" << devs[i].name << "'"
			<< " guid='" << devs[i].guid << "'"
			<< " id='" << devs[i].id << "' />";
	}
	ost << "</devices>";
}


void DeviceManager::print(std::ostream& ost) 
{
	vector<Device> devs;
	getAudioInputDevices(devs);
	ost << "Audio input devices:" << endl;
	for (size_t i = 0; i < devs.size(); ++i) {
		ost << i << ": " << devs[i].name << endl;
	}
	devs.clear();
	getAudioOutputDevices(devs);
	ost << "Audio output devices:" << endl;
	for (size_t i = 0; i < devs.size(); ++i) {
		ost << i << ": " << devs[i].name << endl;
	}
	devs.clear();
	getVideoInputDevices(devs);
	ost << "Video capture devices:" << endl;
	for (size_t i = 0; i < devs.size(); ++i) {
		ost << i << ": " << devs[i].name << endl;
	}
}


bool DeviceManager::getAudioInputDevices(vector<Device>& devices) 
{
	return getAudioDevicesByPlatform(true, devices);
}


bool DeviceManager::getAudioOutputDevices(vector<Device>& devices) 
{
	return getAudioDevicesByPlatform(false, devices);
}


bool DeviceManager::getAudioInputDevice(const string& name, int id, Device& out) 
{
	return getAudioDevice(true, name, id, out);
}


bool DeviceManager::getAudioOutputDevice(const string& name, int id, Device& out) 
{
	return getAudioDevice(false, name, id, out);
}


bool DeviceManager::getAudioInputDevice(int id, Device& out) 
{
	return getAudioDevice(true, "", id, out);
}


bool DeviceManager::getAudioOutputDevice(int id, Device& out) 
{
	return getAudioDevice(false, "", id, out);
}


bool DeviceManager::getVideoInputDevices(vector<Device>& devices) 
{
	return getVideoDevices(devices);
}


bool DeviceManager::getVideoInputDevice(int id, Device& device) 
{
	bool ret = false;
	
	if (id >= 0) {
		vector<Device> devices;
		ret = (getVideoInputDevices(devices) && !devices.empty() && devices.size() >= (id+1));
		if (ret)
			device = devices[id];
	}

	return ret;
}


bool DeviceManager::getVideoInputDevice(const string& name, int id, Device& device) 
{
	bool ret = false;

	vector<Device> devices;
	ret = (getVideoInputDevices(devices) && !devices.empty());
	if (ret) {
		for (size_t i = 0; i < devices.size(); ++i) {
			if (devices[i].name == name) {
				// The device matching the provided name will be returned,
				// but we will try and match the ID as well.
				device = devices[i];
				ret = true;
				if (id == i)
					break;
			}
		}
	}

	return ret;
}


bool DeviceManager::getDefaultAudioInputDevice(Device& device) 
{
	bool ret = false;

	// We just return the first device.
	vector<Device> devices;
	ret = (getAudioInputDevices(devices) && !devices.empty());
	if (ret) {
		device = devices[0];
	}
	return ret;
}


bool DeviceManager::getDefaultVideoInputDevice(Device& device) 
{
	bool ret = false;
#if defined(WIN32) && !defined(NO_DEVICE_MANAGER)
	// If there are multiple capture devices, we want the first USB one.
	// This avoids issues with defaulting to virtual cameras or grabber cards.
	vector<Device> devices;
	ret = (getVideoDevices(devices) && !devices.empty());
	if (ret) {
		device = devices[0];
		for (size_t i = 0; i < devices.size(); ++i) {
			if (strncasecmp(devices[i].name.data(), kUsbDevicePathPrefix,
				ARRAY_SIZE(kUsbDevicePathPrefix) - 1) == 0) {
					device = devices[i];
					break;
			}
		}
	}
#else
	// We just return the first device.
	vector<Device> devices;
	ret = (getVideoInputDevices(devices) && !devices.empty());
	if (ret) {
		device = devices[0];
	}
#endif
	return ret;
}


bool DeviceManager::getAudioDevice(bool isInput, const string& name, int id, Device& out) 
{
	/*
	// If the name is empty, return the default device id.
	if (name.empty() || name == kDefaultDeviceName) {
		out = Device("audio", name, -1);
		return true;
	}
	*/

	vector<Device> devices;
	bool ret = isInput ? 
		getAudioInputDevices(devices) : 
		getAudioOutputDevices(devices);

	if (ret) {
		ret = false;

		// If no name was provided we return the device by ID
		if (name.empty() && devices.size() >= (id+1)) {
			out = devices[id];
			ret = true;
		}
		else {
			for (size_t i = 0; i < devices.size(); ++i) {
				if (devices[i].name == name) {				
					// The device matching the provided name will be returned,
					// but we will try and match the ID as well.
					out = devices[i];
					ret = true;
					if (id == i)
						break;
				}
			}
		}
	}
	return ret;
}


bool DeviceManager::getAudioDevicesByPlatform(bool input, vector<Device>& devs) 
{
	devs.clear();

#if defined(NO_DEVICE_MANAGER)
	return false;

#elif defined(WIN32)
	if (Util::isWindowsVistaOrLater()) {
		return getCoreAudioDevices(input, devs);
	} else {
		return getWaveDevices(input, devs);
	}

#elif defined(OSX)
	vector<AudioDeviceID> dev_ids;
	bool ret = getAudioDeviceIDs(input, &dev_ids);
	if (ret) {
		for (size_t i = 0; i < dev_ids.size(); ++i) {
			string name;
			if (getAudioDeviceName(dev_ids[i], input, &name)) {
				devs.push_back(Device("audio", name, dev_ids[i]));
			}
		}
	}
	return ret;

#elif defined(LINUX)
	int card = -1, dev = -1;
	snd_ctl_t *handle = NULL;
	snd_pcm_info_t *pcminfo = NULL;

	snd_pcm_info_malloc(&pcminfo);

	while (true) {
		if (snd_card_next(&card) != 0 || card < 0)
			break;

		char *card_name;
		if (snd_card_get_name(card, &card_name) != 0)
			continue;

		char card_string[7];
		snprintf(card_string, sizeof(card_string), "hw:%d", card);
		if (snd_ctl_open(&handle, card_string, 0) != 0)
			continue;

		while (true) {
			if (snd_ctl_pcm_next_device(handle, &dev) < 0 || dev < 0)
				break;
			snd_pcm_info_set_device(pcminfo, dev);
			snd_pcm_info_set_subdevice(pcminfo, 0);
			snd_pcm_info_set_stream(pcminfo, input ? SND_PCM_STREAM_CAPTURE :
				SND_PCM_STREAM_PLAYBACK);
			if (snd_ctl_pcm_info(handle, pcminfo) != 0)
				continue;

			char name[128];
			sprintfn(name, sizeof(name), "%s (%s)", card_name,
				snd_pcm_info_get_name(pcminfo));
			// TODO(tschmelcher): We might want to identify devices with something
			// more specific than just their card number (e.g., the PCM names that
			// aplay -L prints out).
			devs.push_back(Device("audio", name, card));

			Log("debug") << "Found device: id = " << card << ", name = "
				<< name;
		}
		snd_ctl_close(handle);
	}
	snd_pcm_info_free(pcminfo);
	return true;
#else
	return false;
#endif
}



#if !defined(NO_DEVICE_MANAGER)
#if defined(WIN32)
bool getVideoDevices(vector<Device>& devices) 
{
	// TODO(juberti): Move the CoInit stuff to Initialize/Terminate.
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		Log("error") << "CoInitialize failed, hr=" << hr << endl;
		if (hr != RPC_E_CHANGED_MODE) {
			return false;
		}
	}

	bool ret = getDevices(CLSID_VideoInputDeviceCategory, devices);
	if (SUCCEEDED(hr)) {
		CoUninitialize();
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
			Log("error") << "Failed to create device enumerator, hr="  << hr << endl;
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
					string name_str, path_str;
					if (SUCCEEDED(bag->Read(kFriendlyName, &name, 0)) &&
						name.vt == VT_BSTR) {
							Poco::UnicodeConverter::toUTF8(name.bstrVal, name_str);
							if (!ShouldDeviceBeIgnored(name_str)) {
								// get the device id if one exists.
								if (SUCCEEDED(bag->Read(kDevicePath, &path, 0)) &&
									path.vt == VT_BSTR) {
										Poco::UnicodeConverter::toUTF8(path.bstrVal, path_str);
								}

								devices.push_back(Device("video", name_str, (int)devices.size(), path_str));
							}
					}
			}
			mk = NULL;
		}
	}

	return true;
}


// Adapted from http://msdn.microsoft.com/en-us/library/dd370812(v=VS.85).aspx
HRESULT DeviceFromImmDevice(IMMDevice& device, Device& out) 
{
	CComPtr<IPropertyStore> props;
	PROPVARIANT name, guid;

	HRESULT hr = device.OpenPropertyStore(STGM_READ, &props);
	if (FAILED(hr)) {
		return hr;
	}

	// get the endpoint's name and id.
	PropVariantInit(&name);
	hr = props->GetValue(PKEY_Device_FriendlyName, &name);
	if (SUCCEEDED(hr)) {
		PropVariantInit(&guid);
		hr = props->GetValue(PKEY_AudioEndpoint_GUID, &guid);

		if (SUCCEEDED(hr)) {
			Poco::UnicodeConverter::toUTF8(name.pwszVal, out.name);
			Poco::UnicodeConverter::toUTF8(guid.pwszVal, out.guid);
		}
		PropVariantClear(&guid);
	}
	PropVariantClear(&name);
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

					// get pointer to endpoint number i.
					hr = devices->Item(i, &device);
					if (FAILED(hr)) {
						break;
					}

					Device dev;
					hr = DeviceFromImmDevice(*device, dev);
					if (SUCCEEDED(hr)) {
						dev.id = i;
						devs.push_back(dev);
					} else {
						break;
					}
				}
			}
		}
	}

	if (!SUCCEEDED(hr)) {
		Log("error") << "getCoreAudioDevices failed with hr " << hr << endl;
		return false;
	}
	return true;
}


bool getWaveDevices(bool input, vector<Device>& devs) 
{
	// Note, we don't use the System Device Enumerator interface here since it
	// adds lots of pseudo-devices to the vector, such as DirectSound and Wave
	// variants of the same device.
	string name;
	if (input) {
		int num_devs = waveInGetNumDevs();
		for (int i = 0; i < num_devs; ++i) {
			WAVEINCAPS caps;
			if (waveInGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR &&
				caps.wChannels > 0) {
					Poco::UnicodeConverter::toUTF8(caps.szPname, name);
					devs.push_back(Device("audio", name, i));
			}
		}
	} else {
		int num_devs = waveOutGetNumDevs();
		for (int i = 0; i < num_devs; ++i) {
			WAVEOUTCAPS caps;
			if (waveOutGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR &&
				caps.wChannels > 0) {
					Poco::UnicodeConverter::toUTF8(caps.szPname, name);
					devs.push_back(Device("audio", name, i));
			}
		}
	}
	return true;
}


/*
DeviceWatcher::DeviceWatcher(DeviceManager* manager)
	: _manager(manager), _audioNotify(NULL), _videoNotify(NULL) {
}

bool DeviceWatcher::start() {
	if (!Create(NULL, _T("Sourcey DeviceWatcher Window"),
		0, 0, 0, 0, 0, 0)) {
			return false;
	}

	_audioNotify = Register(KSCATEGORY_AUDIO);
	if (!_audioNotify) {
		stop();
		return false;
	}

	_videoNotify = Register(KSCATEGORY_VIDEO);
	if (!_videoNotify) {
		stop();
		return false;
	}

	return true;
}

void DeviceWatcher::stop() {
	UnregisterDeviceNotification(_videoNotify);
	_videoNotify = NULL;
	UnregisterDeviceNotification(_audioNotify);
	_audioNotify = NULL;
	Destroy();
}

HDEVNOTIFY DeviceWatcher::Register(REFGUID guid) {
	DEV_BROADCAST_DEVICEINTERFACE dbdi;
	dbdi.dbcc_size = sizeof(dbdi);
	dbdi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	dbdi.dbcc_classguid = guid;
	dbdi.dbcc_name[0] = '\0';
	return RegisterDeviceNotification(handle(), &dbdi,
		DEVICE_NOTIFY_WINDOW_HANDLE);
}

void DeviceWatcher::Unregister(HDEVNOTIFY handle) {
	UnregisterDeviceNotification(handle);
}

bool DeviceWatcher::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
		if (uMsg == WM_DEVICECHANGE) {
			if (wParam == DBT_DEVICEARRIVAL ||
				wParam == DBT_DEVICEREMOVECOMPLETE) {
					DEV_BROADCAST_DEVICEINTERFACE* dbdi =
						reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(lParam);
					if (dbdi->dbcc_classguid == KSCATEGORY_AUDIO ||
						dbdi->dbcc_classguid == KSCATEGORY_VIDEO) {
							int tmp = 0;
							_manager->DevicesChanged.signal(_manager, tmp);
							//_manager->OnDevicesChange();
					}
			}
			result = 0;
			return true;
		}

		return false;
}
*/

#elif defined(OSX)
static bool getVideoDevices(vector<Device>& devices) 
{
	ComponentDescription only_vdig;
	memset(&only_vdig, 0, sizeof(only_vdig));
	only_vdig.componentType = videoDigitizerComponentType;
	only_vdig.componentSubType = kAnyComponentSubType;
	only_vdig.componentManufacturer = kAnyComponentManufacturer;

	// Enumerate components (drivers).
	Component component = 0;
	while ((component = FindNextComponent(component, &only_vdig))) {
		// get the name of the component and see if we want to open it.
		Handle name_handle = NewHandle(0);
		getComponentInfo(component, NULL, name_handle, NULL, NULL);
		Ptr name_ptr = *name_handle;
		string comp_name(name_ptr + 1, static_cast<size_t>(*name_ptr));
		DisposeHandle(name_handle);

		if (!ShouldDeviceBeIgnored(comp_name)) {
			// Try to open the component.
			// DV Video will fail with err=-9408 (deviceCantMeetRequest)
			// IIDC FireWire Video and USB Video Class Video will fail with err=704
			// if no cameras are present, or there is contention for the camera.
			// We can't tell the scenarios apart, so we will retry a few times if
			// we get a 704 to make sure we detect the cam if one is really there.
			int attempts = 0;
			ComponentInstance vdig;
			OSErr err;
			do {
				err = OpenAComponent(component, &vdig);
				attempts++;
			} while (!vdig && err == 704 && attempts < kVideoDeviceOpenAttempts);

			if (vdig) {
				// We were able to open the component.
				Log("debug") << "Opened component \"" << comp_name
					<< "\", tries=" << attempts;

				// Enumerate cameras on the component.
				// Note, that due to QT strangeness VDgetNumberOfMedias really returns
				// the number of inputs minus one. If no inputs are available -1 is
				// returned.
				short num_inputs;  // NOLINT
				VideoDigitizerError err = VDgetNumberOfMedias(vdig, &num_inputs);
				if (err == 0 && num_inputs >= 0) {
					Log("debug") << "Found " << num_inputs + 1 << " webcams attached.";
					Str255 pname;
					for (int i = 0; i <= num_inputs; ++i) {
						err = VDgetMediaName(vdig, i, pname);
						if (err == 0) {
							// The format for camera ids is <component>:<camera index>.
							char id_buf[256];
							sprintfn(id_buf, ARRAY_SIZE(id_buf), "%s:%d",
								comp_name.c_str(), i);
							string name(reinterpret_cast<const char*>(pname + 1),
								static_cast<size_t>(*pname)), id(id_buf);
							Log("debug") << "  Webcam " << i << ": " << name;
							devices.push_back(Device("video", name, id));
						}
					}
				}
				CloseComponent(vdig);
			} else {
				Log("debug") << "Failed to open component \"" << comp_name
					<< "\", err=" << err;
			}
		}
	}
	return true;
}


static bool getAudioDeviceIDs(bool input, vector<AudioDeviceID>* outDevIDs) 
{
		UInt32 propsize;
		OSErr err = AudioHardwaregetPropertyInfo(kAudioHardwarePropertyDevices,
			&propsize, NULL);
		if (0 != err) {
			Log("error") << "Couldn't get information about property, "
				<< "so no device vector acquired.";
			return false;
		}

		size_t num_devices = propsize / sizeof(AudioDeviceID);
		scoped_array<AudioDeviceID> device_ids(
			new AudioDeviceID[num_devices]);

		err = AudioHardwaregetProperty(kAudioHardwarePropertyDevices,
			&propsize, device_ids.get());
		if (0 != err) {
			Log("error") << "Failed to get device ids, "
				<< "so no device vectoring acquired.";
			return false;
		}

		for (size_t i = 0; i < num_devices; ++i) {
			AudioDeviceID an_id = device_ids[i];
			// find out the number of channels for this direction
			// (input/output) on this device -
			// we'll ignore anything with no channels.
			err = AudioDevicegetPropertyInfo(an_id, 0, input,
				kAudioDevicePropertyStreams,
				&propsize, NULL);
			if (0 == err) {
				unsigned num_channels = propsize / sizeof(AudioStreamID);
				if (0 < num_channels) {
					outDevIDs->push_back(an_id);
				}
			} else {
				Log("error") << "No property info for stream property for device id "
					<< an_id << "(isInput == " << input
					<< "), so not including it in the vector.";
			}
		}

		return true;
}


static bool getAudioDeviceName(AudioDeviceID id,
	bool input,
	string* out_name) {
		UInt32 nameLength = kAudioDeviceNameLength;
		char name[kAudioDeviceNameLength + 1];
		OSErr err = AudioDevicegetProperty(id, 0, input,
			kAudioDevicePropertyDeviceName,
			&nameLength, name);
		if (0 != err) {
			Log("error") << "No name acquired for device id " << id;
			return false;
		}

		*out_name = name;
		return true;
}


#elif defined(LINUX)
static const string kVideoMetaPathK2_4("/proc/video/dev/");
static const string kVideoMetaPathK2_6("/sys/class/video4linux/");

enum MetaType { M2_4, M2_6, NONE };

static void ScanDeviceDirectory(const string& devdir, vector<Device>& devices) 
{
	scoped_ptr<DirectoryIterator> directoryIterator(
		Filesystem::IterateDirectory());

	if (directoryIterator->Iterate(Pathname(devdir))) {
		do {
			string filename = directoryIterator->Name();
			string deviceName = devdir + filename;
			if (!directoryIterator->IsDots()) {
				if (filename.find("video") == 0 &&
					V4LLookup::IsV4L2Device(deviceName)) {
						devices.push_back(Device("video", deviceName, deviceName));
				}
			}
		} while (directoryIterator->Next());
	}
}


static string getVideoDeviceNameK2_6(const string& device_meta_path) 
{
	string deviceName;

	scoped_ptr<FileStream> device_meta_stream(
		Filesystem::OpenFile(device_meta_path, "r"));

	if (device_meta_stream.get() != NULL) {
		if (device_meta_stream->ReadLine(deviceName) != SR_SUCCESS) {
			Log("error") << "Failed to read V4L2 device meta " << device_meta_path;
		}
		device_meta_stream->Close();
	}

	return deviceName;
}


static string Trim(const string& s, const string& drop = " \t") 
{
	string::size_type first = s.find_first_not_of(drop);
	string::size_type last  = s.find_last_not_of(drop);

	if (first == string::npos || last == string::npos)
		return string("");

	return s.substr(first, last - first + 1);
}


static string getVideoDeviceNameK2_4(const string& device_meta_path) 
{
	ConfigParser::MapVector all_values;

	ConfigParser config_parser;
	FileStream* file_stream =
		Filesystem::OpenFile(device_meta_path, "r");

	if (file_stream == NULL) return "";

	config_parser.Attach(file_stream);
	config_parser.Parse(&all_values);

	for (ConfigParser::MapVector::iterator i = all_values.begin();
		i != all_values.end(); ++i) {
			ConfigParser::SimpleMap::iterator deviceName_i =
				i->find("name");

			if (deviceName_i != i->end()) {
				return deviceName_i->second;
			}
	}

	return "";
}


static string getVideoDeviceName(MetaType meta,
	const string& device_file_name) 
{
		string device_meta_path;
		string deviceName;
		string meta_file_path;

		if (meta == M2_6) {
			meta_file_path = kVideoMetaPathK2_6 + device_file_name + "/name";

			Log("debug") << "Trying " + meta_file_path;
			deviceName = getVideoDeviceNameK2_6(meta_file_path);

			if (deviceName.empty()) {
				meta_file_path = kVideoMetaPathK2_6 + device_file_name + "/model";

				Log("debug") << "Trying " << meta_file_path;
				deviceName = getVideoDeviceNameK2_6(meta_file_path);
			}
		} else {
			meta_file_path = kVideoMetaPathK2_4 + device_file_name;
			Log("debug") << "Trying " << meta_file_path;
			deviceName = getVideoDeviceNameK2_4(meta_file_path);
		}

		if (deviceName.empty()) {
			deviceName = "/dev/" + device_file_name;
			Log("error")
				<< "Device name not found, defaulting to device path " << deviceName;
		}

		Log("debug") << "Name for " << device_file_name << " is " << deviceName;

		return Trim(deviceName);
}


static void ScanV4L2Devices(vector<Device>& devices) 
{
	Log("debug") << ("Enumerating V4L2 devices");

	MetaType meta;
	string metadata_dir;

	scoped_ptr<DirectoryIterator> directoryIterator(
		Filesystem::IterateDirectory());

	// Try and guess kernel version
	if (directoryIterator->Iterate(kVideoMetaPathK2_6)) {
		meta = M2_6;
		metadata_dir = kVideoMetaPathK2_6;
	} else if (directoryIterator->Iterate(kVideoMetaPathK2_4)) {
		meta = M2_4;
		metadata_dir = kVideoMetaPathK2_4;
	} else {
		meta = NONE;
	}

	if (meta != NONE) {
		Log("debug") << "V4L2 device metadata found at " << metadata_dir;

		do {
			string filename = directoryIterator->Name();

			if (filename.find("video") == 0) {
				string device_path = "/dev/" + filename;

				if (V4LLookup::IsV4L2Device(device_path)) {
					devices.push_back(
						Device("video", getVideoDeviceName(meta, filename), device_path));
				}
			}
		} while (directoryIterator->Next());
	} else {
		Log("error") << "Unable to detect v4l2 metadata directory";
	}

	if (devices.size() == 0) {
		Log("debug") << "Plan B. Scanning all video devices in /dev directory";
		ScanDeviceDirectory("/dev/", devices);
	}

	Log("debug") << "Total V4L2 devices found : " << devices.size();
}


static bool getVideoDevices(vector<Device>& devices) 
{
	ScanV4L2Devices(devices);
	Log("debug") << "Total V4L2 devices found : " << devices.size();

	return true;
}
#endif

#ifndef LINUX

// TODO: Get hold of a copy of Final Cut to understand why we
// crash while scanning their components on OS X.
static bool ShouldDeviceBeIgnored(const string& deviceName) {
	static const char* const kFilteredDevices[] =  {
		"Google Camera Adapter",   // Ignore Google magiccams
#ifdef WIN32
		"Asus virtual Camera",     // Bad Asus desktop virtual cam
		"Bluetooth Video",         // Bad Sony viao bluetooth sharing driver
#endif
#ifdef OSX
		"DVCPRO HD",               // Final cut
#endif
	};

	for (int i = 0; i < ARRAY_SIZE(kFilteredDevices); ++i) {
		if (strncasecmp(deviceName.c_str(), kFilteredDevices[i],
			strlen(kFilteredDevices[i])) == 0) {
				//Log("error") << "Ignoring device " << deviceName << endl;
				return true;
		}
	}
	return false;
}
#endif

#endif // NO_DEVICE_MANAGER


} } // namespace Sourcey::Media


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

