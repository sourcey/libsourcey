///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup socketio
/// @{


#include "scy/socketio/packet.h"
#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace sockio {


Packet::Packet(Frame frame, Type type, int id, const std::string& nsp,
               const std::string& event, const std::string& message, bool ack)
    : _frame(frame)
    , _type(type)
    , _id(id)
    , _nsp(nsp)
    , _event(event)
    , _message(message)
    , _ack(ack)
    , _size(0)
{
}


Packet::Packet(Type type, const std::string& message, bool ack)
    : Packet(Frame::Message, type, util::randomNumber(), "/", "message", message, ack)
{
}


Packet::Packet(const std::string& message, bool ack)
    : Packet(Frame::Message, Type::Event, util::randomNumber(), "/", "message", message, ack)
{
}


Packet::Packet(const json::value& message, bool ack)
    : Packet(Frame::Message, Type::Event, util::randomNumber(), "/", "message", message.dump(), ack)
{
}


Packet::Packet(const std::string& event, const std::string& message, bool ack)
    : Packet(Frame::Message, Type::Event, util::randomNumber(), "/", event, message, ack)
{
}


Packet::Packet(const std::string& event, const json::value& data, bool ack)
    : Packet(Frame::Message, Type::Event, util::randomNumber(), "/", event, data.dump(), ack)
{
}


Packet::Packet(const Packet& r)
    : _frame(r._frame)
    , _type(r._type)
    , _id(r._id)
    , _nsp(r._nsp)
    , _event(r._event)
    , _message(r._message)
    , _ack(r._ack)
    , _size(r._size)
{
}


Packet& Packet::operator=(const Packet& r)
{
    _frame = r._frame;
    _type = r._type;
    _id = r._id;
    _nsp = r._nsp;
    _event = r._event;
    _message = r._message;
    _ack = r._ack;
    _size = r._size;
    return *this;
}


Packet::~Packet()
{
}


IPacket* Packet::clone() const
{
    return new Packet(*this);
}


ssize_t Packet::read(const ConstBuffer& buf)
{
    LTrace("Read raw packet: ", buf.str())
    assert(buf.size() > 0);

    // Reset all data
    _frame = Frame::Unknown;
    _type = Type::Unknown;
    _id = -1;
    _nsp = "/";
    _message = "";
    _size = 0;

    BitReader reader(buf);

    // parse frame type
    char frame[2] = {'\0'};
    reader.get(frame, 1);
    _frame = static_cast<Packet::Frame>(atoi(frame)); // std::stoi(std::string(frame, 1))

    if (_frame == Packet::Frame::Message) {

        // parse packet type
        char type[2] = {'\0'};
        reader.get(type, 1);
        _type = static_cast<Packet::Type>(atoi(type)); // std::stoi(std::string(type, 1))
        // if (_type < TypeMin || _type > TypeMax) {
        //     LWarn("Invalid message type: ", _type)
        //     return false;
        // }

        // LTrace("Parse type: ",  type,  ": ", typeString())
    }

    // parse attachments if type binary (not implemented)

    // parse namespace (if any)
    if (reader.peek() == '/') {
        reader.readToNext(_nsp, ',');
    }

    // parse id
    reader.readNextNumber((unsigned int&)_id);

    // parse json data
    // TODO: Take into account joined messages
    if (reader.available()) {
        std::string temp;
        reader.get(temp, reader.available());

        json::value json = json::value::parse(temp.begin(), temp.end());
        if (json.is_array()) {
            if (json.size() < 2) {
                _event = "message";
                _message = json[0].dump();
            } else {
                assert(json[0].is_string());
                _event = json[0].get<std::string>();
                _message = json[1].dump();
            }
        } 
        else if (json.is_object()) {
            _message = json.dump();
        }
    }

    _size = reader.position();

    // LDebug("Parse success: ", toString())

    return _size;
}


void Packet::write(Buffer& buf) const
{
    assert(valid());
    std::ostringstream os;
    print(os);
    std::string str(os.str());
    buf.insert(buf.end(), str.begin(), str.end());
}


void Packet::setID(int id)
{
    _id = id;
}


void Packet::setNamespace(const std::string& nsp)
{
    _nsp = nsp;
}


void Packet::setMessage(const std::string& message)
{
    _message = message;
}


void Packet::setAck(bool flag)
{
    _ack = flag;
}


Packet::Frame Packet::frame() const
{
    return _frame;
}


Packet::Type Packet::type() const
{
    return _type;
}


int Packet::id() const
{
    return _id;
}


std::string Packet::nsp() const
{
    return _nsp;
}


std::string Packet::message() const
{
    return _message;
}


json::value Packet::json() const
{
    if (!_message.empty()) {
        return json::value::parse(_message.begin(), _message.end());
        //json::value data;
        //json::Reader reader;
        //if (reader.parse(_message, data)) {
        //    return data; //[1];
        //}
    }
    return json::value();
}


std::string Packet::frameString() const
{
    switch (_frame) {
        case Frame::Open:
            return "Open";
        case Frame::Close:
            return "Close";
        case Frame::Ping:
            return "Ping";
        case Frame::Pong:
            return "Pong";
        case Frame::Message:
            return "Message";
        case Frame::Upgrade:
            return "Upgrade";
        case Frame::Noop:
            return "Noop";
        // case Unknown: return "unknown";
        default:
            return "unknown";
    }
}


std::string Packet::typeString() const
{
    switch (_type) {
        case Type::Connect:
            return "Connect";
        case Type::Disconnect:
            return "Disconnect";
        case Type::Event:
            return "Event";
        case Type::Ack:
            return "Ack";
        case Type::Error:
            return "Error";
        case Type::BinaryEvent:
            return "BinaryEvent";
        case Type::BinaryAck:
            return "BinaryAck";
        // case Unknown: return "unknown";
        default:
            return "unknown";
    }
}


std::string Packet::toString() const
{
    std::ostringstream ss;
    print(ss);
    // ss << endl;
    return ss.str();
}


bool Packet::valid() const
{
    // Check that ID and correct type have been set
    return int(_type) >= int(Type::TypeMin) &&
           int(_type) <= int(Type::TypeMax) && _id > 0;
}


size_t Packet::size() const
{
    std::ostringstream ss;
    print(ss);
    assert(ss.tellp());
    return static_cast<size_t>(ss.tellp());
}


void Packet::print(std::ostream& os) const
{
    // 2["message",{"data":"fffffffffffffffffffff","type":"message","id":"k0dsiifb169cz0k9","from":"aaa|/#Zr0vTHQ4JG2Zt-qtAAAA"}]
    os << int(_frame) << int(_type) << _id;

    if (_type == Type::Event) {
        os << "[\"" << (_event.empty() ? "message" : _event) << "\","
           << _message << "]";
        // << "\",\""
        // << _message
        // << "\"]";
    }
}


} // namespace sockio
} // namespace scy


/// @\}
