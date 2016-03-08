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


#include "scy/socketio/packet.h"
#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace sockio {


Packet::Packet(Frame frame, Type type, int id, const std::string& nsp, const std::string& event, const std::string& message, bool ack) :
    _frame(frame),
    _type(type),
    _id(id),
    _nsp(nsp),
    _event(event),
    _message(message),
    _ack(ack),
    _size(0)
{
}


Packet::Packet(Type type, const std::string& message, bool ack) :
    Packet(Frame::Message, type, util::randomNumber(), "/", "message", message, ack)
{
}


Packet::Packet(const std::string& message, bool ack) :
    Packet(Frame::Message, Type::Event, util::randomNumber(), "/", "message", message, ack)
{
}


Packet::Packet(const json::Value& message, bool ack) :
    Packet(Frame::Message, Type::Event, util::randomNumber(), "/", "message", json::stringify(message), ack)
{
}


Packet::Packet(const std::string& event, const std::string& message, bool ack) :
    Packet(Frame::Message, Type::Event, util::randomNumber(), "/", event, message, ack)
{
    // json::Value root;
    // root["name"] = event;
    //
    // // add the data into an array if it isn't already
    // if (!data.isArray()) {
    //     json::Value array(Json::arrayValue);
    //     array.append(data);
    //     root["args"] = array;
    // }
    // else
    //     root["args"] = data;
    //
    // _message = json::stringify(root);
}


Packet::Packet(const std::string& event, const json::Value& data, bool ack) :
    Packet(Frame::Message, Type::Event, util::randomNumber(), "/", event, json::stringify(data), ack)
{
    // assert(_id);
    //
    // json::Value root;
    // root["name"] = event;
    //
    // // add the data into an array if it isn't already
    // if (!data.isArray()) {
    //     json::Value array(Json::arrayValue);
    //     array.append(data);
    //     root["args"] = array;
    // }
    // else
    //     root["args"] = data;
    //
    // _message = json::stringify(root);
}


Packet::Packet(const Packet& r) :
    _frame(r._frame),
    _type(r._type),
    _id(r._id),
    _nsp(r._nsp),
    _event(r._event),
    _message(r._message),
    _ack(r._ack),
    _size(r._size)
{
}


Packet& Packet::operator = (const Packet& r)
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


std::size_t Packet::read(const ConstBuffer& buf)
{
    // DebugN(this) << "Read raw packet: " << std::string(bufferCast<const char*>(buf), buf.size()) << endl;
    assert(buf.size() > 0);

    // Reset all data
    _frame = Frame::Unknown;
    _type = Type::Unknown;
    _id = -1;
    _nsp = "/";
    _message = "";
    _size = -1;

    BitReader reader(buf);

    // look up frame type
    char frame;
    reader.get(&frame, 1);
    _frame = static_cast<Packet::Frame>(atoi(&frame));

    if (_frame == Packet::Frame::Message) {

        // look up packet type
        char type;
        reader.get(&type, 1);
        _type = static_cast<Packet::Type>(atoi(&type));
        // if (_type < TypeMin || _type > TypeMax) {
        //     WarnN(this) << "Invalid message type: " << _type << endl;
        //     return false;
        // }

        // TraceN(this) << "Parse type: " << type << ": " << typeString() << endl;
    }

    // look up attachments if type binary (not implemented)

    // look up namespace (if any)
    if (reader.peek() == '/') {
        reader.readToNext(_nsp, ',');
    }

    // look up id
    if (reader.available() && isdigit(reader.peek())) {
        char next;
        std::string id;
        reader.get(&next, 1);
        while (reader.available() && isdigit(next)) {
            id += next;
            reader.get(&next, 1);
        }
        _id = util::strtoi<int>(id);
    }

    // look up json data
    // TODO: Take into account joined messages
    if (reader.available()) {
        reader.get(_message, reader.available());
    }

    _size = reader.position();

    DebugN(this) << "Parse success: " << toString() << endl;

#if 0 // Socket.IO 0.9x
    std::string data(bufferCast<const char*>(buf), buf.size());
    std::vector<std::string> frags = util::split(data, ':', 4);
    if (frags.size() < 1) {
        //DebugN(this) << "Reading: Invalid Data: " << frags.size() << endl;
        return false;
    }

    if (!frags[0].empty()) {
        _type = util::strtoi<std::uint32_t>(frags[0]);
        //DebugN(this) << "Reading: Type: " << typeString() << endl;
    }

    if (_type < 0 || _type > 7) {
        //DebugN(this) << "Reading: Invalid Type: " << typeString() << endl;
        return false;
    }
    if (frags.size() >= 2 && !frags[1].empty()) {
        _ack = (frags[1].find('+') != std::string::npos);
        _id = util::strtoi<std::uint32_t>(frags[1]);
    }
    if (frags.size() >= 3 && !frags[2].empty()) {
        _nsp = frags[2];
    }
    if (frags.size() >= 4 && !frags[3].empty()) {
        _message = frags[3];
    }

    // For Ack packets the ID is at the start of the message
    if (_type == 6) {
        _ack = true; // This flag is mostly for requests, but we'll set it anyway

        std::string data(frags[frags.size() - 1]);
        std::string::size_type pos = data.find('+');
        if (pos != std::string::npos)
        {    // complex ack
            _id = util::strtoi<std::uint32_t>(data.substr(0, pos));
            _message = data.substr(pos + 1, data.length());
        }
        else
        {    // simple ack
            _message = data;
        }
#endif

#if 0
        frags.clear();
        util::split(_message, '+', frags, 2);
        if (frags.size() != 2) {
            assert(frags.size() == 2 && "invalid ack response");
            return false;
        }

        _ack = true; // This is mostly for requests, but we'll set it anyway
        _id = util::strtoi<std::uint32_t>(frags[0]);
        _message = frags[1];
    }
#endif

    return _size;
}


void Packet::write(Buffer& buf) const
{
    assert(valid());
    std::ostringstream ss;
    // print(ss);

    // 2["message",{"data":"fffffffffffffffffffff","type":"message","id":"k0dsiifb169cz0k9","from":"aaa|/#Zr0vTHQ4JG2Zt-qtAAAA"}]
    ss << int(_frame)
        << int(_type)
        << _id;

    if (_type == Type::Event) {
      ss << "[\""
        << (_event.empty() ? "message" : _event)
        << "\","
        << _message
        << "]";
        // << "\",\""
        // << _message
        // << "\"]";
    }
        // << "[\"message\",\""

    std::string str(ss.str()); // TODO: avoid copy
    buf.insert(buf.end(), str.begin(), str.end());
    //buf.append(ss.str().c_str(), ss.tellp());
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


json::Value Packet::json() const
{
    if (!_message.empty()) {
        json::Value data;
        json::Reader reader;
        if (reader.parse(_message, data))
            return data;
    }
    return json::Value();
}


std::string Packet::frameString() const
{
    switch (_frame) {
    case Frame::Open: return "Open";
    case Frame::Close: return "Close";
    case Frame::Ping: return "Ping";
    case Frame::Pong: return "Pong";
    case Frame::Message: return "Message";
    case Frame::Upgrade: return "Upgrade";
    case Frame::Noop: return "Noop";
    // case Unknown: return "unknown";
    default: return "unknown";
    }
}


std::string Packet::typeString() const
{
    switch (_type) {
    case Type::Connect: return "Connect";
    case Type::Disconnect: return "Disconnect";
    case Type::Event: return "Event";
    case Type::Ack: return "Ack";
    case Type::Error: return "Error";
    case Type::BinaryEvent: return "BinaryEvent";
    case Type::BinaryAck: return "BinaryAck";
    // case Unknown: return "unknown";
    default: return "unknown";
    }
}


std::string Packet::toString() const
{
    std::ostringstream ss;
    print(ss);
    //ss << endl;
    return ss.str();
}


bool Packet::valid() const
{
    // Check that ID and correct type have been set
    return int(_type) >= int(Type::TypeMin) && int(_type) <= int(Type::TypeMax) && _id > 0;
}


std::size_t Packet::size() const
{
    std::ostringstream ss;
    print(ss);
    assert(ss.tellp());
    return static_cast<std::size_t>(ss.tellp());
}


void Packet::print(std::ostream& os) const
{
    os << frameString()
        << ":"
        << typeString();
    if (_id == -1 && _nsp.empty() && _message.empty()) {
        os << "::";
    }
    else if (_id == -1 && _nsp.empty()){
        os << ":::" << _message;
    }
    else if (_id > -1) { // && _type != 6
        os << ":" << _id << (_ack ? "+":"")
            << ":" << _nsp << ":" << _message;
    }
    else {
        os << "::" << _nsp << ":" << _message;
    }
}


} } // namespace scy::sockio
