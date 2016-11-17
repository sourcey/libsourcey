///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Interfaces_H
#define SCY_Interfaces_H


#include <cstdint>
#include <stdexcept>
#include <atomic>
#include <functional>
#include <memory>


namespace scy {


/// Forward the LogStream for logger.h
struct LogStream;


namespace basic { // interface pieces


class Decoder
{
public:
    Decoder() {}
    virtual ~Decoder() {}
    virtual std::size_t decode(const char* inbuf, std::size_t nread, char* outbuf) = 0;
    virtual std::size_t finalize(char* /* outbuf */) { return 0; }
};


class Encoder
{
public:
    Encoder() {}
    virtual ~Encoder() {}
    virtual std::size_t encode(const char* inbuf, std::size_t nread, char* outbuf) = 0;
    virtual std::size_t finalize(char* /* outbuf */) { return 0; }
};


#if 0
/// A base module class for C++ callback polymorphism.
/// @deprecated
class Polymorphic
{
public:
    virtual ~Polymorphic() {};

    template<class T>
    bool is() {
        return dynamic_cast<T*>(this) != nullptr;
    };

    template<class T>
    T* as(bool whiny = false) {
        T* self = dynamic_cast<T*>(this);
        if (self == nullptr && whiny)
            throw std::runtime_error("Polymorphic cast failed");
        return self;
    };

    scy::LogStream& log(const char* level = "debug") const;

    virtual const char* className() const = 0;
};
#endif


/// The LibSourcey base module type.
/// May become a class type in the future.
/// depreciated
// typedef Polymorphic Module;


} // namespace basic
} // namespace scy


#endif // SCY_Interfaces_H

/// @\}
