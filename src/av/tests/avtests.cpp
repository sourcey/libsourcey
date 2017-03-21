#include "avtests.h"


using namespace std;
using namespace scy;
using namespace scy::av;
using namespace scy::test;


int main(int argc, char** argv)
{
    //Logger::instance().add(new ConsoleChannel("debug", LTrace)); // LTrace, LDebug
    //Logger::instance().setWriter(new AsyncLogWriter);
    test::initialize();

    // Define class based tests
#ifdef HAVE_FFMPEG
    describe("audio encoder", new AudioEncoderTest);
    describe("audio resampler", new AudioResamplerTest);
    describe("audio fifo buffer", new AudioBufferTest);
    describe("h264 video file transcoder", new VideoFileTranscoderTest);
    describe("h264 multiplex capture encoder", new MultiplexCaptureEncoderTest);
    // describe("realtime encoder media queue", new RealtimeMediaQueueEncoderTest);
    // describe("audio capture", new AudioCaptureTest);
    // describe("audio capture encoder", new AudioCaptureEncoderTest);
    // describe("audio capture resampler", new AudioCaptureResamplerTest);
    // describe("device capture multiplex encoder", new DeviceCaptureMultiplexEncoderTest);
#endif

    describe("realtime media queue", new RealtimeMediaQueueTest);

    test::runAll();

    return test::finalize();
}


// //
// /// Device Manager Tests
// //
//
// describe("device manager", []() {
//     DebugL << "Starting" << endl;
//     auto& deviceManager = av::MediaFactory::instance().devices();
//
//     av::Device device;
//     if (deviceManager.getDefaultCamera(device)) {
//         DebugA("Default video device: ", device.id, ": ", device.name)
//     }
//     if (deviceManager.getDefaultMicrophone(device)) {
//         DebugA("Default audio device: ", device.id, ": ", device.name)
//     }
//
//     std::vector<av::Device> devices;
//     if (deviceManager.getCameras(devices)) {
//         DebugA("Num video devices: ", devices.size())
//         for (auto& device : devices) {
//             DebugL << "Printing video device: " << device.id << ": " <<
//             device.name << endl;
//         }
//     }
//     else {
//         DebugL << "No video devices detected!" << endl;
//     }
//     if (deviceManager.getMicrophones(devices)) {
//         DebugA("Num audio devices: ", devices.size())
//         for (auto& device : devices) {
//             DebugL << "Printing audio device: " << device.id << ": " <<
//             device.name << endl;
//         }
//     }
//     else {
//         DebugL << "No video devices detected!" << endl;
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
