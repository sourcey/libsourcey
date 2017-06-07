///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_MultiplexPacketEncoder_H
#define SCY_AV_MultiplexPacketEncoder_H


#include "scy/base.h"
#ifdef HAVE_FFMPEG
#include "scy/av/multiplexencoder.h"
#include "scy/packetstream.h"


namespace scy {
namespace av {


/// Encodes and multiplexes a realtime video stream form
/// audio / video capture sources.
/// FFmpeg is used for encoding.
class AV_API MultiplexPacketEncoder : public MultiplexEncoder, public PacketProcessor
{
public:
    MultiplexPacketEncoder(const EncoderOptions& options = EncoderOptions());
    virtual ~MultiplexPacketEncoder();

    virtual void encode(VideoPacket& packet);
    virtual void encode(AudioPacket& packet);

    virtual bool accepts(IPacket* packet) override;
    virtual void process(IPacket& packet) override;

protected:
    virtual void onStreamStateChange(const PacketStreamState& state) override;

    friend class PacketStream;

    mutable std::mutex _mutex;
};


#if 0
//
// PTS Calculator
//

/// Helper class which calculates PTS values for a live source
struct PTSCalculator
{
    AVRational timeBase;
    clock_t frameTime;
    double frameDuration;
    double frameDiff;

    int64_t currentPTS;
    int64_t lastPTS;

    PTSCalculator() {
        reset();
    }

    void reset() {
        lastPTS = 0;
        currentPTS = 0;
        frameTime = 0;
        frameDuration = 0;
        frameDiff = 0;
    }

    void log() {
        Timestamp ts;
        debugL("PTSCalculator", this) << "Values:"
            << "\n\tCurrent PTS: " << currentPTS
            << "\n\tLast PTS: " << lastPTS
            << "\n\tFrame Duration: " << frameDuration
            << "\n\tFrame Diff: " << frameDiff
            << "\n\tFrame Time: " << frameTime
            << "\n\tTime Base: " << timeBase.den << ": " << timeBase.num
            << std::endl;
    }

    int64_t tick() {
    /// Initializing
        if (frameTime == 0) {
            assert(!frameDuration);
            frameTime = clock();
            currentPTS = 1;
        }    /// Updating
        else {
            frameDuration = (double)(clock() - frameTime) / CLOCKS_PER_SEC;
            frameTime = clock();
            frameDiff = timeBase.den/(timeBase.num/(frameDuration));
            currentPTS = lastPTS + frameDiff;
        }

        log();

        assert(currentPTS > lastPTS);
        lastPTS = currentPTS;
        return currentPTS;
    }
};
#endif


} // namespace av
} // namespace scy


#endif
#endif
