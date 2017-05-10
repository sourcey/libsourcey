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


#ifndef SCY_AV_DeviceManager_H
#define SCY_AV_DeviceManager_H

#include <string>
#include <vector>

#include "scy/base.h"
#include "scy/signal.h"


namespace scy {
namespace av {

/// Represents a system audio, video or render device.
struct Device
{
    Device();
    Device(const std::string& type, int id, const std::string& name,
           const std::string& guid = "", bool isDefault = false,
           bool isAvailable = true);

    void print(std::ostream& os);

    std::string type; // audioin, audioout, video
    int id;
    std::string name;
    std::string guid;
    bool isDefault;
    bool isAvailable;

    bool operator==(const Device& that) const
    {
        return id == that.id && type == that.type && name == that.name &&
               guid == that.guid;
    }
};


enum MediaCapabilities
{
    AUDIO_RECV = 1 << 0,
    AUDIO_SEND = 1 << 1,
    VIDEO_RECV = 1 << 2,
    VIDEO_SEND = 1 << 3,
};

/// A platform independent interface to enumerate audio
/// and video devices on the system.
class IDeviceManager
{
public:
    virtual ~IDeviceManager() {}
    /// Initialization
    virtual bool initialize() = 0;
    virtual void uninitialize() = 0;

    /// Device enumeration
    virtual bool getAudioInputDevices(std::vector<Device>& devices) = 0;
    virtual bool getAudioOutputDevices(std::vector<Device>& devices) = 0;

    virtual bool getAudioInputDevice(Device& out, const std::string& name,
                                     int id = -1) = 0;
    virtual bool getAudioInputDevice(Device& out, int id) = 0;

    virtual bool getAudioOutputDevice(Device& out, const std::string& name,
                                      int id = -1) = 0;
    virtual bool getAudioOutputDevice(Device& out, int id) = 0;

    // virtual bool getAudioInputDevice(const std::string& name, Device& out) =
    // 0;
    // virtual bool getAudioOutputDevice(const std::string& name, Device& out) =
    // 0;

    virtual bool getVideoCaptureDevices(std::vector<Device>& devs) = 0;
    virtual bool getVideoCaptureDevice(Device& out, int id) = 0;
    virtual bool getVideoCaptureDevice(Device& out, const std::string& name,
                                       int id = -1) = 0;

    virtual bool getDefaultAudioInputDevice(Device& device) = 0;
    virtual bool getDefaultAudioOutputDevice(Device& device) = 0;
    virtual bool getDefaultVideoCaptureDevice(Device& device) = 0;

    /// Capabilities
    virtual int getCapabilities() = 0;

    virtual void print(std::ostream& ost) = 0; /// Signals on DevicesChanged.
    /// Arg 1 is true when device is video, false for audio
    /// Arg 2 is true when device connects, flase on disconnection
    Signal<void(bool&, bool&)> DevicesChanged;


    static const char kDefaultDeviceName[];
};


class DeviceWatcher
{
public:
    explicit DeviceWatcher(IDeviceManager*) {}
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


class DeviceManager : public IDeviceManager
{
public:
    DeviceManager();
    virtual ~DeviceManager();

    /// Initialization
    virtual bool initialize();
    virtual void uninitialize();

    /// Capabilities
    virtual int getCapabilities();

    /// Device enumeration
    virtual bool getAudioInputDevices(std::vector<Device>& devices);
    virtual bool getAudioOutputDevices(std::vector<Device>& devices);

    // virtual bool getAudioInputDevice(const std::string& name, Device& out);
    // virtual bool getAudioOutputDevice(const std::string& name, Device& out);

    virtual bool getAudioInputDevice(Device& out, const std::string& name,
                                     int id = -1);
    virtual bool getAudioInputDevice(Device& out, int id);

    virtual bool getAudioOutputDevice(Device& out, const std::string& name,
                                      int id = -1);
    virtual bool getAudioOutputDevice(Device& out, int id);

    virtual bool getVideoCaptureDevices(std::vector<Device>& devs);
    virtual bool getVideoCaptureDevice(
        Device& out, const std::string& name,
        int id =
            -1); /// Returns the video capture device at the given system index.
    virtual bool getVideoCaptureDevice(Device& out, int id);


    virtual bool getDefaultAudioInputDevice(Device& device);
    virtual bool getDefaultAudioOutputDevice(Device& device);
    virtual bool getDefaultVideoCaptureDevice(
        Device&
            device); /// The exclusionList MUST be a nullptr terminated list.
    static bool filterDevices(std::vector<Device>& devices,
                              const char* const exclusionList[]);

    /// Returns a device matching the given ID.
    static bool matchID(std::vector<Device>& devices, Device& out, int id);

    /// Returns a device matching the given name and ID.
    /// If the device name is not available at the given ID then first
    /// device of that name will be returned.
    /// If the ID should not be matched the given ID should be -1.
    static bool matchNameAndID(std::vector<Device>& devices, Device& out,
                               const std::string& name, int id = -1);


    bool initialized() const { return _initialized; }

    void print(std::ostream& ost);

protected:
    virtual bool getDefaultAudioDevice(bool input, Device& device);
    virtual bool getAudioDevices(bool input, std::vector<Device>& devs);
    virtual bool getAudioDevice(bool input, Device& out,
                                const std::string& name, int id = -1);
    virtual bool getAudioDevice(bool input, Device& out, int id);

    void setInitialized(bool initialized);

    void setWatcher(DeviceWatcher* watcher);
    DeviceWatcher* watcher();

private: /// The exclusionList MUST be a nullptr terminated list.
    static bool shouldDeviceBeIgnored(const std::string& deviceName,
                                      const char* const exclusionList[]);

    DeviceWatcher* _watcher;
    bool _initialized;
};


} // namespace av
} // namespace scy


#endif // SCY_AV_DeviceManager_H


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
