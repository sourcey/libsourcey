#include "Sourcey/PackageManager/PackageList.h"
//#include "Sourcey/Spot/PackageList.h"


using namespace std;


namespace Sourcey {


PackageList::PackageList() //PackageList& manager:
	//_manager(manager)
{
}


PackageList::~PackageList() 
{	
}


bool PackageList::add(Package* package) 
{
	assert(package);
	assert(!package->name().empty());
	
	Log("trace") << "Adding Package: " << package->name() << endl;	
	return LocalPackageMap::add(package->name(), package);
}


/*
bool PackageList::remove(const string& name) 
{
	assert(!name.empty());

	Log("trace") << "Removing Package: " << name << endl;		
	return LocalPackageMap::free(name);
}


Package* PackageList::get(const string& name) 
{
	return LocalPackageMap::get(name);
}
*/


} // namespace Sourcey