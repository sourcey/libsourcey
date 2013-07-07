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


#ifndef SOURCEY_Pacman_InstallMonitor_H
#define SOURCEY_Pacman_InstallMonitor_H


#include "Sourcey/Pacman/InstallTask.h"


namespace scy { 
namespace pcman {


typedef std::vector<LocalPackage*> LocalPackageList;


class InstallMonitor
{
public:	
	InstallMonitor();
	virtual ~InstallMonitor();
	
	virtual void addTask(InstallTask* task);
		/// Adds a task to monitor.

	virtual void startAll();
		/// Starts all monitored tasks.

	virtual void cancelAll();
		/// Cancels all monitored tasks.

	virtual bool isComplete() const;
		/// Returns true if all install tasks have completed,
		/// either successfully or unsuccessfully.

	virtual InstallTaskList tasks() const;
		/// Returns the list of monitored package tasks.
	
	virtual LocalPackageList packages() const;
		/// Returns the list of monitored packages.
		
	Signal3<InstallTask&, PackageInstallState&, const PackageInstallState&> InstallStateChange;
		/// Proxies state change events from managed packages

	Signal<LocalPackage&> InstallComplete;
		/// Signals when a managed install task completes.
	
	Signal<int&> Progress;
		/// Signals on overall progress update [0-100].

	Signal<LocalPackageList&> Complete;
		/// Signals on all tasks complete.
	
protected:
	virtual void onInstallStateChange(void* sender, PackageInstallState& state, const PackageInstallState& oldState);
	virtual void onInstallComplete(void* sender);
		/// Called when a monitored install task completes.
	
	virtual void setProgress(int value);

protected:
	mutable Poco::FastMutex	_mutex;

	InstallTaskList _tasks;
	LocalPackageList _packages;
	int _progress;
};


inline std::string getInstallTaskNamesString(LocalPackageList& packages) 
	/// Returns a comma delimited package name string.
{
	std::string names;
	for (LocalPackageList::const_iterator it = packages.begin(); it != packages.end();) {
		names += (*it)->name();
		++it;
		if (it != packages.end())
			names += ", ";
	}
	return names;
}


} } // namespace scy::Pacman


#endif // SOURCEY_Pacman_InstallMonitor_H