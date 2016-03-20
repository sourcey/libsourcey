#include "scy/base.h"
#include "scy/platform.h"
#include "scy/filesystem.h"
#include "scy/logger.h"
#include "scy/util.h"
#include "scy/archo/zip.h"

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
        std::string path("recordingmodeplugin-0.9.2-sdk-0.6.0-win32-debug.zip");
        ZipFile zip(path);

        std::string output(scy::getCwd());
        fs::addnode(output, "test");
        zip.extract(output);
    }

    // void testGZStream()
    // {
    //
    //     std::string path("gzstream.tgz");
    //     std::string dest("gzstream");
    //
    //     assert(fs::exists("gzstream.tgz"));
    //
    //     // check alternate way of opening file
    //     igzstream in2;
    //     in2.open(path.c_str());
    //     if (!in2.good()) {
    //         std::cerr << "ERROR: Opening file `" << path << "' failed.\n";
    //         assert(0);
    //     }
    //     in2.close();
    //     if (!in2.good()) {
    //         std::cerr << "ERROR: Closing file `" << path << "' failed.\n";
    //         assert(0);
    //     }
    //
    //     // now use the shorter way with the constructor to open the same file
    //     igzstream in(path.c_str());
    //     if (!in.good()) {
    //         std::cerr << "ERROR: Opening file `" << path << "' failed.\n";
    //         assert(0);
    //     }
    //     std::ofstream out(dest.c_str());
    //     if (!out.good()) {
    //         std::cerr << "ERROR: Opening file `" << dest << "' failed.\n";
    //         assert(0);
    //     }
    //
    //     char c;
    //     while (in.get(c))
    //         out << c;
    //     in.close();
    //     out.close();
    //     if (!in.eof()) {
    //         std::cerr << "ERROR: Reading file `" << path << "' failed.\n";
    //         assert(0);
    //     }
    //     if (!out.good()) {
    //         std::cerr << "ERROR: Writing file `" << dest << "' failed.\n";
    //         assert(0);
    //     }
    // }
};


} } // namespace scy::arc


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("Test", LTrace));
    {
        arc::Tests app;
    }
    Logger::destroy();
    return 0;
}
