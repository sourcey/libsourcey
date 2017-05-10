///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup stun
/// @{


#ifndef SCY_STUN_Message_H
#define SCY_STUN_Message_H


#include "scy/packet.h"
#include "scy/stun/attributes.h"
#include "scy/stun/stun.h"


namespace scy {
namespace stun {


typedef std::string TransactionID;


class STUN_API Message : public IPacket
{
public:
    enum MethodType
    {
        Undefined = 0x0000,        ///< default error type

        /// STUN
        Binding = 0x0001,

        /// TURN
        Allocate = 0x0003,         ///< (only request/response semantics defined)
        Refresh = 0x0004,
        SendIndication = 0x0006,   ///< (only indication semantics defined)
        DataIndication = 0x0007,   ///< (only indication semantics defined)
        CreatePermission = 0x0008, ///< (only request/response semantics defined)
        ChannelBind = 0x0009,      ///< (only request/response semantics defined)

        /// TURN TCP RFC 6062
        Connect = 0x000a,
        ConnectionBind = 0x000b,
        ConnectionAttempt = 0x000c
    };

    enum ClassType
    {
        Request = 0x0000,
        Indication = 0x0010,
        SuccessResponse = 0x0100,
        ErrorResponse = 0x0110
    };

    enum ErrorCodes
    {
        BadRequest = 400,
        NotAuthorized = 401,
        UnknownAttribute = 420,
        StaleCredentials = 430,
        IntegrityCheckFailure = 431,
        MissingUsername = 432,
        UseTLS = 433,
        RoleConflict = 487,
        ServerError = 500,
        GlobalFailure = 600,

        /// TURN TCP
        ConnectionAlreadyExists = 446,
        ConnectionTimeoutOrFailure = 447
    };

public:
    Message();
    Message(ClassType clss, MethodType meth);
    Message(const Message& that);
    Message& operator=(const Message& that);
    virtual ~Message();

    virtual IPacket* clone() const;

    void setClass(ClassType type);
    void setMethod(MethodType type);
    void setTransactionID(const std::string& id);

    ClassType classType() const;
    MethodType methodType() const;
    const TransactionID& transactionID() const { return _transactionID; }
    const std::vector<Attribute*> attrs() const { return _attrs; }
    size_t size() const { return static_cast<size_t>(_size); }

    std::string methodString() const;
    std::string classString() const;
    std::string errorString(uint16_t errorCode) const;

    void add(Attribute* attr);
    Attribute* get(Attribute::Type type, int index = 0) const;

    template <typename T> T* get(int index = 0) const
    {
        return reinterpret_cast<T*>(
            get(static_cast<Attribute::Type>(T::TypeID), index));
    }

    /// Parses the STUN/TURN packet from the given buffer.
    /// The return value indicates the number of bytes read.
    ssize_t read(const ConstBuffer& buf);

    /// Writes this object into a STUN/TURN packet.
    void write(Buffer& buf) const;

    std::string toString() const;
    void print(std::ostream& os) const;

    virtual const char* className() const { return "StunMessage"; }

protected:
    uint16_t _class;
    uint16_t _method;
    uint16_t _size;
    TransactionID _transactionID;
    std::vector<Attribute*> _attrs;
};


inline bool isValidMethod(uint16_t methodType)
{
    switch (methodType) {
        case Message::Binding:
        case Message::Allocate:
        case Message::Refresh:
        case Message::SendIndication:
        case Message::DataIndication:
        case Message::CreatePermission:
        case Message::ChannelBind:
        case Message::Connect:
        case Message::ConnectionBind:
        case Message::ConnectionAttempt:
            return true;
    }
    return false;
}
}
} // namespace scy:stun


#endif // SCY_STUN_Message_H


/// @\}
