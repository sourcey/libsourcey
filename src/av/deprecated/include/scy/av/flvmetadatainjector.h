///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_FLVMetadataInjector_H
#define SCY_AV_FLVMetadataInjector_H


#include "scy/av/format.h"
#include "scy/av/fpscounter.h"
#include "scy/av/types.h"
#include "scy/byteorder.h"
#include "scy/packetstream.h"
#include "scy/signal.h"
#include <sstream>


namespace scy {
namespace av {

/// This class implements a packetizer which appends correct
/// stream headers and modifies the timestamp of FLV packets
/// so Adobe's Flash Player will play our videos mid-stream.
///
/// This adapter is useful for multicast situations where we
/// don't have the option of restarting the encoder stream.
class FLVMetadataInjector : public IPacketizer
{
public:
    enum AMFDataType
    {
        AMF_DATA_TYPE_NUMBER = 0x00,
        AMF_DATA_TYPE_BOOL = 0x01,
        AMF_DATA_TYPE_STRING = 0x02,
        AMF_DATA_TYPE_OBJECT = 0x03,
        AMF_DATA_TYPE_NULL = 0x05,
        AMF_DATA_TYPE_UNDEFINED = 0x06,
        AMF_DATA_TYPE_REFERENCE = 0x07,
        AMF_DATA_TYPE_MIXEDARRAY = 0x08,
        AMF_DATA_TYPE_OBJECT_END = 0x09,
        AMF_DATA_TYPE_ARRAY = 0x0a,
        AMF_DATA_TYPE_DATE = 0x0b,
        AMF_DATA_TYPE_LONG_STRING = 0x0c,
        AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
    };

    enum
    {
        FLV_TAG_TYPE_AUDIO = 0x08,
        FLV_TAG_TYPE_VIDEO = 0x09,
        FLV_TAG_TYPE_SCRIPT = 0x12,
    };

    enum
    {
        FLV_FRAME_KEY = 1 << 4,
        FLV_FRAME_INTER = 2 << 4,
        FLV_FRAME_DISP_INTER = 3 << 4,
    };

    IPacketizer(this->emitter), _format(format), _initial(true),
        _modifyingStream(false), _waitingForKeyframe(false),
        _timestampOffset(0) FLVMetadataInjector(const Format& format)
        :
    {
        traceL("FLVMetadataInjector", this) << "Create" << std::endl;
    } /// This method is called by the Packet Stream
    /// whenever the stream is restarted.
    virtual void onStreamStateChange(const PacketStreamState& state)
    {
        traceL("FLVMetadataInjector", this) << "Stream state change: " << state
                                            << std::endl;

        switch (state.id()) {
            case PacketStreamState::Active:
                _initial = true;
                _modifyingStream = false;
                _waitingForKeyframe = false;
                _timestampOffset = 0;
                break;
        }

        IPacketizer::onStreamStateChange(state);
    }

    virtual void process(IPacket& packet)
    {
        av::MediaPacket* mpacket = dynamic_cast<av::MediaPacket*>(&packet);
        if (mpacket && mpacket->size() > 15) {

            // Read the first packet to determine weather or not
            // we need to generate and inject custom metadata.
            if (_initial && !_modifyingStream) {
                // Buffer buf;
                // packet.write(buf);
                _modifyingStream = true; //! isFLVHeader(buf);
                _waitingForKeyframe = _modifyingStream;
                _timestampOffset = 0;
                _initial = false;
            }

            // Modify the stream only if required. This involves
            // dropping all packets until we receive the first
            // keyframe, and prepending custom FLV headers.
            if (_modifyingStream) {

                // Wait for the first keyframe...
                if (_waitingForKeyframe) {

                    // Drop all frames until we receive the first keyframe.
                    // fastIsFLVHeader(reinterpret_cast<char*>(mpacket->data())
                    if (!fastIsFLVKeyFrame(mpacket->data())) {
                        traceL("FLVMetadataInjector", this)
                            << "Waiting for keyframe, dropping packet"
                            << std::endl;
                        return;
                    }

                    // Create and dispatch our custom header.
                    _waitingForKeyframe = false;
                    traceL("FLVMetadataInjector", this)
                        << "Got keyframe, prepending headers" << std::endl;
                    // Buffer flvHeader(512);
                    std::vector<char> flvHeader(512);
                    BitWriter writer(flvHeader);
                    writeFLVHeader(writer);

                    MediaPacket opacket((std::uint8_t*)flvHeader.data(),
                                        writer.position());
                    emit(opacket);
                }

                // A correct timetsamp value must be sent to the flash
                // player otherwise payback will be jerky or delayed.
                _fpsCounter.tick();

// Generate a timestamp based on frame rate and number.
// std::uint32_t timestamp = static_cast<std::uint32_t>((1000.0 /
// _fpsCounter.fps) * _fpsCounter.frames);

// Update the output packet timestamp.
// fastUpdateTimestamp(mpacket->data(), timestamp);

#if 0
                BitReader reader1(mpacket->data(), mpacket->size());
                dumpFLVTags(reader1);
#endif

                // Dispatch the modified packet...
                traceL("FLVMetadataInjector", this) << "Emit modified packet"
                                                    << std::endl;
                emit(*mpacket);
                return;
            }
        } /// Just proxy the packet if no modification is required.
        traceL("FLVMetadataInjector", this) << "Proxy packet" << std::endl;
        emit(packet);
    } /// Updates the timestamp in the given FLV tag buffer.
    /// No more need to copy data with this method.
    /// Caution: this method does not check buffer size.
    virtual void fastUpdateTimestamp(char* buf, std::uint32_t timestamp)
    {
        std::uint32_t val = hostToNetwork32(timestamp);

        // traceL("FLVMetadataInjector", this) << "Updating timestamp: "    //
        // << "\n\tTimestamp: " << timestamp
        ///    << "\n\tTimestamp Val: " << val
        ///    << "\n\tFrame Number: " << _fpsCounter.frames
        ///    << "\n\tFrame Rate: " << _fpsCounter.fps
        ///    << std::endl;

        std::memcpy(buf + 4, reinterpret_cast<const char*>(&val) + 1, 3);
    } /// Caution: this method does not check buffer size.
    virtual bool fastIsFLVHeader(char* buf)
    {
        return strncmp(buf, "FLV", 3) == 0;
    } /// Caution: this method does not check buffer size.
    virtual bool fastIsFLVKeyFrame(char* buf)
    {
        std::uint8_t flags = buf[11];
        return (flags & FLV_FRAME_KEY) == FLV_FRAME_KEY;
    }

    virtual void writeFLVHeader(BitWriter& writer)
    {
        ///
        /// FLV Header
        writer.put("FLV", 3);
        writer.putU8(0x01);
        writer.putU8(((_format.video.enabled) ? 1 : 0) |
                     ((_format.audio.enabled) ? 4 : 0));
        writer.putU32(0x09);

        writer.putU32(0); // previous tag size

        ///
        /// FLV Metadata Object
        writer.putU8(FLV_TAG_TYPE_SCRIPT);
        int dataSizePos = writer.position(); // - offset;
        writer.putU24(0); // size of data part (sum of all parts below)
        writer.putU24(0); // time stamp
        writer.putU32(0); // reserved

        int dataStartPos = writer.position(); // - offset;

        writer.putU8(AMF_DATA_TYPE_STRING); // AMF_DATA_TYPE_STRING
        writeAMFSring(writer, "onMetaData");

        writer.putU8(AMF_DATA_TYPE_MIXEDARRAY);
        writer.putU32(2 + // number of elements in array
                      (_format.video.enabled ? 5 : 0) +
                      (_format.audio.enabled ? 5 : 0));

        writeAMFSring(writer, "duration");
        writeAMFDouble(writer, 0);

        if (_format.video.enabled) {
            writeAMFSring(writer, "width");
            writeAMFDouble(writer, _format.video.width);

            writeAMFSring(writer, "height");
            writeAMFDouble(writer, _format.video.height);

            // writeAMFSring(writer, "videodatarate");
            // writeAMFDouble(writer, _format.video.bitRate / 1024.0);

            // writeAMFSring(writer, "framerate");
            // writeAMFDouble(writer, _format.video.fps);

            // Not necessary for playback..
            // writeAMFSring(writer, "videocodecid");
            // writeAMFDouble(writer, 2); // FIXME: get FLV Codec ID from FFMpeg
            // ID
        }

        if (_format.audio.enabled) {
            writeAMFSring(writer, "audiodatarate");
            writeAMFDouble(writer, _format.audio.bitRate / 1024.0);

            writeAMFSring(writer, "audiosamplerate");
            writeAMFDouble(writer, _format.audio.sampleRate);

            writeAMFSring(writer, "audiosamplesize");
            writeAMFDouble(writer, 16); // FIXME: audio_enc->codec_id ==
                                        // AV_CODEC_ID_PCM_U8 ? 8 : 16

            writeAMFSring(writer, "stereo");
            writeAMFBool(writer, _format.audio.channels == 2);

            // Not necessary for playback..
            // writeAMFSring(buf, "audiocodecid");
            // writeAMFDouble(buf, 0/* audio_enc->codec_tag*/); // FIXME: get
            // FLV Codec ID from FFMpeg ID
        }

        writeAMFSring(writer, "filesize");
        writeAMFDouble(writer, 0); // delayed write

        writer.put("", 1);
        writer.putU8(AMF_DATA_TYPE_OBJECT_END); /// Write data size
        int dataSize = writer.position() - dataStartPos;
        writer.updateU24(dataSize, dataSizePos); /// Write tag size
        writer.putU32(dataSize + 11);

        traceL("FLVMetadataInjector", this) << "FLV Header:"
                                            //<< "\n\tType: " << (int)tagType
                                            << "\n\tData Size: " << dataSize
                                            //<< "\n\tTimestamp: " << timestamp
                                            //<< "\n\tStream ID: " << streamId
                                            << std::endl;
    }

    static bool dumpFLVTags(BitReader& reader)
    {
        bool result = false;

        std::uint8_t tagType;
        std::uint32_t dataSize;
        std::uint32_t timestamp;
        std::uint8_t timestampExtended;
        std::uint32_t streamId;
        std::uint8_t flags;
        std::uint32_t previousTagSize;

        do {
            if (reader.available() < 12)
                break;

            reader.getU8(tagType);
            if (tagType != FLV_TAG_TYPE_AUDIO && // audio
                tagType != FLV_TAG_TYPE_VIDEO && // video
                tagType != FLV_TAG_TYPE_SCRIPT)  // script
                break;

            reader.getU24(dataSize);
            if (dataSize < 100)
                break;

            reader.getU24(timestamp);
            if (timestamp < 0)
                break;

            reader.getU8(timestampExtended);

            reader.getU24(streamId);
            if (streamId != 0)
                break;

            // Start of data size bytes
            int dataStartPos = reader.position();

            reader.getU8(flags);

            bool isKeyFrame = false;
            bool isInterFrame = false;
            // bool isDispInterFrame = false;
            switch (tagType) {
                case FLV_TAG_TYPE_AUDIO:
                    break;

                case FLV_TAG_TYPE_VIDEO:
                    isKeyFrame = (flags & FLV_FRAME_KEY) == FLV_FRAME_KEY;
                    isInterFrame = (flags & FLV_FRAME_INTER) == FLV_FRAME_INTER;
                    // isDispInterFrame = (flags & FLV_FRAME_DISP_INTER) ==
                    // FLV_FRAME_DISP_INTER;
                    break;

                case FLV_TAG_TYPE_SCRIPT:
                    break;

                default:
                    break;
            }

            // Read to the end of the current tag.
            reader.seek(dataStartPos + dataSize);
            reader.getU32(previousTagSize);
            if (previousTagSize == 0) {
                assert(false);
                break;
            }

            traceL("FLVMetadataInjector")
                << "FLV Tag:"
                << "\n\tType: " << (int)tagType
                << "\n\tTag Size: " << previousTagSize
                << "\n\tData Size: " << dataSize
                << "\n\tTimestamp: " << timestamp
                //<< "\n\tTimestamp Extended: " << (int)timestampExtended
                << "\n\tKey Frame: " << isKeyFrame
                << "\n\tInter Frame: " << isInterFrame
                //<< "\n\tDisp Inter Frame: " << isDispInterFrame
                //<< "\n\tFlags: " << (int)flags
                //<< "\n\tStream ID: " << streamId
                << std::endl;

            result = true;

        } while (0);

        return result;
    }

    std::int64_t doubleToInt(double d)
    {
        int e;
        if (!d)
            return 0;
        else if (d - d)
            return 0x7FF0000000000000LL + ((std::int64_t)(d < 0) << 63) +
                   (d != d);
        d = frexp(d, &e);
        return (std::int64_t)(d < 0) << 63 | (e + 1022LL) << 52 |
               (std::int64_t)((fabs(d) - 0.5) * (1LL << 53));
    }


    ///
    /// AMF Helpers
    //

    virtual void writeAMFSring(BitWriter& writer, const char* val)
    {
        std::uint16_t len = strlen(val);
        writer.putU16(len);
        writer.put(val, len);
    }

    virtual void writeAMFDouble(BitWriter& writer, double val)
    {
#if WIN32 // The implementation is not perfect, but it's sufficient for our
          // needs.
        if ((val > double(_I64_MAX)) || (val < double(_I64_MIN))) {
            traceL("FLVMetadataInjector") << "Double to int truncated"
                                          << std::endl;
            assert(0);
        }
#endif

        writer.putU8(AMF_DATA_TYPE_NUMBER); // AMF_DATA_TYPE_NUMBER
        // writer.putU64(std::int64_t(val));
        writer.putU64(doubleToInt(val));
    }

    virtual void writeAMFBool(BitWriter& writer, bool val)
    {
        writer.putU8(AMF_DATA_TYPE_BOOL); // AMF_DATA_TYPE_NUMBER
        writer.putU8(val ? 1 : 0);
    }

    PacketSignal emitter;

protected:
    Format _format;
    bool _initial;
    bool _modifyingStream;
    bool _waitingForKeyframe;
    std::uint32_t _timestampOffset;
    legacy::FPSCounter _fpsCounter; // Need legacy counter for smooth playback
};


} // namespace av
} // namespace scy


#endif


/*
virtual void updateTimestamp(Buffer& buf, std::uint32_t timestamp)
{
/// Note: The buffer must be positioned at
/// the start of the tag.
    int offset = buf.position();
    if (buf.available() < offset + 4) {
        errorL("FLVMetadataInjector", this) << "The FLV tag buffer is too
small." << std::endl;
        return;
    }

    traceL("FLVMetadataInjector", this) << "Updating timestamp: "
        << "\n\tTimestamp: " << timestamp
        << "\n\tFrame Number: " << _fpsCounter.frames
        << "\n\tFrame Rate: " << _fpsCounter.fps
        << std::endl;

    buf.updateU24(timestamp, offset + 4);
}

virtual bool isFLVHeader(BitWriter& writer)
{
    std::string signature;
    buf.get(signature, 3);
    return signature == "FLV";
}

virtual bool isFLVKeyFrame(BitWriter& writer)
{
    if (buf.available() < 100)
        return false;

    int offset = buf.position();

    //buf.getU8(tagType);
    //if (tagType != FLV_TAG_TYPE_VIDEO)    //    return false;
    //std::uint8_t tagType;


    std::uint8_t flags;
    buf.position(11);
    buf.getU8(flags);

    buf.position(offset);
    return (flags & FLV_FRAME_KEY) == FLV_FRAME_KEY;
}
*/
