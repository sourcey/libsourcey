#include "mediatests.h"


using namespace std;
using namespace scy;
using namespace scy::av;


int main(int argc, char** argv)
{
    // Logger::instance().add(new ConsoleChannel("debug", LTrace));
    test::initialize();
    // net::SSLManager::initNoVerifyClient();

    // // Create the static callback context
    // static CallbackContext context;
    //
    // //
    // /// Device Manager Tests
    // //
    //
    // describe("device manager", []() {
    //     cout << "Starting" << endl;
    //
    //     Device device;
    //     if (MediaFactory::instance().devices().getDefaultVideoCaptureDevice(device)) {
    //         cout << "Default Video Device: " << device.id << ": " << device.name << endl;
    //     }
    //     if (MediaFactory::instance().devices().getDefaultAudioInputDevice(device)) {
    //         cout << "Default Audio Device: " << device.id << ": " << device.name << endl;
    //     }
    //
    //     std::vector<Device> devices;
    //     if (MediaFactory::instance().devices().getVideoCaptureDevices(devices)) {
    //         for (std::vector<Device>::const_iterator it = devices.begin(); it != devices.end(); ++it) {
    //             cout << "Printing Video Device: " << (*it).id << ": " << (*it).name << endl;
    //         }
    //     }
    //     if (MediaFactory::instance().devices().getAudioInputDevices(devices)) {
    //         for (std::vector<Device>::const_iterator it = devices.begin(); it != devices.end(); ++it) {
    //             cout << "Printing Audio Device: " << (*it).id << ": " << (*it).name << endl;
    //         }
    //     }
    // });
    //
    //
    // //
    // /// Video Capture Tests
    // //
    //
    // describe("video capture", []() {
    //     DebugL << "Starting" << endl;
    //
    //     av::VideoCapture::Ptr capture = MediaFactory::instance().createVideoCapture(0);
    //     capture->emitter += packetDelegate(&context, &CallbackContext::onVideoCaptureFrame);
    //
    //     // std::puts("Press any key to continue...");
    //     // std::getchar();
    //
    //     // FIXME: Run loop until x number of frames received
    //
    //     capture->emitter -= packetDelegate(this, &CallbackContext::onVideoCaptureFrame);
    //
    //     DebugL << "Complete" << endl;
    // }
    //
    // describe("video capture stream", []() {
    //     DebugL << "Starting" << endl;
    //
    //     av::VideoCapture::Ptr capture = MediaFactory::instance().createVideoCapture(0);
    //     {
    //         PacketStream stream;
    //         stream.emitter += packetDelegate(&context, &CallbackContext::onVideoCaptureStreamFrame);
    //         stream.attachSource<av::VideoCapture>(capture, true);
    //         stream.start();
    //
    //         // std::puts("Press any key to continue...");
    //         // std::getchar();
    //     }
    //
    //     assert(capture->emitter.ndelegates() == 0);
    //
    //     DebugL << "Complete" << endl;
    // });

    test::runAll();

    return test::finalize();
}
