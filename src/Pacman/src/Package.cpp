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


#include "Sourcey/Pacman/Package.h"
#include "Sourcey/Util.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace Pacman {


// ---------------------------------------------------------------------
// Base Package
//	
Package::Package()
{
}


Package::Package(const JSON::Value& src) :
	JSON::Value(src)
{
}


Package::~Package()
{
}


bool Package::valid() const
{
	return !id().empty()
		&& !name().empty()
		&& !type().empty();
}


string Package::id() const
{
	return (*this)["id"].asString();
}


string Package::type() const
{
	return (*this)["type"].asString();
}


string Package::name() const
{
	return (*this)["name"].asString();
}


string Package::author() const
{
	return (*this)["author"].asString();
}


string Package::description() const
{
	return (*this)["description"].asString();
}


void Package::print(ostream& ost) const
{
	JSON::StyledWriter writer;
	ost << writer.write(*this);
}


// ---------------------------------------------------------------------
// Package Asset
//	
Package::Asset::Asset(JSON::Value& src) :
	root(src)
{
}


Package::Asset::~Asset()
{
}


string Package::Asset::fileName() const
{
	return root["file-name"].asString();
}


string Package::Asset::version() const
{
	return root.get("version", "0.0.0").asString();
}


string Package::Asset::sdkVersion() const
{
	return root.get("sdk-version", "0.0.0").asString();
}


string Package::Asset::url(int index) const
{
	return root["mirrors"][(size_t)index]["url"].asString();
}


int Package::Asset::fileSize() const
{
	return root.get("file-size", 0).asInt();
}


bool Package::Asset::valid() const
{
	return root.isMember("file-name")
		&& root.isMember("version")
		&& root.isMember("mirrors");
}


void Package::Asset::print(ostream& ost) const
{
	JSON::StyledWriter writer;
	ost << writer.write(root);
}


Package::Asset& Package::Asset::operator = (const Asset& r)
{
	root = r.root;
	return *this;
}


bool Package::Asset::operator == (const Asset& r) const
{
	return fileName() == r.fileName();
}


// ---------------------------------------------------------------------
// Remote Package
//	
RemotePackage::RemotePackage()
{
}


RemotePackage::RemotePackage(const JSON::Value& src) :
	Package(src)
{
}


RemotePackage::~RemotePackage()
{
}


JSON::Value& RemotePackage::assets()
{
	return (*this)["assets"];
}


Package::Asset RemotePackage::latestAsset()
{
	if (this->assets().empty())
		throw Exception("Package has no assets");

	// The latest asset may not be in order, so make
	// sure we return the latest one.
	JSON::Value& assets = this->assets();
	JSON::Value& asset = assets[(size_t)0];
	for (unsigned i = 0; i < assets.size(); i++) {
		if (Util::compareVersion(assets[i]["version"].asString(), asset["version"].asString())) {
			asset = assets[i];
		}
	}
	
	return Asset(asset);
}


Package::Asset RemotePackage::lastestAssetForVersion(const string& version)
{
	if (this->assets().empty())
		throw Exception("Package has no assets");

	JSON::Value& assets = this->assets();
	JSON::Value& asset = assets[(size_t)0];
	for (unsigned i = 0; i < assets.size(); i++) {
		if (assets[i]["version"].asString() == version) {
			asset = assets[i];
			break;
		}
	}

	if (asset["version"].asString() != version)
		throw Exception("No asset with version " + version);
	
	return Asset(asset);
}


Package::Asset RemotePackage::latestAssetForSDK(const string& version)
{
	if (this->assets().empty())
		throw Exception("Package has no assets");

	JSON::Value& assets = this->assets();
	JSON::Value& asset = assets[(size_t)0];
	for (unsigned i = 0; i < assets.size(); i++) {		
		if (assets[i]["sdk-version"].asString() == version && (
			asset["sdk-version"].asString() != version || 
			Util::compareVersion(assets[i]["version"].asString(), asset["version"].asString()))) {
			asset = assets[i];
		}
	}

	if (asset["sdk-version"].asString() != version)
		throw Exception("No asset with SDK version " + version);

	return Asset(asset);
}


// ---------------------------------------------------------------------
// Local Package
//	
LocalPackage::LocalPackage()
{
}


LocalPackage::LocalPackage(const JSON::Value& src) :
	Package(src)
{
}


LocalPackage::LocalPackage(const RemotePackage& src) :
	Package(src)
{
	assert(src.valid());	

	// Clear unwanted remote package fields
	//removeMember("state");
	//removeMember("install-state");
	//removeMember("version");
	removeMember("assets");
	assert(valid());
}


LocalPackage::~LocalPackage()
{
}


Package::Asset LocalPackage::installedAsset()
{
	return Package::Asset((*this)["asset"]);
}


LocalPackage::Manifest LocalPackage::manifest()
{
	return Manifest((*this)["manifest"]);
}


void LocalPackage::setState(const string& state)
{
	assert(
		state == "Installing" || 
		state == "Installed" ||
		state == "Failed" ||
		state == "Uninstalled"
	);

	(*this)["state"] = state;
}


void LocalPackage::setInstallState(const string& state)
{
	(*this)["install-state"] = state;
}


void LocalPackage::setVersion(const string& version)
{
	if (state() != "Installed")
		throw Exception("Package must be installed before the version is set.");
	
	(*this)["version"] = version;
}


bool LocalPackage::isInstalled() const
{
	return state() == "Installed";
}


bool LocalPackage::isFailed() const
{
	return state() == "Failed";
}


string LocalPackage::state() const
{
	return get("state", "Incomplete").asString();
}


string LocalPackage::installState() const
{
	return get("install-state", "None").asString();
}


string LocalPackage::installDir() const
{
	return get("install-dir", "").asString();
}


string LocalPackage::getInstalledFilePath(const string& fileName, bool whiny)
{
	string dir = installDir();
	if (whiny && dir.empty())
		throw Exception("Package install directory is not set.");
	Path path(dir);
	path.makeDirectory();
	path.setFileName(fileName);
	return path.toString();
}


string LocalPackage::version() const
{
	return get("version", "0.0.0").asString();
}


bool LocalPackage::isLatestVersion(const Package::Asset& lastestRemoteAsset) const
{
	// If L is greater than R the function returns true.
	// If L is equal or less than R the function returns false.
	return !Util::compareVersion(lastestRemoteAsset.version(), version());
}


void LocalPackage::setInstalledAsset(const Package::Asset& installedRemoteAsset)
{
	if (state() != "Installed")
		throw Exception("Package must be installed before asset can be set.");

	if (!installedRemoteAsset.valid())
		throw Exception("Remote asset is invalid.");

	(*this)["asset"] = installedRemoteAsset.root;
	setVersion(installedRemoteAsset.version());
}


void LocalPackage::setInstallDir(const string& dir)
{
	(*this)["install-dir"] = dir;
}


JSON::Value& LocalPackage::errors()
{	
	return (*this)["errors"];
}


void LocalPackage::addError(const string& message)
{
	errors().append(message);
}


string LocalPackage::lastError()
{
	return errors()[errors().size() - 1].asString();
}


void LocalPackage::clearErrors()
{
	errors().clear();
}
	

bool LocalPackage::valid() const
{
	return Package::valid(); 
}


// ---------------------------------------------------------------------
// Local Package Manifest
//	
LocalPackage::Manifest::Manifest(JSON::Value& src) :
	root(src)
{
}


LocalPackage::Manifest::~Manifest()
{
}

	
void LocalPackage::Manifest::addFile(const string& path)
{
	// Do not allow duplicates
	//if (!find_child_by_(*this)["file", "path", path.data()).empty())
	//	return;

	//JSON::Value node(path);
	root.append(path);
}
	

bool LocalPackage::Manifest::empty() const
{
	return root.empty();
}


// ---------------------------------------------------------------------
// Package Pair
//	
PackagePair::PackagePair(LocalPackage* local, RemotePackage* remote) :
	local(local), remote(remote)
{
}
	

string PackagePair::id() const
{
	return local ? local->id() : remote ? remote->id() : "";
}


string PackagePair::name() const
{
	return local ? local->id() : remote ? remote->name() : "";
}


string PackagePair::type() const
{
	return local ? local->type() : remote ? remote->type() : "";
}


string PackagePair::author() const
{
	return local ? local->author() : remote ? remote->author() : "";
}


bool PackagePair::valid() const
{
	// Packages must be valid, and
	// must have at least one package.
	return (!local || local->valid())
		&& (!remote || remote->valid()) 
		&& (local || remote);
}


} } // namespace Sourcey::Pacman





	//JSON::Value node = append_child();
	//node.set_name("file");
	//node.append_(*this)["path").set_value(path.data());


/*
Package::Asset LocalPackage::getMatchingAsset(const Package::Asset& asset) const
{
	assert(asset.valid());
	return Asset(select_single_node(
		format("//asset[@file-name='%s' and @version='%s']", 
			asset.fileName(), asset.version()).data()).node());
}
*/
/*
void LocalPackage::Manifest::addDir(const string& path)
{
	// Do not allow duplicates
	//if (!find_child_by_(*this)["dir", "path", path.data()).empty())
	//	return;

	//JSON::Value node = append_child();
	//node.set_name("dir");
	//node.append_(*this)["path").set_value(path.data());
	
	JSON::Value node(path);
	root.append(node);
}
*/