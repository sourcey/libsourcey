///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup stun
/// @{


#include "scy/stun/message.h"
#include "scy/logger.h"
#include "scy/util.h"


using namespace std;


namespace scy {
namespace stun {


Message::Message()
    : _class(Request)
    , _method(Undefined)
    , _size(0)
    , _transactionID(util::randomString(kTransactionIdLength))
{
    assert(_transactionID.size() == kTransactionIdLength);
}


Message::Message(ClassType clss, MethodType meth)
    : _class(clss)
    , _method(meth)
    , _size(0)
    , _transactionID(util::randomString(kTransactionIdLength))
{
}


Message::Message(const Message& that)
    : _class(that._class)
    , _method(that._method)
    , _size(that._size)
    , _transactionID(that._transactionID)
{
    assert(_method);
    assert(_transactionID.size() == kTransactionIdLength);

    // Copy attributes from source object
    for (unsigned i = 0; i < that.attrs().size(); i++)
        _attrs.push_back(that.attrs()[i]->clone());
}


Message& Message::operator=(const Message& that)
{
    if (&that != this) {
        _method = that._method;
        _class = that._class;
        _size = that._size;
        _transactionID = that._transactionID;
        assert(_method);
        assert(_transactionID.size() == kTransactionIdLength);

        // Clear current attributes
        for (unsigned i = 0; i < _attrs.size(); i++)
            delete _attrs[i];
        _attrs.clear();

        // Copy attributes from source object
        for (unsigned i = 0; i < that.attrs().size(); i++)
            _attrs.push_back(that.attrs()[i]->clone());
    }

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


void Message::add(Attribute* attr)
{
    _attrs.push_back(attr);
    size_t attrLength = attr->size();
    if (attrLength % 4 != 0)
        attrLength += (4 - (attrLength % 4));
    _size += attrLength + kAttributeHeaderSize;
    //_size += attr->size() + kAttributeHeaderSize;
}


Attribute* Message::get(Attribute::Type type, int index) const
{
    for (unsigned i = 0; i < _attrs.size(); i++) {
        if (_attrs[i]->type() == type) {
            if (index == 0)
                return _attrs[i];
            else
                index--;
        }
    }
    return nullptr;
}


ssize_t Message::read(const ConstBuffer& buf)
{
    LTrace("Parse STUN packet: ", buf.size())

    try {
        BitReader reader(buf);

        // Message type
        uint16_t type;
        reader.getU16(type);
        if (type & 0x8000) {
            // RTP and RTCP set MSB of first byte, since first two bits are version,
            // and version is always 2 (10). If set, this is not a STUN packet.
            LWarn("Not STUN packet")
            return 0;
        }

        // uint16_t method = (type & 0x000F) | ((type & 0x00E0)>>1) |
        //    ((type & 0x0E00)>>2) | ((type & 0x3000)>>2);

        uint16_t classType = type & 0x0110;
        uint16_t methodType = type & 0x000F;
        if (!isValidMethod(methodType)) {
            LWarn("STUN message unknown method: ", methodType)
            return 0;
        }

        _class = classType;   // static_cast<uint16_t>(type & 0x0110);
        _method = methodType; // static_cast<uint16_t>(type & 0x000F);

        // Message length
        reader.getU16(_size);
        if (_size > buf.size()) {
            LWarn("STUN message larger than buffer: " ,  _size,  " > ", buf.size())
            return 0;
        }

        // TODO: Check valid method
        // TODO: Parse message class (Message::State)

        // Magic cookie
        reader.skip(kMagicCookieLength);
        // std::string magicCookie;
        // reader.get(magicCookie, kMagicCookieLength);

        // Transaction ID
        std::string transactionID;
        reader.get(transactionID, kTransactionIdLength);
        assert(transactionID.size() == kTransactionIdLength);
        _transactionID = transactionID;

        // Attributes
        _attrs.clear();
        // int errors = 0;
        int rest = _size;
        uint16_t attrType, attrLength, padLength;
        assert(int(reader.available()) >= rest);
        while (rest > 0) {
            reader.getU16(attrType);
            reader.getU16(attrLength);
            padLength = attrLength % 4 == 0 ? 0 : 4 - (attrLength % 4);

            auto attr = Attribute::create(attrType, attrLength);
            if (attr) {
                attr->read(reader); // parse or throw
                _attrs.push_back(attr);

                // STrace << "Parse attribute: " << Attribute::typeString(attrType) << ": " 
                //    << attrLength << endl;
            } else
                SWarn << "Failed to parse attribute: "
                      << Attribute::typeString(attrType) << ": " << attrLength
                      << endl;

            rest -= (attrLength + kAttributeHeaderSize + padLength);
        }

        LTrace("Parse success: ", reader.position(), ": ", buf.size())
        assert(rest == 0);
        assert(reader.position() == _size + kMessageHeaderSize);
        return reader.position();
    } catch (std::exception& exc) {
        LDebug("Parse error: ", exc.what())
    }

    return 0;
}


void Message::write(Buffer& buf) const
{
    // assert(_method);
    // assert(_size);

    // BitWriter writer(buf);
    DynamicBitWriter writer(buf);
    writer.putU16((uint16_t)(_class | _method));
    writer.putU16(_size);
    writer.putU32(kMagicCookie);
    writer.put(_transactionID.c_str(), _transactionID.size());

    // Note: MessageIntegrity must be at the end

    for (unsigned i = 0; i < _attrs.size(); i++) {
        writer.putU16(_attrs[i]->type());
        writer.putU16(_attrs[i]->size());
        _attrs[i]->write(writer);
    }
}


std::string Message::classString() const
{
    switch (_class) {
        case Request:
            return "Request";
        case Indication:
            return "Indication";
        case SuccessResponse:
            return "SuccessResponse";
        case ErrorResponse:
            return "ErrorResponse";
        default:
            return "UnknownState";
    }
}


std::string Message::errorString(uint16_t errorCode) const
{
    switch (errorCode) {
        case BadRequest:
            return "BAD REQUEST";
        case NotAuthorized:
            return "UNAUTHORIZED";
        case UnknownAttribute:
            return "UNKNOWN ATTRIBUTE";
        case StaleCredentials:
            return "STALE CREDENTIALS";
        case IntegrityCheckFailure:
            return "INTEGRITY CHECK FAILURE";
        case MissingUsername:
            return "MISSING USERNAME";
        case UseTLS:
            return "USE TLS";
        case RoleConflict:
            return "Role Conflict"; // (487) rfc5245
        case ServerError:
            return "SERVER ERROR";
        case GlobalFailure:
            return "GLOBAL FAILURE";
        case ConnectionAlreadyExists:
            return "Connection Already Exists";
        case ConnectionTimeoutOrFailure:
            return "Connection Timeout or Failure";
        default:
            return "UnknownError";
    }
}


std::string Message::methodString() const
{
    switch (_method) {
        case Binding:
            return "BINDING";
        case Allocate:
            return "ALLOCATE";
        case Refresh:
            return "REFRESH";
        case SendIndication:
            return "SEND-INDICATION";
        case DataIndication:
            return "DATA-INDICATION";
        case CreatePermission:
            return "CREATE-PERMISSION";
        case ChannelBind:
            return "CHANNEL-BIND";
        case Connect:
            return "CONNECT";
        case ConnectionBind:
            return "CONNECTION-BIND";
        case ConnectionAttempt:
            return "CONNECTION-ATTEMPT";
        default:
            return "UnknownMethod";
    }
}


std::string Message::toString() const
{
    std::ostringstream os;
    os << "STUN[" << methodString() << ":" << transactionID();
    for (unsigned i = 0; i < _attrs.size(); i++)
        os << ":" << _attrs[i]->typeString();
    os << "]";
    return os.str();
}


void Message::print(std::ostream& os) const
{
    os << "STUN[" << methodString() << ":" << transactionID();
    for (unsigned i = 0; i < _attrs.size(); i++)
        os << ":" << _attrs[i]->typeString();
    os << "]";
}


void Message::setTransactionID(const std::string& id)
{
    assert(id.size() == kTransactionIdLength);
    _transactionID = id;
}


Message::ClassType Message::classType() const
{
    return static_cast<ClassType>(_class);
}


Message::MethodType Message::methodType() const
{
    return static_cast<MethodType>(_method);
}


void Message::setClass(ClassType type)
{
    _class = type;
}

void Message::setMethod(MethodType type)
{
    _method = type;
}
}
} // namespace scy:stun


/// @\}
