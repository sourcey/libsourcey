//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H


#include <time.h>


namespace Sourcey {
namespace Media {


struct FPSCounter 
{
	clock_t start;
	clock_t end;
	double duration;
	double fps;
	double frames;

	FPSCounter() {
		reset();
	}

	void tick() {	
		if (started()) {
			endFrame();
		}		
		startFrame();
	}

	void reset() {
		start = 0;
		end = 0;
		duration = 0;
		fps = 0;
		frames = 0;
	}

	bool started() {
		return start != 0;
	}

	void startFrame() {
		start = clock();
	}

	double endFrame() {
		end = clock();
		duration += (double)(end - start) / CLOCKS_PER_SEC;
		frames++;
		fps = frames / duration;
		return fps;
	}
};


} } // namespace Sourcey::Media


#endif // FPS_COUNTER_H