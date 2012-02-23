#include "FFT.h"
#include <vector>
#include <cassert>

#define PI 3.14159265

using namespace std;

FFT::FFT(int n, bool inverse)
    : n(n), inverse(inverse), result(vector<Complex>(n))
{
    lgN = 0;
    for (int i = n; i > 1; i >>= 1)
    {
        ++lgN;
        assert((i & 1) == 0);
    }
    omega.resize(lgN);
    int m = 1;
    for (int s = 0; s < lgN; ++s)
    {
        m <<= 1;
        if (inverse)
            omega[s] = exp(Complex(0, 2.0 * PI / m));
        else
            omega[s] = exp(Complex(0, -2.0 * PI / m));
    }
}

std::vector<FFT::Complex> FFT::transform(const vector<Complex>& buf)
{
    bitReverseCopy(buf, result);
    int m = 1;
    for (int s = 0; s < lgN; ++s)
    {
        m <<= 1;
        for (int k = 0; k < n; k += m)
        {
            Complex current_omega = 1;
            for (int j = 0; j < (m >> 1); ++j)
            {
                Complex t = current_omega * result[k + j + (m >> 1)];
                Complex u = result[k + j];
                result[k + j] = u + t;
                result[k + j + (m >> 1)] = u - t;
                current_omega *= omega[s];
            }
        }
    }
    if (inverse == false)
        for (int i = 0; i < n; ++i)
            result[i] /= n;
    return result;
}

double FFT::getIntensity(Complex c)
{
    return abs(c);
}

double FFT::getPhase(Complex c)
{
    return arg(c);
}

void FFT::bitReverseCopy(const vector<Complex>& src, vector<Complex>& dest) const
{
    for (int i = 0; i < n; ++i)
    {
        int index = i, rev = 0;
        for (int j = 0; j < lgN; ++j)
        {
            rev = (rev << 1) | (index & 1);
            index >>= 1;
        }
        dest[rev] = src[i];
    }
}


/*
#include "Fft.h"
#include "Poco/Exception.h"


using namespace std;
using namespace Poco;


// log (1) = 0, log(2) = 1, log(3) = 2, log(4) = 2 ...

const double PI = 2.0 * asin (1.0);

// points must be a power of 2

Fft::Fft (unsigned points, unsigned sampleRate)
	: _points (points), _sampleRate (sampleRate)
{
    _tape.resize (_points);
    _sqrtPoints = sqrt (static_cast<double> (_points));
    // calculate binary log
    _logPoints = 0;
    points--;
    while (points != 0)
    {
        points >>= 1;
        _logPoints++;
    }

    _aBitRev.resize (_points);
    _X.resize (_points);
    _W.resize (_logPoints + 1);
    // Precompute complex exponentials
    int _2_l = 2;
    for (unsigned l = 1; l <= _logPoints; l++)
    {
        _W[l].resize (_points);

        for (unsigned i = 0; i != _points; i++ )
        {
            double re =  cos (2. * PI * i / _2_l);
            double im = -sin (2. * PI * i / _2_l);
            _W[l][i] = Complex (re, im);
        }
        _2_l *= 2;
    }

    // set up bit reverse mapping
    int rev = 0;
    unsigned halfPoints = _points/2;
    for (unsigned i = 0; i < _points - 1; i++)
    {
        _aBitRev [i] = static_cast<unsigned> (rev);
        unsigned mask = halfPoints;
        // add 1 backwards
        while (rev >= static_cast<int> (mask))
        {
            rev -= mask; // turn off this bit
            mask >>= 1;
        }
        rev += mask;
    }
    _aBitRev [_points-1] = _points-1;
}

void Fft::DataIn (std::vector<int> const & data)
{
	if (data.size () > _points)
		throw Exception ("Sample buffer larger than FFT capacity");
    // make space for samples at the end of tape
    // shifting previous samples towards the beginning
	// to           from
	// v-------------|
	// xxxxxxxxxxxxxxyyyyyyyyy
	// yyyyyyyyyoooooooooooooo
	// <- old -><- free tail->
	std::copy (_tape.begin () + data.size (), _tape.end (), _tape.begin ());
	std::copy (data.begin (), data.end (), _tape.begin () + (_points - data.size ()));

    // Initialize the FFT buffer
    for (unsigned i = 0; i != _points; ++i)
        PutAt (i, _tape [i]);
}

//----------------------------------------------------------------
//               0   1   2   3   4   5   6   7
//  level   1
//  step    1                                     0
//  increm  2                                   W 
//  j = 0        <--->   <--->   <--->   <--->   1
//  level   2
//  step    2
//  increm  4                                     0
//  j = 0        <------->       <------->      W      1
//  j = 1            <------->       <------->   2   W
//  level   3                                         2
//  step    4
//  increm  8                                     0
//  j = 0        <--------------->              W      1
//  j = 1            <--------------->           3   W      2
//  j = 2                <--------------->            3   W      3
//  j = 3                    <--------------->             3   W
//                                                              3
//----------------------------------------------------------------

void Fft::Transform (std::vector<int> const & data)
{
	DataIn (data);

    // step = 2 ^ (level-1)
    // increm = 2 ^ level;
    unsigned step = 1;
    for (unsigned level = 1; level <= _logPoints; level++)
    {
        unsigned increm = step * 2;
        for (unsigned j = 0; j < step; j++)
        {
            // U = exp ( - 2 PI j / 2 ^ level )
            Complex U = _W [level][j];
            for (unsigned i = j; i < _points; i += increm)
            {
                // butterfly
                Complex T = U;
                T *= _X [i+step];
                _X [i+step] = _X[i];
                _X [i+step] -= T;
                _X [i] += T;
            }
        }
        step *= 2;
    }
}
*/