# Module <!-- group --> `archo`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::archo`](#namespacescy_1_1archo)    | 
# namespace `scy::archo` {#namespacescy_1_1archo}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`scy::archo::ZipFile`](#structscy_1_1archo_1_1ZipFile)    | 
# struct `scy::archo::ZipFile` {#structscy_1_1archo_1_1ZipFile}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`FileInfo`](api-archo.md#structscy_1_1archo_1_1ZipFile_1_1FileInfo)` > info` | 
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

#### `public std::vector< `[`FileInfo`](api-archo.md#structscy_1_1archo_1_1ZipFile_1_1FileInfo)` > info` {#group__archo_1ga031d3193c306c10b0092e5815aead8cb}





#### `public unzFile fp` {#group__archo_1gaf810f2266baaef1a7c693a1a5df6768e}





#### `public  ZipFile()` {#group__archo_1gafbd603f7e87970b25761dde6ee43ed53}





#### `public  ZipFile(const std::string & file)` {#group__archo_1ga83472457ecd60b679786812154a48b2f}





#### `public  ~ZipFile()` {#group__archo_1ga3526297ee66287bd3392b5496e29e296}





#### `public void open(const std::string & file)` {#group__archo_1ga4505de4a4b0258c1be3640370790b3e5}





#### `public bool opened() const` {#group__archo_1ga2cb3083919bf19fdc8440681095ad52b}





#### `public void close()` {#group__archo_1gad5f16432c7097c4f0f3376ec06d9b8aa}





#### `public void extract(const std::string & path)` {#group__archo_1ga729b7bfd554bf838c855ad42b9e4039d}

Extracts the archive contents to the given directory path.



#### `public bool extractCurrentFile(const std::string & path,bool whiny)` {#group__archo_1gac5f1b12cfb628a7fba60abc3fca6e9dc}





#### `public bool goToFirstFile()` {#group__archo_1gaa70485384e6dbcc5783b3228e9b9f942}





#### `public bool goToNextFile()` {#group__archo_1gaa197ae9f0e8e670d365f0cb5e4f2cb94}





#### `public void openCurrentFile()` {#group__archo_1ga084ce7163c3a21c4bb905dab42aee7ba}





#### `public void closeCurrentFile()` {#group__archo_1gaef2d3fd19db8f1d24987e949b413b927}





#### `public std::string currentFileName()` {#group__archo_1ga4ed5731ad68cd4a8be42342963f245ac}





