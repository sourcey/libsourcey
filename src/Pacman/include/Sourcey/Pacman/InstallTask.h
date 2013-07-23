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


#ifndef SOURCEY_Pacman_InstallTask_H
#define SOURCEY_Pacman_InstallTask_H


#include "Sourcey/Pacman/Package.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Logger.h"
#include "Sourcey/HTTP/Client.h"

#include "Poco/Zip/ZipLocalFileHeader.h"


namespace scy { 
namespace pman {

	
class PackageManager;


struct PackageInstallState: public State 
{
	enum Type 
	{
		None = 0,
		Downloading,
		Unpacking,
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
		case Unpacking:				return "Unpacking";
		case Finalizing:			return "Finalizing";
		case Installed:				return "Installed";
		case Cancelled:				return "Cancelled";
		case Failed:				return "Failed";
		default: assert(false);
		}
		return "undefined"; 
	}
};


class InstallTask: 
	public Poco::Runnable, 
	public StatefulSignal<PackageInstallState>, 
	public Polymorphic
	///
	/// This class implements actual package 
	/// installation procedure.
{
public:
	struct Options 
		/// Package installation options.
	{
		std::string version;			// If set then the given package version will be installed.
		std::string sdkVersion;			// If set then the latest package version for given SDK
										// version will be installed.
		std::string installDir;			// Install to the given location, otherwise the manager default
										// installDir will be used.
		//bool copyManifestToInstallDir;  // Copys the local package.json file to the install directory.

		Options() {
			version = "";
			sdkVersion = "";
			installDir = "";
		}
	};

public:
	InstallTask(PackageManager& manager, 
				LocalPackage* local, RemotePackage* remote, 
				const Options& options = Options());
	virtual ~InstallTask();	

	virtual void start();
	virtual void cancel();

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

	virtual Package::Asset getRemoteAsset() const;

	virtual LocalPackage* local() const;
	virtual RemotePackage* remote() const;
	virtual Options& options();
	
	virtual bool valid() const;
	virtual bool cancelled() const;
	virtual bool failed() const;
	virtual bool success() const;
	virtual bool complete() const;
	virtual int progress() const;

	virtual void onStateChange(PackageInstallState& state, const PackageInstallState& oldState);
	virtual void onIncomingProgress(void* sender, const http::TransferProgress& state);
	virtual void onDecompressionError(const void*, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);
	virtual void onDecompressionOk(const void*, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info);

	//virtual void printLog(std::ostream& ost) const;	
	virtual const char* className() const { return "InstallTask"; }
	
	Signal<int&> Progress;
		/// Signals on progress update [0-100].
	
	NullSignal Complete;
		/// Signals on task completion for both
		/// success and failure cases.
	
protected:
	virtual void run();
		/// Called asynchronously by the thread to
		/// do the work.

	virtual void setProgress(int value);

protected:
	mutable Mutex	_mutex;
	
	Poco::Thread	_thread;
	PackageManager& _manager;
	LocalPackage*	_local;
	RemotePackage*	_remote;
	Options			_options;
	int             _progress;
	http::ClientConnection* _transaction;
	
	friend class PackageManager;
	friend class InstallMonitor;	
};


typedef std::vector<InstallTask*> InstallTaskList;


} } // namespace scy::pman


#endif // SOURCEY_Pacman_InstallTask_H