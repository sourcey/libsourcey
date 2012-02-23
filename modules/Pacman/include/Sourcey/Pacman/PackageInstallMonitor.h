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


#ifndef SOURCEY_Pacman_PackageInstallMonitor_H
#define SOURCEY_Pacman_PackageInstallMonitor_H


#include "Sourcey/Pacman/PackageInstallTask.h"


namespace Sourcey { 
namespace Pacman {


typedef std::vector<LocalPackage*> LocalPackageList;


class PackageInstallMonitor
{
public:	
	PackageInstallMonitor();
	virtual ~PackageInstallMonitor();
	
	virtual void addTask(PackageInstallTask* task);

	virtual PackageInstallTaskList tasks() const;
		/// Returns the list of monitored package tasks.
	
	virtual LocalPackageList packages() const;
		/// Returns the list of monitored packages.

	bool isComplete() const;
		
	Signal<LocalPackage&> PackageInstallComplete;
	
protected:
	virtual void onPackageInstallComplete(void* sender);
		/// Called when a monitored install task completes.

	PackageInstallTaskList _tasks;
	LocalPackageList _packages;
};


inline std::string getPackageInstallTaskNamesString(LocalPackageList& packages) 
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


} } // namespace Sourcey::Pacman


#endif // SOURCEY_Pacman_PackageInstallMonitor_H