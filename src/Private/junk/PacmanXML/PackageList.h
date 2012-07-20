#ifndef PACKAGE_MANAGER_PackageList_H
#define PACKAGE_MANAGER_PackageList_H


#include "Sourcey/Base/Logger.h"
#include "Sourcey/Base/BasicManager.h"
#include "Sourcey/PackageManager/Package.h"


namespace Sourcey {


class PackageManager;

/*
class PackageList: protected LocalPackageMap, public ILoggable
{
public:
	PackageList(); //PackageManager& manager
	virtual ~PackageList();
	
	virtual const char* className() const { return "PackageList"; }

protected:
	bool add(Package* package);
	//bool remove(const std::string& name);

	//PackageManager& _manager;
	friend class PackageManager;
};
	virtual Package* get(const std::string& name);
		// Returns the Package instance or throws a
		// NotFoundException exception.
		*/

} // namespace Sourcey


#endif // PACKAGE_MANAGER_PackageList_H