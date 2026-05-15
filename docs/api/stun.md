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

---

{#read-5}

#### read

`virtual` `inline` `override`

```cpp
virtual inline void read(BitReader &) override
```

Defined in src/stun/include/icy/stun/attributes.h:360

No-op: flag attributes carry no body bytes.

---

{#write-20}

#### write

`virtual` `const` `inline` `override`

```cpp
virtual inline void write(BitWriter &) const override
```

Defined in src/stun/include/icy/stun/attributes.h:363

No-op: flag attributes carry no body bytes.

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

---

{#classname-7}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/stun/include/icy/stun/message.h:201

Returns the class name of this packet type for logging and diagnostics.

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

