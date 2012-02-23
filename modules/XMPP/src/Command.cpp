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


#include "Sourcey/XMPP/Command.h"
#include "Sourcey/Util.h"
#include "Sourcey/CryptoProvider.h"
#include "Poco/Format.h"
#include "assert.h"


using namespace std;
using namespace Sourcey::XML;
using namespace Poco;


namespace Sourcey {
namespace XMPP {


Command::Default::Default(
	const string node,
	const string status,
	const string action,
	const string sessionid) 
{
	data = format(
		"<command xmlns='http://jabber.org/protocol/commands' node='%s' status='%s' action='%s' sessionid='%s'/>\n", 
		node, 
		status, 
		action, 
		sessionid.empty() ? CryptoProvider::generateRandomKey(8) : sessionid);
}


Command::Command() 
{
}


Command::Command(XML::Node& node) :
	XML::Node(node)
{
	assert(strcmp(name(), "command") == 0);
}


Command::Command(IQ& iq, XML::Template& t) :
	XML::Node(iq.rootNode().append(t.data))
		//iq.child("command").empty() ?
		//iq.appendChild("command") : 
		//iq.child("command"))
{
	remove_attribute("xmlns");
	append_attribute("xmlns").set_value(Command::xmlns());
}


Command::~Command() 
{
}


bool Command::valid() 
{
	return !params().empty() 
		&& !sessionid().empty() 
		&& !node().empty();
}


bool Command::active() const 
{
	return status() != "completed"
		&& status() != "canceled";
}


string Command::status() const 
{
	return attribute("status").value();
}


string Command::sessionid() const 
{
	return attribute("sessionid").value();
}


string Command::node() const 
{
	return attribute("node").value();
}


string Command::action() const 
{
	string action = attribute("action").value();
	//if (action.empty())
	//	action = _count > 1 ? "next" : "execute";
	return action;
}


void Command::clear() 
{
    for (pugi::xml_node child = first_child(); child; child = child.next_sibling())
		remove_child(child);
}


string Command::note() const 
{
	return child("note").child_value();
}


string Command::param(int n) const 
{
	StringList params = Util::split(node(), ":");
	assert(params.size() >= n);
	if (params.size() < n)
		return "";
	return params[n-1].data();
}


StringList Command::params() 
{
	StringList params = Util::split(node(), ":");
	return params;
}


Command Command::setNode(const std::string& node)
{
	remove_attribute("node");
	append_attribute("node").set_value(node.data());
	return *this;
}


Command Command::setSessionID(const string& id) 
{
	remove_attribute("sessionid");
	append_attribute("sessionid").set_value(id.data());
	return *this;
}


Command Command::setStatus(const string& status) 
{
	assert(
		status == "executing" ||
		status == "completed" ||
		status == "canceled"
	);
	remove_attribute("status");
	append_attribute("status").set_value(status.data());
	return *this;
}

	
Command Command::setAction(const string& action) 
{
	assert(
		action == "execute" ||
		action == "cancel"	||
		action == "prev"	||
		action == "next"	||
		action == "complete"
	);
	remove_attribute("action");
	append_attribute("action").set_value(action.data());
	return *this;
}


Command Command::setNote(const string& type, const string& note) 
{
	clearNotes();
	return addNote(type, note);
}

	
Command Command::addNote(const string& type, const string& note) 
{
	assert(
		type == "info" ||
		type == "warn" ||
		type == "error"
	);
	Node node = append_child();
	node.set_name("note");
	node.append_attribute("type").set_value(type.data());
	node.append_child(pugi::node_pcdata).set_value(note.data());
	return *this;
}


void Command::clearNotes() 
{
    pugi::xml_node notes = child("note");
    for (pugi::xml_node note = notes.first_child(); note; note = note.next_sibling())
		remove_child(note.name());
}


Node Command::var(const string& name)
{
	return select_single_node(format("//var[@name='%s']", name).data()).node();
}


Node Command::addVar(Node& parent, const std::string& name, bool overwrite)
{
	if (overwrite)
		removeVar(name);	

	Node node = parent.append_child();
	node.set_name("var");
	node.append_attribute("name").set_value(name.data());
	return node;
}


Node Command::addVar(const std::string& name, bool overwrite)
{
	return addVar(*this, name, overwrite);
}


Node Command::addVar(const string& name, const Node& xml, bool overwrite) 
{
	Node node = addVar(*this, name, overwrite);
	node.append_copy(xml);
	return node;
}


Node Command::addVar(const string& name, const string& data, bool overwrite) 
{
	Node node = addVar(*this, name, overwrite);
	node.append_child(pugi::node_pcdata).set_value(data.data());
	return node;
}


Node Command::addVar(Node& parent, const string& name, const string& data, bool overwrite)
{
	Node node = addVar(parent, name, overwrite);
	node.append_child(pugi::node_pcdata).set_value(data.data());
	return node;
}


void Command::removeVar(const string& name)
{
	Node node = var(name);
	if (!node.empty())
		node.parent().remove_child(node);
}


bool Command::matches(const string& xnode) const 
{
	return Util::matchNodes(node(), xnode, ":");
}


} // namespace XMPP 
} // namespace Sourcey