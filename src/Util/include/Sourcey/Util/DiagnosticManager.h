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


#ifndef SOURCEY_DiagnosticManager_H
#define SOURCEY_DiagnosticManager_H


#include "Sourcey/Containers.h"
#include "Sourcey/Stateful.h"
#include "Poco/Thread.h"


namespace scy {
	
	
struct DiagnosticState: public State 
{
	enum Type 
	{
		None = 0,
		Checking,
		Passed,
		Failed
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Checking:		return "Checking";
		case Passed:		return "Passed";
		case Failed:		return "Failed";
		default:			assert(false);
		}
		return "undefined"; 
	}
};


// ---------------------------------------------------------------------
//
class IDiagnostic: public StatefulSignal<DiagnosticState>
{
public:
	IDiagnostic();
	virtual ~IDiagnostic();

	std::string name;        /// The name of the diagnostic.
	std::string description; /// The diagnostic description.
	StringVec summary;		 /// The diagnostic summary, maybe including 
							 /// troubleshooting information on failure.
	
	virtual void check();
	virtual void reset();

	virtual bool complete() const;
	virtual bool passed() const;
	virtual bool failed() const;

	Signal<const std::string&> SummaryUpdated;
		/// Fires when a new text item is added 
		/// to the summary.

	/// The StateChange signal will dispatch
	/// diagnostic test results to delegates.

protected:
	virtual void run() = 0;	
		/// Override to implement diagnostic logic.
	
	virtual bool pass();
	virtual bool fail();
	virtual void addSummary(const std::string& text);
};


// ---------------------------------------------------------------------
//
class IAsyncDiagnostic: public IDiagnostic, public Poco::Runnable
{
public:
	virtual ~IAsyncDiagnostic() {};

	virtual void check() {
		reset();
		_thread.start(*this);
	};

protected:
	Poco::Thread _thread;
};


// ---------------------------------------------------------------------
//
typedef PointerManager<std::string, IDiagnostic> DiagnosticStore;	

	
class DiagnosticManager: public DiagnosticStore
{
public:
	DiagnosticManager();
	virtual ~DiagnosticManager();
	
	bool freeDiagnostic(const std::string& name);
	
	bool addDiagnostic(IDiagnostic* test);
		// Adds a IDiagnostic test instance.

	virtual IDiagnostic* getDiagnostic(const std::string& name);
		// Returns the IDiagnostic instance or throws
		// a NotFoundException exception.

	virtual void resetAll();

	virtual void checkAll();
		// Runs all managed IDiagnostic tests.
		// DiagnosticsComplete will be dispatched on
		// completion.
	
	virtual bool allComplete();

	NullSignal DiagnosticsComplete;

	virtual void onDiagnosticStateChange(void*, DiagnosticState& state, const DiagnosticState&);
	
	virtual const char* className() const { return "DiagnosticManager"; }
};


} // namespace scy


#endif // SOURCEY_DiagnosticManager_H