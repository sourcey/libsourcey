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


#include "Sourcey/SDP/Message.h"
#include "Sourcey/SDP/V.h"
#include "Sourcey/SDP/O.h"
#include "Sourcey/SDP/S.h"
#include "Sourcey/SDP/C.h"
#include "Sourcey/SDP/T.h"
#include "Sourcey/SDP/M.h"
#include "Sourcey/SDP/A.h"
#include "Sourcey/SDP/I.h"
#include "Sourcey/SDP/RTPMap.h"
#include "Sourcey/SDP/Candidate.h"
#include "Sourcey/SDP/RemoteCandidate.h"
#include "Sourcey/Logger.h"

#include <sys/types.h>


using namespace std;


namespace Scy {
namespace SDP { 


Message::Message() 
{
}


Message::Message(const string& src) 
{
	read(src);
}


Message::Message(const Message& r) 
{
	read(r.toString());
}


Message::~Message() 
{
	Util::ClearVector(_lines);
	//for (vector<Line*>::iterator it = _lines.begin(); it != _lines.end(); ++it)
	//	delete *it;
}
	

IPacket* Message::clone() const 
{
	return new Message(*this);
}


bool Message::read(Buffer& buf)
{
	// TODO: Use buffer instead of string for more efficient read methods. 
	return read(string(buf.bytes(), buf.size()));
}


bool Message::read(const std::string& src)
{
	int idxM = 1;
	StringVec lines = Util::split(src, '\n'); 
	//LogDebug() << "SDP: Parsing lines: " << lines.size() << endl;
	for (int n = 0; n < lines.size(); n++) {
		//LogDebug() << "SDP: Parsing line: " << n << ": " << lines[n] << endl;

		switch (lines[n][0]) {
			case 'v':
				addLine(new V(lines[n]));
				break;
			case 'o':
				addLine(new O(lines[n]));
				break;
			case 's':
				addLine(new S(lines[n]));
				break;
			case 't':
				addLine(new T(lines[n]));
				break;
			case 'c':
				addLine(new C(lines[n]));
				break;
			case 'i':
				addLine(new I(lines[n]));
				break;
			case 'a':
				addLine(new A(lines[n]));
				break;
			case 'b':
				addLine(new B(lines[n]));
				break;
			case 'm': { 
				M* m = new M(lines[n]);
				m->setIndex(idxM++);
				addLine(m);
				bool done = false;
				do {
					n++;
					done = n == lines.size();
					if (done) continue;

					//LogDebug() << "SDP: Parsing media line: " << n << ": " << lines[n] << endl;
					switch (lines[n][0]) {
						case 'c':
							m->addLine(new C(lines[n]));
							break;
						case 'a': {
								A* a = NULL;
								if (lines[n].substr(0, 11) == "a=candidate")
									a = new Candidate(lines[n]);
								else if (lines[n].substr(0, 19) == "a=remote-candidates")
									a = new RemoteCandidate(lines[n]);
								else if (lines[n].substr(0, 8) == "a=rtpmap")
									a = new RTPMap(lines[n]);
								else 
									a = new A(lines[n]);
								m->addLine(a);
							}
							break;
						case 'i':
							m->addLine(new I(lines[n]));
							break;
						case 'b':
							m->addLine(new B(lines[n]));
							break;
						case 'm':
							// Parse the next media line...
							//LogDebug() << "SDP: Parsing next media line" << endl;
							done = true;
							n--;
							break;
						default:
							LogError() << "ERROR: Unknown SDP media line: " << lines[n] << endl;
							break;
					}
				} while (!done); }
				//LogDebug() << "SDP: Parsed media line: " << n << endl;
				break;
			default:
				LogError() << "ERROR: Unknown SDP line: " << lines[n] << endl;
				break;
		}
	}

	return true;
}


void Message::write(Buffer& buf) const 
{
	buf.writeString(toString());
}


bool Message::empty() const
{
	return _lines.empty();
}


bool Message::valid() const
{
	// TODO: Implement properly
	return !_lines.empty();
}


void Message::addLine(Line* h) 
{
	_lines.push_back(h);
}


string Message::toString() const 
{
	string ret = "";	
	for (int priority = 0; priority < 15; priority++) {
		for (unsigned i = 0; i < _lines.size(); i++) {
			if (_lines[i]->priority() == priority) {	
				ret += "\r\n" + _lines[i]->toString();	
				if (_lines[i]->type() == Line::M) {
					std::vector<Line*> lines = ((M*)_lines[i])->lines();		
					for (std::vector<Line*>::const_iterator it = lines.begin(); it != lines.end(); it++) {	
						ret += "\r\n" + (*it)->toString();
					}			
				}
			}
		}
	}

	Util::trim(ret);
	return ret + "\r\n";
}


void Message::print(std::ostream& os) const
{
	os << toString();
}


vector<M*> Message::mediaLines() const 
{
	return this->lines<M>();
}


bool Message::isICESupported() const
{
	bool isOK = true;
	// For each media stream in the SDP it received, the default destination 
	// for each component of that media stream appears in a candidate 
	// attribute. For example, in the case of RTP, the IP address and
	// port in the c and m lines, respectively, appear in a candidate 
	// attribute and the value in the rtcp attribute appears in a candidate 
	// attribute.
	
	string cIP;
	string mPort;
	bool hasIP = false;
	bool hasPort = false;
	for (std::vector<Line*>::const_iterator it = _lines.begin(); it != _lines.end(); ++it) {

		if ((*it)->type() == Line::C) {
			cIP = reinterpret_cast<C*>(*it)->address();
		}

		if ((*it)->type() == Line::M) {
			M* lineM = reinterpret_cast<M*>(*it);
			mPort = Util::toString(lineM->port());
			vector<Candidate*> attrs = lineM->lines<Candidate>();
			for (vector<Candidate*>::const_iterator it1 = attrs.begin(); it1 != attrs.end(); ++it1) {

				// Check for a matching port and ip.
				if ((*it1)->toString().find(cIP) != string::npos)
					hasIP = true;
				if ((*it1)->toString().find(mPort) != string::npos)
					hasPort = true;
			}

			if (!hasIP || !hasPort) {
				LogError() << "SDP Lines M Port || C IP NOT Matching/Found" << endl;
				isOK = false;
				break;
			}
		}

		hasIP = false;
		hasPort = false;
	}
	return isOK;
}


string Message::firstMediaFormat() const
{
	M* m = NULL;
	for (unsigned i = 0 ; i < _lines.size(); i++)
		if (_lines[i]->type() == Line::M)
			m = (M*)_lines[i];

	if (!m || m->numPayloadTypes()<1) {
		LogError() << "ERROR: BUG: SDP packet did not contain <m> line. Defaulting to PCMu" << endl;
		return NULL;
	}

	return m->payloadType(0);

}


bool Message::isMediaFormatAvailable(const string& fmt) const
{
	M* m = NULL;
	unsigned int i;
	for (i = 0 ; i < _lines.size(); i++)
		if (_lines[i]->type() == Line::M)
			m = (M*)_lines[i];

	if (!m || m->numPayloadTypes() < 1) {
		LogError() << "ERROR: BUG: SDP packet did not contain <m> line. Defaulting to PCMu" << endl;
		return NULL;
	}

	for (i = 0;  (int)i < m->numPayloadTypes(); i++)
		if (m->payloadType(i) == fmt)
			return true;
	return false;
}


void Message::setSessionLevelAttribute(const string& type, const string& value) 
{
	A* a = new A("a=X");
	a->setPriority(6);
	a->setAttributes(type + ":" + value);
	addLine(a);		
}


string Message::getSessionLevelAttribute(const string& type, int index) const 
{
	int i = 0;
	vector<SDP::A*> lines = this->lines<SDP::A>();
	for (vector<SDP::A*>::const_iterator it = lines.begin(); it != lines.end(); it++) {
		if ((*it)->attributeType() == type) {
			if (index == i)
				return (*it)->attributeValue();
			i++;		
		}
	}
	LogError() << "SDP: No media level attribute: " << type << endl;
	return "";
}


string Message::getMediaOrSessionLevelAttribute(const M& m, const string& type, int index) const 
{
	string value = m.getAttribute(type, index);
	if (!value.empty())
		return value;
	return getSessionLevelAttribute(type, index);
}


bool Message::hasSessionLevelAttribute(const std::string& type) const 
{
	A* a;
	for (unsigned i = 0; i < _lines.size(); i++) {
		if (_lines[i]->type() == Line::A) {
			a = (A*)_lines[i];			
			if (a->attributeType() == type) {
				return true;
			}
		}
	}	
	return false;
}


std::vector<Line*> Message::lines() const
{
	return _lines;
}


} // namespace SDP 
} // namespace Scy






/*
O* Message::sessionSessionIdentifier() const
{
	for (unsigned i = 0; i < _lines.size(); i++) {
		if (!_lines[i])
			LogError() << "WARNING: SDP lines are null" << endl;
		if (_lines[i]->type() == Line::O) {
			return reinterpret_cast<O*>(_lines[i]);
		}
	}

	return NULL;
}


C* Message::sessionConnection() const
{
	for (unsigned i = 0; i < _lines.size(); i++) {
		if (!_lines[i])
			LogError() << "WARNING: SDP lines are null" << endl;
		if (_lines[i]->type() == Line::C) {
			return reinterpret_cast<C*>(_lines[i]);
		}
	}

	return NULL;
}


I* Message::sessionInformation() const
{
	for (unsigned i = 0; i < _lines.size(); i++) {
		if (!_lines[i])
			LogError() << "WARNING: SDP lines are null" << endl;
		if (_lines[i]->type() == Line::C) {
			return reinterpret_cast<I*>(_lines[i]);
		}
	}

	return NULL;
}


string Message::getDefaultDestination(int componentID, const M& m) const 
{
	C* connection = getMediaOrSessionLevelLine<C>(m, "c=", componentID);
	assert(m.numPorts() >= componentID);
	if (m.numPorts() >= componentID)
		return connection->address();
}


string Message::getSessionLevelICEUFrag() {
	A* aAttr;

	for (unsigned i = 0; i < _lines.size();i++) {
		if (!_lines[i])
			LogError() << "WARNING: SDP lines are null" << endl;
		if (_lines[i]->type() == Line::A) {
			aAttr = (A*)_lines[i];
			if (aAttr->attributeType() == "ice-ufrag")
				return aAttr->attributeValue();
		}
	}

	return "";
}


string Message::getSessionLevelICEPassword() {
	A* aAttr;

	for (unsigned i = 0; i < _lines.size();i++) {
		if (!_lines[i])
			LogError() << "WARNING: SDP lines are null" << endl;
		if (_lines[i]->type() == Line::A) {
			aAttr = (A*)_lines[i];
			if (aAttr->attributeType() == "ice-pwd")
				return aAttr->attributeValue();
		}
	}

	return "";
}


string Message::getKeyMgmt() {
	A* aAttr;

	for (unsigned i = 0; i < _lines.size();i++) {
		if (!_lines[i])
			LogError() << "WARNING: SDP lines are null" << endl;
		if (_lines[i]->type() == Line::A) {
			aAttr = (A*)_lines[i];
			if (aAttr->attributeType() == "key-mgmt")
				return aAttr->attributeValue();
		}
	}

	return "";
}
	

string Message::payloadTypeMatch(SDP &pack) {
	M* thisM;
	M* otherM;

	unsigned int i;	
	for (i = 0; i < _lines.size(); i++)
		if (_lines[i]->type() == Line::M)
			thisM = (M*)_lines[i];

	for (i = 0; i < pack._lines.size(); i++)
		if (_lines[i]->type() == Line::M)
			otherM = (M*)pack._lines[i];
		
	if (!thisM || !otherM) {
		LogError() << "ERROR: BUG: SDP packet did not contain <m> line. Defaulting to PCMu" << endl;
		return "";
	}

	for (i = 0; (int)i < thisM->numPayloadTypes(); i++)
		for (int j = 0; j< otherM->numPayloadTypes(); j++)
			if (thisM->payloadType(i) == otherM->payloadType(j))
				return thisM->payloadType(i);

	LogError() << "ERROR: could not match any codec format - trying anyway with PCMu" << endl;
	return "";	
}
*/
