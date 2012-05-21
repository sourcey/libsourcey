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


#include "Sourcey/Pacman/PackageInstallMonitor.h"
#include "Sourcey/Pacman/PackageManager.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace Pacman {


PackageInstallMonitor::PackageInstallMonitor()
{
}


PackageInstallMonitor::~PackageInstallMonitor()
{
}


void PackageInstallMonitor::onPackageInstallStateChange(void* sender, PackageInstallState& state, const PackageInstallState& oldState)
{
	PackageInstallTask* task = reinterpret_cast<PackageInstallTask*>(sender);

	Log("debug") << "[PackageInstallMonitor] onPackageInstallStateChange: " << task << ": " << state.toString() << endl;

	PackageInstallStateChange.dispatch(this, *task, state, oldState);
}


void PackageInstallMonitor::onPackageInstallComplete(void* sender) 
{
	PackageInstallTask* task = reinterpret_cast<PackageInstallTask*>(sender);

	Log("debug") << "[PackageInstallMonitor] Package Install Complete: " << task->state().toString() << endl;

	// Notify listeners when each package completes.
	PackageInstallComplete.dispatch(this, *task->_local);
		
	{
		FastMutex::ScopedLock lock(_mutex);

		// Remove the package task reference.
		for (PackageInstallTaskList::iterator it = _tasks.begin(); it != _tasks.end(); it++) {
			if (task == *it) {
				task->StateChange -= delegate(this, &PackageInstallMonitor::onPackageInstallStateChange); 
				task->TaskComplete -= delegate(this, &PackageInstallMonitor::onPackageInstallComplete);
				_tasks.erase(it);
				break;
			}
		}

		Log("info") << "[PackageInstallMonitor] Waiting on " 
			<< _tasks.size() << " packages to complete" << endl;
	}

	if (isComplete())
		PackageInstallationsComplete.dispatch(this, _packages);
}


void PackageInstallMonitor::addTask(PackageInstallTask* task)
{
	FastMutex::ScopedLock lock(_mutex);
	if (!task->valid())
		throw Exception("Invalid package task");
	_tasks.push_back(task);
	_packages.push_back(task->_local);
	task->StateChange += delegate(this, &PackageInstallMonitor::onPackageInstallStateChange);
	task->TaskComplete += delegate(this, &PackageInstallMonitor::onPackageInstallComplete);
}


void PackageInstallMonitor::cancelAll()
{	
	FastMutex::ScopedLock lock(_mutex);
	for (PackageInstallTaskList::iterator it = _tasks.begin(); it != _tasks.end(); it++) {
		(*it)->cancel();
	}
}


PackageInstallTaskList PackageInstallMonitor::tasks() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _tasks; 
}


LocalPackageList PackageInstallMonitor::packages() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _packages; 
}


bool PackageInstallMonitor::isComplete() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _tasks.empty(); 
}


} } // namespace Sourcey::Pacman



	//task->StateChange += delegate(this, &PackageInstallMonitor::onPackageInstallComplete);
	// When all packages complete update our state to
	// notify listeners.
	//if (_tasks.empty())
	//	setState(this, PackageInstallMonitorState::Complete);
	//else
	
	// Set the state to running on the first callback we get.
	//if (stateEquals(PackageInstallMonitorState::None))
	//	setState(this, PackageInstallMonitorState::Running);
		
	// Only continue if the current package has completed, 
	// either successfully or erroneously.
	//if (state.id() == PackageInstallState::Installed ||
	//	state.id() == PackageInstallState::Failed) {

	//}
	/*
			task->StateChange -= delegate(this, &PackageInstallMonitor::onPackageInstallComplete);
	// Set the state to running on the first callback we get.
	//if (stateEquals(PackageInstallMonitorState::None))
	//	setState(this, PackageInstallMonitorState::Running);
		
	// Only continue if the current package has completed, 
	// either successfully or erroneously.
	if (state.id() == PackageInstallState::Installed ||
		state.id() == PackageInstallState::Failed) {

		// When all packages complete update our state to
		// notify listeners.
		if (_tasks.empty())
			setState(this, PackageInstallMonitorState::Complete);
		else
			Log("info") << "[PackageInstallMonitor] Waiting on " 
				<< _tasks.size() << " packages to complete" << endl;

		// Notify listeners when each package completes.
		PackageInstallComplete.dispatch(this, *task->local);
		
		// Remove the package reference.
		for (PackageInstallTaskList::iterator it = _tasks.begin(); it != _tasks.end();) {
			if (task == *it) {
				task->StateChange -= delegate(this, &PackageInstallMonitor::onPackageInstallComplete);
				_tasks.erase(it);
				break;
			}
		}
	}
	*/

/*
PackageInstallTask* PackageInstallMonitor::getTask(const std::string& name) const
{
	for (PackageInstallTaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
		if ((*it)->name() == name)
			return *it;
	}
	return NULL;
}
*/
		
		/*, PackageInstallState& state, const PackageInstallState&
		int numWaiting = 0;		
		PackageInstallTaskList::iterator it = _packages.begin();
		while (it != _packages.end()) {
			if (*it == task)
				it = _packages.erase(it);
			if (*it == task)
				it = _packages.erase(it);
			else {			
				if (!(*it)->stateEquals(PackageInstallState::Complete) ||
					!(*it)->stateEquals(PackageInstallState::Failed)) {
					numWaiting++;
				}
				it++;
			}
		}
			*/
			/*
			Log("debug") << "[PacketStream:" << this << "] Detaching Processor: " << (*it).ptr << endl;
			(*it).ptr->detach(polymorphicDelegate(this, &PacketStream::onProcessedPacket));
			if ((*it).freePointer)
				delete (*it).ptr;
			it = _packages.erase(it);
			*/

		/*
		for (PackageInstallTaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
			if (!(*it)->stateEquals(PackageInstallState::Complete) ||
				!(*it)->stateEquals(PackageInstallState::Failed)) {
				numWaiting++;
			}
		}
		*/
