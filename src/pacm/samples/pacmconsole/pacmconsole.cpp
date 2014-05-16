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


// pacmconsoled.exe --help
// pacmconsoled.exe --endpoint https://anionu.com --uri /packages.json --install surveillancemodeplugin,recordingmodeplugin --print
// pacmconsoled.exe --endpoint https://anionu.com --uri /packages.json --uninstall surveillancemodeplugin,recordingmodeplugin --print


class PacmApplication: public scy::Application	
{
public:
	pacm::PackageManager manager;

	struct Options
	{	
		StringVec install;
		StringVec uninstall;
		bool help;
		bool update;
		bool print;

		Options() {
			help = false;
			update = false;
			print = false;
		}
	} options;

	PacmApplication()
	{
	}
	
	void parseOptions(int argc, char* argv[])
	{
		OptionParser optparse(argc, argv, "--");
		for (auto it = optparse.args.begin(); it != optparse.args.end(); ++it) {	
			const std::string& key = it->first;
			const std::string& value = it->second;
			DebugL << "Setting option: " << key << ": " << value << std::endl;

			if (key == "help") {
				options.help = true;
			}
			else if (key == "endpoint") {
				manager.options().endpoint = value;
			}
			else if (key == "uri") {
				manager.options().indexURI = value;
			}
			else if (key == "install-dir") {
				manager.options().installDir = value;
			}
			else if (key == "data-dir") {
				manager.options().dataDir = value;
			}
			else if (key == "cache-dir") {
				manager.options().cacheDir = value;
			}
			else if (key == "packages") {
				manager.options().endpoint = value;
			}
			else if (key == "install") {
				options.install = util::split(value, ",");
			}
			else if (key == "uninstall") {
				options.uninstall = util::split(value, ",");
			}
			else if (key == "update") {
				options.update = true;
			}
			else if (key == "print") {
				options.print = true;
			}
			else if (key == "logfile") {
				auto log = dynamic_cast<FileChannel*>(scy::Logger::instance().get("Pacm"));
				log->setPath(value);
			}
		}
	}

	virtual ~PacmApplication()
	{
	}

	void work()
	{		
		// Print help
		if (options.help) {
			std::cout << 
				"\nPacm v0.1.0"
				"\n(c) Sourcey"
				"\nhttp://sourcey.com/pacm"
				"\n"
				"\n### Server commands:"
				"\n"
				"\n  --endpoint     HTTP server endpoint"
				"\n  --uri          HTTP server package JSON index URI"
				"\n"
				"\n### Package commands:"
				"\n"
				"\n  --install      Packages to install (comma separated)"
				"\n  --uninstall    Packages to uninstall (comma separated)"
				"\n  --update       Update all packages"
				"\n  --print        Print all local and remote packages" << endl;
			return;
		}

		// Initialize Pacman and query remote packages from the server
		manager.initialize();
		manager.queryRemotePackages();
		scy::Application::run(); 
		assert(manager.initialized());

		// Uninstall packages if requested
		if (!options.uninstall.empty()) {
			std::cout << "### Uninstall packages: " << options.install.size() << endl;
			manager.uninstallPackages(options.uninstall);
			scy::Application::run(); 
		}

		// Install packages if requested
		if (!options.install.empty()) {
			std::cout << "### Install packages: " << options.install.size() << endl;
			manager.installPackages(options.install);
			scy::Application::run(); 
		}

		// Update all packages if requested
		if (options.update) {
			std::cout << "### Update all packages" << endl;
			manager.updateAllPackages();
			scy::Application::run(); 
		}

		// Print packages to stdout
		if (options.print) {
			std::cout << "### Print packages" << endl;

			// Print local packages
			{
				std::cout << "Local packages: " << manager.localPackages().size() << endl;
				for (auto& kv : manager.localPackages().map()) {			
					std::cout << "- " << kv.first << ": version=" 
						<< kv.second->version() << ", state=" 
						<< kv.second->state() << endl;
				}
			}

			// Print remote packages
			{
				std::cout << "Remote packages: " << manager.remotePackages().size() << endl;
				for (auto& kv : manager.remotePackages().map()) {	
					std::cout << "- " << kv.first << ": version=" 
						<< kv.second->latestAsset().version() << ": author=" 
						<< kv.second->author() << endl;
				}
			}
		}
	}	
};


int main(int argc, char** argv)
{
	{
#ifdef _MSC_VER
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

		Logger::instance().add(new ConsoleChannel("Pacm", 
//#ifdef _DEBUG
//		LTrace
//#else
			LInfo
//#endif
			));

		// Init SSL Context 	
		net::SSLManager::initNoVerifyClient();
	
		{
			PacmApplication app;	
			app.parseOptions(argc, argv);
			app.work();
		}
	
		GarbageCollector::destroy();
		net::SSLManager::destroy();
		http::Client::destroy();
		Logger::destroy();
	}
#ifdef _DEBUG
	scy::pause();
#endif
	return 0;
}


#if 0
	Thread console([](void* arg) 
	{
		auto app = reinterpret_cast<PacmApplication*>(arg);		
	
		char o = 0;
		while (o != 'Q') 
		{	
			std::cout << 
				"COMMANDS:\n"
				"  A	Set Active Packages."
				"  L	List Local Packages."
				"  K	List Remote Packages."
				"  J	Display Latest Remote Package Asset."
				"  R	Reload Package List."
				"  I	Install Packages."
				"  U	Uninstall Packages."
				"  D	Update All Packages."
				"  Q	Quit.\n";
		
			o = toupper(getch());		

			// Set Active Packages
			if (o == 'A') {	
				std::cout << "Enter packages names separated by commas: " << endl;
				string s;
				getline(cin,s);
				packages = util::split(s, ",");
			}

			// List Local Packages
			else if (o == 'L') {
				std::cout << "Listing local packages: " << app->manager.localPackages().size() << endl;
				auto items = app->manager.localPackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {				
					std::cout << "Package: " << it->first << endl;
				}
			} 

			// List Remote Packages
			else if (o == 'K') {
				std::cout << "Listing remote packages: " << app->manager.remotePackages().size() << endl;
				auto items = app->manager.remotePackages().map();
				for (auto it = items.begin(); it != items.end(); ++it) {				
					std::cout << "Package: " << it->first << endl;
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
				app->manager.install(packages);
			} 

			// Uninstall Packages
			else if (o == 'U') {
				assert(!packages.empty());
				app->manager.uninstall(packages);
			} 

			// Update All Packages
			else if (o == 'D') {
				app->manager.updateAllPackages();
			}
		}
	}, &app);

	app.waitForShutdown([](void* arg) {			
		reinterpret_cast<PacmApplication*>(arg)->stop();
	}, &app);
#endif