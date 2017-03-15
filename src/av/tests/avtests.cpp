#include "avtests.h"


using namespace std;
using namespace scy;
using namespace scy::av;
using namespace scy::test;


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace)); // LTrace, LDebug
    //Logger::instance().setWriter(new AsyncLogWriter);
    test::initialize();

    // Define class based tests
#ifdef HAVE_FFMPEG
  //  describe("audio encoder", new AudioEncoderTest);
   // describe("audio resampler", new AudioResamplerTest);
   // describe("audio fifo buffer", new AudioBufferTest);
    // describe("audio capture", new AudioCaptureTest);
    // describe("audio capture encoder", new AudioCaptureEncoderTest);
    // describe("audio capture resampler", new AudioCaptureResamplerTest);
    // describe("video file transcoder", new VideoFileTranscoderTest);
    // describe("device capture multiplex encoder", new DeviceCaptureMultiplexEncoderTest);
#endif

    //describe("realtime media queue", new RealtimeMediaQueueTest);
    describe("realtime encoder media queue", new RealtimeMediaQueueEncoderTest);
    
    test::runAll();

    return test::finalize();
}


// //
// /// Device Manager Tests
// //
//
// describe("device manager", []() {
//     cout << "Starting" << endl;
//     auto& deviceManager = av::MediaFactory::instance().devices();
//
//     av::Device device;
//     if (deviceManager.getDefaultCamera(device)) {
//         cout << "Default video device: " << device.id << ": " << device.name
//         << endl;
//     }
//     if (deviceManager.getDefaultMicrophone(device)) {
//         cout << "Default audio device: " << device.id << ": " << device.name
//         << endl;
//     }
//
//     std::vector<av::Device> devices;
//     if (deviceManager.getCameras(devices)) {
//         cout << "Num video devices: " << devices.size() << endl;
//         for (auto& device : devices) {
//             cout << "Printing video device: " << device.id << ": " <<
//             device.name << endl;
//         }
//     }
//     else {
//         cout << "No video devices detected!" << endl;
//     }
//     if (deviceManager.getMicrophones(devices)) {
//         cout << "Num audio devices: " << devices.size() << endl;
//         for (auto& device : devices) {
//             cout << "Printing audio device: " << device.id << ": " <<
//             device.name << endl;
//         }
//     }
//     else {
//         cout << "No video devices detected!" << endl;
//     }
//
//     // TODO: verify data integrity?
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
//     av::VideoCapture::Ptr capture =
//     MediaFactory::instance().createVideoCapture(0);
//     capture->emitter += packetSlot(&context,
//     &CallbackContext::onVideoCaptureFrame);
//
//     // std::puts("Press any key to continue...");
//     // std::getchar();
//
//     // FIXME: Run loop until x number of frames received
//
//     capture->emitter -= packetSlot(this,
//     &CallbackContext::onVideoCaptureFrame);
//
//     DebugL << "Complete" << endl;
// }
//
// describe("video capture stream", []() {
//     DebugL << "Starting" << endl;
//
//     av::VideoCapture::Ptr capture =
//     MediaFactory::instance().createVideoCapture(0);
//     {
//         PacketStream stream;
//         stream.emitter += packetSlot(&context,
//         &CallbackContext::onVideoCaptureStreamFrame);
//         stream.attachSource<av::VideoCapture>(capture, true);
//         stream.start();
//
//         // std::puts("Press any key to continue...");
//         // std::getchar();
//     }
//
//     assert(capture->emitter.nslots() == 0);
//
//     DebugL << "Complete" << endl;
// });
