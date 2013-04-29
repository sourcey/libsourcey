#include "Sourcey/Pacman/IPackage.h"

#include "Poco/format.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace Pacman {


// ----------------------------------------------------------------------------
//
// Base IPackage
//-------	
IPackage::IPackage()
{
}


IPackage::~IPackage()
{
}


bool IPackage::valid() const
{
	return name().empty();
}



/*
IPackage::IAsset IPackage::latestAsset() const 
{
	// The latest asset may not be in order, so make
	// sure we return the latest one.
	pugi::xml_node asset = child("package").child("asset");
	for (pugi::xml_node node = asset.next_sibling("asset"); 
		node; node = node.next_sibling("asset")) {
		if (Util::compareVersion(node.attribute("version").value(), asset.attribute("version").value()))
			asset = node;
	}

	return IAsset(asset);
}
*/


// ----------------------------------------------------------------------------
//
// IPackage IAsset
//-------	
IPackage::IAsset::IAsset()
{
}


IPackage::IAsset::~IAsset()
{
}


string IPackage::IAsset::fileName() const
{
	assert(false);
}


string IPackage::IAsset::version() const
{
	assert(false);
}


std::string IPackage::IAsset::url() const
{
	assert(false);
}


bool IPackage::IAsset::valid() const
{
	return !fileName().empty() 
		&& !version().empty();

}


bool IPackage::IAsset::operator == (const IAsset& rhs) const
{
	return fileName() == rhs.fileName();
}


// ----------------------------------------------------------------------------
//
// Remote IPackage
//-------	
RemoteIPackage::RemoteIPackage()
{
}


RemoteIPackage::~RemoteIPackage()
{
}


// ----------------------------------------------------------------------------
//
// Local IPackage
//-------	
LocalIPackage::LocalIPackage()
{
}


/*
LocalIPackage::LocalIPackage(const RemoteIPackage& src) :
	IPackage(src)
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
*/


LocalIPackage::~LocalIPackage()
{
}


bool LocalIPackage::isInstalled()
{
	return state() == "Installed";
}


bool LocalIPackage::isLatestVersion(const IPackage::IAsset& asset) const
{
	return Util::compareVersion(version(), asset.version());
}


bool LocalIPackage::valid() const
{
	return id() > 0
		&& !type().empty() 		
		&& !name().empty();
		//&& !child("package").child("asset").empty()
		//&& child("package").child("asset").child("mirror").empty();

}


/*
LocalIPackage::Manifest LocalIPackage::manifest() const
{
	return Manifest(child("package").child("manifest"));
}


void LocalIPackage::setState(const string& state)
{
	child("package").remove_attribute("state");
	child("package").append_attribute("state").set_value(state.data());
}


void LocalIPackage::setInstallState(const string& state)
{
	child("package").remove_attribute("install-state");
	child("package").append_attribute("install-state").set_value(state.data());
}


void LocalIPackage::setVersion(const string& version)
{
	if (state() != "Installed")
		throw Exception("The package must be installed before the version can be updated.");

	child("package").remove_attribute("version");
	child("package").append_attribute("version").set_value(version.data());
}
*/


/*
void LocalIPackage::setInstalled(bool flag)
{
	assert(valid());
	
	child("package").remove_attribute("version");
	child("package").remove_attribute("installed");
	
	child("package").append_attribute("version").set_value(latestAsset().version().data());
	child("package").append_attribute("installed").set_value(flag);
}
*/


/*
IPackage::IAsset LocalIPackage::getMatchingIAsset(const IPackage::IAsset& asset) const
{
	assert(asset.valid());
	return IAsset(select_single_node(
		format("//asset[@file-name='%s' and @version='%s']", 
			asset.fileName(), asset.version()).data()).node());
}


IPackage::IAsset LocalIPackage::copyAsset(const IPackage::IAsset& asset)
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
	return IAsset(node);
}


void LocalIPackage::addError(const string& message)
{
	pugi::xml_node node = child("package").prepend_child();
	node.set_name("error");
	node.append_child(pugi::node_pcdata).set_value(message.data());
}


string LocalIPackage::lastError()
{
	return child("package").child_value("error");
}
*/
	


// ----------------------------------------------------------------------------
//
// Local IPackage Manifest
//-------	
LocalIPackage::Manifest::Manifest()
{
}


LocalIPackage::Manifest::~Manifest()
{
}

	
void LocalIPackage::Manifest::addFile(const string&)
{
	assert(false);
}



void LocalIPackage::Manifest::addDir(const string&)
{
	assert(false);
}


// ----------------------------------------------------------------------------
//
// IPackage Pair
//-------	
PackagePair::PackagePair(LocalIPackage& local, RemoteIPackage& remote) :
	local(local), remote(remote)
{
}


bool PackagePair::valid() const
{
	return local.valid() 
		&& remote.valid();
}


} } // namespace Sourcey::Pacman
