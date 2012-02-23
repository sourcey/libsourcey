//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_RTCP_SDESPacket_H
#define SOURCEY_RTCP_SDESPacket_H


#include "Sourcey/RTP/RTCP/Packet.h"

#include <vector>


namespace Sourcey {
namespace RTP {
namespace RTCP {


// ---------------------------------------------------------------------
//
struct SDESItem
	/// The class SDESItem is used for storing a source description item
	/// (type of description, description string) for an RTP end system.
	///
	///  0                   1                   2                   3
	///  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	/// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	/// |     type      |     length    | data                        ...
	/// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
{
    enum Type 
        /// This enumeration holds the types of source description items
        /// as defined in the RFC.
	{
        Unknown	= 0,
        CNAME	= 1,
        NAME	= 2,
        EMAIL	= 3,
        PHONE	= 4,
        LOC		= 5,
        TOOL	= 6,
        NOTE	= 7,
        PRIV	= 8
    };

    SDESItem();
    SDESItem(Type type, const std::string& content);
	virtual ~SDESItem() {};

	// TODO: clone()

    virtual std::string toString() const;
    virtual void print(std::ostream& os) const;

    virtual Type getType();
	
    UInt8 type;
    UInt8 length;
    std::string content;
	
	virtual UInt8 computedLength() const;	
};



// ---------------------------------------------------------------------
//
class SDESChunk
	/// The class SDESChunk is used for storing SDESItem objects
	/// for one RTP end system.
{
public:
    SDESChunk(UInt32 ssrc = 0);
    virtual ~SDESChunk();
	
	std::vector<SDESItem*>& items();
    virtual void addSDESItem(SDESItem* item);
		// Adds an SDESItem to this SDESChunk. If there is already an SDESItem
		// of the same type in this SDESChunk it is replaced by the new one.

    virtual std::string toString() const;
    virtual void print(std::ostream& os) const;
	
	virtual UInt16 computedLength() const; // UInt16?

    UInt32 ssrc;

protected:
	std::vector<SDESItem*> _sdesItems;
};



// ---------------------------------------------------------------------
//
class SDESPacket: public RTCP::Packet
	/// Source Description RTCP Packet
	///
	/// 	    0                   1                   2                   3
	///         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	/// header |V=2|P|    SC   |  PT=SDES=202  |             length            |
	///        +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	/// chunk  |                          SSRC/CSRC_1                          |
	///   1    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                           SDES items                          |
	///        |                              ...                              |
	///        +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	/// chunk  |                          SSRC/CSRC_2                          |
	///   2    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                           SDES items                          |
	///        |                              ...                              |
	///        +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
{
public:
	SDESPacket();
	virtual ~SDESPacket();
	
    virtual void addSDESChunk(SDESChunk* chunk);

	virtual bool read(Buffer& buffer);
	virtual void write(Buffer& buffer) const;
	
	virtual std::string toString() const;	
	virtual void print(std::ostream& os) const;
	
	virtual UInt16 computedLength() const;	

	virtual const char* className() const { return "RTCPSDESPacket"; }

protected:
	std::vector<SDESChunk*> _sdesChunks;
};


} // namespace RTCP
} // namespace RTP
} // namespace Sourcey 


#endif