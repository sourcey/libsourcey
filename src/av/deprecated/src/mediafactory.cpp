///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/mediafactory.h"
#include "scy/logger.h"


#if defined(HAVE_OPENCV) && defined(HAVE_RTAUDIO)


using std::endl;


namespace scy {
namespace av {


static Singleton<MediaFactory> singleton;


MediaFactory& MediaFactory::instance()
{
    return *singleton.get();
}


void MediaFactory::shutdown()
{
    singleton.destroy();
}


MediaFactory::MediaFactory()
{
    _devices = DeviceManagerFactory::create();
    _devices->initialize();
    //_devices->DevicesChanged += sdelegate(this,
    //&MediaFactory::onDevicesChanged);
}


MediaFactory::~MediaFactory()
{
    if (_devices) {
        //_devices->DevicesChanged -= sdelegate(this,
        //&MediaFactory::onDevicesChanged);
        _devices->uninitialize();
        delete _devices;
    }
}


IDeviceManager& MediaFactory::devices()
{
    std::lock_guard<std::mutex> guard(_mutex);
    return *_devices;
}


FormatRegistry& MediaFactory::formats()
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _formats;
}


void MediaFactory::loadVideoCaptures()
{
    LDebug("Loading video captures")
    assert(Thread::mainID == Thread::currentID());

    // Initialize a VideoCapture object for each available device.
    // The video capture object will begin capturing frames when it's
    // reference count becomes positive.
    std::vector<Device> devs;
    devices().getCameras(devs);
    for (std::size_t i = 0; i < devs.size(); ++i) {
        try {
            createVideoCapture(devs[0].id);
        } catch (std::exception& exc) {
            SError << "Cannot load video capture: " << devs[0].id << ": "
                   << exc.what() << endl;
        }
    }
}


void MediaFactory::reloadFailedVideoCaptures()
{
    LDebug("Reloading failed video captures")
    assert(Thread::mainID == Thread::currentID());

    // Loop through captures and attempt to reopen any
    // that may have been unplugged
    auto videoCaptures = this->videoCaptures();
    for (auto& kv : videoCaptures) {
        if (kv.second->error().any()) {
            STrace << "Reloading capture " << kv.second->deviceId() << ": "
                   << kv.second->error() << endl;
            try {
                kv.second->open();
                kv.second->start();

                // Manually emit the capture loaded signal if the
                // capture was successfully reloaded
                if (!kv.second->error().any()) {
                    VideoCaptureLoaded.emit(kv.second);
                }
            } catch (std::exception& exc) {
                SError << "Capture initialization error: " << exc.what()
                       << endl;
            }
        }
    }
}


std::map<int, VideoCapture::Ptr> MediaFactory::videoCaptures() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _videoCaptures;
}


void MediaFactory::unloadVideoCaptures()
{
    std::lock_guard<std::mutex> guard(_mutex);
    _videoCaptures.clear();
}


VideoCapture::Ptr
MediaFactory::createVideoCapture(int deviceId) //, unsigned flags
{
    LTrace("Creating video capture: ", deviceId)

    if (deviceId < 0)
        throw std::runtime_error("Invalid video device ID");

    std::lock_guard<std::mutex> guard(_mutex);

    auto it = _videoCaptures.find(deviceId);
    if (it != _videoCaptures.end())
        return it->second;

    auto capture = std::make_shared<VideoCapture>(deviceId);
    _videoCaptures[deviceId] = capture;
    VideoCaptureLoaded.emit(capture);

    // Listen for errors.
    // NOTE: The capture is opened and started in the constructor,
    // so exceptions thrown during startup will not be handled
    // via this callback.
    capture->Error += sdelegate(this, &MediaFactory::onVideoCaptureError);
    return capture;
}


void MediaFactory::onVideoCaptureError(void* sender, const scy::Error& err)
{
    auto capture = reinterpret_cast<VideoCapture*>(sender);
    auto videoCaptures = this->videoCaptures();
    auto it = videoCaptures.find(capture->deviceId());
    if (it != videoCaptures.end()) {
        VideoCaptureError.emit(it->second);
    } else
        assert(0);
}


VideoCapture::Ptr MediaFactory::createFileCapture(const std::string& file)
{
    LTrace("Create video file capture: ", file)

    return std::make_shared<VideoCapture>(file);
}


AudioCapture::Ptr MediaFactory::createAudioCapture(int deviceId, int channels,
                                                   int sampleRate,
                                                   RtAudioFormat format)
{
    LTrace("Create audio capture: ", deviceId)
    if (deviceId < 0)
        throw std::runtime_error("Invalid audio device ID");

    return std::make_shared<AudioCapture>(deviceId, channels, sampleRate,
                                          format);
}


} // namespace av
} // namespace scy


#endif
