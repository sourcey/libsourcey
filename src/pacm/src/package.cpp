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


#include "scy/pacm/package.h"
#include "scy/util.h"
#include "scy/logger.h"
#include "scy/filesystem.h"

#include "assert.h"


namespace scy { 
namespace pacm {


//
// Base Package
//	


Package::Package()
{
}


Package::Package(const json::Value& src) :
	json::Value(src)
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


std::string Package::id() const
{
	return (*this)["id"].asString();
}


std::string Package::type() const
{
	return (*this)["type"].asString();
}


std::string Package::name() const
{
	return (*this)["name"].asString();
}


std::string Package::author() const
{
	return (*this)["author"].asString();
}


std::string Package::description() const
{
	return (*this)["description"].asString();
}


void Package::print(std::ostream& ost) const
{
	json::StyledWriter writer;
	ost << writer.write(*this);
}


//
// Package Asset
//	


Package::Asset::Asset(json::Value& src) :
	root(src)
{
}


Package::Asset::~Asset()
{
}


std::string Package::Asset::fileName() const
{
	return root["file-name"].asString();
}


std::string Package::Asset::version() const
{
	return root.get("version", "0.0.0").asString();
}


std::string Package::Asset::sdkVersion() const
{
	return root.get("sdk-version", "0.0.0").asString();
}


std::string Package::Asset::checksum() const
{
	return root.get("checksum", "").asString();
}


std::string Package::Asset::url(int index) const
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


void Package::Asset::print(std::ostream& ost) const
{
	json::StyledWriter writer;
	ost << writer.write(root);
}


Package::Asset& Package::Asset::operator = (const Asset& r)
{
	root = r.root;
	return *this;
}


bool Package::Asset::operator == (const Asset& r) const
{
	return fileName() == r.fileName() 
		&& version() == r.version() 
		&& checksum() == r.checksum();
}


//
// Remote Package
//	


RemotePackage::RemotePackage()
{
}


RemotePackage::RemotePackage(const json::Value& src) :
	Package(src)
{
}


RemotePackage::~RemotePackage()
{
}


json::Value& RemotePackage::assets()
{
	return (*this)["assets"];
}


Package::Asset RemotePackage::latestAsset()
{
	json::Value& assets = this->assets();
	if (assets.empty())
		throw std::runtime_error("Package has no assets");

	// The latest asset may not be in order, so
	// make sure we always return the latest one.
	size_t index = 0;
	if (assets.size() > 1) {
		for (unsigned i = 1; i < assets.size(); i++) {
			if (util::compareVersion(assets[i]["version"].asString(), assets[index]["version"].asString())) {
				index = i;
			}
		}
	}
	
	return Asset(assets[index]);
}


Package::Asset RemotePackage::assetVersion(const std::string& version)
{
	json::Value& assets = this->assets();
	if (assets.empty())
		throw std::runtime_error("Package has no assets");

	size_t index = -1;
	for (unsigned i = 0; i < assets.size(); i++) {
		if (assets[i]["version"].asString() == version) {
			index = i;
			break;
		}
	}

	if (index == -1)
		throw std::runtime_error("No package asset with version " + version);
	
	return Asset(assets[index]);
}


Package::Asset RemotePackage::latestSDKAsset(const std::string& version)
{
	json::Value& assets = this->assets();
	if (assets.empty())
		throw std::runtime_error("Package has no assets");

	size_t index = -1;
	for (unsigned i = 0; i < assets.size(); i++) {		
		if (assets[i]["sdk-version"].asString() == version && (index == -1 || (
			assets[index]["sdk-version"].asString() != version || 
			util::compareVersion(assets[i]["version"].asString(), assets[index]["version"].asString())))) {
			index = i;
		}
	}
	
	if (index == -1)
		throw std::runtime_error("No package asset with SDK version " + version);
	
	return Asset(assets[index]);
}


//
// Local Package
//	


LocalPackage::LocalPackage()
{
}


LocalPackage::LocalPackage(const json::Value& src) :
	Package(src)
{
}


LocalPackage::LocalPackage(const RemotePackage& src) :
	Package(src)
{
	assert(src.valid());	

	// Clear unwanted remote package fields
	removeMember("assets");
	assert(valid());
}


LocalPackage::~LocalPackage()
{
}


Package::Asset LocalPackage::asset()
{
	return Package::Asset((*this)["asset"]);
}


LocalPackage::Manifest LocalPackage::manifest()
{
	return Manifest((*this)["manifest"]);
}


void LocalPackage::setState(const std::string& state)
{
	assert(
		state == "Installing" || 
		state == "Installed" ||
		state == "Failed" ||
		state == "Uninstalled"
	);

	(*this)["state"] = state;
}


void LocalPackage::setInstallState(const std::string& state)
{
	(*this)["install-state"] = state;
}


void LocalPackage::setVersion(const std::string& version)
{
	if (state() != "Installed")
		throw std::runtime_error("Package must be installed before the version is set.");
	
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


std::string LocalPackage::state() const
{
	return get("state", "Installing").asString();
}


std::string LocalPackage::installState() const
{
	return get("install-state", "None").asString();
}


std::string LocalPackage::installDir() const
{
	return get("install-dir", "").asString();
}


std::string LocalPackage::getInstalledFilePath(const std::string& fileName, bool whiny)
{
	std::string dir = installDir();
	if (whiny && dir.empty())
		throw std::runtime_error("Package install directory is not set.");
	
	// TODO: What about sub directories?
	fs::addnode(dir, fileName);
	return dir;
}


void LocalPackage::setVersionLock(const std::string& version)
{
	if (version.empty())
		(*this).removeMember("version-lock");
	else
		(*this)["version-lock"] = version;
}


void LocalPackage::setSDKVersionLock(const std::string& version)
{
	if (version.empty())
		(*this).removeMember("sdk-version-lock");
	else
		(*this)["sdk-version-lock"] = version;
}


std::string LocalPackage::version() const
{
	return get("version", "0.0.0").asString();
}


std::string LocalPackage::versionLock() const
{
	return get("version-lock", "").asString();
}


std::string LocalPackage::sdkLockedVersion() const
{
	return get("sdk-version-lock", "").asString();
}


bool LocalPackage::verifyInstallManifest(bool allowEmpty)
{	
	DebugLS(this) << name() << ": Verifying install manifest" << std::endl;

	// Check file system for each manifest file
	LocalPackage::Manifest manifest = this->manifest();
	for (auto it = manifest.root.begin(); it != manifest.root.end(); it++) {		
		std::string path = this->getInstalledFilePath((*it).asString(), false);
		DebugLS(this) << name() << ": Checking exists: " << path << std::endl;
		
		if (!fs::exists(fs::normalize(path))) {
			ErrorLS(this) << name() << ": Missing file: " << path << std::endl;
			return false;
		}
	}
	
	return allowEmpty ? true : !manifest.empty();
}


void LocalPackage::setInstalledAsset(const Package::Asset& installedRemoteAsset)
{
	if (state() != "Installed")
		throw std::runtime_error("Package must be installed before asset can be set.");

	if (!installedRemoteAsset.valid())
		throw std::runtime_error("Remote asset is invalid.");

	(*this)["asset"] = installedRemoteAsset.root;
	setVersion(installedRemoteAsset.version());
}


void LocalPackage::setInstallDir(const std::string& dir)
{
	(*this)["install-dir"] = dir;
}


json::Value& LocalPackage::errors()
{	
	return (*this)["errors"];
}


void LocalPackage::addError(const std::string& message)
{
	errors().append(message);
}


std::string LocalPackage::lastError() const
{	
	json::Value errors = get("errors", Json::arrayValue);
	return errors.empty() ? "" : errors[errors.size() - 1].asString();
}


void LocalPackage::clearErrors()
{
	errors().clear();
}
	

bool LocalPackage::valid() const
{
	return Package::valid(); 
}


//
// Local Package Manifest
//	


LocalPackage::Manifest::Manifest(json::Value& src) :
	root(src)
{
}


LocalPackage::Manifest::~Manifest()
{
}

	
void LocalPackage::Manifest::addFile(const std::string& path)
{
	// Do not allow duplicates
	//if (!find_child_by_(*this)["file", "path", path.c_str()).empty())
	//	return;

	//json::Value node(path);
	root.append(path);
}
	

bool LocalPackage::Manifest::empty() const
{
	return root.empty();
}


//
// Package Pair
//	


PackagePair::PackagePair(LocalPackage* local, RemotePackage* remote) :
	local(local), remote(remote)
{
}
	

std::string PackagePair::id() const
{
	return local ? local->id() : remote ? remote->id() : "";
}


std::string PackagePair::name() const
{
	return local ? local->name() : remote ? remote->name() : "";
}


std::string PackagePair::type() const
{
	return local ? local->type() : remote ? remote->type() : "";
}


std::string PackagePair::author() const
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


} } // namespace scy::pacm