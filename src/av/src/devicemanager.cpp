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


#include "scy/av/devicemanager.h"


// TODO: implement all possible platform formats

#if defined(SCY_WIN)
    #define SCY_CAMERA_INPUTS     {"dshow", "vfwcap"}
    #define SCY_SCREEN_INPUTS     {"gdigrab"}
    #define SCY_MICROPHONE_INPUTS {"audio_device", "dsound"}
    #define SCY_SPEAKER_OUTPUTS   {"audio_device", "dsound"}
#elif defined(SCY_APPLE)
    #define SCY_CAMERA_INPUTS     {"avfoundation", "qtkit"}
    #define SCY_SCREEN_INPUTS     {"avfoundation"} // TODO: filter screen input formats
    #define SCY_MICROPHONE_INPUTS {"avfoundation"} // TODO: filter screen input formats
    #define SCY_SPEAKER_OUTPUTS   {"avfoundation"} // TODO: filter screen output formats
#elif defined(SCY_LINUX)
    #define SCY_CAMERA_INPUTS     {"v4l2", "dv1394"}
    #define SCY_SCREEN_INPUTS     {"x11grab"}
    #define SCY_MICROPHONE_INPUTS {"alsa", "jack"}
    #define SCY_SPEAKER_OUTPUTS   {"alsa", "jack"}
#endif


using std::endl;


namespace scy {
namespace av {


//
// Device
//


Device::Device() :
    type(Unknown), isDefault(false)
{
}


Device::Device(Type type, const std::string& id, const std::string& name, bool isDefault) :
    type(type), id(id), name(name), isDefault(isDefault)
{
}


void Device::print(std::ostream& os)
{
    os << "Device["
        << type << ":"
        << name << ":"
        << id //<< ":"
        // << isDefault
        << "]";
}


//
// Helpers
//


namespace internal {


// Substitute for missing `av_find_output_format()` function
AVOutputFormat* findOutputFormat(const std::string& name)
{
    initializeFFmpeg();
    AVOutputFormat* oformat = av_oformat_next(nullptr);
    while (oformat) {
        if (name == oformat->name) {
            break;
        }
        oformat = av_oformat_next(oformat);
    }
    uninitializeFFmpeg();
    return nullptr;
}


AVOutputFormat* findDefaultOutputFormat(const std::vector<std::string>& outputs)
{
    AVOutputFormat* oformat = nullptr;

    initializeFFmpeg(); // init here so reference is not held
    for (auto& output : outputs) {
        oformat = findOutputFormat(output.c_str());
        if (oformat) break;
    }
    uninitializeFFmpeg();

    assert(oformat && "no output format found");
    return oformat;
}


AVInputFormat* findDefaultInputFormat(const std::vector<std::string>& inputs)
{
    AVInputFormat* iformat = nullptr;

    initializeFFmpeg(); // init here so reference is not held
    for (auto& input : inputs) {
        iformat = av_find_input_format(input.c_str());
        if (iformat) break;
    }
    uninitializeFFmpeg();

    assert(iformat && "no input format found");
    return iformat;
}


// AVInputFormat* getDefaultInputFormat(DeviceManager* devman, Device::Type type)
// {
//     AVInputFormat* iformat = nullptr;
//
//     switch (type) {
//     case Device::Camera:
//         return findDefaultInputFormat(devman->cameraInputs);
//         break;
//     case Device::Microphone:
//         return findDefaultInputFormat(devman->microphoneInputs);
//         break;
//     case Device::Screen:
//         return findDefaultInputFormat(devman->screenInputs);
//         break;
//     default:
//         assert(0 && "unknown input device type");
//         break;
//     }
//
//     return nullptr;
// }
//
//
// AVOutputFormat* getDefaultOutputFormat(DeviceManager* devman, Device::Type type)
// {
//     switch (type) {
//     case Device::Speaker:
//         return findDefaultOutputFormat(devman->speakerOutputs);
//         break;
//     default:
//         assert(0 && "unknown output device type");
//         break;
//     }
//
//     WarnL << "No valid output format found" << endl;
//     return nullptr;
// }


bool enumerateDeviceList(AVFormatContext* ctx, std::vector<av::Device>& devices, Device::Type type)
{
    AVDeviceInfoList* devlist = nullptr;

    // List the devices for this context
    avdevice_list_devices(ctx, &devlist);
    if (!devlist) {
        WarnL << "Cannot list system devices";
        goto cleanup;
    }

    devices.clear();
    for (int i = 0; i < devlist->nb_devices; i++) {
        auto dev = devlist->devices[i];
        devices.push_back(
            av::Device(type, dev->device_name, dev->device_description));
    }

cleanup:
    if (devlist)
        avdevice_free_list_devices(&devlist);

    return !devices.empty();
}


bool getInputDeviceList(const std::vector<std::string>& inputs, std::vector<av::Device>& devices, Device::Type type)
{
    AVFormatContext* ctx = nullptr;
    AVInputFormat* iformat = nullptr;

    iformat = findDefaultInputFormat(inputs);
    if (!iformat) {
        assert(0);
        return false;
    }

    // Alloc an input device context
    if (!(ctx = avformat_alloc_context())) {
        assert(0);
        goto cleanup;
    }

    if (!iformat->priv_class || !AV_IS_INPUT_DEVICE(iformat->priv_class->category)) {
        assert(0 && "not an input device");
        goto cleanup;
    }

    ctx->iformat = iformat;
    if (ctx->iformat->priv_data_size > 0) {
        ctx->priv_data = av_mallocz(ctx->iformat->priv_data_size);
        if (!ctx->priv_data) {
            assert(0);
            goto cleanup;
        }
        if (ctx->iformat->priv_class) {
            *(const AVClass**)ctx->priv_data= ctx->iformat->priv_class;
            av_opt_set_defaults(ctx->priv_data);
        }
    }
    else {
        ctx->priv_data = nullptr;
    }

    enumerateDeviceList(ctx, devices, type);

cleanup:
    if (ctx)
        avformat_free_context(ctx);

    return !devices.empty();
}


bool getOutputDeviceList(const std::vector<std::string>& outputs, std::vector<av::Device>& devices, Device::Type type)
{
    AVFormatContext* ctx = nullptr;
    AVOutputFormat* oformat = nullptr;

    oformat = findDefaultOutputFormat(outputs);
    if (!oformat) {
        assert(0);
        return false;
    }

    // Alloc an input device context
    if (!(ctx = avformat_alloc_context())) {
        assert(0);
        goto cleanup;
    }

    if (!oformat->priv_class || !AV_IS_OUTPUT_DEVICE(oformat->priv_class->category)) {
        assert(0 && "not an output device");
        goto cleanup;
    }

    ctx->oformat = oformat;
    if (ctx->oformat->priv_data_size > 0) {
        ctx->priv_data = av_mallocz(ctx->oformat->priv_data_size);
        if (!ctx->priv_data) {
            assert(0);
            goto cleanup;
        }
        if (ctx->oformat->priv_class) {
            *(const AVClass**)ctx->priv_data = ctx->oformat->priv_class;
            av_opt_set_defaults(ctx->priv_data);
        }
    }
    else {
        ctx->priv_data = nullptr;
    }

    enumerateDeviceList(ctx, devices, type);

cleanup:
    if (ctx)
        avformat_free_context(ctx);

    return !devices.empty();
}


void printDevices(std::ostream& ost, std::vector<Device>& devs)
{
    for (std::size_t i = 0; i < devs.size(); ++i) {
        ost << '\t';
        devs[i].print(ost);
        ost << endl;
    }
}


} // namespace internal


//
// Device Manager
//


static Singleton<DeviceManager> singleton;


DeviceManager& DeviceManager::instance()
{
    return *singleton.get();
}


void DeviceManager::shutdown()
{
    singleton.destroy();
}


DeviceManager::DeviceManager() :
    cameraInputs(SCY_CAMERA_INPUTS),
    screenInputs(SCY_SCREEN_INPUTS),
    microphoneInputs(SCY_MICROPHONE_INPUTS),
    speakerOutputs(SCY_SPEAKER_OUTPUTS),
    _watcher(nullptr)
{
}


DeviceManager::~DeviceManager()
{
}


bool DeviceManager::getCameras(std::vector<Device>& devices)
{
    return internal::getInputDeviceList(cameraInputs, devices, Device::Camera);
}


bool DeviceManager::getMicrophones(std::vector<Device>& devices)
{
    return internal::getInputDeviceList(microphoneInputs, devices, Device::Microphone);
}


bool DeviceManager::getSpeakers(std::vector<Device>& devices)
{
    return internal::getOutputDeviceList(speakerOutputs, devices, Device::Speaker);
}


bool DeviceManager::findCamera(const std::string& name, Device& device)
{
    std::vector<Device> devices;
    if (getCameras(devices)) {
        for (auto& dev : devices) {
            if (dev.id == name || dev.name == name) {
                device = dev;
                return true;
            }
        }
    }

    return false;
}


bool DeviceManager::findMicrophone(const std::string& name, Device& device)
{
    std::vector<Device> devices;
    if (getMicrophones(devices)) {
        for (auto& dev : devices) {
            if (dev.id == name || dev.name == name) {
                device = dev;
                return true;
            }
        }
    }

    return false;
}


bool DeviceManager::findSpeaker(const std::string& name, Device& device)
{
    std::vector<Device> devices;
    if (getSpeakers(devices)) {
        for (auto& dev : devices) {
            if (dev.id == name || dev.name == name) {
                device = dev;
                return true;
            }
        }
    }

    return false;
}


bool DeviceManager::getDefaultCamera(Device& device)
{
    std::vector<Device> devices;
    if (getCameras(devices)) {
        device = devices[0];
        return true;
    }

    return false;
}


bool DeviceManager::getDefaultMicrophone(Device& device)
{
    std::vector<Device> devices;
    if (getMicrophones(devices)) {
        device = devices[0];
        return true;
    }

    return false;
}


bool DeviceManager::getDefaultSpeaker(Device& device)
{
    std::vector<Device> devices;
    if (getSpeakers(devices)) {
        device = devices[0];
        return true;
    }

    return false;
}


AVInputFormat* DeviceManager::findCameraInputFormat()
{
    return internal::findDefaultInputFormat(cameraInputs);
}


AVInputFormat* DeviceManager::findMicrophoneInputFormat()
{
    return internal::findDefaultInputFormat(microphoneInputs);
}


AVOutputFormat* DeviceManager::findSpeakerOutputFormat()
{
    return internal::findDefaultOutputFormat(speakerOutputs);
}


int DeviceManager::getCapabilities()
{
    std::vector<Device> devices;
    int caps = VIDEO_RECV;
    if (getMicrophones(devices)) {
        caps |= AUDIO_SEND;
    }
    if (getSpeakers(devices)) {
        caps |= AUDIO_RECV;
    }
    if (getCameras(devices)) {
        caps |= VIDEO_SEND;
    }
    return caps;
}


void DeviceManager::setWatcher(DeviceWatcher* watcher)
{
    _watcher.reset(watcher);
}


DeviceWatcher* DeviceManager::watcher()
{
    return _watcher.get();
}


void DeviceManager::print(std::ostream& ost)
{
    std::vector<Device> devs;

    ost << "Video capture devices: " << endl;
    if (getCameras(devs)) {
        internal::printDevices(ost, devs);
    }
    else {
        ost << "\tNone" << endl;
    }

    ost << "Audio input devices: " << endl;
    if (getMicrophones(devs)) {
        internal::printDevices(ost, devs);
    }
    else {
        ost << "\tNone" << endl;
    }

    ost << "Audio output devices: " << endl;
    if (getSpeakers(devs)) {
        internal::printDevices(ost, devs);
    }
    else {
        ost << "\tNone" << endl;
    }
}


} } // namespace scy::av
