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


namespace scy {
namespace stun {


Message::Message() : 
	_type(0), 
	_state(Request), 
	_size(0), 
	_transactionID(util::randomString(16)) 
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
	case NotAuthorized:				return "UNAUTHORIZED";
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
	ostringstream os;
	os << "STUN[" << transactionID() << ":" << typeString();
	for (unsigned i = 0; i < _attrs.size(); i++)
		os << ":" << _attrs[i]->typeString();
	os << "]";
	return os.str();
}


void Message::print(std::ostream& os) const
{
	os << "STUN[" << typeString() << ":" << transactionID();
	for (unsigned i = 0; i < _attrs.size(); i++)
		os << ":" << _attrs[i]->typeString();
	os << "]";
}


void Message::setTransactionID(const std::string& id) 
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
	return nullptr;
}


bool Message::read(const ConstBuffer& buf) //BitReader& reader
{	
	try 
	{
		BitReader reader(buf);
		reader.getU16(_type);

		if (_type & 0x8000) {
			// RTP and RTCP set MSB of first byte, since first two bits are version, 
			// and version is always 2 (10). If set, this is not a STUN packet.
			//errorL("STUNMessage") << "Not STUN packet" << endl;
			return false;
		}

		reader.getU16(_size);

		std::string transactionID;
		reader.get(transactionID, 16);
		assert(transactionID.size() == 16);
		_transactionID = transactionID;

		if (_size > reader.available()) {
			warnL("STUNMessage") << "Buffer error: " << _size << " > " << reader.available() << endl;
			//return false;
		}

		_attrs.resize(0);

		size_t rest = reader.available() - _size;
		while (reader.available() > rest) {
			UInt16 attrType, attrLength;
			reader.getU16(attrType);
			reader.getU16(attrLength);

			Attribute* attr = Attribute::create(attrType, attrLength);
			if (!attr) {
				// Allow for unrecognised attributes.
				warnL("STUNMessage") << "Failed to parse attribute: " << Attribute::typeString(attrType) << ": " << attrLength << endl;
				continue;
			}
			attr->read(reader); // parse or throw
			_attrs.push_back(attr);
		}

		if (reader.available() != rest) {
			warnL("STUNMessage") << "Wrong message size (" << rest << " != " << reader.available() << ")" << endl;
			assert(0); // remove me
			return false;
		}
	
		return true; // ok
	}
	catch (std::exception& exc) {
		debugL("STUNMessage") << "Parse error: " << exc.what() << endl;
	}
	
	return false;
}


void Message::write(Buffer& buf) const 
{
	BitWriter writer(buf);
	writer.putU16(_type);
	writer.putU16(_size);
	writer.put(_transactionID);

	// TODO: Shift Message Integrity to end

	for (unsigned i = 0; i < _attrs.size(); i++) {
		writer.putU16(_attrs[i]->type());
		writer.putU16(_attrs[i]->size()); 
		_attrs[i]->write(writer);
	}
}


} } // namespace scy:stun