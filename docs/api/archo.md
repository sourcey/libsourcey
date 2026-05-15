{#archivemodule}

# archo

Archive and ZIP file handling.

### Namespaces

| Name | Description |
|------|-------------|
| [`archo`](#archo) | ZIP archive handling utilities and related helpers. |

### Macros

| Name | Description |
|------|-------------|
| [`Archo_API`](#archo_api)  | Shared library exports. |

---

{#archo_api}

#### Archo_API

```cpp
Archo_API()
```

Shared library exports.

{#archo}

# archo

ZIP archive handling utilities and related helpers.

### Classes

| Name | Description |
|------|-------------|
| [`ZipFile`](#zipfile) | ZIP archive reader and writer. |

{#zipfile}

## ZipFile

```cpp
#include <icy/archo/zipfile.h>
```

```cpp
struct ZipFile
```

Defined in src/archo/include/icy/archo/zipfile.h:35

ZIP archive reader and writer.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`info`](#info) | `variable` | Declared here |
| [`fp`](#fp) | `variable` | Declared here |
| [`ZipFile`](#zipfile-1) | `function` | Declared here |
| [`ZipFile`](#zipfile-2) | `function` | Declared here |
| [`~ZipFile`](#zipfile-3) | `function` | Declared here |
| [`ZipFile`](#zipfile-4) | `function` | Declared here |
| [`ZipFile`](#zipfile-5) | `function` | Declared here |
| [`open`](#open-5) | `function` | Declared here |
| [`opened`](#opened) | `function` | Declared here |
| [`close`](#close-23) | `function` | Declared here |
| [`extract`](#extract) | `function` | Declared here |
| [`extractCurrentFile`](#extractcurrentfile) | `function` | Declared here |
| [`goToFirstFile`](#gotofirstfile) | `function` | Declared here |
| [`goToNextFile`](#gotonextfile) | `function` | Declared here |
| [`openCurrentFile`](#opencurrentfile) | `function` | Declared here |
| [`closeCurrentFile`](#closecurrentfile) | `function` | Declared here |
| [`currentFileName`](#currentfilename) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::vector< FileInfo >` | [`info`](#info)  |  |
| `unzFile` | [`fp`](#fp)  |  |

---

{#info}

#### info

```cpp
std::vector< FileInfo > info
```

Defined in src/archo/include/icy/archo/zipfile.h:104

---

{#fp}

#### fp

```cpp
unzFile fp
```

Defined in src/archo/include/icy/archo/zipfile.h:105

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ZipFile`](#zipfile-1)  | Constructs an unopened [ZipFile](#zipfile). Call [open()](#open-5) before use. |
|  | [`ZipFile`](#zipfile-2) `explicit` | Constructs a [ZipFile](#zipfile) and immediately opens the archive at `file`. |
|  | [`~ZipFile`](#zipfile-3)  | Closes the archive if still open. |
|  | [`ZipFile`](#zipfile-4)  | Deleted constructor. |
|  | [`ZipFile`](#zipfile-5)  | Deleted constructor. |
| `void` | [`open`](#open-5)  | Opens the archive at `file`, closing any previously opened archive. Populates the `[info](#info)` vector with metadata for every entry. |
| `bool` | [`opened`](#opened) `const` `nodiscard` | Returns true if the archive is currently open. |
| `void` | [`close`](#close-23)  | Closes the archive and releases the underlying file handle. |
| `void` | [`extract`](#extract)  | Extracts the archive contents to the given directory path. |
| `bool` | [`extractCurrentFile`](#extractcurrentfile) `nodiscard` | Extracts the current file entry to `path`. Validates each entry against path-traversal attacks before writing. |
| `bool` | [`goToFirstFile`](#gotofirstfile) `nodiscard` | Moves the internal cursor to the first file entry in the archive. |
| `bool` | [`goToNextFile`](#gotonextfile) `nodiscard` | Advances the internal cursor to the next file entry. |
| `void` | [`openCurrentFile`](#opencurrentfile)  | Opens the current file entry for reading. |
| `void` | [`closeCurrentFile`](#closecurrentfile)  | Closes the current file entry. |
| `std::string` | [`currentFileName`](#currentfilename) `nodiscard` | Returns the name (relative path) of the current file entry. |

---

{#zipfile-1}

#### ZipFile

```cpp
ZipFile()
```

Defined in src/archo/include/icy/archo/zipfile.h:38

Constructs an unopened [ZipFile](#zipfile). Call [open()](#open-5) before use.

---

{#zipfile-2}

#### ZipFile

`explicit`

```cpp
explicit ZipFile(const std::filesystem::path & file)
```

Defined in src/archo/include/icy/archo/zipfile.h:42

Constructs a [ZipFile](#zipfile) and immediately opens the archive at `file`. 
#### Parameters
* `file` Path to the ZIP archive to open.

---

{#zipfile-3}

#### ~ZipFile

```cpp
~ZipFile()
```

Defined in src/archo/include/icy/archo/zipfile.h:45

Closes the archive if still open.

---

{#zipfile-4}

#### ZipFile

```cpp
ZipFile(const ZipFile &) = delete
```

Defined in src/archo/include/icy/archo/zipfile.h:47

Deleted constructor.

---

{#zipfile-5}

#### ZipFile

```cpp
ZipFile(ZipFile &&) = delete
```

Defined in src/archo/include/icy/archo/zipfile.h:49

Deleted constructor.

---

{#open-5}

#### open

```cpp
void open(const std::filesystem::path & file)
```

Defined in src/archo/include/icy/archo/zipfile.h:56

Opens the archive at `file`, closing any previously opened archive. Populates the `[info](#info)` vector with metadata for every entry. 
#### Parameters
* `file` Path to the ZIP archive. 

#### Exceptions
* `std::runtime_error` if the file cannot be opened.

---

{#opened}

#### opened

`const` `nodiscard`

```cpp
[[nodiscard]] bool opened() const
```

Defined in src/archo/include/icy/archo/zipfile.h:59

Returns true if the archive is currently open.

---

{#close-23}

#### close

```cpp
void close()
```

Defined in src/archo/include/icy/archo/zipfile.h:62

Closes the archive and releases the underlying file handle.

---

{#extract}

#### extract

```cpp
void extract(const std::filesystem::path & path)
```

Defined in src/archo/include/icy/archo/zipfile.h:67

Extracts the archive contents to the given directory path. 
#### Parameters
* `path` Destination directory; created automatically if necessary. 

#### Exceptions
* `std::runtime_error` if the archive is not open or a read error occurs.

---

{#extractcurrentfile}

#### extractCurrentFile

`nodiscard`

```cpp
[[nodiscard]] bool extractCurrentFile(const std::filesystem::path & path, bool whiny = true)
```

Defined in src/archo/include/icy/archo/zipfile.h:74

Extracts the current file entry to `path`. Validates each entry against path-traversal attacks before writing. 
#### Parameters
* `path` Destination base directory. 

* `whiny` If true, re-throws on error; otherwise returns false. 

#### Returns
true on success, false if `whiny` is false and an error occurred.

---

{#gotofirstfile}

#### goToFirstFile

`nodiscard`

```cpp
[[nodiscard]] bool goToFirstFile()
```

Defined in src/archo/include/icy/archo/zipfile.h:78

Moves the internal cursor to the first file entry in the archive. 
#### Returns
true on success, false if the archive is empty or an error occurred.

---

{#gotonextfile}

#### goToNextFile

`nodiscard`

```cpp
[[nodiscard]] bool goToNextFile()
```

Defined in src/archo/include/icy/archo/zipfile.h:82

Advances the internal cursor to the next file entry. 
#### Returns
true if another entry exists, false at end-of-list.

---

{#opencurrentfile}

#### openCurrentFile

```cpp
void openCurrentFile()
```

Defined in src/archo/include/icy/archo/zipfile.h:86

Opens the current file entry for reading. 
#### Exceptions
* `std::runtime_error` on failure.

---

{#closecurrentfile}

#### closeCurrentFile

```cpp
void closeCurrentFile()
```

Defined in src/archo/include/icy/archo/zipfile.h:90

Closes the current file entry. 
#### Exceptions
* `std::runtime_error` on failure.

---

{#currentfilename}

#### currentFileName

`nodiscard`

```cpp
[[nodiscard]] std::string currentFileName()
```

Defined in src/archo/include/icy/archo/zipfile.h:94

Returns the name (relative path) of the current file entry. 
#### Returns
Entry name as reported by the ZIP directory.

{#fileinfo}

## FileInfo

```cpp
#include <icy/archo/zipfile.h>
```

```cpp
struct FileInfo
```

Defined in src/archo/include/icy/archo/zipfile.h:97

Metadata for a file entry within a ZIP archive.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`path`](#path-2) | `variable` | Declared here |
| [`compressedSize`](#compressedsize) | `variable` | Declared here |
| [`uncompressedSize`](#uncompressedsize) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`path`](#path-2)  | Relative path of the entry within the archive. |
| `size_t` | [`compressedSize`](#compressedsize)  | Compressed size in bytes. |
| `size_t` | [`uncompressedSize`](#uncompressedsize)  | Uncompressed size in bytes. |

---

{#path-2}

#### path

```cpp
std::string path
```

Defined in src/archo/include/icy/archo/zipfile.h:99

Relative path of the entry within the archive.

---

{#compressedsize}

#### compressedSize

```cpp
size_t compressedSize
```

Defined in src/archo/include/icy/archo/zipfile.h:100

Compressed size in bytes.

---

{#uncompressedsize}

#### uncompressedSize

```cpp
size_t uncompressedSize
```

Defined in src/archo/include/icy/archo/zipfile.h:101

Uncompressed size in bytes.

