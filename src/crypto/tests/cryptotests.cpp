#include "scy/base.h"
#include "scy/platform.h"
#include "scy/logger.h"
#include "scy/hex.h"
#include "scy/util.h"
#include "scy/crypto/crypto.h"
#include "scy/crypto/hash.h"
#include "scy/crypto/cipher.h"
#include "scy/crypto/rsa.h"
#include "scy/memory.h"


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
namespace crypto {
	

class Tests
{
public:
	Tests()
	{	
		crypto::initializeEngine();
				
		testHex();
		testCipher("aes256", 10000);
		testSHA1Hash();
		testMD5Hash();

		scy::pause();

		crypto::uninitializeEngine();
	}
	
	void testHex() 
	{	

		hex::Encoder enc;
		hex::Decoder dec;

		std::string in(1000, 'x');

		char encBuf[2048];
		size_t len = enc.encode(in.c_str(), in.length(), encBuf);
		std::string encRes(encBuf, len);
		debugL("HexTest") << "Encoded: " << encRes << endl;
		
		char decBuf[2048];
		len = dec.decode(encBuf, len, decBuf);
		std::string decRes(decBuf, len);
		debugL("HexTest") << "Decoded: " << decRes << endl;

		assert(in == decRes);
	}
	
	void testCipher(const std::string algorithm, int iterations) 
	{	
		{
			Cipher ciph(algorithm);
			clock_t start = clock();
			for (std::size_t n = 1; n < iterations; n++)
			{
				std::string in(n, 'x');
				std::string out = ciph.encryptString(in, Cipher::Binary);
				std::string result = ciph.decryptString(out, Cipher::Binary);
				assert(in == result);
			}
			debugL("CipherTest") << "Binary: " << (clock() - start) << endl;
		}
	
		{
			Cipher ciph(algorithm);
			clock_t start = clock();
			for (std::size_t n = 1; n < iterations; n++)
			{
				std::string in(n, 'x');
				std::string out = ciph.encryptString(in, Cipher::Base64);
				std::string result = ciph.decryptString(out, Cipher::Base64);
				assert(in == result);
			}	
			debugL("CipherTest") << "Base64: " << (clock() - start) << endl;	
		}

		{
			Cipher ciph(algorithm);
			clock_t start = clock();
			for (std::size_t n = 1; n < iterations; n++)
			{
				std::string in(n, 'x');
				std::string out = ciph.encryptString(in, Cipher::BinHex);
				std::string result = ciph.decryptString(out, Cipher::BinHex);
				assert(in == result);
			}		
			debugL("CipherTest") << "BinHex: " << (clock() - start) << endl;
		}
		
		{
			Cipher ciph(algorithm);
			std::string iv(util::randomString(16));
			std::string key(util::randomString(32));
			std::string in(1000, 'x');
			
			std::string out = crypto::encryptString(algorithm, in, key, iv, Cipher::Binary);			
			std::string result = crypto::decryptString(algorithm, out, key, iv, Cipher::Binary);
			assert(in == result);
		}	

		{
			// Quick test using string input buffer

			Cipher ciph(algorithm);
			std::string in(20, 'x');

			// We can use a string as buffer as long 
			// as the implementation is contiguous (as per c++11)
			std::string out(100, '\0');
			int len = ciph.encrypt(in, out, Cipher::BinHex);
			out.resize(len);
			
			std::string result = ciph.decryptString(out, Cipher::BinHex);
			assert(in == result);
		}
	}
	
	void testSHA1Hash() 
	{	
		// test vectors from FIPS 180-1

		crypto::Hash engine("SHA1");
		engine.update("abc", 3);
		assert(crypto::toHex(engine.digest()) == "a9993e364706816aba3e25717850c26c9cd0d89d");
		
		engine.reset();
		engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
		assert (crypto::toHex(engine.digest()) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
		
		engine.reset();
		for (int i = 0; i < 1000000; ++i)
			engine.update('a');
		assert (crypto::toHex(engine.digest()) == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
	}
	
	void testMD5Hash()
	{
		crypto::Hash engine("MD5");

		// test vectors from RFC 1321

		engine.update("");
		assert (crypto::toHex(engine.digest()) == "d41d8cd98f00b204e9800998ecf8427e");

		engine.reset();
		engine.update("a");
		assert (crypto::toHex(engine.digest()) == "0cc175b9c0f1b6a831c399e269772661");
		
		engine.reset();
		engine.update("abc");
		assert (crypto::toHex(engine.digest()) == "900150983cd24fb0d6963f7d28e17f72");
		
		engine.reset();
		engine.update("message digest");
		assert (crypto::toHex(engine.digest()) == "f96b697d7cb7938d525a2f31aaf161d0");
		
		engine.reset();
		engine.update("abcdefghijklmnopqrstuvwxyz");
		assert (crypto::toHex(engine.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");
	
		engine.reset();
		engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
		assert (crypto::toHex(engine.digest()) == "d174ab98d277d9f5a5611c2c9f419d9f");
		
		engine.reset();
		engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
		assert (crypto::toHex(engine.digest()) == "57edf4a22be3c955ac49da2e2107b67a");
	}
};


} } // namespace scy::crypto


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("Test", LTrace));
	{
		crypto::Tests app;
	}	
	Logger::shutdown();
	return 0;
}