#include "Sourcey/Pacman/Config.h"


#ifdef PACKAGE_MANAGER_USE_XML


#include "Sourcey/Pacman/Package_XML.h"
#include "Poco/format.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace Pacman {


// ----------------------------------------------------------------------------
//
// Base Package
//-------	
Package::Package()
{
}


Package::Package(const XML::Document& src) :
	XML::Document(src)
{
}


Package::~Package()
{
}


IPacket* Package::clone() const 
{
	throw Exception("Packages must not be cloned.");
}


bool Package::valid() const
{
	return !empty() 
		&& !name().empty();
}


int Package::id() const
{
	return child("package").attribute("id").as_int();
}

string Package::name() const
{
	return child("package").attribute("name").value();
}


string Package::type() const
{
	return child("package").attribute("type").value();
}

string Package::title() const
{
	return child("package").attribute("title").value();
}


IPackage::IAsset Package::latestAsset() const 
{
	// The latest asset may not be in order, so make
	// sure we return the latest one.
	pugi::xml_node asset = child("package").child("asset");
	for (pugi::xml_node node = asset.next_sibling("asset"); 
		node; node = node.next_sibling("asset")) {
		if (Util::compareVersion(node.attribute("version").value(), asset.attribute("version").value()))
			asset = node;
	}

	return Asset(asset);
}


// ----------------------------------------------------------------------------
//
// Package Asset
//-------	
Package::Asset::Asset()
{
}


Package::Asset::~Asset()
{
}


Package::Asset::Asset(const pugi::xml_node& src) :
	XML::Node(src)
{
}


string Package::Asset::fileName() const
{
	return attribute("file-name").value();
}


string Package::Asset::version() const
{
	string version = attribute("version").value();
	return version.empty() ? "0.0.0" : version;
}


/*
bool Package::Asset::valid() const
{
	return !attribute("file-name").empty() 
		&& !attribute("version").empty();

}


bool Package::Asset::operator == (const Asset& rhs) const
{
	return strcmp(
		attribute("file-name").value(), 
		rhs.attribute("file-name").value()) == 0;
}
*/


// ----------------------------------------------------------------------------
//
// Remote Package
//-------	
RemotePackage::RemotePackage()
{
}


RemotePackage::RemotePackage(const XML::Document& src) :
	XML::Document(src)
{
}


RemotePackage::~RemotePackage()
{
}


// ----------------------------------------------------------------------------
//
// Local Package
//-------	
LocalPackage::LocalPackage()
{
}


LocalPackage::LocalPackage(const XML::Document& src) :
	XML::Document(src)
{
}


LocalPackage::LocalPackage(const RemotePackage& src) :
	XML::Document(src)
{
	assert(src.valid());
	
	child("package").remove_attribute("installed");
	child("package").append_child("manifest");

	pugi::xml_node asset = child("package").child("asset");
	for (pugi::xml_node node = asset.first_child(); 
		node; node = node.next_sibling()) {
		asset.remove_child(node);
	}

	assert(valid());
}


LocalPackage::~LocalPackage()
{
}


LocalPackage::Manifest LocalPackage::manifest()
{
	return Manifest(child("package").child("manifest"));
}


void LocalPackage::setState(const string& state)
{
	child("package").remove_attribute("state");
	child("package").append_attribute("state").set_value(state.data());
}


void LocalPackage::setInstallState(const string& state)
{
	child("package").remove_attribute("install-state");
	child("package").append_attribute("install-state").set_value(state.data());
}


void LocalPackage::setVersion(const string& version)
{
	if (state() != "Installed")
		throw Exception("The package must be installed before the version can be updated.");

	child("package").remove_attribute("version");
	child("package").append_attribute("version").set_value(version.data());
}


bool LocalPackage::isInstalled()
{
	return state() == "Installed";
}


/*
void LocalPackage::setInstalled(bool flag)
{
	assert(valid());
	
	child("package").remove_attribute("version");
	child("package").remove_attribute("installed");
	
	child("package").append_attribute("version").set_value(latestAsset().version().data());
	child("package").append_attribute("installed").set_value(flag);
}
*/


string LocalPackage::state() const
{
	string state = child("package").attribute("state").value();
	return state.empty() ? "Not Installed" : state;
}


string LocalPackage::installState() const
{
	string state = child("package").attribute("install-state").value();
	return state.empty() ? "None" : state;
}


string LocalPackage::version() const
{
	string version = child("package").attribute("version").value();
	return version.empty() ? "0.0.0" : version;
}


/*
bool LocalPackage::isLatestVersion(const Package::Asset& asset) const
{
	return Util::compareVersion(version(), asset.version());
}


Package::Asset LocalPackage::getMatchingAsset(const Package::Asset& asset) const
{
	assert(asset.valid());
	return Asset(select_single_node(
		format("//asset[@file-name='%s' and @version='%s']", 
			asset.fileName(), asset.version()).data()).node());
}
*/


IPackage::IAsset LocalPackage::copyAsset(const Package::Asset& asset)
{
	// Remove matching assets (in case there is any new data) 
	pugi::xml_node old = select_single_node(
		format("//asset[@file-name='%s' and @version='%s']", 
			asset.fileName(), asset.version()).data()).node();
	if (!old.empty())
		remove_child(old);

	// Prepend a copy of the remote asset and return it
	pugi::xml_node node = child("package").prepend_copy(asset);	
	node.print(cout);
	assert(strcmp(node.name(), "asset") == 0);
	node.remove_child("mirror");
	return Package::Asset(node);
}


void LocalPackage::addError(const string& message)
{
	pugi::xml_node node = child("package").prepend_child();
	node.set_name("error");
	node.append_child(pugi::node_pcdata).set_value(message.data());
}


string LocalPackage::lastError()
{
	return child("package").child_value("error");
}
	

bool LocalPackage::valid() const
{
	return !child("package").attribute("type").empty() 
		&& !child("package").attribute("id").empty()
		&& !child("package").attribute("name").empty()
		&& !child("package").child("asset").empty()
		&& child("package").child("asset").child("mirror").empty();

}


// ----------------------------------------------------------------------------
//
// Local Package Manifest
//-------	
LocalPackage::Manifest::Manifest()
{
}


LocalPackage::Manifest::~Manifest()
{
}


LocalPackage::Manifest::Manifest(const pugi::xml_node& src) :
	XML::Node(src)
{
}

	
void LocalPackage::Manifest::addFile(const string& path)
{
	// Do not allow duplicates
	if (!find_child_by_attribute("file", "path", path.data()).empty())
		return;

	pugi::xml_node node = append_child();
	node.set_name("file");
	node.append_attribute("path").set_value(path.data());
}


void LocalPackage::Manifest::addDir(const string& path)
{
	// Do not allow duplicates
	if (!find_child_by_attribute("dir", "path", path.data()).empty())
		return;

	pugi::xml_node node = append_child();
	node.set_name("dir");
	node.append_attribute("path").set_value(path.data());
}


/*
// ----------------------------------------------------------------------------
//
// Package Pair
//-------	
PackagePair::PackagePair(LocalPackage& local, RemotePackage& remote) :
	local(local), remote(remote)
{
}


bool PackagePair::valid() const
{
	return local.valid() 
		&& remote.valid();
}
*/


} } // namespace Sourcey::Pacman


#endif // PACKAGE_MANAGER_USE_JSON