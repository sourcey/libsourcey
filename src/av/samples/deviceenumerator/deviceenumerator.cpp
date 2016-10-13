#include "scy/av/devicemanager.h"


using namespace scy;


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    {
        av::DeviceManager devman;
        devman.print(std::cout);
    }
    Logger::destroy();
    return 0;
}
