{#jsonmodule}

# json

JSON serialization using nlohmann/json.

### Namespaces

| Name | Description |
|------|-------------|
| [`json`](#json) | JSON value aliases, serialization helpers, and file utilities built on nlohmann/json. |

{#json}

# json

JSON value aliases, serialization helpers, and file utilities built on nlohmann/json.

### Classes

| Name | Description |
|------|-------------|
| [`Configuration`](#configuration-1) | JSON configuration file |
| [`ISerializable`](#iserializable) | Abstract interface for JSON-serializable objects. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `nlohmann::json` | [`Value`](#value)  | Primary JSON value type used throughout the library. |

---

{#value}

#### Value

```cpp
using Value = nlohmann::json
```

Primary JSON value type used throughout the library.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`serialize`](#serialize) `inline` | Serializes `pObj` to a pretty-printed JSON string. |
| `bool` | [`deserialize`](#deserialize) `inline` | Deserializes `pObj` from a JSON string. |
| `void` | [`loadFile`](#loadfile) `inline` | Load a JSON file into a value. Throws on missing file or parse error. |
| `void` | [`saveFile`](#savefile-1) `inline` | Save a JSON value to a file. Throws on write error. |
| `void` | [`assertMember`](#assertmember) `inline` | Assert that a required member exists. Throws if missing. |
| `void` | [`countNestedKeys`](#countnestedkeys) `inline` | Count how many nested objects contain the given key. |
| `bool` | [`hasNestedKey`](#hasnestedkey) `inline` | Return true if any nested object contains the given key. |
| `bool` | [`findNestedObjectWithProperty`](#findnestedobjectwithproperty) `inline` | Find a nested object whose property matches the given key/value. |

---

{#serialize}

#### serialize

`inline`

```cpp
inline bool serialize(ISerializable * pObj, std::string & output)
```

Serializes `pObj` to a pretty-printed JSON string. 
#### Parameters
* `pObj` Object to serialize; must not be null. 

* `output` Receives the 4-space indented JSON string. 

#### Returns
true on success, false if `pObj` is null.

---

{#deserialize}

#### deserialize

`inline`

```cpp
inline bool deserialize(ISerializable * pObj, std::string & input)
```

Deserializes `pObj` from a JSON string. 
#### Parameters
* `pObj` Object to populate; must not be null. 

* `input` JSON string to parse. 

#### Returns
true on success, false if `pObj` is null or parsing fails.

---

{#loadfile}

#### loadFile

`inline`

```cpp
inline void loadFile(const std::string & path, json::Value & root)
```

Load a JSON file into a value. Throws on missing file or parse error.

---

{#savefile-1}

#### saveFile

`inline`

```cpp
inline void saveFile(const std::string & path, const json::Value & root, int indent = 4)
```

Save a JSON value to a file. Throws on write error.

---

{#assertmember}

#### assertMember

`inline`

```cpp
inline void assertMember(const json::Value & root, const std::string & name)
```

Assert that a required member exists. Throws if missing.

---

{#countnestedkeys}

#### countNestedKeys

`inline`

```cpp
inline void countNestedKeys(const json::Value & root, const std::string & key, int & count)
```

Count how many nested objects contain the given key.

---

{#hasnestedkey}

#### hasNestedKey

`inline`

```cpp
inline bool hasNestedKey(const json::Value & root, const std::string & key)
```

Return true if any nested object contains the given key.

---

{#findnestedobjectwithproperty}

#### findNestedObjectWithProperty

`inline`

```cpp
inline bool findNestedObjectWithProperty(json::Value & root, json::Value *& result, std::string_view key, std::string_view value, bool partial = true, int index = 0)
```

Find a nested object whose property matches the given key/value.

Key or value may be empty for wildcard matching. If partial is true, substring matches are accepted for string values. The index parameter selects the Nth match (0 = first).

Returns true if found, with result pointing to the matching object.

{#configuration-1}

## Configuration

```cpp
#include <icy/json/configuration.h>
```

```cpp
class Configuration
```

Defined in src/json/include/icy/json/configuration.h:28

> **Inherits:** [`Configuration`](base.md#configuration)

JSON configuration file

See base [Configuration](#configuration-1) for all accessors

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`root`](#root) | `variable` | Declared here |
| [`Configuration`](#configuration-2) | `function` | Declared here |
| [`~Configuration`](#configuration-3) | `function` | Declared here |
| [`load`](#load) | `function` | Declared here |
| [`load`](#load-1) | `function` | Declared here |
| [`save`](#save) | `function` | Declared here |
| [`remove`](#remove) | `function` | Declared here |
| [`removeAll`](#removeall) | `function` | Declared here |
| [`replace`](#replace-2) | `function` | Declared here |
| [`keys`](#keys) | `function` | Declared here |
| [`print`](#print-8) | `function` | Declared here |
| [`path`](#path-1) | `function` | Declared here |
| [`loaded`](#loaded) | `function` | Declared here |
| [`_loaded`](#_loaded) | `variable` | Declared here |
| [`_path`](#_path-3) | `variable` | Declared here |
| [`_mutex`](#_mutex-9) | `variable` | Declared here |
| [`getRaw`](#getraw) | `function` | Declared here |
| [`setRaw`](#setraw) | `function` | Declared here |
| [`PropertyChanged`](base.md#classicy_1_1Configuration_1a1a5aeb22b43da7e846041f07f2955e4c) | `variable` | Inherited from [`Configuration`](base.md#configuration) |
| [`_mutex`](base.md#classicy_1_1Configuration_1a7e91976a39cb67eecf32a6758b957921) | `variable` | Inherited from [`Configuration`](base.md#configuration) |
| [`Configuration`](base.md#classicy_1_1Configuration_1a75c92fc758b3cc1d4baa823963064675) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`~Configuration`](base.md#classicy_1_1Configuration_1a548f226233f711a8a5f16a00c5810835) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`exists`](base.md#classicy_1_1Configuration_1a7eaffe5a3b1b79237b6c6d9179b55437) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getString`](base.md#classicy_1_1Configuration_1ae6ab05b68fdca0ad845059060443bd8c) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getString`](base.md#classicy_1_1Configuration_1a77d3e6a3ef35c3042f3c677f5b9ed787) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getRawString`](base.md#classicy_1_1Configuration_1a24af13805d96c22c53f8e73be207cdee) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getRawString`](base.md#classicy_1_1Configuration_1aab24d10ef25aba0ad368ff1c00c7fce9) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getInt`](base.md#classicy_1_1Configuration_1a2e300793742ae29a47472bab453964c6) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getInt`](base.md#classicy_1_1Configuration_1a9eab78dfe24588237f69190fb70cbbe7) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getLargeInt`](base.md#classicy_1_1Configuration_1a0f7ac22a9d553a6281cba59aa034ceea) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getLargeInt`](base.md#classicy_1_1Configuration_1ab75b1cb29cc2ec51d542359b7a56db55) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getDouble`](base.md#classicy_1_1Configuration_1aefe855a3d5cb2375724752eb11cadd4e) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getDouble`](base.md#classicy_1_1Configuration_1a1b93522d09372c0b21341b7accaa6ceb) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getBool`](base.md#classicy_1_1Configuration_1a3be8357df952d372df53d11280d3a467) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getBool`](base.md#classicy_1_1Configuration_1aa27cff84e4da853bd8e9bc5192851078) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`setString`](base.md#classicy_1_1Configuration_1a7c4010bb5d93dca257aae621b9ec18d8) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`setInt`](base.md#classicy_1_1Configuration_1a692e21f77b8113343e5ea19e155284d9) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`setLargeInt`](base.md#classicy_1_1Configuration_1a9cdfe32156b29e1c87d1dec8059729cc) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`setDouble`](base.md#classicy_1_1Configuration_1a1d590599f581d7eb843bf6d32f40b71b) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`setBool`](base.md#classicy_1_1Configuration_1aa8107039d5ce981fecc2c77277de05f4) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`getRaw`](base.md#classicy_1_1Configuration_1a93755fec3296753ec88e909095026046) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`setRaw`](base.md#classicy_1_1Configuration_1afe19c7376a327bb2cc117f7a1b1a3b40) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`Configuration`](base.md#classicy_1_1Configuration_1a611845aa71dc48450656598d6526d4b7) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`operator=`](base.md#classicy_1_1Configuration_1a8a46d2b2acce5739e4fae6a8b4a960eb) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`Configuration`](base.md#classicy_1_1Configuration_1a73cc87c056e9fdad43603715fa387210) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`operator=`](base.md#classicy_1_1Configuration_1a87a14bac02f0f3198f89fced9bc083af) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`parseInt`](base.md#classicy_1_1Configuration_1a5cc03196e6e67f4423ae1a136de80266) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`parseLargeInt`](base.md#classicy_1_1Configuration_1acdca7992c57f3c927849388b562ecac8) | `function` | Inherited from [`Configuration`](base.md#configuration) |
| [`parseBool`](base.md#classicy_1_1Configuration_1a9195d710a7b0d1f72b764fa3fa1381f6) | `function` | Inherited from [`Configuration`](base.md#configuration) |

### Inherited from [`Configuration`](base.md#configuration)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`PropertyChanged`](base.md#classicy_1_1Configuration_1a1a5aeb22b43da7e846041f07f2955e4c)  | The Key and Value of the changed configuration property. |
| `variable` | [`_mutex`](base.md#classicy_1_1Configuration_1a7e91976a39cb67eecf32a6758b957921)  |  |
| `function` | [`Configuration`](base.md#classicy_1_1Configuration_1a75c92fc758b3cc1d4baa823963064675)  | Creates the [Configuration](base.md#configuration). |
| `function` | [`~Configuration`](base.md#classicy_1_1Configuration_1a548f226233f711a8a5f16a00c5810835) `virtual` | Destroys the [Configuration](base.md#configuration). |
| `function` | [`exists`](base.md#classicy_1_1Configuration_1a7eaffe5a3b1b79237b6c6d9179b55437) `const` | Returns true if the property with the given key exists. |
| `function` | [`getString`](base.md#classicy_1_1Configuration_1ae6ab05b68fdca0ad845059060443bd8c) `const` | Returns the string value of the property with the given name. Throws a NotFoundException if the key does not exist. |
| `function` | [`getString`](base.md#classicy_1_1Configuration_1a77d3e6a3ef35c3042f3c677f5b9ed787) `const` | If a property with the given key exists, returns the property's string value, otherwise returns the given default value. |
| `function` | [`getRawString`](base.md#classicy_1_1Configuration_1a24af13805d96c22c53f8e73be207cdee) `const` | Returns the raw string value of the property with the given name. Throws a NotFoundException if the key does not exist. References to other properties are not expanded. |
| `function` | [`getRawString`](base.md#classicy_1_1Configuration_1aab24d10ef25aba0ad368ff1c00c7fce9) `const` | If a property with the given key exists, returns the property's raw string value, otherwise returns the given default value. References to other properties are not expanded. |
| `function` | [`getInt`](base.md#classicy_1_1Configuration_1a2e300793742ae29a47472bab453964c6) `const` | Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `function` | [`getInt`](base.md#classicy_1_1Configuration_1a9eab78dfe24588237f69190fb70cbbe7) `const` | If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `function` | [`getLargeInt`](base.md#classicy_1_1Configuration_1a0f7ac22a9d553a6281cba59aa034ceea) `const` | Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `function` | [`getLargeInt`](base.md#classicy_1_1Configuration_1ab75b1cb29cc2ec51d542359b7a56db55) `const` | If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `function` | [`getDouble`](base.md#classicy_1_1Configuration_1aefe855a3d5cb2375724752eb11cadd4e) `const` | Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double. |
| `function` | [`getDouble`](base.md#classicy_1_1Configuration_1a1b93522d09372c0b21341b7accaa6ceb) `const` | If a property with the given key exists, returns the property's double value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an double. |
| `function` | [`getBool`](base.md#classicy_1_1Configuration_1a3be8357df952d372df53d11280d3a467) `const` | Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double. |
| `function` | [`getBool`](base.md#classicy_1_1Configuration_1aa27cff84e4da853bd8e9bc5192851078) `const` | If a property with the given key exists, returns the property's bool value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to a boolean. The following string values can be converted into a boolean: |
| `function` | [`setString`](base.md#classicy_1_1Configuration_1a7c4010bb5d93dca257aae621b9ec18d8)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `function` | [`setInt`](base.md#classicy_1_1Configuration_1a692e21f77b8113343e5ea19e155284d9)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `function` | [`setLargeInt`](base.md#classicy_1_1Configuration_1a9cdfe32156b29e1c87d1dec8059729cc)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `function` | [`setDouble`](base.md#classicy_1_1Configuration_1a1d590599f581d7eb843bf6d32f40b71b)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `function` | [`setBool`](base.md#classicy_1_1Configuration_1aa8107039d5ce981fecc2c77277de05f4)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `function` | [`getRaw`](base.md#classicy_1_1Configuration_1a93755fec3296753ec88e909095026046) `virtual` `const` | If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false. |
| `function` | [`setRaw`](base.md#classicy_1_1Configuration_1afe19c7376a327bb2cc117f7a1b1a3b40) `virtual` | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `function` | [`Configuration`](base.md#classicy_1_1Configuration_1a611845aa71dc48450656598d6526d4b7)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1Configuration_1a8a46d2b2acce5739e4fae6a8b4a960eb)  | Deleted assignment operator. |
| `function` | [`Configuration`](base.md#classicy_1_1Configuration_1a73cc87c056e9fdad43603715fa387210)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1Configuration_1a87a14bac02f0f3198f89fced9bc083af)  | Deleted assignment operator. |
| `function` | [`parseInt`](base.md#classicy_1_1Configuration_1a5cc03196e6e67f4423ae1a136de80266) `static` | Parses a decimal or `0x`-prefixed hexadecimal string to int. |
| `function` | [`parseLargeInt`](base.md#classicy_1_1Configuration_1acdca7992c57f3c927849388b562ecac8) `static` | Parses a decimal or `0x`-prefixed hexadecimal string to int64. |
| `function` | [`parseBool`](base.md#classicy_1_1Configuration_1a9195d710a7b0d1f72b764fa3fa1381f6) `static` | Parses a boolean string (true/false/yes/no/on/off/0/1, case-insensitive). |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `json::Value` | [`root`](#root)  |  |

---

{#root}

#### root

```cpp
json::Value root
```

Defined in src/json/include/icy/json/configuration.h:80

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Configuration`](#configuration-2)  |  |
|  | [`~Configuration`](#configuration-3) `virtual` | Destroys the [Configuration](#configuration-1). |
| `void` | [`load`](#load) `virtual` | Sets the file path and loads the configuration. |
| `void` | [`load`](#load-1) `virtual` | Reloads the configuration from the previously set path. Silently ignores parse errors (e.g. empty file). |
| `void` | [`save`](#save) `virtual` | Writes the current JSON root to the file at the stored path. |
| `bool` | [`remove`](#remove) `virtual` | Removes the top-level key `key` from the JSON root. |
| `void` | [`removeAll`](#removeall) `virtual` | Removes all top-level keys whose names contain `baseKey` as a substring. |
| `void` | [`replace`](#replace-2) `virtual` | Performs a global string substitution on the serialized JSON, replacing all occurrences of `from` with `to`, then re-parses. |
| `void` | [`keys`](#keys) `virtual` | Populates `keys` with all top-level key names containing `baseKey` as a substring. |
| `void` | [`print`](#print-8) `virtual` | Writes the pretty-printed JSON to `ost` with 4-space indentation. |
| `std::string` | [`path`](#path-1) `virtual` | Returns the file path that was passed to [load()](#load). |
| `bool` | [`loaded`](#loaded) `virtual` | Returns true if [load()](#load) has been called at least once. |

---

{#configuration-2}

#### Configuration

```cpp
Configuration()
```

Defined in src/json/include/icy/json/configuration.h:31

---

{#configuration-3}

#### ~Configuration

`virtual`

```cpp
virtual ~Configuration()
```

Defined in src/json/include/icy/json/configuration.h:32

Destroys the [Configuration](#configuration-1).

##### Reimplements

- [`~Configuration`](base.md#classicy_1_1Configuration_1a548f226233f711a8a5f16a00c5810835)

---

{#load}

#### load

`virtual`

```cpp
virtual void load(const std::string & path, bool create = false)
```

Defined in src/json/include/icy/json/configuration.h:38

Sets the file path and loads the configuration. 
#### Parameters
* `path` Absolute or relative path to the JSON file. 

* `create` Reserved for future use (currently unused). 

#### Exceptions
* `std::runtime_error` if the path is empty.

---

{#load-1}

#### load

`virtual`

```cpp
virtual void load(bool create = false)
```

Defined in src/json/include/icy/json/configuration.h:44

Reloads the configuration from the previously set path. Silently ignores parse errors (e.g. empty file). 
#### Parameters
* `create` Reserved for future use (currently unused). 

#### Exceptions
* `std::runtime_error` if the path has not been set.

---

{#save}

#### save

`virtual`

```cpp
virtual void save()
```

Defined in src/json/include/icy/json/configuration.h:48

Writes the current JSON root to the file at the stored path. 
#### Exceptions
* `std::runtime_error` if the path is empty or the file cannot be written.

---

{#remove}

#### remove

`virtual`

```cpp
virtual bool remove(const std::string & key)
```

Defined in src/json/include/icy/json/configuration.h:53

Removes the top-level key `key` from the JSON root. 
#### Parameters
* `key` Key to remove. 

#### Returns
true if the key existed and was removed.

---

{#removeall}

#### removeAll

`virtual`

```cpp
virtual void removeAll(const std::string & baseKey)
```

Defined in src/json/include/icy/json/configuration.h:57

Removes all top-level keys whose names contain `baseKey` as a substring. 
#### Parameters
* `baseKey` Substring to match against key names.

---

{#replace-2}

#### replace

`virtual`

```cpp
virtual void replace(const std::string & from, const std::string & to)
```

Defined in src/json/include/icy/json/configuration.h:63

Performs a global string substitution on the serialized JSON, replacing all occurrences of `from` with `to`, then re-parses. 
#### Parameters
* `from` Substring to find. 

* `to` Replacement string.

---

{#keys}

#### keys

`virtual`

```cpp
virtual void keys(std::vector< std::string > & keys, const std::string & baseKey = "")
```

Defined in src/json/include/icy/json/configuration.h:68

Populates `keys` with all top-level key names containing `baseKey` as a substring. 
#### Parameters
* `keys` Output vector to append matching key names to. 

* `baseKey` Filter substring; empty string matches all keys.

---

{#print-8}

#### print

`virtual`

```cpp
virtual void print(std::ostream & ost)
```

Defined in src/json/include/icy/json/configuration.h:72

Writes the pretty-printed JSON to `ost` with 4-space indentation. 
#### Parameters
* `ost` Output stream.

---

{#path-1}

#### path

`virtual`

```cpp
virtual std::string path()
```

Defined in src/json/include/icy/json/configuration.h:75

Returns the file path that was passed to [load()](#load).

---

{#loaded}

#### loaded

`virtual`

```cpp
virtual bool loaded()
```

Defined in src/json/include/icy/json/configuration.h:78

Returns true if [load()](#load) has been called at least once.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`_loaded`](#_loaded)  |  |
| `std::string` | [`_path`](#_path-3)  |  |
| `std::mutex` | [`_mutex`](#_mutex-9)  |  |

---

{#_loaded}

#### _loaded

```cpp
bool _loaded
```

Defined in src/json/include/icy/json/configuration.h:94

---

{#_path-3}

#### _path

```cpp
std::string _path
```

Defined in src/json/include/icy/json/configuration.h:95

---

{#_mutex-9}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/json/include/icy/json/configuration.h:96

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`getRaw`](#getraw) `virtual` `const` `override` | Retrieves the string value for `key` from the JSON root. |
| `void` | [`setRaw`](#setraw) `virtual` `override` | Stores `value` under `key` in the JSON root and emits PropertyChanged. |

---

{#getraw}

#### getRaw

`virtual` `const` `override`

```cpp
virtual bool getRaw(const std::string & key, std::string & value) const override
```

Defined in src/json/include/icy/json/configuration.h:87

Retrieves the string value for `key` from the JSON root. 
#### Parameters
* `key` Top-level JSON key. 

* `value` Set to the string value if the key exists. 

#### Returns
true if the key was found, false otherwise.

##### Reimplements

- [`getRaw`](base.md#classicy_1_1Configuration_1a93755fec3296753ec88e909095026046)

---

{#setraw}

#### setRaw

`virtual` `override`

```cpp
virtual void setRaw(const std::string & key, const std::string & value) override
```

Defined in src/json/include/icy/json/configuration.h:92

Stores `value` under `key` in the JSON root and emits PropertyChanged. 
#### Parameters
* `key` Top-level JSON key. 

* `value` String value to store.

##### Reimplements

- [`setRaw`](base.md#classicy_1_1Configuration_1afe19c7376a327bb2cc117f7a1b1a3b40)

{#iserializable}

## ISerializable

```cpp
#include <icy/json/iserializable.h>
```

```cpp
class ISerializable
```

Defined in src/json/include/icy/json/iserializable.h:24

> **Subclassed by:** [`Scheduler`](sched.md#scheduler), [`Task`](sched.md#task-1), [`Trigger`](sched.md#trigger-1)

Abstract interface for JSON-serializable objects.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`serialize`](#serialize-1) | `function` | Declared here |
| [`deserialize`](#deserialize-1) | `function` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`serialize`](#serialize-1) `virtual` | Serializes this object's state into `root`. |
| `void` | [`deserialize`](#deserialize-1) `virtual` | Populates this object's state from `root`. |

---

{#serialize-1}

#### serialize

`virtual`

```cpp
virtual void serialize(json::Value & root)
```

Defined in src/json/include/icy/json/iserializable.h:31

Serializes this object's state into `root`. 
#### Parameters
* `root` JSON object to populate.

##### Reimplemented by

- [`serialize`](sched.md#serialize-2)
- [`serialize`](sched.md#serialize-3)
- [`serialize`](sched.md#serialize-4)
- [`serialize`](sched.md#serialize-5)

---

{#deserialize-1}

#### deserialize

`virtual`

```cpp
virtual void deserialize(json::Value & root)
```

Defined in src/json/include/icy/json/iserializable.h:35

Populates this object's state from `root`. 
#### Parameters
* `root` JSON object previously produced by [serialize()](#serialize-1).

##### Reimplemented by

- [`deserialize`](sched.md#deserialize-2)
- [`deserialize`](sched.md#deserialize-3)
- [`deserialize`](sched.md#deserialize-4)
- [`deserialize`](sched.md#deserialize-5)

