# Module <!-- group --> `archo`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::archo`](#namespacescy_1_1archo)    | 
`struct `[`scy::archo::ZipFile::FileInfo`](#structscy_1_1archo_1_1ZipFile_1_1FileInfo)    | 
# namespace `scy::archo` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`scy::archo::ZipFile`](#structscy_1_1archo_1_1ZipFile)    | 
# struct `scy::archo::ZipFile` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`FileInfo`](./doc/api-archo.md#structscy_1_1archo_1_1ZipFile_1_1FileInfo)` > info` | 
`public unzFile fp` | 
`public  ZipFile()` | 
`public  ZipFile(const std::string & file)` | 
`public  ~ZipFile()` | 
`public void open(const std::string & file)` | 
`public bool opened() const` | 
`public void close()` | 
`public void extract(const std::string & path)` | Extracts the archive contents to the given directory path.
`public bool extractCurrentFile(const std::string & path,bool whiny)` | 
`public bool goToFirstFile()` | 
`public bool goToNextFile()` | 
`public void openCurrentFile()` | 
`public void closeCurrentFile()` | 
`public std::string currentFileName()` | 

## Members

#### `public std::vector< `[`FileInfo`](./doc/api-archo.md#structscy_1_1archo_1_1ZipFile_1_1FileInfo)` > info` 





#### `public unzFile fp` 





#### `public  ZipFile()` 





#### `public  ZipFile(const std::string & file)` 





#### `public  ~ZipFile()` 





#### `public void open(const std::string & file)` 





#### `public bool opened() const` 





#### `public void close()` 





#### `public void extract(const std::string & path)` 

Extracts the archive contents to the given directory path.



#### `public bool extractCurrentFile(const std::string & path,bool whiny)` 





#### `public bool goToFirstFile()` 





#### `public bool goToNextFile()` 





#### `public void openCurrentFile()` 





#### `public void closeCurrentFile()` 





#### `public std::string currentFileName()` 





# struct `scy::archo::ZipFile::FileInfo` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string path` | 
`public size_t compressedSize` | 
`public size_t uncompressedSize` | 

## Members

#### `public std::string path` 





#### `public size_t compressedSize` 





#### `public size_t uncompressedSize` 





