# Module <!-- group --> `stun`

The `stun` module contains a STUN (rfc5389) implementation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::stun`](#namespacescy_1_1stun)    | 
# namespace `scy::stun` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::stun::AddressAttribute`](#classscy_1_1stun_1_1AddressAttribute)    | Implements a STUN/TURN attribute that contains a socket address.
`class `[`scy::stun::Attribute`](#classscy_1_1stun_1_1Attribute)    | The virtual base class for all STUN/TURN attributes.
`class `[`scy::stun::ErrorCode`](#classscy_1_1stun_1_1ErrorCode)    | Implements STUN/TURN attribute that reflects an error code.
`class `[`scy::stun::FlagAttribute`](#classscy_1_1stun_1_1FlagAttribute)    | Implements STUN/TURN attribute representing a 0 size flag.
`class `[`scy::stun::Message`](#classscy_1_1stun_1_1Message)    | 
`class `[`scy::stun::MessageIntegrity`](#classscy_1_1stun_1_1MessageIntegrity)    | Implements STUN/TURN attributes that reflects an internet address.
`class `[`scy::stun::StringAttribute`](#classscy_1_1stun_1_1StringAttribute)    | Implements STUN/TURN attribute that reflects an arbitrary byte string.
`class `[`scy::stun::Transaction`](#classscy_1_1stun_1_1Transaction)    | 
`class `[`scy::stun::UInt16ListAttribute`](#classscy_1_1stun_1_1UInt16ListAttribute)    | Implements STUN/TURN attribute that reflects a list of attribute names.
`class `[`scy::stun::UInt32Attribute`](#classscy_1_1stun_1_1UInt32Attribute)    | Implements STUN/TURN attribute that reflects a 32-bit integer.
`class `[`scy::stun::UInt64Attribute`](#classscy_1_1stun_1_1UInt64Attribute)    | Implements STUN/TURN attribute that reflects a 64-bit integer.
`class `[`scy::stun::UInt8Attribute`](#classscy_1_1stun_1_1UInt8Attribute)    | Implements STUN/TURN attribute that reflects a 32-bit integer.
# class `scy::stun::AddressAttribute` 

```
class scy::stun::AddressAttribute
  : public scy::stun::Attribute
```  

Implements a STUN/TURN attribute that contains a socket address.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  AddressAttribute(uint16_t type,bool ipv4)` | 
`public  AddressAttribute(const `[`AddressAttribute`](#classscy_1_1stun_1_1AddressAttribute)` & r)` | 
`public virtual `[`stun::Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline `[`stun::AddressFamily`](#group__stun_1gaec260a57e19eb607c18301f58f80e7e1)` family() const` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 
`public inline virtual void setAddress(const `[`net::Address`](#classscy_1_1net_1_1Address)` & addr)` | 

## Members

#### `public  AddressAttribute(uint16_t type,bool ipv4)` 





#### `public  AddressAttribute(const `[`AddressAttribute`](#classscy_1_1stun_1_1AddressAttribute)` & r)` 





#### `public virtual `[`stun::Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public inline `[`stun::AddressFamily`](#group__stun_1gaec260a57e19eb607c18301f58f80e7e1)` family() const` 





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

#### `public inline virtual void setAddress(const `[`net::Address`](#classscy_1_1net_1_1Address)` & addr)` 





# class `scy::stun::Attribute` 


The virtual base class for all STUN/TURN attributes.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~Attribute()` | 
`public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 
`public uint16_t type() const` | 
`public uint16_t size() const` | 
`public void consumePadding(`[`BitReader`](#classscy_1_1BitReader)` & reader) const` | 
`public void writePadding(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 
`public std::string typeString()` | 
`protected uint16_t _type` | 
`protected uint16_t _size` | 
`protected  Attribute(uint16_t type,uint16_t size)` | 
`protected void setLength(uint16_t size)` | 

## Members

#### `public inline virtual  ~Attribute()` 





#### `public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

#### `public uint16_t type() const` 





#### `public uint16_t size() const` 





#### `public void consumePadding(`[`BitReader`](#classscy_1_1BitReader)` & reader) const` 





#### `public void writePadding(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 





#### `public std::string typeString()` 





#### `protected uint16_t _type` 





#### `protected uint16_t _size` 





#### `protected  Attribute(uint16_t type,uint16_t size)` 





#### `protected void setLength(uint16_t size)` 





# class `scy::stun::ErrorCode` 

```
class scy::stun::ErrorCode
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects an error code.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ErrorCode(uint16_t size)` | 
`public  ErrorCode(const `[`ErrorCode`](#classscy_1_1stun_1_1ErrorCode)` & r)` | 
`public virtual  ~ErrorCode()` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public void setErrorCode(int code)` | 
`public void setReason(const std::string & reason)` | 
`public int errorCode() const` | 
`public inline uint8_t errorClass() const` | 
`public inline uint8_t errorNumber() const` | 
`public inline const std::string & reason() const` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  ErrorCode(uint16_t size)` 





#### `public  ErrorCode(const `[`ErrorCode`](#classscy_1_1stun_1_1ErrorCode)` & r)` 





#### `public virtual  ~ErrorCode()` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public void setErrorCode(int code)` 





#### `public void setReason(const std::string & reason)` 





#### `public int errorCode() const` 





#### `public inline uint8_t errorClass() const` 





#### `public inline uint8_t errorNumber() const` 





#### `public inline const std::string & reason() const` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::FlagAttribute` 

```
class scy::stun::FlagAttribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute representing a 0 size flag.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FlagAttribute(uint16_t type)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public inline virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  FlagAttribute(uint16_t type)` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public inline virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public inline virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::Message` 

```
class scy::stun::Message
  : public scy::IPacket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Message()` | 
`public  Message(ClassType clss,`[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` meth)` | 
`public  Message(const `[`Message`](#classscy_1_1stun_1_1Message)` & that)` | 
`public `[`Message`](#classscy_1_1stun_1_1Message)` & operator=(const `[`Message`](#classscy_1_1stun_1_1Message)` & that)` | 
`public virtual  ~Message()` | 
`public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public void setClass(ClassType type)` | 
`public void setMethod(`[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` type)` | 
`public void setTransactionID(const std::string & id)` | 
`public ClassType classType() const` | 
`public `[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` methodType() const` | 
`public inline const TransactionID & transactionID() const` | 
`public inline const std::vector< `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * > attrs() const` | 
`public inline virtual size_t size() const` | 
`public std::string methodString() const` | 
`public std::string classString() const` | 
`public std::string errorString(uint16_t errorCode) const` | 
`public void add(`[`Attribute`](#classscy_1_1stun_1_1Attribute)` * attr)` | 
`public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * get(`[`Attribute::Type`](#group__stun_1ga58f9974b99873c21b541593d70751f76)` type,int index) const` | 
`public template<typename T>`  <br/>`inline T * get(int index) const` | 
`public virtual ssize_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buf)` | 
`public virtual void write(`[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` & buf) const` | Writes this object into a STUN/TURN packet.
`public std::string toString() const` | 
`public virtual void print(std::ostream & os) const` | 
`public inline virtual const char * className() const` | 
`protected uint16_t _class` | 
`protected uint16_t _method` | 
`protected uint16_t _size` | 
`protected TransactionID _transactionID` | 
`protected std::vector< `[`Attribute`](./doc/api-stun.md#classscy_1_1stun_1_1Attribute)` * > _attrs` | 

## Members

#### `public  Message()` 





#### `public  Message(ClassType clss,`[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` meth)` 





#### `public  Message(const `[`Message`](#classscy_1_1stun_1_1Message)` & that)` 





#### `public `[`Message`](#classscy_1_1stun_1_1Message)` & operator=(const `[`Message`](#classscy_1_1stun_1_1Message)` & that)` 





#### `public virtual  ~Message()` 





#### `public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public void setClass(ClassType type)` 





#### `public void setMethod(`[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` type)` 





#### `public void setTransactionID(const std::string & id)` 





#### `public ClassType classType() const` 





#### `public `[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` methodType() const` 





#### `public inline const TransactionID & transactionID() const` 





#### `public inline const std::vector< `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * > attrs() const` 





#### `public inline virtual size_t size() const` 



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__stun_1ga3c6edd2eda4563ce398271ca1e398f2f), but may not be the number of bytes that will be consumed by [read()](#group__stun_1ga5277c8889063ec2d13b50de692a0f544).

#### `public std::string methodString() const` 





#### `public std::string classString() const` 





#### `public std::string errorString(uint16_t errorCode) const` 





#### `public void add(`[`Attribute`](#classscy_1_1stun_1_1Attribute)` * attr)` 





#### `public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * get(`[`Attribute::Type`](#group__stun_1ga58f9974b99873c21b541593d70751f76)` type,int index) const` 





#### `public template<typename T>`  <br/>`inline T * get(int index) const` 





#### `public virtual ssize_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buf)` 



Parses the STUN/TURN packet from the given buffer. The return value indicates the number of bytes read.

#### `public virtual void write(`[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` & buf) const` 

Writes this object into a STUN/TURN packet.



#### `public std::string toString() const` 





#### `public virtual void print(std::ostream & os) const` 





#### `public inline virtual const char * className() const` 





#### `protected uint16_t _class` 





#### `protected uint16_t _method` 





#### `protected uint16_t _size` 





#### `protected TransactionID _transactionID` 





#### `protected std::vector< `[`Attribute`](./doc/api-stun.md#classscy_1_1stun_1_1Attribute)` * > _attrs` 





# class `scy::stun::MessageIntegrity` 

```
class scy::stun::MessageIntegrity
  : public scy::stun::Attribute
```  

Implements STUN/TURN attributes that reflects an internet address.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  MessageIntegrity()` | 
`public  MessageIntegrity(const `[`MessageIntegrity`](#classscy_1_1stun_1_1MessageIntegrity)` & r)` | 
`public virtual  ~MessageIntegrity()` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public bool verifyHmac(const std::string & key) const` | 
`public inline std::string input() const` | 
`public inline std::string hmac() const` | 
`public inline std::string key() const` | 
`public inline void setInput(const std::string & input)` | 
`public inline void setHmac(const std::string & hmac)` | 
`public inline void setKey(const std::string & key)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  MessageIntegrity()` 





#### `public  MessageIntegrity(const `[`MessageIntegrity`](#classscy_1_1stun_1_1MessageIntegrity)` & r)` 





#### `public virtual  ~MessageIntegrity()` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public bool verifyHmac(const std::string & key) const` 





#### `public inline std::string input() const` 





#### `public inline std::string hmac() const` 





#### `public inline std::string key() const` 





#### `public inline void setInput(const std::string & input)` 





#### `public inline void setHmac(const std::string & hmac)` 





#### `public inline void setKey(const std::string & key)` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::StringAttribute` 

```
class scy::stun::StringAttribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects an arbitrary byte string.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  StringAttribute(uint16_t type,uint16_t size)` | 
`public  StringAttribute(const `[`StringAttribute`](#classscy_1_1stun_1_1StringAttribute)` & r)` | 
`public virtual  ~StringAttribute()` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline const char * bytes() const` | 
`public void setBytes(char * bytes,unsigned size)` | 
`public std::string asString() const` | 
`public void copyBytes(const char * bytes)` | 
`public void copyBytes(const void * bytes,unsigned size)` | 
`public uint8_t getByte(int index) const` | 
`public void setByte(int index,uint8_t value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  StringAttribute(uint16_t type,uint16_t size)` 





#### `public  StringAttribute(const `[`StringAttribute`](#classscy_1_1stun_1_1StringAttribute)` & r)` 





#### `public virtual  ~StringAttribute()` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public inline const char * bytes() const` 





#### `public void setBytes(char * bytes,unsigned size)` 





#### `public std::string asString() const` 





#### `public void copyBytes(const char * bytes)` 





#### `public void copyBytes(const void * bytes,unsigned size)` 





#### `public uint8_t getByte(int index) const` 





#### `public void setByte(int index,uint8_t value)` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::Transaction` 

```
class scy::stun::Transaction
  : public scy::net::Transaction< Message >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Transaction(const net::Socket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,long timeout,int retries)` | 
`public virtual bool checkResponse(const `[`Message`](#classscy_1_1stun_1_1Message)` & message)` | 
`public virtual void onResponse()` | Called when a successful response match is received.
`protected virtual  ~Transaction()` | 

## Members

#### `public  Transaction(const net::Socket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,long timeout,int retries)` 





#### `public virtual bool checkResponse(const `[`Message`](#classscy_1_1stun_1_1Message)` & message)` 





#### `public virtual void onResponse()` 

Called when a successful response match is received.



#### `protected virtual  ~Transaction()` 





# class `scy::stun::UInt16ListAttribute` 

```
class scy::stun::UInt16ListAttribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a list of attribute names.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt16ListAttribute(uint16_t type,uint16_t size)` | 
`public  UInt16ListAttribute(const `[`UInt16ListAttribute`](#classscy_1_1stun_1_1UInt16ListAttribute)` & r)` | 
`public virtual  ~UInt16ListAttribute()` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public size_t size() const` | 
`public uint16_t getType(int index) const` | 
`public void setType(int index,uint16_t value)` | 
`public void addType(uint16_t value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt16ListAttribute(uint16_t type,uint16_t size)` 





#### `public  UInt16ListAttribute(const `[`UInt16ListAttribute`](#classscy_1_1stun_1_1UInt16ListAttribute)` & r)` 





#### `public virtual  ~UInt16ListAttribute()` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public size_t size() const` 





#### `public uint16_t getType(int index) const` 





#### `public void setType(int index,uint16_t value)` 





#### `public void addType(uint16_t value)` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::UInt32Attribute` 

```
class scy::stun::UInt32Attribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a 32-bit integer.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt32Attribute(uint16_t type)` | 
`public  UInt32Attribute(const `[`UInt32Attribute`](#classscy_1_1stun_1_1UInt32Attribute)` & r)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline uint32_t value() const` | 
`public inline void setValue(uint32_t bits)` | 
`public bool getBit(int index) const` | 
`public void setBit(int index,bool value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt32Attribute(uint16_t type)` 





#### `public  UInt32Attribute(const `[`UInt32Attribute`](#classscy_1_1stun_1_1UInt32Attribute)` & r)` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public inline uint32_t value() const` 





#### `public inline void setValue(uint32_t bits)` 





#### `public bool getBit(int index) const` 





#### `public void setBit(int index,bool value)` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::UInt64Attribute` 

```
class scy::stun::UInt64Attribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a 64-bit integer.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt64Attribute(uint16_t type)` | 
`public  UInt64Attribute(const `[`UInt64Attribute`](#classscy_1_1stun_1_1UInt64Attribute)` & r)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline uint64_t value() const` | 
`public inline void setValue(uint64_t bits)` | 
`public bool getBit(int index) const` | 
`public void setBit(int index,bool value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt64Attribute(uint16_t type)` 





#### `public  UInt64Attribute(const `[`UInt64Attribute`](#classscy_1_1stun_1_1UInt64Attribute)` & r)` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public inline uint64_t value() const` 





#### `public inline void setValue(uint64_t bits)` 





#### `public bool getBit(int index) const` 





#### `public void setBit(int index,bool value)` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::UInt8Attribute` 

```
class scy::stun::UInt8Attribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a 32-bit integer.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt8Attribute(uint16_t type)` | 
`public  UInt8Attribute(const `[`UInt8Attribute`](#classscy_1_1stun_1_1UInt8Attribute)` & r)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline uint8_t value() const` | 
`public inline void setValue(uint8_t bits)` | 
`public bool getBit(int index) const` | 
`public void setBit(int index,bool value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt8Attribute(uint16_t type)` 





#### `public  UInt8Attribute(const `[`UInt8Attribute`](#classscy_1_1stun_1_1UInt8Attribute)` & r)` 





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` 





#### `public inline uint8_t value() const` 





#### `public inline void setValue(uint8_t bits)` 





#### `public bool getBit(int index) const` 





#### `public void setBit(int index,bool value)` 





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` 



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` 



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

