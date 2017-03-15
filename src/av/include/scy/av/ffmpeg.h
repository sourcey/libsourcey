///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_FFmpeg_H
#define SCY_AV_FFmpeg_H


#include "scy/base.h"
#include "string"


namespace scy {
namespace av {


void initializeFFmpeg(); /// Initialize the FFmpeg library.

void uninitializeFFmpeg(); /// Uninitializes the FFmpeg library.

std::string averror(const int error); /// Get an error string for the given error code.

void printInputFormats(std::ostream& ost, const char* delim = " ");
void printOutputFormats(std::ostream& ost, const char* delim = " ");
void printEncoders(std::ostream& ost, const char* delim = " ");


} // namespace av
} // namespace scy


#endif // SCY_Media_FFmpeg_H


/// @\}
