#include "scy/av/devicemanager.h"


using namespace scy;

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avstring.h>
}


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    {
        std::cout << "Input formats:\n\t";
        av::printInputFormats(std::cout);
        std::cout << std::endl;

        std::cout << "Output formats:\n\t";
        av::printOutputFormats(std::cout);
        std::cout << std::endl;

        av::DeviceManager devman;
        devman.print(std::cout);
    }
    Logger::destroy();
    return 0;
}
