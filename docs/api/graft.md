{#graftmodule}

# graft

Native plugin ABI contract and runtime loading helpers.

### Namespaces

| Name | Description |
|------|-------------|
| [`graft`](#graft) | Shared-library plugin contracts and runtime loading helpers. |

{#graft}

# graft

Shared-library plugin contracts and runtime loading helpers.

### Classes

| Name | Description |
|------|-------------|
| [`Library`](#library) | Loads a native plugin library and resolves its typed entrypoint. |
| [`Manifest`](#manifest-3) | Metadata exported by a plugin under `icy_graft_manifest`. |

### Enumerations

| Name | Description |
|------|-------------|
| [`RuntimeKind`](#runtimekind)  | Runtime contract declared by a plugin manifest. |

---

{#runtimekind}

#### RuntimeKind

```cpp
enum RuntimeKind
```

Runtime contract declared by a plugin manifest.

| Value | Description |
|-------|-------------|
| `Unknown` | Runtime string is missing or not recognized. |
| `Native` | Plugin is loaded directly into the host process. |
| `Worker` | Plugin is intended for a worker runtime. |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `Graft_APIRuntimeKind` | [`parseRuntimeKind`](#parseruntimekind) `nodiscard` `noexcept` | Converts a manifest runtime string to a `[RuntimeKind](#runtimekind)`. |
| `Graft_API const char *` | [`runtimeKindName`](#runtimekindname) `nodiscard` `noexcept` | Returns the manifest runtime string for a `[RuntimeKind](#runtimekind)`. |
| `Graft_API void` | [`validateManifest`](#validatemanifest)  | Throws when a manifest is incompatible or missing required fields. |

---

{#parseruntimekind}

#### parseRuntimeKind

`nodiscard` `noexcept`

```cpp
[[nodiscard]] Graft_APIRuntimeKind parseRuntimeKind(std::string_view runtime) noexcept
```

Converts a manifest runtime string to a `[RuntimeKind](#runtimekind)`.

---

{#runtimekindname}

#### runtimeKindName

`nodiscard` `noexcept`

```cpp
[[nodiscard]] Graft_API const char * runtimeKindName(RuntimeKind runtime) noexcept
```

Returns the manifest runtime string for a `[RuntimeKind](#runtimekind)`.

---

{#validatemanifest}

#### validateManifest

```cpp
Graft_API void validateManifest(const Manifest & manifest, std::string_view path)
```

Throws when a manifest is incompatible or missing required fields.

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `std::uint32_t` | [`ABI_VERSION`](#abi_version) `constexpr` | Current binary manifest ABI version required by the loader. |
| `const char *` | [`MANIFEST_SYMBOL`](#manifest_symbol) `constexpr` | Exported symbol name that plugins use for their manifest. |
| `const char *` | [`RUNTIME_NATIVE`](#runtime_native) `constexpr` | Runtime string for plugins loaded directly into the current process. |
| `const char *` | [`RUNTIME_WORKER`](#runtime_worker) `constexpr` | Runtime string for plugins intended to execute in a worker runtime. |

---

{#abi_version}

#### ABI_VERSION

`constexpr`

```cpp
std::uint32_t ABI_VERSION = 1
```

Current binary manifest ABI version required by the loader.

---

{#manifest_symbol}

#### MANIFEST_SYMBOL

`constexpr`

```cpp
const char * MANIFEST_SYMBOL = "icy_graft_manifest"
```

Exported symbol name that plugins use for their manifest.

---

{#runtime_native}

#### RUNTIME_NATIVE

`constexpr`

```cpp
const char * RUNTIME_NATIVE = "native"
```

Runtime string for plugins loaded directly into the current process.

---

{#runtime_worker}

#### RUNTIME_WORKER

`constexpr`

```cpp
const char * RUNTIME_WORKER = "worker"
```

Runtime string for plugins intended to execute in a worker runtime.

{#library}

## Library

```cpp
#include <icy/graft/graft.h>
```

```cpp
class Library
```

Defined in src/graft/include/icy/graft/graft.h:90

Loads a native plugin library and resolves its typed entrypoint.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Library`](#library-1) | `function` | Declared here |
| [`Library`](#library-2) | `function` | Declared here |
| [`Library`](#library-3) | `function` | Declared here |
| [`open`](#open-6) | `function` | Declared here |
| [`close`](#close-24) | `function` | Declared here |
| [`isOpen`](#isopen) | `function` | Declared here |
| [`path`](#path-3) | `function` | Declared here |
| [`manifest`](#manifest-4) | `function` | Declared here |
| [`requireSymbolAddress`](#requiresymboladdress) | `function` | Declared here |
| [`requireSymbol`](#requiresymbol) | `function` | Declared here |
| [`entrypoint`](#entrypoint-2) | `function` | Declared here |
| [`_library`](#_library) | `variable` | Declared here |
| [`_path`](#_path-4) | `variable` | Declared here |
| [`_manifest`](#_manifest) | `variable` | Declared here |
| [`_open`](#_open) | `variable` | Declared here |
| [`loadManifest`](#loadmanifest) | `function` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Library`](#library-1)  | Defaulted constructor. |
|  | [`Library`](#library-2)  | Deleted constructor. |
|  | [`Library`](#library-3)  | Deleted constructor. |
| `void` | [`open`](#open-6)  |  |
| `void` | [`close`](#close-24) `noexcept` |  |
| `bool` | [`isOpen`](#isopen) `const` `nodiscard` `noexcept` |  |
| `const std::string &` | [`path`](#path-3) `const` `nodiscard` `noexcept` |  |
| `const Manifest &` | [`manifest`](#manifest-4) `const` `nodiscard` |  |
| `void *` | [`requireSymbolAddress`](#requiresymboladdress) `const` `nodiscard` |  |
| `T` | [`requireSymbol`](#requiresymbol) `const` `inline` `nodiscard` |  |
| `T` | [`entrypoint`](#entrypoint-2) `const` `inline` `nodiscard` |  |

---

{#library-1}

#### Library

```cpp
Library() = default
```

Defined in src/graft/include/icy/graft/graft.h:93

Defaulted constructor.

---

{#library-2}

#### Library

```cpp
Library(const Library &) = delete
```

Defined in src/graft/include/icy/graft/graft.h:96

Deleted constructor.

---

{#library-3}

#### Library

```cpp
Library(Library &&) = delete
```

Defined in src/graft/include/icy/graft/graft.h:98

Deleted constructor.

---

{#open-6}

#### open

```cpp
void open(const std::string & path)
```

Defined in src/graft/include/icy/graft/graft.h:101

---

{#close-24}

#### close

`noexcept`

```cpp
void close() noexcept
```

Defined in src/graft/include/icy/graft/graft.h:102

---

{#isopen}

#### isOpen

`const` `nodiscard` `noexcept`

```cpp
[[nodiscard]] bool isOpen() const noexcept
```

Defined in src/graft/include/icy/graft/graft.h:104

---

{#path-3}

#### path

`const` `nodiscard` `noexcept`

```cpp
[[nodiscard]] const std::string & path() const noexcept
```

Defined in src/graft/include/icy/graft/graft.h:105

---

{#manifest-4}

#### manifest

`const` `nodiscard`

```cpp
[[nodiscard]] const Manifest & manifest() const
```

Defined in src/graft/include/icy/graft/graft.h:106

---

{#requiresymboladdress}

#### requireSymbolAddress

`const` `nodiscard`

```cpp
[[nodiscard]] void * requireSymbolAddress(const char * name) const
```

Defined in src/graft/include/icy/graft/graft.h:108

---

{#requiresymbol}

#### requireSymbol

`const` `inline` `nodiscard`

```cpp
template<typename T> [[nodiscard]] inline T requireSymbol(const char * name) const
```

Defined in src/graft/include/icy/graft/graft.h:111

---

{#entrypoint-2}

#### entrypoint

`const` `inline` `nodiscard`

```cpp
template<typename T> [[nodiscard]] inline T entrypoint() const
```

Defined in src/graft/include/icy/graft/graft.h:117

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SharedLibrary` | [`_library`](#_library)  |  |
| `std::string` | [`_path`](#_path-4)  |  |
| `const Manifest *` | [`_manifest`](#_manifest)  |  |
| `bool` | [`_open`](#_open)  |  |

---

{#_library}

#### _library

```cpp
SharedLibrary _library
```

Defined in src/graft/include/icy/graft/graft.h:125

---

{#_path-4}

#### _path

```cpp
std::string _path
```

Defined in src/graft/include/icy/graft/graft.h:126

---

{#_manifest}

#### _manifest

```cpp
const Manifest * _manifest = nullptr
```

Defined in src/graft/include/icy/graft/graft.h:127

---

{#_open}

#### _open

```cpp
bool _open = false
```

Defined in src/graft/include/icy/graft/graft.h:128

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`loadManifest`](#loadmanifest)  |  |

---

{#loadmanifest}

#### loadManifest

```cpp
void loadManifest()
```

Defined in src/graft/include/icy/graft/graft.h:123

{#manifest-3}

## Manifest

```cpp
#include <icy/graft/graft.h>
```

```cpp
struct Manifest
```

Defined in src/graft/include/icy/graft/graft.h:70

Metadata exported by a plugin under `icy_graft_manifest`.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`abiVersion`](#abiversion-1) | `variable` | Declared here |
| [`fileName`](#filename-3) | `variable` | Declared here |
| [`id`](#id-4) | `variable` | Declared here |
| [`name`](#name-8) | `variable` | Declared here |
| [`version`](#version-6) | `variable` | Declared here |
| [`runtime`](#runtime-1) | `variable` | Declared here |
| [`entrypoint`](#entrypoint-1) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::uint32_t` | [`abiVersion`](#abiversion-1)  | ABI version expected to match `[ABI_VERSION](#abi_version)`. |
| `const char *` | [`fileName`](#filename-3)  | Source file that declared the manifest. |
| `const char *` | [`id`](#id-4)  | Stable plugin identifier. |
| `const char *` | [`name`](#name-8)  | Human-readable plugin name. |
| `const char *` | [`version`](#version-6)  | Plugin version string. |
| `const char *` | [`runtime`](#runtime-1)  | Runtime contract string, such as `native` or `worker`. |
| `const char *` | [`entrypoint`](#entrypoint-1)  | Exported symbol name for the plugin entrypoint. |

---

{#abiversion-1}

#### abiVersion

```cpp
std::uint32_t abiVersion
```

Defined in src/graft/include/icy/graft/graft.h:73

ABI version expected to match `[ABI_VERSION](#abi_version)`.

---

{#filename-3}

#### fileName

```cpp
const char * fileName
```

Defined in src/graft/include/icy/graft/graft.h:75

Source file that declared the manifest.

---

{#id-4}

#### id

```cpp
const char * id
```

Defined in src/graft/include/icy/graft/graft.h:77

Stable plugin identifier.

---

{#name-8}

#### name

```cpp
const char * name
```

Defined in src/graft/include/icy/graft/graft.h:79

Human-readable plugin name.

---

{#version-6}

#### version

```cpp
const char * version
```

Defined in src/graft/include/icy/graft/graft.h:81

Plugin version string.

---

{#runtime-1}

#### runtime

```cpp
const char * runtime
```

Defined in src/graft/include/icy/graft/graft.h:83

Runtime contract string, such as `native` or `worker`.

---

{#entrypoint-1}

#### entrypoint

```cpp
const char * entrypoint
```

Defined in src/graft/include/icy/graft/graft.h:85

Exported symbol name for the plugin entrypoint.

