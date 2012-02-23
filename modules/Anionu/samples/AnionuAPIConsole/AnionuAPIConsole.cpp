#include "Sourcey/Anionu/APIClient.h"
#include "Sourcey/Base/TaskRunner.h"
#include "Sourcey/Base/Util.h"

#include "Poco/format.h"
#include "Poco/Path.h"
#include "Poco/File.h"

#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/FilePartSource.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Sourcey;
using namespace Sourcey::Anionu;
using namespace Sourcey::Util;


// Detect Memory Leaks
/*
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif<DataPacket>
*/


class AnionuAPIConsole: public APIClient
{
public:
	AnionuAPIConsole()
	{
	}

	~AnionuAPIConsole()
	{
	}


	void onAPITransactionComplete(void*, APIService& service, HTTP::Response& response)
	{
		Log("trace") << "Anionu API Response:" 
			<< "\n\tService: " << service.name
			<< "\n\tStatus: " << response.getStatus()
			<< "\n\tReason: " << response.getReason()
			<< "\n\tSuccess: " << response.success()
			<< endl;
		
		Log("trace") << "Anionu API Response Header:" << endl;
		response.write(cout);

		Log("trace") << "Anionu API Response Body:" << endl;
		cout << response.data.str() << endl;
	}
};


int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", VerboseLevel));
	
	AnionuAPIConsole api;	
	api.setCredentials("admin", "inusuvup");
	//api.loadServices();

	string service("GetAccount");
	
	char o = 0;
	while (o != 'Q') 
	{	
		cout << 
			"COMMANDS:\n\n"
			"  A	Set Target Service.\n"
			"  C	Call Basic Service.\n"
			"  U	Call Upload Asset Service.\n"
			"  L	List Services.\n"
			//"  K	List Remote Packages.\n"
			//"  R	Reload Service List.\n"
			//"  I	Install Packages.\n"
			//"  U	Uninstall Packages.\n"
			//"  D	Update All Packages.\n"
			"  Q	Quit.\n\n";
		
		o = toupper(getch());
		
		// Set Target Service
		if (o == 'A') {	
			cout << "Enter target service name: " << endl;
			getline(cin, service);
		}

		// Call Basic Service
		else if (o == 'C') {
			assert(!service.empty());
			APITransaction* transaction = api.call(service);
			transaction->APITransactionComplete += delegate(&api, &AnionuAPIConsole::onAPITransactionComplete);
			transaction->start();	
		} 

		// Call Upload Asset Service
		else if (o == 'U') {

			Path file("D:/test.avi");
			
			HTMLForm* form = new HTMLForm(HTMLForm::ENCODING_MULTIPART);
			form->set("asset[type]", "Video");
			form->addPart("asset[file]", new FilePartSource(file.toString()));

			APITransaction* transaction = api.upload("UploadAsset", form); //file.toString(), 
			transaction->APITransactionComplete += delegate(&api, &AnionuAPIConsole::onAPITransactionComplete);
			transaction->start();
		} 

		// List Services
		else if (o == 'L') {
			api.services().print(cout);
		} 

		// Reload Service List
		else if (o == 'R') {
			api.loadServices();
		} 
		
		/*
			LocalPackageMapT items = api.localPackages().items();
			for (LocalPackageMapT::const_iterator it = items.begin(); it != items.end(); ++it) {				
				cout << "Package: " << it->first << endl;
			}

		// Install Packages
		else if (o == 'I') {
			assert(!service.empty());
			api.installPackages(service);
		} 

		// Uninstall Packages
		else if (o == 'U') {
			assert(!service.empty());
			//assert(false);
			api.uninstallPackages(service);
		} 

		// Update All Packages
		else if (o == 'D') {
			api.updateAllPackages();
		}
		*/
	}

	system("pause");

	return 0;
}