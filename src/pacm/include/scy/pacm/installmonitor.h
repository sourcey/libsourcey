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


#ifndef SCY_Pacm_InstallMonitor_H
#define SCY_Pacm_InstallMonitor_H


#include "scy/pacm/installtask.h"


namespace scy { 
namespace pacm {


typedef std::vector<LocalPackage*> LocalPackageVec;


class InstallMonitor
{
public:	
	InstallMonitor();
	virtual ~InstallMonitor();
	
	virtual void addTask(InstallTask::Ptr task);
		// Adds a task to monitor.

	virtual void startAll();
		// Starts all monitored tasks.

	virtual void cancelAll();
		// Cancels all monitored tasks.

	virtual bool isComplete() const;
		// Returns true if all install tasks have completed,
		// either successfully or unsuccessfully.

	virtual InstallTaskPtrVec tasks() const;
		// Returns the list of monitored package tasks.
	
	virtual LocalPackageVec packages() const;
		// Returns the list of monitored packages.
		
	Signal3<InstallTask&, const InstallationState&, const InstallationState&> InstallStateChange;
		// Proxies state change events from managed packages

	Signal<LocalPackage&> InstallComplete;
		// Signals when a managed install task completes.
	
	Signal<int&> Progress;
		// Signals on overall progress update [0-100].

	Signal<LocalPackageVec&> Complete;
		// Signals on all tasks complete.
	
protected:
	virtual void onInstallStateChange(void* sender, InstallationState& state, const InstallationState& oldState);
	virtual void onInstallComplete(void* sender);
		// Called when a monitored install task completes.
	
	virtual void setProgress(int value);

protected:
	mutable Mutex	_mutex;
	InstallTaskPtrVec _tasks;
	LocalPackageVec _packages;
	int _progress;
};


inline std::string getInstallTaskNamesString(LocalPackageVec& packages) 
	// Returns a comma delimited package name string.
{
	std::string names;
	for (auto it = packages.begin(); it != packages.end();) {
		names += (*it)->name();
		++it;
		if (it != packages.end())
			names += ", ";
	}
	return names;
}


} } // namespace scy::pacm


#endif // SCY_Pacm_InstallMonitor_H