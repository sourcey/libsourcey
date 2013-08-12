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


#ifndef SOURCEY_Pacman_PackageManager_H
#define SOURCEY_Pacman_PackageManager_H


#include "Sourcey/Pacman/Config.h"
#include "Sourcey/Pacman/Types.h"
#include "Sourcey/Pacman/Package.h"
#include "Sourcey/Pacman/InstallTask.h"
#include "Sourcey/Pacman/InstallMonitor.h"
#include "Sourcey/Containers.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Runner.h"
#include "Sourcey/HTTP/Client.h"
#include "Sourcey/JSON/JSON.h"

#include "Poco/Zip/ZipLocalFileHeader.h"

#include <iostream>
#include <fstream>
#include <assert.h>


namespace scy { 
namespace pman {


typedef EventedManager<std::string, LocalPackage>	LocalPackageStore;
typedef LocalPackageStore::Map						LocalPackageMap;
typedef EventedManager<std::string, RemotePackage>	RemotePackageStore;
typedef RemotePackageStore::Map						RemotePackageMap;


class PackageManager
	/// The Package Manager provides an interface for managing, 
	/// installing, updating and uninstalling Pacman packages.
{
public:
	struct Options 
		// Package manager initialization options.
	{
		// Server Options
		std::string endpoint;
		std::string indexURI;
		std::string httpUsername;
		std::string httpPassword;
		
		// Directories & Paths
		std::string cacheDir;
		std::string interDir;
		std::string installDir;

		// Platform (win32, linux, mac)
		std::string platform;

		// This flag tells the package manager weather or not
		// to clear the package cache if installation fails.
		bool clearFailedCache;

		Options() {
			endpoint				= DEFAULT_API_ENDPOINT;
			indexURI				= DEFAULT_API_INDEX_URI;

			Poco::Path root(Poco::Path::current());
			root.makeDirectory();
			cacheDir				= root.toString() + DEFAULT_PACKAGE_CACHE_DIR;
			interDir				= root.toString() + DEFAULT_PACKAGE_INTERMEDIATE_DIR;
			installDir				= root.toString() + DEFAULT_PACKAGE_INSTALL_DIR;

			platform				= DEFAULT_PLATFORM; // TODO: Set for current system
			clearFailedCache		= true;
		}
	};

public:	
	PackageManager(const Options& options = Options());
	virtual ~PackageManager();
	
	//
	/// Initialization Methods
	//
	virtual void initialize();
	virtual void uninitialize();

	virtual bool initialized() const;

	virtual void createDirectories();
		// Creates the package manager directory structure
		// if it does not already exist.
	
	virtual void queryRemotePackages();
		// Queries the server for a list of available packages.

	virtual void loadLocalPackages();
		// Loads all local package manifests from file system.
		// Clears all in memory package manifests. 

	virtual void loadLocalPackages(const std::string& dir);
		// Loads all local package manifests residing the the
		// given directory. This method may be called multiple
		// times for different paths because it does not clear
		// in memory package manifests. 
	
	virtual bool saveLocalPackages(bool whiny = false);
	virtual bool saveLocalPackage(LocalPackage& package, bool whiny = false);
		// Saves the local package manifest to the file system.
	
	//
	/// Package Installation Methods
	//
	virtual InstallTask* installPackage(const std::string& name,
		const InstallTask::Options& options = InstallTask::Options(), bool whiny = false);
		// Installs a single package.
		// The returned InstallTask must be started.

	virtual bool installPackages(const StringVec& ids, 
		const InstallTask::Options& options = InstallTask::Options(),
		InstallMonitor* monitor = NULL, bool whiny = false);
		// Installs multiple packages.
		// The same options will be passed to each task.
		// The returned InstallTask(s) must be started.	
		// The returned InstallMonitor will be NULL no tasks were created.	
		// The returned InstallMonitor must be freed by the outside application.	

	virtual InstallTask* updatePackage(const std::string& name, 
		const InstallTask::Options& options = InstallTask::Options(), bool whiny = false);
		// Updates a single package.
		// Throws an exception if the package does exist.
		// The returned InstallTask must be started.

	virtual bool updatePackages(const StringVec& ids, 
		const InstallTask::Options& options = InstallTask::Options(), 
		InstallMonitor* monitor = NULL, bool whiny = false);
		// Updates multiple packages.
		// Throws an exception if the package does exist.
		// The returned InstallTask(s) must be started.	
		// The InstallMonitor must be freed by the outside application.	

	virtual bool updateAllPackages(bool whiny = false);
		// Updates all installed packages.

	virtual bool uninstallPackages(const StringVec& ids, bool whiny = false);
		// Uninstalls multiple packages.

	virtual bool uninstallPackage(const std::string& id, bool whiny = false);
		// Uninstalls a single package.

	virtual bool hasUnfinalizedPackages();
		// Returns true if there are updates available that have
		// not yet been finalized. Packages may be unfinalized if
		// there were files in use at the time of installation.
			
	virtual bool finalizeInstallations(bool whiny = false);
		// Finalizes active installations by moving all package
		// files to their target destination. If files are to be
		// overwritten they must not be in use of finalization
		// will fail.	
		
	//
	/// Task Helper Methods
	//	
	virtual InstallTask* getInstallTask(const std::string& id) const;
		// Gets the install task for the given package ID.

	virtual InstallTaskList tasks() const;
		// Returns a list of all tasks.
	
	virtual void cancelAllTasks();
		// Aborts all package installation tasks. All tasks must
		// be aborted before clearing local or remote manifests.

	//
	/// Package Helper Methods
	//	
	virtual PackagePairList getPackagePairs() const;
		// Returns all package pairs, valid or invalid.
		// Some pairs may not have both local and remote pointers.

	virtual PackagePair getPackagePair(const std::string& id, bool whiny = false) const;
		// Returns a local and remote package pair.
		// An exception will be thrown if either the local or
		// remote packages aren't available or are invalid.	
	
	virtual PackagePair getOrCreatePackagePair(const std::string& id);
		// Returns a local and remote package pair.
		// If the local package doesn't exist it will be created
		// from the remote package.
		// If the remote package doesn't exist a NotFoundException 
		// will be thrown.	
	
	virtual InstallTask* createInstallTask(PackagePair& pair, 
		const InstallTask::Options& options = InstallTask::Options());
		// Creates a package installation task for the given pair.
	
	virtual std::string installedPackageVersion(const std::string& id) const;
		// Returns the version number of an installed package.
		// Exceptions will be thrown if the package does not exist,
		// or is not fully installed.
		
	virtual Package::Asset getAssetToInstall(PackagePair& pair, const InstallTask::Options& options);
		// Returns the best asset to install, or throws a descriptive 
		// exception if no updates are available, or the package is 
		// is up to date.
		// This method takes version and SDK locks into consideration.
		
	//
	/// File Helper Methods
	//
	void clearCache();
		// Clears all files in the cache directory.

	bool clearPackageCache(LocalPackage& package);
		// Clears a package archive from the local cache.

	bool clearCacheFile(const std::string& fileName, bool whiny = false);
		// Clears a file from the local cache.

	bool hasCachedFile(Package::Asset& asset);
		// Checks if a package archive exists in the local cache.
	
	bool isSupportedFileType(const std::string& fileName);
		// Checks if the file type is a supported package archive.
	
	Poco::Path getCacheFilePath(const std::string& fileName);
		// Returns the full path of the cached file if it exists,
		// or an empty path if the file doesn't exist.
	Poco::Path getIntermediatePackageDir(const std::string& id);
		// Returns the the intermediate package directory for the
		// given package ID.
	
	// 
	/// Accessors
	//
	virtual Options& options();
	virtual RemotePackageStore& remotePackages();
	virtual LocalPackageStore& localPackages();
		
	//
	/// Events
	//
	Signal<LocalPackage&> PackageComplete;
	Signal<LocalPackage&> PackageUninstalled;
	
	Signal<InstallTask&> TaskAdded;
	Signal<InstallTask&> TaskRemoved;

protected:

	//
	/// Callbacks
	//
	void onPackageInstallComplete(void* sender);

	void onPackagesResponse(void* sender, const http::Response& response);
	
protected:	
	mutable Mutex _mutex;
	Options				_options;
	LocalPackageStore	_localPackages;
	RemotePackageStore	_remotePackages;
	InstallTaskList		_tasks;
};



} } // namespace scy::pman


#endif // SOURCEY_Pacman_PackageManager_H



	/*	
	//Signal<const std::string&, InstallTask&> TaskState;
		// TaskState notifies the PackageState of each running task.
		// Installing is used when a bew task is created.

	//virtual bool isUpToDate(PackagePair& pair);
		// Checks if a newer version is available for the given
		// package pair.
	
	//virtual bool verifyInstallManifest(LocalPackage& package);
		// Checks the veracity of the install manifest for the given
		// package and and ensures all package files exist on the
		// file system.

	//virtual Poco::Path getInstalledFilePath(const std::string& fileName);
		// Returns the full path of the installed file if it exists,
		// or an empty path if the file doesn't exist.
	
	virtual bool installPackage(const std::string& name, InstallMonitor* monitor = NULL, 
		const InstallTask::Options& options = InstallTask::Options(), bool whiny = false);
		// Installs a single package.

	virtual bool updatePackages(const StringVec& ids, InstallMonitor* monitor = NULL, 
		const InstallTask::Options& options = InstallTask::Options(), bool whiny = false);
		// Updates multiple packages.
		// The package will be installed if it does not exist.

	virtual bool updatePackage(const std::string& name, InstallMonitor* monitor = NULL, 
		const InstallTask::Options& options = InstallTask::Options(), bool whiny = false);
		// Updates a single package.
		// The package will be installed if it does not exist.
		*/