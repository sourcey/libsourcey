#include "Sourcey/Pacman/PackageManager.h"
#include "Sourcey/Util.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


using namespace std;
using namespace Poco;
using namespace Sourcey;
using namespace Sourcey::Util;
using namespace Sourcey::Pacman;


// Detect Memory Leaks
/*
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif<DataPacket>
*/


class PackageManagerConsole: public PackageManager
{
public:
	PackageManagerConsole(const PackageManager::Options& options) :
		PackageManager(options)
	{
	}

	~PackageManagerConsole()
	{
	}
};


int main(int argc, char** argv)
{
	Sourcey::Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	
	PackageManager::Options options;
	options.httpUsername = Anionu_API_USERNAME;
	options.httpPassword = Anionu_API_PASSWORD;
	PackageManagerConsole app(options);	
	app.initialize();
	
	/*	
	// Initialize the HTTP download transaction.
	HTTP::Request* request = new HTTP::Request("GET", "http://localhost:3000/assets/122/RecordingModePlugin-1.3.12-Spot-1.0-win32-bin.zip?1320559835");
	HTTP::Transaction* transaction = new HTTP::Transaction(request);
	transaction->setOutputPath(app.options().cacheDir + "/RecordingModePlugin-1.3.12-Spot-1.0-win32-bin.zip");
	//transaction->TransactionComplete += delegate(this, &PackageTask::onTransactionComplete);
	transaction->start();

	
	HTTP::TransactionState transfer;
	transfer.uri = "http://localhost:3000/assets/122/RecordingModePlugin-1.3.12-Spot-1.0-win32-bin.zip?1320559835";
	string filename = transfer.uri.getPath();
	size_t start = (filename.find_last_of("/") + 1);
	filename = filename.substr(start, filename.length() - start);
	transfer.path = app.options().cacheDir + filename;

	Anionu::APIDownloadTransaction* transaction = new Anionu::APIDownloadTransaction(transfer);
	//transaction->UploadProgress += delegate(this, &PackageTask::onUploadProgress);
	transaction->start();	
	*/

	StringList packages;
	packages.push_back("SurveillanceMode");
	
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
			packages = split(s, ",");
		}

		// List Local Packages
		else if (o == 'L') {
			LocalPackageMapT items = app.localPackages().items();
			for (LocalPackageMapT::const_iterator it = items.begin(); it != items.end(); ++it) {				
				cout << "Package: " << it->first << endl;
			}
		} 

		// List Remote Packages
		else if (o == 'K') {
			RemotePackageMapT items = app.remotePackages().items();
			for (RemotePackageMapT::const_iterator it = items.begin(); it != items.end(); ++it) {				
				cout << "Package: " << it->first << endl;
			}
		} 

		// Display Latest Remote Package Asset
		else if (o == 'J') {
			RemotePackageMapT items = app.remotePackages().items();
			for (RemotePackageMapT::const_iterator it = items.begin(); it != items.end(); ++it) {			
				it->second->latestAsset().print(cout);	
			}
		} 

		// Reload Package List
		else if (o == 'R') {
			app.uninitialize();
			app.initialize();
		} 

		// Install Packages
		else if (o == 'I') {
			assert(!packages.empty());
			app.installPackages(packages);
		} 

		// Uninstall Packages
		else if (o == 'U') {
			assert(!packages.empty());
			//assert(false);
			app.uninstallPackages(packages);
		} 

		// Update All Packages
		else if (o == 'D') {
			app.updateAllPackages();
		}
	}

	system("pause");

	return 0;
}