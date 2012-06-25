//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#ifndef SOURCEY_ICE_CANDIDATE_H
#define SOURCEY_ICE_CANDIDATE_H


#include "Sourcey/Net/Address.h"
#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"

#include <sstream>


namespace Sourcey {
namespace ICE { 


class Candidate: public ILoggable 
	// +------------------------------------------+
	// |                                          |
	// | +---------------------+                  |
	// | |+----+ +----+ +----+ |   +Type          |
	// | || IP | |Port| |Tran| |   +Priority      |
	// | ||Addr| |    | |    | |   +Foundation    |
	// | |+----+ +----+ +----+ |   +ComponentID   |
	// | |      Transport      |   +RelatedAddr   |
	// | |        Addr         |                  |
	// | +---------------------+   +Base          |
	// |             Candidate                    |
	// +------------------------------------------+
	//
	// A CANDIDATE is a transport transportIP -- a combination of
	// IP transportIP and port for a particular transport transport.
{
public:
	enum Type 
		// Numeric values equate to type preference used in
		// prioritization formula.
	{
		NoType				= 0,
		Host				= 126,	// host
		ServerReflexive		= 100,	// srflx
		PeerReflexive		= 110,	// prflx
		Relayed				= 1		// relay
	};

	enum Transport 
	{
		NoTransport	= 0,
		Udp			= 0x01,  // "udp"      - draft-ietf-mmusic-ice-12
		TcpSo		= 0x02,	 // "tcp-so"   - TCP simultaneous-open - draft-ietf-mmusic-ice-tcp-02
		TcpAct		= 0x04,  // "tcp-act"  - TCP active - draft-ietf-mmusic-ice-tcp-02
		TcpPass		= 0x08,  // "tcp-pass" - TCP passive - draft-ietf-mmusic-ice-tcp-02
		TlsSo		= 0x10,  // "tls-so"   - TCP simultaneous-open - draft-ietf-mmusic-ice-tcp-02
		TlsAct		= 0x20,  // "tls-act"  - TCP active - draft-ietf-mmusic-ice-tcp-02
		TlsPass		= 0x40   // "tls-pass" - TCP passive - draft-ietf-mmusic-ice-tcp-02
	};

	Candidate();
	Candidate(Type type, 
			  int componentID = 0, 
			  Transport transport = /*Transport::*/Udp, 
			  const Net::Address& transportAddr = Net::Address());
	Candidate(const Candidate& src);
	virtual ~Candidate();

	virtual void setFoundation(const std::string& foundation) { _foundation = foundation; };
	virtual void setComponentID(int componentID) { _componentID = componentID; };
	virtual void setTransport(const Transport& transport) { _transport = transport; };
	virtual void setTransport(const std::string& transport) { _transport = stringToTransport(transport); };
	virtual void setPriority(UInt32 priority) { _priority = priority; };
	virtual void setTransportAddr(const Net::Address& address) { _transportAddr = address; };
	//virtual void setBaseAddr(const Net::Address& address) { _baseAddr = address; };
	virtual void setType(const Type& type) { _type = type; };
	virtual void setType(const std::string& type) { _type = stringToType(type); };
		
	virtual std::string foundation() const { return _foundation; };
	virtual int componentID() const { return _componentID; };
	virtual Transport transport() const { return _transport; };
	virtual UInt32 priority() const { return _priority; };
	virtual Net::Address transportAddr() const { return _transportAddr; };	
	virtual std::string transportIP() const { return _transportAddr.host().toString(); };	
	virtual unsigned short transportPort() const { return _transportAddr.port(); };		
	//virtual Net::Address baseAddr() const { return _baseAddr; };	
	//virtual std::string base()->transportAddr().host() const {  return _baseAddr.host().toString(); };	
	//virtual unsigned short base()->transportAddr().port() const { return _baseAddr.port(); };
	virtual Type type() const { return _type; };

	// A local host candidate MUST have a component associated with it.
	//virtual Component* component() const { return _component; };	
	//virtual void setComponent(Component* component);

	virtual bool valid() const;
	
	static Type stringToType(const std::string& type);
	static Transport stringToTransport(const std::string& transport);
	static std::string typeToString(UInt32 type);
	static std::string transportToString(UInt32 transport);
	virtual std::string typeString() const;
	virtual std::string transportString() const;
	virtual std::string toString() const;
	virtual void printLog(std::ostream& ost) const;
			
	Candidate& operator = (const Candidate& src);
	bool operator ^= (const Candidate& c) const;
	bool operator == (const Candidate& c) const;
	bool operator != (const Candidate& c) const;
	
	// For stl::list:sort operations.
	static bool CompareProiroty(const Candidate* l, const Candidate* r) {
		return l->priority() > r->priority();
	}
	
	virtual const char* className() const { return "ICE Candidate"; }

protected:
	std::string _foundation;
	int _componentID;
	Transport _transport;
	UInt32 _priority;
	Net::Address _transportAddr;
	//Net::Address _baseAddr;
	Candidate::Type _type;

	//Component* _component;
};


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_CANDIDATE_H
