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


#ifndef SOURCEY_SOCKETIO_Packet_H
#define SOURCEY_SOCKETIO_Packet_H


#include "Sourcey/IPacket.h"
#include "Sourcey/JSON/JSON.h"


namespace Sourcey {
namespace SocketIO {
	

class Packet: public IPacket 
{
public:
	enum Type 
	{
		Disconnect		= 0, 
		Connect			= 1,
		Heartbeat		= 2,
		Message			= 3,
		JSON			= 4,
		Event			= 5,
		Ack				= 6,
		Error			= 7
	};
	
	Packet(Type type = Message, 
		   int id = -1, 
		   const std::string& endpoint = "", 
		   const std::string& message = "", 
		   bool ack = false);	
		// Default contructor
	
	Packet(Type type,
		   const std::string& message = "", 
		   bool ack = false);	
		// General contructor

	Packet(const std::string& message, 
		   bool ack = false);
		// Message contructor

	Packet(const JSON::Value& data, 
		   bool ack = false);
		// JSON contructor

	Packet(const std::string& event, 
		   const JSON::Value& data, 
		   bool ack = false);
		// Event contructor

	Packet(const Packet& r);	
	Packet& operator = (const Packet& r);
	virtual ~Packet();

	virtual IPacket* clone() const;

	Type type() const;
	int id() const;
	std::string endpoint() const;
	std::string message() const;	
	JSON::Value json() const;
	
	void setID(int id);
	void setEndpoint(const std::string& endpoint);
	void setMessage(const std::string& message);
	void setAck(bool flag);

	bool read(Buffer& buf);
	void write(Buffer& buf) const;

	virtual size_t size() const;

	std::string typeString() const;
	std::string toString() const;
	void print(std::ostream& os) const;

	virtual const char* className() const { return "SocketIO::Packet"; }

protected:
	int _type;
	int _id;
	std::string _endpoint;
	std::string _message;
	size_t _size;
	bool _ack;
};


} } // namespace Sourcey::SocketIO


#endif //  SOURCEY_SOCKETIO_Packet_H

