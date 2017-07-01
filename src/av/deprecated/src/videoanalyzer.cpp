///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/videoanalyzer.h"


#ifdef HAVE_FFMPEG


using std::endl;


namespace scy {
namespace av {


VideoAnalyzer::VideoAnalyzer(const Options& options)
    : _options(options)
    , _video(nullptr)
    , _audio(nullptr)
    , _videoConv(nullptr)
{
    LTrace("Create")
}


VideoAnalyzer::~VideoAnalyzer()
{
    LTrace("Destroy")
    uninitialize();
}


void VideoAnalyzer::initialize()
{
    if (_options.ifile.empty())
        throw std::runtime_error("Please specify an input file.");

    LTrace("Loading: ", _options.ifile)

    _error = "";

    // Open the input file decoder.
    _reader.openFile(_options.ifile);

    if (_reader.video()) {
        _videoConv = nullptr;
        _video = new VideoAnalyzer::Stream("Video", _options.rdftSize);
        _video->initialize();
    }

    if (_reader.audio()) {
        _audio = new VideoAnalyzer::Stream("Audio", _options.rdftSize);
        _audio->initialize();
    }

    _reader.emitter += packetSlot(this, &VideoAnalyzer::onVideo);
    _reader.emitter += packetSlot(this, &VideoAnalyzer::onAudio);
    _reader.Closing += sdelegate(this, &VideoAnalyzer::onReadComplete);
}


void VideoAnalyzer::uninitialize()
{
    // STrace << "[VideoAnalyzerStream: " << this << ": " << name << "]
    // Uninitializing" << endl;
    stop();

   

    if (_video)
        delete _video;
    if (_videoConv)
        delete _videoConv;
    if (_audio)
        delete _audio;
}


void VideoAnalyzer::start()
{
    std::lock_guard<std::mutex> guard(_mutex);

    try {
        if (!_reader.audio() && !_reader.video())
            throw std::runtime_error("Video reader not initialized");

        // if (_options.blocking)
        //    _reader.run();
        // else
        _reader.start();
    } catch (std::exception& exc) {
        _error = exc.what();
        LError("Error: ", _error)
        throw exc; //.rethrow()
    }
}


void VideoAnalyzer::stop()
{
    // Can't lock here in case we are inside a callback.
   

    _reader.Closing -= sdelegate(this, &VideoAnalyzer::onReadComplete);
    _reader.emitter.detach(this);
    _reader.stop();
}


void VideoAnalyzer::onVideo(void*, VideoPacket& packet)
{
    // STrace << "On video: "
    //    << packet.size() << ": " << packet.time << endl;

    VideoAnalyzer::Packet pkt(packet.time);
    AVFrame* greyFrame = getGrayVideoFrame();
    int width = greyFrame->width;
    int height = greyFrame->height;
    int step = greyFrame->linesize[0];
    int frames = 0;
    // VideoDecoder* video = _reader.video();

    std::lock_guard<std::mutex> guard(_mutex);

    // Prepeare FFT input data array
    //    http://stackoverflow.com/questions/7790877/forward-fft-an-image-and-backward-fft-an-image-to-get-the-same-result
    //    https://code.google.com/p/video-processing-application/source/browse/trunk/+video-processing-application/untitled6/Fourier/highpassrgb.cpp?r=2
    //    http://codepaste.ru/9226/
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            _video->rdftData[_video->filled++] =
                (float)greyFrame->data[0][y * step + x] * pow(-1.0, y + x);
            if (_video->filled == _video->rdftSize) {
                _video->fft();
                pkt.value += CalculateFrequencyIntensity(*_video);
                frames++;
                _video->frames++;
                _video->filled = 0;
            }
        }
    }

    if (frames) {

        // Calculate and scale the average value for this video frame
        pkt.value /= frames;
        pkt.value = sqrt(pkt.value); ///= _video->rdftSize;

        STrace << "Video Output: " << pkt.time << ", " << pkt.value
                     << endl;
        PacketOut.emit(*_video, pkt);
    }
}

void VideoAnalyzer::onAudio(void*, AudioPacket& packet)
{
    // STrace << "On Audio: "
    //  << packet.size() << ": " << packet.time << endl;

    std::lock_guard<std::mutex> guard(_mutex);

    VideoAnalyzer::Packet pkt(packet.time);
    short const* data = reinterpret_cast<short*>(packet.data());
    int channels = _reader.audio()->stream->codec->channels;
    int size = packet.size();
    int frames = 0;

    // Packet size / 2 = 2 bytes per sample (short)
    // Example at http://blackhole.ubitux.fr/bench-fftw-ffmpeg-fft/fft.c
    for (int i = 0; i < size / 2; i += channels) {
        int k, v = 0;
        for (k = 0; k < channels; k++) // mix channels
            v += data[i + k];

        _audio->rdftData[_audio->filled++] = (float)v / channels / SHRT_MAX;
        if (_audio->filled == _audio->rdftSize) {
            _audio->fft();
            pkt.value += CalculateFrequencyIntensity(*_audio);
            frames++;
            _audio->frames++;
            _audio->filled = 0;
        }
    }

    if (frames) {

        // Calculate the average value for this video frame
        pkt.value /= frames;

        STrace << "Audio Output: " << pkt.time << ", " << pkt.value
                     << endl;
        PacketOut.emit(*_audio, pkt);
    }
}


AVFrame* VideoAnalyzer::getGrayVideoFrame()
{
    std::lock_guard<std::mutex> guard(_mutex);
    VideoDecoder* video = _reader.video();

    // TODO: Conversion via decoder?
    if (_videoConv == nullptr) {
        _videoConv = new VideoConverter();

        auto& iparams = _videoConv->iparams;
        iparams.width = video->ctx->width;
        iparams.height = video->ctx->height;
        iparams.pixelFmt = av_get_pix_fmt_name(video->ctx->pix_fmt);

        auto& oparams = _videoConv->oparams;
        oparams.width = video->ctx->width;
        oparams.height = video->ctx->height;
        oparams.pixelFmt = "gray";

        _videoConv->create();
    }
    if (_videoConv == nullptr)
        throw std::runtime_error("Video Analyzer: Unable to initialize the "
                                 "video conversion context.");

    // Convert the source image to grayscale
    return _videoConv->convert(video->frame);
}


void VideoAnalyzer::onReadComplete(void* sender)
{
    LTrace("On Read Complete")

    MediaCapture* reader = reinterpret_cast<MediaCapture*>(sender);
    {
        std::lock_guard<std::mutex> guard(_mutex);
        if (_error.empty())
            _error = reader->error();
    }

    Complete.emit(/*this*/);
}


MediaCapture& VideoAnalyzer::reader()
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _reader;
}


VideoAnalyzer::Options& VideoAnalyzer::options()
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _options;
}


std::string VideoAnalyzer::error() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _error;
}


// ---------------------------------------------------------------------
//
VideoAnalyzer::Packet::Packet(double time, double value)
    : //, double min, double max
    time(time)
    , value(value)
{
    // this->min = min;
    // this->max = max;
}


// ---------------------------------------------------------------------
//
const int kMaxFFTPow2Size = 24;


VideoAnalyzer::Stream::Stream(const std::string& name, int rdftSize)
    : name(name)
    , rdft(nullptr)
    , rdftData(nullptr)
    , rdftSize(rdftSize)
    , rdftBits(static_cast<int>(log2(rdftSize)))
    , frames(0)
    , filled(0)
{
    STrace << "[VideoAnalyzerStream: " << this << ": " << name
           << "] Creating: " << rdftLize(": ", rdftBits)

    assert(rdftSize);
    assert(rdftBits < kMaxFFTPow2Size);

    // We only allow power of two.
    assert(int(1UL << rdftBits) == rdftSize);
}


VideoAnalyzer::Stream::~Stream()
{
    uninitialize();
}


void VideoAnalyzer::Stream::initialize()
{
    frames = 0;
    filled = 0;

    assert(rdft == nullptr);
    rdft = av_rdft_init(rdftBits, DFT_R2C);
    if (rdft == nullptr)
        throw std::runtime_error("Cannot allocate FFT context");

    assert(rdftData == nullptr);
    rdftData = (FFTSample*)av_malloc(rdftSize * sizeof(*rdftData));
    if (rdftData == nullptr)
        throw std::runtime_error("Cannot allocate FFT buffer");
}


void VideoAnalyzer::Stream::uninitialize()
{
    // STrace << "[VideoAnalyzerStream: " << this << ": " << name << "]
    // Uninitializing" << endl;

    if (rdft)
        av_rdft_end(rdft);
    if (rdftData)
        av_free(rdftData);
}


void VideoAnalyzer::Stream::fft()
{
    assert(filled == rdftSize);
    av_rdft_calc(rdft, rdftData);
}


// ---------------------------------------------------------------------
//
double CalculateCentroidFrequency(VideoAnalyzer::Stream& stream)
{
    // These two values are used for getting the Spectral Centroid
    // http://en.wikipedia.org/wiki/Spectral_centroid
    double centroidFnXn = 0;
    double centroidXn = 0;

    for (int i = 0; i < stream.filled / 2; i += 2) {
        centroidFnXn += i * abs(stream.rdftData[i]);
        centroidXn += abs(stream.rdftData[i]);
    }

    return centroidFnXn / centroidXn;
}


double CalculateFrequencyIntensity(VideoAnalyzer::Stream& stream)
{
    double intensity = 0.0;
    for (int i = 0; i < stream.filled / 2; i += 2) {
        intensity +=
            GetFrequencyIntensity(stream.rdftData[i], stream.rdftData[i + 1]);
    }
    return intensity;
}


double GetFrequencyIntensity(double re, double im)
{
    return sqrt((re * re) + (im * im));
}


double GetDecibels(double re, double im)
{
    return (re == 0 && im == 0) ? 0 : 10.0 * log10(re * re + im * im);
}

double GetAmplitude(double re, double im, int len)
{
    return GetFrequencyIntensity(re, im) / (len);
}

double GetAmplitudeScaled(double re, double im, int len, int scale)
{
    return static_cast<int>(GetAmplitude(re, im, len)) % scale;
}


#ifdef SCY_WIN
double log2(double n)
{
    return log(n) / log(double(2));
}
#endif


} // namespace av
} // namespace scy


#endif


/// @\}
