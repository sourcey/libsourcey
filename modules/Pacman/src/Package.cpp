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
//
// Base Package
//
// ---------------------------------------------------------------------	
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
	return id() > 0
		&& !name().empty()
		&& !type().empty();
}


int Package::id() const
{
	return (*this)/*["package"]*/["id"].asInt();
}

string Package::name() const
{
	return (*this)/*["package"]*/["name"].asString();
}


string Package::type() const
{
	return (*this)/*["package"]*/["type"].asString();
}

string Package::title() const
{
	return (*this)/*["package"]*/["title"].asString();
}


JSON::Value& Package::assets()
{
	return (*this)["assets"];
}


Package::Asset Package::latestAsset()
{
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


void Package::print(std::ostream& ost) const
{
	JSON::StyledWriter writer;
	ost << writer.write(*this);
}


// ---------------------------------------------------------------------
//
// Package Asset
//
// ---------------------------------------------------------------------	
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


std::string Package::Asset::url() const
{
	return root["mirrors"][(size_t)0].asString();
}


bool Package::Asset::valid() const
{
	return !root["file-name"].empty() 
		&& !root["version"].empty();

}


void Package::Asset::print(std::ostream& ost) const
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
//
// Remote Package
//
// ---------------------------------------------------------------------	
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


// ---------------------------------------------------------------------
//
// Local Package
//
// ---------------------------------------------------------------------	
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
	removeMember("installed");
	assets().clear();
	assert(valid());
}


LocalPackage::~LocalPackage()
{
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
		state == "Failed"
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
		throw Exception("The package must be installed before a version can be set");
	
	(*this)["version"] = version;
}


bool LocalPackage::isInstalled()
{
	return state() == "Installed";
}


bool LocalPackage::isFailed()
{
	return state() == "Failed";
}


/*
void LocalPackage::setInstalled(bool flag)
{
	assert(valid());
	
	child("package").remove_(*this)["version");
	child("package").remove_(*this)["installed");
	
	child("package").append_(*this)["version").set_value(latestAsset().version().data());
	child("package").append_(*this)["installed").set_value(flag);
}
*/


string LocalPackage::state() const
{
	return get("state", "Incomplete").asString();
}


string LocalPackage::installState() const
{
	return get("install-state", "None").asString();
}


string LocalPackage::version() const
{
	return get("version", "0.0.0").asString();
}


bool LocalPackage::isLatestVersion(const Package::Asset& asset) const
{
	// If L is greater than R the function returns true.
	// If L is equal or less than R the function returns false.
	return !Util::compareVersion(asset.version(), version());
}


Package::Asset LocalPackage::copyAsset(const Package::Asset& asset)
{
	// Prepend a copy of the remote asset and return it
	return Asset(assets().append(asset.root));
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
//
// Local Package Manifest
//
// ---------------------------------------------------------------------	
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
//
// IPackage Pair
//
// ---------------------------------------------------------------------	
PackagePair::PackagePair(LocalPackage& local, RemotePackage& remote) :
	local(local), remote(remote)
{
}


bool PackagePair::valid() const
{
	return local.valid() 
		&& remote.valid();
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