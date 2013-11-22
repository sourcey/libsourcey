#include "scy/base.h"
#include "scy/platform.h"
#include "scy/filesystem.h"
#include "scy/logger.h"
#include "scy/util.h"
#include "scy/stun/message.h"

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
	

// TODO: Test vectors from http://tools.ietf.org/html/rfc5769


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