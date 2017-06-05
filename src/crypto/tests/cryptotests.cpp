#include "scy/crypto/crypto.h"
#include "scy/crypto/cipher.h"
#include "scy/crypto/hash.h"
#include "scy/crypto/rsa.h"
#include "scy/test.h"
#include "scy/logger.h"
#include "scy/util.h"
#include "scy/hex.h"

#include <algorithm>
#include <stdexcept>


using std::endl;
using namespace scy;
using namespace scy::test;


namespace scy {


void testCipher(const std::string algorithm, int iterations)
{
    {
        crypto::Cipher ciph(algorithm);
        clock_t start = clock();
        for (int n = 1; n < iterations; n++) {
            std::string in(n, 'x');
            std::string out = ciph.encryptString(in, crypto::Cipher::Binary);
            std::string result = ciph.decryptString(out, crypto::Cipher::Binary);
            expect(in == result);
        }
        LDebug("Binary: ", (clock() - start))
    }

    {
        crypto::Cipher ciph(algorithm);
        clock_t start = clock();
        for (int n = 1; n < iterations; n++) {
            std::string in(n, 'x');
            std::string out = ciph.encryptString(in, crypto::Cipher::Base64);
            std::string result = ciph.decryptString(out, crypto::Cipher::Base64);
            expect(in == result);
        }
        LDebug("Base64: ", (clock() - start))
    }

    {
        crypto::Cipher ciph(algorithm);
        clock_t start = clock();
        for (int n = 1; n < iterations; n++) {
            std::string in(n, 'x');
            std::string out = ciph.encryptString(in, crypto::Cipher::BinHex);
            std::string result = ciph.decryptString(out, crypto::Cipher::BinHex);
            expect(in == result);
        }
        LDebug("BinHex: ", (clock() - start))
    }

    {
        crypto::Cipher ciph(algorithm);
        std::string iv(util::randomString(16));
        std::string key(util::randomString(32));
        std::string in(1000, 'x');

        std::string out = crypto::encryptString(algorithm, in, key, iv, crypto::Cipher::Binary);
        std::string result = crypto::decryptString(algorithm, out, key, iv, crypto::Cipher::Binary);
        expect(in == result);
    }

    {
        // Quick test using string input buffer

        crypto::Cipher ciph(algorithm);
        std::string in(20, 'x');

        // We can use a string as buffer as long
        // as the implementation is contiguous (as per c++11)
        std::string out(100, '\0');
        ssize_t len = ciph.encrypt(in, out, crypto::Cipher::BinHex);
        out.resize(len);

        std::string result = ciph.decryptString(out, crypto::Cipher::BinHex);
        expect(in == result);
    }
}


} // namespace scy


int main(int argc, char** argv)
{
    // Logger::instance().add(new ConsoleChannel("debug", Level::Trace));
    test::init();
    crypto::initializeEngine();

    // =========================================================================
    // Cypher
    //
    describe("aes256 cypher", []() {
        testCipher("aes-256-cbc", 10000);
    });

    // =========================================================================
    // SHA1 Hash
    //
    describe("sha1 hash", []() {
        // test vectors from FIPS 180-1

        crypto::Hash engine("SHA1");
        engine.update("abc", 3);
        expect(hex::encode(engine.digest()) == "a9993e364706816aba3e25717850c26c9cd0d89d");

        engine.reset();
        engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
        expect(hex::encode(engine.digest()) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

        engine.reset();
        for (int i = 0; i < 1000000; ++i)
            engine.update('a');
        expect(hex::encode(engine.digest()) == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
    });

    // =========================================================================
    // MD5 Hash
    //
    describe("md5 hash", []() {
        crypto::Hash engine("MD5");

        // test vectors from RFC 1321

        engine.update("");
        expect(hex::encode(engine.digest()) == "d41d8cd98f00b204e9800998ecf8427e");

        engine.reset();
        engine.update("a");
        expect(hex::encode(engine.digest()) == "0cc175b9c0f1b6a831c399e269772661");

        engine.reset();
        engine.update("abc");
        expect(hex::encode(engine.digest()) == "900150983cd24fb0d6963f7d28e17f72");

        engine.reset();
        engine.update("message digest");
        expect(hex::encode(engine.digest()) == "f96b697d7cb7938d525a2f31aaf161d0");

        engine.reset();
        engine.update("abcdefghijklmnopqrstuvwxyz");
        expect(hex::encode(engine.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");

        engine.reset();
        engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
        expect(hex::encode(engine.digest()) == "d174ab98d277d9f5a5611c2c9f419d9f");

        engine.reset();
        engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
        expect(hex::encode(engine.digest()) == "57edf4a22be3c955ac49da2e2107b67a");
    });

    // =========================================================================
    // Hex
    //
    describe("hex encoder/decoder", []() {
        hex::Encoder enc;
        hex::Decoder dec;

        std::string in(1000, 'x');

        char encBuf[2048];
        size_t len = enc.encode(in.c_str(), in.length(), encBuf);
        std::string encRes(encBuf, len);
        LDebug("Encoded: ", encRes)

        char decBuf[2048];
        len = dec.decode(encBuf, len, decBuf);
        std::string decRes(decBuf, len);
        LDebug("Decoded: ", decRes)

        expect(in == decRes);
    });

    // =========================================================================
    // Checksum
    //
    describe("file checksum", []() {
        // NOTE: checksum tests currently located in HTTP tests

        // std::string path("test.mp4");
        // expect(crypto::checksum("MD5", path) == "57e14d2f24ab34a6eb1de3eb82f02f33");
    });

    test::runAll();

    crypto::uninitializeEngine();
    // Logger::destroy();

    return finalize();
}
