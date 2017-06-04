///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup archo
/// @{


#ifndef SCY_Archo_ZipFile_H
#define SCY_Archo_ZipFile_H


#include "scy/archo/archo.h"
#include "scy/base64.h"
#include <unzip.h> // zlib
#include <vector>


namespace scy {
namespace archo {


struct Archo_API ZipFile
{
    ZipFile();
    ZipFile(const std::string& file);
    ~ZipFile();

    void open(const std::string& file);
    bool opened() const;
    void close();

    /// Extracts the archive contents to the given directory path.
    void extract(const std::string& path);
    bool extractCurrentFile(const std::string& path, bool whiny = true);

    bool goToFirstFile();
    bool goToNextFile();

    void openCurrentFile();
    void closeCurrentFile();

    std::string currentFileName();

    struct FileInfo
    {
        std::string path;
        size_t compressedSize;
        size_t uncompressedSize;
    };

    std::vector<FileInfo> info;
    unzFile fp;
};


} // namespace arc
} // namespace scy


#endif // SCY_Archo_ZipFile_H


/// @\}
