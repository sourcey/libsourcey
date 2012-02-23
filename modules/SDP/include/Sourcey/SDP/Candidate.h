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


#ifndef SOURCEY_SDP_LINE_CANDIDATE_H
#define SOURCEY_SDP_LINE_CANDIDATE_H


#include "Sourcey/SDP/A.h"
#include "Sourcey/Util.h"

#include <string>


namespace Sourcey {
namespace SDP { 


class Candidate: public A {
public:
	Candidate(const std::string& src);
	virtual ~Candidate();

	std::string foundation() const;
	unsigned int componentID() const;
	std::string transport() const;
	unsigned int priority() const;
	std::string address() const;
	unsigned int port() const;
	std::string candidateType() const;
	unsigned int relPort() const;
	std::string relAddr() const;

	void setFoundation(const std::string& foundation);
	void setComponentID(unsigned int componentID);
	void setTransport(const std::string& transport);
	void setPriority(unsigned int priority);
	void setAddr(const std::string& address);
	void setPort(unsigned int port);
	void setCandidateType(const std::string& candidateType);
	void setRelPort(unsigned int port);
	void setRelAddr(const std::string& relAddr);

	virtual std::string toString();

private:
	std::string _foundation;
	unsigned int _componentID;
	std::string _transport;
	unsigned int _priority;
	std::string _address;
	unsigned int _port;
	std::string _candidateType;
	std::string _relAddr;
	unsigned int _relPort;
};


} // namespace Sourcey
} // namespace SDP 


#endif
