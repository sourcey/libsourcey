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


#include "scy/archo/zip.h"
#include "scy/filesystem.h"

/*
#include "scy/exception.h"
#include "scy/base64.h"
#include "scy/hex.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <assert.h>
*/


using std::endl; 


namespace scy {
namespace arc {
	

namespace internal {

	std::string errmsg(int code)
	{
		switch(code) {
			case UNZ_END_OF_LIST_OF_FILE:		return "end of file list reached";
			case UNZ_EOF:						return "end of file reached";
			case UNZ_PARAMERROR:				return "invalid parameter given";
			case UNZ_BADZIPFILE:				return "bad zip file";
			case UNZ_INTERNALERROR:				return "internal error";
			case UNZ_CRCERROR:					return "crc error, file is corrupt";
			case UNZ_ERRNO:						return strerror(code);
			default:							return "unknown error";
		}
	}

	void throwError(const std::string& error, int code = 0)
	{
		std::string msg;
		if (!error.empty()) {
			msg += error;
		}
		if (code) {
			msg += ": ";
			msg += errmsg(code);
		}
		throw std::runtime_error(msg);
	}
		
	void api(const char* what, int ret)
	{
		if (ret != UNZ_OK)
			throwError(what, ret);
	}

} // namespace internal


ZipFile::ZipFile() : 
	fp(nullptr) 
{
}


ZipFile::ZipFile(const std::string& file) : fp(nullptr)
{
	this->open(file);
}


ZipFile::~ZipFile()
{
	this->close();
}


bool ZipFile::opened() const
{
	return this->fp != nullptr;
}


void ZipFile::open(const std::string& file) 
{
	this->close();
	this->fp = unzOpen(fs::transcode(file).c_str());
	if (this->fp == nullptr) 
		internal::throwError("Cannot open archive file: " + file);
	
	for (int ret = unzGoToFirstFile(this->fp); ret == UNZ_OK; ret = unzGoToNextFile(this->fp)) {
		unz_file_info fileInfo;
		char fileName[1024];
		internal::api("unzGetCurrentFileInfo", unzGetCurrentFileInfo(this->fp, &fileInfo, fileName, 1024, nullptr, 0, nullptr, 0));

		FileInfo info;
		info.path             = fileName;
		info.compressedSize   = static_cast< size_t >(fileInfo.uncompressed_size);
		info.uncompressedSize = static_cast< size_t >(fileInfo.compressed_size);
		this->info.push_back(info);
	}

	unzGoToFirstFile(this->fp); // rewind
}


void ZipFile::close() 
{
	if (this->opened()) {
		internal::api("unzClose", unzClose(this->fp));
		this->fp = nullptr;
	}
}


void ZipFile::extract(const std::string& path)
{	
	traceL("ZipFile") << "Extracting zip to: " << path << endl;

	if (!opened())
		throw std::runtime_error("The archive must be opened for extraction.");

	if (!goToFirstFile())
		throw std::runtime_error("Cannot read the source archive.");

	while (goToNextFile()) {
		extractCurrentFile(path, true);
	}
}


bool ZipFile::extractCurrentFile(const std::string& path, bool whiny)
{	
	int ret;
	unz_file_info info;
	char fname[1024];

	try {
		internal::api("unzGetCurrentFileInfo", unzGetCurrentFileInfo(this->fp, &info, fname, 1024, nullptr, 0, nullptr, 0));
		
		std::string outPath(path);
		fs::addnode(outPath, fname);

		traceL("ZipFile") << "Extracting zip file: " << outPath << endl;

		// Create directory
#if !WIN32
		const int FILE_ATTRIBUTE_DIRECTORY = 0x10;
#endif
		if (info.external_fa & FILE_ATTRIBUTE_DIRECTORY || 
			fname[strlen(fname) - 1] == fs::delimiter) {
			traceL("ZipFile") << "Create directory: " << outPath << endl;
			fs::mkdirr(outPath);
		}

		// Create file
		else {			
			openCurrentFile();

			traceL("ZipFile") << "Extracting file: " << outPath << endl;
			std::ofstream ofs(outPath, std::ios::binary | std::ios::out);
			if (!ofs.is_open())
				throw std::runtime_error("Cannot open zip output file: " + outPath);
			
			char buffer[16384];
			while ((ret = unzReadCurrentFile(this->fp, buffer, 16384)) > 0)
				ofs.write(buffer, ret);
			
			ofs.close();
			
			internal::api("unzReadCurrentFile", ret); // throw file read errors
			closeCurrentFile();
		}
	} 
	catch (std::exception& exc) {
		errorL("ZipFile") << "Cannot unzip file: " << exc.what() << endl;
		if (whiny)
			throw exc;
		return false;
	}
	return true;
}


bool ZipFile::goToFirstFile()
{
	return unzGoToFirstFile(this->fp) == UNZ_OK;
}


bool ZipFile::goToNextFile()
{
	return unzGoToNextFile(this->fp) == UNZ_OK;
}


void ZipFile::openCurrentFile()
{	
	internal::api("unzOpenCurrentFile", unzOpenCurrentFile(this->fp));
}


void ZipFile::closeCurrentFile()
{
	internal::api("unzCloseCurrentFile", unzOpenCurrentFile(this->fp));
}


std::string ZipFile::currentFileName()
{
	char buf[1024];
	internal::api("unzGetCurrentFileInfo", 
		unzGetCurrentFileInfo(this->fp, nullptr, buf, 
			sizeof(buf), nullptr, 0, nullptr, 0));
	return buf;
}


} } // namespace scy::arc