#include "Sourcey/Base.h"
#include "Sourcey/Platform.h"
#include "Sourcey/FileSystem.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Sourcey/Archo/Zip.h"

#include <assert.h>
#include <algorithm>
#include <stdexcept>


using namespace std;
using namespace scy;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.cpp"
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace arc {
	

class Tests
{
public:
	Tests()
	{					
		testUnzip();
	}
	
	void testUnzip() 
	{			
		std::string path("MediaPlugin-0.8.8-SDK-0.5.0-win32-debug.zip");
		ZipFile zip(path);	
		
		std::string output(scy::getCwd());
		fs::addnode(output, "test");
		zip.extractTo(output);
	}	
};


} } // namespace scy::arc


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("Test", LTrace));
	{
		arc::Tests app;
	}	
	Logger::shutdown();
	return 0;
}