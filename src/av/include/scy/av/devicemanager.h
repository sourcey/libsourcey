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


#ifndef SCY_AV_DeviceManager_H
#define SCY_AV_DeviceManager_H

#include <string>
#include <vector>

#include "scy/base.h"
#include "scy/signal.h"
#include "scy/av/ffmpeg.h"


#ifdef HAVE_FFMPEG_AVDEVICE
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
}
#else
typedef struct {
} AVInputFormat;
typedef struct {
  const char *name;
} AVOutputFormat;
typedef struct {
} AVFormatContext;
#define av_find_input_format(ANY) (nullptr)
#define av_oformat_next(ANY) (nullptr)
#endif


namespace scy {
namespace av {


class DeviceManager;


//
// Device
//


struct Device
    /// Represents a system audio, video or render device.
{
    enum Type {
        Unknown,
        VideoInput,
        VideoOutput,
        AudioInput,
        AudioOutput
    };

    Type type;
    std::string id;
    std::string name;
    bool isDefault;

    Device();
    Device(Type type, const std::string& id, const std::string& name, bool isDefault = false);

    void print(std::ostream& os);

    bool operator == (const Device& that) const
    {
        return type == that.type
            && id == that.id
            && name == that.name;
    }
};


enum MediaCapabilities
{
    AUDIO_RECV = 1 << 0,
    AUDIO_SEND = 1 << 1,
    VIDEO_RECV = 1 << 2,
    VIDEO_SEND = 1 << 3,
};


//
// Device Watcher
//


class DeviceWatcher
{
public:
    explicit DeviceWatcher(DeviceManager*) {}
    virtual ~DeviceWatcher() {}
    virtual bool start() { return true; }
    virtual void stop() {}
};


//
// Device Manager
//


class DeviceManager
{
public:
    DeviceManager();
    ~DeviceManager();

    // Device enumeration
    bool getCameras(std::vector<Device>& devices);
    bool getMicrophones(std::vector<Device>& devices);
    bool getSpeakers(std::vector<Device>& devices);

    // Default devices
    bool getDefaultMicrophone(Device& device);
    bool getDefaultSpeaker(Device& device);
    bool getDefaultCamera(Device& device);

    // Find device by name or id
    bool findCamera(const std::string& name, Device& device);
    bool findMicrophone(const std::string& name, Device& device);
    bool findSpeaker(const std::string& name, Device& device);

    // Base device list
    bool getDeviceList(Device::Type type, std::vector<av::Device>& devices);

    // Capabilities
    int getCapabilities();

    void setWatcher(DeviceWatcher* watcher);
    DeviceWatcher* watcher();

    void print(std::ostream& ost);
        // Print all devices to the output stream.

    static DeviceManager& instance();
        // Returns the default DeviceManager singleton.

    static void shutdown();
        // Shuts down the MediaFactory and deletes the singleton instance.

    // Signal<> DevicesChanged;
        // Signals when a system device is connecetd or removed.
        // This signal is emitted by the platform specific DeviceWatcher.

    // Find base FFmpeg formats
    AVInputFormat* findVideoInputFormat();
    AVInputFormat* findAudioInputFormat();

    // Default platform input formats
    // std::vector<std::string> cameraInputs;
    // std::vector<std::string> screenInputs;
    // std::vector<std::string> microphoneInputs;
    // std::vector<std::string> speakerOutputs;

protected:
    std::unique_ptr<DeviceWatcher> _watcher;
};


//
// Helpers
//




} } // namespace scy::av


#endif  // SCY_AV_DeviceManager_H
