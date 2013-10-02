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


#ifndef SOURCEY_Media_FFmpeg_H
#define SOURCEY_Media_FFmpeg_H


namespace scy {
namespace av {

	
void initializeFFmpeg();
	// Initialize the FFmpeg library.
	

void uninitializeFFmpeg();
	// Uninitializes the FFmpeg library.
	

} } // namespace scy::av


#endif // SOURCEY_Media_FFmpeg_H




	
	/*


#include "Sourcey/Media/Types.h"
#include <vector>
namespace internal {
	void api(int ret, const char* error = nullptr);
		// Check return values from FFmpeg and throw an exception if it failed.
	
	void throwError();
		// Throws the last FFmpeg error.

} // namespace internal
		*/