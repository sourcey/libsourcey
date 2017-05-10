///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_MediaFactory_H
#define SCY_AV_MediaFactory_H


#include "scy/av/formatregistry.h"
#include "scy/av/mediacapture.h"
#include "scy/av/mediacapture.h"
#include "scy/av/types.h"
#include <mutex>

#if defined(HAVE_OPENCV) && defined(HAVE_RTAUDIO)

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <map>

#include "scy/av/devicemanager.h"
#if !defined(USING_FAKE_DeviceManager) && defined(WIN32)
#include "scy/av/devicemanager_win32.h"
#elif !defined(USING_FAKE_DeviceManager) && defined(MACOS)
#include "scy/av/devicemanager_mac.h"
#elif !defined(USING_FAKE_DeviceManager) && defined(LINUX)
#include "scy/av/devicemanager_linux.h"
#else
#include "scy/av/devicemanager_fake.h"
#endif


namespace scy {
namespace av {

/// The MediaFactory class is a singleton manager for audio/video
/// captures, device enumeration and encoder media formats.
class MediaFactory
{
public:
    /// Creates a VideoCapture instance for given device ID.
    ///
    /// If the VideoCapture already exists for this camera then this method
    /// can be used to create VideoCaptures in any thread.
    ///
    /// If the VideoCapture has not been created for this camera yet it will
    /// be created now, but take case since VideoCapture instances should
    /// only be initialized from the main thread (OpenCV limitation).
    /// You can also lazy load video cameras using loadVideoCaptures()
    VideoCapture::Ptr createVideoCapture(int deviceId);

    /// Creates a VideoCapture from given source file.
    /// File captures can be created in any thread.
    VideoCapture::Ptr createFileCapture(const std::string& file);


    AudioCapture::Ptr createAudioCapture(
        int deviceId, int channels = DEFAULT_AUDIO_CHANNELS,
        int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE, // Creates an AudioCapture
                                                    // from given options.
        RtAudioFormat format = RTAUDIO_SINT16);

    /// Preloads a VideoCapture instance for each available camera.
    /// This method MUST be called from the main thread.
    /// This method can be called from the main thread to lazy load
    /// video device captures. Alternatively you can call createVideoCapture()
    /// // This will ensure captures are always available to the
    /// application using createVideoCapture(), from any thread.
    void loadVideoCaptures();

    /// Destroys all managed VideoCapture instances.
    void unloadVideoCaptures();

    /// Reloads video captures that may have failed or been unplugged.
    /// The original VideoCapture instance is not deleted, just reused.
    /// This method MUST be called from the main thread.
    void reloadFailedVideoCaptures();

    /// Siganls when a video capture is loaded.
    Signal<void(const VideoCapture::Ptr&)> VideoCaptureLoaded;

    /// Siganls when a video capture fails, or is unplugged.
    Signal<void(const VideoCapture::Ptr&)> VideoCaptureError;


    std::map<int, VideoCapture::Ptr>
    videoCaptures() const; /// Returns the device manager instance.
    IDeviceManager& devices();

    /// Returns all registered media formats.
    FormatRegistry& formats();

    /// Returns the default MediaFactory singleton.
    static MediaFactory& instance();

    /// Shuts down the MediaFactory and deletes the singleton instance.
    static void shutdown();


protected:
    MediaFactory();
    MediaFactory(MediaFactory const&){};
    // MediaFactory& operator=(MediaFactory const&){};
    ~MediaFactory();

    void onVideoCaptureError(void*, const scy::Error& err);

    // VideoCapture::Ptr getVideoCapture(int deviceId);

    friend class Singleton<MediaFactory>;
    friend class VideoCapture;

    mutable std::mutex _mutex;

    IDeviceManager* _devices;
    FormatRegistry _formats;
    // bool _hasFailedVideoCaptures;
    std::map<int, VideoCapture::Ptr> _videoCaptures;
};


} // namespace av
} // namespace scy


#endif
#endif
