#include "filevideocapturer.h"

#include "scy/logger.h"
#include "scy/filesystem.h"
#include "scy/av/ffmpeg.h"

// #include "webrtc/modules/video_capture/video_capture_factory.h"
// #include "webrtc/media/engine/webrtcvideocapturerfactory.h"
// #include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"
//
//
// // CustomAudioTrack
// #include "webrtc/base/checks.h"
//
// using rtc::scoped_refptr;
// using namespace scy;
//
// namespace webrtc {
//
//
//
//
//
// // HAPPENS HERE
//
// // void AudioRtpSender::SetAudioSend() {
// //   RTC_DCHECK(!stopped_ && can_send_track());
// //   if (!channel_) {
// //     LOG(LS_ERROR) << "SetAudioSend: No audio channel exists.";
// //     return;
// //   }
// //   cricket::AudioOptions options;
// //   cricket::AudioSource* source = sink_adapter_.get();
// //   RTC_DCHECK(source != nullptr);
// //   if (!channel_->SetAudioSend(ssrc_, track_->enabled(), &options, source)) {
// //     LOG(LS_ERROR) << "SetAudioSend: ssrc is incorrect: " << ssrc_;
// //   }
// // }
//
// // Create a new instance of PeerConnectionFactoryInterface.
// //
// // |network_thread|, |worker_thread| and |signaling_thread| are
// // the only mandatory parameters.
// //
// // If non-null, ownership of |default_adm|, |encoder_factory| and
// // |decoder_factory| are transferred to the returned factory.
// // rtc::scoped_refptr<PeerConnectionFactoryInterface> CreatePeerConnectionFactory(
// //     rtc::Thread* network_thread,
// //     rtc::Thread* worker_thread,
// //     rtc::Thread* signaling_thread,
// //     AudioDeviceModule* default_adm,
// //     cricket::WebRtcVideoEncoderFactory* encoder_factory,
// //     cricket::WebRtcVideoDecoderFactory* decoder_factory);
// //
// //
// //     FakeAudioCaptureModule
// //
// //     FileAudioDevice
//
// // Does audio device get recreated on each playout????
//
//
// // Figured it out. You need to create your own AudioDeviceModule and pass it to the PeerConnectionFactory, and then use the module to enumerate the audio devices and set the correct one.
//
//
//
// CustomLocalAudioSinkAdapter::CustomLocalAudioSinkAdapter() : sink_(nullptr) {
//
//   InfoL << "############ CustomLocalAudioSinkAdapter" << std::endl;
//
// }
//
// CustomLocalAudioSinkAdapter::~CustomLocalAudioSinkAdapter() {
//   rtc::CritScope lock(&lock_);
//   if (sink_)
//     sink_->OnClose();
// }
//
// void CustomLocalAudioSinkAdapter::OnData(const void* audio_data,
//                                    int bits_per_sample,
//                                    int sample_rate,
//                                    size_t number_of_channels,
//                                    size_t number_of_frames) {
//
//   InfoL << "############ OnData: " << bits_per_sample << std::endl;
//   rtc::CritScope lock(&lock_);
//   if (sink_) {
//     sink_->OnData(audio_data, bits_per_sample, sample_rate, number_of_channels,
//                   number_of_frames);
//   }
// }
//
// void CustomLocalAudioSinkAdapter::SetSink(cricket::AudioSource::Sink* sink) {
//   rtc::CritScope lock(&lock_);
//   ASSERT(!sink || !sink_);
//   sink_ = sink;
// }
//
// /////////////////////
//
// // const char MediaStreamTrackInterface::kAudioKind[] = "audio";
//
// // static
// scoped_refptr<CustomAudioTrack> CustomAudioTrack::Create(
//     const std::string& id,
//     const scoped_refptr<AudioSourceInterface>& source) {
//   return new rtc::RefCountedObject<CustomAudioTrack>(id, source);
// }
//
// CustomAudioTrack::CustomAudioTrack(const std::string& label,
//                        const scoped_refptr<AudioSourceInterface>& source)
//     : MediaStreamTrack<AudioTrackInterface>(label), audio_source_(source) {
//   if (audio_source_) {
//     audio_source_->RegisterObserver(this);
//     OnChanged();
//   }
// }
//
// CustomAudioTrack::~CustomAudioTrack() {
//   RTC_DCHECK(thread_checker_.CalledOnValidThread());
//   set_state(MediaStreamTrackInterface::kEnded);
//   if (audio_source_)
//     audio_source_->UnregisterObserver(this);
// }
//
// std::string CustomAudioTrack::kind() const {
//   RTC_DCHECK(thread_checker_.CalledOnValidThread());
//   return kAudioKind;
// }
//
// AudioSourceInterface* CustomAudioTrack::GetSource() const {
//   RTC_DCHECK(thread_checker_.CalledOnValidThread());
//   InfoL << "############ GetSource" << std::endl;
//   return audio_source_.get();
// }
//
// void CustomAudioTrack::AddSink(AudioTrackSinkInterface* sink) {
//   RTC_DCHECK(thread_checker_.CalledOnValidThread());
//   InfoL << "############ ADDING SINK: " << sink << std::endl;
//   if (audio_source_) {
//
//       // audio_source_->AddSink(sink);
//
//       audio_source_->AddSink(new CustomLocalAudioSinkAdapter());
//   }
// }
//
// void CustomAudioTrack::RemoveSink(AudioTrackSinkInterface* sink) {
//   RTC_DCHECK(thread_checker_.CalledOnValidThread());
//   InfoL << "############ REMVE SINK: " << sink << std::endl;
//   if (audio_source_)
//     audio_source_->RemoveSink(sink);
// }
//
// void CustomAudioTrack::OnChanged() {
//   RTC_DCHECK(thread_checker_.CalledOnValidThread());
//   if (audio_source_->state() == MediaSourceInterface::kEnded) {
//     InfoL << "############ STATE ENDED" << std::endl;
//     set_state(kEnded);
//   } else {
//     InfoL << "############ STATE LIVE" << std::endl;
//     set_state(kLive);
//   }
// }
//
//
// } // namespace webrtc


namespace scy {


FileVideoCapturer::FileVideoCapturer() // : const av::EncoderOptions& options
    // _encoder(options),
    // _awaitingVideo(false),
    // _awaitingAudio(false)
{
    // Disable audio and video until tracks are set
    // _encoder.options().oformat.video.enabled = false;
    // _encoder.options().oformat.audio.enabled = false;
}


FileVideoCapturer::~FileVideoCapturer()
{
    // if (_videoTrack)
    //     _videoTrack->RemoveSink(this);
    //
    // if (_audioTrack)
    //     _audioTrack->RemoveSink(this);
}


// AudioTrackInterface implementation
// void FileVideoCapturer::AddSink(AudioTrackSinkInterface* sink);
// void FileVideoCapturer::RemoveSink(AudioTrackSinkInterface* sink);


// void FileVideoCapturer::setVideoTrack(webrtc::VideoTrackInterface* track)
// {
//     assert(!_videoTrack);
//     _videoTrack = track;
//     _videoTrack->AddOrUpdateSink(this, rtc::VideoSinkWants());
//     _encoder.options().oformat.video.enabled = true;
//     _awaitingVideo = true;
// }
//
//
// void FileVideoCapturer::setCustomAudioTrack(webrtc::AudioTrackInterface* track)
// {
//     assert(!_audioTrack);
//     _audioTrack = track;
//     _audioTrack->AddSink(this);
//     _encoder.options().oformat.audio.enabled = true;
//     _awaitingAudio = true;
// }
//
//
// void FileVideoCapturer::OnFrame(const cricket::VideoFrame& yuvframe)
// {
//     TraceL << "On video frame: " << yuvframe.width() << 'x' << yuvframe.height() << std::endl;
//
//     // TODO: Recreate encoder context on input stream change
//     if (_awaitingVideo) {
//         _awaitingVideo = false;
//
//         auto& ivideo = _encoder.options().iformat.video;
//         ivideo.width = yuvframe.width();
//         ivideo.height = yuvframe.height();
//         ivideo.pixelFmt = "yuv420p";
//         ivideo.fps = 25;
//
//         if (!_awaitingAudio)
//             _encoder.initialize();
//     }
//
//     if (_encoder.isActive()) {
//         // Set AVFrame->data pointers manually so we don't need to copy any data or
//         // convert the pixel format from YUV to some contiguous format.
//         auto frame = _encoder.video()->frame;
//         frame->data[0] = (uint8_t*)yuvframe.video_frame_buffer()->DataY();
//         frame->data[1] = (uint8_t*)yuvframe.video_frame_buffer()->DataU();
//         frame->data[2] = (uint8_t*)yuvframe.video_frame_buffer()->DataV();
//         frame->width = yuvframe.width();
//         frame->height = yuvframe.height();
//         frame->pts = AV_NOPTS_VALUE; // set by encoder
//
//         // Encode the video frame
//         _encoder.encodeVideo(frame);
//     }
// }
//
//
// void FileVideoCapturer::OnData(const void* audio_data,
//                             int bits_per_sample,
//                             int sample_rate,
//                             size_t number_of_channels,
//                             size_t number_of_frames)
// {
//     TraceL << "On audio frame: "
//       << "number_of_frames=" << number_of_frames << ", "
//       << "number_of_channels=" << number_of_channels << ", "
//       << "sample_rate=" << sample_rate << ", "
//       << "bits_per_sample=" << bits_per_sample
//       << std::endl;
//
//     // FIXME: For some reason the first couple of samples come though as mono,
//     // so let's just skip those for now.
//     if (number_of_channels < 2) {
//         WarnL << "Dropping initial mono samples" << std::endl;
//         return;
//     }
//
//     // TODO: Recreate encoder context on input stream change
//     if (_awaitingAudio) {
//         _awaitingAudio = false;
//
//         auto& iaudio = _encoder.options().iformat.audio;
//         iaudio.channels = number_of_channels;
//         iaudio.sampleRate = sample_rate;
//         iaudio.sampleFmt = "s16";
//
//         // WebRTC always sets 16 bits per sample, hence we use the s16 sample
//         // format. Set an assertion just incase this ever changes or varies.
//         assert(bits_per_sample == 16);
//
//         if (!_awaitingVideo)
//             _encoder.initialize();
//     }
//
//     if (_encoder.isActive())
//         _encoder.encodeAudio((std::uint8_t*)audio_data, number_of_frames);
// }


} // namespace scy
