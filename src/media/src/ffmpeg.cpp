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

#include "scy/media/ffmpeg.h"

#ifdef HAVE_FFMPEG

#include "scy/mutex.h"

#include <stdexcept>

extern "C" {
#include <libavformat/avformat.h>
}


namespace scy {
namespace av {
namespace internal {


static int LockManagerOperation(void** lock, enum AVLockOp op) {
  switch (op) {
    case AV_LOCK_CREATE:
      *lock = new Mutex();
      if (!*lock)
        return 1;
      return 0;

    case AV_LOCK_OBTAIN:
      static_cast<Mutex*>(*lock)->lock();
      return 0;

    case AV_LOCK_RELEASE:
      static_cast<Mutex*>(*lock)->unlock();
      return 0;

    case AV_LOCK_DESTROY:
      delete static_cast<Mutex*>(*lock);
      *lock = NULL;
      return 0;
  }
  return 1;
}


static Mutex _mutex;
static int _refCount(0);


void initialize()
{
    Mutex::ScopedLock lock(_mutex);

    if (++_refCount == 1)
    {
        // Before doing anything disable logging as it interferes with layout tests.
        //av_log_set_level(AV_LOG_QUIET);

        // Register our protocol glue code with FFmpeg.
        av_lockmgr_register(&LockManagerOperation);

        // Now register the rest of FFmpeg.
        av_register_all();
    }
}


void uninitialize()
{
    Mutex::ScopedLock lock(_mutex);

    if (--_refCount == 0) {
        av_lockmgr_register(NULL);
    }
}


} // namespace internal


void initializeFFmpeg()
{
    internal::initialize();
}


void uninitializeFFmpeg()
{
    internal::uninitialize();
}


std::string averror(const int error)
{
   static char error_buffer[255];
   av_strerror(error, error_buffer, sizeof(error_buffer));
   return error_buffer;
}


} } // namespace scy::av


#endif
