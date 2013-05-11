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


#include "Sourcey/Util/DiagnosticManager.h"


using namespace std;


namespace Scy {
	
	
IDiagnostic::IDiagnostic()
{
}


IDiagnostic::~IDiagnostic()
{
}
	

void IDiagnostic::reset() 
{ 
	summary.clear();
	setState(this, DiagnosticState::None); 
}


void IDiagnostic::check() 
{ 
	reset(); 
	run(); 
}


void IDiagnostic::addSummary(const std::string& text)
{
	summary.push_back(text);
	SummaryUpdated.emit(this, text);
}

	
bool IDiagnostic::pass() 
{ 
	return setState(this, DiagnosticState::Passed); 
}

	
bool IDiagnostic::fail() 
{ 
	return setState(this, DiagnosticState::Failed); 
}

	
bool IDiagnostic::complete() const 
{ 
	return stateEquals(DiagnosticState::Passed)
		|| stateEquals(DiagnosticState::Failed); 
}

	
bool IDiagnostic::passed() const 
{ 
	return stateEquals(DiagnosticState::Passed)
		|| stateEquals(DiagnosticState::Failed); 
}

	
bool IDiagnostic::failed() const 
{ 
	return stateEquals(DiagnosticState::Failed); 
}


// ---------------------------------------------------------------------
//	
DiagnosticManager::DiagnosticManager()
{	
	LogTrace() << "Creating" << endl;
}


DiagnosticManager::~DiagnosticManager() 
{
	LogTrace() << "Destroying" << endl;
}

void DiagnosticManager::resetAll()
{
	Map tests = items();
	for (Map::const_iterator it = tests.begin(); it != tests.end(); ++it) {	
		it->second->reset();
	}
}


void DiagnosticManager::checkAll()
{
	Map tests = items();
	for (Map::const_iterator it = tests.begin(); it != tests.end(); ++it) {	
		it->second->check();
	}
}
	

bool DiagnosticManager::allComplete()
{
	Map tests = items();
	for (Map::const_iterator it = tests.begin(); it != tests.end(); ++it) {	
		if (!it->second->complete())
			return false;
	}
	return true;
}


bool DiagnosticManager::addDiagnostic(IDiagnostic* test) 
{
	assert(test);
	assert(!test->name.empty());
	
	LogTrace() << "Adding Diagnostic: " << test->name << endl;	
	test->StateChange += delegate(this, &DiagnosticManager::onDiagnosticStateChange);
	return DiagnosticStore::add(test->name, test);
}


bool DiagnosticManager::freeDiagnostic(const string& name) 
{
	assert(!name.empty());

	LogTrace() << "Removing Diagnostic: " << name << endl;	
	IDiagnostic* test = DiagnosticStore::remove(name);
	if (test) {
		// TODO: 
		test->StateChange -= delegate(this, &DiagnosticManager::onDiagnosticStateChange);
		delete test;
		return true;
	}
	return false;
}


IDiagnostic* DiagnosticManager::getDiagnostic(const string& name) 
{
	return DiagnosticStore::get(name);
}


void DiagnosticManager::onDiagnosticStateChange(void* sender, DiagnosticState& state, const DiagnosticState&)
{
	IDiagnostic* test = reinterpret_cast<IDiagnostic*>(sender);
	LogTrace() << "Diagnostic State Change: " << test->name << ": " << state.toString() << endl;

	if (test->complete() && allComplete())
		DiagnosticsComplete.emit(this);
}


} // namespace Scy
