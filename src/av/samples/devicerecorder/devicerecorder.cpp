#include "scy/application.h"
#include "scy/packetstream.h"
#include "scy/av/iencoder.h"
#include "scy/av/avpacketencoder.h"
#include "scy/av/avinputreader.h"
#include "scy/av/mediafactory.h"
#include "scy/av/videocapture.h"
#include "scy/av/audiocapture.h"


// This demo showcases how to implement a H.264 multiplex recorder from realtime
// device capture sources.

#define VIDEO_DEVICE_ID 0
#define AUDIO_DEVICE_ID 0
#define OUTPUT_FILENAME "deviceoutput.mp4"
#define OUTPUT_FORMAT av::Format("MP4", "mp4", \
    av::VideoCodec("H.264", "libx264", 400, 300, 25, 48000, 128000, "yuv420p"), \
    av::AudioCodec("AAC", "libfdk_aac", 2, 44100, 64000, "s16"));


using namespace scy;
using std::endl;

namespace scy {


class DeviceRecorder: public Application
{
public:
    DeviceRecorder(av::VideoCapture::Ptr video, av::AudioCapture::Ptr audio)
    {
        DebugL << "Creating" << endl;

        av::EncoderOptions options;
        options.ofile = OUTPUT_FILENAME;
        options.oformat = OUTPUT_FORMAT;

        // Attach the video capture source
        if (video) {
            video->getEncoderFormat(options.iformat);
            stream.attachSource(video, true);
        }
        else options.oformat.video.enabled = false;

        // Attach the audio capture source
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


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));

    {
        av::VideoCapture::Ptr video(std::make_shared<av::VideoCapture>(VIDEO_DEVICE_ID));
        av::AudioCapture::Ptr audio(std::make_shared<av::AudioCapture>(AUDIO_DEVICE_ID, 2, 44100));

        DeviceRecorder rec(video, audio);
        rec.start();

    		// Run until shutdown signal
    		rec.waitForShutdown([](void* opaque) {
    		    reinterpret_cast<DeviceRecorder*>(opaque)->stop();
    		}, &rec);
    }

    Logger::destroy();
    return 0;
}
