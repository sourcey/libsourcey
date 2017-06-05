///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup util
/// @{


#ifndef SCY_RateLimiter_H
#define SCY_RateLimiter_H


#include "scy/logger.h"


namespace scy {


/// @addtogroup util
/// A simple message rate limiter based on the token bucket algorithm.
class /* SCY_EXTERN */ RateLimiter
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

        STrace << "Can send:"
               << "\n\tAllowance: " << allowance
               << "\n\tElapsed: " << elapsed
               << "\n\tRate: " << rate
               << "\n\tSeconds: " << seconds << std::endl;

        if (allowance > rate) {
            allowance = rate; // throttle
            STrace << "Throttling: " << allowance << std::endl;
        } else if (allowance < 1.0) {
            STrace << "Message rate exceeded: " << allowance << std::endl;
            return false;
        }
        allowance -= 1.0;
        STrace << "Can send message: " << allowance << std::endl;
        return true;
    }
};


} // namespace scy


#endif // SCY_RateLimiter_H


/// @\}
