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


#ifndef SOURCEY_Pacman_PackageInstallTask_H
#define SOURCEY_Pacman_PackageInstallTask_H


#include "Sourcey/Pacman/Package.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Logger.h"
#include "Sourcey/HTTP/Transaction.h"

#include "Poco/Zip/ZipLocalFileHeader.h"


namespace Sourcey { 
namespace Pacman {

	
class PackageManager;


struct PackageInstallState: public StateT 
{
	enum Type 
	{
		None = 0,
		Downloading,
		Unpacking,
		Finalizing,
		//Incomplete,
		Installed,
		Failed
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:					return "None";
		case Downloading:			return "Downloading";
		case Unpacking:				return "Unpacking";
		case Finalizing:			return "Finalizing";
		//case Incomplete:			return "Incomplete";
		case Installed:				return "Installed";
		case Failed:				return "Failed";
		default: assert(false);
		}
		return "undefined"; 
	}
};


class PackageInstallTask: public Poco::Runnable, public StatefulSignal<PackageInstallState>, public ILoggable
	/// This class is responsible for the installation process
	/// of a single package.
	///
	/// TODO: Revise mutex usage for better thread safety.
{
public:
	struct Options 
		/// Package installation options.
	{
		std::string version;		// If set then the specified package version will
									// be installed.
		std::string projectVersion; // If set only package versions with the specified
									// parent project version will be installed.

		Options() {
			version = "";
			projectVersion = "";
		}
	};

public:
	PackageInstallTask(PackageManager& manager, LocalPackage* local, RemotePackage* remote, const Options& options = Options());
	virtual ~PackageInstallTask();	

	virtual void start();
	virtual void cancel();

	virtual void run();
		/// Called asynchronously by the thread to
		/// kick off the state machine.

	virtual void doDownload();
		/// Downloads the package archive from the
		/// server.

	virtual void doUnpack();
		/// Unpacks downloaded package files to the
		/// intermediate directory.

	virtual void doFinalize();
		/// Moves extracted files from the intermediate
		/// directory to the installation directory.
	
	virtual void setComplete();
		/// Called when the task completes either
		/// successfully or in error.

	virtual LocalPackage* local() const;
	virtual RemotePackage* remote() const;
	
	virtual bool cancelled() const;
	virtual bool failed() const;
	virtual bool success() const;
	virtual bool complete() const;
	virtual bool valid() const;

	virtual void onStateChange(PackageInstallState& state, const PackageInstallState& oldState);

	virtual void onDecompressionError(const void*, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);
	virtual void onDecompressionOk(const void*, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info);

	//virtual void printLog(std::ostream& ost) const;	
	virtual const char* className() const { return "PackageInstallTask"; }
	
	NullSignal TaskComplete;

protected:
	mutable Poco::FastMutex	_mutex;
	
	Poco::Thread	_thread;
	PackageManager& _manager;
	LocalPackage*	_local;
	RemotePackage*	_remote;
	Options			_options;
	bool			_cancelled;
	HTTP::Transaction _transaction;
	
	friend class PackageManager;
	friend class PackageInstallMonitor;	
};


typedef std::vector<PackageInstallTask*> PackageInstallTaskList;


} } // namespace Sourcey::Pacman


#endif // SOURCEY_Pacman_PackageInstallTask_H