{#stunmodulerfc5389}

# stun

The `[STUN module (rfc5389)](#stunmodulerfc5389)` module contains a STUN (rfc5389) implementation.

### Namespaces

| Name | Description |
|------|-------------|
| [`stun`](#stun) | STUN messages, attributes, and transaction helpers defined by RFC 5389. |

### Macros

| Name | Description |
|------|-------------|
| [`DECLARE_FIXLEN_STUN_ATTRIBUTE`](#declare_fixlen_stun_attribute)  | Attribute macros. |

---

{#declare_fixlen_stun_attribute}

#### DECLARE_FIXLEN_STUN_ATTRIBUTE

```cpp
DECLARE_FIXLEN_STUN_ATTRIBUTE()
```

Attribute macros.

{#stun}

# stun

STUN messages, attributes, and transaction helpers defined by RFC 5389.

### Classes

| Name | Description |
|------|-------------|
| [`AddressAttribute`](#addressattribute) | Implements a STUN/TURN attribute that contains a socket address. Handles XOR encoding/decoding for address and port as required by RFC 5389 section 15.2. |
| [`Attribute`](#attribute) | The virtual base class for all STUN/TURN attributes. |
| [`ErrorCode`](#errorcode-1) | Implements the STUN ERROR-CODE attribute (RFC 5389 section 15.6). Encodes a 3-digit error code as a class (hundreds digit) and number (tens + units digits), plus an optional UTF-8 reason phrase. |
| [`FlagAttribute`](#flagattribute) | Implements a zero-length STUN/TURN flag attribute (presence implies the flag is set). |
| [`Message`](#message-5) | STUN/TURN protocol message with method, class, transaction ID, and attributes. |
| [`MessageIntegrity`](#messageintegrity) | Implements the STUN MESSAGE-INTEGRITY attribute (RFC 5389 section 15.4). On write, computes an HMAC-SHA1 over the message bytes preceding this attribute when a key is set. On read, captures the raw HMAC bytes and the input bytes needed to verify them later via [verifyHmac()](#verifyhmac). |
| [`StringAttribute`](#stringattribute) | Implements a STUN/TURN attribute that holds an arbitrary byte string. Used for Username, Password, Realm, Nonce, Software, Data, and similar attributes. |
| [`Transaction`](#transaction-2) | STUN request/response transaction with timeout and retry logic. Extends the generic [net::Transaction](net.md#transaction) with STUN-specific transaction ID matching and response class inference (Success, [Error](base.md#error), or Indication). |
| [`UInt16ListAttribute`](#uint16listattribute) | Implements a STUN/TURN attribute that holds a list of attribute type codes. Used by the UNKNOWN-ATTRIBUTES attribute (RFC 5389 section 15.9). |
| [`UInt32Attribute`](#uint32attribute) | Implements a STUN/TURN attribute that holds a 32-bit integer. |
| [`UInt64Attribute`](#uint64attribute) | Implements a STUN/TURN attribute that holds a 64-bit integer. |
| [`UInt8Attribute`](#uint8attribute) | Implements a STUN/TURN attribute that holds an 8-bit integer. |

### Enumerations

| Name | Description |
|------|-------------|
| [`AddressFamily`](#addressfamily)  | STUN address types as defined in RFC 5389. NB: Undefined is not part of the STUN spec. |

---

{#addressfamily}

#### AddressFamily

```cpp
enum AddressFamily
```

STUN address types as defined in RFC 5389. NB: Undefined is not part of the STUN spec.

| Value | Description |
|-------|-------------|
| `Undefined` | Not a valid STUN address family; used as a sentinel. |
| `IPv4` | IPv4 transport address. |
| `IPv6` | IPv6 transport address. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`TransactionID`](#transactionid-1)  | Fixed-width 12-byte STUN transaction identifier stored as raw bytes. |

---

{#transactionid-1}

#### TransactionID

```cpp
using TransactionID = std::string
```

Fixed-width 12-byte STUN transaction identifier stored as raw bytes.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`isValidMethod`](#isvalidmethod) `nodiscard` `constexpr` | Returns true if `methodType` corresponds to a recognised STUN/TURN method. Used during parsing to reject malformed packets. |

---

{#isvalidmethod}

#### isValidMethod

`nodiscard` `constexpr`

```cpp
[[nodiscard]] constexpr bool isValidMethod(uint16_t methodType)
```

Returns true if `methodType` corresponds to a recognised STUN/TURN method. Used during parsing to reject malformed packets. 
#### Parameters
* `methodType` Raw method bits extracted from the message type field. 

#### Returns
true if the method is one of the defined MethodType values.

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`kAttributeHeaderSize`](#kattributeheadersize) `constexpr` | Bytes in a STUN attribute header. |
| `int` | [`kMessageHeaderSize`](#kmessageheadersize) `constexpr` | Bytes in the fixed STUN message header. |
| `int` | [`kTransactionIdOffset`](#ktransactionidoffset) `constexpr` | Byte offset of the transaction ID inside the message header. |
| `int` | [`kTransactionIdLength`](#ktransactionidlength) `constexpr` | Length in bytes of a STUN transaction ID. |
| `uint32_t` | [`kMagicCookie`](#kmagiccookie) `constexpr` | RFC 5389 magic cookie used by modern STUN/TURN messages. |
| `int` | [`kMagicCookieLength`](#kmagiccookielength) `constexpr` | Length in bytes of the magic cookie field. |

---

{#kattributeheadersize}

#### kAttributeHeaderSize

`constexpr`

```cpp
int kAttributeHeaderSize = 4
```

Bytes in a STUN attribute header.

---

{#kmessageheadersize}

#### kMessageHeaderSize

`constexpr`

```cpp
int kMessageHeaderSize = 20
```

Bytes in the fixed STUN message header.

---

{#ktransactionidoffset}

#### kTransactionIdOffset

`constexpr`

```cpp
int kTransactionIdOffset = 8
```

Byte offset of the transaction ID inside the message header.

---

{#ktransactionidlength}

#### kTransactionIdLength

`constexpr`

```cpp
int kTransactionIdLength = 12
```

Length in bytes of a STUN transaction ID.

---

{#kmagiccookie}

#### kMagicCookie

`constexpr`

```cpp
uint32_t kMagicCookie = 0x2112A442
```

RFC 5389 magic cookie used by modern STUN/TURN messages.

---

{#kmagiccookielength}

#### kMagicCookieLength

`constexpr`

```cpp
int kMagicCookieLength = sizeof()
```

Length in bytes of the magic cookie field.

{#addressattribute}

## AddressAttribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class AddressAttribute
```

Defined in src/stun/include/icy/stun/attributes.h:164

> **Inherits:** [`Attribute`](#attribute)

Implements a STUN/TURN attribute that contains a socket address. Handles XOR encoding/decoding for address and port as required by RFC 5389 section 15.2.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`AddressAttribute`](#addressattribute-1) | `function` | Declared here |
| [`AddressAttribute`](#addressattribute-2) | `function` | Declared here |
| [`clone`](#clone-7) | `function` | Declared here |
| [`family`](#family-2) | `function` | Declared here |
| [`address`](#address-12) | `function` | Declared here |
| [`read`](#read-2) | `function` | Declared here |
| [`write`](#write-17) | `function` | Declared here |
| [`setAddress`](#setaddress) | `function` | Declared here |
| [`isXorType`](#isxortype) | `function` | Declared here |
| [`IPv4Size`](#ipv4size) | `variable` | Declared here |
| [`IPv6Size`](#ipv6size) | `variable` | Declared here |
| [`_address`](#_address-1) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AddressAttribute`](#addressattribute-1)  |  |
|  | [`AddressAttribute`](#addressattribute-2)  | Copy constructor; duplicates the stored address. |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-7) `virtual` `override` | Returns a deep copy of this attribute. |
| `stun::AddressFamily` | [`family`](#family-2) `const` `inline` `nodiscard` |  |
| `net::Address` | [`address`](#address-12) `virtual` `const` `nodiscard` |  |
| `void` | [`read`](#read-2) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-17) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |
| `void` | [`setAddress`](#setaddress) `virtual` `inline` | Sets the address to encode into this attribute. |
| `bool` | [`isXorType`](#isxortype) `const` `inline` `nodiscard` |  |

---

{#addressattribute-1}

#### AddressAttribute

```cpp
AddressAttribute(uint16_t type, bool ipv4 = true)
```

Defined in src/stun/include/icy/stun/attributes.h:169

#### Parameters
* `type` Wire type code (e.g. XorMappedAddress::TypeID). 

* `ipv4` When true, initialises the size for IPv4; otherwise IPv6.

---

{#addressattribute-2}

#### AddressAttribute

```cpp
AddressAttribute(const AddressAttribute & r)
```

Defined in src/stun/include/icy/stun/attributes.h:172

Copy constructor; duplicates the stored address.

---

{#clone-7}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:175

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#family-2}

#### family

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline stun::AddressFamily family() const
```

Defined in src/stun/include/icy/stun/attributes.h:181

#### Returns
The STUN address family (IPv4, IPv6, or Undefined) of the stored address.

---

{#address-12}

#### address

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual net::Address address() const
```

Defined in src/stun/include/icy/stun/attributes.h:193

#### Returns
The decoded socket address stored in this attribute.

---

{#read-2}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:196

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-17}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:199

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

---

{#setaddress}

#### setAddress

`virtual` `inline`

```cpp
virtual inline void setAddress(const net::Address & addr)
```

Defined in src/stun/include/icy/stun/attributes.h:203

Sets the address to encode into this attribute. 
#### Parameters
* `addr` Address to store.

---

{#isxortype}

#### isXorType

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool isXorType() const
```

Defined in src/stun/include/icy/stun/attributes.h:217

#### Returns
true if this attribute type uses XOR encoding (RFC 5389).

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`IPv4Size`](#ipv4size) `static` `constexpr` |  |
| `uint16_t` | [`IPv6Size`](#ipv6size) `static` `constexpr` |  |

---

{#ipv4size}

#### IPv4Size

`static` `constexpr`

```cpp
uint16_t IPv4Size = 8
```

Defined in src/stun/include/icy/stun/attributes.h:177

---

{#ipv6size}

#### IPv6Size

`static` `constexpr`

```cpp
uint16_t IPv6Size = 20
```

Defined in src/stun/include/icy/stun/attributes.h:178

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::Address` | [`_address`](#_address-1)  |  |

---

{#_address-1}

#### _address

```cpp
net::Address _address
```

Defined in src/stun/include/icy/stun/attributes.h:224

{#attribute}

## Attribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class Attribute
```

Defined in src/stun/include/icy/stun/attributes.h:34

> **Subclassed by:** [`AddressAttribute`](#addressattribute), [`ErrorCode`](#errorcode-1), [`FlagAttribute`](#flagattribute), [`MessageIntegrity`](#messageintegrity), [`StringAttribute`](#stringattribute), [`UInt16ListAttribute`](#uint16listattribute), [`UInt32Attribute`](#uint32attribute), [`UInt64Attribute`](#uint64attribute), [`UInt8Attribute`](#uint8attribute)

The virtual base class for all STUN/TURN attributes.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`clone`](#clone-8) | `function` | Declared here |
| [`read`](#read-3) | `function` | Declared here |
| [`write`](#write-18) | `function` | Declared here |
| [`type`](#type-14) | `function` | Declared here |
| [`size`](#size-4) | `function` | Declared here |
| [`paddingBytes`](#paddingbytes) | `function` | Declared here |
| [`paddedBytes`](#paddedbytes) | `function` | Declared here |
| [`consumePadding`](#consumepadding) | `function` | Declared here |
| [`writePadding`](#writepadding) | `function` | Declared here |
| [`typeString`](#typestring) | `function` | Declared here |
| [`TypeID`](#typeid) | `variable` | Declared here |
| [`create`](#create-8) | `function` | Declared here |
| [`paddingBytes`](#paddingbytes-1) | `function` | Declared here |
| [`paddedBytes`](#paddedbytes-1) | `function` | Declared here |
| [`typeString`](#typestring-1) | `function` | Declared here |
| [`_type`](#_type-1) | `variable` | Declared here |
| [`_size`](#_size) | `variable` | Declared here |
| [`Attribute`](#attribute-1) | `function` | Declared here |
| [`setLength`](#setlength) | `function` | Declared here |
| [`Type`](#type-15) | `enum` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< Attribute >` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `void` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `uint16_t` | [`type`](#type-14) `const` `nodiscard` |  |
| `uint16_t` | [`size`](#size-4) `const` `nodiscard` |  |
| `uint16_t` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `uint16_t` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `void` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `void` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `std::string` | [`typeString`](#typestring) `nodiscard` |  |

---

{#clone-8}

#### clone

`virtual`

```cpp
virtual std::unique_ptr< Attribute > clone()
```

Defined in src/stun/include/icy/stun/attributes.h:86

Returns a deep copy of this attribute.

##### Reimplemented by

- [`clone`](#clone-7)
- [`clone`](#clone-9)
- [`clone`](#clone-10)
- [`clone`](#clone-12)
- [`clone`](#clone-13)
- [`clone`](#clone-14)
- [`clone`](#clone-15)
- [`clone`](#clone-16)
- [`clone`](#clone-17)

---

{#read-3}

#### read

`virtual`

```cpp
virtual void read(BitReader & reader)
```

Defined in src/stun/include/icy/stun/attributes.h:91

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplemented by

- [`read`](#read-2)
- [`read`](#read-4)
- [`read`](#read-5)
- [`read`](#read-7)
- [`read`](#read-8)
- [`read`](#read-9)
- [`read`](#read-10)
- [`read`](#read-11)
- [`read`](#read-12)

---

{#write-18}

#### write

`virtual` `const`

```cpp
virtual void write(BitWriter & writer) const
```

Defined in src/stun/include/icy/stun/attributes.h:95

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplemented by

- [`write`](#write-17)
- [`write`](#write-19)
- [`write`](#write-20)
- [`write`](#write-22)
- [`write`](#write-23)
- [`write`](#write-24)
- [`write`](#write-25)
- [`write`](#write-26)
- [`write`](#write-27)

---

{#type-14}

#### type

`const` `nodiscard`

```cpp
[[nodiscard]] uint16_t type() const
```

Defined in src/stun/include/icy/stun/attributes.h:105

#### Returns
The wire type code for this attribute.

---

{#size-4}

#### size

`const` `nodiscard`

```cpp
[[nodiscard]] uint16_t size() const
```

Defined in src/stun/include/icy/stun/attributes.h:108

#### Returns
The body length of this attribute in bytes (before padding).

---

{#paddingbytes}

#### paddingBytes

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint16_t paddingBytes() const
```

Defined in src/stun/include/icy/stun/attributes.h:123

#### Returns
The 4-byte alignment padding required for this attribute body.

---

{#paddedbytes}

#### paddedBytes

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint16_t paddedBytes() const
```

Defined in src/stun/include/icy/stun/attributes.h:126

#### Returns
The body length including 4-byte alignment padding.

---

{#consumepadding}

#### consumePadding

`const`

```cpp
void consumePadding(BitReader & reader) const
```

Defined in src/stun/include/icy/stun/attributes.h:131

Advances the reader past any 4-byte alignment padding that follows this attribute's body. 
#### Parameters
* `reader` Reader to advance.

---

{#writepadding}

#### writePadding

`const`

```cpp
void writePadding(BitWriter & writer) const
```

Defined in src/stun/include/icy/stun/attributes.h:136

Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. 
#### Parameters
* `writer` Writer to append padding to.

---

{#typestring}

#### typeString

`nodiscard`

```cpp
[[nodiscard]] std::string typeString()
```

Defined in src/stun/include/icy/stun/attributes.h:141

#### Returns
Human-readable name for this attribute's type.

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`TypeID`](#typeid) `static` `constexpr` |  |

---

{#typeid}

#### TypeID

`static` `constexpr`

```cpp
uint16_t TypeID = 0
```

Defined in src/stun/include/icy/stun/attributes.h:138

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< Attribute >` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `uint16_t` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `uint16_t` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `std::string` | [`typeString`](#typestring-1) `static` `nodiscard` |  |

---

{#create-8}

#### create

`static` `nodiscard`

```cpp
[[nodiscard]] static std::unique_ptr< Attribute > create(uint16_t type, uint16_t size = 0)
```

Defined in src/stun/include/icy/stun/attributes.h:102

Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. 
#### Parameters
* `type` Wire type code (one of [Attribute::Type](#type-15)). 

* `size` Body length in bytes as read from the wire header. 

#### Returns
Owning pointer to the new attribute, or nullptr on failure.

---

{#paddingbytes-1}

#### paddingBytes

`static` `inline` `nodiscard` `constexpr`

```cpp
[[nodiscard]] constexpr static inline uint16_t paddingBytes(uint16_t size)
```

Defined in src/stun/include/icy/stun/attributes.h:111

Returns the 4-byte alignment padding required for a body of `size` bytes.

---

{#paddedbytes-1}

#### paddedBytes

`static` `inline` `nodiscard` `constexpr`

```cpp
[[nodiscard]] constexpr static inline uint16_t paddedBytes(uint16_t size)
```

Defined in src/stun/include/icy/stun/attributes.h:117

Returns the body length including 4-byte alignment padding.

---

{#typestring-1}

#### typeString

`static` `nodiscard`

```cpp
[[nodiscard]] static std::string typeString(uint16_t type)
```

Defined in src/stun/include/icy/stun/attributes.h:145

#### Parameters
* `type` Wire type code. 

#### Returns
Human-readable name for the given type code.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`_type`](#_type-1)  |  |
| `uint16_t` | [`_size`](#_size)  |  |

---

{#_type-1}

#### _type

```cpp
uint16_t _type
```

Defined in src/stun/include/icy/stun/attributes.h:156

---

{#_size}

#### _size

```cpp
uint16_t _size
```

Defined in src/stun/include/icy/stun/attributes.h:157

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Attribute`](#attribute-1)  |  |
| `void` | [`setLength`](#setlength)  | Updates the stored body length. |

---

{#attribute-1}

#### Attribute

```cpp
Attribute(uint16_t type, uint16_t size = 0)
```

Defined in src/stun/include/icy/stun/attributes.h:150

#### Parameters
* `type` Wire type code for this attribute. 

* `size` Initial body length in bytes.

---

{#setlength}

#### setLength

```cpp
void setLength(uint16_t size)
```

Defined in src/stun/include/icy/stun/attributes.h:154

Updates the stored body length. 
#### Parameters
* `size` New body length in bytes.

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type-15)  |  |

---

{#type-15}

#### Type

```cpp
enum Type
```

Defined in src/stun/include/icy/stun/attributes.h:37

| Value | Description |
|-------|-------------|
| `NotExist` |  |
| `MappedAddress` |  |
| `ResponseAddress` |  |
| `ChangeRequest` |  |
| `SourceAddress` |  |
| `ChangedAddress` |  |
| `Username` |  |
| `Password` |  |
| `MessageIntegrity` |  |
| `ErrorCode` |  |
| `Bandwidth` |  |
| `DestinationAddress` |  |
| `UnknownAttributes` |  |
| `ReflectedFrom` |  |
| `MagicCookie` |  |
| `Realm` |  |
| `Nonce` |  |
| `XorMappedAddress` |  |
| `Software` |  |
| `Options` |  |
| `AlternateServer` |  |
| `Fingerprint` |  |
| `ChannelNumber` | TURN. |
| `Lifetime` |  |
| `XorPeerAddress` |  |
| `Data` |  |
| `XorRelayedAddress` |  |
| `EventPort` |  |
| `RequestedTransport` |  |
| `DontFragment` |  |
| `ReservationToken` | 0x0021: Reserved (was TIMER-VAL) |
| `ConnectionID` | TURN TCP. |
| `ICEControlled` | ICE. |
| `ICEControlling` |  |
| `ICEPriority` |  |
| `ICEUseCandidate` |  |

{#errorcode-1}

## ErrorCode

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class ErrorCode
```

Defined in src/stun/include/icy/stun/attributes.h:520

> **Inherits:** [`Attribute`](#attribute)

Implements the STUN ERROR-CODE attribute (RFC 5389 section 15.6). Encodes a 3-digit error code as a class (hundreds digit) and number (tens + units digits), plus an optional UTF-8 reason phrase.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ErrorCode`](#errorcode-2) | `function` | Declared here |
| [`ErrorCode`](#errorcode-3) | `function` | Declared here |
| [`clone`](#clone-9) | `function` | Declared here |
| [`setErrorCode`](#seterrorcode) | `function` | Declared here |
| [`setReason`](#setreason-1) | `function` | Declared here |
| [`errorCode`](#errorcode-4) | `function` | Declared here |
| [`errorClass`](#errorclass) | `function` | Declared here |
| [`errorNumber`](#errornumber-1) | `function` | Declared here |
| [`reason`](#reason-1) | `function` | Declared here |
| [`read`](#read-4) | `function` | Declared here |
| [`write`](#write-19) | `function` | Declared here |
| [`TypeID`](#typeid-1) | `variable` | Declared here |
| [`MinSize`](#minsize) | `variable` | Declared here |
| [`_class`](#_class) | `variable` | Declared here |
| [`_number`](#_number) | `variable` | Declared here |
| [`_reason`](#_reason-1) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ErrorCode`](#errorcode-2)  |  |
|  | [`ErrorCode`](#errorcode-3)  | Copy constructor. |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-9) `virtual` `override` | Returns a deep copy of this attribute. |
| `void` | [`setErrorCode`](#seterrorcode)  | Sets the error code, splitting it into class and number fields. |
| `void` | [`setReason`](#setreason-1)  | Sets the UTF-8 reason phrase and updates the attribute size. |
| `int` | [`errorCode`](#errorcode-4) `const` `nodiscard` |  |
| `uint8_t` | [`errorClass`](#errorclass) `const` `inline` `nodiscard` |  |
| `uint8_t` | [`errorNumber`](#errornumber-1) `const` `inline` `nodiscard` |  |
| `const std::string &` | [`reason`](#reason-1) `const` `inline` `nodiscard` |  |
| `void` | [`read`](#read-4) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-19) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |

---

{#errorcode-2}

#### ErrorCode

```cpp
ErrorCode(uint16_t size = MinSize)
```

Defined in src/stun/include/icy/stun/attributes.h:524

#### Parameters
* `size` Initial body length in bytes (must be >= MinSize).

---

{#errorcode-3}

#### ErrorCode

```cpp
ErrorCode(const ErrorCode & r)
```

Defined in src/stun/include/icy/stun/attributes.h:527

Copy constructor.

---

{#clone-9}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:531

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#seterrorcode}

#### setErrorCode

```cpp
void setErrorCode(int code)
```

Defined in src/stun/include/icy/stun/attributes.h:538

Sets the error code, splitting it into class and number fields. 
#### Parameters
* `code` 3-digit error code (e.g. 401, 438).

---

{#setreason-1}

#### setReason

```cpp
void setReason(const std::string & reason)
```

Defined in src/stun/include/icy/stun/attributes.h:542

Sets the UTF-8 reason phrase and updates the attribute size. 
#### Parameters
* `reason` Human-readable error description.

---

{#errorcode-4}

#### errorCode

`const` `nodiscard`

```cpp
[[nodiscard]] int errorCode() const
```

Defined in src/stun/include/icy/stun/attributes.h:545

#### Returns
The full 3-digit error code (class * 100 + number).

---

{#errorclass}

#### errorClass

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint8_t errorClass() const
```

Defined in src/stun/include/icy/stun/attributes.h:548

#### Returns
The hundreds digit of the error code (e.g. 4 for a 4xx error).

---

{#errornumber-1}

#### errorNumber

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint8_t errorNumber() const
```

Defined in src/stun/include/icy/stun/attributes.h:551

#### Returns
The tens+units portion of the error code (0-99).

---

{#reason-1}

#### reason

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & reason() const
```

Defined in src/stun/include/icy/stun/attributes.h:554

#### Returns
The reason phrase string (may be empty).

---

{#read-4}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:556

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-19}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:557

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`TypeID`](#typeid-1) `static` `constexpr` |  |
| `uint16_t` | [`MinSize`](#minsize) `static` `constexpr` | 4 bytes before the reason phrase. |

---

{#typeid-1}

#### TypeID

`static` `constexpr`

```cpp
uint16_t TypeID = 0x0009
```

Defined in src/stun/include/icy/stun/attributes.h:533

---

{#minsize}

#### MinSize

`static` `constexpr`

```cpp
uint16_t MinSize = 4
```

Defined in src/stun/include/icy/stun/attributes.h:534

4 bytes before the reason phrase.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint8_t` | [`_class`](#_class)  |  |
| `uint8_t` | [`_number`](#_number)  |  |
| `std::string` | [`_reason`](#_reason-1)  |  |

---

{#_class}

#### _class

```cpp
uint8_t _class
```

Defined in src/stun/include/icy/stun/attributes.h:560

---

{#_number}

#### _number

```cpp
uint8_t _number
```

Defined in src/stun/include/icy/stun/attributes.h:561

---

{#_reason-1}

#### _reason

```cpp
std::string _reason
```

Defined in src/stun/include/icy/stun/attributes.h:562

{#flagattribute}

## FlagAttribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class FlagAttribute
```

Defined in src/stun/include/icy/stun/attributes.h:348

> **Inherits:** [`Attribute`](#attribute)

Implements a zero-length STUN/TURN flag attribute (presence implies the flag is set).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`FlagAttribute`](#flagattribute-1) | `function` | Declared here |
| [`clone`](#clone-10) | `function` | Declared here |
| [`read`](#read-5) | `function` | Declared here |
| [`write`](#write-20) | `function` | Declared here |
| [`Size`](#size-5) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FlagAttribute`](#flagattribute-1)  |  |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-10) `virtual` `override` | Returns a deep copy of this attribute. |
| `void` | [`read`](#read-5) `virtual` `inline` `override` | No-op: flag attributes carry no body bytes. |
| `void` | [`write`](#write-20) `virtual` `const` `inline` `override` | No-op: flag attributes carry no body bytes. |

---

{#flagattribute-1}

#### FlagAttribute

```cpp
FlagAttribute(uint16_t type)
```

Defined in src/stun/include/icy/stun/attributes.h:352

#### Parameters
* `type` Wire type code for the concrete attribute.

---

{#clone-10}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:355

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#read-5}

#### read

`virtual` `inline` `override`

```cpp
virtual inline void read(BitReader &) override
```

Defined in src/stun/include/icy/stun/attributes.h:360

No-op: flag attributes carry no body bytes.

##### Reimplements

- [`read`](#read-3)

---

{#write-20}

#### write

`virtual` `const` `inline` `override`

```cpp
virtual inline void write(BitWriter &) const override
```

Defined in src/stun/include/icy/stun/attributes.h:363

No-op: flag attributes carry no body bytes.

##### Reimplements

- [`write`](#write-18)

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`Size`](#size-5) `static` `constexpr` |  |

---

{#size-5}

#### Size

`static` `constexpr`

```cpp
uint16_t Size = 0
```

Defined in src/stun/include/icy/stun/attributes.h:357

{#message-5}

## Message

```cpp
#include <icy/stun/message.h>
```

```cpp
class Message
```

Defined in src/stun/include/icy/stun/message.h:31

> **Inherits:** [`IPacket`](base.md#ipacket)
> **Subclassed by:** [`Request`](turn.md#request-12)

STUN/TURN protocol message with method, class, transaction ID, and attributes.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Message`](#message-6) | `function` | Declared here |
| [`Message`](#message-7) | `function` | Declared here |
| [`Message`](#message-8) | `function` | Declared here |
| [`Message`](#message-9) | `function` | Declared here |
| [`operator=`](#operator-30) | `function` | Declared here |
| [`operator=`](#operator-31) | `function` | Declared here |
| [`clone`](#clone-11) | `function` | Declared here |
| [`setClass`](#setclass) | `function` | Declared here |
| [`setMethod`](#setmethod-1) | `function` | Declared here |
| [`setTransactionID`](#settransactionid) | `function` | Declared here |
| [`classType`](#classtype) | `function` | Declared here |
| [`methodType`](#methodtype) | `function` | Declared here |
| [`transactionID`](#transactionid-2) | `function` | Declared here |
| [`size`](#size-6) | `function` | Declared here |
| [`methodString`](#methodstring) | `function` | Declared here |
| [`classString`](#classstring) | `function` | Declared here |
| [`errorString`](#errorstring) | `function` | Declared here |
| [`add`](#add-1) | `function` | Declared here |
| [`add`](#add-2) | `function` | Declared here |
| [`get`](#get-5) | `function` | Declared here |
| [`get`](#get-6) | `function` | Declared here |
| [`read`](#read-6) | `function` | Declared here |
| [`write`](#write-21) | `function` | Declared here |
| [`toString`](#tostring-8) | `function` | Declared here |
| [`print`](#print-13) | `function` | Declared here |
| [`className`](#classname-7) | `function` | Declared here |
| [`_class`](#_class-1) | `variable` | Declared here |
| [`_method`](#_method-1) | `variable` | Declared here |
| [`_size`](#_size-1) | `variable` | Declared here |
| [`_transactionID`](#_transactionid) | `variable` | Declared here |
| [`_attrs`](#_attrs) | `variable` | Declared here |
| [`MethodType`](#methodtype-1) | `enum` | Declared here |
| [`ClassType`](#classtype-1) | `enum` | Declared here |
| [`ErrorCodes`](#errorcodes) | `enum` | Declared here |
| [`computeBodySize`](#computebodysize) | `function` | Declared here |
| [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) | `friend` | Inherited from [`IPacket`](base.md#ipacket) |

### Inherited from [`IPacket`](base.md#ipacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930)  | Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime. |
| `variable` | [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)  | Optional extra information about the packet. |
| `variable` | [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0)  | Provides basic information about the packet. |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) `inline` |  |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) `inline` | Copy constructor; clones the info object if present. |
| `function` | [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) `inline` | Copy assignment; clones the info object if present. |
| `function` | [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) `virtual` `const` | Returns a heap-allocated deep copy of this packet. |
| `function` | [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) `virtual` | Defaulted destructor. |
| `function` | [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) `virtual` | Read/parse to the packet from the given input buffer. The number of bytes read is returned. |
| `function` | [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) `virtual` `const` | Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer. |
| `function` | [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) `virtual` `const` `inline` | The size of the packet in bytes. |
| `function` | [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |
| `function` | [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) `virtual` `const` `inline` | The const packet data pointer for buffered packets. |
| `function` | [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) `virtual` `const` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) `virtual` `const` `inline` | Prints a human-readable representation to the given stream. |
| `friend` | [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) `inline` | [Stream](base.md#stream) insertion operator; delegates to [print()](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Message`](#message-6)  | Constructs a default message (Request class, Undefined method) with a randomly generated 12-byte transaction ID. |
|  | [`Message`](#message-7)  | Constructs a message with explicit class and method. |
|  | [`Message`](#message-8)  | Deep-copy constructor; clones all attributes. |
|  | [`Message`](#message-9) `noexcept` | Move constructor. |
| `Message &` | [`operator=`](#operator-30)  | Deep-copy assignment; clones all attributes from `that`. |
| `Message &` | [`operator=`](#operator-31) `noexcept` | Move assignment. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-11) `virtual` `const` `override` |  |
| `void` | [`setClass`](#setclass)  | Sets the message class field. |
| `void` | [`setMethod`](#setmethod-1)  | Sets the message method field. |
| `void` | [`setTransactionID`](#settransactionid)  | Sets the 12-byte transaction ID. |
| `ClassType` | [`classType`](#classtype) `const` `nodiscard` |  |
| `MethodType` | [`methodType`](#methodtype) `const` `nodiscard` |  |
| `const TransactionID &` | [`transactionID`](#transactionid-2) `const` `inline` `nodiscard` |  |
| `size_t` | [`size`](#size-6) `virtual` `const` `inline` `nodiscard` `override` |  |
| `std::string` | [`methodString`](#methodstring) `const` `nodiscard` |  |
| `std::string` | [`classString`](#classstring) `const` `nodiscard` |  |
| `std::string` | [`errorString`](#errorstring) `const` `nodiscard` | Maps a numeric error code to its canonical string description. |
| `T &` | [`add`](#add-1) `inline` | Constructs an attribute of type T in-place and appends it to the message. Returns a reference to the new attribute for further configuration. |
| `void` | [`add`](#add-2)  | Appends an attribute to the message, taking ownership via unique_ptr. |
| `Attribute *` | [`get`](#get-5) `const` `nodiscard` | Returns the Nth attribute of the given type, or nullptr if not found. |
| `T *` | [`get`](#get-6) `const` `inline` `nodiscard` | Type-safe attribute accessor using the concrete attribute's TypeID. |
| `ssize_t` | [`read`](#read-6) `virtual` `override` | Parses a STUN/TURN packet from the given buffer. |
| `void` | [`write`](#write-21) `virtual` `const` `override` | Serialises this message into a STUN/TURN wire-format packet. |
| `std::string` | [`toString`](#tostring-8) `const` `nodiscard` |  |
| `void` | [`print`](#print-13) `virtual` `const` `override` | Writes the same representation as [toString()](#tostring-8) to the given stream. |
| `const char *` | [`className`](#classname-7) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

---

{#message-6}

#### Message

```cpp
Message()
```

Defined in src/stun/include/icy/stun/message.h:92

Constructs a default message (Request class, Undefined method) with a randomly generated 12-byte transaction ID.

---

{#message-7}

#### Message

```cpp
Message(ClassType clss, MethodType meth)
```

Defined in src/stun/include/icy/stun/message.h:97

Constructs a message with explicit class and method. 
#### Parameters
* `clss` [Message](#message-5) class (Request, Indication, SuccessResponse, or ErrorResponse). 

* `meth` [Message](#message-5) method (Binding, Allocate, Refresh, etc.).

---

{#message-8}

#### Message

```cpp
Message(const Message & that)
```

Defined in src/stun/include/icy/stun/message.h:100

Deep-copy constructor; clones all attributes.

---

{#message-9}

#### Message

`noexcept`

```cpp
Message(Message && that) noexcept
```

Defined in src/stun/include/icy/stun/message.h:103

Move constructor.

---

{#operator-30}

#### operator=

```cpp
Message & operator=(const Message & that)
```

Defined in src/stun/include/icy/stun/message.h:106

Deep-copy assignment; clones all attributes from `that`.

---

{#operator-31}

#### operator=

`noexcept`

```cpp
Message & operator=(Message && that) noexcept
```

Defined in src/stun/include/icy/stun/message.h:109

Move assignment.

---

{#clone-11}

#### clone

`virtual` `const` `override`

```cpp
virtual std::unique_ptr< IPacket > clone() const override
```

Defined in src/stun/include/icy/stun/message.h:114

#### Returns
A heap-allocated deep copy of this message.

##### Reimplements

- [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7)

---

{#setclass}

#### setClass

```cpp
void setClass(ClassType type)
```

Defined in src/stun/include/icy/stun/message.h:118

Sets the message class field. 
#### Parameters
* `type` One of Request, Indication, SuccessResponse, ErrorResponse.

---

{#setmethod-1}

#### setMethod

```cpp
void setMethod(MethodType type)
```

Defined in src/stun/include/icy/stun/message.h:122

Sets the message method field. 
#### Parameters
* `type` One of the [MethodType](#methodtype-1) enumerators.

---

{#settransactionid}

#### setTransactionID

```cpp
void setTransactionID(const std::string & id)
```

Defined in src/stun/include/icy/stun/message.h:126

Sets the 12-byte transaction ID. 
#### Parameters
* `id` Must be exactly kTransactionIdLength (12) bytes.

---

{#classtype}

#### classType

`const` `nodiscard`

```cpp
[[nodiscard]] ClassType classType() const
```

Defined in src/stun/include/icy/stun/message.h:129

#### Returns
The message class.

---

{#methodtype}

#### methodType

`const` `nodiscard`

```cpp
[[nodiscard]] MethodType methodType() const
```

Defined in src/stun/include/icy/stun/message.h:132

#### Returns
The message method.

---

{#transactionid-2}

#### transactionID

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const TransactionID & transactionID() const
```

Defined in src/stun/include/icy/stun/message.h:135

#### Returns
Reference to the 12-byte transaction ID string.

---

{#size-6}

#### size

`virtual` `const` `inline` `nodiscard` `override`

```cpp
[[nodiscard]] virtual inline size_t size() const override
```

Defined in src/stun/include/icy/stun/message.h:138

#### Returns
Total body size in bytes (sum of padded attribute headers and bodies).

##### Reimplements

- [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c)

---

{#methodstring}

#### methodString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string methodString() const
```

Defined in src/stun/include/icy/stun/message.h:141

#### Returns
Human-readable method name (e.g. "BINDING", "ALLOCATE").

---

{#classstring}

#### classString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string classString() const
```

Defined in src/stun/include/icy/stun/message.h:144

#### Returns
Human-readable class name (e.g. "Request", "SuccessResponse").

---

{#errorstring}

#### errorString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string errorString(uint16_t errorCode) const
```

Defined in src/stun/include/icy/stun/message.h:149

Maps a numeric error code to its canonical string description. 
#### Parameters
* `errorCode` One of the [ErrorCodes](#errorcodes) enumerators. 

#### Returns
Human-readable error string, or "UnknownError" if not recognised.

---

{#add-1}

#### add

`inline`

```cpp
template<typename T> inline T & add()
```

Defined in src/stun/include/icy/stun/message.h:156

Constructs an attribute of type T in-place and appends it to the message. Returns a reference to the new attribute for further configuration. 
#### Parameters
* `T` Concrete attribute type (e.g. stun::Lifetime, stun::XorMappedAddress). 

#### Returns
Reference to the newly added attribute.

---

{#add-2}

#### add

```cpp
void add(std::unique_ptr< Attribute > attr)
```

Defined in src/stun/include/icy/stun/message.h:166

Appends an attribute to the message, taking ownership via unique_ptr. 
#### Parameters
* `attr` [Attribute](#attribute) to add.

---

{#get-5}

#### get

`const` `nodiscard`

```cpp
[[nodiscard]] Attribute * get(Attribute::Type type, int index = 0) const
```

Defined in src/stun/include/icy/stun/message.h:172

Returns the Nth attribute of the given type, or nullptr if not found. 
#### Parameters
* `type` [Attribute](#attribute) type code to search for. 

* `index` Zero-based occurrence index (0 = first match). 

#### Returns
Raw pointer to the attribute (owned by this message), or nullptr.

---

{#get-6}

#### get

`const` `inline` `nodiscard`

```cpp
template<typename T> [[nodiscard]] inline T * get(int index = 0) const
```

Defined in src/stun/include/icy/stun/message.h:179

Type-safe attribute accessor using the concrete attribute's TypeID. 
#### Parameters
* `T` Concrete attribute type (must define TypeID). 

#### Parameters
* `index` Zero-based occurrence index. 

#### Returns
Pointer to T, or nullptr if the attribute is absent.

---

{#read-6}

#### read

`virtual` `override`

```cpp
virtual ssize_t read(const ConstBuffer & buf) override
```

Defined in src/stun/include/icy/stun/message.h:188

Parses a STUN/TURN packet from the given buffer. 
#### Parameters
* `buf` [Buffer](base.md#buffer-2) containing at least one complete STUN message. 

#### Returns
Number of bytes consumed, or 0 on parse failure.

##### Reimplements

- [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b)

---

{#write-21}

#### write

`virtual` `const` `override`

```cpp
virtual void write(Buffer & buf) const override
```

Defined in src/stun/include/icy/stun/message.h:192

Serialises this message into a STUN/TURN wire-format packet. 
#### Parameters
* `buf` Destination buffer; data is appended.

##### Reimplements

- [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c)

---

{#tostring-8}

#### toString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string toString() const
```

Defined in src/stun/include/icy/stun/message.h:195

#### Returns
A concise string representation for logging (method, transaction ID, attribute types).

---

{#print-13}

#### print

`virtual` `const` `override`

```cpp
virtual void print(std::ostream & os) const override
```

Defined in src/stun/include/icy/stun/message.h:199

Writes the same representation as [toString()](#tostring-8) to the given stream. 
#### Parameters
* `os` Output stream.

##### Reimplements

- [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc)

---

{#classname-7}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/stun/include/icy/stun/message.h:201

Returns the class name of this packet type for logging and diagnostics.

##### Reimplements

- [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1)

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`_class`](#_class-1)  |  |
| `uint16_t` | [`_method`](#_method-1)  |  |
| `uint16_t` | [`_size`](#_size-1)  | Set by [read()](#read-6); [write()](#write-21) uses computeBodySize() instead. |
| `TransactionID` | [`_transactionID`](#_transactionid)  |  |
| `std::vector< std::unique_ptr< Attribute > >` | [`_attrs`](#_attrs)  |  |

---

{#_class-1}

#### _class

```cpp
uint16_t _class
```

Defined in src/stun/include/icy/stun/message.h:204

---

{#_method-1}

#### _method

```cpp
uint16_t _method
```

Defined in src/stun/include/icy/stun/message.h:205

---

{#_size-1}

#### _size

```cpp
uint16_t _size
```

Defined in src/stun/include/icy/stun/message.h:206

Set by [read()](#read-6); [write()](#write-21) uses computeBodySize() instead.

---

{#_transactionid}

#### _transactionID

```cpp
TransactionID _transactionID
```

Defined in src/stun/include/icy/stun/message.h:207

---

{#_attrs}

#### _attrs

```cpp
std::vector< std::unique_ptr< Attribute > > _attrs
```

Defined in src/stun/include/icy/stun/message.h:208

### Public Types

| Name | Description |
|------|-------------|
| [`MethodType`](#methodtype-1)  |  |
| [`ClassType`](#classtype-1)  |  |
| [`ErrorCodes`](#errorcodes)  |  |

---

{#methodtype-1}

#### MethodType

```cpp
enum MethodType
```

Defined in src/stun/include/icy/stun/message.h:34

| Value | Description |
|-------|-------------|
| `Undefined` | default error type |
| `Binding` | STUN. |
| `Allocate` | TURN. |
| `Refresh` |  |
| `SendIndication` | (only indication semantics defined) |
| `DataIndication` | (only indication semantics defined) |
| `CreatePermission` | (only request/response semantics defined) |
| `ChannelBind` | (only request/response semantics defined) |
| `Connect` | TURN TCP RFC 6062. |
| `ConnectionBind` |  |
| `ConnectionAttempt` |  |

---

{#classtype-1}

#### ClassType

```cpp
enum ClassType
```

Defined in src/stun/include/icy/stun/message.h:55

| Value | Description |
|-------|-------------|
| `Request` |  |
| `Indication` |  |
| `SuccessResponse` |  |
| `ErrorResponse` |  |

---

{#errorcodes}

#### ErrorCodes

```cpp
enum ErrorCodes
```

Defined in src/stun/include/icy/stun/message.h:63

| Value | Description |
|-------|-------------|
| `TryAlternate` |  |
| `BadRequest` |  |
| `NotAuthorized` |  |
| `Forbidden` |  |
| `UnknownAttribute` |  |
| `StaleCredentials` |  |
| `IntegrityCheckFailure` |  |
| `MissingUsername` |  |
| `UseTLS` |  |
| `AllocationMismatch` |  |
| `StaleNonce` |  |
| `WrongCredentials` |  |
| `UnsupportedTransport` |  |
| `AllocationQuotaReached` |  |
| `RoleConflict` |  |
| `ServerError` |  |
| `InsufficientCapacity` |  |
| `GlobalFailure` |  |
| `ConnectionAlreadyExists` | TURN TCP. |
| `ConnectionTimeoutOrFailure` |  |

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`computeBodySize`](#computebodysize) `const` `nodiscard` | Computes the wire body size from the current attribute list. |

---

{#computebodysize}

#### computeBodySize

`const` `nodiscard`

```cpp
[[nodiscard]] uint16_t computeBodySize() const
```

Defined in src/stun/include/icy/stun/message.h:212

Computes the wire body size from the current attribute list.

{#messageintegrity}

## MessageIntegrity

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class MessageIntegrity
```

Defined in src/stun/include/icy/stun/attributes.h:468

> **Inherits:** [`Attribute`](#attribute)

Implements the STUN MESSAGE-INTEGRITY attribute (RFC 5389 section 15.4). On write, computes an HMAC-SHA1 over the message bytes preceding this attribute when a key is set. On read, captures the raw HMAC bytes and the input bytes needed to verify them later via [verifyHmac()](#verifyhmac).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`MessageIntegrity`](#messageintegrity-1) | `function` | Declared here |
| [`MessageIntegrity`](#messageintegrity-2) | `function` | Declared here |
| [`clone`](#clone-12) | `function` | Declared here |
| [`verifyHmac`](#verifyhmac) | `function` | Declared here |
| [`input`](#input) | `function` | Declared here |
| [`hmac`](#hmac) | `function` | Declared here |
| [`key`](#key-2) | `function` | Declared here |
| [`setInput`](#setinput) | `function` | Declared here |
| [`setHmac`](#sethmac) | `function` | Declared here |
| [`setKey`](#setkey) | `function` | Declared here |
| [`read`](#read-7) | `function` | Declared here |
| [`write`](#write-22) | `function` | Declared here |
| [`TypeID`](#typeid-2) | `variable` | Declared here |
| [`Size`](#size-7) | `variable` | Declared here |
| [`_input`](#_input) | `variable` | Declared here |
| [`_hmac`](#_hmac) | `variable` | Declared here |
| [`_key`](#_key-1) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MessageIntegrity`](#messageintegrity-1)  |  |
|  | [`MessageIntegrity`](#messageintegrity-2)  |  |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-12) `virtual` `override` | Returns a deep copy of this attribute. |
| `bool` | [`verifyHmac`](#verifyhmac) `const` `nodiscard` | Verifies the stored HMAC against the stored input bytes using `key`. |
| `std::string` | [`input`](#input) `const` `inline` `nodiscard` |  |
| `std::string` | [`hmac`](#hmac) `const` `inline` `nodiscard` |  |
| `std::string` | [`key`](#key-2) `const` `inline` `nodiscard` |  |
| `void` | [`setInput`](#setinput) `inline` | Sets the raw message bytes used as HMAC input during verification. |
| `void` | [`setHmac`](#sethmac) `inline` | Sets the raw HMAC value (used when copying a received attribute). |
| `void` | [`setKey`](#setkey) `inline` | Sets the HMAC key; triggers HMAC computation on [write()](#write-22). |
| `void` | [`read`](#read-7) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-22) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |

---

{#messageintegrity-1}

#### MessageIntegrity

```cpp
MessageIntegrity()
```

Defined in src/stun/include/icy/stun/attributes.h:471

---

{#messageintegrity-2}

#### MessageIntegrity

```cpp
MessageIntegrity(const MessageIntegrity & r)
```

Defined in src/stun/include/icy/stun/attributes.h:472

---

{#clone-12}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:476

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#verifyhmac}

#### verifyHmac

`const` `nodiscard`

```cpp
[[nodiscard]] bool verifyHmac(std::string_view key) const
```

Defined in src/stun/include/icy/stun/attributes.h:484

Verifies the stored HMAC against the stored input bytes using `key`. 
#### Parameters
* `key` HMAC key (MD5 of username:realm:password for long-term creds). 

#### Returns
true if the computed HMAC matches the stored HMAC.

---

{#input}

#### input

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline std::string input() const
```

Defined in src/stun/include/icy/stun/attributes.h:487

#### Returns
The raw message bytes captured at read time, used for HMAC verification.

---

{#hmac}

#### hmac

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline std::string hmac() const
```

Defined in src/stun/include/icy/stun/attributes.h:490

#### Returns
The raw 20-byte HMAC value as read from the wire.

---

{#key-2}

#### key

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline std::string key() const
```

Defined in src/stun/include/icy/stun/attributes.h:493

#### Returns
The HMAC key set for outgoing message signing (empty if not set).

---

{#setinput}

#### setInput

`inline`

```cpp
inline void setInput(const std::string & input)
```

Defined in src/stun/include/icy/stun/attributes.h:497

Sets the raw message bytes used as HMAC input during verification. 
#### Parameters
* `input` Byte string of the message up to this attribute.

---

{#sethmac}

#### setHmac

`inline`

```cpp
inline void setHmac(const std::string & hmac)
```

Defined in src/stun/include/icy/stun/attributes.h:501

Sets the raw HMAC value (used when copying a received attribute). 
#### Parameters
* `hmac` 20-byte HMAC string.

---

{#setkey}

#### setKey

`inline`

```cpp
inline void setKey(const std::string & key)
```

Defined in src/stun/include/icy/stun/attributes.h:505

Sets the HMAC key; triggers HMAC computation on [write()](#write-22). 
#### Parameters
* `key` MD5 digest of the long-term credential (username:realm:password).

---

{#read-7}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:507

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-22}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:508

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`TypeID`](#typeid-2) `static` `constexpr` |  |
| `uint16_t` | [`Size`](#size-7) `static` `constexpr` | HMAC-SHA1 output is always 20 bytes. |

---

{#typeid-2}

#### TypeID

`static` `constexpr`

```cpp
uint16_t TypeID = 0x0008
```

Defined in src/stun/include/icy/stun/attributes.h:478

---

{#size-7}

#### Size

`static` `constexpr`

```cpp
uint16_t Size = 20
```

Defined in src/stun/include/icy/stun/attributes.h:479

HMAC-SHA1 output is always 20 bytes.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_input`](#_input)  |  |
| `std::string` | [`_hmac`](#_hmac)  |  |
| `std::string` | [`_key`](#_key-1)  |  |

---

{#_input}

#### _input

```cpp
std::string _input
```

Defined in src/stun/include/icy/stun/attributes.h:511

---

{#_hmac}

#### _hmac

```cpp
std::string _hmac
```

Defined in src/stun/include/icy/stun/attributes.h:512

---

{#_key-1}

#### _key

```cpp
std::string _key
```

Defined in src/stun/include/icy/stun/attributes.h:513

{#stringattribute}

## StringAttribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class StringAttribute
```

Defined in src/stun/include/icy/stun/attributes.h:369

> **Inherits:** [`Attribute`](#attribute)

Implements a STUN/TURN attribute that holds an arbitrary byte string. Used for Username, Password, Realm, Nonce, Software, Data, and similar attributes.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`StringAttribute`](#stringattribute-1) | `function` | Declared here |
| [`StringAttribute`](#stringattribute-2) | `function` | Declared here |
| [`clone`](#clone-13) | `function` | Declared here |
| [`bytes`](#bytes-1) | `function` | Declared here |
| [`setBytes`](#setbytes) | `function` | Declared here |
| [`asString`](#asstring) | `function` | Declared here |
| [`copyBytes`](#copybytes) | `function` | Declared here |
| [`copyBytes`](#copybytes-1) | `function` | Declared here |
| [`getByte`](#getbyte) | `function` | Declared here |
| [`setByte`](#setbyte) | `function` | Declared here |
| [`read`](#read-8) | `function` | Declared here |
| [`write`](#write-23) | `function` | Declared here |
| [`_bytes`](#_bytes) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`StringAttribute`](#stringattribute-1)  |  |
|  | [`StringAttribute`](#stringattribute-2)  | Copy constructor; duplicates stored bytes. |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-13) `virtual` `override` | Returns a deep copy of this attribute. |
| `const char *` | [`bytes`](#bytes-1) `const` `inline` `nodiscard` |  |
| `void` | [`setBytes`](#setbytes)  | Replaces the stored bytes with a copy of the given buffer and updates the attribute's reported size. |
| `std::string` | [`asString`](#asstring) `const` `nodiscard` |  |
| `void` | [`copyBytes`](#copybytes)  | Copies a null-terminated string into the attribute, using strlen to determine the length. |
| `void` | [`copyBytes`](#copybytes-1)  | Copies an arbitrary block of memory into the attribute. |
| `uint8_t` | [`getByte`](#getbyte) `const` `nodiscard` | Returns a single byte from the stored buffer. |
| `void` | [`setByte`](#setbyte)  | Overwrites a single byte in the stored buffer. |
| `void` | [`read`](#read-8) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-23) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |

---

{#stringattribute-1}

#### StringAttribute

```cpp
StringAttribute(uint16_t type, uint16_t size = 0)
```

Defined in src/stun/include/icy/stun/attributes.h:374

#### Parameters
* `type` Wire type code for the concrete attribute. 

* `size` Initial body length in bytes (0 for variable-length attributes).

---

{#stringattribute-2}

#### StringAttribute

```cpp
StringAttribute(const StringAttribute & r)
```

Defined in src/stun/include/icy/stun/attributes.h:377

Copy constructor; duplicates stored bytes.

---

{#clone-13}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:381

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#bytes-1}

#### bytes

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const char * bytes() const
```

Defined in src/stun/include/icy/stun/attributes.h:384

#### Returns
Pointer to the raw byte buffer.

---

{#setbytes}

#### setBytes

```cpp
void setBytes(const char * bytes, size_t size)
```

Defined in src/stun/include/icy/stun/attributes.h:390

Replaces the stored bytes with a copy of the given buffer and updates the attribute's reported size. 
#### Parameters
* `bytes` Source data pointer. 

* `size` Number of bytes to copy.

---

{#asstring}

#### asString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string asString() const
```

Defined in src/stun/include/icy/stun/attributes.h:393

#### Returns
The stored bytes as a std::string.

---

{#copybytes}

#### copyBytes

```cpp
void copyBytes(const char * bytes)
```

Defined in src/stun/include/icy/stun/attributes.h:398

Copies a null-terminated string into the attribute, using strlen to determine the length. 
#### Parameters
* `bytes` Null-terminated source string.

---

{#copybytes-1}

#### copyBytes

```cpp
void copyBytes(const void * bytes, size_t size)
```

Defined in src/stun/include/icy/stun/attributes.h:403

Copies an arbitrary block of memory into the attribute. 
#### Parameters
* `bytes` Source data pointer. 

* `size` Number of bytes to copy.

---

{#getbyte}

#### getByte

`const` `nodiscard`

```cpp
[[nodiscard]] uint8_t getByte(int index) const
```

Defined in src/stun/include/icy/stun/attributes.h:408

Returns a single byte from the stored buffer. 
#### Parameters
* `index` Zero-based byte offset. 

#### Returns
The byte value at `index`.

---

{#setbyte}

#### setByte

```cpp
void setByte(int index, uint8_t value)
```

Defined in src/stun/include/icy/stun/attributes.h:413

Overwrites a single byte in the stored buffer. 
#### Parameters
* `index` Zero-based byte offset. 

* `value` New value to write.

---

{#read-8}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:415

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-23}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:416

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::vector< char >` | [`_bytes`](#_bytes)  |  |

---

{#_bytes}

#### _bytes

```cpp
std::vector< char > _bytes
```

Defined in src/stun/include/icy/stun/attributes.h:419

{#transaction-2}

## Transaction

```cpp
#include <icy/stun/transaction.h>
```

```cpp
class Transaction
```

Defined in src/stun/include/icy/stun/transaction.h:31

> **Inherits:** [`Transaction< Message >`](net.md#transaction)

STUN request/response transaction with timeout and retry logic. Extends the generic [net::Transaction](net.md#transaction) with STUN-specific transaction ID matching and response class inference (Success, [Error](base.md#error), or Indication).

Lifetime is managed by [IntrusivePtr](base.md#intrusiveptr). Create via makeIntrusive or wrap in [IntrusivePtr](base.md#intrusiveptr) immediately after construction.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`IntrusivePtr`](#icy-intrusiveptr-1) | `friend` | Declared here |
| [`Transaction`](#transaction-3) | `function` | Declared here |
| [`checkResponse`](#checkresponse-1) | `function` | Declared here |
| [`onResponse`](#onresponse-1) | `function` | Declared here |
| [`Ptr`](#ptr-16) | `typedef` | Declared here |
| [`IntrusivePtr`](net.md#icy-intrusiveptr) | `friend` | Inherited from [`Transaction`](net.md#transaction) |
| [`Transaction`](net.md#transaction-1) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`send`](net.md#send-6) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`cancel`](net.md#cancel) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`dispose`](net.md#dispose) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`peerAddress`](net.md#peeraddress-3) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`_peerAddress`](net.md#_peeraddress-1) | `variable` | Inherited from [`Transaction`](net.md#transaction) |
| [`onPacket`](net.md#onpacket-1) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`onResponse`](net.md#onresponse) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`checkResponse`](net.md#checkresponse) | `function` | Inherited from [`Transaction`](net.md#transaction) |
| [`BaseT`](net.md#baset) | `typedef` | Inherited from [`Transaction`](net.md#transaction) |
| [`Ptr`](base.md#classicy_1_1PacketTransaction_1a44ea6d9d47efb59eebdc71158c0158c5) | `typedef` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`IntrusivePtr`](base.md#classicy_1_1PacketTransaction_1a04a374ecab2499e81653198fd49ff15e) | `friend` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_request`](base.md#classicy_1_1PacketTransaction_1a03e21f66e0f957926f03f37b48276dca) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_response`](base.md#classicy_1_1PacketTransaction_1a6322ae70f87db74c1d5c130b7fec7eb8) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_timer`](base.md#classicy_1_1PacketTransaction_1a1b02dbf0ac65fcad82c50a6867f81a79) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_retries`](base.md#classicy_1_1PacketTransaction_1a8c76a907486cae828c77f0302d20a844) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_attempts`](base.md#classicy_1_1PacketTransaction_1ace42ededd6279b9a3ce13bdfd6619c11) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_disposed`](base.md#classicy_1_1PacketTransaction_1a70d8cc5501fba3b9c26426af411241a4) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a5b3c9258f951866005347123bdf7a7ff) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a711873135d73ab6cb3426db65e980196) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`send`](base.md#classicy_1_1PacketTransaction_1acf7a9bbcfff95999c0c227cb36f9cfd4) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`cancel`](base.md#classicy_1_1PacketTransaction_1ab7c1e492203b850fc9b5c1aa4f97c3ef) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`cancelled`](base.md#classicy_1_1PacketTransaction_1a24663172b10c70530b29c9dccdf5813d) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`dispose`](base.md#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`disposed`](base.md#classicy_1_1PacketTransaction_1a2a85d3be5eb4088c1f033c04c0a0342c) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`canResend`](base.md#classicy_1_1PacketTransaction_1aca2387c6623fb21b6345bf7a1e45612b) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`attempts`](base.md#classicy_1_1PacketTransaction_1a7c19c40febe7e9f6471a7ea5624c933e) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`retries`](base.md#classicy_1_1PacketTransaction_1a81bb9f59219ebd7399dfdbafe821dcb4) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`request`](base.md#classicy_1_1PacketTransaction_1a3a5ebb2afffd6d94e465a1db318161e5) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`request`](base.md#classicy_1_1PacketTransaction_1abe8ba9b6ee2c439944d84386204d5818) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`response`](base.md#classicy_1_1PacketTransaction_1a16cdc5ec77f02ab8213de87bb56f709f) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`response`](base.md#classicy_1_1PacketTransaction_1a50e68834b240b876dae1a7d1f84afb56) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`~PacketTransaction`](base.md#classicy_1_1PacketTransaction_1ac118e461ae9ba6fc8f3b69fe82fda134) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`onStateChange`](base.md#classicy_1_1PacketTransaction_1a14bf74aa47986f672b4f711a2f98e38d) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`handlePotentialResponse`](base.md#classicy_1_1PacketTransaction_1ad392bb447a02bcd8425614589966613a) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`checkResponse`](base.md#classicy_1_1PacketTransaction_1a36b55484206d69dff6375d61b5fb834b) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`onResponse`](base.md#classicy_1_1PacketTransaction_1a2ba8f73029cf89d03c1d641c8c3a42f3) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`onTimeout`](base.md#classicy_1_1PacketTransaction_1ad515edcc4134a741acea3f407409fc56) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`send`](base.md#send-9) | `function` | Inherited from [`Sendable`](base.md#sendable) |
| [`cancel`](base.md#cancel-2) | `function` | Inherited from [`Sendable`](base.md#sendable) |
| [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`_refCount`](base.md#classicy_1_1RefCounted_1a9ad69b4e82cc4cce1d55780afea9e589) | `variable` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`RefCounted`](base.md#classicy_1_1RefCounted_1a69db516c7fc776ec189af3cbd4c9804b) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`RefCounted`](base.md#classicy_1_1RefCounted_1a4c715cce08cc8f065a0740716d828c51) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`operator=`](base.md#classicy_1_1RefCounted_1a40bc82fe701cbdad57046c7b0ebd782b) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`addRef`](base.md#classicy_1_1RefCounted_1a8d439708a2a8535bd0f98d58ba8b8950) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`releaseRef`](base.md#classicy_1_1RefCounted_1a842da5db14ba73d10c95b5be072cbd1f) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`refCount`](base.md#classicy_1_1RefCounted_1a0bd7a4e6db7b4582fcec1e632901ad2f) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`~RefCounted`](base.md#classicy_1_1RefCounted_1a605be798c4be05f218ef0e84f5c5e909) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`factory`](net.md#factory) | `variable` | Inherited from [`PacketSocketEmitter`](net.md#packetsocketemitter) |
| [`PacketSocketEmitter`](net.md#packetsocketemitter-1) | `function` | Inherited from [`PacketSocketEmitter`](net.md#packetsocketemitter) |
| [`onSocketRecv`](net.md#onsocketrecv) | `function` | Inherited from [`PacketSocketEmitter`](net.md#packetsocketemitter) |
| [`onPacket`](net.md#onpacket) | `function` | Inherited from [`PacketSocketEmitter`](net.md#packetsocketemitter) |
| [`Connect`](net.md#connect-4) | `variable` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`Recv`](net.md#recv) | `variable` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`Error`](net.md#error-6) | `variable` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`Close`](net.md#close-13) | `variable` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`impl`](net.md#impl) | `variable` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`SocketEmitter`](net.md#socketemitter-1) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`SocketEmitter`](net.md#socketemitter-2) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`~SocketEmitter`](net.md#socketemitter-3) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`addReceiver`](net.md#addreceiver-1) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`removeReceiver`](net.md#removereceiver-1) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`swap`](net.md#swap-1) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`as`](net.md#as) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`operator->`](net.md#operator-9) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`onSocketConnect`](net.md#onsocketconnect-1) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`onSocketRecv`](net.md#onsocketrecv-2) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`onSocketError`](net.md#onsocketerror-1) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`onSocketClose`](net.md#onsocketclose-1) | `function` | Inherited from [`SocketEmitter`](net.md#socketemitter) |
| [`priority`](net.md#priority-1) | `variable` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`SocketAdapter`](net.md#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`~SocketAdapter`](net.md#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`send`](net.md#send) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`send`](net.md#send-1) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`sendOwned`](net.md#sendowned-2) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`sendOwned`](net.md#sendowned-3) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`sendPacket`](net.md#sendpacket) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`sendPacket`](net.md#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`sendPacket`](net.md#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`setSender`](net.md#setsender) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`sender`](net.md#sender) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`addReceiver`](net.md#addreceiver) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`removeReceiver`](net.md#removereceiver) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`hasReceiver`](net.md#hasreceiver) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`receivers`](net.md#receivers) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`onSocketConnect`](net.md#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`onSocketRecv`](net.md#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`onSocketError`](net.md#onsocketerror) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`onSocketClose`](net.md#onsocketclose) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`_sender`](net.md#_sender) | `variable` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`_receivers`](net.md#_receivers) | `variable` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`_dirty`](net.md#_dirty) | `variable` | Inherited from [`SocketAdapter`](net.md#socketadapter) |
| [`cleanupReceivers`](net.md#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](net.md#socketadapter) |

### Inherited from [`Transaction`](net.md#transaction)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`IntrusivePtr`](net.md#icy-intrusiveptr)  |  |
| `function` | [`Transaction`](net.md#transaction-1) `inline` | Constructs a [Transaction](net.md#transaction) on the given socket targeting `peerAddress`. |
| `function` | [`send`](net.md#send-6) `virtual` `inline` `override` | Sends the request packet to the peer address and starts the timeout timer. Sets state to Failed and returns false if the packet could not be sent. |
| `function` | [`cancel`](net.md#cancel) `virtual` `inline` `override` | Cancels the transaction and stops the timeout timer. |
| `function` | [`dispose`](net.md#dispose) `virtual` `inline` `override` | Stops the timer and unregisters callbacks. |
| `function` | [`peerAddress`](net.md#peeraddress-3) `const` `inline` | Returns the remote peer address used for this transaction. |
| `variable` | [`_peerAddress`](net.md#_peeraddress-1)  |  |
| `function` | [`onPacket`](net.md#onpacket-1) `virtual` `inline` `override` | Checks whether `packet` is a matching response for the pending request. If it matches, the transaction completes; socket data propagation stops. |
| `function` | [`onResponse`](net.md#onresponse) `virtual` `inline` `override` | Called when a confirmed response is received; emits the response via [PacketSignal](base.md#packetsignal). |
| `function` | [`checkResponse`](net.md#checkresponse) `virtual` `inline` `override` | Returns true if `packet` is a valid response for this transaction. |
| `typedef` | [`BaseT`](net.md#baset)  |  |

### Inherited from [`PacketTransaction`](base.md#packettransaction)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`Ptr`](base.md#classicy_1_1PacketTransaction_1a44ea6d9d47efb59eebdc71158c0158c5)  |  |
| `friend` | [`IntrusivePtr`](base.md#classicy_1_1PacketTransaction_1a04a374ecab2499e81653198fd49ff15e)  |  |
| `variable` | [`_request`](base.md#classicy_1_1PacketTransaction_1a03e21f66e0f957926f03f37b48276dca)  |  |
| `variable` | [`_response`](base.md#classicy_1_1PacketTransaction_1a6322ae70f87db74c1d5c130b7fec7eb8)  |  |
| `variable` | [`_timer`](base.md#classicy_1_1PacketTransaction_1a1b02dbf0ac65fcad82c50a6867f81a79)  | The request timeout callback. |
| `variable` | [`_retries`](base.md#classicy_1_1PacketTransaction_1a8c76a907486cae828c77f0302d20a844)  | The maximum number of attempts before the transaction is considered failed. |
| `variable` | [`_attempts`](base.md#classicy_1_1PacketTransaction_1ace42ededd6279b9a3ce13bdfd6619c11)  | The number of times the transaction has been sent. |
| `variable` | [`_disposed`](base.md#classicy_1_1PacketTransaction_1a70d8cc5501fba3b9c26426af411241a4)  |  |
| `function` | [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a5b3c9258f951866005347123bdf7a7ff) `inline` |  |
| `function` | [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a711873135d73ab6cb3426db65e980196) `inline` |  |
| `function` | [`send`](base.md#classicy_1_1PacketTransaction_1acf7a9bbcfff95999c0c227cb36f9cfd4) `virtual` `inline` `override` | Starts the transaction timer and sends the request. Overriding classes should implement send logic here. |
| `function` | [`cancel`](base.md#classicy_1_1PacketTransaction_1ab7c1e492203b850fc9b5c1aa4f97c3ef) `virtual` `inline` `override` | Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response. Transitions the transaction to the `Cancelled` state. |
| `function` | [`cancelled`](base.md#classicy_1_1PacketTransaction_1a24663172b10c70530b29c9dccdf5813d) `const` `inline` |  |
| `function` | [`dispose`](base.md#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) `virtual` `inline` | Stops the timer and unregisters callbacks. Does NOT delete the object; the [IntrusivePtr](base.md#intrusiveptr) destructor handles deletion when the last reference is released. Safe to call multiple times. |
| `function` | [`disposed`](base.md#classicy_1_1PacketTransaction_1a2a85d3be5eb4088c1f033c04c0a0342c) `const` `inline` `nodiscard` |  |
| `function` | [`canResend`](base.md#classicy_1_1PacketTransaction_1aca2387c6623fb21b6345bf7a1e45612b) `virtual` `inline` |  |
| `function` | [`attempts`](base.md#classicy_1_1PacketTransaction_1a7c19c40febe7e9f6471a7ea5624c933e) `const` `inline` |  |
| `function` | [`retries`](base.md#classicy_1_1PacketTransaction_1a81bb9f59219ebd7399dfdbafe821dcb4) `const` `inline` |  |
| `function` | [`request`](base.md#classicy_1_1PacketTransaction_1a3a5ebb2afffd6d94e465a1db318161e5) `inline` |  |
| `function` | [`request`](base.md#classicy_1_1PacketTransaction_1abe8ba9b6ee2c439944d84386204d5818) `const` `inline` |  |
| `function` | [`response`](base.md#classicy_1_1PacketTransaction_1a16cdc5ec77f02ab8213de87bb56f709f) `inline` |  |
| `function` | [`response`](base.md#classicy_1_1PacketTransaction_1a50e68834b240b876dae1a7d1f84afb56) `const` `inline` |  |
| `function` | [`~PacketTransaction`](base.md#classicy_1_1PacketTransaction_1ac118e461ae9ba6fc8f3b69fe82fda134) `virtual` `inline` |  |
| `function` | [`onStateChange`](base.md#classicy_1_1PacketTransaction_1a14bf74aa47986f672b4f711a2f98e38d) `virtual` `inline` `override` | Post state change hook. Calls [dispose()](base.md#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) on terminal states to stop the timer, but does not delete the object; [IntrusivePtr](base.md#intrusiveptr) handles that. |
| `function` | [`handlePotentialResponse`](base.md#classicy_1_1PacketTransaction_1ad392bb447a02bcd8425614589966613a) `virtual` `inline` | Processes a potential response candidate and updates the state accordingly. |
| `function` | [`checkResponse`](base.md#classicy_1_1PacketTransaction_1a36b55484206d69dff6375d61b5fb834b) `virtual` | Checks a potential response candidate and returns true on successful match. |
| `function` | [`onResponse`](base.md#classicy_1_1PacketTransaction_1a2ba8f73029cf89d03c1d641c8c3a42f3) `virtual` `inline` | Called when a successful response is received. |
| `function` | [`onTimeout`](base.md#classicy_1_1PacketTransaction_1ad515edcc4134a741acea3f407409fc56) `virtual` `inline` | Called by the timer when the transaction timeout elapses. Retransmits if retries remain, otherwise transitions to `Failed`. |

### Inherited from [`Sendable`](base.md#sendable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`send`](base.md#send-9) `virtual` | Initiates the send operation. |
| `function` | [`cancel`](base.md#cancel-2) `virtual` | Cancels a pending send operation. |

### Inherited from [`Stateful`](base.md#stateful)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0)  | Signals when the state changes. |
| `variable` | [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d)  |  |
| `function` | [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) `inline` |  |
| `function` | [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) `virtual` `inline` |  |
| `function` | [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) `virtual` `const` `inline` | Returns true if the current state ID equals the given ID. |
| `function` | [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) `virtual` `const` `inline` | Returns true if the current state ID is in the inclusive range [lid, rid]. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) `virtual` `inline` | Returns a mutable reference to the current state. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) `virtual` `const` `inline` | Returns a copy of the current state. |
| `function` | [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) `virtual` `inline` | Override to handle pre state change logic. Return false to prevent state change. |
| `function` | [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) `virtual` `inline` | Override to handle post state change logic. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |

### Inherited from [`RefCounted`](base.md#refcounted)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_refCount`](base.md#classicy_1_1RefCounted_1a9ad69b4e82cc4cce1d55780afea9e589)  |  |
| `function` | [`RefCounted`](base.md#classicy_1_1RefCounted_1a69db516c7fc776ec189af3cbd4c9804b)  | Defaulted constructor. |
| `function` | [`RefCounted`](base.md#classicy_1_1RefCounted_1a4c715cce08cc8f065a0740716d828c51) `inline` `noexcept` |  |
| `function` | [`operator=`](base.md#classicy_1_1RefCounted_1a40bc82fe701cbdad57046c7b0ebd782b) `inline` `noexcept` |  |
| `function` | [`addRef`](base.md#classicy_1_1RefCounted_1a8d439708a2a8535bd0f98d58ba8b8950) `const` `inline` `noexcept` | Increments the reference count. Called by [IntrusivePtr](base.md#intrusiveptr) on acquisition. |
| `function` | [`releaseRef`](base.md#classicy_1_1RefCounted_1a842da5db14ba73d10c95b5be072cbd1f) `const` `inline` `noexcept` | Decrements the reference count. |
| `function` | [`refCount`](base.md#classicy_1_1RefCounted_1a0bd7a4e6db7b4582fcec1e632901ad2f) `const` `inline` `nodiscard` `noexcept` | Returns the current reference count. |
| `function` | [`~RefCounted`](base.md#classicy_1_1RefCounted_1a605be798c4be05f218ef0e84f5c5e909)  | Defaulted destructor. |

### Inherited from [`PacketSocketEmitter`](net.md#packetsocketemitter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`factory`](net.md#factory)  | The packet factory. |
| `function` | [`PacketSocketEmitter`](net.md#packetsocketemitter-1)  | Creates the [PacketSocketEmitter](net.md#packetsocketemitter) and attaches it to the given socket. |
| `function` | [`onSocketRecv`](net.md#onsocketrecv) `virtual` `override` | Parses raw received data into packets via the factory and forwards each parsed packet to [onPacket()](net.md#onpacket). Returns true if propagation should stop. |
| `function` | [`onPacket`](net.md#onpacket) `virtual` | [Process](base.md#process) a parsed packet. Returns true to stop propagation. |

### Inherited from [`SocketEmitter`](net.md#socketemitter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`Connect`](net.md#connect-4)  | Signals that the socket is connected. |
| `variable` | [`Recv`](net.md#recv)  | Signals when data is received by the socket. |
| `variable` | [`Error`](net.md#error-6)  | Signals that the socket is closed in error. This signal will be sent just before the Closed signal. |
| `variable` | [`Close`](net.md#close-13)  | Signals that the underlying socket is closed. |
| `variable` | [`impl`](net.md#impl)  | Pointer to the underlying socket. Sent data will be proxied to this socket. |
| `function` | [`SocketEmitter`](net.md#socketemitter-1)  | Creates the [SocketEmitter](net.md#socketemitter) and optionally attaches it to a socket. If `socket` is provided, this emitter registers itself as a receiver. |
| `function` | [`SocketEmitter`](net.md#socketemitter-2)  | Copy constructor; copies all signal connections and attaches to the same socket. |
| `function` | [`~SocketEmitter`](net.md#socketemitter-3) `virtual` `noexcept` | Destroys the [SocketAdapter](net.md#socketadapter). |
| `function` | [`addReceiver`](net.md#addreceiver-1) `virtual` `override` | Attaches a [SocketAdapter](net.md#socketadapter) as a receiver; wires it to all four socket signals. |
| `function` | [`removeReceiver`](net.md#removereceiver-1) `virtual` `override` | Detaches a [SocketAdapter](net.md#socketadapter) from all four socket signals. |
| `function` | [`swap`](net.md#swap-1) `virtual` | Replaces the underlying socket with `socket`. |
| `function` | [`as`](net.md#as) `inline` | Returns the underlying socket cast to type T, or nullptr if the cast fails. |
| `function` | [`operator->`](net.md#operator-9) `const` `inline` | Returns a raw pointer to the underlying socket for direct method access. Follows shared_ptr semantics; the caller must not delete the returned pointer. |
| `function` | [`onSocketConnect`](net.md#onsocketconnect-1) `virtual` `override` | Forwards the connect event to chained adapters, then fires the Connect signal. |
| `function` | [`onSocketRecv`](net.md#onsocketrecv-2) `virtual` `override` | Forwards the recv event to chained adapters, then fires the Recv signal. |
| `function` | [`onSocketError`](net.md#onsocketerror-1) `virtual` `override` | Forwards the error event to chained adapters, then fires the [Error](base.md#error) signal. |
| `function` | [`onSocketClose`](net.md#onsocketclose-1) `virtual` `override` | Forwards the close event to chained adapters, then fires the Close signal. |

### Inherited from [`SocketAdapter`](net.md#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](net.md#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](net.md#socketadapter-1)  | Creates the [SocketAdapter](net.md#socketadapter). |
| `function` | [`~SocketAdapter`](net.md#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](net.md#socketadapter). |
| `function` | [`send`](net.md#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](net.md#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](net.md#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](net.md#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](net.md#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](net.md#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](net.md#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](net.md#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](net.md#sender)  | Returns the output [SocketAdapter](net.md#socketadapter) pointer. |
| `function` | [`addReceiver`](net.md#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](net.md#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](net.md#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](net.md#receivers)  | Returns all currently registered input [SocketAdapter](net.md#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](net.md#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](net.md#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](net.md#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](net.md#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](net.md#_sender)  |  |
| `variable` | [`_receivers`](net.md#_receivers)  |  |
| `variable` | [`_dirty`](net.md#_dirty)  |  |
| `function` | [`cleanupReceivers`](net.md#cleanupreceivers) `virtual` |  |

### Friends

| Name | Description |
|------|-------------|
| [`icy::IntrusivePtr`](#icy-intrusiveptr-1)  |  |

---

{#icy-intrusiveptr-1}

#### icy::IntrusivePtr

```cpp
template<typename U> friend class icy::IntrusivePtr
```

Defined in src/stun/include/icy/stun/transaction.h:57

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Transaction`](#transaction-3)  | Constructs a STUN transaction bound to a specific socket and peer. |
| `bool` | [`checkResponse`](#checkresponse-1) `override` | Checks that `message` is a valid response for the pending request. In addition to the base class check, verifies that the transaction IDs match. |
| `void` | [`onResponse`](#onresponse-1) `virtual` `override` | Called when a valid response is received. Infers the response class (SuccessResponse, ErrorResponse, or Indication) from the response attributes and delegates to the base class handler. |

---

{#transaction-3}

#### Transaction

```cpp
Transaction(const net::Socket::Ptr & socket, const net::Address & peerAddress, long timeout = 10000, int retries = 1)
```

Defined in src/stun/include/icy/stun/transaction.h:41

Constructs a STUN transaction bound to a specific socket and peer. 
#### Parameters
* `socket` Socket used to send the request and receive the response. 

* `peerAddress` Remote address of the STUN/TURN server. 

* `timeout` Response timeout in milliseconds (default 10 s). 

* `retries` Number of send retries before declaring failure (default 1).

---

{#checkresponse-1}

#### checkResponse

`override`

```cpp
bool checkResponse(const Message & message) override
```

Defined in src/stun/include/icy/stun/transaction.h:48

Checks that `message` is a valid response for the pending request. In addition to the base class check, verifies that the transaction IDs match. 
#### Parameters
* `message` Incoming STUN message to evaluate. 

#### Returns
true if `message` is the expected response.

---

{#onresponse-1}

#### onResponse

`virtual` `override`

```cpp
virtual void onResponse() override
```

Defined in src/stun/include/icy/stun/transaction.h:53

Called when a valid response is received. Infers the response class (SuccessResponse, ErrorResponse, or Indication) from the response attributes and delegates to the base class handler.

##### Reimplements

- [`onResponse`](net.md#onresponse)

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-16)  |  |

---

{#ptr-16}

#### Ptr

```cpp
using Ptr = IntrusivePtr< Transaction >
```

Defined in src/stun/include/icy/stun/transaction.h:34

{#uint16listattribute}

## UInt16ListAttribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class UInt16ListAttribute
```

Defined in src/stun/include/icy/stun/attributes.h:425

> **Inherits:** [`Attribute`](#attribute)

Implements a STUN/TURN attribute that holds a list of attribute type codes. Used by the UNKNOWN-ATTRIBUTES attribute (RFC 5389 section 15.9).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UInt16ListAttribute`](#uint16listattribute-1) | `function` | Declared here |
| [`UInt16ListAttribute`](#uint16listattribute-2) | `function` | Declared here |
| [`clone`](#clone-14) | `function` | Declared here |
| [`size`](#size-8) | `function` | Declared here |
| [`getType`](#gettype) | `function` | Declared here |
| [`setType`](#settype) | `function` | Declared here |
| [`addType`](#addtype) | `function` | Declared here |
| [`read`](#read-9) | `function` | Declared here |
| [`write`](#write-24) | `function` | Declared here |
| [`_attrTypes`](#_attrtypes) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UInt16ListAttribute`](#uint16listattribute-1)  |  |
|  | [`UInt16ListAttribute`](#uint16listattribute-2)  | Copy constructor; duplicates the type list. |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-14) `virtual` `override` | Returns a deep copy of this attribute. |
| `size_t` | [`size`](#size-8) `const` `nodiscard` |  |
| `uint16_t` | [`getType`](#gettype) `const` `nodiscard` | Returns the type code at the given list position. |
| `void` | [`setType`](#settype)  | Overwrites the type code at the given list position. |
| `void` | [`addType`](#addtype)  | Appends a type code to the list and updates the attribute size. |
| `void` | [`read`](#read-9) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-24) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |

---

{#uint16listattribute-1}

#### UInt16ListAttribute

```cpp
UInt16ListAttribute(uint16_t type, uint16_t size)
```

Defined in src/stun/include/icy/stun/attributes.h:430

#### Parameters
* `type` Wire type code for the concrete attribute. 

* `size` Initial body length in bytes.

---

{#uint16listattribute-2}

#### UInt16ListAttribute

```cpp
UInt16ListAttribute(const UInt16ListAttribute & r)
```

Defined in src/stun/include/icy/stun/attributes.h:433

Copy constructor; duplicates the type list.

---

{#clone-14}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:437

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#size-8}

#### size

`const` `nodiscard`

```cpp
[[nodiscard]] size_t size() const
```

Defined in src/stun/include/icy/stun/attributes.h:440

#### Returns
Number of attribute type codes in the list.

---

{#gettype}

#### getType

`const` `nodiscard`

```cpp
[[nodiscard]] uint16_t getType(int index) const
```

Defined in src/stun/include/icy/stun/attributes.h:445

Returns the type code at the given list position. 
#### Parameters
* `index` Zero-based list index. 

#### Returns
[Attribute](#attribute) type code at `index`.

---

{#settype}

#### setType

```cpp
void setType(int index, uint16_t value)
```

Defined in src/stun/include/icy/stun/attributes.h:450

Overwrites the type code at the given list position. 
#### Parameters
* `index` Zero-based list index. 

* `value` New attribute type code.

---

{#addtype}

#### addType

```cpp
void addType(uint16_t value)
```

Defined in src/stun/include/icy/stun/attributes.h:454

Appends a type code to the list and updates the attribute size. 
#### Parameters
* `value` [Attribute](#attribute) type code to append.

---

{#read-9}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:456

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-24}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:457

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::vector< uint16_t >` | [`_attrTypes`](#_attrtypes)  |  |

---

{#_attrtypes}

#### _attrTypes

```cpp
std::vector< uint16_t > _attrTypes
```

Defined in src/stun/include/icy/stun/attributes.h:460

{#uint32attribute}

## UInt32Attribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class UInt32Attribute
```

Defined in src/stun/include/icy/stun/attributes.h:269

> **Inherits:** [`Attribute`](#attribute)

Implements a STUN/TURN attribute that holds a 32-bit integer.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UInt32Attribute`](#uint32attribute-1) | `function` | Declared here |
| [`UInt32Attribute`](#uint32attribute-2) | `function` | Declared here |
| [`clone`](#clone-15) | `function` | Declared here |
| [`value`](#value-1) | `function` | Declared here |
| [`setValue`](#setvalue-1) | `function` | Declared here |
| [`getBit`](#getbit) | `function` | Declared here |
| [`setBit`](#setbit) | `function` | Declared here |
| [`read`](#read-10) | `function` | Declared here |
| [`write`](#write-25) | `function` | Declared here |
| [`Size`](#size-9) | `variable` | Declared here |
| [`_bits`](#_bits) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UInt32Attribute`](#uint32attribute-1)  |  |
|  | [`UInt32Attribute`](#uint32attribute-2)  | Copy constructor. |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-15) `virtual` `override` | Returns a deep copy of this attribute. |
| `uint32_t` | [`value`](#value-1) `const` `inline` `nodiscard` |  |
| `void` | [`setValue`](#setvalue-1) `inline` | Sets the stored 32-bit value. |
| `bool` | [`getBit`](#getbit) `const` `nodiscard` | Returns the state of a single bit within the stored word. |
| `void` | [`setBit`](#setbit)  | Sets or clears a single bit within the stored word. |
| `void` | [`read`](#read-10) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-25) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |

---

{#uint32attribute-1}

#### UInt32Attribute

```cpp
UInt32Attribute(uint16_t type)
```

Defined in src/stun/include/icy/stun/attributes.h:273

#### Parameters
* `type` Wire type code for the concrete attribute.

---

{#uint32attribute-2}

#### UInt32Attribute

```cpp
UInt32Attribute(const UInt32Attribute & r)
```

Defined in src/stun/include/icy/stun/attributes.h:276

Copy constructor.

---

{#clone-15}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:279

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#value-1}

#### value

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint32_t value() const
```

Defined in src/stun/include/icy/stun/attributes.h:284

#### Returns
The stored 32-bit value.

---

{#setvalue-1}

#### setValue

`inline`

```cpp
inline void setValue(uint32_t bits)
```

Defined in src/stun/include/icy/stun/attributes.h:288

Sets the stored 32-bit value. 
#### Parameters
* `bits` Value to store.

---

{#getbit}

#### getBit

`const` `nodiscard`

```cpp
[[nodiscard]] bool getBit(int index) const
```

Defined in src/stun/include/icy/stun/attributes.h:293

Returns the state of a single bit within the stored word. 
#### Parameters
* `index` Bit position (0 = LSB, 31 = MSB). 

#### Returns
true if the bit is set.

---

{#setbit}

#### setBit

```cpp
void setBit(int index, bool value)
```

Defined in src/stun/include/icy/stun/attributes.h:298

Sets or clears a single bit within the stored word. 
#### Parameters
* `index` Bit position (0 = LSB, 31 = MSB). 

* `value` true to set, false to clear.

---

{#read-10}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:300

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-25}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:301

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`Size`](#size-9) `static` `constexpr` |  |

---

{#size-9}

#### Size

`static` `constexpr`

```cpp
uint16_t Size = 4
```

Defined in src/stun/include/icy/stun/attributes.h:281

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint32_t` | [`_bits`](#_bits)  |  |

---

{#_bits}

#### _bits

```cpp
uint32_t _bits
```

Defined in src/stun/include/icy/stun/attributes.h:304

{#uint64attribute}

## UInt64Attribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class UInt64Attribute
```

Defined in src/stun/include/icy/stun/attributes.h:309

> **Inherits:** [`Attribute`](#attribute)

Implements a STUN/TURN attribute that holds a 64-bit integer.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UInt64Attribute`](#uint64attribute-1) | `function` | Declared here |
| [`UInt64Attribute`](#uint64attribute-2) | `function` | Declared here |
| [`clone`](#clone-16) | `function` | Declared here |
| [`value`](#value-2) | `function` | Declared here |
| [`setValue`](#setvalue-2) | `function` | Declared here |
| [`getBit`](#getbit-1) | `function` | Declared here |
| [`setBit`](#setbit-1) | `function` | Declared here |
| [`read`](#read-11) | `function` | Declared here |
| [`write`](#write-26) | `function` | Declared here |
| [`Size`](#size-10) | `variable` | Declared here |
| [`_bits`](#_bits-1) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UInt64Attribute`](#uint64attribute-1)  |  |
|  | [`UInt64Attribute`](#uint64attribute-2)  | Copy constructor. |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-16) `virtual` `override` | Returns a deep copy of this attribute. |
| `uint64_t` | [`value`](#value-2) `const` `inline` `nodiscard` |  |
| `void` | [`setValue`](#setvalue-2) `inline` | Sets the stored 64-bit value. |
| `bool` | [`getBit`](#getbit-1) `const` `nodiscard` | Returns the state of a single bit within the stored quad-word. |
| `void` | [`setBit`](#setbit-1)  | Sets or clears a single bit within the stored quad-word. |
| `void` | [`read`](#read-11) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-26) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |

---

{#uint64attribute-1}

#### UInt64Attribute

```cpp
UInt64Attribute(uint16_t type)
```

Defined in src/stun/include/icy/stun/attributes.h:313

#### Parameters
* `type` Wire type code for the concrete attribute.

---

{#uint64attribute-2}

#### UInt64Attribute

```cpp
UInt64Attribute(const UInt64Attribute & r)
```

Defined in src/stun/include/icy/stun/attributes.h:316

Copy constructor.

---

{#clone-16}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:319

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#value-2}

#### value

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint64_t value() const
```

Defined in src/stun/include/icy/stun/attributes.h:324

#### Returns
The stored 64-bit value.

---

{#setvalue-2}

#### setValue

`inline`

```cpp
inline void setValue(uint64_t bits)
```

Defined in src/stun/include/icy/stun/attributes.h:328

Sets the stored 64-bit value. 
#### Parameters
* `bits` Value to store.

---

{#getbit-1}

#### getBit

`const` `nodiscard`

```cpp
[[nodiscard]] bool getBit(int index) const
```

Defined in src/stun/include/icy/stun/attributes.h:333

Returns the state of a single bit within the stored quad-word. 
#### Parameters
* `index` Bit position (0 = LSB, 63 = MSB). 

#### Returns
true if the bit is set.

---

{#setbit-1}

#### setBit

```cpp
void setBit(int index, bool value)
```

Defined in src/stun/include/icy/stun/attributes.h:338

Sets or clears a single bit within the stored quad-word. 
#### Parameters
* `index` Bit position (0 = LSB, 63 = MSB). 

* `value` true to set, false to clear.

---

{#read-11}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:340

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-26}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:341

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`Size`](#size-10) `static` `constexpr` |  |

---

{#size-10}

#### Size

`static` `constexpr`

```cpp
uint16_t Size = 8
```

Defined in src/stun/include/icy/stun/attributes.h:321

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint64_t` | [`_bits`](#_bits-1)  |  |

---

{#_bits-1}

#### _bits

```cpp
uint64_t _bits
```

Defined in src/stun/include/icy/stun/attributes.h:344

{#uint8attribute}

## UInt8Attribute

```cpp
#include <icy/stun/attributes.h>
```

```cpp
class UInt8Attribute
```

Defined in src/stun/include/icy/stun/attributes.h:229

> **Inherits:** [`Attribute`](#attribute)

Implements a STUN/TURN attribute that holds an 8-bit integer.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UInt8Attribute`](#uint8attribute-1) | `function` | Declared here |
| [`UInt8Attribute`](#uint8attribute-2) | `function` | Declared here |
| [`clone`](#clone-17) | `function` | Declared here |
| [`value`](#value-3) | `function` | Declared here |
| [`setValue`](#setvalue-3) | `function` | Declared here |
| [`getBit`](#getbit-2) | `function` | Declared here |
| [`setBit`](#setbit-2) | `function` | Declared here |
| [`read`](#read-12) | `function` | Declared here |
| [`write`](#write-27) | `function` | Declared here |
| [`Size`](#size-11) | `variable` | Declared here |
| [`_bits`](#_bits-2) | `variable` | Declared here |
| [`clone`](#clone-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`read`](#read-3) | `function` | Inherited from [`Attribute`](#attribute) |
| [`write`](#write-18) | `function` | Inherited from [`Attribute`](#attribute) |
| [`type`](#type-14) | `function` | Inherited from [`Attribute`](#attribute) |
| [`size`](#size-4) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes) | `function` | Inherited from [`Attribute`](#attribute) |
| [`consumePadding`](#consumepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`writePadding`](#writepadding) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring) | `function` | Inherited from [`Attribute`](#attribute) |
| [`TypeID`](#typeid) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`create`](#create-8) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddingBytes`](#paddingbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`paddedBytes`](#paddedbytes-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`typeString`](#typestring-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`_type`](#_type-1) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`_size`](#_size) | `variable` | Inherited from [`Attribute`](#attribute) |
| [`Attribute`](#attribute-1) | `function` | Inherited from [`Attribute`](#attribute) |
| [`setLength`](#setlength) | `function` | Inherited from [`Attribute`](#attribute) |
| [`Type`](#type-15) | `enum` | Inherited from [`Attribute`](#attribute) |

### Inherited from [`Attribute`](#attribute)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`clone`](#clone-8) `virtual` | Returns a deep copy of this attribute. |
| `function` | [`read`](#read-3) `virtual` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `function` | [`write`](#write-18) `virtual` `const` | Writes the body (not the type or size) to the given buffer. |
| `function` | [`type`](#type-14) `const` `nodiscard` |  |
| `function` | [`size`](#size-4) `const` `nodiscard` |  |
| `function` | [`paddingBytes`](#paddingbytes) `const` `inline` `nodiscard` |  |
| `function` | [`paddedBytes`](#paddedbytes) `const` `inline` `nodiscard` |  |
| `function` | [`consumePadding`](#consumepadding) `const` | Advances the reader past any 4-byte alignment padding that follows this attribute's body. |
| `function` | [`writePadding`](#writepadding) `const` | Writes zero-fill padding bytes to align this attribute to a 4-byte boundary. |
| `function` | [`typeString`](#typestring) `nodiscard` |  |
| `variable` | [`TypeID`](#typeid) `static` `constexpr` |  |
| `function` | [`create`](#create-8) `static` `nodiscard` | Creates an attribute of the given wire type and body size. Returns nullptr if the type is unknown or the size is invalid. |
| `function` | [`paddingBytes`](#paddingbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the 4-byte alignment padding required for a body of `size` bytes. |
| `function` | [`paddedBytes`](#paddedbytes-1) `static` `inline` `nodiscard` `constexpr` | Returns the body length including 4-byte alignment padding. |
| `function` | [`typeString`](#typestring-1) `static` `nodiscard` |  |
| `variable` | [`_type`](#_type-1)  |  |
| `variable` | [`_size`](#_size)  |  |
| `function` | [`Attribute`](#attribute-1)  |  |
| `function` | [`setLength`](#setlength)  | Updates the stored body length. |
| `enum` | [`Type`](#type-15)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UInt8Attribute`](#uint8attribute-1)  |  |
|  | [`UInt8Attribute`](#uint8attribute-2)  | Copy constructor. |
| `std::unique_ptr< Attribute >` | [`clone`](#clone-17) `virtual` `override` | Returns a deep copy of this attribute. |
| `uint8_t` | [`value`](#value-3) `const` `inline` `nodiscard` |  |
| `void` | [`setValue`](#setvalue-3) `inline` | Sets the stored 8-bit value. |
| `bool` | [`getBit`](#getbit-2) `const` `nodiscard` | Returns the state of a single bit within the stored byte. |
| `void` | [`setBit`](#setbit-2)  | Sets or clears a single bit within the stored byte. |
| `void` | [`read`](#read-12) `virtual` `override` | Reads the body (not the type or size) for this type of attribute from the given buffer. |
| `void` | [`write`](#write-27) `virtual` `const` `override` | Writes the body (not the type or size) to the given buffer. |

---

{#uint8attribute-1}

#### UInt8Attribute

```cpp
UInt8Attribute(uint16_t type)
```

Defined in src/stun/include/icy/stun/attributes.h:233

#### Parameters
* `type` Wire type code for the concrete attribute.

---

{#uint8attribute-2}

#### UInt8Attribute

```cpp
UInt8Attribute(const UInt8Attribute & r)
```

Defined in src/stun/include/icy/stun/attributes.h:236

Copy constructor.

---

{#clone-17}

#### clone

`virtual` `override`

```cpp
virtual std::unique_ptr< Attribute > clone() override
```

Defined in src/stun/include/icy/stun/attributes.h:239

Returns a deep copy of this attribute.

##### Reimplements

- [`clone`](#clone-8)

---

{#value-3}

#### value

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint8_t value() const
```

Defined in src/stun/include/icy/stun/attributes.h:244

#### Returns
The stored 8-bit value.

---

{#setvalue-3}

#### setValue

`inline`

```cpp
inline void setValue(uint8_t bits)
```

Defined in src/stun/include/icy/stun/attributes.h:248

Sets the stored 8-bit value. 
#### Parameters
* `bits` Value to store.

---

{#getbit-2}

#### getBit

`const` `nodiscard`

```cpp
[[nodiscard]] bool getBit(int index) const
```

Defined in src/stun/include/icy/stun/attributes.h:253

Returns the state of a single bit within the stored byte. 
#### Parameters
* `index` Bit position (0 = LSB, 7 = MSB). 

#### Returns
true if the bit is set.

---

{#setbit-2}

#### setBit

```cpp
void setBit(int index, bool value)
```

Defined in src/stun/include/icy/stun/attributes.h:258

Sets or clears a single bit within the stored byte. 
#### Parameters
* `index` Bit position (0 = LSB, 7 = MSB). 

* `value` true to set, false to clear.

---

{#read-12}

#### read

`virtual` `override`

```cpp
virtual void read(BitReader & reader) override
```

Defined in src/stun/include/icy/stun/attributes.h:260

Reads the body (not the type or size) for this type of attribute from the given buffer. 
#### Parameters
* `reader` Source bit reader positioned at the attribute body.

##### Reimplements

- [`read`](#read-3)

---

{#write-27}

#### write

`virtual` `const` `override`

```cpp
virtual void write(BitWriter & writer) const override
```

Defined in src/stun/include/icy/stun/attributes.h:261

Writes the body (not the type or size) to the given buffer. 
#### Parameters
* `writer` Destination bit writer.

##### Reimplements

- [`write`](#write-18)

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`Size`](#size-11) `static` `constexpr` |  |

---

{#size-11}

#### Size

`static` `constexpr`

```cpp
uint16_t Size = 1
```

Defined in src/stun/include/icy/stun/attributes.h:241

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint8_t` | [`_bits`](#_bits-2)  |  |

---

{#_bits-2}

#### _bits

```cpp
uint8_t _bits
```

Defined in src/stun/include/icy/stun/attributes.h:264

