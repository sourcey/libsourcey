///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{
// Implemented from libjingle r116 Feb 16, 2012


#include "scy/av/devicemanager_mac.h"

#include <CoreAudio/CoreAudio.h>
#include <QuickTime/QuickTime.h>

// #include "talk/base/logging.h"
// #include "talk/base/stringutils.h"
// #include "talk/base/thread.h"
// #include "talk/base/mediacommon.h"
using std::endl;

class DeviceWatcherImpl;

namespace scy {
namespace av {


IDeviceManager* DeviceManagerFactory::create()
{
    return new MacDeviceManager();
}


class MacDeviceWatcher : public DeviceWatcher
{
public:
    explicit MacDeviceWatcher(IDeviceManager* dm);
    virtual ~MacDeviceWatcher();
    virtual bool start();
    virtual void stop();

private:
    IDeviceManager* manager_;
    DeviceWatcherImpl* impl_;
};


static const char* kFilteredAudioDevicesName[] = {
    NULL,
};
// TODO: Try to get hold of a copy of Final Cut to understand
// why we crash while scanning their components on OS X.
static const char* const kFilteredVideoDevicesName[] = {
    "Google Camera Adapter", // Our own magiccams
    "DVCPRO HD",             // Final cut
    "Sonix SN9C201p",        // Crashes in OpenAComponent and CloseComponent
    NULL,
};
static const int kVideoDeviceOpenAttempts = 3;
static const std::uint32_t kAudioDeviceNameLength = 64;
// Obj-C functions defined in DeviceManager_MAC.mm
// TODO: have a shared header for these function defines.
extern DeviceWatcherImpl* CreateDeviceWatcherCallback(IDeviceManager* dm);
extern void ReleaseDeviceWatcherCallback(DeviceWatcherImpl* impl);
extern bool GetAVFoundationVideoDevices(std::vector<Device>* out);
// static bool getAudioDeviceIDs(bool inputs, std::vector<AudioDeviceID>* out);
// static bool getAudioDeviceName(AudioDeviceID id, bool input, std::string*
// out);


MacDeviceManager::MacDeviceManager()
{
    setWatcher(new MacDeviceWatcher(this));
}


MacDeviceManager::~MacDeviceManager()
{
}


bool MacDeviceManager::getCameras(std::vector<Device>& devices)
{
    devices.clear();
    if (!GetAVFoundationVideoDevices(&devices)) {
        return false;
    }
    return filterDevices(devices, kFilteredVideoDevicesName);
}


// bool MacDeviceManager::getAudioDevices(bool input,
//                                        std::vector<Device>& devs) {
//   devs.clear();
//   std::vector<AudioDeviceID> dev_ids;
//   bool ret = getAudioDeviceIDs(input, &dev_ids);
//   if (!ret) {
//     return false;
//   }
//   for (std::size_t i = 0; i < dev_ids.size(); ++i) {
//     std::string name;
//     if (getAudioDeviceName(dev_ids[i], input, &name)) {
//       devs.push_back(Device(name, dev_ids[i]));
//     }
//   }
//   return filterDevices(devs, kFilteredAudioDevicesName);
// }
//
//
// static bool getAudioDeviceIDs(bool input,
//                               std::vector<AudioDeviceID>* out_dev_ids) {
//   std::uint32_t propsize;
//   OSErr err = AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices,
//                                            &propsize, NULL);
//   if (0 != err) {
//     SError << "Couldn't get information about property, "
//                   << "so no device list acquired." << endl;
//     return false;
//   }
//
//   std::size_t num_devices = propsize / sizeof(AudioDeviceID);
//   auto device_ids = new AudioDeviceID[num_devices];
//   // talk_base::scoped_array<AudioDeviceID> device_ids(
//   //     new AudioDeviceID[num_devices]);
//
//   err = AudioHardwareGetProperty(kAudioHardwarePropertyDevices,
//                                  &propsize, device_ids); //.get()
//   if (0 != err) {
//     SError << "Failed to get device ids, "
//                   << "so no device listing acquired." << endl;
// 	delete device_ids;
//     return false;
//   }
//
//   for (std::size_t i = 0; i < num_devices; ++i) {
//     AudioDeviceID an_id = device_ids[i];
//     // find out the number of channels for this direction
//     // (input/output) on this device -
//     // we'll ignore anything with no channels.
//     err = AudioDeviceGetPropertyInfo(an_id, 0, input,
//                                      kAudioDevicePropertyStreams,
//                                      &propsize, NULL);
//     if (0 == err) {
//       unsigned num_channels = propsize / sizeof(AudioStreamID);
//       if (0 < num_channels) {
//         out_dev_ids->push_back(an_id);
//       }
//     } else {
//       SError << "No property info for stream property for device id "
//                     << an_id << "(input == " << input
//                     << "), so not including it in the list." << endl;
//     }
//   }
//
//   delete device_ids;
//   return true;
// }
//
//
// static bool getAudioDeviceName(AudioDeviceID id,
//                                bool input,
//                                std::string* out_name) {
//   std::uint32_t nameLength = kAudioDeviceNameLength;
//   char name[kAudioDeviceNameLength + 1];
//   OSErr err = AudioDeviceGetProperty(id, 0, input,
//                                      kAudioDevicePropertyDeviceName,
//                                      &nameLength, name);
//   if (0 != err) {
//     LError("No name acquired for device id ", id)
//     return false;
//   }
//
//   *out_name = name;
//   return true;
// }


MacDeviceWatcher::MacDeviceWatcher(IDeviceManager* manager)
    : DeviceWatcher(manager)
    , manager_(manager)
    , impl_(NULL)
{
}


MacDeviceWatcher::~MacDeviceWatcher()
{
}


bool MacDeviceWatcher::start()
{
    if (!impl_) {
        impl_ = CreateDeviceWatcherCallback(manager_);
    }
    return impl_ != NULL;
}


void MacDeviceWatcher::stop()
{
    if (impl_) {
        ReleaseDeviceWatcherCallback(impl_);
        impl_ = NULL;
    }
}


} // namespace av
} // namespace scy


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
