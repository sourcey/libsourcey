#include "Sourcey/Base.h"
#include "Sourcey/Platform.h"
#include "Sourcey/FileSystem.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Sourcey/STUN/Message.h"

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
namespace stun {
	

class Tests
{
public:
	Tests()
	{					
		testMessageIntegrity();
	}
	
	void testMessageIntegrity() 
	{	
		std::string username("someuser");
		std::string password("somepass");
		
		stun::Message request;
		request.setType(stun::Message::Allocate);
		
		auto usernameAttr = new stun::Username;
		usernameAttr->copyBytes(username.c_str(), username.size());
		request.add(usernameAttr);
		
		auto integrityAttr = new stun::MessageIntegrity;
		integrityAttr->setKey(password);
		request.add(integrityAttr);

		Buffer buf;
		request.write(buf);

		stun::Message response;
		response.read(constBuffer(buf));
		
		integrityAttr = response.get<stun::MessageIntegrity>();
		assert(integrityAttr->verifyHmac(password));
	}
};


} } // namespace scy::stun


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("Test", LTrace));
	{
		stun::Tests app;
	}	
	Logger::shutdown();
	return 0;
}