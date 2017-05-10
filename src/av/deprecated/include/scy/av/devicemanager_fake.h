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


#ifndef SCY_AV_DeviceManager_FAKE_H
#define SCY_AV_DeviceManager_FAKE_H


#include <string>
#include <vector>

#include "scy/av/devicemanager.h"
//#include "talk/session/phone/devicemanager.h"
//#include "talk/session/phone/mediacommon.h"


namespace scy {
namespace av {


class FakeDeviceManager : public IDeviceManager
{
public:
    FakeDeviceManager() {}

    virtual bool initialize() { return true; }

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

    virtual bool getMicrophones(std::vector<Device>& devs)
    {
        devs = input_devices_;
        return true;
    }

    virtual bool getSpeakers(std::vector<Device>& devs)
    {
        devs = output_devices_;
        return true;
    }

    virtual bool getMicrophone(const std::string& name, Device& out)
    {
        return getAudioDevice(true, name, out);
    }

    virtual bool getSpeaker(const std::string& name, Device& out)
    {
        return getAudioDevice(false, name, out);
    }

    virtual bool getCameras(std::vector<Device>& devs)
    {
        devs = vidcap_devices_;
        return true;
    }

    virtual bool getDefaultCamera(Device& device)
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

    void setMicrophones(const std::vector<Device>& devices)
    {
        // for (std::size_t i = 0; i < devices.size(); ++i) {
        ///    input_devices_.push_back(Device(devices[i], i));
        //}
        // input_devices_.clear();

        // DevicesChanged();
        input_devices_ = devices;
    }

    void setSpeakers(const std::vector<Device>& devices)
    {
        // for (std::size_t i = 0; i < devices.size(); ++i) {
        ///    output_devices_.push_back(Device(devices[i], i));
        //}
        // output_devices_.clear();

        // DevicesChanged();
        output_devices_ = devices;
    }

    void setCameras(const std::vector<Device>& devices)
    {
        // for (std::size_t i = 0; i < devices.size(); ++i) {
        ///    vidcap_devices_.push_back(Device(devices[i], i));
        //}
        // vidcap_devices_.clear();

        // DevicesChanged();
        vidcap_devices_ = devices;
    }


    virtual bool getCamera(const std::string& name, Device& out)
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
        // if (name.empty() || name == kDefaultDeviceName) {
        //    out = Device(name, -1);
        //    return true;
        //}

        return findDeviceByName((input ? input_devices_ : output_devices_),
                                name, out);
    }

    static bool findDeviceByName(const std::vector<Device>& devices,
                                 const std::string& name, Device& out)
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


} // namespace av
} // namespace scy


#endif // SCY_AV_DeviceManager_FAKE_H


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