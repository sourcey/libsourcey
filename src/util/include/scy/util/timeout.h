///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{
//


#ifndef SCY_Timeout_H
#define SCY_Timeout_H


#include "scy/base.h"
#include <string>
#include <ctime>


namespace scy {


//
// Timeout
//


/// Timeout counter which expires after a given delay.
class Timeout
{
public:
    Timeout(long delay = 0, bool autoStart = false);
    Timeout(const Timeout& src);
    ~Timeout();

    bool running() const;
    void start();
    void stop();
    void reset();
    long remaining() const;
    bool expired() const;

    void setDelay(long delay) { _delay = delay; };

    clock_t startAt() const { return _startAt; };
    long delay() const { return _delay; };

    Timeout& operator=(const Timeout& src);

protected:
    clock_t _startAt;
    long _delay;
};


//
// Timed Token
//


/// Token that expires after the specified duration.
class Base_API TimedToken : public Timeout
{
public:
    TimedToken(long duration = 1000);
    TimedToken(const std::string& id, long duration = 1000);

    std::string id() const { return _id; }

    bool operator==(const TimedToken& r) const { return _id == r._id; }
    bool operator==(const std::string& r) const { return _id == r; }

protected:
    std::string _id;
};


} // namespace scy


#endif // SCY_Timeout_H
