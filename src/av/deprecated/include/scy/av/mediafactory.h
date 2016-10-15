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


#ifndef SCY_AV_MediaFactory_H
#define SCY_AV_MediaFactory_H


#include "scy/av/types.h"
#include "scy/av/mediacapture.h"
#include "scy/av/mediacapture.h"
#include "scy/av/formatregistry.h"
#include "scy/mutex.h"

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


class MediaFactory
    /// The MediaFactory class is a singleton manager for audio/video
    /// captures, device enumeration and encoder media formats.
{
public:
    VideoCapture::Ptr createVideoCapture(int deviceId);
        // Creates a VideoCapture instance for given device ID.
        //
        // If the VideoCapture already exists for this camera then this method
        // can be used to create VideoCaptures in any thread.
        //
        // If the VideoCapture has not been created for this camera yet it will
        // be created now, but take case since VideoCapture instances should
        // only be initialized from the main thread (OpenCV limitation).
        // You can also lazy load video cameras using loadVideoCaptures()

    VideoCapture::Ptr createFileCapture(const std::string& file);
        // Creates a VideoCapture from given source file.
        // File captures can be created in any thread.

    AudioCapture::Ptr createAudioCapture(int deviceId,
        int channels = DEFAULT_AUDIO_CHANNELS,
        int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE,
        RtAudioFormat format = RTAUDIO_SINT16);
        // Creates an AudioCapture from given options.

    void loadVideoCaptures();
        // Preloads a VideoCapture instance for each available camera.
        // This method MUST be called from the main thread.
        // This method can be called from the main thread to lazy load
        // video device captures. Alternatively you can call createVideoCapture()
        // This will ensure captures are always available to the
        // application using createVideoCapture(), from any thread.

    void unloadVideoCaptures();
        // Destroys all managed VideoCapture instances.

    void reloadFailedVideoCaptures();
        // Reloads video captures that may have failed or been unplugged.
        // The original VideoCapture instance is not deleted, just reused.
        // This method MUST be called from the main thread.

    Signal<const VideoCapture::Ptr&> VideoCaptureLoaded;
        // Siganls when a video capture is loaded.

    Signal<const VideoCapture::Ptr&> VideoCaptureError;
        // Siganls when a video capture fails, or is unplugged.

    std::map<int, VideoCapture::Ptr> videoCaptures() const;

    IDeviceManager& devices();
        // Returns the device manager instance.

    FormatRegistry& formats();
        // Returns all registered media formats.

    static MediaFactory& instance();
        // Returns the default MediaFactory singleton.

    static void shutdown();
        // Shuts down the MediaFactory and deletes the singleton instance.

protected:
    MediaFactory();
    MediaFactory(MediaFactory const&){};
    //MediaFactory& operator=(MediaFactory const&){};
    ~MediaFactory();

    void onVideoCaptureError(void*, const scy::Error& err);

    //VideoCapture::Ptr getVideoCapture(int deviceId);

    friend class Singleton<MediaFactory>;
    friend class VideoCapture;

    mutable Mutex _mutex;

    IDeviceManager* _devices;
    FormatRegistry    _formats;
    //bool _hasFailedVideoCaptures;
    std::map<int, VideoCapture::Ptr> _videoCaptures;
};


} } // namespace scy::av


#endif
#endif
