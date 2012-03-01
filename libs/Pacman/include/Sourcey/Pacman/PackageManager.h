//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_Pacman_PackageManager_H
#define SOURCEY_Pacman_PackageManager_H


#include "Sourcey/Pacman/Config.h"
#include "Sourcey/Pacman/Types.h"
#include "Sourcey/Pacman/Package.h"
#include "Sourcey/Pacman/PackageInstallTask.h"
#include "Sourcey/Pacman/PackageInstallMonitor.h"
#include "Sourcey/BasicManager.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Runner.h"
#include "Sourcey/HTTP/Transaction.h"
#include "Sourcey/JSON/JSON.h"

#include "Poco/Zip/ZipLocalFileHeader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <assert.h>


namespace Sourcey { 
namespace Pacman {


typedef BasicManager<std::string, LocalPackage>		LocalPackageMap;
typedef LocalPackageMap::Map						LocalPackageMapT;
typedef BasicManager<std::string, RemotePackage>	RemotePackageMap;
typedef RemotePackageMap::Map						RemotePackageMapT;


class PackageManager
	/// The Package Manager provides an interface for managing, 
	/// installing, updating and uninstalling Pacman packages.
{
public:
	struct Options 
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

		std::string platform;

		// This flag tells the package manager weather or not
		// to clear the package cache if installation fails.
		bool clearFailedCache;

		Options() {
			endpoint				= DEFAULT_API_ENDPOINT;
			indexURI				= DEFAULT_API_INDEX_URI;

			cacheDir				= Poco::Path::current() + DEFAULT_PACKAGE_CACHE_DIR;
			interDir				= Poco::Path::current() + DEFAULT_PACKAGE_INTERMEDIATE_DIR;
			installDir				= Poco::Path::current() + DEFAULT_PACKAGE_INSTALL_DIR;

			platform				= DEFAULT_PLATFORM;
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
		/// Creates the package manager directory structure if it
		/// does not already exist.
	
	virtual void queryRemotePackages();
		/// Queries the server for a list of available packages.

	virtual void loadLocalPackages();
		/// Loads all local package manifests from file system.
		/// Clears all in memory package manifests. 

	virtual void loadLocalPackages(const std::string& dir);
		/// Loads all local package manifests residing the the
		/// given directory. This method may be called multiple
		/// times for different paths because it does not clear
		/// in memory package manifests. 
	
	virtual bool saveLocalPackages(bool whiny = false);
	virtual bool saveLocalPackage(LocalPackage& package, bool whiny = false);
		/// Saves the local package manifest to the file system.

	//
	/// Package Installation Methods
	//
	virtual bool installPackages(const StringList& names, PackageInstallMonitor* monitor = NULL, bool whiny = false);
		/// Installs multiple packages.

	virtual bool installPackage(const std::string& name, PackageInstallMonitor* monitor = NULL, bool whiny = false);
		/// Installs a single package.

	virtual bool updatePackages(const StringList& names, PackageInstallMonitor* monitor = NULL, bool whiny = false);
		/// Updates multiple packages.
		/// The package will be installed if it does not exist.

	virtual bool updatePackage(const std::string& name, PackageInstallMonitor* monitor = NULL, bool whiny = false);
		/// Updates a single package.
		/// The package will be installed if it does not exist.

	virtual bool updateAllPackages(PackageInstallMonitor* monitor = NULL, bool whiny = false);
		/// Updates all installed packages.

	virtual bool uninstallPackages(const StringList& names, bool whiny = false);
		/// Uninstalls multiple packages.

	virtual bool uninstallPackage(const std::string& name, bool whiny = false);
		/// Uninstalls a single package.

	virtual bool hasUnfinalizedPackages();
		/// Returns true if there are updates available that have
		/// not yet been finalized. Packages may be unfinalized if
		/// there were files in use at the time of installation.
			
	virtual bool finalizeInstallations(bool whiny = false);
		/// Finalizes active installations by moving all package
		/// files to their target destination. If files are to be
		/// overwritten they must not be in use of finalization
		/// will fail.	
	
	virtual void abortAllTasks();
		/// Aborts all package installation tasks. All tasks must
		/// be aborted before clearing local or remote manifests.
		
	//
	/// Package Helper Methods
	//	
	virtual PackagePair getPackagePair(const std::string& name) const;
		/// Returns a local and remote package pair.
		/// An exception will be thrown if either the local or
		/// remote packages aren't available or are invalid.	
	
	virtual PackagePair getOrCreatePackagePair(const std::string& name);
		/// Returns a local and remote package pair.
		/// If the local package doesn't exist it will be created
		/// from the remote package.
		/// If the remote package doesn't exist a NotFoundException 
		/// will be thrown.	
	
	virtual PackageInstallTask* createPackageInstallTask(PackagePair& pair);
		/// Creates a package installation task for the given pair.
	
	virtual bool isLatestVersion(PackagePair& pair);
		/// Checks if a newer version is available for the given
		/// package pair.
	
	virtual bool checkInstallManifest(LocalPackage& package);
		/// Checks the veracity of the install manifest for the given
		/// package and and ensures all package files exist on the
		/// file system.

	
	//
	/// File Helper Methods
	//
	virtual void clearCache();
		/// Clears all files in the cache directory.

	virtual bool clearPackageCache(LocalPackage& package);
		/// Clears a package archive from the local cache.

	virtual bool clearCacheFile(const std::string& fileName, bool whiny = false);
		/// Clears a file from the local cache.

	virtual bool hasCachedFile(const std::string& fileName);
		/// Checks if a package archive exists in the local cache.
	
	virtual bool isSupportedFileType(const std::string& fileName);
		/// Checks if the file type is a supported package archive.
	
	virtual Poco::Path getCacheFilePath(const std::string& fileName);
		/// Returns the full path of the cached file if it exists, or
		/// an empty path if the file doesn't exist.
	
	virtual Poco::Path getInstallFilePath(const std::string& fileName);
		/// Returns the full path of the installed file if it exists,
		/// or an empty path if the file doesn't exist.
	
	virtual Poco::Path getIntermediatePackageDir(const std::string& packageName);
		/// Returns the full path of the intermediate file if it exists,
		/// or an empty path if the file doesn't exist.

	/// 
	/// Accessors
	//	
	virtual Options& options() { return _options; }
	virtual RemotePackageMap& remotePackages() { return _remotePackages; }
	virtual LocalPackageMap& localPackages() { return _localPackages; }

protected:

	//
	/// Event Callbacks
	//
	virtual void onPackageInstallComplete(void* sender);
	
protected:
	Options				_options;
	LocalPackageMap		_localPackages;
	RemotePackageMap	_remotePackages;
	PackageInstallTaskList _tasks;
	
	mutable Poco::Mutex	_mutex;
};



} } // namespace Sourcey::Pacman


#endif // SOURCEY_Pacman_PackageManager_H