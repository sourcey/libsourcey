///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup stun
/// @{


#ifdef SCY_WIN
#include <winsock2.h>
#endif

#include "scy/crypto/hmac.h"
#include "scy/logger.h"
#include "scy/stun/attributes.h"
#include "scy/stun/message.h"


using namespace std;


namespace scy {
namespace stun {


Attribute::Attribute(uint16_t type, uint16_t size)
    : _type(type)
    , _size(size)
{
}


std::string Attribute::typeString(uint16_t type)
{
    switch (type) {
        case Attribute::XorMappedAddress:
            return "XOR-MAPPED-ADDRESS";
        case Attribute::XorPeerAddress:
            return "XOR-PEER-ADDRESS";
        case Attribute::XorRelayedAddress:
            return "XOR-RELAYED-ADDRESS";
        case Attribute::MappedAddress:
            return "MAPPED-ADDRESS";
        case Attribute::ResponseAddress:
            return "RESPONSE-ADDRESS";
        case Attribute::ChangeRequest:
            return "CHANGE-REQUEST";
        case Attribute::SourceAddress:
            return "SOURCE-ADDRESS";
        case Attribute::ChangedAddress:
            return "CHANGED-ADDRESS";
        case Attribute::Username:
            return "USERNAME";
        case Attribute::Password:
            return "PASSWORD";
        case Attribute::MessageIntegrity:
            return "MESSAGE-INTEGRITY";
        case Attribute::ErrorCode:
            return "ERROR-CODE";
        case Attribute::Bandwidth:
            return "BANDWIDTH";
        case Attribute::DestinationAddress:
            return "DESTINATION-ADDRESS";
        case Attribute::UnknownAttributes:
            return "UNKNOWN-ATTRIBUTES";
        case Attribute::ReflectedFrom:
            return "REFLECTED-FORM";
        // case Attribute::TransportPreferences: return "TRANSPORT-PREFERENCES";
        case Attribute::MagicCookie:
            return "MAGIC-COOKIE";
        case Attribute::Realm:
            return "REALM";
        case Attribute::Nonce:
            return "NONCE";
        case Attribute::Software:
            return "SOFTWARE";
        case Attribute::Options:
            return "OPTIONS";
        case Attribute::AlternateServer:
            return "ALTERNATE-SERVER";
        case Attribute::Fingerprint:
            return "FINGERPRINT";
        case Attribute::ChannelNumber:
            return "CHANNEL-NUMBER";
        case Attribute::Lifetime:
            return "LIFETIME";
        case Attribute::Data:
            return "DATA";
        case Attribute::RequestedTransport:
            return "REQUESTED-TRANSPORT";
        case Attribute::ReservationToken:
            return "RESERVED-TOKEN";
        case Attribute::EventPort:
            return "EVEN-PORT";
        case Attribute::DontFragment:
            return "DONT-FRAGMENT";
        case Attribute::ICEControlled:
            return "ICE-CONTROLLED";
        case Attribute::ICEControlling:
            return "ICE-CONTROLLING";
        case Attribute::ICEPriority:
            return "PRIORITY";
        case Attribute::ICEUseCandidate:
            return "USE-CANDIDATE";
        case Attribute::ConnectionID:
            return "CONNECTION-ID";
        default:
            return "Unknown";
    }
}


uint16_t Attribute::size() const
{
    return _size;
}


uint16_t Attribute::type() const // Attribute::Type
{
    return _type; // static_cast<Attribute::Type>(_type);
}


void Attribute::setLength(uint16_t size)
{
    _size = size;
}


std::string Attribute::typeString()
{
    return typeString(_type);
}


void Attribute::consumePadding(BitReader& reader) const
{
    int remainder = _size % 4;
    if (remainder > 0) {
        reader.skip(4 - remainder);
    }
}


void Attribute::writePadding(BitWriter& writer) const
{
    int remainder = _size % 4;
    if (remainder > 0) {
        char zeroes[4] = {0};
        writer.put(zeroes, 4 - remainder);
    }
}


Attribute* Attribute::create(uint16_t type, uint16_t size)
{
    // Attribute* attr = get(type);
    // if (attr)
    //    return attr;

    switch (type) {
        case Attribute::MappedAddress:
            if (size != AddressAttribute::IPv4Size &&
                size != AddressAttribute::IPv6Size)
                return nullptr;
            return new stun::MappedAddress();

        case Attribute::XorMappedAddress:
            if (size != AddressAttribute::IPv4Size &&
                size != AddressAttribute::IPv6Size)
                return nullptr;
            return new stun::XorMappedAddress();

        case Attribute::XorRelayedAddress:
            if (size != AddressAttribute::IPv4Size &&
                size != AddressAttribute::IPv6Size)
                return nullptr;
            return new stun::XorRelayedAddress();

        case Attribute::XorPeerAddress:
            if (size != AddressAttribute::IPv4Size &&
                size != AddressAttribute::IPv6Size)
                return nullptr;
            return new stun::XorPeerAddress();

        case Attribute::AlternateServer:
            if (size != AddressAttribute::IPv4Size &&
                size != AddressAttribute::IPv6Size)
                return nullptr;
            return new stun::AlternateServer();

        case Attribute::ErrorCode:
            if (size < ErrorCode::MinSize)
                return nullptr;
            return new stun::ErrorCode(size);

        case Attribute::UnknownAttributes:
            return new stun::UnknownAttributes(size);

        case Attribute::Fingerprint:
            if (size != Fingerprint::Size)
                return nullptr;
            return new stun::Fingerprint();

        case Attribute::RequestedTransport:
            if (size != RequestedTransport::Size)
                return nullptr;
            return new stun::RequestedTransport();

        case Attribute::Lifetime:
            if (size != Lifetime::Size)
                return nullptr;
            return new stun::Lifetime();

        case Attribute::Bandwidth:
            if (size != Bandwidth::Size)
                return nullptr;
            return new stun::Bandwidth();

        case Attribute::ChannelNumber:
            if (size != ICEUseCandidate::Size)
                return nullptr;
            return new stun::ICEUseCandidate();

        case Attribute::ConnectionID:
            if (size != ConnectionID::Size)
                return nullptr;
            return new stun::ConnectionID();

        case Attribute::MessageIntegrity:
            return (size == 20) ? new stun::MessageIntegrity() : nullptr;

        case Attribute::Nonce:
            return (size <= 128) ? new stun::Nonce(size) : nullptr;

        case Attribute::Realm:
            return (size <= 128) ? new stun::Realm(size) : nullptr;

        case Attribute::Software:
            return (size <= 128) ? new stun::Software(size) : nullptr;

        case Attribute::ReservationToken:
            return (size == 8) ? new stun::ReservationToken() : nullptr;

        case Attribute::MagicCookie:
            return (size == 4) ? new stun::MagicCookie() : nullptr;

        case Attribute::Data:
            return new stun::Data(size);

        case Attribute::Username:
            return (size <= 128) ? new stun::Username(size) : nullptr;

        case Attribute::Password:
            return (size <= 128) ? new stun::Password(size) : nullptr;

        case Attribute::ICEPriority:
            if (size != ICEPriority::Size)
                return nullptr;
            return new stun::ICEPriority();

        case Attribute::ICEControlled:
            if (size != ICEControlled::Size)
                return nullptr;
            return new stun::ICEControlled();

        case Attribute::ICEControlling:
            if (size != ICEControlling::Size)
                return nullptr;
            return new stun::ICEControlling();

        case Attribute::ICEUseCandidate:
            return (size == 0) ? new stun::ICEUseCandidate() : nullptr;

        case Attribute::DontFragment:
            if (size != DontFragment::Size)
                return nullptr;
            return new stun::DontFragment();

        case Attribute::EventPort:
            if (size != EventPort::Size)
                return nullptr;
            return new stun::EventPort();

        // case Attribute::UnknownAttributes:
        //    return (size % 2 == 0) ? new stun::UnknownAttributes(size) :
        //    nullptr;
        //    break;

        // case Attribute::TransportPrefs:
        //    if ((size != TransportPrefs::Size1) &&
        //        (size != TransportPrefs::Size2))
        //        return nullptr;
        //    return new stun::TransportPrefs(size);

        // case Attribute::MagicCookie:
        //    return (size == 4) ? new stun::MagicCookie() : nullptr;
        //    break;

        default:
            LError("Cannot create attribute for type: ", type)
            break;
    }

    //_attrs.push_back(attr);
    // return attr;
    // assert(false);
    return nullptr;
}


// ---------------------------------------------------------------------------
//
AddressAttribute::AddressAttribute(uint16_t type, bool ipv4)
    : Attribute(type, ipv4 ? IPv4Size : IPv6Size)
{
}


AddressAttribute::AddressAttribute(const AddressAttribute& r)
    : Attribute(r._type, r._size)
    , _address(r._address)
{
}


Attribute* AddressAttribute::clone()
{
    return new AddressAttribute(*this);
}


net::Address AddressAttribute::address() const
{
    return _address;
}


std::string intToIPv4(uint32_t ip)
{
    // Input should be in host network order
    // ip = ntohl(ip);
    char str[20];
    sprintf(str, "%d.%d.%d.%d", (ip >> 24) & 0xff, (ip >> 16) & 0xff,
            (ip >> 8) & 0xff, ip & 0xff);
    return std::string(str);

#if 0
    ostringstream ost;
    ost << ((ip >> 24) & 0xff);
    ost << '.';
    ost << ((ip >> 16) & 0xff);
    ost << '.';
    ost << ((ip >> 8) & 0xff);
    ost << '.';
    ost << ((ip >> 0) & 0xff);
    return ost.str();
#endif
}


void AddressAttribute::read(BitReader& reader)
{
    // X-Port is computed by taking the mapped port in host byte order,
    // XOR'ing it with the most significant 16 bits of the magic cookie, and
    // then the converting the result to network byte order.  If the IP
    // address family is IPv4, X-Address is computed by taking the mapped IP
    // address in host byte order, XOR'ing it with the magic cookie, and
    // converting the result to network byte order.  If the IP address
    // family is IPv6, X-Address is computed by taking the mapped IP address
    // in host byte order, XOR'ing it with the magic cookie and the 96-bit
    // transaction ID, and converting the result to network byte order.

    uint8_t dummy, family;
    reader.getU8(dummy);
    reader.getU8(family);

    uint16_t port;
    reader.getU16(port);
    port = ntohs(port) ^ ntohs(kMagicCookie >> 16); // XOR
    // port ^= (kMagicCookie >> 16);

    if (family == AddressFamily::IPv4) {
        if (size() != IPv4Size) {
            assert(0 && "invalid IPv4 address");
            return;
        }

        uint32_t ip;
        reader.getU32(ip);
        ip = ntohl(ip) ^ ntohl(kMagicCookie); // XOR
        // ip ^= ntohl(kMagicCookie);
        // ip ^= kMagicCookie;

        _address = net::Address(intToIPv4(ntohl(ip)), ntohs(port));
    } else if (family == AddressFamily::IPv6) {
        assert(0 && "IPv6 not supported");
    } else {
        assert(0 && "invalid address");
    }
}


void AddressAttribute::write(BitWriter& writer) const
{
    writer.putU8(0);
    writer.putU8(family());
    // writer.putU8(_family);
    // writer.putU16(_port);
    // writer.putU32(_ip);

    switch (_address.family()) {
        case net::Address::IPv4: {
            auto v4addr = reinterpret_cast<sockaddr_in*>(
                const_cast<sockaddr*>(_address.addr()));

            // Port
            uint16_t port = ntohs(v4addr->sin_port);
            // assert(port == 5555);
            // assert(port == 0x15B3);
            port ^= (kMagicCookie >> 16); // XOR
            // port = port ^ (kMagicCookie >> 16); // XOR
            // assert(port == 0x34A1);
            writer.putU16(port);

            // Address
            uint32_t ip = ntohl(v4addr->sin_addr.s_addr);
            ip ^= kMagicCookie; // XOR
            writer.putU32(ip);
            break;
        }
        case net::Address::IPv6: {
            assert(0 && "IPv6 not supported");
            break;
        }
    }
}


// ---------------------------------------------------------------------------
//
UInt8Attribute::UInt8Attribute(uint16_t type)
    : Attribute(type, Size)
    , _bits(0)
{
}


UInt8Attribute::UInt8Attribute(const UInt8Attribute& r)
    : Attribute(r._type, Size)
    , _bits(r._bits)
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
    _bits &= ~(1 << index);
    _bits |= value ? (1 << index) : 0;
}


void UInt8Attribute::read(BitReader& reader)
{
    reader.getU8(_bits);
}


void UInt8Attribute::write(BitWriter& writer) const
{
    writer.putU8(_bits);
}


// ---------------------------------------------------------------------------
//
UInt32Attribute::UInt32Attribute(uint16_t type)
    : Attribute(type, Size)
    , _bits(0)
{
}


UInt32Attribute::UInt32Attribute(const UInt32Attribute& r)
    : Attribute(r._type, Size)
    , _bits(r._bits)
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
    _bits &= ~(1 << index);
    _bits |= value ? (1 << index) : 0;
}


void UInt32Attribute::read(BitReader& reader)
{
    reader.getU32(_bits);
}

void UInt32Attribute::write(BitWriter& writer) const
{
    writer.putU32(_bits);
}


// ---------------------------------------------------------------------------
//
UInt64Attribute::UInt64Attribute(uint16_t type)
    : Attribute(type, Size)
    , _bits(0)
{
}


UInt64Attribute::UInt64Attribute(const UInt64Attribute& r)
    : Attribute(r._type, Size)
    , _bits(r._bits)
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
    _bits &= ~(1 << index);
    _bits |= value ? (1 << index) : 0;
}


void UInt64Attribute::read(BitReader& reader)
{
    reader.getU64(_bits);
}


void UInt64Attribute::write(BitWriter& writer) const
{
    writer.putU64(_bits);
}


// ---------------------------------------------------------------------------
//
FlagAttribute::FlagAttribute(uint16_t type)
    : Attribute(type, 0)
{
}


Attribute* FlagAttribute::clone()
{
    return new FlagAttribute(type());
}


// ---------------------------------------------------------------------------
//
StringAttribute::StringAttribute(uint16_t type, uint16_t size)
    : Attribute(type, size)
    , _bytes(0)
{
}


StringAttribute::StringAttribute(const StringAttribute& r)
    : Attribute(r._type, r._size)
    , _bytes(0)
{
    copyBytes(r._bytes, r._size);
}


StringAttribute::~StringAttribute()
{
    if (_bytes)
        delete[] _bytes;
}


Attribute* StringAttribute::clone()
{
    return new StringAttribute(*this);
}


void StringAttribute::setBytes(char* bytes, unsigned size)
{
    if (_bytes)
        delete[] _bytes;
    _bytes = bytes;
    setLength(size);
}


void StringAttribute::copyBytes(const char* bytes)
{
    copyBytes(bytes, static_cast<uint16_t>(strlen(bytes)));
}


void StringAttribute::copyBytes(const void* bytes, unsigned size)
{
    char* newBytes = new char[size];
    memcpy(newBytes, bytes, size);
    setBytes(newBytes, size);
}


uint8_t StringAttribute::getByte(int index) const
{
    assert(_bytes != nullptr);
    assert((0 <= index) && (index < size()));
    return static_cast<uint8_t>(_bytes[index]);
}


void StringAttribute::setByte(int index, uint8_t value)
{
    assert(_bytes != nullptr);
    assert((0 <= index) && (index < size()));
    _bytes[index] = value;
}


void StringAttribute::read(BitReader& reader)
{
    if (_bytes)
        delete[] _bytes;
    _bytes = new char[size()];
    reader.get(_bytes, size());

    consumePadding(reader);
}


void StringAttribute::write(BitWriter& writer) const
{
    if (_bytes)
        writer.put(_bytes, size());

    writePadding(writer);
}


std::string StringAttribute::asString() const
{
    return std::string(_bytes, size());
}


/*
//--------------- Fingerprint ----------------
Fingerprint::Fingerprint() :
    UInt32Attribute(Attribute::Fingerprint) {
}


//void Fingerprint::setCRC32(unsigned int crc) {
//    _crc32 = crc;
//}


//unsigned int Fingerprint::crc32() {
//    return _crc32;
//}
*/



// ---------------------------------------------------------------------------
//
MessageIntegrity::MessageIntegrity()
    : Attribute(Attribute::MessageIntegrity, Size)
{
}


MessageIntegrity::MessageIntegrity(const MessageIntegrity& r)
    : Attribute(r._type, Size)
    , _input(r._input)
    , _hmac(r._hmac)
    , _key(r._key)
{
}


MessageIntegrity::~MessageIntegrity()
{
}


Attribute* MessageIntegrity::clone()
{
    return new MessageIntegrity(*this);
}


bool MessageIntegrity::verifyHmac(const std::string& key) const
{
    // LDebug("Message: Verify HMAC: ", key)

    assert(!key.empty());
    assert(!_hmac.empty());
    assert(!_input.empty());

    // LDebug("Message: Packet integrity input (", _input, ")")
    // LDebug("Message: Packet integrity key (", key, ")")

    std::string hmac = crypto::computeHMAC(_input, key);
    assert(hmac.size() == MessageIntegrity::Size);

    // SDebug << "Message: Verifying message integrity (" << hmac << ": " <<
    // _hmac << ")" << endl;

    return _hmac == hmac;
}


void MessageIntegrity::read(BitReader& reader)
{
    // LDebug("Message: Read HMAC")
    int sizeBeforeMessageIntegrity = reader.position() - kAttributeHeaderSize;

    // Read the HMAC value.
    // reader.get(_hmac, MessageIntegrity::Size);

    // _input.assign(reader.begin(), sizeBeforeMessageIntegrity);

    // Get the message prior to the current attribute and fill the
    // attribute with dummy content.
    Buffer hmacBuf(sizeBeforeMessageIntegrity);
    BitWriter hmacWriter(hmacBuf);

    hmacWriter.put(reader.begin(), reader.position() - kAttributeHeaderSize);
    // hmacWriter.put("00000000000000000000");

    // Ensure the STUN message size reflects the message up to and
    // including the MessageIntegrity attribute.
    hmacWriter.updateU32((uint32_t)sizeBeforeMessageIntegrity + MessageIntegrity::Size, 2);
    _input.assign(hmacWriter.begin(), hmacWriter.position());

    _hmac.assign(reader.current(), MessageIntegrity::Size);

    reader.skip(MessageIntegrity::Size);

#if 0
    //LDebug("Message: Parsed message integrity (", _hmac, ")")

    // Remember the original position and set the buffer position back to 0.
    int originalPos = reader.position();
    reader.seek(0);

    // Get the message prior to the current attribute and fill the
    // attribute with dummy content.
    Buffer hmacBuf;
    BitWriter hmacWriter(hmacBuf);

    hmacWriter.put(reader.begin(), reader.available() - MessageIntegrity::Size);
    hmacWriter.put("00000000000000000000");

    // Ensure the STUN message size value represents the real
    // size of the buffered message.
    hmacWriter.updateU32((uint32_t)hmacWriter.position(), 2);
    _input.assign(hmacWriter.begin(), hmacWriter.position());

    // Reset the original buffer position.
    reader.seek(originalPos);
#endif
}


void MessageIntegrity::write(BitWriter& writer) const
{
    // If the key (password) is present then compute the HMAC
    // for the current message, otherwise the attribute content
    // will be copied.
    if (!_key.empty()) {

        // The hash used to construct MESSAGE-INTEGRITY includes the length
        // field from the STUN message header.
        // Prior to performing the hash, the MESSAGE-INTEGRITY attribute MUST be
        // inserted into the message (with dummy content).
        int sizeBeforeMessageIntegrity = writer.position() - kAttributeHeaderSize;

        // Get the message prior to the current attribute and
        // fill the attribute with dummy content.
        //  + MessageIntegrity::Size
        Buffer hmacBuf(sizeBeforeMessageIntegrity);
        BitWriter hmacWriter(hmacBuf);
        hmacWriter.put(writer.begin(), sizeBeforeMessageIntegrity);

        // hmacWriter.put("00000000000000000000");

        // The length MUST then
        // be set to point to the length of the message up to, and including,
        // the MESSAGE-INTEGRITY attribute itself, but excluding any attributes
        // after it.  Once the computation is performed, the value of the
        // MESSAGE-INTEGRITY attribute can be filled in, and the value of the
        // length in the STUN header can be set to its correct value -- the
        // length of the entire message.  Similarly, when validating the
        // MESSAGE-INTEGRITY, the length field should be adjusted to point to
        // the end of the MESSAGE-INTEGRITY attribute prior to calculating the
        // HMAC.  Such adjustment is necessary when attributes, such as
        // FINGERPRINT, appear after MESSAGE-INTEGRITY.
        hmacWriter.updateU32((uint32_t)sizeBeforeMessageIntegrity + MessageIntegrity::Size, 2);

        std::string input(hmacWriter.begin(), hmacWriter.position());
        assert(input.size() == sizeBeforeMessageIntegrity); // + MessageIntegrity::Size

        // std::string input(writer.begin(), sizeBeforeMessageIntegrity);
        std::string hmac(crypto::computeHMAC(input, _key));
        assert(hmac.size() == MessageIntegrity::Size);

        // Append the real HAMC to the buffer.
        writer.put(hmac.c_str(), hmac.length());

#if 0
        // Get the message prior to the current attribute and
        // fill the attribute with dummy content.
        Buffer hmacBuf; // TODO: alloc exact size
        BitWriter hmacWriter(hmacBuf);
        hmacWriter.put(writer.begin(), writer.position());
        hmacWriter.put("00000000000000000000");

        // Ensure the STUN message size value represents the real
        // size of the buffered message.
        hmacWriter.updateU32((uint32_t)hmacWriter.position() + MessageIntegrity::Size, 2);

        //string input(hmacBuf.data(), hmacBuf.available());
        std::string input(hmacWriter.begin(), hmacWriter.position());
        std::string hmac(crypto::computeHMAC(input, _key));
        assert(hmac.size() == MessageIntegrity::Size);

        // Append the real HAMC to the buffer.
        writer.put(hmac.c_str(), MessageIntegrity::Size);
#endif
    } else {
        assert(_hmac.size() == MessageIntegrity::Size);
        writer.put(_hmac.c_str(), MessageIntegrity::Size);
    }
}


// ---------------------------------------------------------------------------
//
ErrorCode::ErrorCode(uint16_t size)
    : Attribute(Attribute::ErrorCode, size)
    , _class(0)
    , _number(0)
{
    assert(size >= MinSize);
}


ErrorCode::ErrorCode(const ErrorCode& r)
    : Attribute(Attribute::ErrorCode, r._size)
    , _class(r._class)
    , _number(r._number)
    , _reason(r._reason)
{
}


ErrorCode::~ErrorCode()
{
}


Attribute* ErrorCode::clone()
{
    return new ErrorCode(*this);
}


int ErrorCode::errorCode() const
{
    return _class * 100 + _number;
}


void ErrorCode::setErrorCode(int code)
{
    _class = static_cast<uint8_t>(code / 100);
    _number = static_cast<uint8_t>(code % 100);
}


void ErrorCode::setReason(const std::string& reason)
{
    setLength(MinSize + static_cast<uint16_t>(reason.size()));
    _reason = reason;
}


void ErrorCode::read(BitReader& reader)
{
    uint32_t val;
    reader.getU32(val);

    if ((val >> 11) != 0)
        throw std::runtime_error("error-code bits not zero");

    _class = ((val >> 8) & 0x7);
    _number = (val & 0xff);

    reader.get(_reason, size() - 4);
    consumePadding(reader);
}


void ErrorCode::write(BitWriter& writer) const
{
    writer.putU32(_class << 8 | _number); // errorCode());
    writer.put(_reason);
    writePadding(writer);
}


// ---------------------------------------------------------------------------
//
UInt16ListAttribute::UInt16ListAttribute(uint16_t type, uint16_t size)
    : Attribute(type, size)
{
}


UInt16ListAttribute::UInt16ListAttribute(const UInt16ListAttribute& r)
    : Attribute(r._type, r._size)
    , _attrTypes(r._attrTypes)
{
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


uint16_t UInt16ListAttribute::getType(int index) const
{
    return _attrTypes[index];
}


void UInt16ListAttribute::setType(int index, uint16_t value)
{
    _attrTypes[index] = value;
}


void UInt16ListAttribute::addType(uint16_t value)
{
    _attrTypes.push_back(value);
    setLength(static_cast<uint16_t>(_attrTypes.size() * 2));
}


void UInt16ListAttribute::read(BitReader& reader)
{
    for (unsigned i = 0; i < size() / 2; i++) {
        uint16_t attr;
        reader.getU16(attr);
        _attrTypes.push_back(attr);
    }

    // Padding of these attributes is done in RFC 5389 style. This is
    // slightly different from RFC3489, but it shouldn't be important.
    // RFC3489 pads out to a 32 bit boundary by duplicating one of the
    // entries in the list (not necessarily the last one - it's unspecified).
    // RFC5389 pads on the end, and the bytes are always ignored.
    consumePadding(reader);
}

void UInt16ListAttribute::write(BitWriter& writer) const
{
    for (unsigned i = 0; i < _attrTypes.size(); i++)
        writer.putU16(_attrTypes[i]);
    writePadding(writer);
}


} } // namespace scy:stun


/// @\}
