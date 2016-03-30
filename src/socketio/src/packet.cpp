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
}


Packet::Packet(const std::string& event, const json::Value& data, bool ack) :
    Packet(Frame::Message, Type::Event, util::randomNumber(), "/", event, json::stringify(data), ack)
{
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
    // DebugN(this) << "Read raw packet: " << buf.str() << endl;
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
        std::string temp;
        reader.get(temp, reader.available());
		TraceN(this) << "SOCKETIO PACKET: available: " << temp << endl;

        json::Value json;
        json::Reader reader;
        if (reader.parse(temp, json)) {
            if (json.isArray()) {
                if (json.size() < 2) {
                    _event = "message";
                    _message = json::stringify(json[0], true);
                }
                else {
                    assert(json[0].isString());
                _event = json[0].asString();
                _message = json::stringify(json[1], true);

                }
            }
            else if (json.isObject()) {
                _message = json::stringify(json, true);
            }
        }
    }

    _size = reader.position();

    // DebugN(this) << "Parse success: " << toString() << endl;

    return _size;
}


void Packet::write(Buffer& buf) const
{
    assert(valid());
    std::ostringstream os;
    print(os);
    std::string str = os.str();
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


json::Value Packet::json() const
{
    if (!_message.empty()) {
        json::Value data;
        json::Reader reader;
        if (reader.parse(_message, data)) {
            return data; //[1];
        }
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
    // 2["message",{"data":"fffffffffffffffffffff","type":"message","id":"k0dsiifb169cz0k9","from":"aaa|/#Zr0vTHQ4JG2Zt-qtAAAA"}]
    os << int(_frame)
        << int(_type)
        << _id;

    if (_type == Type::Event) {
      os << "[\""
        << (_event.empty() ? "message" : _event)
        << "\","
        << _message
        << "]";
        // << "\",\""
        // << _message
        // << "\"]";
    }
}


} } // namespace scy::sockio
