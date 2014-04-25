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


#ifndef SCY_Pacm_InstallTask_H
#define SCY_Pacm_InstallTask_H


#include "scy/pacm/package.h"
#include "scy/http/client.h"
#include "scy/stateful.h"
#include "scy/logger.h"
#include "scy/idler.h"


namespace scy { 
namespace pacm {

	
class PackageManager;


struct InstallationState: public State 
{
	enum Type 
	{
		None = 0,
		Downloading,
		Extracting,
		Finalizing,
		Installed,
		Cancelled,
		Failed
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:					return "None";
		case Downloading:			return "Downloading";
		case Extracting:			return "Extracting";
		case Finalizing:			return "Finalizing";
		case Installed:				return "Installed";
		case Cancelled:				return "Cancelled";
		case Failed:				return "Failed";
		default: assert(false);
		}
		return "undefined"; 
	}
};


struct InstallOptions 
	// Package installation options.
{
	std::string version;			// If set then the given package version will be installed.
	std::string sdkVersion;			// If set then the latest package version for given SDK
									// version will be installed.
	std::string installDir;			// Install to the given location, otherwise the manager default
									// installDir will be used.
	//bool copyManifestToInstallDir;  // Copys the local package.json file to the install directory.

	InstallOptions() {
		version = "";
		sdkVersion = "";
		installDir = "";
	}
};


class InstallTask: 
	public async::Runnable, 
	public Stateful<InstallationState>
	///
	/// This class implements the package 
	/// installation procedure.
{
public:
	typedef std::shared_ptr<InstallTask> Ptr;

	InstallTask(PackageManager& manager, 
				LocalPackage* local, RemotePackage* remote, 
				const InstallOptions& options = InstallOptions(), 
				uv::Loop* loop = uv::defaultLoop());
	virtual ~InstallTask();	

	virtual void start();
	virtual void cancel();

	virtual void doDownload();
		// Downloads the package archive from the server.

	virtual void doExtract();
		// Extracts the downloaded package files
		// to the intermediate directory.

	virtual void doFinalize();
		// Moves extracted files from the intermediate
		// directory to the installation directory.
	
	virtual void setComplete();
		// Called when the task completes either
		// successfully or in error.
		// This will trigger destruction.

	virtual Package::Asset getRemoteAsset() const;

	virtual LocalPackage* local() const;
	virtual RemotePackage* remote() const;
	virtual InstallOptions& options();
	virtual uv::Loop* loop() const;
	
	virtual bool valid() const;
	virtual bool cancelled() const;
	virtual bool failed() const;
	virtual bool success() const;
	virtual bool complete() const;
	virtual int progress() const;
	
	Signal<int&> Progress;
		// Signals on progress update [0-100].
	
	NullSignal Complete;
		// Signals on task completion for both
		// success and failure cases.
	
protected:
	virtual void run();
		// Called asynchronously by the thread to
		// do the work.

	virtual void onStateChange(InstallationState& state, const InstallationState& oldState);
	virtual void onDownloadProgress(void* sender, const double& progress);
	virtual void onDownloadComplete(void* sender, const http::Response& response);

	virtual void setProgress(int value);

protected:
	mutable Mutex	_mutex;
	
	Idler			_runner;
	//Thread          _thread;
	PackageManager& _manager;
	LocalPackage*	_local;
	RemotePackage*	_remote;
	InstallOptions	_options;
	int             _progress;
	bool			_downloading;
	uv::Loop*       _loop;
	http::ClientConnection::Ptr _dlconn;
	
	friend class PackageManager;
	friend class InstallMonitor;	
};


typedef std::vector<InstallTask*> InstallTaskVec;
typedef std::vector<InstallTask::Ptr> InstallTaskPtrVec;


} } // namespace scy::pacm


#endif // SCY_Pacm_InstallTask_H