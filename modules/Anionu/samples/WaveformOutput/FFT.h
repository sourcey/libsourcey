#ifndef _FFT_H_
#define _FFT_H_

#include <complex>
#include <vector>

class FFT
{
    public:
        typedef std::complex<double> Complex;
        
        /* Initializes FFT. n must be a power of 2. */
        FFT(int n, bool inverse = false);
        /* Computes Discrete Fourier Transform of given buffer. */
        std::vector<Complex> transform(const std::vector<Complex>& buf);
        static double getIntensity(Complex c);
        static double getPhase(Complex c);
        
    private:
        int n, lgN;
        bool inverse;
        std::vector<Complex> omega;
        std::vector<Complex> result;
        
        void bitReverseCopy(const std::vector<Complex>& src,
                std::vector<Complex>& dest) const;
};

#endif


/*
#if !defined FFT_H
#define FFT_H
//------------------------------------
//  Reliable Software, (c) 1996-2002
//------------------------------------
#include <complex>
#include <vector>
#include <cassert>

typedef std::complex<double> Complex;

class Fft
{
public:
    Fft (unsigned points, unsigned sampleRate);
	// Hard work
    void    Transform (std::vector<int> const & data);
	// Output: from i = 0 to points-1
    double  GetIntensity (unsigned i) const
    { 
        assert (i < _points);
        return std::abs (_X[i])/_sqrtPoints; 
    }
    unsigned Points () const { return _points; }
	// return frequency in Hz of a given point
    int GetFrequency (unsigned point) const
    {
        assert (point < _points);
        long x =_sampleRate * point;
        return x / _points;
    }
    int HzToPoint (int freq) const 
    { 
        return (long)_points * freq / _sampleRate; 
    }
    int MaxFreq() const { return _sampleRate; }

private:
	void DataIn (std::vector<int> const & data);
    void PutAt (unsigned i, double val)
    {
        _X [_aBitRev[i]] = Complex (val);
    }
private:
    unsigned	_points;
    unsigned	_sampleRate;
    unsigned	_logPoints;		// binary log of _points
    double		_sqrtPoints;	// square root of _points

	std::vector<unsigned>				_aBitRev;	// bit reverse vector
	std::vector<Complex>				_X;	// in-place fft array
	std::vector<std::vector<Complex> >	_W;	// exponentials
	std::vector<double>					_tape;	// recording tape
};

#endif
*/