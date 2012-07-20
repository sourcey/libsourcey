#ifndef PACKAGE_MANAGER_Package_XML_H
#define PACKAGE_MANAGER_Package_XML_H


#include "Sourcey/Pacman/Types.h"
#include "Sourcey/Pacman/IPackage.h"
#include "Sourcey/XML/XML.h"


namespace Sourcey { 
namespace Pacman {
	
	/*
// ---------------------------------------------------------------------
//
inline bool parseRemotePackageRequest(RemotePackageMap& packages, const std::string& data) 
	/// Returns a comma delimited package name string.
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load(data.data());
	if (!result)
		throw Exception(result.description());	
	
	// Create a package instance for each package node
	for (pugi::xml_node node = doc.first_child().child("package"); 
		node; node = node.next_sibling("package")) {				
		std::stringstream ss;
		node.print(ss);
		RemotePackage* package = new RemotePackage();
		pugi::xml_parse_result result = package->load(ss);
		if (!result) {
			delete package;
			throw Exception(result.description()); 
		}
		packages.add(package->name(), package);
	}
}
	*/


// ---------------------------------------------------------------------
//
struct Package: public IPackage, public XML::Document
	/// This class provides an XML representation of a package
	/// which is available to the Package Manager.
{	
	struct Asset: public IAsset, public XML::Node
		/// This class provides an XML representation of a file
		/// asset, generally an archive, that contains the package
		/// files.
	{
		Asset();
		Asset(const pugi::xml_node& src);
		virtual ~Asset();
		
		virtual std::string fileName() const;
		virtual std::string version() const;

		/*
		virtual bool valid() const;

		virtual bool operator == (const Asset& rhs) const;
		*/
	};

	Package();
	Package(const XML::Document& src);
	virtual ~Package();

	virtual IPacket* clone() const;

	virtual int id() const;
	virtual std::string name() const;
	virtual std::string type() const;
	virtual std::string title() const;

	virtual bool valid() const;
	
	virtual IAsset latestAsset() const;
		// The latest update description set from the
		// web service response.
};


// ---------------------------------------------------------------------
//
struct RemotePackage: public RemoteIPackage, public XML::Document
	/// This class provides an XML representation of a package
	/// that exists on the remote server.
{
	RemotePackage();
	RemotePackage(const XML::Document& src);
	virtual ~RemotePackage();
};


// ---------------------------------------------------------------------
//
struct LocalPackage: public LocalIPackage, public XML::Document
	/// This class provides an XML representation of a package
	/// that exists on the local filesystem.
{	
	struct Manifest: public XML::Node
		/// This class provides a list of all files installed
		/// by the parent package on the local filesystem.
	{
		Manifest();
		Manifest(const pugi::xml_node& src);
		virtual ~Manifest();
		
		virtual void addFile(const std::string& path);
		virtual void addDir(const std::string& path);
	};

	LocalPackage();
	LocalPackage(const XML::Document& src);
	LocalPackage(const RemotePackage& src);
		// Create the local package from the remote package
		// reference with the following manipulations.
		//	1) Add a local manifest element.
		//	2) Remove asset mirror elements.

	virtual ~LocalPackage();

	virtual std::string version() const;
		// Returns the current version of the local package.		
	
	virtual void setVersion(const std::string& state);
		// Sets the current version of the local package
		// once installation is complete.
	
	virtual std::string state() const;
		// Returns the current state of the local package.	
	
	virtual void setState(const std::string& state);
		// Set's the package state:
		//	Not Installed, Installed, Error.	
	
	virtual std::string installState() const;
		// Returns the current installation state of the 
		// local package.	

	virtual void setInstallState(const std::string& state);
		// Set's the package installation state:
		//	@see PackageInstallState.

	virtual bool isInstalled();
		// Returns weather the package is installed or not.

	//virtual void setInstalled(bool flag);
		// This method is called when package installation
		// completes in a successfull or failed capacity. 
		// The package version will be set from the asset,
		// and the installed attribute will be updated.
	
	virtual Manifest manifest(); // const;
		// Returns the installation file manifest.
	
	//virtual bool isLatestVersion(const Package::Asset& asset) const;
		// Checks if the provided asset is a newer than the
		// currently installed version.

	virtual IAsset copyAsset(const Package::Asset& asset);
	
	//virtual Asset getMatchingAsset(const Package::Asset& asset) const;
	//virtual void setVersion(const std::string& message);
	
	virtual void addError(const std::string& message);
	virtual std::string lastError();

	virtual bool valid() const;
};

	
inline bool parseRemotePackageRequest(RemotePackageMap& packages, const std::string& data) 
	/// Returns a comma delimited package name string.
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load(data.data());
	if (!result)
		throw Exception(result.description());	
	
	// Create a package instance for each package node
	for (pugi::xml_node node = doc.first_child().child("package"); 
		node; node = node.next_sibling("package")) {				
		std::stringstream ss;
		node.print(ss);
		RemotePackage* package = new RemotePackage();
		pugi::xml_parse_result result = package->load(ss);
		if (!result) {
			delete package;
			throw Exception(result.description()); 
		}
		packages.add(package->name(), package);
	}
}


} } // namespace Sourcey::Pacman


#endif // PACKAGE_MANAGER_Package_XML_H