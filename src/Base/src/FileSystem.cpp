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


#include "Sourcey/FileSystem.h"
#include "Sourcey/UV/UVPP.h"
#include <fstream>

#if defined(WIN32) && defined(UNICODE)
#include <locale>
#include <codecvt>
#endif

//#define ALLOWABLE_DIRECTORY_DELIMITERS "/\\"
//#define DIRECTORY_DELIMITER '\\'
//#define DIRECTORY_DELIMITER_STRING "\\"
//#define ALLOWABLE_DIRECTORY_DELIMITERS "/"
//#define DIRECTORY_DELIMITER '/'
//#define DIRECTORY_DELIMITER_STRING "/"


namespace scy {
namespace fs {


#ifdef WIN32
	char fs::separator = '\\';
	static const char* delimiters = "/\\";
#else
	char* fs::separator = '/';
	static const char* delimiters = "/";
#endif


std::string filename(const std::string& path)
{
	size_t dirp = path.find_last_of(fs::delimiters /*ALLOWABLE_DIRECTORY_DELIMITERS*/);
	if (dirp == std::string::npos) return path;
	return path.substr(dirp + 1);
}


std::string dirname(const std::string& path)
{
	size_t dirp = path.find_last_of(fs::delimiters /*ALLOWABLE_DIRECTORY_DELIMITERS*/);
	if (dirp == std::string::npos) return "";
	return path.substr(0, dirp);
}


std::string basename(const std::string& path)
{
	size_t dotp = path.find_last_of(".");
	if (dotp == std::string::npos) 
		return path;

	size_t dirp = path.find_last_of(fs::delimiters /*ALLOWABLE_DIRECTORY_DELIMITERS*/);
	if (dirp != std::string::npos && dotp < dirp)
		return path;

	return path.substr(0, dotp);
}


std::string extname(const std::string& path, bool includeDot)
{
	size_t dotp = path.find_last_of(".");
	if (dotp == std::string::npos) 
		return "";

	// Ensure the dot was not part of the pathname
	size_t dirp = path.find_last_of(fs::delimiters /*ALLOWABLE_DIRECTORY_DELIMITERS*/);
	if (dirp != std::string::npos && dotp < dirp)
		return "";

	return path.substr(dotp + includeDot ? 0 : 1);
}


bool exists(const std::string& path)
{
	std::ifstream ifs;
	ifs.open(path.c_str(), std::ios::in);
	bool res = ifs.is_open();
	ifs.close();
	return res;
}


bool readdir(const std::string& dir, std::vector<std::string>& res)
{	
	uv_fs_t req;
    if (uv_fs_readdir(uv_default_loop(), &req, dir.c_str(), 0, nil)) {
        char *namebuf = static_cast<char*>(req.ptr);
        int nnames = req.result;                       
        for (int i = 0; i < nnames; i++) {
            std::string name(namebuf);
            res.push_back(name);                            
#ifdef _DEBUG
            namebuf += name.length();
            assert(*namebuf == '\0');
            namebuf += 1;
#else
            namebuf += name.length() + 1;
#endif
        }
    }
    uv_fs_req_cleanup(&req);
    return !res.empty();
}


bool isdir(const std::string& path)
{
	size_t dirp = path.find_last_of(fs::delimiters);
	return (dirp != std::string::npos && dirp == path.length() - 1);
}


std::string transcode(const std::string& path)
{	
#if defined(WIN32) && defined(UNICODE)
	std::wstring_convert<std::codecvt<char16_t,char,std::mbstate_t>,char16_t> convert;
	std::u16string u16s = convert.from_bytes(path);
	std::wstring uniPath(u16s.begin(), u16s.end()); // copy data across, w_char is 16 bit on windows so this should be OK
	DWORD len = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(), static_cast<int>(uniPath.length()), NULL, 0, NULL, NULL);
	if (len > 0) {
		std::unique_ptr<char[]> buffer(new char[len]);
		DWORD rc = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(), static_cast<int>(uniPath.length()), buffer.get(), static_cast<int>(len), NULL, NULL);
		if (rc) {
			return std::string(buffer.get(), len);
		}
	}
#endif
	return path;
}


//
// FileSystem Stat Watcher (TODO)
//


/*
StatWatcher::StatWatcher() : 
	uv::Handle(new uv_fs_poll_t) 
{
	uv_fs_poll_init(uv_default_loop(), _handle);
	_handle->data = static_cast<void*>(this);
}


StatWatcher::~StatWatcher()
{
  stop();
}


void StatWatcher::Callback(uv_fs_poll_t* handle,
                           int status,
                           const uv_stat_t* prev,
                           const uv_stat_t* curr) {
  StatWatcher* wrap = static_cast<StatWatcher*>(handle->data);
  assert(wrap->watcher_ == handle);
  HandleScope scope(node_isolate);
  Local<Value> argv[3];
  argv[0] = BuildStatsObject(curr);
  argv[1] = BuildStatsObject(prev);
  argv[2] = Integer::New(status, node_isolate);
  if (onchange_sym.IsEmpty()) {
    onchange_sym = String::New("onchange");
  }
  MakeCallback(wrap->handle(node_isolate),
               onchange_sym,
               ARRAY_SIZE(argv),
               argv);
}


void StatWatcher::start(const std::string& path, uint32_t interval, bool persistent) 
{
	if (!persistent) uv_unref(reinterpret_cast<uv_handle_t*>(wrap->watcher_));
	uv_fs_poll_start(wrap->watcher_, Callback, *path, interval);
}


void StatWatcher::stop(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope(node_isolate);
  StatWatcher* wrap = ObjectWrap::Unwrap<StatWatcher>(args.This());
  if (onstop_sym.IsEmpty()) {
    onstop_sym = String::New("onstop");
  }
  MakeCallback(wrap->handle(node_isolate), onstop_sym, 0, NULL);
  wrap->stop();
}


void StatWatcher::stop() {
  if (!uv_is_active(reinterpret_cast<uv_handle_t*>(watcher_))) return;
  uv_fs_poll_stop(watcher_);
  Unref();
}


static void ReadDir(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope(node_isolate);

  if (args.Length() < 1) return TYPE_ERROR("path required");
  if (!args[0]->IsString()) return TYPE_ERROR("path must be a string");

  String::Utf8Value path(args[0]);

  if (args[1]->IsFunction()) {
    ASYNC_CALL(readdir, args[1], *path, 0 //flags/)
  } else {
    SYNC_CALL(readdir, *path, *path, 0 /flags/)

    char *namebuf = static_cast<char*>(SYNC_REQ.ptr);
    int nnames = req_wrap.req.result;
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

    args.GetReturnValue().Set(names);
  }
}
*/


} } // namespace scy::fs