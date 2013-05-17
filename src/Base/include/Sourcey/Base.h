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


#ifndef SOURCEY_H
#define SOURCEY_H


//
/// Version number  
//
#define SOURCEY_MAJOR_VERSION    0
#define SOURCEY_MINOR_VERSION    8
#define SOURCEY_PATCH_VERSION    2

#define SOURCEY_AUX_STR_EXP(__A) #__A
#define SOURCEY_AUX_STR(__A)     SOURCEY_AUX_STR_EXP(__A)
#define SOURCEY_VERSION          SOURCEY_AUX_STR(SOURCEY_MAJOR_VERSION) "." SOURCEY_AUX_STR(SOURCEY_MINOR_VERSION) "." SOURCEY_AUX_STR(SOURCEY_PATCH_VERSION)


//
/// Cross platform configuration
//
#include "LibSourceyConfig.h"

#ifdef _WIN32
    // Windows (x64 and x86)
    #define WIN32 1
#endif
#if __unix__ // all unices
    // Unix    
    #define UNIX
#endif
#if __posix__
    // POSIX    
    #define POSIX
#endif
#if __linux__
    // linux    
    #define LINUX
#endif
#if __GNUC__
    // GCC specific
    #define GNUC
#endif
#if __APPLE__
    // Mac OS
    #define APPLE
#endif


#endif // SOURCEY_H
