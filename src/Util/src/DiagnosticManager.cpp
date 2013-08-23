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


#include "Sourcey/Util/DiagnosticManager.h"


using namespace std;


namespace scy {
	
	
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
	traceL() << "Create" << endl;
}


DiagnosticManager::~DiagnosticManager() 
{
	traceL() << "Destroy" << endl;
}

void DiagnosticManager::resetAll()
{
	Map tests = map();
	for (Map::const_iterator it = tests.begin(); it != tests.end(); ++it) {	
		it->second->reset();
	}
}


void DiagnosticManager::checkAll()
{
	Map tests = map();
	for (Map::const_iterator it = tests.begin(); it != tests.end(); ++it) {	
		it->second->check();
	}
}
	

bool DiagnosticManager::allComplete()
{
	Map tests = map();
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
	
	traceL() << "Adding Diagnostic: " << test->name << endl;	
	test->StateChange += delegate(this, &DiagnosticManager::onDiagnosticStateChange);
	return DiagnosticStore::add(test->name, test);
}


bool DiagnosticManager::freeDiagnostic(const std::string& name) 
{
	assert(!name.empty());

	traceL() << "Removing Diagnostic: " << name << endl;	
	IDiagnostic* test = DiagnosticStore::remove(name);
	if (test) {
		// TODO: 
		test->StateChange -= delegate(this, &DiagnosticManager::onDiagnosticStateChange);
		delete test;
		return true;
	}
	return false;
}


IDiagnostic* DiagnosticManager::getDiagnostic(const std::string& name) 
{
	return DiagnosticStore::get(name, true);
}


void DiagnosticManager::onDiagnosticStateChange(void* sender, DiagnosticState& state, const DiagnosticState&)
{
	IDiagnostic* test = reinterpret_cast<IDiagnostic*>(sender);
	traceL() << "Diagnostic state change: " << test->name << ": " << state.toString() << endl;

	if (test->complete() && allComplete())
		DiagnosticsComplete.emit(this);
}


} // namespace scy
