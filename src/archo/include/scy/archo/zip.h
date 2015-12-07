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


#ifndef SCY_Archo_Zip_H
#define SCY_Archo_Zip_H


#include "scy/base64.h"
#include <vector>
#include <unzip.h> // zlib


namespace scy {
namespace arc {

    
struct ZipFile 
{
    ZipFile();
    ZipFile(const std::string& file);
    ~ZipFile();

    void open(const std::string& file);
    bool opened() const;
    void close();
    
    void extract(const std::string& path);
        // Extracts the archive contents to the given directory path.
    
    bool extractCurrentFile(const std::string& path, bool whiny = true);

    bool goToFirstFile();
    bool goToNextFile();

    void openCurrentFile();
    void closeCurrentFile();

    std::string currentFileName();

    struct FileInfo
    {
        std::string path;
        size_t      compressedSize;
        size_t      uncompressedSize;
    };


    std::vector<FileInfo> info;
    unzFile fp;
};


#if 0
class zip_error : public std::runtime_error 
{
public:
    zip_error(const std::string& func) : runtime_error(func) {}
    zip_error(const std::string& func, int msg) : runtime_error(func + ": " + errmsg(msg)) {}
    zip_error(const std::string& func, const std::string& msg) : runtime_error(func + ": " + msg) {}

    static std::string errmsg(int e);
};
#endif
    

} } // namespace scy::arc


#endif // SCY_Archo_Zip_H



    

    //typedef std::vector<FileInfo> FileInfoList;
    //std::vector< ci::fs::path > getFiles();
    //std::pair<boost::shared_array<char>,size_t> ReadCurrentFile();
    //private: