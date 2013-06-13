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


#ifndef SOURCEY_MEDIA_FFT_H
#define SOURCEY_MEDIA_FFT_H


#include "Sourcey/Base.h"

extern "C" {
#include <libavcodec/avfft.h>
}

//#include <iostream>
//#include <string>
//#include <vector>


namespace Sourcey {
namespace Media {


	/*
template<typename T>
class AudioArray {
public:
    AudioArray() : _allocation(0), _alignedData(0), _size(0) { }
    explicit AudioArray(size_t n) : _allocation(0), _alignedData(0), _size(0)
    {
        allocate(n);
    }

    ~AudioArray()
    {
        fastFree(_allocation);
    }

    // It's OK to call allocate() multiple times, but data will *not* be copied from an initial allocation
    // if re-allocated. Allocations are zero-initialized.
    void allocate(size_t n)
    {
        // Although n is a size_t, its true limit is max unsigned because we use unsigned in zeroRange()
        // and copyToRange(). Also check for integer overflow.
        if (n > std::numeric_limits<unsigned>::max() / sizeof(T))
            CRASH();
      
        unsigned initialSize = sizeof(T) * n;

//#if USE(WEBAUDIO_FFMPEG) || USE(WEBAUDIO_OPENMAX_DL_FFT)
        const size_t alignment = 32;
//#else
//        const size_t alignment = 16;
//#endif

        if (_allocation)
            fastFree(_allocation);
        
        bool isAllocationGood = false;
        
        while (!isAllocationGood) {
            // Initially we try to allocate the exact size, but if it's not aligned
            // then we'll have to reallocate and from then on allocate extra.
            static size_t extraAllocationBytes = 0;

            // Again, check for integer overflow.
            if (initialSize + extraAllocationBytes < initialSize)
                CRASH();

            T* allocation = static_cast<T*>(fastMalloc(initialSize + extraAllocationBytes));
            if (!allocation)
                CRASH();
            T* alignedData = alignedAddress(allocation, alignment);

            if (alignedData == allocation || extraAllocationBytes == alignment) {
                _allocation = allocation;
                _alignedData = alignedData;
                _size = n;
                isAllocationGood = true;
                zero();
            } else {
                extraAllocationBytes = alignment; // always allocate extra after the first alignment failure.
                fastFree(allocation);
            }
        }
    }

    T* data() { return _alignedData; }
    const T* data() const { return _alignedData; }
    size_t size() const { return _size; }

    T& at(size_t i)
    {
        // Note that although it is a size_t, _size is now guaranteed to be
        // no greater than max unsigned. This guarantee is enforced in allocate().
        ASSERT_WITH_SECURITY_IMPLICATION(i < size());
        return data()[i];
    }

    T& operator[](size_t i) { return at(i); }

    void zero()
    {
        // This multiplication is made safe by the check in allocate().
        memset(this->data(), 0, sizeof(T) * this->size());
    }

    void zeroRange(unsigned start, unsigned end)
    {
        bool isSafe = (start <= end) && (end <= this->size());
        ASSERT(isSafe);
        if (!isSafe)
            return;

        // This expression cannot overflow because end - start cannot be
        // greater than _size, which is safe due to the check in allocate().
        memset(this->data() + start, 0, sizeof(T) * (end - start));
    }

    void copyToRange(const T* sourceData, unsigned start, unsigned end)
    {
        bool isSafe = (start <= end) && (end <= this->size());
        ASSERT(isSafe);
        if (!isSafe)
            return;

        // This expression cannot overflow because end - start cannot be
        // greater than _size, which is safe due to the check in allocate().
        memcpy(this->data() + start, sourceData, sizeof(T) * (end - start));
    }

    template<typename MemoryObjectInfo>
    void reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
    {
        typename MemoryObjectInfo::ClassInfo info(memoryObjectInfo, this);
        info.addRawBuffer(_allocation, _size * sizeof(T), "AudioArrayData", "allocation");
        info.ignoreMember(_alignedData);
    }

private:
    static T* alignedAddress(T* address, intptr_t alignment)
    {
        intptr_t value = reinterpret_cast<intptr_t>(address);
        return reinterpret_cast<T*>((value + alignment - 1) & ~(alignment - 1));
    }

    T* _allocation;
    T* _alignedData;
    size_t _size;
};

typedef AudioArray<float> AudioFloatArray;
typedef AudioArray<double> AudioDoubleArray;


#ifdef WIN32
double log2(double n) {
	return log(n) / log(double(2));
}
#endif


inline void vsmul(const float* sourceP, int sourceStride, const float* scale, float* destP, int destStride, size_t framesToProcess);

// Multiplies two complex vectors.
inline void zvmul(const float* real1P, const float* imag1P, const float* real2P, const float* imag2P, float* realDestP, float* imagDestP, size_t framesToProcess);


// Defines the interface for an "FFT frame", an object which is able to perform a forward
// and reverse FFT, internally storing the resultant frequency-domain data.

class FFTFrame 
{
public:
    FFTFrame(unsigned fftSize);
    //FFTFrame(); // creates a blank/empty frame for later use with createInterpolatedFrame()
    FFTFrame(const FFTFrame& frame);
    ~FFTFrame();

    void doFFT(const float* data);
    //void doInverseFFT(float* data);
    //void multiply(const FFTFrame& frame); // multiplies ourself with frame : effectively operator*=()

    float* realData() const;
    float* imagData() const;

    //void print(); // for debugging

    // CROSS-PLATFORM
    // The remaining public methods have cross-platform implementations:

    // Interpolates from frame1 -> frame2 as x goes from 0.0 -> 1.0
    //static PassOwnPtr<FFTFrame> createInterpolatedFrame(const FFTFrame& frame1, const FFTFrame& frame2, double x);

    //void doPaddedFFT(const float* data, size_t dataSize); // zero-padding with dataSize <= fftSize
    //double extractAverageGroupDelay();
    //void addConstantGroupDelay(double sampleFrameDelay);

    //void reportMemoryUsage(MemoryObjectInfo*) const;

    unsigned fftSize() const { return _FFTSize; }
    unsigned log2FFTSize() const { return static_cast<unsigned>(log2(_FFTSize)); }

private:
    unsigned _FFTSize;
    unsigned _log2FFTSize;

    //void interpolateFrequencyComponents(const FFTFrame& frame1, const FFTFrame& frame2, double x);
	
    static RDFTContext* contextForSize(unsigned fftSize, RDFTransformType trans);

    RDFTContext* _forwardContext;
    //RDFTContext* _inverseContext;

    //float* getUpToDateComplexData();
    float* _complexData;
    float* _realData;
    float* _imagData;
};
*/


} // namespace Media
} // namespace Sourcey


#endif




	/*
	enum ID 
	{
		Unknown		= 0,
		Raw			= 1,

		/// Video
		MP4			= 2,
		FLV			= 3,
		OGG			= 4,
		AVI			= 5,
		MJPEG		= 6,
		MPNG		= 7,

		/// Audio
		M4A			= 20,
		MP3			= 21
	};
	*/
	//
	/// Methods
	//	
	//virtual std::string extension() const;
		/// Returns the file extension for this format.
		/// The default implementation just transforms the 
		/// id string to lowercase.
		/// TODO: Consider removing or making a member variable

	//virtual std::string name() const;
		/// Returns a string representation of the Codec name.
		/// The default implementation uses idToName.
	

	//virtual std::string encoderName() const;
		/// Returns the encoder name for this format.
		/// The default implementation uses idToEncoderName.
		/// This function should return the short name
		/// for use with av_guess_format.
	//static ID toID(const std::string& name);	
	//static std::string idString(const char* id);
	//static std::string encoderName(const char* id);
	//static ID nameToID(const std::string& name);
	//static std::string idToName(const char* id);
	//static std::string idToEncoderName(const char* id);