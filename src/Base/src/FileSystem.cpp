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
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Sourcey/UV/UVPP.h"
#include <sstream>
#include <fstream>
#include <memory>
#include <algorithm> 
#if defined(WIN32) && defined(SOURCEY_UNICODE)
#include <locale>
#include <codecvt>
#endif


namespace scy {
namespace fs {

	
static const char* separatorWin = "\\";
static const char* separatorUnix = "/";
#ifdef WIN32
	const char delimiter = '\\';
	const char* separator = separatorWin;
	static const char* sepPattern = "/\\";
#else
	const char delimiter = '/';
	const char* separator = separatorUnix;
	static const char* sepPattern = "/";
#endif


std::string filename(const std::string& path)
{
	size_t dirp = path.find_last_of(fs::sepPattern);
	if (dirp == std::string::npos) return path;
	return path.substr(dirp + 1);
}


std::string dirname(const std::string& path)
{
	size_t dirp = path.find_last_of(fs::sepPattern);
	if (dirp == std::string::npos) return "";
	return path.substr(0, dirp);
}


std::string basename(const std::string& path)
{
	size_t dotp = path.find_last_of(".");
	if (dotp == std::string::npos) 
		return path;

	size_t dirp = path.find_last_of(fs::sepPattern);
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
	size_t dirp = path.find_last_of(fs::sepPattern);
	if (dirp != std::string::npos && dotp < dirp)
		return "";

	return path.substr(dotp + includeDot ? 0 : 1);
}


bool exists(const std::string& path)
{
#ifdef WIN32
	struct _stat s;
	if (_stat(path.c_str(), &s) == 0)
#else
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
#endif
		return true;
	return false;
}


bool isdir(const std::string& path)
{
#ifdef WIN32
	struct _stat s;
	_stat(path.c_str(), &s);
#else
	struct stat s;
	stat(path.c_str(), &s);
#endif
	// s.st_mode & S_IFDIR == directory
	// s.st_mode & S_IFREG == file
	return (s.st_mode & S_IFDIR) == S_IFDIR;
}


Int64 filesize(const std::string& path)
{
#ifdef WIN32
	struct _stat s;
	if (_stat(path.c_str(), &s) == 0)
#else
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
#endif
		return s.st_size;
	return -1;
}


namespace internal {
		
	struct FSReq
	{
		FSReq() {}
		~FSReq() { uv_fs_req_cleanup(&req); }
		FSReq(const FSReq& req);
		FSReq& operator=(const FSReq& req);
		uv_fs_t req;
	};

#define FSapi(func, ...)										\
	FSReq wrap;													\
	int err = uv_fs_ ## func(uv_default_loop(),					\
		&wrap.req, __VA_ARGS__, nullptr);						\
	if (err < 0) 												\
		uv::throwError(std::string("Filesystem error: ") +		\
			#func + std::string(" failed"), err);				\
	
} // namespace internal


void readdir(const std::string& path, std::vector<std::string>& res)
{	
	internal::FSapi(readdir, path.c_str(), 0)
		
    char *namebuf = static_cast<char*>(wrap.req.ptr);
    int nnames = wrap.req.result;                       
    for (int i = 0; i < nnames; i++) 
	{
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


void mkdir(const std::string& path, int mode)
{
	internal::FSapi(mkdir, path.c_str(), mode)
}


void mkdirr(const std::string& path, int mode)
{
	std::string current;
	std::string level;
	std::istringstream istr(fs::normalize(path));

	while (std::getline(istr, level, fs::delimiter))
	{
		if (level.empty()) continue;
		current += level;
		
#ifdef WIN32		
		if (level.at(level.length() - 1) == ':') {
			current += fs::separator;
			continue; // skip drive letter
		}
#endif
		// create current level
		if (!fs::exists(current))
			mkdir(current.c_str(), mode); // create or throw
				
		current += fs::separator;
	}
}


void rmdir(const std::string& path)
{
	internal::FSapi(rmdir, path.c_str())
}


void unlink(const std::string& path)
{
	internal::FSapi(unlink, path.c_str())
}


void rename(const std::string& path, const std::string& target)
{
	internal::FSapi(rename, path.c_str(), target.c_str())
}


std::string normalize(const std::string& path)
{	
#ifdef WIN32	
	return util::replace(path, separatorUnix, separatorWin);
#else
	return util::replace(path, separatorWin, separatorUnix);  // probably redundant...
#endif
}


std::string transcode(const std::string& path)
{	
#if defined(WIN32) && defined(SOURCEY_UNICODE)
	std::wstring_convert<std::codecvt<char16_t,char,std::mbstate_t>,char16_t> convert;
	std::u16string u16s = convert.from_bytes(path);
	std::wstring uniPath(u16s.begin(), u16s.end()); // copy data across, w_char is 16 bit on windows so this should be OK
	DWORD len = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(), static_cast<int>(uniPath.length()), nullptr, 0, nullptr, nullptr);
	if (len > 0) {
		std::unique_ptr<char[]> buffer(new char[len]);
		DWORD rc = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(), static_cast<int>(uniPath.length()), buffer.get(), static_cast<int>(len), nullptr, nullptr);
		if (rc) {
			return std::string(buffer.get(), len);
		}
	}
#endif
	return path;
}


void addsep(std::string& path)
{
	if (!path.empty() && path.at(path.length() - 1) != fs::separator[0])
		path.append(fs::separator, 1);
}


void addnode(std::string& path, const std::string& node)
{
	fs::addsep(path);
	path += node;
}


} } // namespace scy::fs


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
  MakeCallback(wrap->handle(node_isolate), onstop_sym, 0, nullptr);
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
    AFSapi(readdir, args[1], *path, 0 //flags/)
  } else {
    FSapi(readdir, *path, *path, 0 /flags/)

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


	//int err = uv_fs_readdir(uv_default_loop(), &req, path.c_str(), 0, nullptr);
    //if (err == 0) uv::throwError(err, "Cannot create directory: " + path);
    //uv_fs_req_cleanup(&req);

    //}
    //uv_fs_req_cleanup(&req);
    //return !res.empty();

	//uv_fs_t req;
	//int err = uv_fs_mkdir(uv_default_loop(), &req, path.c_str(), mode, nullptr);
    //uv_fs_req_cleanup(&req);
    //if (err != 0) uv::throwError(err, "Cannot create directory: " + path);

	//uv_fs_t req;
	//int err = uv_fs_rmdir(uv_default_loop(), &req, path.c_str(), nullptr);
   // uv_fs_req_cleanup(&req);
    //if (err != 0) uv::throwError(err, "Cannot remove directory: " + path);
	

		/*
		if (s.st_mode & S_IFDIR)
		{
			//it's a directory
		}
		else if(s.st_mode & S_IFREG)
		{
			//it's a file
		}
		else
		{
			//something else
		}
		*/
	/*
	std::ifstream ifs;
	ifs.open(path.c_str(), std::ios::in);
	bool res = ifs.is_open();
	ifs.close();
	return res;
	*/