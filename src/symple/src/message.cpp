///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup symple
/// @{

#include "scy/symple/message.h"
#include "assert.h"
#include "scy/logger.h"
#include "scy/util.h"

using std::endl;

namespace scy {
namespace smpl {

Message::Message()
    : json::Value(Json::objectValue)
{
    (*this)["id"] = util::randomString(16);
    (*this)["type"] = "message";
}

Message::Message(const Message& root)
    : json::Value(root)
{
    if (!isMember("id"))
        (*this)["id"] = util::randomString(16);
    if (!isMember("type"))
        (*this)["type"] = "message";
}

Message::Message(const json::Value& root)
    : json::Value(root)
{
    if (!isMember("id"))
        (*this)["id"] = util::randomString(16);
    if (!isMember("type"))
        (*this)["type"] = "message";
}

Message::~Message()
{
}

IPacket* Message::clone() const
{
    return new Message(*this);
}

std::size_t Message::read(const ConstBuffer& buf)
{
    return read(buf.str()); // refactor
}

std::size_t Message::read(const std::string& root)
{
    json::Reader reader;
    return reader.parse(root, *this) ? root.length() : 0;
}

void Message::write(Buffer& buf) const
{
    std::string data(json::stringify(*this));

    // buf.append(data.c_str(), data.size());
    buf.insert(buf.end(), data.begin(), data.end());
}

std::size_t Message::size() const
{
    // KLUDGE: is there a better way?
    return json::stringify(*this).size();
}

void Message::print(std::ostream& os) const
{
    os << json::stringify(*this, true);
}

bool Message::valid() const
{
    return isMember("type") && isMember("id") && isMember("from") &&
           (*this)["from"].isString();
}

void Message::clear()
{
    json::Value::clear();
}

void Message::clearData()
{
    (*this)["data"].clear();
}

void Message::clearNotes()
{
    (*this)["notes"].clear();
}

std::string Message::type() const
{
    return get("type", "message").asString();
}

std::string Message::id() const
{
    return get("id", "").asString();
}

Address Message::to() const
{
    return Address(get("to", "").asString());
}

Address Message::from() const
{
    return Address(get("from", "").asString());
}

int Message::status() const
{
    return isMember("status") ? (*this)["status"].asInt() : -1;
}

bool Message::isRequest() const
{
    return status() == -1;
}

json::Value& Message::notes()
{
    return (*this)["notes"];
}

json::Value Message::data(const std::string& name) const
{
    return (*this)["data"][name];
}

json::Value& Message::data(const std::string& name)
{
    return (*this)["data"][name];
}

void Message::setType(const std::string& type)
{
    (*this)["type"] = type;
}

void Message::setTo(const Peer& to)
{
    (*this)["to"] = to.address().toString();
}

void Message::setTo(const Address& to)
{
    (*this)["to"] = to.toString();
}

void Message::setTo(const std::string& to)
{
    (*this)["to"] = to;
}

void Message::setFrom(const Peer& from)
{
    (*this)["from"] = from.address().toString();
}

void Message::setFrom(const Address& from)
{
    (*this)["from"] = from.toString();
}

void Message::setFrom(const std::string& from)
{
    (*this)["from"] = from;
}

void Message::setStatus(int code)
{
    assert(code > 100 && code < 505);
    (*this)["status"] = code;
}

void Message::setNote(const std::string& type, const std::string& text)
{
    clearNotes();
    addNote(type, text);
}

void Message::addNote(const std::string& type, const std::string& text)
{
    assert(type == "info" || type == "warn" || type == "error");

    json::Value note;
    note["type"] = type;
    note["text"] = text;
    (*this)["notes"].append(note);
}

json::Value& Message::setData(const std::string& name)
{
    return (*this)["data"][name] = name;
}

void Message::setData(const std::string& name, const char* data)
{
    (*this)["data"][name] = data;
}

void Message::setData(const std::string& name, const std::string& data)
{
    (*this)["data"][name] = data;
}

void Message::setData(const std::string& name, const json::Value& data)
{
    (*this)["data"][name] = data;
}

void Message::setData(const std::string& name, int data)
{
    (*this)["data"][name] = data;
}

void Message::removeData(const std::string& name)
{
    (*this)["data"].removeMember(name);
}

bool Message::hasData(const std::string& name)
{
    return (*this)["data"].isMember(name);
}

} // namespace smpl
} // namespace scy

/// @\}
