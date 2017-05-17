# Module <!-- group --> `symple`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::smpl`](#namespacescy_1_1smpl)    | 
`struct `[`scy::smpl::Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)    | 
# namespace `scy::smpl` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::smpl::Client`](#classscy_1_1smpl_1_1Client)    | 
`class `[`scy::smpl::Command`](#classscy_1_1smpl_1_1Command)    | 
`class `[`scy::smpl::Event`](#classscy_1_1smpl_1_1Event)    | 
`class `[`scy::smpl::Form`](#classscy_1_1smpl_1_1Form)    | 
`class `[`scy::smpl::FormElement`](#classscy_1_1smpl_1_1FormElement)    | 
`class `[`scy::smpl::FormField`](#classscy_1_1smpl_1_1FormField)    | 
`class `[`scy::smpl::Message`](#classscy_1_1smpl_1_1Message)    | 
`class `[`scy::smpl::Peer`](#classscy_1_1smpl_1_1Peer)    | 
`class `[`scy::smpl::Presence`](#classscy_1_1smpl_1_1Presence)    | 
`class `[`scy::smpl::Roster`](#classscy_1_1smpl_1_1Roster)    | 
`class `[`scy::smpl::SSLClient`](#classscy_1_1smpl_1_1SSLClient)    | 
`class `[`scy::smpl::TCPClient`](#classscy_1_1smpl_1_1TCPClient)    | 
`struct `[`scy::smpl::Address`](#structscy_1_1smpl_1_1Address)    | 
`struct `[`scy::smpl::Filter`](#structscy_1_1smpl_1_1Filter)    | 
# class `scy::smpl::Client` 

```
class scy::smpl::Client
  : public scy::sockio::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)< void(const int &)`> Announce` | 
`public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> PeerConnected` | Signals when a peer connects.
`public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> PeerDisconnected` | Signals when a peer disconnects.
`public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> CreatePresence` | 
`public  Client(const net::Socket::Ptr & socket,const `[`Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options)` | 
`public virtual  ~Client()` | 
`public virtual void connect()` | 
`public virtual void close()` | 
`public virtual int send(`[`Message`](#classscy_1_1smpl_1_1Message)` & message,bool ack)` | 
`public virtual int send(const std::string & message,bool ack)` | 
`public `[`sockio::Transaction`](#structscy_1_1sockio_1_1Transaction)` * createTransaction(`[`Message`](#classscy_1_1smpl_1_1Message)` & message)` | 
`public virtual int respond(`[`Message`](#classscy_1_1smpl_1_1Message)` & message,bool ack)` | Swap the 'to' and 'from' fields and send the given message.
`public virtual int sendPresence(bool probe)` | 
`public virtual int sendPresence(const `[`Address`](#structscy_1_1smpl_1_1Address)` & to,bool probe)` | Send directed presence to the given peer.
`public virtual int joinRoom(const std::string & room)` | Join the given room.
`public virtual int leaveRoom(const std::string & room)` | Leave the given room.
`public virtual std::string ourID() const` | 
`public StringVec rooms() const` | Return a list of rooms the client has joined.
`public virtual `[`Peer`](#classscy_1_1smpl_1_1Peer)` * ourPeer()` | 
`public virtual `[`Roster`](#classscy_1_1smpl_1_1Roster)` & roster()` | Return the roster which stores all online peers.
`public virtual `[`PersistenceT`](#classscy_1_1TimedManager)` & persistence()` | 
`public `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options()` | Return a reference to the client options object.
`public virtual `[`Client`](#classscy_1_1smpl_1_1Client)` & operator>>(`[`Message`](#classscy_1_1smpl_1_1Message)` & message)` | [Stream](#classscy_1_1Stream) operator alias for [send()](#group__symple_1ga4dffb71d16288852dbeb54e38ce9cc82).
`public virtual void onPresenceData(const json::value & data,bool whiny)` | Update the roster from the given client object.
`protected `[`Roster`](./doc/api-symple.md#classscy_1_1smpl_1_1Roster)` _roster` | 
`protected std::string _ourID` | 
`protected `[`PersistenceT`](./doc/api-util.md#classscy_1_1TimedManager)` _persistence` | 
`protected `[`Client::Options`](./doc/api-symple.md#structscy_1_1smpl_1_1Client_1_1Options)` _options` | 
`protected StringVec _rooms` | 
`protected int _announceStatus` | 
`protected virtual int announce()` | 
`protected virtual void reset()` | 
`protected virtual void createPresence(`[`Presence`](#classscy_1_1smpl_1_1Presence)` & p)` | Creates a [Presence](./doc/api-symple.md#classscy_1_1smpl_1_1Presence) object.
`protected virtual void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | Override PacketSignal::emit.
`protected virtual void onOnline()` | 
`protected virtual void onAnnounceState(void * sender,`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` | 

## Members

#### `public `[`Signal`](#classscy_1_1Signal)< void(const int &)`> Announce` 



SignalsNotifies the outside application about the response status code of our [announce()](#group__symple_1gadf0ac053e026a7c792d00ac165cb1298) call. Possible status codes are: // - 200: Authentication success

* 401: Authentication failed


* 400: Bad request data


* 500: Server not found

#### `public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> PeerConnected` 

Signals when a peer connects.



#### `public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> PeerDisconnected` 

Signals when a peer disconnects.



#### `public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> CreatePresence` 



Called by [createPresence()](#group__symple_1gace2ee9e3f211f1fb9979953c6d0dfa34) so outside classes can modify the outgoing [Peer](#classscy_1_1smpl_1_1Peer) JSON object.

#### `public  Client(const net::Socket::Ptr & socket,const `[`Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options)` 





#### `public virtual  ~Client()` 





#### `public virtual void connect()` 





#### `public virtual void close()` 





#### `public virtual int send(`[`Message`](#classscy_1_1smpl_1_1Message)` & message,bool ack)` 



Send a message. May be a polymorphic [Command](#classscy_1_1smpl_1_1Command), [Presence](#classscy_1_1smpl_1_1Presence), [Event](#classscy_1_1smpl_1_1Event) or other ...

#### `public virtual int send(const std::string & message,bool ack)` 



Send a string message. The message must be a valid Symple message otherwise it will not be delivered.

#### `public `[`sockio::Transaction`](#structscy_1_1sockio_1_1Transaction)` * createTransaction(`[`Message`](#classscy_1_1smpl_1_1Message)` & message)` 



Create a Transaction object with the given message which will notify on Ack response from the server.

#### `public virtual int respond(`[`Message`](#classscy_1_1smpl_1_1Message)` & message,bool ack)` 

Swap the 'to' and 'from' fields and send the given message.



#### `public virtual int sendPresence(bool probe)` 



Broadcast presence to the user group scope. The outgoing [Presence](#classscy_1_1smpl_1_1Presence) object may be modified via the CreatePresence signal.

#### `public virtual int sendPresence(const `[`Address`](#structscy_1_1smpl_1_1Address)` & to,bool probe)` 

Send directed presence to the given peer.



#### `public virtual int joinRoom(const std::string & room)` 

Join the given room.



#### `public virtual int leaveRoom(const std::string & room)` 

Leave the given room.



#### `public virtual std::string ourID() const` 



Return the session ID of our current peer object. Return an empty string when offline.

#### `public StringVec rooms() const` 

Return a list of rooms the client has joined.



#### `public virtual `[`Peer`](#classscy_1_1smpl_1_1Peer)` * ourPeer()` 



Return the peer object for the current session, or throws an exception when offline.

#### `public virtual `[`Roster`](#classscy_1_1smpl_1_1Roster)` & roster()` 

Return the roster which stores all online peers.



#### `public virtual `[`PersistenceT`](#classscy_1_1TimedManager)` & persistence()` 



Return the persistence manager which stores long lived messages.

#### `public `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options()` 

Return a reference to the client options object.



#### `public virtual `[`Client`](#classscy_1_1smpl_1_1Client)` & operator>>(`[`Message`](#classscy_1_1smpl_1_1Message)` & message)` 

[Stream](#classscy_1_1Stream) operator alias for [send()](#group__symple_1ga4dffb71d16288852dbeb54e38ce9cc82).



#### `public virtual void onPresenceData(const json::value & data,bool whiny)` 

Update the roster from the given client object.



#### `protected `[`Roster`](./doc/api-symple.md#classscy_1_1smpl_1_1Roster)` _roster` 





#### `protected std::string _ourID` 





#### `protected `[`PersistenceT`](./doc/api-util.md#classscy_1_1TimedManager)` _persistence` 





#### `protected `[`Client::Options`](./doc/api-symple.md#structscy_1_1smpl_1_1Client_1_1Options)` _options` 





#### `protected StringVec _rooms` 





#### `protected int _announceStatus` 





#### `protected virtual int announce()` 



Called when a new connection is established to announce and authenticate the peer on the server.

#### `protected virtual void reset()` 



Resets variables and data at the beginning and end of each session.

#### `protected virtual void createPresence(`[`Presence`](#classscy_1_1smpl_1_1Presence)` & p)` 

Creates a [Presence](#classscy_1_1smpl_1_1Presence) object.



#### `protected virtual void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 

Override PacketSignal::emit.



#### `protected virtual void onOnline()` 





#### `protected virtual void onAnnounceState(void * sender,`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` 





# class `scy::smpl::Command` 

```
class scy::smpl::Command
  : public scy::smpl::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Command()` | 
`public  Command(const json::value & root)` | 
`public  Command(const `[`Command`](#classscy_1_1smpl_1_1Command)` & root)` | 
`public virtual  ~Command()` | 
`public std::string node() const` | 
`public std::string action() const` | 
`public void setNode(const std::string & node)` | 
`public void setAction(const std::string & action)` | 
`public virtual bool valid() const` | 
`public std::string param(int n) const` | 
`public std::vector< std::string > params()` | 
`public bool matches(const std::string & xnode) const` | 

## Members

#### `public  Command()` 





#### `public  Command(const json::value & root)` 





#### `public  Command(const `[`Command`](#classscy_1_1smpl_1_1Command)` & root)` 





#### `public virtual  ~Command()` 





#### `public std::string node() const` 





#### `public std::string action() const` 





#### `public void setNode(const std::string & node)` 





#### `public void setAction(const std::string & action)` 





#### `public virtual bool valid() const` 





#### `public std::string param(int n) const` 





#### `public std::vector< std::string > params()` 





#### `public bool matches(const std::string & xnode) const` 





# class `scy::smpl::Event` 

```
class scy::smpl::Event
  : public scy::smpl::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Event()` | 
`public  Event(const json::value & root)` | 
`public  Event(const `[`Event`](#classscy_1_1smpl_1_1Event)` & root)` | 
`public virtual  ~Event()` | 
`public virtual bool valid() const` | 
`public std::string name() const` | 
`public std::time_t time() const` | 
`public void setName(const std::string & name)` | 
`public void setTime(std::time_t time)` | 

## Members

#### `public  Event()` 





#### `public  Event(const json::value & root)` 





#### `public  Event(const `[`Event`](#classscy_1_1smpl_1_1Event)` & root)` 





#### `public virtual  ~Event()` 





#### `public virtual bool valid() const` 





#### `public std::string name() const` 





#### `public std::time_t time() const` 





#### `public void setName(const std::string & name)` 





#### `public void setTime(std::time_t time)` 





# class `scy::smpl::Form` 

```
class scy::smpl::Form
  : public scy::smpl::FormElement
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Form()` | 
`public  Form(json::value & root)` | 
`public  Form(`[`Command`](#classscy_1_1smpl_1_1Command)` & root)` | 
`public virtual  ~Form()` | 
`public bool valid()` | 
`public std::string action() const` | 
`public bool partial() const` | 
`public void setAction(const std::string & action)` | 
`public void setPartial(bool flag)` | 

## Members

#### `public  Form()` 





#### `public  Form(json::value & root)` 





#### `public  Form(`[`Command`](#classscy_1_1smpl_1_1Command)` & root)` 





#### `public virtual  ~Form()` 





#### `public bool valid()` 





#### `public std::string action() const` 





#### `public bool partial() const` 





#### `public void setAction(const std::string & action)` 



Possible "action" values `form` The form-processing entity is asking the form-submitting entity to complete a form. `submit` The form-submitting entity is submitting data to the form-processing entity. `cancel` The form-submitting entity has cancelled submission of data to the form-processing entity. `result` The form-processing entity is returning data to the form-submitting entity.

#### `public void setPartial(bool flag)` 



Notifies the form is a partial section of the form. This is used for transmitting and updating live fields such as for auto-complete.

# class `scy::smpl::FormElement` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FormElement()` | 
`public  FormElement(json::value & root,const std::string & type,const std::string & id,const std::string & label)` | 
`public  FormElement(const `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & r)` | 
`public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & operator=(const `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & r)` | 
`public virtual  ~FormElement()` | 
`public std::string type() const` | 
`public std::string id() const` | 
`public std::string label() const` | 
`public void setType(const std::string & type)` | 
`public void setId(const std::string & id)` | 
`public void setLabel(const std::string & text)` | 
`public void setHint(const std::string & text)` | 
`public void setError(const std::string & error)` | Sets and optional validation error message.
`public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` addPage(const std::string & id,const std::string & label)` | 
`public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` addSection(const std::string & id,const std::string & label)` | 
`public `[`FormField`](#classscy_1_1smpl_1_1FormField)` addField(const std::string & type,const std::string & id,const std::string & label)` | 
`public `[`FormField`](#classscy_1_1smpl_1_1FormField)` getField(const std::string & id,bool partial)` | 
`public bool getField(const std::string & id,`[`FormField`](#classscy_1_1smpl_1_1FormField)` & field,bool partial)` | 
`public bool hasField(const std::string & id,bool partial)` | 
`public void setLive(bool flag)` | 
`public bool live() const` | 
`public bool clearElements(const std::string & id,bool partial)` | Clears child elements matching the given ID.
`public void clear()` | Clear the entire form.
`public bool valid() const` | Returns true if the form element is valid.
`public int numElements()` | Returns the number of child elements.
`public bool hasErrors()` | Returns true if any fields have errors.
`public bool hasPages()` | Returns true if the form has multiple pages.
`public json::value & root() const` | 
`protected json::value * _root` | 

## Members

#### `public  FormElement()` 





#### `public  FormElement(json::value & root,const std::string & type,const std::string & id,const std::string & label)` 





#### `public  FormElement(const `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & r)` 





#### `public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & operator=(const `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & r)` 





#### `public virtual  ~FormElement()` 





#### `public std::string type() const` 





#### `public std::string id() const` 





#### `public std::string label() const` 





#### `public void setType(const std::string & type)` 



Possible "type" values page, section, text, text-multi, list, list-multi, checkbox, media, custom

#### `public void setId(const std::string & id)` 





#### `public void setLabel(const std::string & text)` 





#### `public void setHint(const std::string & text)` 





#### `public void setError(const std::string & error)` 

Sets and optional validation error message.



#### `public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` addPage(const std::string & id,const std::string & label)` 





#### `public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` addSection(const std::string & id,const std::string & label)` 





#### `public `[`FormField`](#classscy_1_1smpl_1_1FormField)` addField(const std::string & type,const std::string & id,const std::string & label)` 





#### `public `[`FormField`](#classscy_1_1smpl_1_1FormField)` getField(const std::string & id,bool partial)` 





#### `public bool getField(const std::string & id,`[`FormField`](#classscy_1_1smpl_1_1FormField)` & field,bool partial)` 





#### `public bool hasField(const std::string & id,bool partial)` 



Returns true if the given [Address](#structscy_1_1smpl_1_1Address) matches any of the internal form element IDs. If the partial flag is set then substring matches will be counted.

#### `public void setLive(bool flag)` 



Live fields or elements are used to submit partial sections a form, without sending the entire form.

#### `public bool live() const` 



Returns true if this field is live, meaning the form-processing entity should auto-update this field's value whenever it changes.

#### `public bool clearElements(const std::string & id,bool partial)` 

Clears child elements matching the given ID.



#### `public void clear()` 

Clear the entire form.



#### `public bool valid() const` 

Returns true if the form element is valid.



#### `public int numElements()` 

Returns the number of child elements.



#### `public bool hasErrors()` 

Returns true if any fields have errors.



#### `public bool hasPages()` 

Returns true if the form has multiple pages.



#### `public json::value & root() const` 





#### `protected json::value * _root` 



The root pointer is just a reference to the externally managed JSON value memory.

# class `scy::smpl::FormField` 

```
class scy::smpl::FormField
  : public scy::smpl::FormElement
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FormField()` | 
`public  FormField(json::value & root,const std::string & type,const std::string & id,const std::string & label)` | 
`public virtual  ~FormField()` | 
`public void addOption(const std::string & key,const std::string & value)` | Adds an option for list based fields.
`public void addOption(const std::string & value)` | 
`public void setValue(const std::string & value)` | Sets the value clearing all other values.
`public void setValue(int value)` | 
`public void setValue(double value)` | 
`public void setValue(bool value)` | 
`public void addValue(const std::string & value)` | Appends a value to the value array.
`public void addValue(int value)` | 
`public void addValue(double value)` | 
`public void addValue(bool value)` | 
`public json::value & values()` | Returns a JSON array of all values.
`public std::string value() const` | 
`public int intValue() const` | 
`public double doubleValue() const` | 
`public bool boolValue() const` | 

## Members

#### `public  FormField()` 





#### `public  FormField(json::value & root,const std::string & type,const std::string & id,const std::string & label)` 





#### `public virtual  ~FormField()` 





#### `public void addOption(const std::string & key,const std::string & value)` 

Adds an option for list based fields.



#### `public void addOption(const std::string & value)` 





#### `public void setValue(const std::string & value)` 

Sets the value clearing all other values.



#### `public void setValue(int value)` 





#### `public void setValue(double value)` 





#### `public void setValue(bool value)` 





#### `public void addValue(const std::string & value)` 

Appends a value to the value array.



#### `public void addValue(int value)` 





#### `public void addValue(double value)` 





#### `public void addValue(bool value)` 





#### `public json::value & values()` 

Returns a JSON array of all values.



#### `public std::string value() const` 



Returns the first value. Most formats (except multi) only require single value.

#### `public int intValue() const` 





#### `public double doubleValue() const` 





#### `public bool boolValue() const` 





# class `scy::smpl::Message` 

```
class scy::smpl::Message
  : public value
  : public scy::IPacket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Message()` | 
`public  Message(const json::value & root)` | 
`public  Message(const `[`Message`](#classscy_1_1smpl_1_1Message)` & root)` | 
`public virtual  ~Message()` | 
`public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public virtual bool valid() const` | 
`public virtual void clear()` | 
`public virtual void clearData()` | 
`public virtual void clearNotes()` | 
`public std::string type() const` | 
`public std::string id() const` | 
`public `[`Address`](#structscy_1_1smpl_1_1Address)` to() const` | 
`public `[`Address`](#structscy_1_1smpl_1_1Address)` from() const` | 
`public int status() const` | 
`public void setType(const std::string & type)` | 
`public void setTo(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & to)` | 
`public void setTo(const `[`Address`](#structscy_1_1smpl_1_1Address)` & to)` | 
`public void setTo(const std::string & to)` | 
`public void setFrom(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & from)` | 
`public void setFrom(const `[`Address`](#structscy_1_1smpl_1_1Address)` & from)` | 
`public void setFrom(const std::string & from)` | 
`public void setStatus(int code)` | 
`public json::value & notes()` | 
`public void setNote(const std::string & type,const std::string & text)` | 
`public void addNote(const std::string & type,const std::string & text)` | 
`public json::value data(const std::string & name) const` | 
`public json::value & data(const std::string & name)` | 
`public json::value & setData(const std::string & name)` | 
`public void setData(const std::string & name,const char * data)` | 
`public void setData(const std::string & name,const std::string & data)` | 
`public void setData(const std::string & name,const json::value & data)` | 
`public void setData(const std::string & name,int data)` | 
`public void removeData(const std::string & name)` | 
`public bool hasData(const std::string & name)` | 
`public virtual ssize_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public virtual ssize_t read(const std::string & root)` | 
`public virtual void write(`[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` &) const` | 
`public bool isRequest() const` | 
`public virtual size_t size() const` | 
`public virtual void print(std::ostream & os) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public  Message()` 





#### `public  Message(const json::value & root)` 





#### `public  Message(const `[`Message`](#classscy_1_1smpl_1_1Message)` & root)` 





#### `public virtual  ~Message()` 





#### `public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public virtual bool valid() const` 





#### `public virtual void clear()` 





#### `public virtual void clearData()` 





#### `public virtual void clearNotes()` 





#### `public std::string type() const` 





#### `public std::string id() const` 





#### `public `[`Address`](#structscy_1_1smpl_1_1Address)` to() const` 





#### `public `[`Address`](#structscy_1_1smpl_1_1Address)` from() const` 





#### `public int status() const` 





#### `public void setType(const std::string & type)` 





#### `public void setTo(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & to)` 





#### `public void setTo(const `[`Address`](#structscy_1_1smpl_1_1Address)` & to)` 





#### `public void setTo(const std::string & to)` 





#### `public void setFrom(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & from)` 





#### `public void setFrom(const `[`Address`](#structscy_1_1smpl_1_1Address)` & from)` 





#### `public void setFrom(const std::string & from)` 





#### `public void setStatus(int code)` 



HTTP status codes are used to describe the message response. 
**See also**: [http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html](http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html)

#### `public json::value & notes()` 





#### `public void setNote(const std::string & type,const std::string & text)` 



Set a note for the application Possible "type" values: `info`, `warn`, `error`

#### `public void addNote(const std::string & type,const std::string & text)` 





#### `public json::value data(const std::string & name) const` 





#### `public json::value & data(const std::string & name)` 





#### `public json::value & setData(const std::string & name)` 





#### `public void setData(const std::string & name,const char * data)` 





#### `public void setData(const std::string & name,const std::string & data)` 





#### `public void setData(const std::string & name,const json::value & data)` 





#### `public void setData(const std::string & name,int data)` 





#### `public void removeData(const std::string & name)` 





#### `public bool hasData(const std::string & name)` 





#### `public virtual ssize_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public virtual ssize_t read(const std::string & root)` 





#### `public virtual void write(`[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

#### `public bool isRequest() const` 





#### `public virtual size_t size() const` 



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__symple_1gac31e3c36ff63b87942058867a282fa29), but may not be the number of bytes that will be consumed by [read()](#group__symple_1ga41bc3235b35ef5ade9c5bc5e639701b9).

#### `public virtual void print(std::ostream & os) const` 





#### `public inline virtual const char * className() const` 





# class `scy::smpl::Peer` 

```
class scy::smpl::Peer
  : public value
```  



This class represents a peer on the network. A [Peer](#classscy_1_1smpl_1_1Peer) object may also contain arbitrary data set by the client to share with other peers on the network. 
**See also**: [Address](#structscy_1_1smpl_1_1Address) for further methods and [basic](#namespacescy_1_1basic) accessors.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Peer()` | 
`public  Peer(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & r)` | 
`public  Peer(const json::value & r)` | 
`public virtual  ~Peer()` | 
`public `[`Address`](#structscy_1_1smpl_1_1Address)` address() const` | 
`public std::string id() const` | 
`public std::string user() const` | 
`public std::string name() const` | 
`public std::string type() const` | 
`public std::string host() const` | 
`public void setID(const std::string & id)` | 
`public void setUser(const std::string & user)` | 
`public void setName(const std::string & name)` | 
`public void setType(const std::string & type)` | 
`public void setHost(const std::string & host)` | 
`public virtual bool valid()` | Updates the peer from the given data object.
`public void print(std::ostream & os) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public  Peer()` 





#### `public  Peer(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & r)` 





#### `public  Peer(const json::value & r)` 





#### `public virtual  ~Peer()` 





#### `public `[`Address`](#structscy_1_1smpl_1_1Address)` address() const` 





#### `public std::string id() const` 





#### `public std::string user() const` 





#### `public std::string name() const` 





#### `public std::string type() const` 





#### `public std::string host() const` 





#### `public void setID(const std::string & id)` 





#### `public void setUser(const std::string & user)` 





#### `public void setName(const std::string & name)` 





#### `public void setType(const std::string & type)` 





#### `public void setHost(const std::string & host)` 





#### `public virtual bool valid()` 

Updates the peer from the given data object.



#### `public void print(std::ostream & os) const` 





#### `public inline virtual const char * className() const` 





# class `scy::smpl::Presence` 

```
class scy::smpl::Presence
  : public scy::smpl::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Presence()` | 
`public  Presence(const json::value & root)` | 
`public  Presence(const `[`Presence`](#classscy_1_1smpl_1_1Presence)` & root)` | 
`public virtual  ~Presence()` | 
`public bool isProbe()` | 
`public void setProbe(bool flag)` | 

## Members

#### `public  Presence()` 





#### `public  Presence(const json::value & root)` 





#### `public  Presence(const `[`Presence`](#classscy_1_1smpl_1_1Presence)` & root)` 





#### `public virtual  ~Presence()` 





#### `public bool isProbe()` 





#### `public void setProbe(bool flag)` 





# class `scy::smpl::Roster` 

```
class scy::smpl::Roster
  : public scy::LiveCollection< std::string, Peer >
```  



The [Roster](#classscy_1_1smpl_1_1Roster) provides a registry for active network peers indexed by session ID.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Roster()` | 
`public virtual  ~Roster()` | 
`public `[`Peer`](#classscy_1_1smpl_1_1Peer)` * getByHost(const std::string & host)` | Returns the first peer which matches the given host address.
`public virtual PeerMap peers() const` | 
`public virtual void print(std::ostream & os) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public  Roster()` 





#### `public virtual  ~Roster()` 





#### `public `[`Peer`](#classscy_1_1smpl_1_1Peer)` * getByHost(const std::string & host)` 

Returns the first peer which matches the given host address.



#### `public virtual PeerMap peers() const` 





#### `public virtual void print(std::ostream & os) const` 





#### `public inline virtual const char * className() const` 





# class `scy::smpl::SSLClient` 

```
class scy::smpl::SSLClient
  : public scy::smpl::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SSLClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  SSLClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` 





# class `scy::smpl::TCPClient` 

```
class scy::smpl::TCPClient
  : public scy::smpl::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TCPClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  TCPClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` 





# struct `scy::smpl::Address` 




The [Address](#structscy_1_1smpl_1_1Address) structure is an endpoint identifier for a peer on the network. The format is like so: user|id

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string user` | 
`public std::string id` | 
`public  Address()` | 
`public  Address(const std::string & addr)` | 
`public  Address(const std::string & user,const std::string & id)` | 
`public virtual  ~Address()` | 
`public bool parse(const std::string & addr)` | 
`public bool valid() const` | 
`public void print(std::ostream & os) const` | 
`public std::string toString() const` | 
`public bool operator==(const `[`Address`](#structscy_1_1smpl_1_1Address)` & r)` | 
`public bool operator==(std::string & r)` | 

## Members

#### `public std::string user` 





#### `public std::string id` 





#### `public  Address()` 





#### `public  Address(const std::string & addr)` 





#### `public  Address(const std::string & user,const std::string & id)` 





#### `public virtual  ~Address()` 





#### `public bool parse(const std::string & addr)` 





#### `public bool valid() const` 





#### `public void print(std::ostream & os) const` 





#### `public std::string toString() const` 





#### `public bool operator==(const `[`Address`](#structscy_1_1smpl_1_1Address)` & r)` 





#### `public bool operator==(std::string & r)` 





# struct `scy::smpl::Filter` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Bitwise`](./doc/api-base.md#structscy_1_1Bitwise)` flags` | 
`public std::string path` | 
`public inline  Filter(const std::string & path,unsigned flags)` | 
`public inline  Filter(unsigned flags)` | 

## Members

#### `public `[`Bitwise`](./doc/api-base.md#structscy_1_1Bitwise)` flags` 





#### `public std::string path` 





#### `public inline  Filter(const std::string & path,unsigned flags)` 





#### `public inline  Filter(unsigned flags)` 





# struct `scy::smpl::Client::Options` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string host` | 
`public uint16_t port` | 
`public bool reconnection` | Weather or not to reconnect if disconnected from the server.
`public int reconnectAttempts` | 
`public std::string user` | 
`public std::string name` | 
`public std::string type` | 
`public std::string token` | 
`public inline  Options()` | 

## Members

#### `public std::string host` 





#### `public uint16_t port` 





#### `public bool reconnection` 

Weather or not to reconnect if disconnected from the server.



#### `public int reconnectAttempts` 



The number of times to attempt to reconnect if disconnected from the server. (0 = unlimited)

#### `public std::string user` 





#### `public std::string name` 





#### `public std::string type` 





#### `public std::string token` 





#### `public inline  Options()` 





