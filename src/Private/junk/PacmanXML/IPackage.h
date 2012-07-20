#ifndef PACKAGE_MANAGER_IIPackage_XML_H
#define PACKAGE_MANAGER_IIPackage_XML_H


//#include "Sourcey/XML/XML.h"
#include "Sourcey/Base/BasicManager.h"


namespace Sourcey { 
namespace Pacman {


// ----------------------------------------------------------------------------
//
struct IPackage
	/// This class provides an XML representation of a package
	/// which is available to the IPackage Manager.
{		
	struct IAsset
		/// This class provides an XML representation of a file
		/// asset, generally an archive, that contains the package
		/// files.
	{
		IAsset();
		//IAsset(const pugi::xml_node& src) = 0;
		virtual ~IAsset();
		
		virtual std::string fileName() const;
		virtual std::string version() const;
		virtual std::string url() const;

		virtual bool valid() const;

		virtual bool operator == (const IAsset& rhs) const;
	};

	//IPackage() = 0;
	//IPackage(const XML::Document& src) = 0;
	IPackage(); // {};
	virtual ~IPackage(); // {};

	//virtual IPacket* clone() const = 0;

	virtual int id() const = 0;
	virtual std::string name() const = 0;
	virtual std::string type() const = 0;
	virtual std::string title() const = 0;

	virtual bool valid() const; // = 0;
	
	virtual IAsset latestAsset() const = 0;
		// The latest update description set from the
		// web service response.

	//virtual std::string latestAssetVersion() const = 0;
		// Returns the current version of the local package.	
};


// ----------------------------------------------------------------------------
//
struct RemoteIPackage: public IPackage
	/// This class provides an XML representation of a package
	/// that exists on the remote server.
{
	RemoteIPackage();
	//RemoteIPackage(const XML::Document& src) = 0;
	virtual ~RemoteIPackage();
};


// ----------------------------------------------------------------------------
//
struct LocalIPackage: public IPackage
	/// This class provides an XML representation of a package
	/// that exists on the local filesystem.
{	
	struct Manifest
		/// This class provides a list of all files installed
		/// by the parent package on the local filesystem.
	{
		Manifest();
		//Manifest(const pugi::xml_node& src) = 0;
		virtual ~Manifest();
		
		virtual void addFile(const std::string& path);
		virtual void addDir(const std::string& path);
	};

	LocalIPackage();
	//LocalIPackage(const XML::Document& src) = 0;
	//LocalIPackage(const RemoteIPackage& src) = 0;
		// Create the local package from the remote package
		// reference with the following manipulations.
		//	1) Add a local manifest element.
		//	2) Remove asset mirror elements.

	virtual ~LocalIPackage();

	virtual std::string version() const = 0;
		// Returns the current version of the local package.		
	
	virtual void setVersion(const std::string& state) = 0;
		// Sets the current version of the local package
		// once installation is complete.
	
	virtual std::string state() const = 0;
		// Returns the current state of the local package.	
	
	virtual void setState(const std::string& state) = 0;
		// Set's the package state:
		//	Not Installed, Installed, Error.	
	
	virtual std::string installState() const = 0;
		// Returns the current installation state of the 
		// local package.	

	virtual void setInstallState(const std::string& state) = 0;
		// Set's the package installation state:
		//	@see IPackageInstallState.

	virtual bool isInstalled();
		// Returns weather the package is installed or not.

	//virtual void setInstalled(bool flag) = 0;
		// This method is called when package installation
		// completes in a successfull or failed capacity. 
		// The package version will be set from the asset,
		// and the installed attribute will be updated.

	virtual Manifest manifest() const = 0;
		// Returns a temporary pointer to the installation
		// file manifest. The pointer must be deleted.
	
	virtual bool isLatestVersion(const IPackage::IAsset& asset) const;
		// Checks if the provided asset is a newer than the
		// currently installed version.

	virtual IAsset copyAsset(const IPackage::IAsset& asset) = 0;
	
	//virtual IAsset getMatchingIAsset(const IPackage::IAsset& asset) const = 0;
	//virtual void setVersion(const std::string& message) = 0;
	
	virtual void addError(const std::string& message) = 0;
	virtual std::string lastError() = 0;

	virtual bool valid() const = 0;
};


// ----------------------------------------------------------------------------
//
struct PackagePair
	/// This class provides easy pairing for a local and a
	/// remote package.
{
	PackagePair(LocalIPackage& local, RemoteIPackage& remote);
	
	virtual bool valid() const;

	LocalIPackage& local;
	RemoteIPackage& remote;
};
		
	
// ----------------------------------------------------------------------------
//
typedef BasicManager<std::string, LocalIPackage>		LocalPackageMap;
typedef LocalPackageMap::Map							LocalPackageMapT;
typedef BasicManager<std::string, RemoteIPackage>		RemotePackageMap;
typedef RemotePackageMap::Map							RemotePackageMapT;


} } // namespace Sourcey::Pacman


#endif // PACKAGE_MANAGER_IIPackage_XML_H