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


#ifndef SOURCEY_Pacman_Package_JSON_H
#define SOURCEY_Pacman_Package_JSON_H


#include "Sourcey/JSON/JSON.h"
#include "Poco/Path.h"


namespace Sourcey { 
namespace Pacman {
	

struct Package: public JSON::Value
	/// This class is a JSON representation of an
	/// package belonging to the PackageManager.
{	
	struct Asset
		/// This class represents a archived file
		/// asset containing files belonging to
		/// the parent package.
	{
		Asset(JSON::Value& src);
		virtual ~Asset();
		
		virtual std::string fileName() const;
		virtual std::string version() const;
		virtual std::string sdkVersion() const;
		virtual std::string url(int index = 0) const;
		virtual int fileSize() const;

		virtual bool valid() const;

		virtual void print(std::ostream& ost) const;
		
		virtual Asset& operator = (const Asset& r);
		virtual bool operator == (const Asset& r) const;
		
		JSON::Value& root;
	};

	Package();
	Package(const JSON::Value& src);
	virtual ~Package();

	virtual std::string id() const;
	virtual std::string name() const;
	virtual std::string type() const;
	virtual std::string author() const;
	virtual std::string description() const;

	virtual bool valid() const;

	virtual void print(std::ostream& ost) const;
};


// ---------------------------------------------------------------------
//
struct RemotePackage: public Package
	/// This class is a JSON representation of an
	/// package existing on the remote server that
	/// may be downloaded and installed.
{
	RemotePackage();
	RemotePackage(const JSON::Value& src);
	virtual ~RemotePackage();

	virtual JSON::Value& assets();	

	virtual Asset latestAsset();
		/// Returns the latest asset for this package.
		/// For local packages this is the currently installed version.
		/// For remote packages this is the latest available version.
		/// Throws an exception if no asset exists.

	virtual Asset lastestAssetForVersion(const std::string& version);
		/// Returns the latest asset for the specified package version.
		/// Throws an exception if no asset exists.
	
	virtual Asset latestAssetForSDK(const std::string& version);
		/// Returns the latest asset for the specified SDK version,
		/// or a blank asset.
		/// This method is for safely installing plug-ins which
		/// must be compiled against a specific SDK version.
		/// The package JSON must have a "sdk-version" member
		/// for this function to work as intended.
		/// Throws an exception if no asset exists.
};


// ---------------------------------------------------------------------
//
struct LocalPackage: public Package
	/// This class is a JSON representation of an
	/// installed local package that exists on the
	/// file system.
{	
	struct Manifest
		/// This class provides a list of all package
		/// files and their location on the file system.
	{
		Manifest(JSON::Value& src);
		virtual ~Manifest();
		
		virtual bool empty() const;
		
		virtual void addFile(const std::string& path);
		//virtual void addDir(const std::string& path);
		
		JSON::Value& root;
	};

	LocalPackage();
	LocalPackage(const JSON::Value& src);
	LocalPackage(const RemotePackage& src);
		/// Create the local package from the remote package
		/// reference with the following manipulations.
		//	1) Add a local manifest element.
		//	2) Remove asset mirror elements.

	virtual ~LocalPackage();
	
	virtual void setState(const std::string& state);
		/// Set's the package state. Possible values are:
		/// Installing, Installed, Failed, Uninstalled.
		/// If the packages completes while still Installing, 
		/// this means the package has yet to be finalized.

	virtual void setInstallState(const std::string& state);
		/// Set's the package installation state.
		/// See PackageInstallState for possible values.

	virtual void setInstallDir(const std::string& dir);
		/// Set's the installation directory for this package.
	
	virtual void setInstalledAsset(const Package::Asset& installedRemoteAsset);
		/// Sets the installed asset, once installed.
		/// This method also sets the version.

	virtual void setVersion(const std::string& state);
		/// Sets the current version of the local package.
		/// Installation must be complete.

	virtual std::string version() const;
		/// Returns the installed package version.
	
	virtual std::string state() const;
		/// Returns the current state of this package.	
		
	virtual std::string installState() const;
		/// Returns the installation state of this package.

	virtual std::string installDir() const;
		/// Returns the installation directory for this package.

	virtual Asset installedAsset();
		/// Returns the installed asset, if any.

	virtual bool isInstalled() const;
		/// Returns true or false depending on weather or
		/// not the package is installed successfully.
		/// False if package is in Failed state.

	virtual bool isFailed() const;

	virtual Manifest manifest();
		/// Returns the installation manifest.
	
	virtual bool isLatestVersion(const Package::Asset& lastestRemoteAsset) const;
		/// Checks if the given asset is a newer than the
		/// current version.	
	
	virtual std::string getInstalledFilePath(const std::string& fileName, bool whiny = false);
		/// Returns the full full path of the installed file.
		/// Thrown an exception if the install directory is unset.
	
	virtual JSON::Value& errors();
	virtual void addError(const std::string& message);
	virtual std::string lastError();
	virtual void clearErrors();

	virtual bool valid() const;
};


// ---------------------------------------------------------------------
//
struct PackagePair
	/// This class provides pairing of a local and a
	/// remote package.
{
	PackagePair(LocalPackage* local = NULL, RemotePackage* remote = NULL);
	
	virtual bool valid() const;

	std::string id() const;
	std::string name() const;
	std::string type() const;
	std::string author() const;
	
	LocalPackage* local;
	RemotePackage* remote;
};


typedef std::vector<PackagePair> PackagePairList;


} } // namespace Sourcey::Pacman


#endif // SOURCEY_Pacman_Package_JSON_H


	
	//virtual Asset getMatchingAsset(const Package::Asset& asset) const;
	//virtual void setVersion(const std::string& message);


	//virtual void setComplete(bool flag);
		/// This method is called when package installation
		/// completes in a successful or failed capacity. 
		/// The package version will be set from the asset,
		/// and the installed attribute will be updated.