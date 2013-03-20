//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_RTP_Depacketizer_H
#define SOURCEY_RTP_Depacketizer_H


#include "Sourcey/IDepacketizer.h"
#include "Sourcey/Signal.h"
#include "Sourcey/RTP/Packet.h"
#include <deque>


namespace Sourcey {
namespace RTP {


#define MAX_RTP_PACKETS_IN_QUEUE 1024


struct PacketQueueEntry
	// Following struct is used to store pending packets in |_queue|.
	// Each entry may be in three different states:
	//  |received| == false, |packet| == NULL - packet with the corresponding
	//    sequence number hasn't been received.
	//  |received| == true, |packet| != NULL - packet with the corresponding
	//    sequence number has been received, but hasn't been processed, still
	//    waiting for other fragments.
	//  |received| == true, |packet| == NULL - packet with the corresponding
	//    sequence number has been received and processed. Ignore any additional
	//    packet with the same sequence number.
{
	enum PacketOrder 
	{
		First,
		Middle,
		Last
	};

	bool received;
	PacketOrder order;
	RTP::Packet* packet;

	PacketQueueEntry() : received(false), packet(NULL) {}
};


typedef std::deque<PacketQueueEntry> PacketQueue;


//template <class T>
class Depacketizer: public IDepacketizer//<T> 
	/// Provides RTP depacketization.
	/// Loosely based on Chromium's RTP Video Reader:
	///	http://src.chromium.org/svn/trunk/src/remoting/protocol/rtp_video_reader.cc
{
public:
	Depacketizer() : 
	    _lastSequenceNumber(0), 
		_lastMarkerSequenceNumber(0)//, 
		//_marker(true)
		//_headerLength(50),
		//_payloadType(0),
		//_timestamp(0),
		//_ssrc(0) 
	{
		setMTU(RTP_MAX_PACKET_LEN);
	}

	virtual void process(IPacket& packet)
		// Processes and sequentially orders incoming RTP packets,
		// and outputs T packets.
	{
		RTP::Packet* rtpPacket = new RTP::Packet;
		Buffer buf(RTP_MAX_PACKET_LEN);
		packet.write(buf);
		rtpPacket->read(buf);

		UInt16 sequenceNumber = rtpPacket->header().sequenceNumber;
		UInt16 relativeNumber = sequenceNumber - _lastSequenceNumber;
		int packetIndex;
		
		/*
		LogDebug() << "[RTP Depacketizer:" << this << "] Packet Info:"
			<< "\n\tSequence Number: " << sequenceNumber
			<< "\n\tRelative Number: " << relativeNumber
			<< "\n\tPacket Index: " << packetIndex
			<< endl;
			*/

		if (_queue.empty()) {
			// This is the first packet we've received. Setup the queue.
			resetQueue();
			_lastSequenceNumber = sequenceNumber;
			packetIndex = 0; //_queue.size() - 1;
		} 
		else if (relativeNumber > 0) {
			if (relativeNumber > MAX_RTP_PACKETS_IN_QUEUE) {
				// Sequence number jumped too much for some reason. Reset the queue.
				LogDebug() << "[RTP Depacketizer] Resetting unstable queue "
						<< relativeNumber << std::endl;
				resetQueue();
			} 
			else {
				_queue.resize(_queue.size() + relativeNumber);

				// Cleanup old and received packets, so that we don't have more than
				// |MAX_RTP_PACKETS_IN_QUEUE| packets.
				while (static_cast<int>(_queue.size()) > MAX_RTP_PACKETS_IN_QUEUE) {
					delete _queue.front().packet;
					_queue.pop_front();
					//LogDebug() << "[RTP Depacketizer] Purging " << _queue.size() << std::endl;
				}
			}
			_lastSequenceNumber = sequenceNumber;
			packetIndex = _queue.size() - 1;
		} 
		else {
			packetIndex = _queue.size() - 1 + relativeNumber;
			if (packetIndex < 0) {
				// The packet is too old. Just drop it.
				LogDebug() << "[RTP Depacketizer] Dropping old packet at "
						<< sequenceNumber << std::endl;
				delete rtpPacket;
				return;
			}
		}

		assert(packetIndex < static_cast<int>(_queue.size()));

		if (_queue[packetIndex].received) {
			LogDebug() << "Received duplicate packet with sequence number "
				<< sequenceNumber << std::endl;
			delete rtpPacket;
			return;
		}

		_queue[packetIndex].packet = rtpPacket;
		_queue[packetIndex].received = true;
		_queue[packetIndex].order = PacketQueueEntry::Middle;		

		// Set the packet order to first if this packet is one above the 
		// last marker position.
		if (rtpPacket->header().sequenceNumber == (_lastMarkerSequenceNumber + 1)) {
			_queue[packetIndex].order = PacketQueueEntry::First;
			//LogDebug() << "[RTP Depacketizer] Received first fragment "
			//		<< sequenceNumber << std::endl;
		}

		// If the marker is set then this is the last packet in the frame.
		// Set our packet order accordingly.
		if (rtpPacket->header().marker) {
			_lastMarkerSequenceNumber = sequenceNumber;
			_queue[packetIndex].order = PacketQueueEntry::Last;
			//LogDebug() << "[RTP Depacketizer] Received last fragment "
			//		<< sequenceNumber << std::endl;
		}

		checkFullPacket(_queue.begin() + packetIndex);
	}

	void resetQueue() 
	{
		LogDebug() << "[RTP Depacketizer] Resetting queue " << std::endl;

		for (PacketQueue::iterator it = _queue.begin();
			it != _queue.end(); ++it) {
				delete it->packet;
		}
		_queue.assign(MAX_RTP_PACKETS_IN_QUEUE, PacketQueueEntry());
	}

	void checkFullPacket(PacketQueue::iterator pos) 
	{
		//LogDebug() << "[RTP Depacketizer] Checking for full packet" << std::endl;

		/*
		if (pos->packet->vp8_descriptor().fragmentation_info ==
		Vp8Descriptor::NOT_FRAGMENTED) {
		// The packet is not fragmented.
		rebuildDataPacket(pos, pos);
		return;
		}

		PacketQueue::iterator first = pos;
		while (first > _queue.begin() && first->packet &&
		first->packet->vp8_descriptor().fragmentation_info !=
		Vp8Descriptor::FIRST_FRAGMENT) {
		first--;
		}
		if (!first->packet || first->packet->vp8_descriptor().fragmentation_info !=
		Vp8Descriptor::FIRST_FRAGMENT) {
		// We don't have first fragment.
		return;
		}

		PacketQueue::iterator last = pos;
		while (last < (_queue.end() - 1) && last->packet &&
		last->packet->vp8_descriptor().fragmentation_info !=
		Vp8Descriptor::LAST_FRAGMENT) {
		last++;
		}
		if (!last->packet || last->packet->vp8_descriptor().fragmentation_info !=
		Vp8Descriptor::LAST_FRAGMENT) {
		// We don't have last fragment.
		return;
		}
		*/

		PacketQueue::iterator first = pos;
		while (first > _queue.begin() && first->packet && 
			first->order != PacketQueueEntry::First) {
			first--;
		}
		if (!first->packet || 
			first->order != PacketQueueEntry::First) {
			// We don't have first fragment.
			//LogDebug() << "[RTP Depacketizer] We don't have first fragment" << std::endl;
			return;
		}

		PacketQueue::iterator last = pos;
		while (last < (_queue.end() - 1) && last->packet &&
			last->order != PacketQueueEntry::Last) {
			last++;
		}
		if (!last->packet || 
			last->order != PacketQueueEntry::Last) {
			// We don't have last fragment.
			//LogDebug() << "[RTP Depacketizer] We don't have last fragment" << std::endl;
			return;
		}

		// We've found first and last fragments, and we have all fragments in the
		// middle, so we can rebuild fill packet.
		rebuildDataPacket(first, last);
	}

	void rebuildDataPacket(PacketQueue::iterator first, PacketQueue::iterator last) 
	{
		DataPacket packet;

		// Rebuild packet content from the fragments.
		// TODO: Use CompoundBuffer inside of VideoPacket, so that we don't
		// need to memcopy any data.
		
		Buffer buf;
		for (PacketQueue::iterator it = first; it <= last; ++it) {			
			buf.writeBytes(it->packet->payload(), it->packet->payloadLength());

			//LogDebug() << "[RTP Depacketizer] Rebuilding packet with fragment " 
			//	<< it->packet->header().sequenceNumber << std::endl;

			// Delete packet because we don't need it anymore.
			delete it->packet;
			it->packet = NULL;

			// Here we keep |received| flag set to true, so that duplicate RTP
			// packets will be ignored.
		}

		packet.read(buf);

		// LogTrace() << "[RTP Depacketizer] Rebuilt data packet: " << packet.size << std::endl;

		// Output our packet!
		broadcast(this, packet);
	}

protected:
	PacketQueue _queue;

	unsigned _lastSequenceNumber;
	unsigned _lastMarkerSequenceNumber;
};


} // namespace RTP
} // namespace Sourcey 


#endif
