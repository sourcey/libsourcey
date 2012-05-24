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
		virtual std::string url(int index = 0) const;

		virtual bool valid() const;

		virtual void print(std::ostream& ost) const;
		
		virtual Asset& operator = (const Asset& r);
		virtual bool operator == (const Asset& r) const;
		
		JSON::Value& root;
	};

	Package();
	Package(const JSON::Value& src);
	virtual ~Package();

	virtual int id() const;
	virtual std::string name() const;
	virtual std::string type() const;
	virtual std::string title() const;
	virtual std::string author() const;
	virtual std::string description() const;

	virtual JSON::Value& assets();	

	virtual Asset latestAsset();
		/// Returns the file asset with the greatest version.
		/// number belonging to this package.
		/// For local packages this is the currently
		/// installed version.
		/// For remote packages this is the latest 
		/// available version.

	virtual Asset assetVersion(const std::string& version);
		/// Returns an asset matching the specified version,
		/// or a blank asset.
	
	virtual Asset latestProjectAsset(const std::string& version);
		/// Returns the latest asset for the specified project 	
		/// version, or a blank asset.
		/// This method is for safely installing plug-ins which
		/// require a specific parent project version.
		/// The package JSON must have a "project-version"
		/// member for this function to work.

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

	virtual std::string version() const;
		/// Returns the installed package version.		
	
	virtual void setVersion(const std::string& state);
		/// Sets the current version of the local package.
		/// Installation must be complete.
	
	virtual std::string state() const;
		/// Returns the current state of the local package.	
	
	virtual void setState(const std::string& state);
		/// Set's the package state:
		/// Installing, Installed, Failed.
		/// An incomplete state means the package has yet
		/// to be finalized.
	
	virtual std::string installState() const;
		/// Returns the current installation state of the 
		/// local package.	

	virtual void setInstallState(const std::string& state);
		/// Set's the package installation state:
		/// See PackageInstallState for possible values.

	virtual bool isInstalled();
		/// Returns true or false depending on weather or
		/// not the package is installed successfully.
		/// False if package is in Failed state.

	virtual bool isFailed();

	virtual Manifest manifest();
		/// Returns the installation manifest.
	
	virtual bool isLatestVersion(const Package::Asset& asset) const;
		/// Checks if the given asset is a newer than the
		/// current version.

	virtual Asset copyAsset(const Package::Asset& asset);
	
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
	PackagePair(LocalPackage& local, RemotePackage& remote);
	
	virtual bool valid() const;

	LocalPackage& local;
	RemotePackage& remote;
};


} } // namespace Sourcey::Pacman


#endif // SOURCEY_Pacman_Package_JSON_H


	
	//virtual Asset getMatchingAsset(const Package::Asset& asset) const;
	//virtual void setVersion(const std::string& message);


	//virtual void setComplete(bool flag);
		/// This method is called when package installation
		/// completes in a successful or failed capacity. 
		/// The package version will be set from the asset,
		/// and the installed attribute will be updated.