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


#ifndef SCY_AV_FFmpeg_H
#define SCY_AV_FFmpeg_H


#include "scy/base.h"
#include "string"


namespace scy {
namespace av {


void initializeFFmpeg();
    // Initialize the FFmpeg library.

void uninitializeFFmpeg();
    // Uninitializes the FFmpeg library.

std::string averror(const int error);
    // Get an error string for the given error code.

void printInputFormats(std::ostream& ost, const char* delim = " ");
void printOutputFormats(std::ostream& ost, const char* delim = " ");
void printEncoders(std::ostream& ost, const char* delim = " ");


} } // namespace scy::av


#endif // SCY_Media_FFmpeg_H
