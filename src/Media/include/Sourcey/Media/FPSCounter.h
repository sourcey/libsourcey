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

#ifndef SOURCEY_MEDIA_FPSCounter_H
#define SOURCEY_MEDIA_FPSCounter_H


#include "Sourcey/PacketStream.h"

#include <time.h>


namespace scy {
namespace av {


class FPSCounter 
	/// An FPS counter based on the simple moving average (SMA) algorithm
{
private:
	static const int MAX = 100;

	int tickIndex;
	clock_t prevTick;
	clock_t tickSum;
	clock_t tickList[MAX];

	double updateAvg(clock_t newTick)
	{
		tickSum -= tickList[tickIndex];  // subtract value falling off
		tickSum += newTick;              // add new value
		tickList[tickIndex] = newTick;   // save new value so it can be subtracted later
		tickIndex = (tickIndex+1) % MAX;
		
		return((double)tickSum / MAX);   // return average
	}

public:
	double fps;
	Int64 frames;

	FPSCounter()
	{
		reset();
	}

	void reset() 
	{
		fps = 0;
		frames = 0;
		tickIndex = 0;
		tickSum = 0;
		prevTick = 0;
		for (int i = 0; i < MAX; i++)
			tickList[i] = 0;
	}
	
	void tick()
	{
		frames++;
		clock_t newTick = clock();
		double avgTick = updateAvg(newTick - prevTick);
		prevTick = newTick;
		if (avgTick == 0.)
			fps = 0.0; //-1.;
		else 
			fps = CLOCKS_PER_SEC / avgTick;
	}
};

	
namespace legacy {

struct FPSCounter 
{
	clock_t start;
	clock_t end;
	Int64 frames;
	double total;
	double fps;

	FPSCounter()
	{
		reset();
	}

	void tick() 
	{	
		if (started())
			endFrame();
		startFrame();
	}

	void reset() 
	{
		start = 0;
		end = 0;
		total = 0;
		fps = 0;
		frames = 0;
	}

	bool started() 
	{
		return start != 0;
	}

	void startFrame() 
	{
		start = clock();
	}

	double endFrame() 
	{
		end = clock();
		total += (double)(end - start) / CLOCKS_PER_SEC;
		frames++;
		fps = (1.0 * frames) / total;
		return fps;
	}
};

} // legacy


class FPSLimiter: public PacketProcessor
	/// This class limits the throughput rate of IPackets
	/// in a PacketStream. If the throughput rate exceeds the
	/// max specified FPS then packets will be dropped.
	///	
	/// Note that upstream processors should not fragment packets
	/// otherwise this class will not be accurate, and the packet 
	/// drop rate will be too high.
{
public:
	FPSLimiter(int max) : 
		PacketProcessor(Emitter), _max(max)
	{
	}

	virtual void process(IPacket& packet) 
	{
		traceL("FPSLimiter", this) << "Processing" << std::endl;
		_counter.tick();
		if (static_cast<int>(_counter.fps) > _max) {			
			traceL("FPSLimiter", this) << "####################### Dropping packet: " 
				<< _counter.fps << " > " << _max << std::endl;
			return;
		}
		emit(packet);
	};
		
	PacketSignal Emitter;

	// virtual void onStreamStateChange(const PacketStreamState&) {};

protected:	
	int _max;
	FPSCounter _counter;
};


} } // namespace scy::av


#endif // SOURCEY_MEDIA_FPSCounter_H

	


/*
#define MAX 100
int tickIndex=0
int tickSum=0;
int tickList[MAX];

// need to zero out the tickList array before starting
// average will ramp up until the buffer is full 
// returns average ticks per frame over the MAXSAMPPLES last frames

double CalcAverageTick(int newTick)
{
    tickSum-=tickList[tickIndex];  // subtract value falling off
    tickSum+=newTick;              // add new value
    tickList[tickIndex]=newTick;   // save new value so it can be subtracted later
    if(++tickIndex==MAX)    // inc buffer index
        tickIndex=0;

    / return average
    return((double)tickSum/MAX);
}
*/



	/*
	double getFps()
	{
		clock_t newTick = clock();
		double avgTick = updateAvg(newTick-prevTick);
		prevTick = newTick;
		if (avgTick==0.) {
			return -1.;
		}
		return CLOCKS_PER_SEC/avgTick;
	}
	*/

/*
FPSCounter::FPSCounter() {
	tickIndex=0;
	tickSum=0;
	prevTick=0;
	for(int i=0;i<MAX;i++){
		tickList[i]=0;
	}
}


inline double FPSCounter::updateAvg(clock_t newTick)


inline double FPSCounter::getFps()


struct FPSCounter 
{
	clock_t start;
	clock_t end;
	int lastMS;
	Int64 totalMS;
	Int64 frames;
	double fps;

	FPSCounter()
	{
		reset();
	}

	void tick() 
	{	
		if (started())
			endFrame();
		startFrame();
	}

	void reset() 
	{
		start = 0;
		end = 0;
		lastMS = 0;
		totalMS = 0;
		fps = 0;
		frames = 0;
	}

	bool started() 
	{
		return start != 0;
	}

	void startFrame() 
	{
		start = clock();
	}

	double endFrame() 
	{
		end = clock();
		lastMS = (end - start); //(double)
		totalMS += lastMS; //(double)(end - start) / CLOCKS_PER_SEC;
		frames++;
		fps = (1.0 * frames) / (totalMS / CLOCKS_PER_SEC); //
		traceL() << "[FPSLimiter: " << this <<"] FPS MS: " << std::endl;
		traceL() << "[FPSLimiter: " << this <<"] Processing" << std::endl;
		return fps;
	}
};
*/
	/*
	virtual FPSCounter& counter()
	{
		return _counter;
	};
	*/


		/*

	virtual bool accepts(IPacket&)
		// Reject the packet if we have exceeded the maximum FPS.
	{ 
		traceL() << "[FPSLimiter: " << this <<"] Accepting: \n" 
			<< "\n\tFPS Limit: " << _max
			<< "\n\tCurrent FPS: " << _counter.fps
			<< "\n\tAcceptable: " << (static_cast<int>(_counter.fps) <= _max)
			<< std::endl;
		
		if (_counter.started()) {
			_counter.endFrame();
			if (_counter.fps > _max)
				return false;
		}
		_counter.startFrame();
		*/
		/*
		_counter.tick();
		if (_counter.fps > _max)
			return false;
		if () {
			_counter.endFrame();
			if (_counter.fps > _max)
				return false;
		}
		return true; 
	};
		*/