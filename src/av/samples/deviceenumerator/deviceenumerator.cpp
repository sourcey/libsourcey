#include "scy/av/devicemanager.h"


using namespace scy;

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avstring.h>
}


// static int print_device_sources(AVInputFormat *fmt, AVDictionary *opts)
// {
//     int ret, i;
//     AVDeviceInfoList *device_list = NULL;
//
//     if (!fmt || !fmt->priv_class  || !AV_IS_INPUT_DEVICE(fmt->priv_class->category))
//         return AVERROR(EINVAL);
//
//     printf("Auto-detected sources for %s:\n", fmt->name);
//     if (!fmt->get_device_list) {
//         ret = AVERROR(ENOSYS);
//         printf("Cannot list sources. Not implemented.\n");
//         goto fail;
//     }
//
//     if ((ret = avdevice_list_input_sources(fmt, NULL, opts, &device_list)) < 0) {
//         printf("Cannot list sources.\n");
//         goto fail;
//     }
//
//     for (i = 0; i < device_list->nb_devices; i++) {
//         printf("%s %s [%s]\n", device_list->default_device == i ? "*" : " ",
//                device_list->devices[i]->device_name, device_list->devices[i]->device_description);
//     }
//
//   fail:
//     avdevice_free_list_devices(&device_list);
//     return ret;
// }
//
//
// static int print_device_sinks(AVOutputFormat *fmt, AVDictionary *opts)
// {
//     int ret, i;
//     AVDeviceInfoList *device_list = NULL;
//
//     if (!fmt || !fmt->priv_class  || !AV_IS_OUTPUT_DEVICE(fmt->priv_class->category))
//         return AVERROR(EINVAL);
//
//     printf("Auto-detected sinks for %s:\n", fmt->name);
//     if (!fmt->get_device_list) {
//         ret = AVERROR(ENOSYS);
//         printf("Cannot list sinks. Not implemented.\n");
//         goto fail;
//     }
//
//     if ((ret = avdevice_list_output_sinks(fmt, NULL, opts, &device_list)) < 0) {
//         printf("Cannot list sinks.\n");
//         goto fail;
//     }
//
//     for (i = 0; i < device_list->nb_devices; i++) {
//         printf("%s %s [%s]\n", device_list->default_device == i ? "*" : " ",
//                device_list->devices[i]->device_name, device_list->devices[i]->device_description);
//     }
//
//   fail:
//     avdevice_free_list_devices(&device_list);
//     return ret;
// }


int main(int argc, char** argv)
{

    // av::initializeFFmpeg(); // init here so reference is not held

    // AVInputFormat *fmt = NULL;
    // char *dev = NULL;
    // AVDictionary *opts = NULL;
    //
    // do {
    //     fmt = av_input_audio_device_next(fmt);
    //     if (fmt) {
    //         if (!strcmp(fmt->name, "lavfi"))
    //             continue; //it's pointless to probe lavfi
    //         if (dev && !av_match_name(dev, fmt->name))
    //             continue;
    //         print_device_sources(fmt, opts);
    //     }
    // } while (fmt);
    // do {
    //     fmt = av_input_video_device_next(fmt);
    //     if (fmt) {
    //         if (dev && !av_match_name(dev, fmt->name))
    //             continue;
    //         print_device_sources(fmt, opts);
    //     }
    // } while (fmt);
    //
    //
    // do {
    //     fmt = av_output_audio_device_next(fmt);
    //     if (fmt) {
    //         if (dev && !av_match_name(dev, fmt->name))
    //             continue;
    //         print_device_sinks(fmt, opts);
    //     }
    // } while (fmt);
    // do {
    //     fmt = av_output_video_device_next(fmt);
    //     if (fmt) {
    //         if (dev && !av_match_name(dev, fmt->name))
    //             continue;
    //         print_device_sinks(fmt, opts);
    //     }
    // } while (fmt);

    // av::uninitializeFFmpeg();

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
