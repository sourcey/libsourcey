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


#ifndef SOURCEY_XMPP_JINGLE_Jingle_H
#define SOURCEY_XMPP_JINGLE_Jingle_H


#include "Sourcey/XMPP/Stanza.h"
//#include "Sourcey/XMPP/Jingle/Jingle.h"


namespace Scy {
namespace XMPP {


namespace Jingle {


// Jingle Example
//
// <iq from='juliet@capulet.lit/balcony'
//     id='yd71f495'
//     to='romeo@montague.lit/orchard'
//     type='set'>
//   <jingle xmlns='urn:xmpp:jingle:1'
//           action='session-accept'
//           responder='juliet@capulet.lit/balcony'
//           sid='a73sjjvkla37jfea'>
//     <content creator='initiator' name='voice'>
//       <description xmlns='urn:xmpp:jingle:apps:rtp:1' media='audio'>
//         <payload-type id='97' name='speex' clockrate='8000'/>
//         <payload-type id='18' name='G729'/>
//       </description>
//       <transport xmlns='urn:xmpp:jingle:transports:ice-udp:1'>
//         <candidate component='1'
//                    foundation='2'
//                    generation='0'
//                    id='y3s2b30v3r'
//                    ip='192.0.2.3'
//                    network='1'
//                    port='45664'
//                    priority='1694498815'
//                    protocol='udp'
//                    rel-addr='10.0.1.1'
//                    rel-port='8998'
//                    type='srflx'/>
//       </transport>
//     </content>
//   </jingle>
// </iq>


struct Jingle: public XML::Node //Query 
{
	// Jingle::Content::Description::PayloadType
	struct PayloadType: public XML::Node 
	{
		PayloadType();
		PayloadType(XML::Node& node);

		int id() const { return attribute("id").as_int(); };
		std::string name() const { return attribute("name").value(); };
		int clockrate() const { return attribute("clockrate").as_int(); };
		int channels() const { return attribute("channels").as_int() ? attribute("channels").as_int() : 1; };

		XML::Attribute idAttr() const { return attribute("id"); };
		XML::Attribute nameAttr() const { return attribute("name"); };
		XML::Attribute clockrateAttr() const { return attribute("clockrate"); };
		XML::Attribute channelsAttr() const { return attribute("channels"); };

		PayloadType setId(int id);
		PayloadType setName(const std::string& name);
		PayloadType setClockrate(int clockrate);
		PayloadType setChannels(int channels);
	};


	// Jingle::Content::Transport::Candidate
	struct Candidate: public XML::Node 
	{
		Candidate();
		Candidate(XML::Node& node);

		int component() const { return attribute("component").as_int(); };
		int generation() const { return attribute("generation").as_int(); };
		std::string id() const { return attribute("id").value(); };
		std::string ip() const { return attribute("ip").value(); };
		int network() const { return attribute("network").as_int(); };
		int port() const { return attribute("port").as_int(); };
		unsigned int priority() const { return attribute("priority").as_uint(); };
		std::string foundation() const { return attribute("foundation").value(); };
		std::string protocol() const { return attribute("protocol").value(); };
		std::string relAddr() const { return attribute("rel-addr").value(); };
		int relPort() const { return attribute("rel-port").as_int(); };
		std::string type() const { return attribute("type").value(); };
		
		XML::Attribute componentAttr() const { return attribute("component"); };
		XML::Attribute foundationAttr() const { return attribute("foundation"); };
		XML::Attribute generationAttr() const { return attribute("generation"); };
		XML::Attribute idAttr() const { return attribute("id"); };
		XML::Attribute ipAttr() const { return attribute("ip"); };
		XML::Attribute networkAttr() const { return attribute("network"); };
		XML::Attribute portAttr() const { return attribute("port"); };
		XML::Attribute priorityAttr() const { return attribute("priority"); };
		XML::Attribute protocolAttr() const { return attribute("protocol"); };
		XML::Attribute relAddrAttr() const { return attribute("relAddr"); };
		XML::Attribute relPortAttr() const { return attribute("relPort"); };
		XML::Attribute typeAttr() const { return attribute("type"); };

		Candidate setComponent(int component);
		Candidate setFoundation(const std::string& foundation);
		Candidate setGeneration(int generation);
		Candidate setId(const std::string& id);
		Candidate setIp(const std::string& ip);
		Candidate setNetwork(int network);
		Candidate setPort(int port);
		Candidate setPriority(unsigned int priority);
		Candidate setProtocol(const std::string& protocol);
		Candidate setRelAddr(const std::string& relAddr);
		Candidate setRelPort(int relPort);
		Candidate setType(const std::string& type);
	};


	// Jingle::Content::Transport::RemoteCandidate
	struct RemoteCandidate: public XML::Node 
	{
		RemoteCandidate();
		RemoteCandidate(XML::Node& node);

		int component() const { return attribute("component").as_int(); };
		std::string ip() const { return attribute("ip").value(); };
		int port() const { return attribute("port").as_int(); };

		XML::Attribute componentAttr() const { return attribute("component"); };
		XML::Attribute ipAttr() const { return attribute("ip"); };
		XML::Attribute portAttr() const { return attribute("port"); };

		RemoteCandidate setComponent(int component);
		RemoteCandidate setIp(const std::string& ip);
		RemoteCandidate setPort(int port);
	};


	// Jingle::Content::Description
	struct Description: public XML::Node 
	{
		Description();
		Description(XML::Node& node);

		std::string mediaType() const { return attribute("media").value(); };

		Description setMediaType(const std::string& media);

		PayloadType addPayloadType();
	};

	// Jingle::Content::Transport
	struct Transport: public XML::Node 
	{
		Transport();
		Transport(XML::Node& node);

		Candidate addCandidate();
		RemoteCandidate addRemoteCandidate();
	};

	// Jingle::Content
	struct Content: public XML::Node 
	{
		Content();
		Content(XML::Node& node);

		std::string creator() const { return attribute("creator").value(); };
		std::string name() const { return attribute("name").value(); };
		std::string senders() const { return attribute("name").empty() ? "both" : attribute("name").value(); };
			// 'senders' value of both, initiator, or responder 
			// a= line of sendrecv, recvonly, or sendonly    

		Content setCreator(const std::string& creator);
		Content setName(const std::string& name);
		Content setSenders(const std::string& senders);

		Description description() const;
		Transport transport() const;
	};


	struct Default: public XML::Template
	{
		Default(
			const std::string& action,
			const std::string& initiator = "",
			const std::string& responder = "",
			const std::string& sid = "");
	};

	/*
	struct Full: public XML::Template
	{
		Full(
			const std::string& action,
			const std::string& initiator = "",
			const std::string& responder = "",
			const std::string& sid = "");
	};
	*/

	struct Terminate: public XML::Template
	{
		Terminate(
			const std::string& reason,
			const std::string& initiator = "",
			const std::string& responder = "",
			const std::string& sid = "");
	};

	static const char* name() { return "jingle"; }
	static const char* xmlns() { return "urn:xmpp:jingle:1"; }
	
	Jingle();
	Jingle(XML::Node& node);
	Jingle(IQ& iq, XML::Template& t);
	virtual ~Jingle();

	bool empty() const;
	bool valid() const;

	std::string sid() const { return attribute("sid").value(); };
	std::string action() const { return attribute("action").value(); };
	std::string initiator() const { return attribute("initiator").value(); };
	std::string responder() const { return attribute("responder").value(); };
	std::string ufrag() const { return transport().attribute("ufrag").value(); };
	std::string pwd() const { return transport().attribute("pwd").value(); };

	Jingle setSID(const std::string& sid);
	Jingle setAction(const std::string& action);
	Jingle setInitiator(const std::string& initiator);
	Jingle setResponder(const std::string& responder);
	Jingle setUFrag(const std::string& ufrag);
	Jingle setPwd(const std::string& pwd);

	Content content() const;
	Description description() const;
	Transport transport() const;

	PayloadType addPayloadType();
	Candidate addCandidate();
	Candidate firstCandidate();
	Candidate highestPriorityCandidate() const;
};


} // namespace Jingle
} // namespace XMPP 
} // namespace Scy 


#endif // SOURCEY_XMPP_JINGLE_Jingle_H