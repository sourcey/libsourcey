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


#include "Sourcey/STUN/Message.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace Scy {
namespace STUN {


Message::Message() : 
	_type(0), 
	_state(Request), 
	_size(0), 
	_transactionID(CryptoProvider::generateRandomKey(16)) 
{
	assert(_transactionID.size() == 16);
}


Message::Message(const Message& r) : 
	_type(r.type()), 
	_state(r.state()), 
	_size(r.size()), 
	_transactionID(r.transactionID()) 
{
	assert(_type);
	assert(_transactionID.size() == 16);	

	// Clear our current attributes
	for (unsigned i = 0; i < _attrs.size(); i++)
		delete _attrs[i];
	_attrs.clear();

	// Copy attributes from source object
	for (unsigned i = 0; i < r.attrs().size(); i++)
		_attrs.push_back(r.attrs()[i]->clone());
}


Message& Message::operator = (const Message& r) 
{
	_type = r.type();
	_state = r.state();
	_size = r.size();
	_transactionID = r.transactionID();
	assert(_type);
	assert(_transactionID.size() == 16);	

	// Clear current attributes
	for (unsigned i = 0; i < _attrs.size(); i++)
		delete _attrs[i];
	_attrs.clear();

	// Copy attributes from source object
	for (unsigned i = 0; i < r.attrs().size(); i++)
		_attrs.push_back(r.attrs()[i]->clone());

	return *this;
}


Message::~Message() 
{
	for (unsigned i = 0; i < _attrs.size(); i++)
		delete _attrs[i];
}

	
IPacket* Message::clone() const
{
	return new Message(*this);
}


string Message::stateString() const 
{
	switch (_state) {
	case Request:					return "Request";
	case Indication:				return "Indication";
	case SuccessResponse:			return "SuccessResponse";
	case ErrorResponse:				return "ErrorResponse";	
	default:						return "Unknown";
	}
}


string Message::errorString(UInt16 errorCode) const
{
	switch (errorCode) {
	case BadRequest:				return "BAD REQUEST";
	case Unauthorized:				return "UNAUTHORIZED";
	case UnknownAttribute:			return "UNKNOWN ATTRIBUTE";
	case StaleCredentials:			return "STALE CREDENTIALS";
	case IntegrityCheckFailure:		return "INTEGRITY CHECK FAILURE";
	case MissingUsername:			return "MISSING USERNAME";
	case UseTLS:					return "USE TLS";		
	case RoleConflict:				return "Role Conflict"; // (487) rfc5245
	case ServerError:				return "SERVER ERROR";		
	case GlobalFailure:				return "GLOBAL FAILURE";	
	case ConnectionAlreadyExists:	return "Connection Already Exists";		
	case ConnectionTimeoutOrFailure:	return "Connection Timeout or Failure";			
	default:						return "Unknown";
	}
}


string Message::typeString() const 
{
	switch (_type) {
	case Binding:					return "BINDING";
	case Allocate:					return "ALLOCATE";
	case Refresh:					return "REFRESH";
	case SendIndication:			return "SEND-INDICATION";
	case DataIndication:			return "DATA-INDICATION";
	case CreatePermission:			return "CREATE-PERMISSION";
	case ChannelBind:				return "CHANNEL-BIND";		
	case Connect:					return "CONNECT";		
	case ConnectionBind:			return "CONNECTION-BIND";		
	case ConnectionAttempt:			return "CONNECTION-ATTEMPT";			
	default:						return "Unknown";
	}
}


string Message::toString() const 
{
	ostringstream ss;
	ss << "STUN[" << typeString() << ":" << transactionID() << "]";
	//for (unsigned i = 0; i < _attrs.size(); i++)
	//	os << ":" << _attrs[i]->typeString();
	return ss.str();
}


void Message::print(std::ostream& os) const
{
	os << "STUN[" << typeString() << ":" << transactionID();
	for (unsigned i = 0; i < _attrs.size(); i++)
		os << ":" << _attrs[i]->typeString();
	os << "]";
}


void Message::setTransactionID(const string& id) 
{
	assert(id.size() == 16);
	_transactionID = id;
}


void Message::add(Attribute* attr) 
{
	_attrs.push_back(attr);
	_size += attr->size() + 4;
}


Attribute* Message::get(Attribute::Type type, int index) const 
{
	for (unsigned i = 0; i < _attrs.size(); i++) {
		if (_attrs[i]->type() == type) {			
			if (index == 0)
				return _attrs[i];
			else index--;
		}
	}
	return NULL;
}


bool Message::read(Buffer& buf) 
{			
	if (!buf.readUInt16(_type)) {
		LogError() << "STUN: Not STUN type: " << _type << endl;
		return false;
	}

	if (_type & 0x8000) {
		// RTP and RTCP set MSB of first byte, since first two bits are version, 
		// and version is always 2 (10). If set, this is not a STUN packet.
		//LogError() << "STUN: Not STUN packet" << endl;
		return false;
	}

	if (!buf.readUInt16(_size)) {
		//LogError() << "STUN: Packet has no size: " << _size << endl;
		return false;
	}

	string transactionID;
	if (!buf.readString(transactionID, 16)) {
		//LogError() << "STUN: Packet has no Transaction ID: " << transactionID << endl;
		return false;
	}
	assert(transactionID.size() == 16);
	_transactionID = transactionID;

	if (_size > buf.remaining()) {
		LogError() << "STUN: Buffer error" << endl;
		return false;
	}

	_attrs.resize(0);

	size_t rest = buf.remaining() - _size;
	while (buf.remaining() > rest) {
		UInt16 attrType, attrLength;
		if (!buf.readUInt16(attrType)) {
			LogError() << "STUN: Attribute has no type: " << attrType << endl;
			return false;
		}
		if (!buf.readUInt16(attrLength)) {
			LogError() << "STUN: Attribute has no size: " << attrLength << endl;
			return false;
		}

		Attribute* attr = Attribute::create(attrType, attrLength);
		if (attr && attr->read(buf)) {
			//LogDebug() << "STUN: Parsed attribute: " << attrType << ": " << Attribute::typeString(attrType) << ": " << attrLength << endl;
			_attrs.push_back(attr);
		} else {
			// Allow for unrecognised attributes.
			//return false;	
			LogError() << "STUN: Failed to parse attribute: " << Attribute::typeString(attrType) << ": " << attrLength << endl;
		}
	}

	if (buf.remaining() != rest) {
		// FIXME: Shouldn't be doing this
		LogError() << "STUN: Wrong message size (" << rest << " != " << buf.remaining() << ")" << endl;
		return false;
	}

	return true;
}


void Message::write(Buffer& buf) const 
{
	buf.writeUInt16(_type);
	buf.writeUInt16(_size);
	buf.writeString(_transactionID);

	// TODO: Move Message Integrity to end

	for (unsigned i = 0; i < _attrs.size(); i++) {
		buf.writeUInt16(_attrs[i]->type());
		buf.writeUInt16(_attrs[i]->size()); 
		_attrs[i]->write(buf);
	}
}


} } // namespace Scy::STUN


/*
 * libjingle
 * Copyright 2004--2006, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products 
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

