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


#include "Sourcey/RTP/RTCP/SDESPacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


using namespace std;


namespace scy {
namespace rtp {
namespace RTCP {



// ---------------------------------------------------------------------
SDESItem::SDESItem() :
    type(SDESItem::Unknown),
	length(2) // length minus content
{
}


SDESItem::SDESItem(Type type, const std::string& content) 
{
    this->type = static_cast<UInt8>(type);
    this->content = content;
    this->length = 2 + content.length();
}


UInt8 SDESItem::computedLength() const 
{
    // An sdes item requires one byte for the type
    // field, one byte for the length field and bytes
    // for the content string.
	return 2 + content.length();
}


SDESItem::Type SDESItem::getType() 
{
    return static_cast<SDESItem::Type>(type);
}


string SDESItem::toString() const
{
    stringstream out;
    out << "SDESItem=" << content;
    return out.str();
}


void SDESItem::print(ostream& os) const
{
    os << "SDESItem:" << endl;
    os << "  type = " << (int)type << endl;
    os << "  content = " << content << endl;
}


// ---------------------------------------------------------------------
SDESChunk::SDESChunk(UInt32 ssrc) 
{
    this->ssrc = ssrc;
    //this->length = 4; // length of ssrc
}


SDESChunk::~SDESChunk() 
{
	util::ClearVector(_sdesItems);
}


UInt16 SDESChunk::computedLength() const 
{
	UInt16 len = 4;	// length of ssrc
	for (unsigned i = 0; i < _sdesItems.size(); i++)
		len += _sdesItems[i]->computedLength();
	return len;
}


vector<SDESItem*>& SDESChunk::items() 
{
	return _sdesItems;
}


void SDESChunk::addSDESItem(SDESItem* sdesItem) 
{
	_sdesItems.push_back(sdesItem);     
	//this->length += sdesItem->length;   
}


string SDESChunk::toString() const
{
    stringstream out;
    out << "SDESChunk.ssrc=" << ssrc << " items=" << computedLength();
    return out.str();
}


void SDESChunk::print(ostream& os) const
{
    os << "SDESChunk:" << endl;
    os << "  ssrc = " << (int)ssrc << endl;
    os << "  length = " << (int)computedLength() << endl;
	for (unsigned i = 0; i < _sdesItems.size(); i++) {
		_sdesItems[i]->print(os);
	}
}



// ---------------------------------------------------------------------
SDESPacket::SDESPacket() :
	Packet(Packet::SDES)
{    
}


SDESPacket::~SDESPacket() 
{
	util::ClearVector(_sdesChunks);
}


UInt16 SDESPacket::computedLength() const 
{
	// SDES chunks directly follow the standard
	// RTCP header. The size of the RTCP packet
    // increases by the size of each SDES chunk.
	UInt16 len = Packet::computedLength();	
	for (unsigned i = 0; i < _sdesChunks.size(); i++)
		len += _sdesChunks[i]->computedLength();
	return len;
}


bool SDESPacket::read(Buffer& buffer) 
{
	if (RTCP::Packet::read(buffer)) {		
		
		int remaining = length - 4;
		while (remaining >= 4) {
			SDESChunk* chunk = new SDESChunk();
			buffer.readU32(chunk->ssrc);
			traceL() << "RTCP: SDESPacket: Read Chunk SSRC: " << chunk->ssrc << endl;

			// TODO: Support multiple items per chunk
			SDESItem* item = new SDESItem();
			buffer.readU8(item->type);
			buffer.readU8(item->length);
			buffer.read(item->content, item->length - 2);

			chunk->addSDESItem(item);
			addSDESChunk(chunk);

			remaining -= chunk->computedLength();
		}

		return true;
	}

	return false;
}


void SDESPacket::write(Buffer& buffer) const 
{
	RTCP::Packet::write(buffer);
	
	for (unsigned i = 0; i < _sdesChunks.size(); i++) {
		SDESChunk* chunk = _sdesChunks[i];
		
		debugL() << "RTCP: SDESPacket: Writing Chunk SSRC: " << chunk->ssrc << endl;

		buffer.writeU32(chunk->ssrc);

		for (unsigned ia = 0; ia < chunk->items().size(); ia++) {
			SDESItem* item = chunk->items()[ia];
			buffer.writeU8(item->type);
			buffer.writeU8(item->length);
			buffer.write(item->content);
		}
	}
}


void SDESPacket::addSDESChunk(SDESChunk* chunk) 
{
	_sdesChunks.push_back(chunk);    
	this->sources++;
}


std::string SDESPacket::toString() const
{
    std::stringstream out;
    out << "RTCP SDESPacket";
    return out.str();
}


void SDESPacket::print(std::ostream& os) const 
{	
    os << "RTCPSDESPacket:" << endl;
    os << "  version = " << (int)version << endl;
    os << "  padding = " << padding << endl;
    os << "  sources = " << (int)sources << endl;
    os << "  packetType = " << (int)packetType << endl;
    os << "  length = " << (int)computedLength() << endl;
	for (unsigned i = 0; i < _sdesChunks.size(); i++) {
		_sdesChunks[i]->print(os);
	}
}


} // namespace RTCP
} // namespace rtp
} // namespace scy 