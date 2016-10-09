#ifndef SCY_StreamRecorder_H
#define SCY_StreamRecorder_H


#include "scy/media/avencoder.h"

#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/jsep.h"
#include "webrtc/api/datachannelinterface.h"
#include "webrtc/api/test/fakeconstraints.h"


namespace scy {


class StreamRecorderVideoTrack;
class StreamRecorderAudioTrack;


class StreamRecorder:
    public rtc::VideoSinkInterface<cricket::VideoFrame>,
    public webrtc::AudioTrackSinkInterface
{
public:
    StreamRecorder(); //, const std::string& basenamewebrtc::VideoTrackInterface* track_to_render
    virtual ~StreamRecorder();

    void setVideoTrack(webrtc::VideoTrackInterface* track_to_render);
    void setAudioTrack(webrtc::AudioTrackInterface* track_to_render);

    void onPacketEncoded(av::MediaPacket& packet);

    // VideoSinkInterface implementation
    virtual void OnFrame(const cricket::VideoFrame& frame) override;

    // AudioTrackSinkInterface implementation
    virtual void OnData(const void* audio_data,
                        int bits_per_sample,
                        int sample_rate,
                        size_t number_of_channels,
                        size_t number_of_frames) override;

protected:
    av::AVEncoder _encoder;
    // av::VideoEncoderContext _encoder;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> _videoTrack;
    rtc::scoped_refptr<webrtc::AudioTrackInterface> _audioTrack;
    bool _awaitingVideo;
    bool _awaitingAudio;
    std::ofstream _output;


    AVFrame *in_frame;
    uint8_t *raw_in;
    int in_offset;
    // StreamRecorderVideoTrack *_videoTrack;
    // StreamRecorderAudioTrack *_audioTrack;

    // const std::string _basename;
    // size_t _count;
    // int _width;
    // int _height;
    // av::VideoEncoderContext _encoder;
    // AVFrame* _avframe;
};


// class StreamRecorderVideoTrack: public rtc::VideoSinkInterface<cricket::VideoFrame>
// {
// public:
//     StreamRecorderVideoTrack(StreamRecorder* recorder, webrtc::VideoTrackInterface* track_to_render); //, const std::string& basename
//     virtual ~StreamRecorderVideoTrack();
//
//     // std::string getNextFilename();
//
//     // VideoSinkInterface implementation
//     void OnFrame(const cricket::VideoFrame& frame) override;
//
// protected:
//     StreamRecorder* _recorder;
//     rtc::scoped_refptr<webrtc::VideoTrackInterface> _track;
//     size_t _count;
//     int _width;
//     int _height;
//
//     // const std::string _basename;
//     // av::VideoEncoderContext _encoder;
//     // AVFrame* _avframe;
// };
//
//
// // SEE: https://github.com/vmolsa/webrtc-native/blob/master/src/MediaCapturer.h
// class StreamRecorderAudioTrack: public webrtc::AudioTrackSinkInterface
// {
// public:
//     StreamRecorderAudioTrack(StreamRecorder* recorder, webrtc::AudioTrackInterface* track_to_render);
//     virtual ~StreamRecorderAudioTrack();
//
//     // AudioTrackSinkInterface implementation
//     virtual void OnData(const void* audio_data,
//                         int bits_per_sample,
//                         int sample_rate,
//                         size_t number_of_channels,
//                         size_t number_of_frames) override;
//
// protected:
//     StreamRecorder* _recorder;
//     rtc::scoped_refptr<webrtc::AudioTrackInterface> _track;
// };


} // namespace scy


#endif
