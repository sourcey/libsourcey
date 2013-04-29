//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/Media/FFT.h"

#include <assert.h>
#include <math.h>
//#include <cmath.h>
#include <stddef.h>
#include <stdint.h>
#include <limits>
#include <iterator>

#include <sstream>
#include <algorithm>


using namespace std;


namespace Sourcey {
namespace Media {


	/*
// -------------------------------------------------------------------
//
const int kMaxFFTPow2Size = 24;


// Normal constructor: allocates for a given fftSize.
FFTFrame::FFTFrame(unsigned fftSize)
    : _FFTSize(fftSize)
    //, _log2FFTSize(static_cast<unsigned>(log2(fftSize)))
    , _forwardContext(0)
    //, _inverseContext(0)
    , _complexData(new float(fftSize))
    , _realData(new float(fftSize / 2))
    , _imagData(new float(fftSize / 2))
{
    // We only allow power of two.
    assert(1UL << log2FFTSize() == _FFTSize);

    _forwardContext = contextForSize(fftSize, DFT_R2C);
    //_inverseContext = contextForSize(fftSize, IDFT_C2R);
}

// Copy constructor.
FFTFrame::FFTFrame(const FFTFrame& frame)
    : _FFTSize(frame._FFTSize)
    //, _log2FFTSize(frame._log2FFTSize)
    , _forwardContext(0)
    //, _inverseContext(0)
    , _complexData(new float(frame._FFTSize))
    , _realData(new float(frame._FFTSize / 2))
    , _imagData(new float(frame._FFTSize / 2))
{
    _forwardContext = contextForSize(_FFTSize, DFT_R2C);
    //_inverseContext = contextForSize(_FFTSize, IDFT_C2R);

    // Copy/setup frame data.
    unsigned nbytes = sizeof(float) * (_FFTSize / 2);
    memcpy(realData(), frame.realData(), nbytes);
    memcpy(realData(), frame.realData(), nbytes);
    memcpy(imagData(), frame.imagData(), nbytes);
}

FFTFrame::~FFTFrame()
{
    av_rdft_end(_forwardContext);
    //av_rdft_end(_inverseContext);
	
	delete [] _complexData;
	delete [] _realData;
	delete [] _imagData;
}

void FFTFrame::doFFT(const float* data)
{
    // Copy since processing is in-place.
    float* p = _complexData; //.data();
    memcpy(p, data, sizeof(float) * _FFTSize);

    // Compute Forward transform.
    av_rdft_calc(_forwardContext, p);

    // De-interleave to separate real and complex arrays.
    int len = _FFTSize / 2;

    // FIXME: see above comment in multiply() about scaling.
    const float scale = 2.0f;

    for (int i = 0; i < len; ++i) {
        int baseComplexIndex = 2 * i;
        // _realData[0] is the DC component and _imagData[0]
		// is the nyquist component since the interleaved 
		// complex data is packed.
        _realData[i] = scale * p[baseComplexIndex];
        _imagData[i] = scale * p[baseComplexIndex + 1];
    }
}


float* FFTFrame::realData() const
{
    return const_cast<float*>(_realData); //.data());
}

float* FFTFrame::imagData() const
{
    return const_cast<float*>(_imagData); //.data());
}



RDFTContext* FFTFrame::contextForSize(unsigned fftSize, RDFTransformType trans)
{
    assert(fftSize);
    int pow2size = static_cast<int>(log2(fftSize));
    assert(pow2size < kMaxFFTPow2Size);

    RDFTContext* context = av_rdft_init(pow2size, trans);
    return context;
}


// -------------------------------------------------------------------
//
void vsmul(const float* sourceP, int sourceStride, const float* scale, float* destP, int destStride, size_t framesToProcess)
{
    int n = framesToProcess;
    float k = *scale;
    while (n--) {
        *destP = k * *sourceP;
        sourceP += sourceStride;
        destP += destStride;
    }
}

void zvmul(const float* real1P, const float* imag1P, const float* real2P, const float* imag2P, float* realDestP, float* imagDestP, size_t framesToProcess)
{
    unsigned i = 0;
    for (; i < framesToProcess; ++i) {
        // Read and compute result before storing them, in case the
        // destination is the same as one of the sources.
        float realResult = real1P[i] * real2P[i] - imag1P[i] * imag2P[i];
        float imagResult = real1P[i] * imag2P[i] + imag1P[i] * real2P[i];

        realDestP[i] = realResult;
        imagDestP[i] = imagResult;
    }
}
*/
	
/*
float* FFTFrame::getUpToDateComplexData()
{
    // FIXME: if we can't completely get rid of this method, SSE
    // optimization could be considered if it shows up hot on profiles.
    int len = _FFTSize / 2;
    for (int i = 0; i < len; ++i) {
        int baseComplexIndex = 2 * i;
        _complexData[baseComplexIndex] = _realData[i];
        _complexData[baseComplexIndex + 1] = _imagData[i];
    }
    return const_cast<float*>(_complexData); //.data());
}
*/



/*
void FFTFrame::doInverseFFT(float* data)
{
    // Prepare interleaved data.
    float* interleavedData = getUpToDateComplexData();

    // Compute inverse transform.
    av_rdft_calc(_inverseContext, interleavedData);

    // Scale so that a forward then inverse FFT yields exactly the original data.
    const float scale = 1.0 / _FFTSize;
    vsmul(interleavedData, 1, &scale, data, 1, _FFTSize);
}
*/


/*
// Creates a blank/empty frame (interpolate() must later be called).
FFTFrame::FFTFrame()
    : _FFTSize(0)
    , _log2FFTSize(0)
    , _forwardContext(0)
    , _inverseContext(0)
{
}
*/
/*
void FFTFrame::multiply(const FFTFrame& frame)
{
    FFTFrame& frame1 = *this;
    FFTFrame& frame2 = const_cast<FFTFrame&>(frame);

    float* realP1 = frame1.realData();
    float* imagP1 = frame1.imagData();
    const float* realP2 = frame2.realData();
    const float* imagP2 = frame2.imagData();

    unsigned halfSize = fftSize() / 2;
    float real0 = realP1[0];
    float imag0 = imagP1[0];

    zvmul(realP1, imagP1, realP2, imagP2, realP1, imagP1, halfSize); 

    // Multiply the packed DC/nyquist component
    realP1[0] = real0 * realP2[0];
    imagP1[0] = imag0 * imagP2[0];

    // Scale accounts the peculiar scaling of vecLib on the Mac.
    // This ensures the right scaling all the way back to inverse FFT.
    // FIXME: if we change the scaling on the Mac then this scale
    // factor will need to change too.
    float scale = 0.5f;

    vsmul(realP1, 1, &scale, realP1, 1, halfSize);
    vsmul(imagP1, 1, &scale, imagP1, 1, halfSize);
}
*/



} // namespace Media 
} // namespace Sourcey