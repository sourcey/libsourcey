///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_VideoCapture_H
#define SCY_AV_VideoCapture_H


#include "scy/av/format.h"
#include "scy/av/fpscounter.h"
#include "scy/av/icapture.h"
#include "scy/av/types.h"
#include "scy/bitwise.h"
#include "scy/interface.h"
#include <mutex>
#include <cstdint>

#ifdef HAVE_OPENCV
#include "opencv2/opencv.hpp"

#include <iostream>
#include <map>

#define HAVE_OPENCV_VIDEOCAPTURE


namespace scy {
namespace av {


//
// Video Capture
//

/// Class for capturing video from cameras and files using OpenCV.
/// Do not use this class directly, use VideoCapture instead.
///
/// To handle output packets listen in on the ICapture::emitter signal.
///
/// Limitations:/// OpenCV doesn't support multi-thread capturing so
/// VideoCapture
/// instances should be created in the main thread.
/// File captures do not have this limitation.
/// Also avoid creating multiple instances using the same device.
/// Instead reuse the same instance, preferably using the
/// MediaFactory interface.
///
/// Windows:/// OpenCV HighGUI DirectShow must be compiled with
/// VI_COM_MULTI_THREADED
/// defined otherwise capture there will be CoInitialize conflicts
/// with the DeviceManager.
class VideoCapture : public ICapture, public basic::Runnable
{
public:
    typedef std::shared_ptr<VideoCapture>
        Ptr; /// Creates and opens the given device.
    /// Should be created in the main thread.
    VideoCapture(int deviceId);

    /// Creates and opens the given video file.
    /// Can be created in any thread.
    VideoCapture(const std::string& filename);

    /// Destroys the VideoCapture.
    virtual ~VideoCapture();

    /// Opens the VideoCapture.
    bool open(bool whiny = true);


    virtual void start();
    virtual void stop(); /// True when the system device is open.
    bool opened() const;

    /// True when the internal thread is running.
    bool running() const;


    void getFrame(cv::Mat& frame, int width = 0, int height = 0);

    virtual void getEncoderFormat(Format& iformat);

    int deviceId() const;
    std::string filename() const;
    std::string name() const;
    const scy::Error& error() const;
    double fps() const;
    int width();
    int height();
    cv::Mat lastFrame() const;
    cv::VideoCapture&
    capture(); /// Signals that the capture is closed in error.
    Signal<void(const scy::Error&)> Error;


protected:
    cv::Mat grab();
    virtual void run();

    void setError(const std::string& error);

    friend class MediaFactory;

private:
    mutable std::mutex _mutex;

    std::string _filename; // Source file to capture from if any
    int _deviceId;         // Source device to capture from
    bool _opened;
    bool _started;
    bool _stopping;
    bool _capturing;
    cv::Mat _frame;    // Current video image
    scy::Error _error; // Error message if any
    FPSCounter _counter;
    cv::VideoCapture _capture;
    Thread _thread;
};


typedef std::map<int, VideoCapture::Ptr> VideoCaptureMap;


//
// Matrix Packet
//


class MatrixPacket : public VideoPacket
{
public:
    cv::Mat* mat; // For OpenCV generated packets.
                  // TODO: Use stream offset time instead of process time
    // for consistency with AudioCapture for realtime pts calculation

    MatrixPacket(cv::Mat* mat, double time = time::clockSecs())
        : VideoPacket(reinterpret_cast<std::uint8_t*>(mat->data),
                      mat->rows * mat->step, mat->cols, mat->rows, time)
        , mat(mat)
    {
    }

    MatrixPacket(char* data = nullptr, int size = 0, int width = 0,
                 int height = 0, double time = time::clockSecs())
        : VideoPacket(reinterpret_cast<std::uint8_t*>(data), size, width,
                      height, time)
        , mat(nullptr){};

    virtual IPacket* clone() const { return new MatrixPacket(*this); }

    virtual const char* className() const { return "MatrixPacket"; }
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_VideoCapture_H


/// @\}
