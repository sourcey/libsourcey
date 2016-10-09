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


#ifndef SCY_Media_Tests_H
#define SCY_Media_Tests_H


#include "scy/base.h"
#include "scy/test.h"
#include "scy/logger.h"
// #include "scy/signal.h"
// #include "scy/queue.h"
// #include "scy/packetqueue.h"
#include "scy/av/devicemanager.h"
// #include "scy/av/flvmetadatainjector.h"
// #include "scy/av/formatregistry.h"
#include "scy/av/mediafactory.h"
// #include "scy/av/avinputreader.h"
// #include "scy/av/avpacketencoder.h"
// #include "scy/av/thumbnailer.h"
// #include "scy/av/iencoder.h"
#include "scy/av/videocapture.h"
#include "scy/av/audiocapture.h"
#include "scy/av/audiocontext.h"
#include "scy/av/audioresampler.h"


using std::cout;
using std::cerr;
using std::endl;
using scy::test::Test;


namespace scy {


//
/// Generic Callback Context
//

struct CallbackContext
{
    // ---------------------------------------------------------------------
    // Video Capture Test
    //

    void onVideoCaptureFrame(void* sender, av::MatrixPacket& packet)
    {
        DebugL << "On packet: " << packet.size() << endl;

        // cv::imshow("VideoCaptureTest", *packet.mat);
    }
};


//
// Heleprs
//


// Generate sin tone with 440Hz frequency and duplicated channels
void fillAudioSamplesUInt(std::uint16_t* samples, int sample_rate, int nb_channels, int nb_samples, double *t)
{
    int j, k;
    float tincr = 2 * M_PI * 440.0 / sample_rate;
    for (j = 0; j < nb_samples; j++) {
        samples[2*j] = (int)(sin(*t) * 10000);
        for (k = 1; k < nb_channels; k++)
            samples[2*j + k] = samples[2*j];
        *t += tincr;
    }
}


// Generate sin tone with 440Hz frequency and duplicated channels
void fillAudioSamples(double* samples, int sample_rate, int nb_channels, int nb_samples, double *t)
{
    int i, j;
    double tincr = 1.0 / sample_rate, *dstp = samples;
    const double c = 2 * M_PI * 440.0;
    for (i = 0; i < nb_samples; i++) {
        *dstp = sin(c * *t);
        for (j = 1; j < nb_channels; j++)
            dstp[j] = dstp[0];
        dstp += nb_channels;
        *t += tincr;
    }
}


std::vector<std::uint16_t*> createTestAudioSamplesS16(int numFrames, int nbSamples, const av::AudioCodec& params) //const char* sampleFmt, int sampleRate, int channels, int frameSize
{
    double t = 0;
    int bufferSize = av_samples_get_buffer_size(nullptr, params.channels, nbSamples, av_get_sample_fmt(params.sampleFmt.c_str()), 0);
    auto vec = std::vector<std::uint16_t*>();
    do {
        auto samples = new std::uint16_t[bufferSize];
        fillAudioSamplesUInt(samples, params.sampleRate, params.channels, nbSamples, &t);
        vec.push_back(samples);
    } while (--numFrames);
    return vec;
}


std::vector<double*> createTestAudioSamplesDBL(int numFrames, int nbSamples, const av::AudioCodec& params) //const char* sampleFmt, int sampleRate, int channels, int frameSize
{
    double t = 0;
    int bufferSize = av_samples_get_buffer_size(nullptr, params.channels, nbSamples, av_get_sample_fmt(params.sampleFmt.c_str()), 0);
    auto vec = std::vector<double*>();
    do {
        auto samples = new double[bufferSize];
        fillAudioSamples(samples, params.sampleRate, params.channels, nbSamples, &t);
        vec.push_back(samples);
    } while (--numFrames);
    return vec;
}


// =============================================================================
// Audio Encoder
//
class AudioEncoderTest: public Test
{
    void run()
    {
        int numberFramesWanted = 200;
        int inNbSamples = 1024;

        av::AudioEncoderContext encoder;
        av::AudioCodec iparams;
        av::AudioCodec oparams;

        iparams.channels = 2;
        iparams.sampleRate = 48000;
        iparams.sampleFmt = "dbl";

        oparams.encoder = "mp2";
        oparams.bitRate = 64000;
        oparams.channels = 2;
        oparams.sampleRate = 44100;
        oparams.sampleFmt = "s16";
        oparams.enabled = true;

        encoder.iparams = iparams;
        encoder.oparams = oparams;
        encoder.create();
        encoder.open();

        std::ofstream output("test.mp2", std::ios::out | std::ios::binary);

        auto testSamples = createTestAudioSamplesDBL(numberFramesWanted, inNbSamples, iparams);
        for (auto samples : testSamples) {
            AVPacket opacket;
            if (encoder.encode(reinterpret_cast<std::uint8_t*>(samples), inNbSamples, AV_NOPTS_VALUE, opacket)) {
                output.write(reinterpret_cast<const char*>(opacket.data), opacket.size);
            }
        }

        encoder.close();
        output.close();
        util::clearVector<>(testSamples);

        // TODO: verify data integrity
    }
};


// =============================================================================
// Audio Resampler
//
class AudioResamplerTest: public Test
{
    void run()
    {
        int numberFramesWanted = 200;
        int inNbSamples = 1024;

        av::AudioResampler resampler;
        av::AudioCodec iparams;
        av::AudioCodec oparams;

        iparams.channels = 2;
        iparams.sampleRate = 48000;
        iparams.sampleFmt = "dbl";

        oparams.channels = 2;
        oparams.sampleRate = 44100;
        oparams.sampleFmt = "s16";

        resampler.iparams = iparams;
        resampler.oparams = oparams;
        resampler.create();

        std::ofstream output("test.pcm", std::ios::out | std::ios::binary);

        auto testSamples = createTestAudioSamplesDBL(numberFramesWanted, inNbSamples, iparams);
        for (auto samples : testSamples) {
            if (resampler.resample(reinterpret_cast<std::uint8_t*>(samples), inNbSamples)) {
                output.write(reinterpret_cast<const char*>(resampler.outSamples[0]), resampler.outSamplesBytes);
            }
        }

        resampler.close();
        output.close();
        util::clearVector<>(testSamples);

        // The file is playable with the following command:
        // ffplay -f s16le -channel_layout 3 -channels 2 -ar 44100 test.pcm

        // TODO: verify data integrity
    }
};



// class Tests
// {
// public:
//     Application& app;
//
//     Tests(Application& app) : app(app)
//     {
//         DebugL << "Running tests..." << endl;
//
//         try {
//
//             runAVInputReaderRecorderTest();
// #if 0
//             runAudioRecorderTest();
//             testVideoThumbnailer();
//             runVideoCaptureThreadTest();
//             testVideoCapture();
//             testVideoCaptureStream();
//
//             MediaFactory::instance().devices().print(cout);
//             audioCapture = new AudioCapture(device.id, 2, 44100);
//
//             videoCapture = new VideoCapture(0, true);
//             audioCapture = new AudioCapture(0, 1, 16000);
//             audioCapture = new AudioCapture(0, 1, 11025);
//             for (int i = 0; i < 100; i++) { //0000000000
//                 runAudioCaptureTest();
//             }
//
//             testDeviceManager();
//             runAudioCaptureThreadTest();
//             runAudioCaptureTest();
//             runOpenCVMJPEGTest();
//             runVideoRecorderTest();
//             runCaptureRecorderTest();
//             runAVEncoderTest();
//             runStreamEncoderTest();
//             runMediaSocketTest();
//             runFormatFactoryTest();
//             runMotionDetectorTest();
//             runBuildXMLString();
//
//             runStreamEncoderTest();
//             runOpenCVCaptureTest();
//             runDirectShowCaptureTest();
// #endif
//         }
//         catch (std::exception& exc) {
//             ErrorL << "Error: " << exc.what() << endl;
//         }
//     };
//
//
//
//
//     // ---------------------------------------------------------------------
//     // Video Capture Test
//     //
//     void onVideoCaptureStreamFrame(void* sender, av::MatrixPacket& packet)
//     {
//         DebugL << "On stream packet: " << packet.size() << endl;
//     }
//
//
//     // ---------------------------------------------------------------------
//     // Video Capture Thread Test
//     //
//     class VideoCaptureThread: public async::Runnable
//     {
//     public:
//         VideoCaptureThread(int deviceID, const std::string& name = "Capture Thread") :
//             _deviceID(deviceID),
//             _name(name),
//             frames(0),
//             closed(false)
//         {
//             //_thread.setName(name);
//             _thread.start(*this);
//         }
//
//         VideoCaptureThread(const std::string& filename, const std::string& name = "Capture Thread") :
//             _deviceID(0),
//             _filename(filename),
//             _name(name),
//             frames(0),
//             closed(false)
//         {
//             //_thread.setName(name);
//             _thread.start(*this);
//         }
//
//         ~VideoCaptureThread()
//         {
//             closed = true;
//             _thread.join();
//         }
//
//         void run()
//         {
// #if 0
//             VideoCapture::Ptr capture = !_filename.empty() ?
//                 MediaFactory::instance().createFileCapture(_filename) :
//                 MediaFactory::instance().createVideoCapture(_deviceID);
// #endif
//
//             // Initialize the VideoCapture inside the thread context.
//             // This will fail in windows unless the VideoCapture was
//             // previously initialized in the main thread.
//             VideoCapture::Ptr capture(!_filename.empty() ?
//                 new VideoCapture(_filename) :
//                 new VideoCapture(_deviceID));
//
//             capture->emitter += packetDelegate(this, &VideoCaptureThread::onVideo);
//             capture->start();
//
//             while (!closed) {
//                 cv::waitKey(5);
//             }
//
//             capture->emitter -= packetDelegate(this, &VideoCaptureThread::onVideo);
//             capture->stop();
//
//             DebugL << " Ended.................." << endl;
//         }
//
//         void onVideo(void* sender, MatrixPacket& packet)
//         {
//             DebugL << "On thread frame: " << packet.size() << endl;
//             cv::imshow(_name, *packet.mat);
//             frames++;
//         }
//
//         Thread _thread;
//         int _deviceID;
//         std::string _filename;
//         std::string _name;
//         int frames;
//         bool closed;
//     };
//
//
//     void runVideoCaptureThreadTest()
//     {
//         DebugL << "Running video capture test..." << endl;
//
//
//         // start and destroy multiple receivers
//         std::list<VideoCaptureThread*> threads;
//
//         try {
//             for (int i = 0; i < 3; i++) {
//                 threads.push_back(new VideoCaptureThread(0));
//             }
//
//             DebugL << " Ending.................." << endl;
//         }
//         catch (std::exception& exc) {
//             ErrorL << "[VideoCaptureThread] Error: " << exc.what() << endl;
//         }
//
//         std::puts("Press any key to continue...");
//         std::getchar();
//         //scy::pause();
//
//         util::clearList(threads);
//     }
//
//
//     // ---------------------------------------------------------------------
//     // Video Thembnailer Test
//     //
//     void testVideoThumbnailer()
//     {
//         DebugL << "Starting" << endl;
//
//         try {
//             assert(0 && "fixme");
//             //Thumbnailer thumb;
//             //thumb.init("D://test1.mp4", "resto.jpg", 0, 0, 3.0);
//             //thumb.grab();
//         }
//         catch (std::exception& exc) {
//             ErrorL << "VideoThumbnailer: " << exc.what() << endl;
//         }
//         catch (...) {
//             ErrorL << "VideoThumbnailer: Unknown error" << endl;
//         }
//
//         DebugL << "Complete" << endl;
//     }
//
//
// #if 0
//     bool PullFrame(const std::string& capturedUrl)
// {
//     AVCodec* pMJPEGCodec  = avcodec_find_encoder(AV_CODEC_ID_MJPEG );
//
//     int videoStream   = -1;
//
//     AVDictionary *optionsDict = NULL;
//     AVInputFormat   *pFormat = NULL;
//     const char      formatName[] = "mp4";
//
//     if (!(pFormat = av_find_input_format(formatName)))
//     {
//         std::cout << "can't find input format " << formatName << "\n";
//         return false;
//     }
//
//     AVFormatContextHandle FormatCtx(avformat_alloc_context());
//
//     if(!FormatCtx.is_valid())
//     {
//         std::cout << "\n NULL CONTEXT \n ";
//         return false;
//     }
//
//     if(avformat_open_input (&FormatCtx, capturedUrl.c_str(), pFormat, NULL))
//         return false;
//
//     for(int i=0; i<(int)FormatCtx->nb_streams; i++)
//     {
//         if(FormatCtx->streams[i]->codeencoder.ctx->codec_type==AVMEDIA_TYPE_VIDEO)
//         {
//             videoStream=i;
//             break;
//         }
//     }
//     if(videoStream < 0 )
//         return false;
//
//     CodecContextHandle CodecCtx(FormatCtx->streams[videoStream]->codec, avcodec_close);
//     AVCodec *pCodec = avcodec_find_decoder(CodecCtx->codec_id);
//
//     if(pCodec == NULL)
//         return false;
//
//     if( avcodec_open2(CodecCtx, pCodec, &optionsDict) < 0 )
//         return false;
//
//     FrameHandle Frame(av_frame_alloc(), av_free);
//
//     if(!Frame.is_valid())
//         return false;
//
//     int frameFinished=0;
//     AVPacket packet;
//
//     while(av_read_frame(FormatCtx, &packet)>=0)
//     {
//         if(packet.stream_index==videoStream)
//         {
//             avcodec_decode_video2(CodecCtx, Frame, &frameFinished, &packet);
//
//             if(frameFinished)
//             {
//                 std::string uu (capturedUrl);
//                 std::size_t pos = capturedUrl.rfind(".mp4");
//                 uu.replace(pos, 4, "thumbnail.jpg");
//                 // save the frame to file
//                 int Bytes = avpicture_get_size(AV_PIX_FMT_YUVJ420P, CodecCtx->width, CodecCtx->height);
//                 BufferHandle buffer((uint8_t*)av_malloc(Bytes*sizeof(uint8_t)), av_free);
//                 CodecContextHandle OutContext(avcodec_alloc_context3(NULL), free_context);
//                 OutContext->bit_rate = CodecCtx->bit_rate;
//                 OutContext->width = CodecCtx->width;
//                 OutContext->height = CodecCtx->height;
//                 OutContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
//                 OutContext->codec_id = AV_CODEC_ID_MJPEG;
//                 OutContext->codec_type = AVMEDIA_TYPE_VIDEO;
//                 OutContext->time_base.num = CodecCtx->time_base.num;
//                 OutContext->time_base.den = CodecCtx->time_base.den;
//                 AVCodec *OutCodec = avcodec_find_encoder(OutContext->codec_id);
//                 avcodec_open2(OutContext, OutCodec, NULL);
//                 OutContext->mb_lmin = OutContext->lmin = OutContext->qmin * 118;
//                 OutContext->mb_lmax = OutContext->lmax = OutContext->qmax * 118;
//                 OutContext->flags = 2;
//                 OutContext->global_quality = OutContext->qmin * 118;
//
//                 Frame->pts = 1;
//                 Frame->quality = OutContext->global_quality;
//
//                 int ActualSize = avcodec_encode_video(OutContext, buffer, Bytes, Frame);
//                 std::ofstream file("c:\\temp\\output.jpg", std::ios_base::binary | std::ios_base::out);
//                 file.write((const char*)(uint8_t*)buffer, ActualSize);
//             }
//             if(CodecCtx->refcounted_frames == 1)
//                 av_frame_unref(Frame);
//         }
//         av_free_packet(&packet);
//     }
//
//     return true;
// }
//
//
//     // ---------------------------------------------------------------------
//     // Packet Stream Encoder Test
//     //
//     static bool stopStreamEncoders;
//
//     class StreamEncoderTest
//     {
//     public:
//         StreamEncoderTest(const av::EncoderOptions& opts) :
//             closed(false), options(opts), frames(0), videoCapture(nullptr), audioCapture(nullptr)
//         {
//             //ofile.open("enctest1.mp4", ios::out | ios::binary);
//             //assert(ofile.is_open());
//
//             // Synchronize events and packet output with the default loop
//             //stream.synchronizeOutput(uv::defaultLoop());
//             //stream.setAsyncContext(std::make_shared<Idler>(uv::defaultLoop()));
//
//             // Init captures
//             if (options.oformat.video.enabled) {
//                 DebugL << "Video device: " << 0 << endl;
//                 videoCapture = MediaFactory::instance().createVideoCapture(0); //0
//                 //videoCapture = MediaFactory::instance().createFileCapture("D:/dev/lib/ffmpeg/bin/channel1.avi"); //0
//                 //videoCapture->emitter += packetDelegate(this, &StreamEncoderTest::onVideoCapture);
//                 videoCapture->getEncoderFormat(options.iformat);
//                 stream.attachSource(videoCapture, true, true); //
//
//                 //options.iformat.video.pixelFmt = "yuv420p";
//             }
//             if (options.oformat.audio.enabled) {
//                 Device device;
//                 if (MediaFactory::instance().devices().getDefaultAudioInputDevice(device)) {
//                     DebugL << "Audio device: " << device.id << endl;
//                     audioCapture = MediaFactory::instance().createAudioCapture(device.id,
//                         options.oformat.audio.channels,
//                         options.oformat.audio.sampleRate);
//                     stream.attachSource(audioCapture, true, true);
//                 }
//                 else assert(0);
//             }
//
//             // Init as async queue for testing
//             //stream.attach(new FPSLimiter(5), 4, true);
//             //stream.attach(new AsyncPacketQueue, 2, true);
//             //stream.attach(new AsyncPacketQueue, 3, true);
//             //stream.attach(new AsyncPacketQueue, 4, true);
//
//             // Init encoder
//             encoder = new AVPacketEncoder(options,
//                 options.oformat.video.enabled &&
//                 options.oformat.audio.enabled);
//             stream.attach(encoder, 5, true);
//
//             // Start the stream
//             stream.emitter += packetDelegate(this, &StreamEncoderTest::onVideoEncoded);
//             stream.StateChange += sdelegate(this, &StreamEncoderTest::onStreamStateChange);
//             stream.start();
//         }
//
//         virtual ~StreamEncoderTest()
//         {
//             DebugL << "Destroying" << endl;
//             close();
//             DebugL << "Destroying: OK" << endl;
//         }
//
//         void close()
//         {
//             DebugL << "########### Closing: " << frames << endl;
//             closed = true;
//
//             // Close the stream
//             // This will flush any queued items
//             //stream.stop();
//             //stream.waitForSync();
//             stream.close();
//
//             // Make sure everything shutdown properly
//             //assert(stream.queue().empty());
//             //assert(encoder->isStopped());
//
//             // Close the output file
//             //ofile.close();
//         }
//
//         void onStreamStateChange(void* sender, PacketStreamState& state, const PacketStreamState& oldState)
//         {
//             DebugL << "########### On stream state change: " << oldState << " => " << state << endl;
//         }
//
//         void onVideoEncoded(void* sender, RawPacket& packet)
//         {
//             DebugL << "########### On packet: " << closed << ":" << packet.size() << endl;
//             frames++;
//             //assert(!closed);
//             assert(packet.data());
//             assert(packet.size());
//
//             // Do not call stream::close from inside callback
//             //ofile.write(packet.data(), packet.size());
//             //assert(frames <= 3);
//             //if (frames == 20)
//             //    close();
//         }
//
//         void onVideoCapture(void* sender, av::MatrixPacket& packet)
//         {
//             DebugL << "On packet: " << packet.size() << endl;
//
//             //cv::imshow("StreamEncoderTest", *packet.mat);
//         }
//
//         int frames;
//         bool closed;
//         PacketStream stream;
//         VideoCapture::Ptr videoCapture;
//         AudioCapture::Ptr audioCapture;
//         //AsyncPacketQueue* queue;
//         AVPacketEncoder* encoder;
//         av::EncoderOptions options;
//         //std::ofstream ofile;
//     };
//
//     static void onShutdownSignal(void* opaque)
//     {
//         auto& tests = *reinterpret_cast<std::vector<StreamEncoderTest*>*>(opaque);
//
//         stopStreamEncoders = true;
//
//         for (unsigned i = 0; i < tests.size(); i++) {
//             // Delete the pointer directly to
//             // ensure synchronization is golden.
//             delete tests[i];
//         }
//     }
//
//     void runStreamEncoderTest()
//     {
//         DebugL << "Running" << endl;
//         try
//         {
//             // Setup encoding format
//             Format mp4(Format("MP4", "mp4",
//                 VideoCodec("MPEG4", "mpeg4", 640, 480, 60),
//                 //VideoCodec("H264", "libx264", 640, 480, 20)//,
//                 //AudioCodec("AAC", "aac", 2, 44100)
//                 //AudioCodec("MP3", "libmp3lame", 1, 8000, 64000)
//                 //AudioCodec("MP3", "libmp3lame", 2, 44100, 64000)
//                 AudioCodec("AC3", "ac3_fixed", 2, 44100, 64000)
//             ));
//
//             Format mp3("MP3", "mp3",
//                 AudioCodec("MP3", "libmp3lame", 1, 8000, 64000));
//
//             //stopStreamEncoders = false;
//
//             av::EncoderOptions options;
//             //options.ofile = "enctest.mp3";
//             options.ofile = "itsanewday.mp4";
//             //options.ofile = "enctest.mjpeg";
//             options.oformat = mp4;
//
//             // Initialize test runners
//             int numTests = 1;
//             std::vector<StreamEncoderTest*> threads;
//             for (unsigned i = 0; i < numTests; i++) {
//                 threads.push_back(new StreamEncoderTest(options));
//             }
//
//             // Run until Ctrl-C is pressed
//             //Application app;
//             app.waitForShutdown(); //&threadsnullptr, nullptr
//
//             for (unsigned i = 0; i < threads.size(); i++) {
//                 // Delete the pointer directly to
//                 // ensure synchronization is golden.
//                 delete threads[i];
//             }
//
//             // Shutdown the garbage collector so we can free memory.
//             //GarbageCollector::instance().shutdown();
//             //DebugL << "#################### Finalizing" << endl;
//             //GarbageCollector::instance().shutdown();
//             //DebugL << "#################### Exiting" << endl;
//
//             //DebugL << "#################### Finalizing" << endl;
//             //app.cleanup();
//             //DebugL << "#################### Exiting" << endl;
//
//             // Wait for enter keypress
//             //scy::pause();
//
//             // Finalize the application to free all memory
//             // Note: 2 tiny mem leaks (964 bytes) are from OpenCV
//             //app.finalize();
//         }
//         catch (std::exception& exc)
//         {
//             ErrorL << "Error: " << exc.what() << endl;
//             assert(0);
//         }
//
//         DebugL << "Ended" << endl;
//     }
//
//
//     void runCaptureRecorderTest()
//     {
//         DebugL << "Starting" << endl;
//
//         /*
//         av::VideoCapture capture(0);
//
//         // Setup encoding format
//         Format mp4(Format("MP4", "mp4",
//             VideoCodec("MPEG4", "mpeg4", 640, 480, 10)//,
//             //VideoCodec("H264", "libx264", 320, 240, 25),
//             //AudioCodec("AAC", "aac", 2, 44100)
//             //AudioCodec("MP3", "libmp3lame", 2, 44100, 64000)
//             //AudioCodec("AC3", "ac3_fixed", 2, 44100, 64000)
//         ));
//
//         av::EncoderOptions options;
//         options.ofile = "enctest.mp4"; // enctest.mjpeg
//         options.oformat = mp4;
//         setVideoCaptureInputFormat(&capture, options.iformat);
//
//         CaptureRecorder encoder(&capture, options);
//         encoder.start();
//
//         std::puts("Press any key to continue...");
//         std::getchar();
//
//         encoder.stop();
//         */
//
//         DebugL << "Complete" << endl;
//     }
//
//
//     // ---------------------------------------------------------------------
//     // OpenCV Capture Test
//     //
//     void runOpenCVCaptureTest()
//     {
//         DebugL << "Starting" << endl;
//
//         cv::VideoCapture cap(0);
//         if (!cap.isOpened())
//             assert(false);
//
//         cv::Mat edges;
//         cv::namedWindow("edges",1);
//         for(;;) {
//             cv::Mat frame;
//             cap >> frame; // get a new frame from camera
//             cv::cvtColor(frame, edges, CV_BGR2GRAY);
//             cv::GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
//             cv::Canny(edges, edges, 0, 30, 3);
//             cv::imshow("edges", edges);
//             if (cv::waitKey(30) >= 0) break;
//         }
//
//         DebugL << "Complete" << endl;
//     }
//
//
//     // ---------------------------------------------------------------------
//     // Audio Capture Thread Test
//     //
//     class AudioCaptureThread: public basic::Runnable
//     {
//     public:
//         AudioCaptureThread(const std::string& name = "Capture Thread")
//         {
//             _thread.setName(name);
//             _thread.start(*this);
//         }
//
//         ~AudioCaptureThread()
//         {
//             _wakeUp.set();
//             _thread.join();
//         }
//
//         void run()
//         {
//             try
//             {
//                 //capture = new AudioCapture(0, 2, 44100);
//                 //capture = new AudioCapture(0, 1, 16000);
//                 //capture = new AudioCapture(0, 1, 11025);
//                 AudioCapture* capture = new AudioCapture(0, 1, 16000);
//                 capture->attach(audioDelegate(this, &AudioCaptureThread::onAudio));
//
//                 _wakeUp.wait();
//
//                 capture->detach(audioDelegate(this, &AudioCaptureThread::onAudio));
//                 delete capture;
//
//                 DebugL << "[AudioCaptureThread] Ending.................." << endl;
//             }
//             catch (std::exception& exc)
//             {
//                 ErrorL << "[AudioCaptureThread] Error: " << exc.what() << endl;
//             }
//
//             DebugL << "[AudioCaptureThread] Ended.................." << endl;
//             //delete this;
//         }
//
//         void onAudio(void* sender, AudioPacket& packet)
//         {
//             DebugL << "[AudioCaptureThread] On Packet: " << packet.size() << endl;
//             //cv::imshow(_thread.name(), *packet.mat);
//         }
//
//         Thread    _thread;
//         Poco::Event        _wakeUp;
//         int                frames;
//     };
//
//
//     void runAudioCaptureThreadTest()
//     {
//         DebugL << "Running Audio Capture Thread test..." << endl;
//
//         // start and destroy multiple receivers
//         list<AudioCaptureThread*> threads;
//         for (int i = 0; i < 10; i++) { //0000000000
//             threads.push_back(new AudioCaptureThread()); //Poco::format("Audio Capture Thread %d", i))
//         }
//
//         //scy::pause();
//
//         util::clearList(threads);
//     }
//
//
//     // ---------------------------------------------------------------------
//     // Audio Capture Test
//     //
//     void onCaptureTestAudioCapture(void*, AudioPacket& packet)
//     {
//         DebugL << "onCaptureTestAudioCapture: " << packet.size() << endl;
//         //cv::imshow("Target", *packet.mat);
//     }
//
//     void runAudioCaptureTest()
//     {
//         DebugL << "Running Audio Capture test..." << endl;
//
//         AudioCapture* capture = new AudioCapture(0, 1, 16000);
//         capture->attach(packetDelegate(this, &Tests::onCaptureTestAudioCapture));
//         //scy::pause();
//         capture->detach(packetDelegate(this, &Tests::onCaptureTestAudioCapture));
//         delete capture;
//
//         AudioCapture* capture = new AudioCapture(0, 1, 16000);
//         capture->attach(packetDelegate(this, &Tests::onCaptureTestAudioCapture));
//         //audioCapture->start();
//
//         //scy::pause();
//
//         capture->detach(packetDelegate(this, &Tests::onCaptureTestAudioCapture));
//         delete capture;
//         //audioCapture->stop();
//     }
//
//
//     // ---------------------------------------------------------------------
//     //
//     // Video Media Socket Test
//     //
//     // ---------------------------------------------------------------------
//
//     class MediaConnection: public TCPServerConnection
//     {
//     public:
//         MediaConnection(const StreamSocket& s) :
//           TCPServerConnection(s), stop(false)//, lastTimestamp(0), timestampGap(0), waitForKeyFrame(true)
//         {
//         }
//
//         void run()
//         {
//             try
//             {
//                 av::EncoderOptions options;
//                 //options.ofile = "enctest.mp4";
//                 //options.stopAt = time(0) + 3;
//                 av::setVideoCaptureInputForma(videoCapture, options.iformat);
//                 //options.oformat = Format("MJPEG", "mjpeg", VideoCodec(Codec::MJPEG, "MJPEG", 1024, 768, 25));
//                 //options.oformat = Format("FLV", "flv", VideoCodec(Codec::H264, "H264", 400, 300, 25));
//                 options.oformat = Format("FLV", "flv", VideoCodec(Codec::FLV, "FLV", 640, 480, 100));
//                 //options.oformat = Format("FLV", "flv", VideoCodec(Codec::FLV, "FLV", 320, 240, 15));
//                 //options.oformat = Format("FLV", "flv", VideoCodec(Codec::H264, "H264", 400, 300, 25));
//
//
//                 //options.iformat.video.pixfmt = (scy::av::PixelFormat::ID)AV_PIX_FMT_GRAY8; //AV_PIX_FMT_BGR8; //AV_PIX_FMT_BGR32 // AV_PIX_FMT_BGR32
//                 //MotionDetector* detector = new MotionDetector();
//                 //detector->setVideoCapture(videoCapture);
//                 //stream.attach(detector, true);
//                 //stream.attach(new SurveillanceMJPEGPacketizer(*detector), 20, true);
//
//                 stream.attach(videoCapture, false);
//
//                 stream.attach(packetDelegate(this, &MediaConnection::onVideoEncoded));
//
//                 // init encoder
//                 AVEncoder* encoder = new AVEncoder();
//                 encoder->setParams(options);
//                 encoder->initialize();
//                 stream.attach(encoder, 5, true);
//
//                 //HTTPMultipartAdapter* packetizer = new HTTPMultipartAdapter("image/jpeg");
//                 //stream.attach(packetizer);
//
//                 //FLVMetadataInjector* injector = new FLVMetadataInjector(options.oformat);
//                 //stream.attach(injector);
//
//                 // start the stream
//                 stream.start();
//
//                 while (!stop)
//                 {
//                     Thread::sleep(50);
//                 }
//
//                 //stream.detach(packetDelegate(this, &MediaConnection::onVideoEncoded));
//                 //stream.stop();
//
//                 //outputFile.close();
//                 cerr << "MediaConnection: ENDING!!!!!!!!!!!!!" << endl;
//             }
//             catch (std::exception& exc)
//             {
//                 cerr << "MediaConnection: " << exc.what() << endl;
//             }
//         }
//
//         void onVideoEncoded(void* sender, RawPacket& packet)
//         {
//             StreamSocket& ss = socket();
//
//             fpsCounter.tick();
//             DebugL << "On Video Packet Encoded: " << fpsCounter.fps << endl;
//
//             //if (fpsCounter.frames < 10)
//             //    return;
//             //if (fpsCounter.frames == 10) {
//             //    stream.reset();
//             //    return;
//             //}
//
//             try
//             {
//                 ss.sendBytes(packet.data, packet.size);
//             }
//             catch (std::exception& exc)
//             {
//                 cerr << "MediaConnection: " << exc.what() << endl;
//                 stop = true;
//             }
//         }
//
//         bool stop;
//         PacketStream stream;
//         FPSCounter fpsCounter;
//     };
//
//     void runMediaSocketTest()
//     {
//         DebugL << "Running Media Socket Test" << endl;
//
//         ServerSocket svs(666);
//         TCPServer srv(new TCPServerConnectionFactoryImpl<MediaConnection>(), svs);
//         srv.start();
//         //scy::pause();
//     }
//
//
//     // ---------------------------------------------------------------------
//     // Video CaptureRecorder Test
//     //
//     //UDPSocket outputSocket;
//
//     void runCaptureRecorderTest()
//     {
//         DebugL << "Running Capture Encoder Test" << endl;
//
//         av::EncoderOptions options;
//         options.ofile = "enctest.mp4";
//         //options.stopAt = time(0) + 3;
//         av::setVideoCaptureInputForma(videoCapture, options.iformat);
//         //options.oformat = Format("MJPEG", "mjpeg", VideoCodec(Codec::MJPEG, "MJPEG", 400, 300));
//         options.oformat = Format("FLV", "flv", VideoCodec(Codec::H264, "H264", 400, 300, 25));
//         //options.oformat = Format("FLV", "flv", VideoCodec(Codec::FLV, "FLV", 320, 240, 15));
//
//         //CaptureRecorder<VideoEncoder> enc(videoCapture, options);
//         //encoder = new AVEncoder(stream.options());
//         CaptureRecorder enc(options, videoCapture, audioCapture);
//         //enc.initialize();
//
//         enc.attach(packetDelegate(this, &Tests::onCaptureRecorderTestVideoEncoded));
//         enc.start();
//         //scy::pause();
//         enc.stop();
//
//         DebugL << "Running Capture Encoder Test: END" << endl;
//     }
//
//     FPSCounter fpsCounter;
//     void onCaptureRecorderTestVideoEncoded(void* sender, MediaPacket& packet)
//     {
//         fpsCounter.tick();
//         DebugL << "On Video Packet Encoded: " << fpsCounter.fps << endl;
//     }
//
//     // ---------------------------------------------------------------------
//     // Video CaptureRecorder Test
//     //
//     void runVideoRecorderTest()
//     {
//         av::EncoderOptions options;
//         options.ofile = "av_capture_test.flv";
//
//         options.oformat = Format("FLV", "flv",
//             VideoCodec(Codec::FLV, "FLV", 320, 240, 15),
//             //AudioCodec(Codec::NellyMoser, "NellyMoser", 1, 11025),
//             AudioCodec(Codec::Speex, "Speex", 1, 16000)//,
//             //AudioCodec(Codec::Speex, "Speex", 2, 44100)
//             );
//         //options.oformat = Format("MP4", Format::MP4,
//         options.oformat = Format("FLV", "flv",
//             //VideoCodec(Codec::MPEG4, "MPEG4", 640, 480, 25),
//             //VideoCodec(Codec::H264, "H264", 640, 480, 25),
//             VideoCodec(Codec::FLV, "FLV", 640, 480, 25),
//             //AudioCodec(Codec::NellyMoser, "NellyMoser", 1, 11025)
//             //AudioCodec(Codec::Speex, "Speex", 2, 44100)
//             //AudioCodec(Codec::MP3, "MP3", 2, 44100)
//             //AudioCodec(Codec::AAC, "AAC", 2, 44100)
//             AudioCodec(Codec::AAC, "AAC", 1, 11025)
//         );
//
//         options.oformat = Format("M4A", Format::M4A,
//             //AudioCodec(Codec::NellyMoser, "NellyMoser", 1, 44100)
//             AudioCodec(Codec::AAC, "AAC", 2, 44100)
//             //AudioCodec(Codec::AC3, "AC3", 2, 44100)
//         );
//
//
//
//         //options.stopAt = time(0) + 5; // Max 24 hours
//         av::setVideoCaptureInputForma(videoCapture, options.iformat);
//
//         CaptureRecorder enc(options, nullptr, audioCapture); //videoCapture
//
//         audioCapture->start();
//         enc.start();
//         //scy::pause();
//         enc.stop();
//     }
// #endif
//
//
//
//     // ---------------------------------------------------------------------
//     // Capture Encoder Test
//     //
//     class CaptureRecorder {
//     public:
//         CaptureRecorder(ICapture* capture, const av::EncoderOptions& options) :
//             capture(capture), encoder(options), closed(false) {
//             assert(capture);
//             encoder.initialize();
//         };
//
//         void start()
//         {
//             capture->emitter += packetDelegate(this, &CaptureRecorder::onFrame);
//             capture->start();
//             //audioCapture->start();
//         }
//
//         void stop()
//         {
//             capture->emitter -= packetDelegate(this, &CaptureRecorder::onFrame);
//             capture->stop();
//             encoder.uninitialize();
//             closed = true;
//         }
//
//         void onFrame(void* sender, RawPacket& packet)
//         {
//             DebugL << "On packet: " << packet.size() << endl;
//             assert(!closed);
//             try
//             {
//                 encoder.process(packet);
//             }
//             catch (std::exception& exc)
//             {
//                 ErrorL << "Capture Recorder Error: " << exc.what() << endl;
//                 stop();
//             }
//         }
//
//         ICapture* capture;
//         AVPacketEncoder encoder;
//         bool closed;
//     };
//
//     // ---------------------------------------------------------------------
//     // Audio CaptureRecorder Test
//     //
//     void runAudioRecorderTest()
//     {
//         av::EncoderOptions options;
//         options.ofile = "audio_test.mp3";
//         //options.stopAt = time(0) + 5;
//         options.oformat = av::Format("MP3", "mp3",
//             av::AudioCodec("MP3", "libmp3lame", 2, 44100, 128000, "s16p"));
//
//         av::Device dev;
//         auto& media = av::MediaFactory::instance();
//         media.devices().getDefaultAudioInputDevice(dev);
//         InfoL << "Default audio capture " << dev.id << endl;
//         av::AudioCapture::Ptr audioCapture = media.createAudioCapture(0, //dev.id
//             options.oformat.audio.channels,
//             options.oformat.audio.sampleRate);
//         audioCapture->getEncoderFormat(options.iformat);
//
//         CaptureRecorder enc(audioCapture.get(), options);
//
//         enc.start();
//         scy::pause();
//         enc.stop();
//     }
//
//     // ---------------------------------------------------------------------
//     // Audio CaptureRecorder Test
//     //
//     void runAVInputReaderRecorderTest()
//     {
//         PacketStream stream;
//
//         // Create the Encoder Options
//         av::EncoderOptions options;
//         options.ofile = "audio_test.mp4";
//         options.duration = 5; //time(0) +
//         options.oformat = av::Format("AAC", "aac",
//             av::AudioCodec("AAC", "aac", 2, 44100, 96000, "fltp"));
//             //av::AudioCodec("MP3", "libmp3lame", 2, 44100, 128000, "s16p"));
//
//         // Attach the Audio Capture
//         av::AVInputReader::Ptr reader(new av::AVInputReader());
//         reader->openAudioDevice(0,
//             options.oformat.audio.channels,
//             options.oformat.audio.sampleRate);
//         reader->getEncoderFormat(options.iformat);
//
//         // Attach the Audio Capture
//         stream.attachSource<av::AVInputReader>(reader, true);
//
//         // Attach the Audio Encoder
//         auto encoder = new av::AVPacketEncoder(options);
//         encoder->initialize();
//         stream.attach(encoder, 5, true);
//
//         stream.start();
//         scy::pause();
//         stream.stop();
//     }
//
//
//     // ---------------------------------------------------------------------
//     // Audio CaptureRecorder Test
//     //
//     void runAudioStreamRecorderTest()
//     {
//         PacketStream stream;
//
//         // Create the Encoder Options
//         av::EncoderOptions options;
//         options.ofile = "audio_test.mp3";
//         //options.stopAt = time(0) + 5;
//         options.oformat = av::Format("MP3", "mp3",
//             av::AudioCodec("MP3", "libmp3lame", 2, 44100, 128000, "s16p"));
//
//         // Create the Audio Capture
//         av::Device dev;
//         auto& media = av::MediaFactory::instance();
//         media.devices().getDefaultAudioInputDevice(dev);
//         InfoL << "Default audio capture " << dev.id << endl;
//         av::AudioCapture::Ptr audioCapture = media.createAudioCapture(0, //dev.id
//             options.oformat.audio.channels,
//             options.oformat.audio.sampleRate);
//         audioCapture->getEncoderFormat(options.iformat);
//
//         // Attach the Audio Capture
//         stream.attachSource<av::AudioCapture>(audioCapture, true);
//
//         // Attach the Audio Encoder
//         //auto encoder = new av::AVPacketEncoder(options);
//         //encoder->initialize();
//         //stream.attach(encoder, 5, true);
//
//         //CaptureRecorder enc(audioCapture, options);
//
//         stream.start();
//         scy::pause();
//         stream.stop();
//     }
// };


} // namespace scy


#endif // SCY_Media_Tests_H
