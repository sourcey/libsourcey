#include "Sourcey/Media/DeviceManager.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Util.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


using namespace std;
using namespace Poco;
using namespace Sourcey;
using namespace Sourcey::Media;


IDeviceManager* devices;


// Detect Memory Leaks
/*
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif<DataPacket>
*/


int main(int argc, char** argv)
{
	Sourcey::Logger::instance().add(new ConsoleChannel("debug", TraceLevel));

	devices = DeviceManagerFactory::create();
	devices->initialize();
	
	char o = 0;
	while (o != 'Q') 
	{	
		cout << 
			"COMMANDS:\n\n"
			"  P	Print Devices.\n"
			"  Q	Quit.\n\n";
		
		o = toupper(getch());
		
		// Print Devices
		if (o == 'P') {	
			reinterpret_cast<DeviceManager*>(devices)->print(cout);
		}
	}

	return 0;
}