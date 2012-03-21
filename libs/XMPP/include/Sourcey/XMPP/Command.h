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


#ifndef SOURCEY_XMPP_COMMAND_H
#define SOURCEY_XMPP_COMMAND_H


#include "Sourcey/XMPP/Stanza.h"
#include "Sourcey/XMPP/DataForm.h"


namespace Sourcey {
namespace XMPP {


struct Command: public XML::Node //Query 
{
public:	
	struct Default: public XML::Template
	{
		Default(
			const std::string node,
			const std::string status = "",
			const std::string action = "next",
			const std::string sessionid = "");
	};

	static const char* name()	{ return "command"; }
	static const char* xmlns()	{ return "http://jabber.org/protocol/commands"; }

	//static Command create(IQ& iq, XML::Template& t);
	
	Command();
	Command(XML::Node& node);
	Command(IQ& iq, XML::Template& t);
	virtual ~Command();

	bool active() const;
	void clear();
	bool valid();
	bool matches(const std::string& xnode) const;

	std::string status() const;
	std::string sessionid() const;
	std::string node() const;
	std::string note() const;
	std::string action() const;
	std::string param(int n) const;
	StringList params();
	//DataForm form(bool create = false);
	
	Command setNode(const std::string& node);	
	Command setSessionID(const std::string& id);
	
	Command setStatus(const std::string& status);	
		// Possible "status" values
		// executing 	The command is being executed.
		// completed 	The command has completed. The command session has ended.
		// canceled 	The command has been canceled. The command session has ended.

	Command setAction(const std::string& action);
		// Possible "action" values
		// execute 		The command should be executed or continue to be executed. This is the default value.
		// cancel 		The command should be canceled.
		// prev 		The command should be digress to the previous stage of execution.
		// next 		The command should progress to the next stage of execution.
		// complete 	The command should be completed (if possible).

	Command setNote(const std::string& type, const std::string& note);
		// Set's the note element, overriding all previous notes.
		// Possible "type" values
		// info 		The note is informational only. This is not really an exceptional condition.
		// warn 		The note indicates a warning. Possibly due to illogical (yet valid) data.
		// error 		The note indicates an error. The text should indicate the reason for the error.

	Command addNote(const std::string& type, const std::string& note);
		// The XMPP standard only allows us one note element, but where's the fun in following the rules.

	void clearNotes();

	// Non-Standard data carriers
	Node var(const std::string& name);
	
	Node addVar(const std::string& name, bool overwrite = true);
	Node addVar(Node& parent, const std::string& name, bool overwrite = true);

	Node addVar(const std::string& name, const Node& xml, bool overwrite = true);
	Node addVar(const std::string& name, const std::string& data, bool overwrite = true);
	//Command addVar(const std::string& name, const char* data);
	Node addVar(Node& parent, const std::string& name, const std::string& data, bool overwrite = true);
	//Command addVar(const Node& xml, bool overwrite = true);
	
	void removeVar(const std::string& name);
};


struct CommandList: public XML::Node 
{
public:
	CommandList() {
		set_name("commands");
	}

	void add(const std::string& name, const std::string& node, const std::string& description, bool enabled = true) {
		pugi::xml_node xml = append_child();
		xml.set_name("command");
		xml.append_attribute("name").set_value(name.data());
		xml.append_attribute("node").set_value(node.data());
		xml.append_attribute("description").set_value(description.data());
		xml.append_attribute("enabled").set_value(enabled);
	}
};


} // namespace XMPP 
} // namespace Sourcey


#endif // SOURCEY_XMPP_COMMAND_H
