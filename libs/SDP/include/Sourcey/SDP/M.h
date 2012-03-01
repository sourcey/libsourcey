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


#ifndef SOURCEY_SDP_LINE_M_H
#define SOURCEY_SDP_LINE_M_H


#include "Sourcey/SDP/Line.h"
#include "Sourcey/SDP/A.h"
#include "Sourcey/SDP/C.h"
#include "Sourcey/SDP/I.h"
#include "Sourcey/SDP/Candidate.h"
#include "Sourcey/SDP/RTPMap.h"


namespace Sourcey {
namespace SDP { 


class M: public Line 
{
public:
	M(const std::string& src);
	M(const std::string& media, unsigned int port, unsigned int nPorts, std::string transport);
	M(const M &src);
	virtual ~M();
	
	M &operator = (const M &src);

	void addLine(Line* line);

	template<typename T>
	T* getLine(const std::string& key, int index = 0) const {
		int i = 0;
		for (std::vector<Line*>::const_iterator it = _lines.begin(); it != _lines.end(); it++) {
			T* l = dynamic_cast<T*>(*it);
			if (l && (*it)->toString().find(key) != std::string::npos) {
				if (index == i)
					return l;
				i++;
			}
		}
		return NULL;
	}

	template<typename T>
	std::vector<T*> lines() const {		
		std::vector<T*> l;
		for (std::vector<Line*>::const_iterator it = _lines.begin(); it != _lines.end(); it++) {
			T* line = dynamic_cast<T*>(*it);
			if (line)
				l.push_back(line);
		}
		return l; 
	}
	std::vector<Line*> lines() const;

	//void addAttribute(A* line);
	std::string getAttribute(const std::string& type, int index = 0) const;

	unsigned int numPayloadTypes() const;
	StringList payloadTypes() const;
	std::string payloadType(unsigned int i) const;
	std::string mediaType() const;
	unsigned int numPorts() const;
	unsigned int port() const;
	std::string transport() const;
	//C* connection() const;	
	RTPMap* rtpmap(const std::string& payloadType) const;	
	std::string fmtpParam(const std::string& payloadType) const;
	std::vector<Candidate*> candidates() const;
	size_t numComponents() const;
	size_t numCandidates() const;
	unsigned int index() const;

	void addPayloadType(const std::string& payloadType);
	void setMediaType(const std::string& media);
	void setPort(unsigned int p);
	void setTransport(const std::string& t);
	//void setConnection(C* c);
	void setIndex(unsigned int index);
	
	virtual std::string toString();

private:
	// The numerical index position of the media line in the SDP.
	unsigned int _index;
	std::string _mediaType;
	unsigned int _port;
	unsigned int _nPorts;
	std::string _transport;
	StringList _payloadTypes;
	std::vector<Line*> _lines;
	//C* _connection;
};


} // namespace SDP 
} // namespace Sourcey


#endif
