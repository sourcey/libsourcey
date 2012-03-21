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


#include "Sourcey/STUN/Attributes.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace Sourcey {
namespace STUN {


Attribute::Attribute(UInt16 type, UInt16 size) : 
	_type(type), _size(size) 
{
}


std::string Attribute::typeString(UInt16 type) 
{
	switch (type) {
	case Attribute::XorMappedAddress: return "XOR-MAPPED-ADDRESS";
	case Attribute::XorPeerAddress: return "XOR-PEER-ADDRESS";
	case Attribute::XorRelayedAddress: return "XOR-RELAYED-ADDRESS";
	case Attribute::MappedAddress: return "MAPPED-ADDRESS";
	case Attribute::ResponseAddress: return "RESPONSE-ADDRESS";
	case Attribute::ChangeRequest: return "CHANGE-REQUEST";		
	case Attribute::SourceAddress: return "SOURCE-ADDRESS";
	case Attribute::ChangedAddress: return "CHANGED-ADDRESS";
	case Attribute::Username: return "USERNAME";
	case Attribute::Password: return "PASSWORD";	
	case Attribute::MessageIntegrity: return "MESSAGE-INTEGRITY";	
	case Attribute::ErrorCode: return "ERROR-CODE";	
	case Attribute::Bandwidth: return "BANDWIDTH";	
	case Attribute::DestinationAddress: return "DESTINATION-ADDRESS";	
	case Attribute::UnknownAttributes: return "UNKNOWN-ATTRIBUTES";	
	case Attribute::ReflectedFrom: return "REFLECTED-FORM";		
	//case Attribute::TransportPreferences: return "TRANSPORT-PREFERENCES";	
	case Attribute::MagicCookie: return "MAGIC-COOKIE";		
	case Attribute::Realm: return "REALM";		
	case Attribute::Nonce: return "NONCE";		
	case Attribute::Software: return "SOFTWARE";	
	case Attribute::Options: return "OPTIONS";		
	case Attribute::AlternateServer: return "ALTERNATE-SERVER";		
	case Attribute::Fingerprint: return "FINGERPRINT";	
	case Attribute::ChannelNumber: return "CHANNEL-NUMBER";		
	case Attribute::Lifetime: return "LIFETIME";	
	case Attribute::Data: return "DATA";
	case Attribute::RequestedTransport: return "REQUESTED-TRANSPORT";	
	case Attribute::ReservationToken: return "RESERVED-TOKEN";	
	case Attribute::EventPort: return "EVEN-PORT";	
	case Attribute::DontFragment: return "DONT-FRAGMENT";	
	case Attribute::ICEControlled: return "ICE-CONTROLLED";	
	case Attribute::ICEControlling: return "ICE-CONTROLLING";	
	case Attribute::ICEPriority: return "PRIORITY";	
	case Attribute::ICEUseCandidate: return "USE-CANDIDATE";				
	case Attribute::ConnectionID: return "CONNECTION-ID";
	default: return "Unknown";
	}
}


UInt16 Attribute::size() const 
{ 
	return _size; 
}


Attribute::Type Attribute::type() const 
{ 
	return static_cast<Attribute::Type>(_type); 
} 


void Attribute::setLength(UInt16 size) 
{ 
	_size = size;
}


string Attribute::typeString() 
{
	return typeString(_type);
}


Attribute* Attribute::create(UInt16 type, UInt16 size)
{
	//Attribute* attr = get(type);
	//if (attr)
	//	return attr;

	switch (type)
	{
	case Attribute::MappedAddress:
		if (size != AddressAttribute::Size)
			return NULL;
		return new STUN::MappedAddress();

	case Attribute::XorMappedAddress:
		if (size != AddressAttribute::Size)
			return NULL;
		return new STUN::XorMappedAddress();

	case Attribute::XorRelayedAddress:
		if (size != AddressAttribute::Size)
			return NULL;
		return new STUN::XorRelayedAddress();

	case Attribute::XorPeerAddress:
		if (size != AddressAttribute::Size)
			return NULL;
		return new STUN::XorPeerAddress();

	case Attribute::AlternateServer:
		if (size != AddressAttribute::Size)
			return NULL;
		return new STUN::AlternateServer();

	case Attribute::ErrorCode:
		if (size < ErrorCode::MinSize)
			return NULL;
		return new STUN::ErrorCode(size);

	case Attribute::UnknownAttributes:
		return new STUN::UnknownAttributes(size);

	case Attribute::Fingerprint:
		if (size != UInt32Attribute::Size)
			return NULL;
		return new STUN::Fingerprint();

	case Attribute::RequestedTransport:
		if (size != UInt32Attribute::Size)
			return NULL;
		return new STUN::RequestedTransport();	

	case Attribute::Lifetime:
		if (size != UInt32Attribute::Size)
			return NULL;
		return new STUN::Lifetime();	

	case Attribute::Bandwidth:
		if (size != UInt32Attribute::Size)
			return NULL;
		return new STUN::Bandwidth();	

	case Attribute::ChannelNumber:
		if (size != UInt32Attribute::Size)
			return NULL;
		return new STUN::ICEUseCandidate();
		
	case Attribute::ConnectionID:
		if (size != UInt32Attribute::Size)
			return NULL;
		return new STUN::ConnectionID();

	case Attribute::MessageIntegrity:
		return (size == 20) ? new STUN::MessageIntegrity() : NULL;

	case Attribute::Nonce:
		return (size <= 128) ? new STUN::Nonce(size) : NULL;

	case Attribute::Realm:
		return (size <= 128) ? new STUN::Realm(size) : NULL;

	case Attribute::Software:
		return (size <= 128) ? new STUN::Software(size) : NULL;

	case Attribute::ReservationToken:
		return (size == 8) ? new STUN::ReservationToken() : NULL;		

	case Attribute::MagicCookie:
		return (size == 4) ? new STUN::MagicCookie() : NULL;		

	case Attribute::Data:
		return new STUN::Data(size);

	case Attribute::Username:
		return (size <= 128) ? new STUN::Username(size) : NULL;

	case Attribute::Password:
		return (size <= 128) ? new STUN::Password(size) : NULL;

	case Attribute::ICEPriority:
		if (size != UInt32Attribute::Size)
			return NULL;
		return new STUN::ICEPriority();

	case Attribute::ICEControlled:
		if (size != UInt64Attribute::Size)
			return NULL;
		return new STUN::ICEControlled();

	case Attribute::ICEControlling:
		if (size != UInt64Attribute::Size)
			return NULL;
		return new STUN::ICEControlling();

	case Attribute::ICEUseCandidate:
		return (size == 0) ? new STUN::ICEUseCandidate() : NULL;
		
	case Attribute::DontFragment:
		if (size != FlagAttribute::Size)
			return NULL;
		return new STUN::DontFragment();
		
	case Attribute::EventPort:
		if (size != UInt8Attribute::Size)
			return NULL;
		return new STUN::EventPort();

	//case Attribute::UnknownAttributes:
	//	return (size % 2 == 0) ? new STUN::UnknownAttributes(size) : NULL;
	//	break;

	//case Attribute::TransportPrefs:
	//	if ((size != TransportPrefs::Size1) &&
	//		(size != TransportPrefs::Size2))
	//		return NULL;
	//	return new STUN::TransportPrefs(size);

	//case Attribute::MagicCookie:
	//	return (size == 4) ? new STUN::MagicCookie() : NULL;
	//	break;

	default:
		Log("error") << "[STUN] Unable to create attribute for type: " << type << endl;
		break;
	}

	//_attrs.push_back(attr);
	//return attr;
	//assert(false);
	return NULL;
}


// ---------------------------------------------------------------------------
//
AddressAttribute::AddressAttribute(UInt16 type) : 
	Attribute(type, Size), _family(0), _port(0), _ip(0) 
{
}


AddressAttribute::AddressAttribute(const AddressAttribute& r) :
	Attribute(r._type, Size),
	_family(r._family),
	_port(r._port),
	_ip(r._ip)
{
}


Attribute* AddressAttribute::clone() 
{
	return new AddressAttribute(*this);
}


string AddressAttribute::ipString() const 
{ 
	ostringstream ost;
	ost << ((_ip >> 24) & 0xff);
	ost << '.';
	ost << ((_ip >> 16) & 0xff);
	ost << '.';
	ost << ((_ip >> 8) & 0xff);
	ost << '.';
	ost << ((_ip >> 0) & 0xff);
	return ost.str();
}


Net::Address AddressAttribute::address() const 
{ 
	return Net::Address(ipString(), _port); 
}


void AddressAttribute::setIP(const string& ip) 
{ 
	in_addr address;
	address.s_addr = inet_addr(ip.c_str());
	_ip = ntohl(address.s_addr);
}


bool AddressAttribute::read(Buffer& buf) 
{
	UInt8 dummy;
	if (!buf.readUInt8(dummy))
		return false;
	if (!buf.readUInt8(_family))
		return false;
	if (!buf.readUInt16(_port))
		return false;
	if (!buf.readUInt32(_ip))
		return false;
	return true;
}


void AddressAttribute::write(Buffer& buf) const 
{
	buf.writeUInt8(0);
	buf.writeUInt8(_family);
	buf.writeUInt16(_port);
	buf.writeUInt32(_ip);
}


// ---------------------------------------------------------------------------
//
UInt8Attribute::UInt8Attribute(UInt16 type) : 
	Attribute(type, Size), _bits(0) 
{
}


UInt8Attribute::UInt8Attribute(const UInt8Attribute& r) :
	Attribute(r._type, Size),
	_bits(r._bits)
{
}


Attribute* UInt8Attribute::clone() 
{
	return new UInt8Attribute(*this);
}


bool UInt8Attribute::getBit(int index) const 
{
	assert((0 <= index) && (index < 32));
	return static_cast<bool>((_bits >> index) & 0x1);
}


void UInt8Attribute::setBit(int index, bool value) 
{
	assert((0 <= index) && (index < 32));
	_bits &=  ~(1 << index);
	_bits |=  value ? (1 << index) : 0;
}


bool UInt8Attribute::read(Buffer& buf) 
{
	if (!buf.readUInt8(_bits)) {		
		return false;
	}
	return true;
}


void UInt8Attribute::write(Buffer& buf) const 
{
	buf.writeUInt8(_bits);
}


// ---------------------------------------------------------------------------
//
UInt32Attribute::UInt32Attribute(UInt16 type) : 
	Attribute(type, Size), _bits(0) 
{
}	


UInt32Attribute::UInt32Attribute(const UInt32Attribute& r) :
	Attribute(r._type, Size),
	_bits(r._bits)
{
}


Attribute* UInt32Attribute::clone() 
{
	return new UInt32Attribute(*this);
}


bool UInt32Attribute::getBit(int index) const 
{
	assert((0 <= index) && (index < 32));
	return static_cast<bool>((_bits >> index) & 0x1);
}


void UInt32Attribute::setBit(int index, bool value) 
{
	assert((0 <= index) && (index < 32));
	_bits &=  ~(1 << index);
	_bits |=  value ? (1 << index) : 0;
}


bool UInt32Attribute::read(Buffer& buf) 
{
	if (!buf.readUInt32(_bits)) {		
		return false;
	}
	return true;
}

void UInt32Attribute::write(Buffer& buf) const 
{
	buf.writeUInt32(_bits);
}


// ---------------------------------------------------------------------------
//
UInt64Attribute::UInt64Attribute(UInt16 type) : 
	Attribute(type, Size), _bits(0) 
{
}


UInt64Attribute::UInt64Attribute(const UInt64Attribute& r) :
	Attribute(r._type, Size),
	_bits(r._bits)
{
}


Attribute* UInt64Attribute::clone() 
{
	return new UInt64Attribute(*this);
}


bool UInt64Attribute::getBit(int index) const 
{
	assert((0 <= index) && (index < 32));
	return static_cast<bool>((_bits >> index) & 0x1);
}


void UInt64Attribute::setBit(int index, bool value) 
{
	assert((0 <= index) && (index < 32));
	_bits &=  ~(1 << index);
	_bits |=  value ? (1 << index) : 0;
}


bool UInt64Attribute::read(Buffer& buf) 
{
	if (!buf.readUInt64(_bits))	
		return false;
	return true;
}


void UInt64Attribute::write(Buffer& buf) const 
{
	buf.writeUInt64(_bits);
}


// ---------------------------------------------------------------------------
//
FlagAttribute::FlagAttribute(UInt16 type) : 
	Attribute(type, 0) 
{
}


Attribute* FlagAttribute::clone() 
{
	return new FlagAttribute(type());
}


// ---------------------------------------------------------------------------
//
StringAttribute::StringAttribute(UInt16 type, UInt16 size) : 
	Attribute(type, size), _bytes(0) 
{
}


StringAttribute::StringAttribute(const StringAttribute& r) :
	Attribute(r._type, r._size), _bytes(0) 
{
	copyBytes(r._bytes, r._size);
}


StringAttribute::~StringAttribute() 
{
	//if (_bytes)
		delete [] _bytes;
}


Attribute* StringAttribute::clone() 
{
	return new StringAttribute(*this);
}


/*
Attribute* StringAttribute::clone() 
{
	//StringAttribute* attr = new StringAttribute(type(), size());
	//attr->copyBytes(bytes(), size());
	//return attr;
}
*/


void StringAttribute::setBytes(char* bytes, UInt16 size) 
{
	//if (_bytes)
		delete [] _bytes;
	_bytes = bytes;
	setLength(size);
}


void StringAttribute::copyBytes(const char* bytes) 
{
	copyBytes(bytes, static_cast<UInt16>(strlen(bytes)));
}


void StringAttribute::copyBytes(const void* bytes, UInt16 size) 
{
	char* newBytes = new char[size];
	memcpy(newBytes, bytes, size);
	setBytes(newBytes, size);
}


UInt8 StringAttribute::getByte(int index) const 
{
	assert(_bytes != NULL);
	assert((0 <= index) && (index < size()));
	return static_cast<UInt8>(_bytes[index]);
}


void StringAttribute::setByte(int index, UInt8 value) 
{
	assert(_bytes != NULL);
	assert((0 <= index) && (index < size()));
	_bytes[index] = value;
}


bool StringAttribute::read(Buffer& buf) 
{
	//if (_bytes)
		delete [] _bytes;
	_bytes = new char[size()];	
	if (!buf.readBytes(_bytes, size()))
		return false;
	return true;
}


void StringAttribute::write(Buffer& buf) const 
{
	if (_bytes) {
		buf.writeBytes(_bytes, size());
	}
}


string StringAttribute::asString() const 
{
	return string(_bytes, size());
}


/*
//--------------- Fingerprint ----------------
Fingerprint::Fingerprint() :
	UInt32Attribute(Attribute::Fingerprint) {
}


//void Fingerprint::setCRC32(unsigned int crc) {
//	_crc32 = crc;
//}


//unsigned int Fingerprint::crc32() {
//	return _crc32;
//}
*/


	/*
void Fingerprint::write(Buffer& buf) const {
	buf.writeUInt32(_crc32);
}


bool Fingerprint::read(Buffer& buf) {
	try {
		buf.readUInt32(_crc32);
	}
	catch(...) {
		return false;
	}
	return true;
}
*/


// ---------------------------------------------------------------------------
//
MessageIntegrity::MessageIntegrity() : 
	Attribute(Attribute::MessageIntegrity, Size) 
{
}
	

MessageIntegrity::MessageIntegrity(const MessageIntegrity& r) :
	Attribute(r._type, Size),
	_input(r._input),
	_hmac(r._hmac),
	_key(r._key)
{
}


MessageIntegrity::~MessageIntegrity() 
{
}



Attribute* MessageIntegrity::clone() 
{
	return new MessageIntegrity(*this);
}

	
bool MessageIntegrity::verifyHmac(const string& key) const 
{
	//Log("debug") << "Message: Verify HMAC: " << key << endl;

	assert(key.size());
	assert(!_hmac.empty());
	assert(!_input.empty());

	//Log("debug") << "Message: Packet integrity input (" << _input << ")" << endl;
	//Log("debug") << "Message: Packet integrity key (" << key << ")" << endl;

	string hmac = CryptoProvider::computeHMAC(_input, key);
	assert(hmac.size() == Size);

	//Log("debug") << "Message: Verifying message integrity (" << hmac << ":" << _hmac << ")" << endl;

	return _hmac == hmac;
}


bool MessageIntegrity::read(Buffer& buf) 
{
	//Log("debug") << "Message: Read HMAC" << endl;

	// Read the HMAC value.
	if (!buf.readString(_hmac, Size))
		return false;

	//Log("debug") << "Message: Parsed message integrity (" << _hmac << ")" << endl;

	// Remember the original position ans reset the buffer 
	// position to 0.	
	size_t originalPos = buf.position();
	buf.setPosition(0);

	// Get the message prior to the current attribute and
	// fill the attribute with dummy content.
	Buffer hmacBuf(buf.data(), buf.size() - Size);
	hmacBuf.writeString("00000000000000000000");

	// Ensure the STUN message size value represents the real 
	// size of the buffered message.
	hmacBuf.updateUInt32((UInt32)buf.size(), 2);
	_input.assign(hmacBuf.data(), hmacBuf.size());
	  
	// Reset the original position of the buffer.
	buf.setPosition(originalPos);

	return true;
}


void MessageIntegrity::write(Buffer& buf) const 
{
	//Log("debug") << "Message: Write HMAC" << endl;

	// If the key (password) is present we will compute the 
	// HMAC for the current message, otherwise the attribute
	// content will be copied.
	if (!_key.empty()) {

		// Get the message prior to the current attribute and
		// fill the attribute with dummy content.
		Buffer hmacBuf(buf.data(), buf.size());
		hmacBuf.writeString("00000000000000000000");

		// Ensure the STUN message size value represents the real 
		// size of the buffered message.
		hmacBuf.updateUInt32((UInt32)buf.size() + Size, 2);

		string input(hmacBuf.data(), hmacBuf.size());
		string hmac(CryptoProvider::computeHMAC(input, _key));
		assert(hmac.size() == Size);

		// Append the real HAMC to the buffer.
		buf.writeBytes(hmac.data(), Size);
	}
	else {
		assert(!_hmac.empty());
		buf.writeBytes(_hmac.data(), Size);
	}
}


// ---------------------------------------------------------------------------
//
ErrorCode::ErrorCode(UInt16 size) : 
	Attribute(Attribute::ErrorCode, size), _class(0), _number(0) 
{
	assert(size >= MinSize);
}
	

ErrorCode::ErrorCode(const ErrorCode& r) :
	Attribute(Attribute::ErrorCode, r._size),
	_class(r._class),
	_number(r._number),
	_reason(r._reason)
{
}


ErrorCode::~ErrorCode() 
{
}


Attribute* ErrorCode::clone() 
{
	//ErrorCode* attr = new ErrorCode();
	//attr->setErrorCode(errorCode());
	//attr->setErrorClass(errorClass());
	//attr->setErrorNumber(errorNumber());
	//attr->setReason(reason());
	//return attr;
	return new ErrorCode(*this);
}
	

UInt32 ErrorCode::errorCode() const 
{ 
	return (_class << 8) | _number;
}


void ErrorCode::setErrorCode(UInt32 eCode) 
{
	_class = (UInt8)((eCode >> 8) & 0x7);
	_number = (UInt8)(eCode & 0xff);
}


void ErrorCode::setReason(const string& reason) 
{
	setLength(MinSize + static_cast<UInt16>(reason.size()));
	_reason = reason;
}

bool ErrorCode::read(Buffer& buf) 
{
	UInt32 val;
	if (!buf.readUInt32(val))
		return false;

	if ((val >> 11) != 0)
		Log("debug") << "error-code bits not zero";

	setErrorCode(val);

	if (!buf.readString(_reason, size() - 4))
		return false;

	return true;
}

void ErrorCode::write(Buffer& buf) const 
{
	buf.writeUInt32(errorCode());
	buf.writeString(_reason);
}


// ---------------------------------------------------------------------------
//
UInt16ListAttribute::UInt16ListAttribute(UInt16 type, UInt16 size) : 
	Attribute(type, size) 
{
}


UInt16ListAttribute::UInt16ListAttribute(const UInt16ListAttribute& r) :
	Attribute(r._type, r._size),
	_attrTypes(r._attrTypes)
{
	/*
	//UInt16ListAttribute* attr = new UInt16ListAttribute(type(), size());
	for (unsigned i = 0; i < r._attrTypes.size(); i++)
		addType((r._attrTypes)[i]);
	return attr;
	*/
}


UInt16ListAttribute::~UInt16ListAttribute() 
{
}


Attribute* UInt16ListAttribute::clone() 
{
	return new UInt16ListAttribute(*this);
}


size_t UInt16ListAttribute::size() const 
{
	return _attrTypes.size();
}


UInt16 UInt16ListAttribute::getType(int index) const 
{
	return _attrTypes[index];
}


void UInt16ListAttribute::setType(int index, UInt16 value) 
{
	_attrTypes[index] = value;
}


void UInt16ListAttribute::addType(UInt16 value) 
{
	_attrTypes.push_back(value);
	setLength(static_cast<UInt16>(_attrTypes.size() * 2));
}


bool UInt16ListAttribute::read(Buffer& buf) 
{
	for (int i = 0; i < size() / 2; i++) {
		UInt16 attr;
		if (!buf.readUInt16(attr))
			return false;
		_attrTypes.push_back(attr);
	}
	return true;
}

void UInt16ListAttribute::write(Buffer& buf) const 
{
	for (unsigned i = 0; i < _attrTypes.size(); i++)
		buf.writeUInt16(_attrTypes[i]);
}


/*
TransportPrefs::TransportPrefs(UInt16 type, UInt16 size) : 
	Attribute(type, size), _preallocate(false), _prefs(0), _address(0) 
{
}


TransportPrefs::~TransportPrefs() {
	delete _address;
}


void TransportPrefs::setPreallocateAddress(
	AddressAttribute* address) {
		if (!address) {
			_preallocate = false;
			_address = 0;
			setLength(Size1);
		} else {
			_preallocate = true;
			_address = address;
			setLength(Size2);
		}
}


bool TransportPrefs::read(Buffer& buf) 
{
	UInt32 val;
	if (!buf.readUInt32(val))
		return false;

	if ((val >> 3) != 0)
		Log("debug") << "transport-preferences bits not zero";

	_preallocate = static_cast<bool>((val >> 2) & 0x1);
	_prefs = (UInt8)(val & 0x3);

	if (_preallocate && (_prefs == 3))
		Log("debug") << "transport-preferences imcompatible P and Typ";

	if (!_preallocate) {
		if (size() != UInt32Attribute::Size)
			return false;
	} else {
		if (size() != UInt32Attribute::Size + AddressAttribute::Size)
			return false;

		_address = new AddressAttribute(SourceAddress);
		_address->read(buf);
	}

	return true;
}


void TransportPrefs::write(Buffer& buf) const {
	buf.writeUInt32((_preallocate ? 4 : 0) | _prefs);

	if (_preallocate)
		_address->write(buf);
}
*/


} } // namespace Sourcey::STUN
