//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include "scy/application.h"
#include "scy/packetstream.h"
#include "scy/av/avpacketencoder.h"
#include "scy/av/avcapture.h"
#include "scy/av/devicemanager.h"


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
    Logger::instance().add(new ConsoleChannel("debug", LTrace));

    {
        // Create a PacketStream to pass packets
        // from device captures to the encoder
        PacketStream stream;

        av::Device device;
        av::EncoderOptions options;
        options.ofile = OUTPUT_FILENAME;
        options.oformat = OUTPUT_FORMAT;

        // Create and attach the default video capture
        av::AVCapture video;
        if (av::DeviceManager::instance().getDefaultCamera(device)) {
            InfoL << "Using video device: " << device.name << endl;
            video.openCamera(device.id, 640, 480, 30);
            video.getEncoderFormat(options.iformat);
            stream.attachSource(&video, false, true);
        }

        // Create and attach the default audio capture
        av::AVCapture audio;
        if (av::DeviceManager::instance().getDefaultMicrophone(device)) {
            InfoL << "Using audio device: " << device.name << endl;
            audio.openMicrophone(device.id, 2, 44100);
            audio.getEncoderFormat(options.iformat);
            stream.attachSource(&audio, false, true);
        }

        // Create and attach the multiplex encoder
        av::AVPacketEncoder encoder(options);
        encoder.initialize();
        stream.attach(&encoder, 5, false);

        // Start the stream
        stream.start();

        // Keep recording until Ctrl-C is pressed
        uv::waitForShutdown([](void* opaque) {
    		    reinterpret_cast<PacketStream*>(opaque)->stop();
    		}, &stream);
    }

    av::DeviceManager::shutdown();
    Logger::destroy();
    return 0;
}
