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


#include "Sourcey/Media/AVPacketEncoder.h"


using namespace std;


namespace scy {
namespace av {


AVPacketEncoder::AVPacketEncoder(const RecordingOptions& options, bool muxLiveStreams) :
	AVEncoder(options), 
	PacketProcessor(AVEncoder::Emitter), 
	_lastVideoPacket(nil), 
	_muxLiveStreams(muxLiveStreams)
{
}


AVPacketEncoder::AVPacketEncoder(bool muxLiveStreams) :
	AVEncoder(), 
	PacketProcessor(AVEncoder::Emitter), 
	_muxLiveStreams(muxLiveStreams)
{
}


AVPacketEncoder::~AVPacketEncoder()
{
}


void AVPacketEncoder::process(IPacket& packet)
{	
	Mutex::ScopedLock lock(_mutex);

	// We may be receiving either audio or video packets	
	VideoPacket* vPacket = nil;
	AudioPacket* aPacket = nil;
	if (vPacket = dynamic_cast<VideoPacket*>(&packet)) ;
	else if (aPacket = dynamic_cast<AudioPacket*>(&packet)) ;
	else throw ArgumentException("Unknown packet type");

	// Do some special synchronizing for muxing live variable framerate streams
	if (_muxLiveStreams) {	
		VideoEncoderContext* video = AVEncoder::video();
		AudioEncoderContext* audio = AVEncoder::audio();
		assert(audio && video);
		double audioPts, videoPts;
		int times = 0;
		for (;;) 
		{
			times++;
			assert(times < 10);			
			audioPts = audio ? (double)audio->stream->pts.val * audio->stream->time_base.num / audio->stream->time_base.den : 0.0;
			videoPts = video ? (double)video->stream->pts.val * video->stream->time_base.num / video->stream->time_base.den : 0.0;

			if (aPacket)
			{
				// Write the audio packet when the encoder is ready
				if (!video || audioPts < videoPts) {
					encode(*aPacket);
					break;
				}

				// Write dummy video frames until we can encode the audio
				else {
					// May be null if the first packet was audio, skip...
					if (!_lastVideoPacket)
						break;

					encode(*_lastVideoPacket);
				}
			}
			else if (vPacket) 
			{
				// Write the video packet if the encoder is ready
				if (!audio || audioPts > videoPts)
					encode(*vPacket);
				
				if (audio) {
					// Clone and buffer the last video packet it can be used
					// as soon as we need an available frame.
					// used as a filler if the source framerate is inconstant.
					if (_lastVideoPacket)
						delete _lastVideoPacket;
					_lastVideoPacket = reinterpret_cast<scy::av::VideoPacket*>(vPacket->clone());
				}
				break;
			}
		}
	}
	else if (vPacket) {
		encode(*vPacket);
	}
	else if (aPacket) {
		encode(*aPacket);
	}
}


void AVPacketEncoder::encode(VideoPacket& packet)
{
	encodeVideo((unsigned char*)packet.data(), packet.size(), packet.width, packet.height, packet.time);
}


void AVPacketEncoder::encode(AudioPacket& packet)
{
	encodeAudio((unsigned char*)packet.data(), packet.size());
}


bool AVPacketEncoder::accepts(IPacket& packet) 
{ 
	return dynamic_cast<av::MediaPacket*>(&packet) != 0; 
}

					
void AVPacketEncoder::onStreamStateChange(const PacketStreamState& state) 
{ 
	traceL("AVPacketEncoder", this) << "Stream state change: " << state << endl;
	
	Mutex::ScopedLock lock(_mutex);

	switch (state.id()) {
	case PacketStreamState::Running:
		if (stateEquals(EncoderState::None))
			AVEncoder::initialize();
		break;
		
	case PacketStreamState::Stopped:
	case PacketStreamState::Error:
	case PacketStreamState::Resetting:
		//if (stateEquals(EncoderState::Encoding))
		if (isActive())
			AVEncoder::uninitialize();
		break;
	//case PacketStreamState::None:
	//case PacketStreamState::Stopping:
	//case PacketStreamState::Closed:
	}
}


} } // namespace scy::av




				// Otherwise write a dummy video frame to keep the encoder happy
				// We should be able to write the audio packet on next iteration.
				//{
					//if (videoPts > audioPts)
					//	return;
				//}

			//write_audio_frame(oc, _audio->stream);
			//encodeAudio((unsigned char*)aPacket->data(), aPacket->size());	
		//} 
		//else {
		/*
			*/
		//}

	/*

	AudioPacket* aPacket = dynamic_cast<AudioPacket*>(&packet);
	if (vPacket) {	
		encodeVideo((unsigned char*)vPacket->data(), vPacket->size(), vPacket->width, vPacket->height, vPacket->time);
		return;
	}
	*/
	/*
	if (aPacket) {		
		encodeAudio((unsigned char*)aPacket->data(), aPacket->size());
		return;
	}
	*/