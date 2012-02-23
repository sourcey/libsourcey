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


#include "Sourcey/SDP/M.h"
#include "Sourcey/SDP/A.h"
#include "Sourcey/SDP/C.h"
#include "Sourcey/SDP/I.h"
#include "Sourcey/SDP/RTPMap.h"
#include "Sourcey/SDP/Candidate.h"
#include "Sourcey/SDP/RemoteCandidate.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include <iostream>


using namespace std;


namespace Sourcey {
namespace SDP { 


M::M(const string& src) : 
	Line(Line::M, 14)
{
	size_t len = src.length();
	if (src.substr(0, 2) != "m=") {
		Log("error") << "ERROR: SDP line does not start with <m=>" << endl;
	}
	size_t i = 2;
	while (src[i] == ' ' && i < len)
		i++;
	
	//_mediaType = "";
	while (src[i] != ' ' && i < len)
		_mediaType += src[i++];

	while (src[i] == ' ' && i < len)
		i++;

	string portstr = "";
	while (src[i] != ' ' && i < len)
		portstr += src[i++];
	
	unsigned int np = 0;
	for (unsigned j = 0; j < portstr.length(); j++)
		if (portstr[j] == '/')
			np = j;
	if (np > 0) {
		_port = Util::atoi(portstr.substr(0, np).c_str());
		_nPorts =  Util::atoi(portstr.substr(np+1, portstr.length()-(np+1)-1).c_str());
	} else {
		_port = Util::atoi(portstr.c_str());
		_nPorts = 1;
	}
	
	while (src[i] == ' ' && i < len)
		i++;
	
	while (src[i] != ' ' && i < len)
		_transport += src[i++];

	bool done = false;
	while (!done) {
		while (src[i] == ' '  && i < len)
			i++;

		string f = "";
		while (src[i] != ' ' && i < len)
			f += src[i++];
		if (!f.empty()) {
			_payloadTypes.push_back(f);
		}
		
		if (i >= len)
			done = true;
	}
}


M::M(const string& media, 
	 unsigned int port, 
	 unsigned int nPorts, 
	 string transport) : 
	Line(Line::M, 8) 
{
	_mediaType = media;
	_port = port;
	_nPorts = _nPorts;
	_transport = transport;
}


M::M(const M &src) : 
	Line(Line::M, 8) 
{
	*this = src;
}


M &M::operator = (const M &src) 
{
	setPriority(src.priority());
	_mediaType = src._mediaType;
	_port = src._port;
	_nPorts = src._nPorts;
	_transport = src._transport;
	_payloadTypes = src._payloadTypes;
	
	Util::ClearVector(_lines);
	for (vector<Line*>::const_iterator it = src._lines.begin(); it != src._lines.end(); it++) {
		addLine(*it);
	}

	return *this;
}


M::~M() 
{
	//if (_connection)
	//	delete _connection;
	//if (_information)
	//	delete _information;
	//Util::ClearVector(_lines);

	for (vector<Line*>::iterator it = _lines.begin(); it != _lines.end(); ++it)
		delete *it;
}


string M::mediaType() const 
{
	return _mediaType;
}


void M::setMediaType(const string& media) 
{
	_mediaType = media;
}


unsigned int M::port() const 
{
	return _port;
}


void M::setPort(unsigned int p) 
{
	_port = p;
}


unsigned int M::numPorts() const 
{
	return _nPorts;
}


string M::transport() const 
{
	return _transport;
}


void M::setTransport(const string& t) 
{
	_transport = t;
}


void M::addPayloadType(const string& type) 
{
	_payloadTypes.push_back(type);
}


unsigned int M::numPayloadTypes() const 
{
	return (unsigned int)_payloadTypes.size();
}


string M::payloadType(unsigned int i) const 
{
	return _payloadTypes[i];
}


string M::toString() 
{
	string ret = "m=" + _mediaType + " ";
	
	if (_nPorts > 1)
		ret += _port + "/" + Util::itoa(_nPorts);
	else
		ret += Util::itoa(_port);

	ret += " " + _transport;

	for (unsigned i = 0; i < _payloadTypes.size(); i++)
		ret += " " + _payloadTypes[i];

	//if (_connection)
	//	ret += "\r\n" + _connection->toString();
		
	//if (_information)
	//	ret += "\r\n" + _information->toString();		

	return ret;
}


void M::addLine(Line* line) 
{
	_lines.push_back(line);
}


vector<Line*> M::lines() const 
{ 
	return _lines; 
}


/*
template<typename T>
T M::getLine(const string& key, int index) const {
	int i = 0;
	for (vector<Line*>::const_iterator it = _lines.begin(); it != _lines.end(); it++) {
		if ((*it)->toString().find(key) != string::npos) {
			if (index == i)
				return dynamic_cast<T>(*it);
			i++;
		}
	}
	return NULL;
}


Line* M::getLine(const std::string& key, int index) const {
	int i = 0;
	for (vector<Line*>::const_iterator it = _lines.begin(); it != _lines.end(); it++) {
		if ((*it)->toString().find(key) != string::npos) {
			if (index == i)
				return *it;
			i++;
		}
	}
	return NULL;
}
*/


string M::getAttribute(const string& key, int index) const 
{	
	int i = 0;
	vector<SDP::A*> lines = this->lines<SDP::A>();
	for (vector<SDP::A*>::const_iterator it = lines.begin(); it != lines.end(); it++) {
		if ((*it)->attributeType() == key) {
			if (index == i)
				return (*it)->attributeValue();
			i++;		
		}
	}
	return "";
}


vector<Candidate*> M::candidates() const 
{
	/*
	vector<Candidate*> candidates;
	vector<SDP::A*> lines = this->lines<SDP::A>();
	for (vector<SDP::A*>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
		candidates.push_back(reinterpret_cast<Candidate*>(*it));
	}
	return candidates;
	*/
	return lines<SDP::Candidate>();
}


RTPMap* M::rtpmap(const string& payloadType) const 
{
	vector<RTPMap*> lines = this->lines<RTPMap>();
	for (vector<RTPMap*>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
		string attr = (*it)->attributeValue();
		size_t firstSpace = attr.find(" ");		
		if (attr.substr(0, firstSpace) == payloadType)
			return reinterpret_cast<RTPMap*>(*it);
	}
	return NULL;
}


string M::fmtpParam(const string& payloadType) const 
{
	int i = 0;
	string attr;
	while ((attr = getAttribute("fmtp", i)) != "") {
		size_t firstSpace = attr.find(" ");
		if (attr.substr(0, firstSpace) == payloadType) {
			return attr.substr(firstSpace+1, attr.size());
		}
		i++;
	}
	return "";
}
	

unsigned int M::index() const 
{ 
	return _index; 
}
	

void M::setIndex(unsigned int index) 
{ 
	_index = index; 
}
	

StringList M::payloadTypes() const 
{ 
	return _payloadTypes; 
}


unsigned int M::numComponents() const 
{
	int n = 0;
	int lastID = 0;
	vector<Candidate*> candidates = this->candidates();
	for (vector<Candidate*>::const_iterator it = candidates.begin(); it != candidates.end(); it++) {
		if (lastID < (*it)->componentID()) {
			lastID = (*it)->componentID();
			n++;
		}
	}
	return n;

}


unsigned int M::numCandidates() const 
{
	return (unsigned int)candidates().size();
}


/*
void M::setConnection(SDP::C* c)
{
	_connection = c;
}


C* M::connection() const 
{
	return _connection;
}
*/


} // namespace Sourcey
} // namespace SDP 