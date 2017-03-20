///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/application.h"
#include "scy/av/audiocapture.h"
#include "scy/av/devicemanager.h"
#include "scy/av/multiplexpacketencoder.h"
#include "scy/av/videocapture.h"
#include "scy/packetstream.h"


// This demo showcases how to implement a H.264 multiplex recorder from realtime
// device captures using LibSourcey.

using namespace scy;
using std::endl;

#define OUTPUT_FILENAME "deviceoutput.mp4"
#define OUTPUT_FORMAT av::Format("MP4", "mp4", \
    av::VideoCodec("H.264", "libx264", 400, 300, 25, 48000, 128000, "yuv420p"), \
    av::AudioCodec("AAC", "libfdk_aac", 2, 44100, 64000, "s16"));


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace)); // Debug

    {
        // Create a PacketStream to pass packets
        // from device captures to the encoder
        PacketStream stream;

        av::EncoderOptions options;
        options.ofile = OUTPUT_FILENAME;
        options.oformat = OUTPUT_FORMAT;

        // Create a device manager instance to enumerate system devices
        av::Device device;
        av::DeviceManager devman;

        // Create and attach the default video capture
        av::VideoCapture video;
        if (devman.getDefaultCamera(device)) {
            InfoL << "Using video device: " << device.name << endl;
            video.open(device.id, 640, 480, 30);
            video.getEncoderFormat(options.iformat);
            stream.attachSource(&video, false, true);
        }

        // Create and attach the default audio capture
        av::AudioCapture audio;
        if (devman.getDefaultMicrophone(device)) {
            InfoL << "Using audio device: " << device.name << endl;
            audio.open(device.id, 2, 44100);
            audio.getEncoderFormat(options.iformat);
            stream.attachSource(&audio, false, true);
        }

        // Create and attach the multiplex encoder
        av::MultiplexPacketEncoder encoder(options);
        encoder.initialize();
        stream.attach(&encoder, 5, false);

        // Start the stream
        stream.start();

        // Keep recording until Ctrl-C is pressed
        InfoL << "Recording video: " << OUTPUT_FILENAME << endl;
        uv::waitForShutdown([](void* opaque) {
            reinterpret_cast<PacketStream*>(opaque)->stop();
        }, &stream);
    }

    // av::DeviceManager::shutdown();
    // Logger::destroy();
    return 0;
}
