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


#ifndef SCY_SharedLibrary_H
#define SCY_SharedLibrary_H


#include "scy/util.h"
#include "scy/uv/uvpp.h"


namespace scy { 
    

struct SharedLibrary 
{
    bool open(const std::string& path)
        // Opens a shared library. The filename is in utf-8. Returns true on success and
        // false on error. Call `SharedLibrary::error()` to get the error message.
    {        
        if (uv_dlopen(path.c_str(), &_lib)) {
            setError("Cannot load library");
            return false;
        }
        return true;
    }
    
    void close()        
        // Closes the shared library.
    {        
        uv_dlclose(&_lib);
    }
    
    bool sym(const char* name, void** ptr)        
        // Retrieves a data pointer from a dynamic library. It is legal for a symbol to
        // map to nullptr. Returns 0 on success and -1 if the symbol was not found.
    {        
        if (uv_dlsym(&_lib, name, ptr)) {
            setError(util::format("Symbol '%s' not found.", name));
            return false;
        }
        return true;
    }

    void setError(const std::string& prefix)
    {    
        std::string err(uv_dlerror(&_lib));
        assert(!err.empty());
        _error = prefix + ": " + err;
        throw std::runtime_error(prefix + ": " + err);
    }
    
    std::string error() const
    {    
        return _error;
    }

protected:
    uv_lib_t _lib;
    std::string _error;
};


} // namespace scy


#endif // SCY_SharedLibrary_H