///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_VideoAnalyzer_H
#define SCY_AV_VideoAnalyzer_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/format.h"
#include "scy/av/fpscounter.h"
#include "scy/av/mediacapture.h"
#include "scy/av/videoconverter.h"
#include "scy/interface.h"
#include "scy/packetstream.h"
#include "scy/packetstream.h"
#include "scy/signal.h"
#include "scy/stateful.h"

extern "C" {
#include <libavcodec/avfft.h>
}


namespace scy {
namespace av {

/// This class provides basic AV spectrum analysis on a
/// video using the Fourier Transform algorithm.
/// Data is outputted in CSV format.
///
/// TODO:///        - Pluggable algorithms and processors
///        - Normalization (scaling) for output values
///        - Process multiple audio channels properly
///        - Inherit from PacketProcessor
class VideoAnalyzer //: public PacketProcessor
{
public:
    struct Options
    {
        std::string ifile; // The input video file.
        int rdftSize;      // Size of the FFT input array
        // bool blocking;        // Blocking mode (disable async)

        Options()
        {
            rdftSize = 1024;
            // blocking = false;
        }
    };

    struct Stream
    {
        std::string name;
        RDFTContext* rdft;
        FFTSample* rdftData;
        int rdftSize;
        int rdftBits;
        std::int64_t frames;
        int filled;

        Stream(const std::string& name, int rdftSize);
        ~Stream();

        void initialize();
        void uninitialize();

        void fft();
        // Preforms FFT on internal rdftData
    };

    struct Packet
    {
        double time;
        // double min;
        // double max;
        double value;

        Packet(
            double time = 0.0,
            double value = 0.0); //, double min = 99999.9, double max = -99999.9
    };

public:
    VideoAnalyzer(
        const VideoAnalyzer::Options& options = VideoAnalyzer::Options());
    virtual ~VideoAnalyzer(); /// Set everything up, and open the input file.
    virtual void initialize();

    /// Stop processing and free everything.
    virtual void uninitialize();

    /// Begin processing.
    virtual void start();

    /// Stop processing.
    virtual void stop();

    /// Signals on VideoAnalyzer::Packet output
    /// Raw FFT data is available via VideoAnalyzer::Stream->rdftData
    Signal<void(const VideoAnalyzer::Stream&, const VideoAnalyzer::Packet&)>
        PacketOut;

    /// Signals on analysis complete
    NullSignal Complete;


    virtual MediaCapture& reader();
    virtual Options& options();
    virtual std::string error() const;

protected:
    AVFrame* getGrayVideoFrame();

    virtual void onReadComplete(void* sender);
    virtual void onVideo(void* sender, VideoPacket& packet);
    virtual void onAudio(void* sender, AudioPacket& packet);

    const char* className() const { return "VideoAnalyzer"; }

protected:
    mutable std::mutex _mutex;

    Options _options;
    std::string _error;
    MediaCapture _reader;
    VideoAnalyzer::Stream* _video;
    VideoAnalyzer::Stream* _audio;
    VideoConverter* _videoConv;
};


// ---------------------------------------------------------------------
// FFT Helpers
// Based on http://www.codeproject.com/Articles/6855/FFT-of-waveIn-audio-signals
//
inline double CalculateFrequencyIntensity(VideoAnalyzer::Stream& stream);
inline double CalculateCentroidFrequency(VideoAnalyzer::Stream& stream);

inline double GetFrequencyIntensity(double re, double im);
inline double GetDecibels(double re, double im);
inline double GetAmplitude(double re, double im, int len);
inline double GetAmplitudeScaled(double re, double im, int len, int scale);

#ifdef SCY_WIN
inline double log2(double n);
#endif


} // namespace av
} // namespace scy


#endif
#endif


// virtual void writeCSV(const VideoAnalyzer::Packet& packet);, double time
// std::ofstream _ofile;
