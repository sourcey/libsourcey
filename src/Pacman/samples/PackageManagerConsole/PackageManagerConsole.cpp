#include "scy/application.h"
#include "scy/pacman/packagemanager.h"
#include "scy/util.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


using namespace std;
using namespace scy;
using namespace pman;


// Detect Memory Leaks
/*
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif<DataPacket>
*/


class PackageManagerApplication: public scy::Application	
{
public:
	PackageManagerApplication(const pman::PackageManager::Options& options) :
		manager(options)
	{
	}

	~PackageManagerApplication()
	{
	}

	pman::PackageManager manager;
};


int main(int argc, char** argv)
{
	scy::Logger::instance().add(new ConsoleChannel("debug", LTrace));

	PackageManager::Options opts;
	opts.endpoint = "http://127.0.0.1:3000";	
	opts.indexURI = "/packages.json";
	opts.httpUsername = Anionu_API_USERNAME;
	opts.httpPassword = Anionu_API_KEY;

	PackageManagerApplication app(opts);	
	app.manager.initialize();
	app.manager.queryRemotePackages();
	app.run(); // run event loop until we have queried server packages

	Thread console([](void* arg) 
	{
		auto app = reinterpret_cast<PackageManagerApplication*>(arg);		

		StringVec packages;
		packages.push_back("SurveillanceModePlugin");
		packages.push_back("RecordingModePlugin");
	
		char o = 0;
		while (o != 'Q') 
		{	
			cout << 
				"COMMANDS:\n\n"
				"  A	Set Active Packages.\n"
				"  L	List Local Packages.\n"
				"  K	List Remote Packages.\n"
				"  J	Display Latest Remote Package Asset.\n"
				"  R	Reload Package List.\n"
				"  I	Install Packages.\n"
				"  U	Uninstall Packages.\n"
				"  D	Update All Packages.\n"
				"  Q	Quit.\n\n";
		
			o = toupper(getch());		

			// Set Active Packages
			if (o == 'A') {	
				cout << "Enter packages names separated by commas: " << endl;
				string s;
				getline(cin,s);
				packages = util::split(s, ",");
			}

			// List Local Packages
			else if (o == 'L') {
				cout << "Listing local packages: " << app->manager.localPackages().size() << endl;
				auto items = app->manager.localPackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {				
					cout << "Package: " << it->first << endl;
				}
			} 

			// List Remote Packages
			else if (o == 'K') {
				cout << "Listing remote packages: " << app->manager.remotePackages().size() << endl;
				auto items = app->manager.remotePackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {				
					cout << "Package: " << it->first << endl;
				}
			} 

			// Display Latest Remote Package Asset
			else if (o == 'J') {
				auto  items = app->manager.remotePackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {			
					it->second->latestAsset().print(cout);	
				}
			} 

			// Reload Package List
			else if (o == 'R') {
				app->manager.uninitialize();
				app->manager.initialize();
			} 

			// Install Packages
			else if (o == 'I') {
				assert(!packages.empty());
				app->manager.installPackages(packages);
			} 

			// Uninstall Packages
			else if (o == 'U') {
				assert(!packages.empty());
				app->manager.uninstallPackages(packages);
			} 

			// Update All Packages
			else if (o == 'D') {
				app->manager.updateAllPackages();
			}
		}
	}, &app);

	app.waitForShutdown([](void* arg) {			
		reinterpret_cast<PackageManagerApplication*>(arg)->stop();
	}, &app);

	scy::pause();
	return 0;
}