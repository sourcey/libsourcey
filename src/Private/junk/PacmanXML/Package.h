#ifndef PACKAGE_MANAGER_Package_H
#define PACKAGE_MANAGER_Package_H


//#include "Sourcey/Pacman/Package.h"
#include "Sourcey/Pacman/Config.h"


#if defined PACKAGE_MANAGER_USE_JSON
#include "Sourcey/Pacman/Package_JSON.h"
#elif defined PACKAGE_MANAGER_USE_XML
#include "Sourcey/Pacman/Package_XML.h"
#else
#error "No package format specified"
#endif


namespace Sourcey { 
namespace Pacman {






/*
#include "Sourcey/Pacman/Package.h"
#error "No package format specified"

*/



} } // namespace Sourcey::Pacman



#endif // PACKAGE_MANAGER_Package_H

/*
#include "Sourcey/XML/XML.h"
#include "Sourcey/Base/BasicManager.h"


namespace Sourcey { 
namespace Pacman {


// ----------------------------------------------------------------------------
//
	struct IAsset//: public XML::Node
		/// This class provides an XML representation of a file
		/// asset, generally an archive, that contains the package
		/// files.
	{
		//IAsset();
		//IAsset(const pugi::xml_node& src);
		virtual ~IAsset();
		
		virtual std::string fileName() const = 0;
		virtual std::string version() const = 0;

		virtual bool valid() const = 0;

		virtual bool operator == (const IAsset& rhs) const = 0;
	};

struct Package//: public XML::Document
	/// This class provides an XML representation of a package
	/// which is available to the Package Manager.
{	

	//Package();
	//Package(const XML::Document& src);
	virtual ~Package();

	virtual IPacket* clone() const = 0;

	virtual int id() const = 0;
	virtual std::string name() const = 0;
	virtual std::string type() const = 0;
	virtual std::string title() const = 0;

	virtual bool valid() const = 0;
	
	virtual IAsset latestAsset() const = 0;
		// The latest update description set from the
		// web service response.
};


// ----------------------------------------------------------------------------
//
struct RemotePackage//: public Package
	/// This class provides an XML representation of a package
	/// that exists on the remote server.
{
	RemotePackage();
	//RemotePackage(const XML::Document& src);
	virtual ~RemotePackage();
};


// ----------------------------------------------------------------------------
//
struct LocalPackage//: public Package
	/// This class provides an XML representation of a package
	/// that exists on the local filesystem.
{	
	struct IManifest//: public XML::Node
		/// This class provides a list of all files installed
		/// by the parent package on the local filesystem.
	{
		//IManifest();
		//IManifest(const pugi::xml_node& src);
		virtual ~IManifest();
		
		virtual void addFile(const std::string& path) = 0;
		virtual void addDir(const std::string& path) = 0;
	};

	//LocalPackage();
	//LocalPackage(const XML::Document& src);
	//LocalPackage(const RemotePackage& src) = 0;
		// Create the local package from the remote package
		// reference with the following manipulations.
		//	1) Add a local manifest element.
		//	2) Remove asset mirror elements.

	virtual ~LocalPackage();

	virtual std::string version() const = 0;
		// Returns the current version of the local package.		
	
	virtual void setVersion(const std::string& state);
		// Sets the current version of the local package
		// once installation is complete.
	
	virtual std::string state() const = 0;
		// Returns the current state of the local package.	
	
	virtual void setState(const std::string& state);
		// Set's the package state:
		//	Not Installed, Installed, Error.	
	
	virtual std::string installState() const = 0;
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

	virtual IManifest manifest() const = 0;
		// Returns the installation file manifest.
	
	virtual bool isLatestVersion(const Package::IAsset& asset) const = 0;
		// Checks if the provided asset is a newer than the
		// currently installed version.

	virtual IAsset copyAsset(const Package::IAsset& asset);
	
	//virtual IAsset getMatchingIAsset(const Package::IAsset& asset) const = 0;
	//virtual void setVersion(const std::string& message);
	
	virtual void addError(const std::string& message);
	virtual std::string lastError();

	virtual bool valid() const = 0;
};



} } // namespace Sourcey::Pacman
*/