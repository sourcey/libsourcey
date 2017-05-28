///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Error_H
#define SCY_Error_H


#include "scy/base.h"

#include "uv.h"

#include <exception>
#include <stdexcept>
#include <string>


namespace scy {


/// Basic error type.
///
/// Errors contain an error code, message, and exception pointer.
struct Error
{
    int errorno;
    std::string message;
    std::exception_ptr exception;

    Error() { reset(); }

    Error(const std::string& msg)
    {
        reset();
        message = msg;
    }

    Error(const char* msg)
    {
        reset();
        message = msg;
    }

    bool any() const
    {
        return errorno != 0 || !message.empty() || exception != nullptr;
    }

    void reset()
    {
        errorno = 0;
        message.clear();
        exception = nullptr;
    }

    void rethrow()
    {
        if (exception)
            std::rethrow_exception(exception);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Error& err)
    {
        stream << err.message;
        return stream;
    }
};


//
// UV Error Helpers
//


namespace uv {


inline std::string formatError(const std::string& message, int errorno = UV_UNKNOWN)
{
    std::string m(message); // prefix the message since libuv errors are brisk
    if (errorno != UV_UNKNOWN && errorno != 0) {
        if (!m.empty())
            m.append(": ");
        m.append(uv_strerror(errorno));
    }
    return m;
}


inline void throwError(const std::string& message, int errorno = UV_UNKNOWN)
{
    throw std::runtime_error(formatError(message, errorno));
}


} // namespace uv
} // namespace scy


#endif // SCY_Error_H


/// @\}
