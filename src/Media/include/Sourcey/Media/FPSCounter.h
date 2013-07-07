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


#include <time.h>

#include "Sourcey/IPacketProcessor.h"


namespace scy {
namespace av {


struct FPSCounter 
{
	clock_t start;
	clock_t end;
	double duration;
	double fps;
	UInt64 frames;

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
		duration = 0;
		fps = 0.0;
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
		duration += (double)(end - start) / CLOCKS_PER_SEC;
		frames++;
		fps = (1.0 * frames) / duration;
		return fps;
	}
};


class FPSLimiter: public IPacketProcessor
	/// This class limits the throughput rate of IPackets
	/// in a PacketStream.
	/// If the throughput rate exceeds the max secified FPS
	/// packets will be dropped.
{
public:
	FPSLimiter(int max) : _max(max)
	{
	}

	virtual bool accepts(IPacket&)
		/// Reject the packet if we have exceeded the maximum FPS.
	{ 
		/*
		traceL() << "[FPSLimiter:" << this <<"] Accepting: \n" 
			<< "\n\tFPS Limit: " << _max
			<< "\n\tCurrent FPS: " << _counter.fps
			<< "\n\tAcceptable: " << (static_cast<int>(_counter.fps) <= _max)
			<< std::endl;
			*/
		
		if (_counter.started()) {
			_counter.endFrame();
			if (_counter.fps > _max)
				return false;
		}
		_counter.startFrame();
		return true; 
	};

	virtual void process(IPacket& packet) 
	{
		//traceL() << "[FPSLimiter:" << this <<"] Processing" << std::endl;
		_counter.tick();
		emit(this, packet);
	};
	
	virtual FPSCounter& counter()
	{
		return _counter;
	};

protected:	
	int _max;
	FPSCounter _counter;
};


} } // namespace scy::av


#endif // SOURCEY_MEDIA_FPSCounter_H