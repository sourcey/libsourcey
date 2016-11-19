# Module <!-- group --> `stun`

The `stun` module contains a STUN (rfc5389) implementation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::stun`](#namespacescy_1_1stun)    | 
# namespace `scy::stun` {#namespacescy_1_1stun}



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
# class `scy::stun::AddressAttribute` {#classscy_1_1stun_1_1AddressAttribute}

```
class scy::stun::AddressAttribute
  : public scy::stun::Attribute
```  

Implements a STUN/TURN attribute that contains a socket address.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  AddressAttribute(std::uint16_t type,bool ipv4)` | 
`public  AddressAttribute(const `[`AddressAttribute`](#classscy_1_1stun_1_1AddressAttribute)` & r)` | 
`public virtual `[`stun::Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline `[`stun::AddressFamily`](#group__stun_1gaec260a57e19eb607c18301f58f80e7e1)` family() const` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 
`public inline virtual void setAddress(const `[`net::Address`](#classscy_1_1net_1_1Address)` & addr)` | 

## Members

#### `public  AddressAttribute(std::uint16_t type,bool ipv4)` {#group__stun_1gab229f050517a8d129c651103061ed128}





#### `public  AddressAttribute(const `[`AddressAttribute`](#classscy_1_1stun_1_1AddressAttribute)` & r)` {#group__stun_1gab1889e0952a12fef4df481639ff5dfd6}





#### `public virtual `[`stun::Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1ga353495e9c7ba4c4bb79df619385f7adc}





#### `public inline `[`stun::AddressFamily`](#group__stun_1gaec260a57e19eb607c18301f58f80e7e1)` family() const` {#group__stun_1gaa211f6fa23b641e1c922595b1884c287}





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` {#group__stun_1gae694a695a83a7310bb2c2da8e504353c}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1ga8bfb33c07b10d89dbdd90d08f278fa52}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1ga32ac592e83353d0fe7c5231583dd1510}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

#### `public inline virtual void setAddress(const `[`net::Address`](#classscy_1_1net_1_1Address)` & addr)` {#group__stun_1ga6c71d6b0ab7739e63ef2eb58241501c1}





# class `scy::stun::Attribute` {#classscy_1_1stun_1_1Attribute}


The virtual base class for all STUN/TURN attributes.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~Attribute()` | 
`public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 
`public std::uint16_t type() const` | 
`public std::uint16_t size() const` | 
`public void consumePadding(`[`BitReader`](#classscy_1_1BitReader)` & reader) const` | 
`public void writePadding(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 
`public std::string typeString()` | 
`protected std::uint16_t _type` | 
`protected std::uint16_t _size` | 
`protected  Attribute(std::uint16_t type,std::uint16_t size)` | 
`protected void setLength(std::uint16_t size)` | 

## Members

#### `public inline virtual  ~Attribute()` {#group__stun_1ga3d20ce1b5f294cccb1ceecfd7dde685b}





#### `public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1ga9a66e1f6785bf95bb8d7a2fb17ca7178}





#### `public void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1ga7a8eaf6fae7ce506dead82c972fee53d}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1ga57e4567afce55ae046a19e20d5c96cb8}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

#### `public std::uint16_t type() const` {#group__stun_1ga83a6053b569bdffa2bda1c09cc572d79}





#### `public std::uint16_t size() const` {#group__stun_1ga23903df08c8efb6200822e1390517930}





#### `public void consumePadding(`[`BitReader`](#classscy_1_1BitReader)` & reader) const` {#group__stun_1ga49af5c5548066acb3de3553218ba04e4}





#### `public void writePadding(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1ga9337ef7bb7032ea501938e7713f7bc99}





#### `public std::string typeString()` {#group__stun_1ga11de2203671a16db2e059abaade0d148}





#### `protected std::uint16_t _type` {#group__stun_1gafe98512173246af64861f8cbcbc775d0}





#### `protected std::uint16_t _size` {#group__stun_1ga4e2f51ff31b5f70b44fbb37faed1a937}





#### `protected  Attribute(std::uint16_t type,std::uint16_t size)` {#group__stun_1ga95401393d6fc4308c9167e1f97d03ef4}





#### `protected void setLength(std::uint16_t size)` {#group__stun_1ga661180b8cab96a02b4833863d82bb87d}





# class `scy::stun::ErrorCode` {#classscy_1_1stun_1_1ErrorCode}

```
class scy::stun::ErrorCode
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects an error code.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ErrorCode(std::uint16_t size)` | 
`public  ErrorCode(const `[`ErrorCode`](#classscy_1_1stun_1_1ErrorCode)` & r)` | 
`public virtual  ~ErrorCode()` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public void setErrorCode(int code)` | 
`public void setReason(const std::string & reason)` | 
`public int errorCode() const` | 
`public inline std::uint8_t errorClass() const` | 
`public inline std::uint8_t errorNumber() const` | 
`public inline const std::string & reason() const` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  ErrorCode(std::uint16_t size)` {#group__stun_1gaa4fe8c4fbc9876d32eddc64c05dd6c99}





#### `public  ErrorCode(const `[`ErrorCode`](#classscy_1_1stun_1_1ErrorCode)` & r)` {#group__stun_1ga90128dc8b841038c32835371b84afa93}





#### `public virtual  ~ErrorCode()` {#group__stun_1gadcf975e8279a398a328a16b75dc29ca5}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1gaeddb43338163009609066b78d793a904}





#### `public void setErrorCode(int code)` {#group__stun_1gabffcf45778d52bcb75550ca73f7c8f9e}





#### `public void setReason(const std::string & reason)` {#group__stun_1ga84d94ef041ed8a5e0ddc63547ac86114}





#### `public int errorCode() const` {#group__stun_1gaca2120c5893bbf603e0b199a285008ae}





#### `public inline std::uint8_t errorClass() const` {#group__stun_1ga7bd232404be9fc6d01ad90a9428982f7}





#### `public inline std::uint8_t errorNumber() const` {#group__stun_1ga77e1c7bc8756747708820357bdb2665b}





#### `public inline const std::string & reason() const` {#group__stun_1ga212965d6079e0d911e29c72dc51f37e3}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1gaea0dbfaec3174f09753e115bc23be3fe}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1ga9713f09994fe58c79c17379f04f91b2d}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::FlagAttribute` {#classscy_1_1stun_1_1FlagAttribute}

```
class scy::stun::FlagAttribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute representing a 0 size flag.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FlagAttribute(std::uint16_t type)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public inline virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  FlagAttribute(std::uint16_t type)` {#group__stun_1ga2ae466bf8f9dd28dd16436728fcf8a41}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1ga49912a94c00a205c9077096cf04ea603}





#### `public inline virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1gad3cc271ce3750886413629b2770b2b5d}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public inline virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1gaf9da0d01262873cf261bb55b0b8ad41b}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::Message` {#classscy_1_1stun_1_1Message}

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
`public inline virtual std::size_t size() const` | 
`public std::string methodString() const` | 
`public std::string classString() const` | 
`public std::string errorString(std::uint16_t errorCode) const` | 
`public void add(`[`Attribute`](#classscy_1_1stun_1_1Attribute)` * attr)` | 
`public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * get(`[`Attribute::Type`](#group__stun_1ga58f9974b99873c21b541593d70751f76)` type,int index) const` | 
`public template<typename T>`  <br/>`inline T * get(int index) const` | 
`public virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buf)` | 
`public virtual void write(Buffer & buf) const` | Writes this object into a STUN/TURN packet.
`public std::string toString() const` | 
`public virtual void print(std::ostream & os) const` | 
`public inline virtual const char * className() const` | 
`protected std::uint16_t _class` | 
`protected std::uint16_t _method` | 
`protected std::uint16_t _size` | 
`protected TransactionID _transactionID` | 
`protected std::vector< `[`Attribute`](./doc/api-stun.md#classscy_1_1stun_1_1Attribute)` * > _attrs` | 

## Members

#### `public  Message()` {#group__stun_1ga87d2d455f130210e6ebdcf10d0318f12}





#### `public  Message(ClassType clss,`[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` meth)` {#group__stun_1ga19a2f2943326de6b24e4364c136482bf}





#### `public  Message(const `[`Message`](#classscy_1_1stun_1_1Message)` & that)` {#group__stun_1ga871f9b3b19510574bae25bd42509a1e7}





#### `public `[`Message`](#classscy_1_1stun_1_1Message)` & operator=(const `[`Message`](#classscy_1_1stun_1_1Message)` & that)` {#group__stun_1gaa7b3f543c5d432bb2b052820b42d9dbd}





#### `public virtual  ~Message()` {#group__stun_1ga4a2d3d7faa8d1fae09689521c719f0e3}





#### `public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__stun_1gab3b901af3b3812c59a1e6e495f9ff9bd}





#### `public void setClass(ClassType type)` {#group__stun_1gab1616a105946047c77332b9e5caadaac}





#### `public void setMethod(`[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` type)` {#group__stun_1gac7c6555f867fef8dd90c388d13ba2afa}





#### `public void setTransactionID(const std::string & id)` {#group__stun_1ga7e1341a7429f6b7199354fda9b235a02}





#### `public ClassType classType() const` {#group__stun_1ga7243fa9b4b257cd1ed0a00ae6ed131af}





#### `public `[`MethodType`](#group__stun_1gac2630e3f14c223cdb85801feb8f848ee)` methodType() const` {#group__stun_1ga6453d6cb76a0ac955cf7c2d049b8ccce}





#### `public inline const TransactionID & transactionID() const` {#group__stun_1ga43b94a7267e6192c331a72364586bbd9}





#### `public inline const std::vector< `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * > attrs() const` {#group__stun_1ga090e77b617ae87ab854f761d1228927c}





#### `public inline virtual std::size_t size() const` {#group__stun_1ga5c6b12c3ed9f53849195935d0562e689}



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__stun_1ga3c6edd2eda4563ce398271ca1e398f2f), but may not be the number of bytes that will be consumed by [read()](#group__stun_1ga39d2f47307460a6e1178466b0fd06b16).

#### `public std::string methodString() const` {#group__stun_1ga6cb46477fe5b8deca78bcfe3be57630c}





#### `public std::string classString() const` {#group__stun_1gae0caeab266e261ca5e0f904fbac29a2a}





#### `public std::string errorString(std::uint16_t errorCode) const` {#group__stun_1ga653feec11da0c7ea7ceea6a4a78b6f23}





#### `public void add(`[`Attribute`](#classscy_1_1stun_1_1Attribute)` * attr)` {#group__stun_1ga6d8a672d498981ae3aaf6ae56b64ad57}





#### `public `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * get(`[`Attribute::Type`](#group__stun_1ga58f9974b99873c21b541593d70751f76)` type,int index) const` {#group__stun_1ga42b45b4a75a1c9a1b7f4e3ee8117e953}





#### `public template<typename T>`  <br/>`inline T * get(int index) const` {#group__stun_1ga64605e3e2ab251df91571180c3228d71}





#### `public virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buf)` {#group__stun_1ga39d2f47307460a6e1178466b0fd06b16}



Parses the STUN/TURN packet from the given buffer. The return value indicates the number of bytes read.

#### `public virtual void write(Buffer & buf) const` {#group__stun_1ga3c6edd2eda4563ce398271ca1e398f2f}

Writes this object into a STUN/TURN packet.



#### `public std::string toString() const` {#group__stun_1ga50c1ef178929f59e10581ce89f38dbea}





#### `public virtual void print(std::ostream & os) const` {#group__stun_1gab7840803d8273b4343225476ed0c8b2a}





#### `public inline virtual const char * className() const` {#group__stun_1gab3a3ece9dbc59d45f2ae864ef6e601f6}





#### `protected std::uint16_t _class` {#group__stun_1ga24a6e8eae578a83e0aa3e8f027f76488}





#### `protected std::uint16_t _method` {#group__stun_1ga3569fe9a1490eac578f5292f29a8fcc8}





#### `protected std::uint16_t _size` {#group__stun_1gaa83b315ab5af67d8507d9a68539d4ec1}





#### `protected TransactionID _transactionID` {#group__stun_1ga664565d5c2616896d5128d84ca2a7f5f}





#### `protected std::vector< `[`Attribute`](./doc/api-stun.md#classscy_1_1stun_1_1Attribute)` * > _attrs` {#group__stun_1ga782bc1c8f90278aa447d27c83e797f8a}





# class `scy::stun::MessageIntegrity` {#classscy_1_1stun_1_1MessageIntegrity}

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

#### `public  MessageIntegrity()` {#group__stun_1ga6d3a6a3407b970154055dbe446670c8f}





#### `public  MessageIntegrity(const `[`MessageIntegrity`](#classscy_1_1stun_1_1MessageIntegrity)` & r)` {#group__stun_1ga0694d43ce6dc45ba4a11feffcf3c739b}





#### `public virtual  ~MessageIntegrity()` {#group__stun_1gac74f0c9b519ad52390a6a4c231f9e60a}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1gae4754761b864734af2312feaec93309a}





#### `public bool verifyHmac(const std::string & key) const` {#group__stun_1gac299a78f8fa192ac0ae022ede8abaad1}





#### `public inline std::string input() const` {#group__stun_1ga403625dab98075c2b89fe8a65a9c0360}





#### `public inline std::string hmac() const` {#group__stun_1ga7d79aa318b53e4414af5669f911ec13b}





#### `public inline std::string key() const` {#group__stun_1ga0f30d8f2c2a6d93893ac5a520bfd9f4f}





#### `public inline void setInput(const std::string & input)` {#group__stun_1ga68372c04e7f849e206c8ceb5f0a19e3b}





#### `public inline void setHmac(const std::string & hmac)` {#group__stun_1gad71c737937a85fd4af54f6529d30d739}





#### `public inline void setKey(const std::string & key)` {#group__stun_1gaeafda2d185a37768cf3c1537bfa48447}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1ga2f5d833067f52d8818ab6163c1f3e837}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1ga6306fc5f049917738c5f199f119102f4}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::StringAttribute` {#classscy_1_1stun_1_1StringAttribute}

```
class scy::stun::StringAttribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects an arbitrary byte string.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  StringAttribute(std::uint16_t type,std::uint16_t size)` | 
`public  StringAttribute(const `[`StringAttribute`](#classscy_1_1stun_1_1StringAttribute)` & r)` | 
`public virtual  ~StringAttribute()` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline const char * bytes() const` | 
`public void setBytes(char * bytes,unsigned size)` | 
`public std::string asString() const` | 
`public void copyBytes(const char * bytes)` | 
`public void copyBytes(const void * bytes,unsigned size)` | 
`public std::uint8_t getByte(int index) const` | 
`public void setByte(int index,std::uint8_t value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  StringAttribute(std::uint16_t type,std::uint16_t size)` {#group__stun_1ga86ab6d17cbe93921c56b5cfbff4709b4}





#### `public  StringAttribute(const `[`StringAttribute`](#classscy_1_1stun_1_1StringAttribute)` & r)` {#group__stun_1ga6e51e83ff47fc75c46e8da2f87c338bf}





#### `public virtual  ~StringAttribute()` {#group__stun_1ga44630835710accc66471ff5b43c19e87}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1gae98d4e3d1e679a3b0ef93e85dadebd86}





#### `public inline const char * bytes() const` {#group__stun_1ga194bbae8b24f3b0e6d89a8d23d6ade79}





#### `public void setBytes(char * bytes,unsigned size)` {#group__stun_1ga888af01fcadc64d36ce0f997750f3ffc}





#### `public std::string asString() const` {#group__stun_1gaf3e4233dab7e4fa689f5bc1930391b99}





#### `public void copyBytes(const char * bytes)` {#group__stun_1gace004bc4eda7af876d91f1b7e5a01d02}





#### `public void copyBytes(const void * bytes,unsigned size)` {#group__stun_1ga3480d63f8b4d93697092fba0bdfd2e4d}





#### `public std::uint8_t getByte(int index) const` {#group__stun_1ga6b39cf56203913ead6997c5844ce76ae}





#### `public void setByte(int index,std::uint8_t value)` {#group__stun_1ga1b6cfcc27aeb241782685a0c2e53e4ef}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1ga9c7a53c394639c0981edc403f76e2944}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1ga61a2e9284efecad7fd37c77a8d341499}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::Transaction` {#classscy_1_1stun_1_1Transaction}

```
class scy::stun::Transaction
  : public scy::net::Transaction< Message >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Transaction(const net::Socket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,long timeout,int retries,uv::Loop * loop)` | 
`public virtual bool checkResponse(const `[`Message`](#classscy_1_1stun_1_1Message)` & message)` | 
`public virtual void onResponse()` | Called when a successful response match is received.
`protected virtual  ~Transaction()` | 

## Members

#### `public  Transaction(const net::Socket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,long timeout,int retries,uv::Loop * loop)` {#group__stun_1gaf8eeed1383daa7390adf13c2fce60470}





#### `public virtual bool checkResponse(const `[`Message`](#classscy_1_1stun_1_1Message)` & message)` {#group__stun_1ga82ab3d79a18d478172c6b39cc6d7428a}





#### `public virtual void onResponse()` {#group__stun_1gabeecdd054fceca609f3fd449a323159f}

Called when a successful response match is received.



#### `protected virtual  ~Transaction()` {#group__stun_1ga8ab19322de3bf9d004ff2dacbcee5daf}





# class `scy::stun::UInt16ListAttribute` {#classscy_1_1stun_1_1UInt16ListAttribute}

```
class scy::stun::UInt16ListAttribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a list of attribute names.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt16ListAttribute(std::uint16_t type,std::uint16_t size)` | 
`public  UInt16ListAttribute(const `[`UInt16ListAttribute`](#classscy_1_1stun_1_1UInt16ListAttribute)` & r)` | 
`public virtual  ~UInt16ListAttribute()` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public std::size_t size() const` | 
`public std::uint16_t getType(int index) const` | 
`public void setType(int index,std::uint16_t value)` | 
`public void addType(std::uint16_t value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt16ListAttribute(std::uint16_t type,std::uint16_t size)` {#group__stun_1ga1f68dbc72441c0e906590acecb7da037}





#### `public  UInt16ListAttribute(const `[`UInt16ListAttribute`](#classscy_1_1stun_1_1UInt16ListAttribute)` & r)` {#group__stun_1ga41111abd042a5eabd072a592e423a293}





#### `public virtual  ~UInt16ListAttribute()` {#group__stun_1gacc94c85b8a1b73a30b6a764e40685940}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1ga6cd6d7c6633041cb6081f10998f20f79}





#### `public std::size_t size() const` {#group__stun_1ga15a86cb2a82b8bbe32264dee6565df03}





#### `public std::uint16_t getType(int index) const` {#group__stun_1gaedc90efe4aacbb54e9a58f78e634cad9}





#### `public void setType(int index,std::uint16_t value)` {#group__stun_1gab28ae2db6e480d3872f6b609cf887ca9}





#### `public void addType(std::uint16_t value)` {#group__stun_1ga2468101b162dca9a58dec84d3d60dab7}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1gad84d782791bbf383686b42961424c0dc}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1gaf14b570a4d6d233e8c05d48a00e4160e}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::UInt32Attribute` {#classscy_1_1stun_1_1UInt32Attribute}

```
class scy::stun::UInt32Attribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a 32-bit integer.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt32Attribute(std::uint16_t type)` | 
`public  UInt32Attribute(const `[`UInt32Attribute`](#classscy_1_1stun_1_1UInt32Attribute)` & r)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline std::uint32_t value() const` | 
`public inline void setValue(std::uint32_t bits)` | 
`public bool getBit(int index) const` | 
`public void setBit(int index,bool value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt32Attribute(std::uint16_t type)` {#group__stun_1gaa9796cf60b084f1d2cb8219f8a9642f5}





#### `public  UInt32Attribute(const `[`UInt32Attribute`](#classscy_1_1stun_1_1UInt32Attribute)` & r)` {#group__stun_1ga0bf66ed373b22ff7629d82410fa4209a}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1ga449e977050879e53d8f6f974a5cbf35b}





#### `public inline std::uint32_t value() const` {#group__stun_1gaae1d32cad730956417c7d33dd3f28e30}





#### `public inline void setValue(std::uint32_t bits)` {#group__stun_1ga8a81a2f289d2ab9c8c585ae6cd278e3c}





#### `public bool getBit(int index) const` {#group__stun_1gabe7f310ea52d35848ff63ae5e02ef153}





#### `public void setBit(int index,bool value)` {#group__stun_1ga9745eaf379f6449f0a9aa3723cb342f2}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1ga35d119715876608a654b7eec9358985d}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1ga5011ddbe1dee2aa14bac532f38f199dc}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::UInt64Attribute` {#classscy_1_1stun_1_1UInt64Attribute}

```
class scy::stun::UInt64Attribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a 64-bit integer.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt64Attribute(std::uint16_t type)` | 
`public  UInt64Attribute(const `[`UInt64Attribute`](#classscy_1_1stun_1_1UInt64Attribute)` & r)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline std::uint64_t value() const` | 
`public inline void setValue(std::uint64_t bits)` | 
`public bool getBit(int index) const` | 
`public void setBit(int index,bool value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt64Attribute(std::uint16_t type)` {#group__stun_1ga8f1a238309c10b7a45de2f22ac3a4763}





#### `public  UInt64Attribute(const `[`UInt64Attribute`](#classscy_1_1stun_1_1UInt64Attribute)` & r)` {#group__stun_1gae47d568c25008f4d3a6f85c540b0d3bd}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1ga7e3f6b4276ddc4c30663e2616fecb312}





#### `public inline std::uint64_t value() const` {#group__stun_1gadbb64f89d11aa87949ceaaf5695e1553}





#### `public inline void setValue(std::uint64_t bits)` {#group__stun_1gad2a8af482c899f4d5501e451e1162913}





#### `public bool getBit(int index) const` {#group__stun_1ga6de43e36aa136e9b6a14e9821b5038e7}





#### `public void setBit(int index,bool value)` {#group__stun_1ga48748faff06ececd74ef35a1410ca2f6}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1ga19211d4ed0ed81056896ab653ef012bc}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1gaad47c6f9630fdb678980667ffb438e14}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

# class `scy::stun::UInt8Attribute` {#classscy_1_1stun_1_1UInt8Attribute}

```
class scy::stun::UInt8Attribute
  : public scy::stun::Attribute
```  

Implements STUN/TURN attribute that reflects a 32-bit integer.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UInt8Attribute(std::uint16_t type)` | 
`public  UInt8Attribute(const `[`UInt8Attribute`](#classscy_1_1stun_1_1UInt8Attribute)` & r)` | 
`public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` | 
`public inline std::uint8_t value() const` | 
`public inline void setValue(std::uint8_t bits)` | 
`public bool getBit(int index) const` | 
`public void setBit(int index,bool value)` | 
`public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` | 
`public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` | 

## Members

#### `public  UInt8Attribute(std::uint16_t type)` {#group__stun_1ga41164add368af3cd3fa10f5adc5fe85e}





#### `public  UInt8Attribute(const `[`UInt8Attribute`](#classscy_1_1stun_1_1UInt8Attribute)` & r)` {#group__stun_1gae1a4c7c1a53211e52739bd68955a26dd}





#### `public virtual `[`Attribute`](#classscy_1_1stun_1_1Attribute)` * clone()` {#group__stun_1ga99363a9c386f49d4d66656342e3184ad}





#### `public inline std::uint8_t value() const` {#group__stun_1gae5052d4fa856ce16cbbac10f3797dc6a}





#### `public inline void setValue(std::uint8_t bits)` {#group__stun_1ga64fa8077f79e754c88e1ef7cc7a0fec7}





#### `public bool getBit(int index) const` {#group__stun_1gae230c8a7751d5edfd007cd34b6f56cca}





#### `public void setBit(int index,bool value)` {#group__stun_1ga05fb3927bbbb041bd82e06473b686cb3}





#### `public virtual void read(`[`BitReader`](#classscy_1_1BitReader)` & reader)` {#group__stun_1ga118428fe9039df05f3bd59f6d9c2cd18}



Reads the body (not the type or size) for this type of attribute from the given buffer. Return value is true if successful.

#### `public virtual void write(`[`BitWriter`](#classscy_1_1BitWriter)` & writer) const` {#group__stun_1gab168e136e6435b0bb6ee8f81d7d7e27b}



Writes the body (not the type or size) to the given buffer. Return value is true if successful.

