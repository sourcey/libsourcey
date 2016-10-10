#include "scy/application.h"
#include "scy/packetstream.h"
#include "scy/av/iencoder.h"
#include "scy/av/avpacketencoder.h"
#include "scy/av/avinputreader.h"
#include "scy/av/mediafactory.h"
#include "scy/av/videocapture.h"
#include "scy/av/audiocapture.h"

using namespace std;
using namespace scy;

#define USE_AVDEVICE_CAPTURE 0
#define VIDEO_FILE_SOURCE "test.mp4"


namespace scy {


class DeviceRecorder: public Application
{
public:
    DeviceRecorder(av::VideoCapture::Ptr video, av::AudioCapture::Ptr audio)
    {
        DebugL << "Creating" << endl;

        av::EncoderOptions options;
        options.ofile = "devicerecorder.mp4";
        options.oformat = av::Format("MP4", "mp4",
            av::VideoCodec("H.264", "libx264", 400, 300, 25, 48000, 128000, "yuv420p"),
            av::AudioCodec("AAC", "aac", 2, 44100, 64000, "fltp"));

        // Attach the capture sources
        if (video) {
            video->getEncoderFormat(options.iformat);
            stream.attachSource(video, true);
        }
        else options.oformat.video.enabled = false;

        if (audio) {
            audio->getEncoderFormat(options.iformat);
            stream.attachSource(audio, true);
        }
        else options.oformat.audio.enabled = false;

        // Create and attach the encoder
        auto encoder = new av::AVPacketEncoder(options);
        encoder->initialize();
        stream.attach(encoder, 5, true);
    }

    ~DeviceRecorder()
    {
        DebugL << "Destroying" << endl;
        stop();
    }

    void start()
    {
        DebugL << "Start" << endl;
        stream.start();
    }

    void stop()
    {
        DebugL << "Stop" << endl;
        stream.stop();
    }

    PacketStream stream;
};


} // namespace scy


// static void onShutdownSignal(void* opaque)
// {
//     reinterpret_cast<http::Server*>(opaque)->shutdown();
// }


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));

// #if USE_AVDEVICE_CAPTURE
//     auto video = new av::AVInputReader();
//     video->openDevice(0);
//     // video->openFile(VIDEO_FILE_SOURCE);
//     video->start();
// #else
//     // VideoCapture instances should be instantiated in the main thread.
//
//     auto video = new av::VideoCapture(0);
// #endif

    {
        auto& media = av::MediaFactory::instance();
        av::VideoCapture::Ptr video = nullptr; //media.createVideoCapture(0);
        av::AudioCapture::Ptr audio = media.createAudioCapture(0, 2, 44100); //nullptr; //

        DeviceRecorder rec(video, audio);
        rec.start();

    		// Run until shutdown signal
    		rec.waitForShutdown([](void* opaque) {
    		    reinterpret_cast<DeviceRecorder*>(opaque)->stop();
    		}, &rec);
    }

    // delete video;
    Logger::destroy();
    return 0;
}
