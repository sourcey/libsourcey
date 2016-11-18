///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup util
/// @{


#ifndef SCY_RateLimiter_H
#define SCY_RateLimiter_H


#include "scy/logger.h"


namespace scy {


/// @addtogroup util
/// A simple message rate limiter based on the token bucket algorithm.
class RateLimiter
{
public:
    double rate;       ///< How many messages
    double seconds;    ///< Over how many seconds
    double allowance;  ///< Remaining send allowance
    clock_t lastCheck; ///< Last time canSend() was called

    RateLimiter(double rate = 5.0, double seconds = 6.0)
        : rate(rate)
        , seconds(seconds)
        , allowance(rate)
        , lastCheck(0)
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

        traceL("RateLimiter")
            << "Can Send:"
            << "\n\tAllowance: " << allowance << "\n\tElapsed: " << elapsed
            << "\n\tRate: " << rate << "\n\tSeconds: " << seconds << std::endl;

        if (allowance > rate) {
            allowance = rate; // throttle
            traceL("RateLimiter") << "Throttling: " << allowance << std::endl;
        } else if (allowance < 1.0) {
            traceL("RateLimiter") << "Message Rate Exceeded: " << allowance
                                  << std::endl;
            return false;
        }
        allowance -= 1.0;
        traceL("RateLimiter") << "Can Send Message: " << allowance << std::endl;
        return true;
    }
};


} // namespace scy


#endif // SCY_RateLimiter_H


/// @\}
