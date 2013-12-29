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


#ifndef SCY_MEDIA_AVPacketEncoder_H
#define SCY_MEDIA_AVPacketEncoder_H


#include "scy/base.h"
#ifdef HAVE_FFMPEG
#include "scy/packetstream.h"
#include "scy/media/avencoder.h"


namespace scy { 
namespace av {


class AVPacketEncoder: public AVEncoder, public PacketProcessor
	/// Encodes and multiplexes a realtime video stream form 
	/// audio / video capture sources.
	/// FFmpeg is used for encoding.
{
public:
	AVPacketEncoder(const EncoderOptions& options, bool muxLiveStreams = false);
	AVPacketEncoder(bool muxLiveStreams = false);
	virtual ~AVPacketEncoder();
	
	virtual void encode(VideoPacket& packet);
	virtual void encode(AudioPacket& packet);

protected:		
	virtual bool accepts(IPacket& packet);
	virtual void process(IPacket& packet);	
	virtual void onStreamStateChange(const PacketStreamState& state);

	friend class PacketStream;
			
	mutable Mutex _mutex;
	bool _muxLiveStreams;
	VideoPacket* _lastVideoPacket;
};


#if 0
//
// PTS Calculator
//


struct PTSCalculator
	/// Helper class which calculates PTS values for a live source
{
	AVRational timeBase;
	clock_t frameTime;
	double frameDuration;
	double frameDiff;

	Int64 currentPTS;
	Int64 lastPTS;

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

	Int64 tick() {
		// Initializing
		if (frameTime == 0) {
			assert(!frameDuration);
			frameTime = clock();
			currentPTS = 1;
		}

		// Updating
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


} } // namespace scy::av


#endif
#endif