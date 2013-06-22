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


#ifndef SOURCEY_MEDIA_FLVMetadataInjector_H
#define SOURCEY_MEDIA_FLVMetadataInjector_H


#include "Sourcey/IPacketizer.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/FPSCounter.h"
#include "Sourcey/Media/Format.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Format.h"
#include <sstream>


namespace Scy {
namespace Media {


class FLVMetadataInjector: public IPacketizer
	/// This class implements a packetizer which appends correct
	/// stream headers and modifies the timestamp of FLV packets
	/// so Adobe's Flash Player will play our video mid-stream.
	/// This is useful for multicast situations where we don't 
	/// have the option of restarting the encoder stream.
{
public:
	typedef enum {
		AMF_DATA_TYPE_NUMBER      = 0x00,
		AMF_DATA_TYPE_BOOL        = 0x01,
		AMF_DATA_TYPE_STRING      = 0x02,
		AMF_DATA_TYPE_OBJECT      = 0x03,
		AMF_DATA_TYPE_NULL        = 0x05,
		AMF_DATA_TYPE_UNDEFINED   = 0x06,
		AMF_DATA_TYPE_REFERENCE   = 0x07,
		AMF_DATA_TYPE_MIXEDARRAY  = 0x08,
		AMF_DATA_TYPE_OBJECT_END  = 0x09,
		AMF_DATA_TYPE_ARRAY       = 0x0a,
		AMF_DATA_TYPE_DATE        = 0x0b,
		AMF_DATA_TYPE_LONG_STRING = 0x0c,
		AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
	} AMFDataType;
			
	enum {
		FLV_TAG_TYPE_AUDIO	= 0x08,
		FLV_TAG_TYPE_VIDEO	= 0x09,
		FLV_TAG_TYPE_SCRIPT = 0x12,
	};

	enum {
		FLV_FRAME_KEY        = 1 << 4,
		FLV_FRAME_INTER      = 2 << 4,
		FLV_FRAME_DISP_INTER = 3 << 4,
	};

	FLVMetadataInjector(const Format& format) :
		_format(format),
		_initial(true),
		_modifyingStream(false),
		_waitingForKeyframe(false),
		_timestampOffset(0)
	{
		LogDebug("FLVMetadataInjector", this) << "Creating" << std::endl;
	}
					
	virtual void onStreamStateChange(const PacketStreamState& state) 
		/// This method is called by the Packet Stream
		/// whenever the stream is restarted.
	{ 
		LogDebug("FLVMetadataInjector", this) << "Stream State Change: " << state << std::endl;
		switch (state.id()) {
		case PacketStreamState::Running:
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
		Media::MediaPacket* mpacket = dynamic_cast<Media::MediaPacket*>(&packet);		
		if (mpacket) {

			// Read the first packet to determine weather or not
			// we need to generate and inject custom metadata. 
			if (_initial && !_modifyingStream) {
				Buffer buf;
				packet.write(buf);
				_modifyingStream = true; //!isFLVHeader(buf);
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
					//fastIsFLVHeader(reinterpret_cast<char*>(mpacket->data())
					if (!fastIsFLVKeyFrame(reinterpret_cast<char*>(mpacket->data()))) {
						LogDebug("FLVMetadataInjector", this) << "Waiting for keyframe, dropping packet" << std::endl;
						return;
					}

					// Create and dispatch our custom header.
					_waitingForKeyframe = false;				
					LogDebug("FLVMetadataInjector", this) << "Got keyframe, prepending headers" << std::endl;
					Buffer flvHeader(512);
					writeFLVHeader(flvHeader);

					MediaPacket opacket(reinterpret_cast<unsigned char*>(flvHeader.bytes()), flvHeader.size());
					emit(this, opacket);
				}

				// A correct FPS value must be sent to the flash
				// player otherwise payback will be jerky or delayed.
				_fpsCounter.tick();
		
				// Generate the timestamp based on frame rate and number.
				UInt32 timestamp = static_cast<UInt32>((1000.0 / _fpsCounter.fps) * _fpsCounter.frames);		

				// Update the output packet's timestamp.
				fastUpdateTimestamp(reinterpret_cast<char*>(mpacket->data()), timestamp);	
			
				//int offset = buf.position();
				//dumpFLVTags(buf);
				//buf.position(offset);	

				// Dispatch the modified packet...
				emit(this, *mpacket);
				return;
			}
		}

		// Just proxy the packet if no modification is required.
		//LogDebug("FLVMetadataInjector", this) << "Proxy Packet" << std::endl;
		emit(this, packet);
	}
	
	virtual void fastUpdateTimestamp(char* buf, UInt32 timestamp)
		/// Updates the timestamp in the given FLV tag buffer.
		/// No more need to copy data with this method.
	{
		UInt32 val = HostToNetwork32(timestamp);
		/*
		LogDebug("FLVMetadataInjector", this) << "Updating timestamp: "
			<< "\n\tTimestamp: " << timestamp
			<< "\n\tFrame Number: " << _fpsCounter.frames
			<< "\n\tFrame Rate: " << _fpsCounter.fps
			<< std::endl;	
			*/
		std::memcpy(buf + 4, reinterpret_cast<const char*>(&val) + 1, 3);
	}

	virtual void updateTimestamp(Buffer& buf, UInt32 timestamp)
	{
		// NOTE: The buffer must be positioned at
		// the start of the tag.
		int offset = buf.position();
		if (buf.size() < offset + 4) {
			LogError("FLVMetadataInjector", this) << "The FLV tag buffer is too small." << std::endl;
			return;
		}
		
		/*
		LogDebug("FLVMetadataInjector", this) << "Updating timestamp: "
			<< "\n\tTimestamp: " << timestamp
			<< "\n\tFrame Number: " << _fpsCounter.frames
			<< "\n\tFrame Rate: " << _fpsCounter.fps
			<< std::endl;
			*/

		buf.updateUInt24(timestamp, offset + 4);
	}

	virtual bool fastIsFLVHeader(char* buf)
	{		
		return strncmp(buf, "FLV", 3) == 0;
	}

	virtual bool isFLVHeader(Buffer& buf)
	{		
		std::string signature; 
		buf.readString(signature, 3);
		return signature == "FLV";
	}

	virtual bool isFLVKeyFrame(Buffer& buf)
	{	
		if (buf.size() < 100)
			return false;
			
		int offset = buf.position();

		//UInt8 tagType; 
		//buf.readUInt8(tagType);
		//if (tagType != FLV_TAG_TYPE_VIDEO)
		//	return false;
						
		UInt8 flags;				
		buf.setPosition(11);
		buf.readUInt8(flags);	
					
		buf.setPosition(offset);
		return (flags & FLV_FRAME_KEY) == FLV_FRAME_KEY;
	}

	virtual bool fastIsFLVKeyFrame(char* buf)
	{		
		UInt8 flags = buf[11];
		return (flags & FLV_FRAME_KEY) == FLV_FRAME_KEY;
	}

	virtual void writeFLVHeader(Buffer& buf)
	{			
		int offset = buf.size();

		//
		// FLV Header
		buf.writeBytes("FLV", 3);
		buf.writeUInt8(0x01);
		buf.writeUInt8(
			((_format.video.enabled) ? 1 : 0) | 
			((_format.audio.enabled) ? 4 : 0));
		buf.writeUInt32(0x09);
			
		buf.writeUInt32(0);	 // previous tag size 

		//
		// FLV Metadata Object
		buf.writeUInt8(FLV_TAG_TYPE_SCRIPT);
		int dataSizePos = buf.size() - offset;
		buf.writeUInt24(0);	// size of data part (sum of all parts below)			
		buf.writeUInt24(0);	// time stamp
		buf.writeUInt32(0);	// reserved			

		int dataStartPos = buf.size() - offset; 

		buf.writeUInt8(AMF_DATA_TYPE_STRING);	// AMF_DATA_TYPE_STRING
		writeAMFSring(buf, "onMetaData");
	
		buf.writeUInt8(AMF_DATA_TYPE_MIXEDARRAY);
		buf.writeUInt32(2 + // number of elements in array			
			(_format.video.enabled ? 5 : 0) + 
			(_format.audio.enabled ? 5 : 0));

		writeAMFSring(buf, "duration");
		writeAMFDouble(buf, 0);
			
		if (_format.video.enabled){
			writeAMFSring(buf, "width");
			writeAMFDouble(buf, _format.video.width);

			writeAMFSring(buf, "height");
			writeAMFDouble(buf, _format.video.height);

			//writeAMFSring(buf, "videodatarate");
			//writeAMFDouble(buf, _format.video.bitRate / 1024.0);

			//writeAMFSring(buf, "framerate");
			//writeAMFDouble(buf, _format.video.fps);

			// Not necessary for playback..
			//writeAMFSring(buf, "videocodecid");
			//writeAMFDouble(buf, 2); // FIXME: get FLV Codec ID from FFMpeg ID
		}
			
		if (_format.audio.enabled){
			writeAMFSring(buf, "audiodatarate");
			writeAMFDouble(buf, _format.audio.bitRate / 1024.0);

			writeAMFSring(buf, "audiosamplerate");
			writeAMFDouble(buf, _format.audio.sampleRate);

			writeAMFSring(buf, "audiosamplesize");
			writeAMFDouble(buf, 16); //FIXME: audio_enc->codec_id == CODEC_ID_PCM_U8 ? 8 : 16

			writeAMFSring(buf, "stereo");
			writeAMFBool(buf, _format.audio.channels == 2);
			
			// Not necessary for playback..
			//writeAMFSring(buf, "audiocodecid");
			//writeAMFDouble(buf, 0/* audio_enc->codec_tag*/); // FIXME: get FLV Codec ID from FFMpeg ID
		}

		writeAMFSring(buf, "filesize");
		writeAMFDouble(buf, 0);	// delayed write
			
		buf.writeBytes("", 1);
		buf.writeUInt8(AMF_DATA_TYPE_OBJECT_END);
	
		// Write data size
		int dataSize = buf.size() - dataStartPos;
		buf.updateUInt24(dataSize, dataSizePos);
			
		// Write tag size
		buf.writeUInt32(dataSize + 11);
			
		//LogDebug() << "FLV Header:" 
		//	<< "\n\tType: " << (int)tagType
		//	<< "\n\tData Size: " << dataSize
		//	<< "\n\tTimestamp: " << timestamp
		//	<< "\n\tStream ID: " << streamId
		//	<< std::endl;
	}	

	static bool dumpFLVTags(Buffer& buf)
	{	
		bool result = false; 

		UInt8 tagType; 
		UInt32 dataSize;
		UInt32 timestamp;
		UInt8 timestampExtended;
		UInt32 streamId;
		UInt8 flags;
		UInt32 previousTagSize;		

		do {
			if (buf.size() < 12)
				break;

			buf.readUInt8(tagType);
			if (tagType != FLV_TAG_TYPE_AUDIO &&	// audio
				tagType != FLV_TAG_TYPE_VIDEO &&	// video
				tagType != FLV_TAG_TYPE_SCRIPT)		// script
				break;

			buf.readUInt24(dataSize);
			if (dataSize < 100)
				break;
				
			buf.readUInt24(timestamp);
			if (timestamp < 0)
				break;
				
			buf.readUInt8(timestampExtended);	
				
			buf.readUInt24(streamId);
			if (streamId != 0)
				break;	
				
			// Start of data size bytes
			int dataStartPos = buf.position();
			
			buf.readUInt8(flags);	
				
			bool isKeyFrame = false;
			bool isInterFrame = false;
			bool isDispInterFrame = false;
            switch (tagType)
            {
                case FLV_TAG_TYPE_AUDIO:
                    break;

                case FLV_TAG_TYPE_VIDEO:
					isKeyFrame = (flags & FLV_FRAME_KEY) == FLV_FRAME_KEY;
					isInterFrame = (flags & FLV_FRAME_INTER) == FLV_FRAME_INTER;
					isDispInterFrame = (flags & FLV_FRAME_DISP_INTER) == FLV_FRAME_DISP_INTER;						
                    break;

                case FLV_TAG_TYPE_SCRIPT:
                    break;

                default:
                    break;
            }
				
			// Read to the end of the current tag.
			buf.setPosition(dataStartPos + dataSize);
			buf.readUInt32(previousTagSize);
			if (previousTagSize == 0) {
				assert(false);
				break;	
			}				

			LogDebug() << "FLV Tag:" 
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
			
		} while(0);
			
		return result;
	}


	//
	// AMF Helpers
	//
	virtual void writeAMFSring(Buffer& buf, const char* val)
	{
		size_t len = strlen(val);
		buf.writeUInt16(len);
		buf.writeBytes(val, len);
	}
	
	virtual void writeAMFDouble(Buffer& buf, double val)
	{
		buf.writeUInt8(AMF_DATA_TYPE_NUMBER); // AMF_DATA_TYPE_NUMBER
		buf.writeUInt64(DoubleToInt(val));
	}
	
	virtual void writeAMFBool(Buffer& buf, bool val)
	{
		buf.writeUInt8(AMF_DATA_TYPE_BOOL); // AMF_DATA_TYPE_NUMBER
		buf.writeUInt8(val ? 1 : 0);
	}
		
protected:
	Format _format;
	bool _initial;
	bool _modifyingStream;
	bool _waitingForKeyframe;	
	UInt32 _timestampOffset;
	FPSCounter _fpsCounter;
};


} // namespace Media 
} // namespace Scy 


#endif
