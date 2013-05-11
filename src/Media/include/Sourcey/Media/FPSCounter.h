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


#ifndef SOURCEY_MEDIA_FPSCounter_H
#define SOURCEY_MEDIA_FPSCounter_H


#include <time.h>

#include "Sourcey/IPacketProcessor.h"


namespace Scy {
namespace Media {


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
		LogTrace() << "[FPSLimiter:" << this <<"] Accepting: \n" 
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
		//LogTrace() << "[FPSLimiter:" << this <<"] Processing" << std::endl;
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


} } // namespace Scy::Media


#endif // SOURCEY_MEDIA_FPSCounter_H