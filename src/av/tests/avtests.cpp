#include "avtests.h"


using namespace std;
using namespace scy;
using namespace scy::av;
using namespace scy::test;


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LDebug)); //Trace
    // test::initialize();
    // net::SSLManager::initNoVerifyClient();

    // Create the static callback context
    // static CallbackContext context;

    //
    /// Device Manager Tests
    //

    // describe("device manager", []() {
    //     cout << "Starting" << endl;
    //     auto& deviceManager = av::MediaFactory::instance().devices();
    //
    //     av::Device device;
    //     if (deviceManager.getDefaultCamera(device)) {
    //         cout << "Default video device: " << device.id << ": " << device.name << endl;
    //     }
    //     if (deviceManager.getDefaultMicrophone(device)) {
    //         cout << "Default audio device: " << device.id << ": " << device.name << endl;
    //     }
    //
    //     std::vector<av::Device> devices;
    //     if (deviceManager.getCameras(devices)) {
    //         cout << "Num video devices: " << devices.size() << endl;
    //         for (auto& device : devices) {
    //             cout << "Printing video device: " << device.id << ": " << device.name << endl;
    //         }
    //     }
    //     else {
    //         cout << "No video devices detected!" << endl;
    //     }
    //     if (deviceManager.getMicrophones(devices)) {
    //         cout << "Num audio devices: " << devices.size() << endl;
    //         for (auto& device : devices) {
    //             cout << "Printing audio device: " << device.id << ": " << device.name << endl;
    //         }
    //     }
    //     else {
    //         cout << "No video devices detected!" << endl;
    //     }
    //
    //     // TODO: verify data integrity?
    // });

    // Define class based tests
    describe("audio encoder", new AudioEncoderTest);
    describe("audio resampler", new AudioResamplerTest);
    describe("audio capture encoder", new AudioCaptureEncoderTest);
    describe("audio capture resampler", new AudioCaptureResamplerTest);
    // describe("device capture multiplex encoder", new DeviceCaptureMultiplexEncoderTest);

    test::runAll();

    return test::finalize();
}


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
