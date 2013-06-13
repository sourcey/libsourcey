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


#ifndef SOURCEY_RateLimiter_H
#define SOURCEY_RateLimiter_H


#include "Sourcey/Logger.h"


namespace Scy {


class RateLimiter
	/// A simple message rate limiter based on the token bucket algorithm.
{
public:	
	double rate;		// How many messages
	double seconds;		// Over how many seconds
	double allowance;	// Remaining send allowance
	clock_t lastCheck;  // Last time canSend() was called

	RateLimiter(double rate = 5.0, double seconds = 6.0) : 
		rate(rate), seconds(seconds), allowance(rate), lastCheck(0)
	{
	}

	bool canSend() 
	{
		clock_t current = clock();
		if (!lastCheck)
			lastCheck = current;
		double elapsed = (double)(current - lastCheck) / CLOCKS_PER_SEC;
		lastCheck = current;
		allowance += elapsed * (rate / seconds);
				
		LogTrace("RateLimiter") << "Can Send:"
			<< "\n\tAllowance: " << allowance
			<< "\n\tElapsed: " << elapsed
			<< "\n\tRate: " << rate
			<< "\n\tSeconds: " << seconds
			<< std::endl;

		if (allowance > rate) {
			allowance = rate; // throttle
			LogTrace("RateLimiter") << "Throttling: " 
				<< allowance << std::endl;
		}
		else if (allowance < 1.0) {
			LogTrace("RateLimiter") << "Message Rate Exceeded: " 
				<< allowance << std::endl;
			return false;
		}
		allowance -= 1.0;
		LogTrace("RateLimiter") << "Can Send Message: " 
			<< allowance << std::endl;
		return true;
	}
};


} // namespace Scy


#endif // SOURCEY_RateLimiter_H