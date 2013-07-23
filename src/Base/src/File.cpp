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


#include "Sourcey/File.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Platform.h"
#include "assert.h"


using namespace std;


namespace scy {
	

/*
File::File(uv::Loop& loop) : 
	Base(&loop, new uv_fs_t)
{
	traceL("File", this) << "Creating" << endl;
}


File::~File()
{
	traceL("File", this) << "Destroying" << endl;
}


	//init();

	static void After(uv_fs_t *req) {
  HandleScope scope(node_isolate);

  FSReqWrap* req_wrap = (FSReqWrap*) req->data;
  assert(&req_wrap->req_ == req);

  // there is always at least one argument. "error"
  int argc = 1;

  // Allocate space for two args. We may only use one depending on the case.
  // (Feel free to increase this if you need more)
  Local<Value> argv[2];

  // NOTE: This may be needed to be changed if something returns a -1
  // for a success, which is possible.
  if (req->result == -1) {
    // If the request doesn't have a path parameter set.

    if (!req->path) {
      argv[0] = UVException(req->errorno,
                            NULL,
                            req_wrap->syscall());
    } else {
      argv[0] = UVException(req->errorno,
                            NULL,
                            req_wrap->syscall(),
                            static_cast<const char*>(req->path));
    }
  } else {
    // error value is empty or null for non-error.
    argv[0] = Null(node_isolate);

    // All have at least two args now.
    argc = 2;

    switch (req->fs_type) {
      // These all have no data to pass.
      case UV_FS_CLOSE:
      case UV_FS_RENAME:
      case UV_FS_UNLINK:
      case UV_FS_RMDIR:
      case UV_FS_MKDIR:
      case UV_FS_FTRUNCATE:
      case UV_FS_FSYNC:
      case UV_FS_FDATASYNC:
      case UV_FS_LINK:
      case UV_FS_SYMLINK:
      case UV_FS_CHMOD:
      case UV_FS_FCHMOD:
      case UV_FS_CHOWN:
      case UV_FS_FCHOWN:
        // These, however, don't.
        argc = 1;
        break;

      case UV_FS_UTIME:
      case UV_FS_FUTIME:
        argc = 0;
        break;

      case UV_FS_OPEN:
        argv[1] = Integer::New(req->result, node_isolate);
        break;

      case UV_FS_WRITE:
        argv[1] = Integer::New(req->result, node_isolate);
        break;

      case UV_FS_STAT:
      case UV_FS_LSTAT:
      case UV_FS_FSTAT:
        argv[1] = BuildStatsObject(static_cast<const uv_stat_t*>(req->ptr));
        break;

      case UV_FS_READLINK:
        argv[1] = String::New(static_cast<char*>(req->ptr));
        break;

      case UV_FS_READ:
        // Buffer interface
        argv[1] = Integer::New(req->result, node_isolate);
        break;

      case UV_FS_READDIR:
        {
          char *namebuf = static_cast<char*>(req->ptr);
          int nnames = req->result;

          Local<Array> names = Array::New(nnames);

          for (int i = 0; i < nnames; i++) {
            Local<String> name = String::New(namebuf);
            names->Set(Integer::New(i, node_isolate), name);
#ifndef NDEBUG
            namebuf += strlen(namebuf);
            assert(*namebuf == '\0');
            namebuf += 1;
#else
            namebuf += strlen(namebuf) + 1;
#endif
          }

          argv[1] = names;
        }
        break;

      default:
        assert(0 && "Unhandled eio response");
    }
  }

  if (oncomplete_sym.IsEmpty()) {
    oncomplete_sym = String::New("oncomplete");
  }
  MakeCallback(req_wrap->object(), oncomplete_sym, argc, argv);

  uv_fs_req_cleanup(&req_wrap->req_);
  delete req_wrap;
}
*/


} // namespace scy::uv