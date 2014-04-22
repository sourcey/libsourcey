#include "scy/application.h"
#include "scy/pacm/packagemanager.h"
#include "scy/net/sslmanager.h"
#include "scy/util.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


using std::endl;
using namespace scy;


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
	PackageManagerApplication(const pacm::PackageManager::Options& options) :
		manager(options)
	{
		// Default packages to install
		packages.push_back("surveillancemodeplugin");
		//packages.push_back("recordingmodeplugin");
	}

	virtual ~PackageManagerApplication()
	{
	}
	
	StringVec packages;
	pacm::PackageManager manager;
};


int main(int argc, char** argv)
{
	scy::Logger::instance().add(new ConsoleChannel("debug", LTrace));

	// Init SSL Context
	net::SSLContext::Ptr ptrContext = new net::SSLContext(net::SSLContext::CLIENT_USE, "", "", "",
		net::SSLContext::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");	
	net::SSLManager::instance().initializeClient(ptrContext);

	pacm::PackageManager::Options opts;
	opts.endpoint = "https://anionu.com"; // "http://127.0.0.1:3000";	
	opts.indexURI = "/packages.json";
	opts.httpUsername = Anionu_API_USERNAME;
	opts.httpPassword = Anionu_API_KEY;

	PackageManagerApplication app(opts);	
	app.manager.initialize();
	app.manager.queryRemotePackages();
	
	char o = 0;
	while (o != 'Q') {	
		// Run event loop while installation tasks are running
		app.run(); 

		// Wait for input commands on idle
		InfoL << 
			"COMMANDS:\n\n"
			"  A	Set Active Packages.\n"
			"  L	List Local Packages.\n"
			"  K	List Remote Packages.\n"
			"  J	Display Latest Remote Package Asset.\n"
			"  R	Reload Package List.\n"
			"  I	Install Packages.\n"
			"  U	Uninstall Packages.\n"
			"  D	Update All Packages.\n"
			"  Q	Quit.\n\n" << endl;
		
		o = toupper(getch());	

		// Set Active Packages
		if (o == 'A') {	
			InfoL << "Enter packages names separated by commas: " << endl;
			std::string s;
			std::getline(std::cin, s);
			app.packages = util::split(s, ",");
		}

		// List Local Packages
		else if (o == 'L') {
			InfoL << "Listing local packages: " << app.manager.localPackages().size() << endl;
			auto items = app.manager.localPackages().map();
			for (auto it = items.begin(); it != items.end(); ++it) {				
				InfoL << "Package: " << it->first << endl;
			}
		} 

		// List Remote Packages
		else if (o == 'K') {
			InfoL << "Listing remote packages: " << app.manager.remotePackages().size() << endl;
			auto items = app.manager.remotePackages().map();
			for (auto it = items.begin(); it != items.end(); ++it) {				
				InfoL << "Package: " << it->first << endl;
			}
		} 

		// Display Latest Remote Package Asset
		else if (o == 'J') {
			auto  items = app.manager.remotePackages().map();
			for (auto it = items.begin(); it != items.end(); ++it) {			
				it->second->latestAsset().print(std::cout);	
			}
		} 

		// Reload Package List
		else if (o == 'R') {
			app.manager.uninitialize();
			app.manager.initialize();
			app.manager.queryRemotePackages();
		} 

		// Install Packages
		else if (o == 'I') {
			InfoL << "Install packages: " << app.packages.size() << endl;
			assert(!app.packages.empty());
			app.manager.installPackages(app.packages);
			InfoL << "Install packages: OK: " << app.packages.size() << endl;
		} 

		// Uninstall Packages
		else if (o == 'U') {
			assert(!app.packages.empty());
			app.manager.uninstallPackages(app.packages);
		} 

		// Update All Packages
		else if (o == 'D') {
			app.manager.updateAllPackages();
		}
	}
	
	GarbageCollector::instance().finalize();
	http::Client::instance().shutdown();
	net::SSLManager::instance().shutdown();
	Logger::shutdown();
	return 0;
}


#if 0
	Thread console([](void* arg) 
	{
		auto app = reinterpret_cast<PackageManagerApplication*>(arg);		
	
		char o = 0;
		while (o != 'Q') 
		{	
			InfoL << 
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
				InfoL << "Enter packages names separated by commas: " << endl;
				string s;
				getline(cin,s);
				packages = util::split(s, ",");
			}

			// List Local Packages
			else if (o == 'L') {
				InfoL << "Listing local packages: " << app->manager.localPackages().size() << endl;
				auto items = app->manager.localPackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {				
					InfoL << "Package: " << it->first << endl;
				}
			} 

			// List Remote Packages
			else if (o == 'K') {
				InfoL << "Listing remote packages: " << app->manager.remotePackages().size() << endl;
				auto items = app->manager.remotePackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {				
					InfoL << "Package: " << it->first << endl;
				}
			} 

			// Display Latest Remote Package Asset
			else if (o == 'J') {
				auto  items = app->manager.remotePackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {			
					it->second->latestAsset().print(InfoL);	
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
#endif