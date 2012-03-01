//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_STUN_MESSAGE_H
#define SOURCEY_STUN_MESSAGE_H


#include "Sourcey/STUN/Attributes.h"
#include "Sourcey/IPacket.h"


namespace Sourcey {
namespace STUN {


typedef std::string TransactionID;


class Message: public IPacket
{
public:
	enum Type 
	{
		// STUN
		Binding					= 0x001, 

		// TURN
		Allocate				= 0x003,	// (only request/response semantics defined)
		Refresh					= 0x004,
		SendIndication			= 0x006,	// (only indication semantics defined)
		DataIndication			= 0x007,	// (only indication semantics defined)
		CreatePermission		= 0x008,	// (only request/response semantics defined)
		ChannelBind				= 0x009,	// (only request/response semantics defined)

		// TURN TCP
		Connect					= 0x000A, 
		ConnectionBind			= 0x000B, 
		ConnectionAttempt		= 0x000C
	};

	enum State 	
	{
		Request					= 0x000,
		Indication				= 0x001,
		SuccessResponse			= 0x002,
		ErrorResponse			= 0x003
	};	

	enum ErrorCodes 
	{
		BadRequest				= 400, 
		Unauthorized			= 401, 
		UnknownAttribute		= 420, 
		StaleCredentials		= 430, 
		IntegrityCheckFailure	= 431, 
		MissingUsername			= 432, 
		UseTLS					= 433, 
		RoleConflict			= 487,
		ServerError				= 500, 
		GlobalFailure			= 600, 

		// TURN TCP
		ConnectionAlreadyExists		= 446, 
		ConnectionTimeoutOrFailure	= 447
	};

public:
	Message();
	Message(const Message& r);	
	Message& operator = (const Message& r);
	virtual ~Message();
	
	virtual IPacket* clone() const;

	Type type() const { return static_cast<Type>(_type); }
	State state() const { return _state; }
	size_t size() const { return static_cast<size_t>(_size); }
	const TransactionID& transactionID() const { return _transactionID; }
	const std::vector<Attribute*> attrs() const { return _attrs; }

	void setType(UInt16 type) { _type = type; }
	void setState(State state) { _state = state; }
	void setTransactionID(const std::string& id);

	std::string errorString(UInt16 errorCode) const;
	std::string typeString() const;
	std::string stateString() const;

	void add(Attribute* attr);
	Attribute* get(Attribute::Type type, int index = 0) const;	

	template<typename T>
	T* get(int index = 0) const {
		return reinterpret_cast<T*>(
			get(static_cast<Attribute::Type>(T::TypeID), index));
	}

	bool read(Buffer& buf);
		// Parses the STUN/TURN packet from the given buffer.
		// The return value indicates whether this was successful.

	void write(Buffer& buf) const;
		// Writes this object into a STUN/TURN packet.

	std::string toString() const;
	void print(std::ostream& os) const;

	virtual const char* className() const { return "STUNMessage"; }

private:
	UInt16	_type;
	UInt16	_size;
	State	_state;
	TransactionID _transactionID;
	std::vector<Attribute*> _attrs;
};


} } // namespace Sourcey::STUN


#endif //  SOURCEY_STUN_MESSAGE_H

