#ifndef SCY_WebRTCRecorder_FileVideoCapturer_H
#define SCY_WebRTCRecorder_FileVideoCapturer_H


#include "scy/av/mediacapture.h"

#include "webrtc/api/peerconnectioninterface.h"


// // CustomAudioTrack
// #include <string>
//
// #include "webrtc/api/mediastreaminterface.h"
// #include "webrtc/api/mediastreamtrack.h"
// #include "webrtc/api/notifier.h"
// #include "webrtc/base/constructormagic.h"
// #include "webrtc/base/scoped_ref_ptr.h"
// #include "webrtc/base/thread_checker.h"
//
//
// // CustomLocalAudioSinkAdapter
// #include <memory>
// #include <string>
//
// #include "webrtc/api/mediastreaminterface.h"
// #include "webrtc/api/rtpsenderinterface.h"
// #include "webrtc/api/statscollector.h"
// #include "webrtc/base/basictypes.h"
// #include "webrtc/base/criticalsection.h"
// #include "webrtc/media/base/audiosource.h"
// #include "webrtc/pc/channel.h"
//
// namespace webrtc {
//
//
// // CustomLocalAudioSinkAdapter receives data callback as a sink to the local
// // AudioTrack, and passes the data to the sink of AudioSource.
// class CustomLocalAudioSinkAdapter : public AudioTrackSinkInterface,
//                               public cricket::AudioSource {
//  public:
//   CustomLocalAudioSinkAdapter();
//   virtual ~CustomLocalAudioSinkAdapter();
//
//  private:
//   // AudioSinkInterface implementation.
//   void OnData(const void* audio_data,
//               int bits_per_sample,
//               int sample_rate,
//               size_t number_of_channels,
//               size_t number_of_frames) override;
//
//   // cricket::AudioSource implementation.
//   void SetSink(cricket::AudioSource::Sink* sink) override;
//
//   cricket::AudioSource::Sink* sink_;
//   // Critical section protecting |sink_|.
//   rtc::CriticalSection lock_;
// };
//
// ////////////////////////////////
//
// class CustomAudioTrack : public MediaStreamTrack<AudioTrackInterface>,
//                    public ObserverInterface {
//  protected:
//   // Protected ctor to force use of factory method.
//   CustomAudioTrack(const std::string& label,
//              const rtc::scoped_refptr<AudioSourceInterface>& source);
//   ~CustomAudioTrack() override;
//
//  public:
//   static rtc::scoped_refptr<CustomAudioTrack> Create(
//       const std::string& id,
//       const rtc::scoped_refptr<AudioSourceInterface>& source);
//
//  private:
//   // MediaStreamTrack implementation.
//   std::string kind() const override;
//
//   // AudioTrackInterface implementation.
//   AudioSourceInterface* GetSource() const override;
//
//   void AddSink(AudioTrackSinkInterface* sink) override;
//   void RemoveSink(AudioTrackSinkInterface* sink) override;
//
//   // ObserverInterface implementation.
//   void OnChanged() override;
//
//  private:
//   const rtc::scoped_refptr<AudioSourceInterface> audio_source_;
//   rtc::ThreadChecker thread_checker_;
//   RTC_DISALLOW_IMPLICIT_CONSTRUCTORS(CustomAudioTrack);
// };
//
//
// } // namespace webrtc


namespace scy {


class FileVideoCapturer:
    // public rtc::VideoSinkInterface<cricket::VideoFrame>,
    // public webrtc::AudioTrackSinkInterface
    public webrtc::AudioTrackInterface
{
public:
    FileVideoCapturer(); //const av::EncoderOptions& options
    virtual ~FileVideoCapturer();


    // AudioTrackInterface implementation
    // virtual void AddSink(AudioTrackSinkInterface* sink);
    // virtual void RemoveSink(AudioTrackSinkInterface* sink);

    // void setVideoTrack(webrtc::VideoTrackInterface* track);
    // void setCustomAudioTrack(webrtc::AudioTrackInterface* track);
    //
    // void onPacketEncoded(av::MediaPacket& packet);
    //
    // // VideoSinkInterface implementation
    // void OnFrame(const cricket::VideoFrame& frame) override;
    //
    // // AudioTrackSinkInterface implementation
    // void OnData(const void* audio_data,
    //                     int bits_per_sample,
    //                     int sample_rate,
    //                     size_t number_of_channels,
    //                     size_t number_of_frames) override;

protected:
    av::MediaCapture _capture;
    // rtc::scoped_refptr<webrtc::VideoTrackInterface> _videoTrack;
    // rtc::scoped_refptr<webrtc::AudioTrackInterface> _audioTrack;
    // bool _awaitingVideo;
    // bool _awaitingAudio;
};


} // namespace scy


#endif
