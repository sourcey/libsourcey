///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/filesystem.h"
#include "scy/logger.h"
#include "scy/util.h"
#include "scy/util.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#if defined(_MSC_VER) && defined(SCY_UNICODE)
#include <codecvt>
#include <locale>
#endif


namespace scy {
namespace fs {


static const char* separatorWin = "\\";
static const char* separatorUnix = "/";
#ifdef SCY_WIN
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
    if (dirp == std::string::npos)
        return path;
    return path.substr(dirp + 1);
}


std::string dirname(const std::string& path)
{
    size_t dirp = path.find_last_of(sepPattern);
    if (dirp == std::string::npos)
        return ".";
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

    return path.substr((dotp + includeDot) ? 0 : 1);
}


bool exists(const std::string& path)
{
// Normalize is needed to ensure no
// trailing slash for directories or
// stat fails to recognize validity.
// TODO: Do we need transcode here?
#ifdef SCY_WIN
    struct _stat s;
    return _stat(fs::normalize(path).c_str(), &s) != -1;
#else
    struct stat s;
    return stat(fs::normalize(path).c_str(), &s) != -1;
#endif
}


bool isdir(const std::string& path)
{
// TODO: Do we need transcode here?
#ifdef SCY_WIN
    struct _stat s;
    _stat(fs::normalize(path).c_str(), &s);
#else
    struct stat s;
    stat(fs::normalize(path).c_str(), &s);
#endif
    // S_IFDIR: directory file.
    // S_IFCHR: character-oriented device file
    // S_IFBLK: block-oriented device file
    // S_IFREG: regular file
    // S_IFLNK: symbolic link
    // S_IFSOCK: socket
    // S_IFIFO: FIFO or pipe
    return (s.st_mode & S_IFDIR) != 0;
}


std::int64_t filesize(const std::string& path)
{
#ifdef SCY_WIN
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
    FSReq(const FSReq& req) = delete;
    FSReq& operator=(const FSReq& req) = delete;
    uv_fs_t req;
};

#define FSapi(func, ...)                                                       \
    FSReq wrap;                                                                \
    int err =                                                                  \
        uv_fs_##func(uv_default_loop(), &wrap.req, __VA_ARGS__, nullptr);      \
    if (err < 0)                                                               \
        uv::throwError(std::string("Filesystem error: ") + #func +             \
                       std::string(" failed"), err);

} // namespace internal


void readdir(const std::string& path, std::vector<std::string>& res)
{
    internal::FSapi(scandir, path.c_str(), 0)

    uv_dirent_t dent;
    while (UV_EOF != uv_fs_scandir_next(&wrap.req, &dent)) {
        res.push_back(dent.name);
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

    while (std::getline(istr, level, fs::delimiter)) {
        if (level.empty())
            continue;

#ifdef SCY_WIN
        current += level;
        if (level.at(level.length() - 1) == ':') {
            current += fs::separator;
            continue; // skip drive letter
        }
#else
        if (current.empty())
            current += fs::separator;
        current += level;
#endif
        // create current level
        if (!fs::exists(current))
            fs::mkdir(current.c_str(), mode); // create or throw

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


void trimslash(std::string& path)
{
    if (path.empty()) return;
    size_t dirp = path.find_last_of(sepPattern);
    if (dirp == path.length() - 1)
        path.resize(dirp);
}


std::string normalize(const std::string& path)
{
    std::string s(util::replace(path,
#ifdef SCY_WIN
                                separatorUnix, separatorWin
#else
                                separatorWin, separatorUnix
#endif
                                ));

    // Trim the trailing slash for stat compatability
    trimslash(s);
    return s;
}


std::string transcode(const std::string& path)
{
#if defined(_MSC_VER) && defined(SCY_UNICODE)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>
        convert; // conversion between UTF-16 and UTF-8
    std::wstring uniPath = convert.from_bytes(
        path); // convert UTF-8 std::string to UTF-16 std::wstring
    DWORD len = WideCharToMultiByte(
        CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(),
        static_cast<int>(uniPath.length()), nullptr, 0, nullptr, nullptr);
    if (len > 0) {
        std::unique_ptr<char[]> buffer(new char[len]);
        DWORD rc = WideCharToMultiByte(
            CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(),
            static_cast<int>(uniPath.length()), buffer.get(),
            static_cast<int>(len), nullptr, nullptr);
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


bool savefile(const std::string& path, const char* data, size_t size,
              bool whiny)
{
    std::ofstream ofs(path, std::ios_base::binary | std::ios_base::out);
    if (ofs.is_open())
        ofs.write(data, size);
    else {
        if (whiny)
            throw std::runtime_error("Cannot save file: " + path);
        return false;
    }
    return true;
}


} // namespace fs
} // namespace scy


/// @\}
