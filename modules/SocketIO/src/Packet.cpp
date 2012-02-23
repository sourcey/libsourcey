//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#include "Sourcey/SocketIO/Packet.h"
#include "Sourcey/CryptoProvider.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"


using namespace std;
using Poco::format;


namespace Sourcey {
namespace SocketIO {


Packet::Packet(Type type, int id, const string& endpoint, const string& message, bool ack) : 
	_type(type), 
	_id(id),
	_endpoint(endpoint),
	_message(message),
	_ack(ack),
	_size(0)
{
}

	
Packet::Packet(Type type, const string& message, bool ack) : 
	_type(type), 
	_id(CryptoProvider::generateRandomNumber(4)),
	_message(message),
	_ack(ack),
	_size(0)
{
}

	
Packet::Packet(const string& message, bool ack) : 
	_type(Packet::Message), 
	_id(CryptoProvider::generateRandomNumber(4)),
	_message(message),
	_ack(ack),
	_size(0)
{
}

	
Packet::Packet(const JSON::Value& data, bool ack) : 
	_type(Packet::JSON), 
	_id(CryptoProvider::generateRandomNumber(4)),
	_message(JSON::stringify(data)),
	_ack(ack),
	_size(0)
{
}

	
Packet::Packet(const string& event, const JSON::Value& data, bool ack) : 
	_type(Packet::Event), 
	_id(CryptoProvider::generateRandomNumber(4)),
	_ack(ack),
	_size(0)
{	
	JSON::Value root;
	root["name"] = event;

	// add the data into an array if it isn't already
	if (!data.isArray()) {
		JSON::Value array(Json::arrayValue); 
		array.append(data);
		root["args"] = array;
	}
	else
		root["args"] = data;
		
	_message = JSON::stringify(root);
}


Packet::Packet(const Packet& r) : 
	_type(r._type), 
	_id(r._id),
	_endpoint(r._endpoint), 
	_message(r._message),
	_ack(true),
	_size(0)
{
}


Packet& Packet::operator = (const Packet& r) 
{
	_type = r._type;
	_id = r._id;
	_ack = r._ack;
	_endpoint = r._endpoint;
	_message = r._message;
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


bool Packet::read(Buffer& buf) 
{	
	//https://github.com/LearnBoost/socket.io-spec#Encoding
	if (buf.size() < 3)
		return false;
	
	//Log("debug") << "[SocketIO::Packet::" << this << "] Reading: " << (string(buf.data(), buf.size())) << endl;

	string data;
	buf.readString(data, buf.size());
	StringList content = Util::split(data, ':', 4);
	if (content.size() < 1) {
		//Log("debug") << "[SocketIO::Packet::" << this << "] Reading: Invalid Data: " << content.size() << endl;
		return false;
	}
		
	if (!content[0].empty()) {
		_type = Util::atoi(content[0]);
		//Log("debug") << "[SocketIO::Packet::" << this << "] Reading: Type: " << typeString() << endl;
	}

	if (_type < 0 || 
		_type > 7) {
		//Log("debug") << "[SocketIO::Packet::" << this << "] Reading: Invalid Type: " << _type << endl;
		return false;
	}
	if (content.size() >= 2 && !content[1].empty()) {
		_ack = (content[1].find('+') != string::npos);
		_id = Util::atoi(content[1]);
	}	
	if (content.size() >= 3 && !content[2].empty()) {
		_endpoint = content[2];
	}
	if (content.size() >= 4 && !content[3].empty()) {
		_message = content[3];
	}

	// For Ack packets the ID is at the start of the message
	if (_type == 6) {
		content.clear();
		Util::split(_message, '+', content, 2);
		if (content.size() != 2) {
			assert(content.size() == 2 && "invalid ack response");
			return false;
		}

		_ack = true; // This is mostly for requests, but we'll set it anyway
		_id = Util::atoi(content[0]);
		_message = content[1];
	}

	_size = data.length();
	//Log("debug") << "[SocketIO::Packet::" << this << "] Parse Success: " << toString() << endl;

	return true;
}


void Packet::write(Buffer& buf) const 
{
	ostringstream ss;
	print(ss);
	buf.writeString(ss.str());
}


void Packet::setID(int id) 
{ 
	_id = id; 
}


void Packet::setEndpoint(const std::string& endpoint) 
{ 
	_endpoint = endpoint; 
}


void Packet::setMessage(const std::string& message) 
{ 
	_message = message; 
}


void Packet::setAck(bool flag) 
{ 
	_ack = flag; 
}


Packet::Type Packet::type() const 
{ 
	return static_cast<Packet::Type>(_type); 
}


int Packet::id() const 
{ 
	return _id; 
}


string Packet::endpoint() const 
{ 
	return _endpoint; 
}


string Packet::message() const 
{ 
	return _message; 
}


JSON::Value Packet::json() const
{
	if (!_message.empty()) {
		JSON::Value data;
		JSON::Reader reader;
		if (reader.parse(_message, data))
			return data;
	}
	return JSON::Value();
}


string Packet::typeString() const 
{
	switch (_type) {
	case Disconnect: return "Disconnect";
	case Connect: return "Connect";
	case Heartbeat: return "Heartbeat";
	case Message: return "Message";
	case JSON: return "JSON";
	case Event: return "Event";
	case Ack: return "Ack";
	case Error: return "Error";
		
	default: return "Unknown";
	}
}


string Packet::toString() const 
{
	ostringstream ss;
	print(ss);
	//ss << endl;
	return ss.str();
}


size_t Packet::size() const
{
	ostringstream ss;
	print(ss);
	assert(ss.tellp());
	return ss.tellp();
}


void Packet::print(ostream& os) const
{
	os << _type;
	if (_id == -1 && _endpoint.empty() && _message.empty()) {
		os << "::";
	}
	else if (_id == -1 && _endpoint.empty()){
		os << ":::" << _message;
	}  
	else if (_id > -1 && _type != 6) {
		os << ":" << _id << (_ack ? "+" : "") 
			<< ":" << _endpoint << ":" << _message;
	}
	else {
		os << "::" << _endpoint << ":" << _message;
	}
}



} } // namespace Sourcey::SocketIO
