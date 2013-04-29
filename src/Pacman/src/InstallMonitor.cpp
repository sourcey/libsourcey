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


#include "Sourcey/Pacman/InstallMonitor.h"
#include "Sourcey/Pacman/PackageManager.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace Pacman {


InstallMonitor::InstallMonitor()
{
}


InstallMonitor::~InstallMonitor()
{
}


void InstallMonitor::onInstallStateChange(void* sender, PackageInstallState& state, const PackageInstallState& oldState)
{
	InstallTask* task = reinterpret_cast<InstallTask*>(sender);

	LogDebug() << "[InstallMonitor] onInstallStateChange: " << task << ": " << state.toString() << endl;

	InstallStateChange.emit(this, *task, state, oldState);
}


void InstallMonitor::onInstallComplete(void* sender) 
{
	InstallTask* task = reinterpret_cast<InstallTask*>(sender);

	LogDebug() << "[InstallMonitor] Package Install Complete: " << task->state().toString() << endl;

	// Notify listeners when each package completes.
	InstallComplete.emit(this, *task->local());
	
	int progress = 0;
	{
		FastMutex::ScopedLock lock(_mutex);

		// Remove the package task reference.
		for (InstallTaskList::iterator it = _tasks.begin(); it != _tasks.end(); it++) {
			if (task == *it) {
				task->StateChange -= delegate(this, &InstallMonitor::onInstallStateChange); 
				task->Complete -= delegate(this, &InstallMonitor::onInstallComplete);
				_tasks.erase(it);
				break;
			}
		}

		progress = (_packages.size() - _tasks.size()) / _packages.size();

		LogInfo() << "[InstallMonitor] Waiting on " 
			<< _tasks.size() << " packages to complete" << endl;
	}

	// Set progress
	setProgress(progress);

	if (isComplete())
		Complete.emit(this, _packages);
}


void InstallMonitor::addTask(InstallTask* task)
{
	FastMutex::ScopedLock lock(_mutex);
	if (!task->valid())
		throw Exception("Invalid package task");
	_tasks.push_back(task);
	_packages.push_back(task->_local);
	task->StateChange += delegate(this, &InstallMonitor::onInstallStateChange);
	task->Complete += delegate(this, &InstallMonitor::onInstallComplete);
}


void InstallMonitor::startAll()
{	
	FastMutex::ScopedLock lock(_mutex);
	for (InstallTaskList::iterator it = _tasks.begin(); it != _tasks.end(); it++)
		(*it)->start();
}


void InstallMonitor::cancelAll()
{	
	FastMutex::ScopedLock lock(_mutex);
	for (InstallTaskList::iterator it = _tasks.begin(); it != _tasks.end(); it++)
		(*it)->cancel();
}


void InstallMonitor::setProgress(int value)
{
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		_progress = value;
	}
	Progress.emit(this, value);
}


InstallTaskList InstallMonitor::tasks() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _tasks; 
}


LocalPackageList InstallMonitor::packages() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _packages; 
}


bool InstallMonitor::isComplete() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _tasks.empty(); 
}


} } // namespace Sourcey::Pacman



	//task->StateChange += delegate(this, &InstallMonitor::onInstallComplete);
	// When all packages complete update our state to
	// notify listeners.
	//if (_tasks.empty())
	//	setState(this, InstallMonitorState::Complete);
	//else
	
	// Set the state to running on the first callback we get.
	//if (stateEquals(InstallMonitorState::None))
	//	setState(this, InstallMonitorState::Running);
		
	// Only continue if the current package has completed, 
	// either successfully or erroneously.
	//if (state.id() == PackageInstallState::Installed ||
	//	state.id() == PackageInstallState::Failed) {

	//}
	/*
			task->StateChange -= delegate(this, &InstallMonitor::onInstallComplete);
	// Set the state to running on the first callback we get.
	//if (stateEquals(InstallMonitorState::None))
	//	setState(this, InstallMonitorState::Running);
		
	// Only continue if the current package has completed, 
	// either successfully or erroneously.
	if (state.id() == PackageInstallState::Installed ||
		state.id() == PackageInstallState::Failed) {

		// When all packages complete update our state to
		// notify listeners.
		if (_tasks.empty())
			setState(this, InstallMonitorState::Complete);
		else
			LogInfo() << "[InstallMonitor] Waiting on " 
				<< _tasks.size() << " packages to complete" << endl;

		// Notify listeners when each package completes.
		InstallComplete.emit(this, *task->local);
		
		// Remove the package reference.
		for (InstallTaskList::iterator it = _tasks.begin(); it != _tasks.end();) {
			if (task == *it) {
				task->StateChange -= delegate(this, &InstallMonitor::onInstallComplete);
				_tasks.erase(it);
				break;
			}
		}
	}
	*/

/*
InstallTask* InstallMonitor::getTask(const std::string& name) const
{
	for (InstallTaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
		if ((*it)->name() == name)
			return *it;
	}
	return NULL;
}
*/
		
		/*, PackageInstallState& state, const PackageInstallState&
		int numWaiting = 0;		
		InstallTaskList::iterator it = _packages.begin();
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
			LogDebug() << "[PacketStream:" << this << "] Detaching Processor: " << (*it).ptr << endl;
			(*it).ptr->detach(polymorphicDelegate(this, &PacketStream::onProcessedPacket));
			if ((*it).freePointer)
				delete (*it).ptr;
			it = _packages.erase(it);
			*/

		/*
		for (InstallTaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
			if (!(*it)->stateEquals(PackageInstallState::Complete) ||
				!(*it)->stateEquals(PackageInstallState::Failed)) {
				numWaiting++;
			}
		}
		*/
