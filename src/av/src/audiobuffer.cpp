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


#include "scy/av/audiobuffer.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/av/ffmpeg.h"


using std::endl;


namespace scy {
namespace av {


AudioBuffer::AudioBuffer() :
    fifo(nullptr)
{
    TraceS(this) << "Create" << endl;
    assert(!fifo);
}


AudioBuffer::~AudioBuffer()
{
    TraceS(this) << "Destroy" << endl;
    close();
}

void AudioBuffer::alloc(const std::string& sampleFmt, int channels, int numSamples)
{
    TraceS(this) << "Create audio buffer:\n"
        << "\n\tNb Channels: " << channels
        << "\n\tSample Fmt: " << sampleFmt
        << "\n\tNb Samples: " << numSamples
        << "\n\tfifo: " << fifo
        << endl;

    enum AVSampleFormat format = av_get_sample_fmt(sampleFmt.c_str());
    assert(!fifo);
    assert(channels);
    assert(format != AV_SAMPLE_FMT_NONE);

    // Create the FIFO buffer based on the specified sample format.
    fifo = av_audio_fifo_alloc(format, channels, numSamples);
    if (!fifo) {
        throw std::runtime_error("Cannot allocate FIFO: Out of memory");
    }
}


void AudioBuffer::reset()
{
    TraceS(this) << "Reset" << endl;

    if (fifo) {
        av_audio_fifo_reset(fifo);
    }
}


void AudioBuffer::close()
{
    TraceS(this) << "Close" << endl;

    if (fifo) {
        av_audio_fifo_free(fifo);
        fifo = nullptr;
    }
}


void AudioBuffer::write(void** samples, int numSamples)
{
    int error;
    assert(fifo);
    assert(samples);
    assert(samples[0]);

    // Make the FIFO as large as it needs to be to hold both
    // the old and the new samples.
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + numSamples)) < 0) {
        throw std::runtime_error("Cannot reallocate FIFO: " + averror(error));
    }

    // Store the new samples in the FIFO buffer.
    if (av_audio_fifo_write(fifo, (void**)samples, numSamples) < numSamples) {
        throw std::runtime_error("Cannot write data to FIFO");
    }
}


bool AudioBuffer::read(void** samples, int numSamples)
{
    TraceL << "Read samples: " << numSamples << endl;

    // Make sure that there is one frame worth of samples in the FIFO
    // buffer so that the encoder can do its work.
    // Since the decoder's and the encoder's frame size may differ, we
    // need to FIFO buffer to store as many frames worth of input samples
    // that they make up at least one frame worth of output samples.
    if (available() < numSamples) {
        TraceL << "No packets in buffer" << endl;
        return false;
    }

    // Read as many samples from the FIFO buffer as required.
    // The samples are stored in the output buffer temporarily.
    if (av_audio_fifo_read(fifo, (void**)samples, numSamples) < numSamples) {
        throw std::runtime_error("Cannot read data from FIFO");
    }

    return true;
}


int AudioBuffer::available() const
{
    if (fifo)
        return av_audio_fifo_size(fifo);
    return 0;
}


} } // namespace scy::av


#endif
