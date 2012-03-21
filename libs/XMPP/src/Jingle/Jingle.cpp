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


#include "Sourcey/XMPP/Jingle/Jingle.h"
#include "Sourcey/XMPP/Stanza.h"
#include "Sourcey/CryptoProvider.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace XMPP {
namespace Jingle {


// ---------------------------------------------------------------------
//
// Jingle
//
// ---------------------------------------------------------------------
Jingle::Default::Default(
	const string& action,
	const string& initiator,
	const string& responder,
	const string& sid) 
{
	data = format(
		"<jingle xmlns='urn:xmpp:jingle:1' action='%s' initiator='%s' responder='%s' sid='%s'/>\n", 
		action, 
		initiator, 
		responder, 
		sid.empty() ? CryptoProvider::generateRandomKey(16) : sid);
}


Jingle::Terminate::Terminate(
	const string& reason,
	const string& initiator,
	const string& responder,
	const string& sid) 
{
	data = format(
		"<jingle xmlns='urn:xmpp:jingle:1' action='session-terminate' initiator='%s' responder='%s' sid='%s'>\n"
		"\t<reason>\n"
		"\t\t<%s/>\n"
		"\t</reason>\n"
		"</jingle>\n", 
		initiator, 
		responder, 
		sid.empty() ? CryptoProvider::generateRandomKey(16) : sid,
		reason);
}


Jingle::Jingle() 
{
}


/*
Jingle::Jingle(IQ& iq) :
	XML::Node(iq.rootNode().appendChild("jingle"))
{
	append_attribute("xmlns").set_value(Jingle::xmlns());
}
*/
Jingle::Jingle(XML::Node& node) :
	XML::Node(node)
{
	append_attribute("xmlns").set_value(Jingle::xmlns());
}


Jingle::Jingle(IQ& iq, XML::Template& t) :
	XML::Node(iq.rootNode().append(t.data))
{
	append_attribute("xmlns").set_value(Jingle::xmlns());
}


Jingle::~Jingle() 
{
}


/*
Jingle Jingle::create(IQ& iq, XML::Template& t) 
{
	return (Jingle&)iq.rootNode().append(t.data);
}
*/


Jingle Jingle::setSID(const string& sid) 
{ 
	remove_attribute("sid"); 
	append_attribute("sid").set_value(sid.data()); 
	return *this;
}


Jingle Jingle::setAction(const string& action) 
{ 
	remove_attribute("action"); 
	append_attribute("action").set_value(action.data()); 
	return *this;
}


Jingle Jingle::setInitiator(const string& initiator) 
{ 
	remove_attribute("initiator"); 
	append_attribute("initiator").set_value(initiator.data()); 
	return *this;
}


Jingle Jingle::setResponder(const string& responder) 
{ 
	remove_attribute("responder"); 
	append_attribute("responder").set_value(responder.data()); 
	return *this;
}


Jingle Jingle::setUFrag(const string& ufrag) 
{ 
	child("content").child("transport").remove_attribute("ufrag"); 
	child("content").child("transport").append_attribute("ufrag").set_value(ufrag.data()); 
	return *this;
}


Jingle Jingle::setPwd(const string& pwd) 
{ 
	child("content").child("transport").remove_attribute("pwd"); 
	child("content").child("transport").append_attribute("pwd").set_value(pwd.data()); 
	return *this;
}


bool Jingle::empty() const
{
	return child("content").empty();
}



bool Jingle::valid() const
{
	// TODO: Implement this properly
	return !child("content").empty();
}


Jingle::Content Jingle::content() const 
{ 
	XML::Node node = child("content");
	return Jingle::Content(node); 
}


Jingle::Description Jingle::description() const 
{ 
	XML::Node node = child("content").child("description");
	return Jingle::Description(node);
}


Jingle::Transport Jingle::transport() const 
{ 
	XML::Node node = child("content").child("transport");
	return Jingle::Transport(node);
}


Jingle::PayloadType Jingle::addPayloadType() 
{
	//Log("debug") << "Jingle::addPayloadType: " << endl;
	//this->print(cout);
	//Jingle::PayloadType node = (Jingle::PayloadType&)description().append_child();
	//node.set_name("payload-type");
	XML::Node node = child("content").child("description").append_child();
	return Jingle::PayloadType(node);
}


Jingle::Candidate Jingle::addCandidate() 
{
	//Jingle::Candidate node = (Jingle::Candidate&)transport().append_child();
	//node.set_name("candidate");
	XML::Node node = child("content").child("transport").append_child();
	return Jingle::Candidate(node);
}


Jingle::Candidate Jingle::firstCandidate() 
{
	XML::Node node = child("jingle").child("content").child("transport").child("candidate");
	if (!node.empty())
		return Jingle::Candidate(node);
	return Jingle::Candidate();
}


Jingle::Candidate Jingle::highestPriorityCandidate() const 
{
	unsigned int priority = 0;
	unsigned int value = 0;
	XML::Node trans = child("content").child("transport");
	for (XML::Node c = trans.child("candidate"); 
		c; c = c.next_sibling("candidate")) {
		value = c.attribute("priority").as_int();
		if (value > priority)
			priority = value;
	}

	for (XML::Node c = trans.child("candidate"); 
		c; c = c.next_sibling("candidate"))
		if (c.attribute("priority").as_int() == priority)
			return Jingle::Candidate(c);

	// Return an empty candidate if none exists
	return Jingle::Candidate();
}


// ---------------------------------------------------------------------
//
// Jingle::Content
//
// ---------------------------------------------------------------------
Jingle::Content::Content()
{
}


Jingle::Content::Content(XML::Node& node) :
	XML::Node(node)
{
	set_name("content");
}


Jingle::Content Jingle::Content::setCreator(const string& creator) 
{ 
	remove_attribute("creator"); 
	append_attribute("creator").set_value(creator.data()); 
	return *this;
}

Jingle::Content Jingle::Content::setName(const string& name) 
{ 
	remove_attribute("name"); 
	append_attribute("name").set_value(name.data()); 
	return *this;
}

Jingle::Content Jingle::Content::setSenders(const string& senders) 
{ 
	remove_attribute("senders"); 
	append_attribute("senders").set_value(senders.data()); 
	return *this;
}


Jingle::Description Jingle::Content::description() const 
{ 
	XML::Node node = child("description");
	return Jingle::Description(node); 
}

Jingle::Transport Jingle::Content::transport() const 
{ 
	XML::Node node = child("content").child("transport");
	return Jingle::Transport(node);
}


// ---------------------------------------------------------------------
//
// Jingle::Content::Description
//
// ---------------------------------------------------------------------
Jingle::Description::Description(XML::Node& node) :
	XML::Node(node)
{
	set_name("description");
}


Jingle::PayloadType Jingle::Description::addPayloadType() 
{
	//Node node = append_child();
	//node.set_name("payload-type");
	//Log("debug") << "Jingle::Description::addPayloadType: " << endl;
	//this->print(cout);
	XML::Node node = append_child();
	return Jingle::PayloadType(node);
}
		

Jingle::Description Jingle::Description::setMediaType(const string& media) 
{ 
	remove_attribute("media"); 
	append_attribute("media").set_value(media.data()); 
	//Log("debug") << "Jingle::Description::setMediaType: " << endl;
	//this->print(cout);
	return *this;
}


// ---------------------------------------------------------------------
//
// Jingle::Content::Transport
//
// ---------------------------------------------------------------------
Jingle::Transport::Transport(XML::Node& node) :
	XML::Node(node)
{
	set_name("transport");
}


Jingle::Candidate Jingle::Transport::addCandidate() 
{
	XML::Node node = append_child();
	return Jingle::Candidate(node);
}


Jingle::RemoteCandidate Jingle::Transport::addRemoteCandidate() 
{
	XML::Node node = append_child();
	return Jingle::RemoteCandidate(node);
}


// ---------------------------------------------------------------------
//
// Jingle::PayloadType
//
// ---------------------------------------------------------------------
Jingle::PayloadType::PayloadType(XML::Node& node) :
	XML::Node(node)
{
	set_name("payload-type");
}
	

Jingle::PayloadType Jingle::PayloadType::setId(int id) 
{ 
	remove_attribute("id"); 
	append_attribute("id").set_value(id); 
	//Log("debug") << "Jingle::Description::setMediaType: " << endl;
	//this->print(cout);
	return *this;
}


Jingle::PayloadType Jingle::PayloadType::setName(const string& name) 
{ 
	remove_attribute("name"); 
	append_attribute("name").set_value(name.data()); 
	return *this;
}


Jingle::PayloadType Jingle::PayloadType::setClockrate(int clockrate) 
{ 
	remove_attribute("clockrate"); 
	append_attribute("clockrate").set_value(clockrate); 
	return *this;
}


Jingle::PayloadType Jingle::PayloadType::setChannels(int channels) 
{ 
	remove_attribute("channels"); 
	append_attribute("channels").set_value(channels); 
	return *this;
}


// ---------------------------------------------------------------------
//
// Jingle::Candidate
//
// ---------------------------------------------------------------------
Jingle::Candidate::Candidate()
{
}


Jingle::Candidate::Candidate(XML::Node& node) :
	XML::Node(node)
{
	set_name("candidate");
}


Jingle::Candidate Jingle::Candidate::setComponent(int component) 
{ 
	remove_attribute("component"); 
	append_attribute("component").set_value(component); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setFoundation(const string& foundation) 
{ 
	remove_attribute("foundation"); 
	append_attribute("foundation").set_value(foundation.data()); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setGeneration(int generation) 
{ 
	remove_attribute("generation"); 
	append_attribute("generation").set_value(generation); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setId(const string& id) 
{ 
	remove_attribute("id"); 
	append_attribute("id").set_value(id.data()); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setIp(const string& ip) 
{ 
	remove_attribute("ip"); 
	append_attribute("ip").set_value(ip.data()); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setNetwork(int network) 
{ 
	remove_attribute("network"); 
	append_attribute("network").set_value(network); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setPort(int port) 
{ 
	remove_attribute("port"); 
	append_attribute("port").set_value(port); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setPriority(unsigned int priority)
{ 
	remove_attribute("priority"); 
	append_attribute("priority").set_value(priority); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setProtocol(const string& protocol) 
{ 
	remove_attribute("protocol"); 
	append_attribute("protocol").set_value(protocol.data()); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setRelAddr(const string& relAddr) 
{ 
	remove_attribute("rel-addr"); 
	append_attribute("rel-addr").set_value(relAddr.data()); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setRelPort(int relPort) 
{ 
	remove_attribute("rel-port"); 
	append_attribute("rel-port").set_value(relPort); 
	return *this;
}


Jingle::Candidate Jingle::Candidate::setType(const string& type) 
{ 
	remove_attribute("type"); 
	append_attribute("type").set_value(type.data()); 
	return *this;
}


// ---------------------------------------------------------------------
//
// Jingle::RemoteCandidate
//
// ---------------------------------------------------------------------
Jingle::RemoteCandidate::RemoteCandidate()
{
}


Jingle::RemoteCandidate::RemoteCandidate(XML::Node& node) :
	XML::Node(node)
{
	set_name("remote-candidate");
}


Jingle::RemoteCandidate Jingle::RemoteCandidate::setComponent(int component) 
{ 
	remove_attribute("component"); 
	append_attribute("component").set_value(component); 
	return *this;
}


Jingle::RemoteCandidate Jingle::RemoteCandidate::setIp(const string& ip) 
{ 
	remove_attribute("ip"); 
	append_attribute("ip").set_value(ip.data()); 
	return *this;
}


Jingle::RemoteCandidate Jingle::RemoteCandidate::setPort(int port) 
{ 
	remove_attribute("port"); 
	append_attribute("port").set_value(port); 
	return *this;
}


} // namespace Jingle
} // namespace XMPP 
} // namespace Sourcey 
