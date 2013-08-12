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


#ifndef SOURCEY_STUN_ATTRIBUTES_H
#define SOURCEY_STUN_ATTRIBUTES_H


#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

#include "Sourcey/Base.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/Crypto/Crypto.h"
#include "Sourcey/Net/Address.h"


namespace scy {
namespace stun {


class Attribute 
	/// The virtual base class for all STUN/TURN attributes.
{
public:
	enum Type 
	{
		NotExist				= 0,
		MappedAddress			= 0x0001, 
		ResponseAddress         = 0x0002, // Not implemented
		ChangeRequest			= 0x0003, // Not implemented
		SourceAddress			= 0x0004, // Not implemented
		ChangedAddress			= 0x0005, // Not implemented
		Username				= 0x0006,
		Password				= 0x0007, // Not implemented
		MessageIntegrity		= 0x0008,
		ErrorCode				= 0x0009,
		Bandwidth				= 0x0010, // Not implemented
		DestinationAddress      = 0x0011, // Not implemented
		UnknownAttributes		= 0x000a,
		ReflectedFrom			= 0x000b, // Not implemented
		//TransportPreferences    = 0x000c, // Not implemented
		MagicCookie				= 0x000f, // Not implemented, ByteString, 4 bytes
		Realm					= 0x0014,
		Nonce					= 0x0015,
		XorMappedAddress		= 0x0020,
		Software				= 0x8022,
		Options					= 0x8001, // Not implemented
		AlternateServer			= 0x000e,
		Fingerprint				= 0x8028,

		/// TURN
		ChannelNumber			= 0x000c,
		Lifetime				= 0x000d,
		/// 0x0010: Reserved (was BANDWIDTH)
		XorPeerAddress			= 0x0012,
		Data					= 0x0013,
		XorRelayedAddress		= 0x0016,
		EventPort				= 0x0018, // Not implemented
		RequestedTransport		= 0x0019,
		DontFragment			= 0x001A, // Not implemented
		/// 0x0021: Reserved (was TIMER-VAL)
		ReservationToken			= 0x0022, // 8 bytes token value
		
		/// TURN TCP
		ConnectionID			= 0x002a,

		/// ICE
		ICEControlled			= 0x8029,
		ICEControlling			= 0x802A,
		ICEPriority				= 0x0024,
		ICEUseCandidate			= 0x0025
	};
	
	virtual ~Attribute() {}
	virtual Attribute* clone() = 0;

	virtual bool read(Buffer& buf) = 0;
		/// Reads the body (not the type or size) for this
		/// type  of attribute from  the given buffer. Return
		/// value is true if successful.

	virtual void write(Buffer& buf) const = 0;
		/// Writes the body (not the type or size) to the
		/// given buffer. Return value is true if successful.

	static Attribute* create(UInt16 type, UInt16 size = 0);
		/// Creates an attribute object with the given type 
		/// and size.
	
	Type type() const;
	UInt16 size() const;

	static const UInt16 TypeID = 0;

	std::string typeString();
	static std::string typeString(UInt16 type);

protected:
	//Attribute(UInt16 size = 0) {};
	Attribute(UInt16 type, UInt16 size = 0);
	void setLength(UInt16 size);

	UInt16 _type;
	UInt16 _size;
};


// ---------------------------------------------------------------------------
//
class AddressAttribute: public Attribute 
	/// Implements a STUN/TURN attribute that reflects a socket address.
{
public:
	AddressAttribute(UInt16 type);
	AddressAttribute(const AddressAttribute& r);

	virtual stun::Attribute* clone();

	static const UInt16 Size = 8;

	virtual UInt8 family() const { return _family; }
	virtual UInt16 port() const { return _port; }
	virtual UInt32 ip() const { return _ip; }
	virtual std::string ipString() const;
	virtual net::Address address() const;

	virtual void setFamily(UInt8 family) { _family = family; }
	virtual void setIP(UInt32 ip) { _ip = ip; }
	virtual void setIP(const std::string& ip);
	virtual void setPort(UInt16 port) { _port = port; }

	virtual bool read(Buffer& buf);
	virtual void write(Buffer& buf) const;

private:
	UInt8 _family;
	UInt16 _port;
	UInt32 _ip;
};


// ---------------------------------------------------------------------------
//
class UInt8Attribute: public Attribute 
	/// Implements STUN/TURN attribute that reflects a 32-bit integer.
{
public:
	UInt8Attribute(UInt16 type);
	UInt8Attribute(const UInt8Attribute& r);

	virtual Attribute* clone();

	static const UInt16 Size = 1;

	UInt8 value() const { return _bits; }
	void setValue(UInt8 bits) { _bits = bits; }

	bool getBit(int index) const;
	void setBit(int index, bool value);

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

private:
	UInt8 _bits;
};


// ---------------------------------------------------------------------------
//
class UInt32Attribute: public Attribute 
	/// Implements STUN/TURN attribute that reflects a 32-bit integer.
{
public:
	UInt32Attribute(UInt16 type);
	UInt32Attribute(const UInt32Attribute& r);

	virtual Attribute* clone();

	static const UInt16 Size = 4;

	UInt32 value() const { return _bits; }
	void setValue(UInt32 bits) { _bits = bits; }

	bool getBit(int index) const;
	void setBit(int index, bool value);

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

private:
	UInt32 _bits;
};


// ---------------------------------------------------------------------------
//
class UInt64Attribute: public Attribute 
	/// Implements STUN/TURN attribute that reflects a 64-bit integer.
{
public:
	UInt64Attribute(UInt16 type);
	UInt64Attribute(const UInt64Attribute& r);

	virtual Attribute* clone();

	static const UInt16 Size = 8;

	UInt64 value() const { return _bits; }
	void setValue(UInt64 bits) { _bits = bits; }

	bool getBit(int index) const;
	void setBit(int index, bool value);

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

private:
	UInt64 _bits;
};


class FlagAttribute: public Attribute 
	/// Implements STUN/TURN attribute representing a 0 size flag.
{
public:
	FlagAttribute(UInt16 type);

	virtual Attribute* clone();

	static const UInt16 Size = 0;

	bool read(Buffer&) { return true; }
	void write(Buffer&) const { }
};


// ---------------------------------------------------------------------------
//
class StringAttribute: public Attribute 
	/// Implements STUN/TURN attribute that reflects an arbitrary byte string
{
public:
	StringAttribute(UInt16 type, UInt16 size = 0);
	StringAttribute(const StringAttribute& r);
	virtual ~StringAttribute();

	virtual Attribute* clone();

	const char* bytes() const { return _bytes; }
	void setBytes(char* bytes, UInt16 size);

	std::string asString() const;
	void copyBytes(const char* bytes); //  uses strlen
	void copyBytes(const void* bytes, UInt16 size);

	UInt8 getByte(int index) const;
	void setByte(int index, UInt8 value);

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

private:
	char* _bytes;
};


// ---------------------------------------------------------------------------
//
class UInt16ListAttribute: public Attribute 
	/// Implements STUN/TURN attribute that reflects a list of attribute names.
{
public:
	UInt16ListAttribute(UInt16 type, UInt16 size);
	UInt16ListAttribute(const UInt16ListAttribute& r);
	virtual ~UInt16ListAttribute();

	virtual Attribute* clone();

	size_t size() const;
	UInt16 getType(int index) const;
	void setType(int index, UInt16 value);
	void addType(UInt16 value);

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

private:
	std::vector<UInt16> _attrTypes;
};


// ---------------------------------------------------------------------------
//
class MessageIntegrity: public Attribute 
	/// Implements STUN/TURN attributes that reflects an internet address.
{
public:
	MessageIntegrity();
	MessageIntegrity(const MessageIntegrity& r);
	virtual ~MessageIntegrity();

	virtual Attribute* clone();
	
	static const UInt16 TypeID = 0x0008;
	static const UInt16 Size = 20;

	std::string hmac() const { return _hmac; }
	std::string key() const { return _key; }

	bool verifyHmac(const std::string& key) const;

	void setInput(const std::string& input) { _input = input; }
	void setHmac(const std::string& hmac) { _hmac = hmac; }
	void setKey(const std::string& key) { _key = key; }

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

private:
	std::string _input;
	std::string _hmac;
	std::string _key;
};


// ---------------------------------------------------------------------------
//
class ErrorCode: public Attribute 
	/// Implements STUN/TURN attribute that reflects an error code.
{
public:	
	ErrorCode(UInt16 size = MinSize);
	ErrorCode(const ErrorCode& r);
	virtual ~ErrorCode();

	virtual Attribute* clone();
	
	static const UInt16 TypeID = 0x0009;
	static const UInt16 MinSize = 4;

	UInt32 errorCode() const;
	UInt8 errorClass() const { return _class; }
	UInt8 errorNumber() const { return _number; }
	const std::string& reason() const { return _reason; }

	void setErrorCode(UInt32 eCode);
	void setErrorClass(UInt8 eClass) { _class = eClass; }
	void setErrorNumber(UInt8 eNumber) { _number = eNumber; }
	void setReason(const std::string& reason);

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

private:
	UInt8 _class;
	UInt8 _number;
	std::string _reason;
};


// ---------------------------------------------------------------------------
//
#define DECLARE_FIXLEN_STUN_ATTRIBUTE(Name, Type, Derives)	\
															\
	class Name: public Derives								\
	{														\
	public:													\
		static const UInt16 TypeID = Type;					\
        Name() : Derives(TypeID) {};						\
        virtual ~Name() {};									\
    };														\

#define DECLARE_STUN_ATTRIBUTE(Name, Type, Derives, Length)	\
															\
	class Name: public Derives								\
	{														\
	public:													\
		static const UInt16 TypeID = Type;					\
        Name(UInt16 size = Length) :						\
			Derives(TypeID, size) {};						\
        virtual ~Name() {};									\
    };														\


// ---------------------------------------------------------------------------
//
// Address attributes
DECLARE_FIXLEN_STUN_ATTRIBUTE(MappedAddress, 0x0001, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ResponseAddress, 0x0002, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ChangedAddress, 0x0005, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ReflectedFrom, 0x000b, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(AlternateServer, 0x000e, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(SourceAddress, 0x0004, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(DestinationAddress, 0x0011, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(XorMappedAddress, 0x0020, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(XorPeerAddress, 0x0012, AddressAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(XorRelayedAddress, 0x0016, AddressAttribute)

// UInt32 attributes
DECLARE_FIXLEN_STUN_ATTRIBUTE(Fingerprint, 0x8028, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(RequestedTransport, 0x0019, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ChangeRequest, 0x0003, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(Lifetime, 0x000d, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(Bandwidth, 0x0010, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(Options, 0x8001, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ChannelNumber, 0x000c, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ICEPriority, 0x0024, UInt32Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ConnectionID, 0x002a, UInt32Attribute)

// UInt8 attributes
DECLARE_FIXLEN_STUN_ATTRIBUTE(EventPort, 0x0018, UInt8Attribute)

// UInt32 list attributes
DECLARE_STUN_ATTRIBUTE(UnknownAttributes, 0x000a, UInt16ListAttribute, 0)

// String attributes
DECLARE_STUN_ATTRIBUTE(Username, 0x0006, StringAttribute, 0)
DECLARE_STUN_ATTRIBUTE(Password, 0x0007, StringAttribute, 0)
DECLARE_STUN_ATTRIBUTE(MagicCookie, 0x000f, StringAttribute, 4)
DECLARE_STUN_ATTRIBUTE(Data, 0x0013, StringAttribute, 0)
DECLARE_STUN_ATTRIBUTE(Realm, 0x0014, StringAttribute, 0)
DECLARE_STUN_ATTRIBUTE(Nonce, 0x0015, StringAttribute, 0)
DECLARE_STUN_ATTRIBUTE(Software, 0x8022, StringAttribute, 0)
DECLARE_STUN_ATTRIBUTE(ReservationToken, 0x0022, StringAttribute, 8)

// UInt64 attributes
DECLARE_FIXLEN_STUN_ATTRIBUTE(ICEControlling, 0x802A, UInt64Attribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(ICEControlled, 0x8029, UInt64Attribute)

// Flag attributes
DECLARE_FIXLEN_STUN_ATTRIBUTE(ICEUseCandidate, 0x0025, FlagAttribute)
DECLARE_FIXLEN_STUN_ATTRIBUTE(DontFragment, 0x001A, FlagAttribute)


} } // namespace scy:stun


#endif // SOURCEY_STUN_ATTRIBUTES_H
