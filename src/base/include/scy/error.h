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


#ifndef SCY_Error_H
#define SCY_Error_H


#include "scy/base.h"
#include <string>
#include <exception> // use std::exception types
#include <stdexcept>


namespace scy {


//
// Error type
//


struct Error
{
    int errorno;
    std::string message;
    std::exception_ptr exception;

    Error()
    {
        reset();
    }

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

    friend std::ostream& operator << (std::ostream& stream, const Error& err)
    {
        stream << err.message;
        return stream;
    }
};


} // namespace scy


#endif // SCY_Error_H
