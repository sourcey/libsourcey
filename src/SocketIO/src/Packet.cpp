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
//#include "scy/crypto/crypto.h"
#include "scy/logger.h"
#include "scy/util.h"
//#include "Poco/Format.h"


using std::endl;
//using Poco::format;


namespace scy {
namespace sockio {


Packet::Packet(Type type, int id, const std::string& endpoint, const std::string& message, bool ack) : 
	_type(type), 
	_id(id),
	_endpoint(endpoint),
	_message(message),
	_ack(ack),
	_size(0)
{
}

	
Packet::Packet(Type type, const std::string& message, bool ack) : 
	_type(type), 
	_id(util::randomNumber()),
	_message(message),
	_ack(ack),
	_size(0)
{
	assert(_id);
}

	
Packet::Packet(const std::string& message, bool ack) : 
	_type(Packet::Message), 
	_id(util::randomNumber()),
	_message(message),
	_ack(ack),
	_size(0)
{
	assert(_id);
}

	
Packet::Packet(const json::Value& data, bool ack) : 
	_type(Packet::JSON), 
	_id(util::randomNumber()),
	_message(json::stringify(data)),
	_ack(ack),
	_size(0)
{
	assert(_id);
}

	
Packet::Packet(const std::string& event, const json::Value& data, bool ack) : 
	_type(Packet::Event), 
	_id(util::randomNumber()),
	_ack(ack),
	_size(0)
{	
	assert(_id);

	json::Value root;
	root["name"] = event;

	// add the data into an array if it isn't already
	if (!data.isArray()) {
		json::Value array(Json::arrayValue); 
		array.append(data);
		root["args"] = array;
	}
	else
		root["args"] = data;
		
	_message = json::stringify(root);
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


std::size_t Packet::read(const ConstBuffer& buf) 
{			
	// https://github.com/LearnBoost/socket.io-spec#Encoding

	// Reset all data
	_type = Packet::Message;
	_id = 0;
	_endpoint = "";
	_message = "";
	_size = 0;

	if (buf.size() < 3)
		return 0;

	std::string data(bufferCast<const char*>(buf), buf.size());	
	std::vector<std::string> frags = util::split(data, ':', 4);
	if (frags.size() < 1) {
		//debugL("sockio::Packet", this) << "Reading: Invalid Data: " << frags.size() << endl;
		return false;
	}
		
	if (!frags[0].empty()) {
		_type = util::strtoi<UInt32>(frags[0]);
		//debugL("sockio::Packet", this) << "Reading: Type: " << typeString() << endl;
	}

	if (_type < 0 || _type > 7) {
		//debugL("sockio::Packet", this) << "Reading: Invalid Type: " << _type << endl;
		return false;
	}
	if (frags.size() >= 2 && !frags[1].empty()) {
		_ack = (frags[1].find('+') != std::string::npos);
		_id = util::strtoi<UInt32>(frags[1]);
	}	
	if (frags.size() >= 3 && !frags[2].empty()) {
		_endpoint = frags[2];
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
		{	// complex ack
			_id = util::strtoi<UInt32>(data.substr(0, pos));
			_message = data.substr(pos + 1, data.length());
		}
		else
		{	// simple ack
			_message = data;
		}

		/*
		frags.clear();
		util::split(_message, '+', frags, 2);
		if (frags.size() != 2) {
			assert(frags.size() == 2 && "invalid ack response");
			return false;
		}

		_ack = true; // This is mostly for requests, but we'll set it anyway
		_id = util::strtoi<UInt32>(frags[0]);
		_message = frags[1];
		*/
	}

	_size = data.length();
	//debugL("sockio::Packet", this) << "Parse Success: " << toString() << endl;

	return _size;
}


void Packet::write(Buffer& buf) const 
{
	assert(valid());
	std::ostringstream ss;
	print(ss);
	
	std::string str(ss.str()); // TODO: avoid copy
	buf.insert(buf.end(), str.begin(), str.end()); 
	//buf.append(ss.str().c_str(), ss.tellp());
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


std::string Packet::endpoint() const 
{ 
	return _endpoint; 
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


std::string Packet::typeString() const 
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
	return _type >= 0 && _type <= 8 && _id > 0;
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
	os << _type;
	if (_id == -1 && _endpoint.empty() && _message.empty()) {
		os << "::";
	}
	else if (_id == -1 && _endpoint.empty()){
		os << ":::" << _message;
	}  
	else if (_id > -1 && _type != 6) {
		os << ":" << _id << (_ack ? "+":"") 
			<< ":" << _endpoint << ":" << _message;
	}
	else {
		os << "::" << _endpoint << ":" << _message;
	}
}



} } // namespace scy::sockio
