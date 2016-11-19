# Module <!-- group --> `symple`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::smpl`](#namespacescy_1_1smpl)    | 
`struct `[`scy::smpl::Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)    | 
# namespace `scy::smpl` {#namespacescy_1_1smpl}



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
# class `scy::smpl::Client` {#classscy_1_1smpl_1_1Client}

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
`public virtual void onPresenceData(const json::Value & data,bool whiny)` | Update the roster from the given client object.
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

#### `public `[`Signal`](#classscy_1_1Signal)< void(const int &)`> Announce` {#group__symple_1ga0228d93d0cedc21b61f12a580bb7c6b9}



SignalsNotifies the outside application about the response status code of our [announce()](#group__symple_1gadf0ac053e026a7c792d00ac165cb1298) call. Possible status codes are: // - 200: Authentication success

* 401: Authentication failed


* 400: Bad request data


* 500: Server not found

#### `public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> PeerConnected` {#group__symple_1ga01d918d49dcf259d612543a353a7e98e}

Signals when a peer connects.



#### `public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> PeerDisconnected` {#group__symple_1ga317fcf03f379f086db4b77bbc58ba7f6}

Signals when a peer disconnects.



#### `public `[`Signal](#classscy_1_1Signal)< void([Peer`](#classscy_1_1smpl_1_1Peer) &)`> CreatePresence` {#group__symple_1ga026c3952b78bd1b4b4f18d9bd815e94e}



Called by [createPresence()](#group__symple_1gace2ee9e3f211f1fb9979953c6d0dfa34) so outside classes can modify the outgoing [Peer](#classscy_1_1smpl_1_1Peer) JSON object.

#### `public  Client(const net::Socket::Ptr & socket,const `[`Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options)` {#group__symple_1ga5b46d09313a3360148246158e11dce4c}





#### `public virtual  ~Client()` {#group__symple_1ga1eb6bbd30e54e4c0e918c7ce43fadeec}





#### `public virtual void connect()` {#group__symple_1gaa8c97bf06d3fc80ceea252a2a611c5c7}





#### `public virtual void close()` {#group__symple_1gae27e4556310a4b935ffe78b62bbe3a9c}





#### `public virtual int send(`[`Message`](#classscy_1_1smpl_1_1Message)` & message,bool ack)` {#group__symple_1ga4dffb71d16288852dbeb54e38ce9cc82}



Send a message. May be a polymorphic [Command](#classscy_1_1smpl_1_1Command), [Presence](#classscy_1_1smpl_1_1Presence), [Event](#classscy_1_1smpl_1_1Event) or other ...

#### `public virtual int send(const std::string & message,bool ack)` {#group__symple_1ga6e121b7e49c36023f49190c9ab5bebef}



Send a string message. The message must be a valid Symple message otherwise it will not be delivered.

#### `public `[`sockio::Transaction`](#structscy_1_1sockio_1_1Transaction)` * createTransaction(`[`Message`](#classscy_1_1smpl_1_1Message)` & message)` {#group__symple_1ga51add0f918bee39b510fad050c973db9}



Create a Transaction object with the given message which will notify on Ack response from the server.

#### `public virtual int respond(`[`Message`](#classscy_1_1smpl_1_1Message)` & message,bool ack)` {#group__symple_1gaea0ec59bcf35da1f20a3cd46adf1e4f4}

Swap the 'to' and 'from' fields and send the given message.



#### `public virtual int sendPresence(bool probe)` {#group__symple_1gaa2b2c55f34473f05b397c88e6a1f1e19}



Broadcast presence to the user group scope. The outgoing [Presence](#classscy_1_1smpl_1_1Presence) object may be modified via the CreatePresence signal.

#### `public virtual int sendPresence(const `[`Address`](#structscy_1_1smpl_1_1Address)` & to,bool probe)` {#group__symple_1gaaf17806e06f596c196e007c5e98a0ea6}

Send directed presence to the given peer.



#### `public virtual int joinRoom(const std::string & room)` {#group__symple_1ga426afee6e4914560d5b82fb9098898d6}

Join the given room.



#### `public virtual int leaveRoom(const std::string & room)` {#group__symple_1ga7d696ae2396d9da0d87c7d0ed464b579}

Leave the given room.



#### `public virtual std::string ourID() const` {#group__symple_1gaeb1cebadd1f004d6b259c4831ff0901a}



Return the session ID of our current peer object. Return an empty string when offline.

#### `public StringVec rooms() const` {#group__symple_1ga468d33d5531670856a74208ecca2e8f6}

Return a list of rooms the client has joined.



#### `public virtual `[`Peer`](#classscy_1_1smpl_1_1Peer)` * ourPeer()` {#group__symple_1ga680bfb6b0db4f75e9a863b61b61d734b}



Return the peer object for the current session, or throws an exception when offline.

#### `public virtual `[`Roster`](#classscy_1_1smpl_1_1Roster)` & roster()` {#group__symple_1ga6998f1375a4767e630a98f60cb304300}

Return the roster which stores all online peers.



#### `public virtual `[`PersistenceT`](#classscy_1_1TimedManager)` & persistence()` {#group__symple_1ga7ee28acdc40837302c167b543fc56a15}



Return the persistence manager which stores long lived messages.

#### `public `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options()` {#group__symple_1ga6272271fea7d57a5719e2da9163b3224}

Return a reference to the client options object.



#### `public virtual `[`Client`](#classscy_1_1smpl_1_1Client)` & operator>>(`[`Message`](#classscy_1_1smpl_1_1Message)` & message)` {#group__symple_1ga991c5b0bbcca7d16536b0dccce4107d2}

[Stream](#classscy_1_1Stream) operator alias for [send()](#group__symple_1ga4dffb71d16288852dbeb54e38ce9cc82).



#### `public virtual void onPresenceData(const json::Value & data,bool whiny)` {#group__symple_1gab77c005a2749fe63631b1737904ab165}

Update the roster from the given client object.



#### `protected `[`Roster`](./doc/api-symple.md#classscy_1_1smpl_1_1Roster)` _roster` {#group__symple_1ga8a6dc184e85eb02e1180a6ff11af402f}





#### `protected std::string _ourID` {#group__symple_1ga7aa335a7730f803fabab6a0ac7bf629f}





#### `protected `[`PersistenceT`](./doc/api-util.md#classscy_1_1TimedManager)` _persistence` {#group__symple_1ga0ecdbbbdc4f57cac6e7b5fda9072f5fa}





#### `protected `[`Client::Options`](./doc/api-symple.md#structscy_1_1smpl_1_1Client_1_1Options)` _options` {#group__symple_1gadfb98483f4d159dc718c788f787d6adf}





#### `protected StringVec _rooms` {#group__symple_1ga362e9d47266c26dcf32e1603e6d39a85}





#### `protected int _announceStatus` {#group__symple_1gacece49388fbe130519f6b58232dd8067}





#### `protected virtual int announce()` {#group__symple_1gadf0ac053e026a7c792d00ac165cb1298}



Called when a new connection is established to announce and authenticate the peer on the server.

#### `protected virtual void reset()` {#group__symple_1gafe1d7b3e16488e4a61f6fc045601dc2a}



Resets variables and data at the beginning and end of each session.

#### `protected virtual void createPresence(`[`Presence`](#classscy_1_1smpl_1_1Presence)` & p)` {#group__symple_1gace2ee9e3f211f1fb9979953c6d0dfa34}

Creates a [Presence](#classscy_1_1smpl_1_1Presence) object.



#### `protected virtual void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__symple_1gae43d7d98d83566304f5de257d0049190}

Override PacketSignal::emit.



#### `protected virtual void onOnline()` {#group__symple_1gaf47e8e5c1090b3b5f5e53e0de3dbfda6}





#### `protected virtual void onAnnounceState(void * sender,`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` {#group__symple_1gae30976d64dc3e2b64b470eb0774fdf16}





# class `scy::smpl::Command` {#classscy_1_1smpl_1_1Command}

```
class scy::smpl::Command
  : public scy::smpl::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Command()` | 
`public  Command(const json::Value & root)` | 
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

#### `public  Command()` {#group__symple_1ga1d83f2b5a9be1780e3b77f5c96b34a9a}





#### `public  Command(const json::Value & root)` {#group__symple_1ga516ba951f279e5d4466da4eb1720006c}





#### `public  Command(const `[`Command`](#classscy_1_1smpl_1_1Command)` & root)` {#group__symple_1ga2f980c97f7fdfee4882f8923cb2853c2}





#### `public virtual  ~Command()` {#group__symple_1ga964d5a75ca62c76ff2c27e9236f90e00}





#### `public std::string node() const` {#group__symple_1gac5f9bb1710c74e1f8281c267ecf04c5f}





#### `public std::string action() const` {#group__symple_1ga81df2719cd693fbda9e76dff7027a654}





#### `public void setNode(const std::string & node)` {#group__symple_1ga904c2957be56206f513c33f92f89bcad}





#### `public void setAction(const std::string & action)` {#group__symple_1ga276a75d047efd959d77cfe473f8a1c58}





#### `public virtual bool valid() const` {#group__symple_1ga6047e80d0c1118c1ac2f077b3e43426b}





#### `public std::string param(int n) const` {#group__symple_1ga1611d5b3bdcc91574bae67dfbde437ae}





#### `public std::vector< std::string > params()` {#group__symple_1ga43384ed4f7303ac921bcf65e6f18db26}





#### `public bool matches(const std::string & xnode) const` {#group__symple_1gae80a575c75e000c040d320ead0896323}





# class `scy::smpl::Event` {#classscy_1_1smpl_1_1Event}

```
class scy::smpl::Event
  : public scy::smpl::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Event()` | 
`public  Event(const json::Value & root)` | 
`public  Event(const `[`Event`](#classscy_1_1smpl_1_1Event)` & root)` | 
`public virtual  ~Event()` | 
`public virtual bool valid() const` | 
`public std::string name() const` | 
`public std::time_t time() const` | 
`public void setName(const std::string & name)` | 
`public void setTime(std::time_t time)` | 

## Members

#### `public  Event()` {#group__symple_1gafe163cd025c2c0dafa7e779df5f5962d}





#### `public  Event(const json::Value & root)` {#group__symple_1ga6907e4b6efcb9e87a715eccda36cd79a}





#### `public  Event(const `[`Event`](#classscy_1_1smpl_1_1Event)` & root)` {#group__symple_1gae6aaee01e127e7de476f49d0ecb9f8c7}





#### `public virtual  ~Event()` {#group__symple_1ga9b9bb5b13b492ec09f88e72249a83e3b}





#### `public virtual bool valid() const` {#group__symple_1ga365f48210b899bf487ac334a6b50bb6a}





#### `public std::string name() const` {#group__symple_1gaf4898557f256d37c2ec91dae453ba244}





#### `public std::time_t time() const` {#group__symple_1ga3f9805f507320d2a864fe81eaac155b5}





#### `public void setName(const std::string & name)` {#group__symple_1ga97e213f54bcc541345da6867f3f083ed}





#### `public void setTime(std::time_t time)` {#group__symple_1gab3e4b9fa8d12289de33477c9ac68330a}





# class `scy::smpl::Form` {#classscy_1_1smpl_1_1Form}

```
class scy::smpl::Form
  : public scy::smpl::FormElement
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Form()` | 
`public  Form(json::Value & root)` | 
`public  Form(`[`Command`](#classscy_1_1smpl_1_1Command)` & root)` | 
`public virtual  ~Form()` | 
`public bool valid()` | 
`public std::string action() const` | 
`public bool partial() const` | 
`public void setAction(const std::string & action)` | 
`public void setPartial(bool flag)` | 

## Members

#### `public  Form()` {#group__symple_1ga68e4093955870a7823a1a0e2fb32781a}





#### `public  Form(json::Value & root)` {#group__symple_1ga2075d74a1acd059bae9bdab951d4c29f}





#### `public  Form(`[`Command`](#classscy_1_1smpl_1_1Command)` & root)` {#group__symple_1gaa8a3c6e4f9bca2257fdae351d7621a45}





#### `public virtual  ~Form()` {#group__symple_1ga78e6a83ae030945f05c990ea856d7281}





#### `public bool valid()` {#group__symple_1ga0259ae314e90190494940841638b4a0e}





#### `public std::string action() const` {#group__symple_1ga2974536cdcca932554f986ed1dbd7e58}





#### `public bool partial() const` {#group__symple_1ga25bd29f8f64828762cfeb0d44a2be46c}





#### `public void setAction(const std::string & action)` {#group__symple_1ga8bc4b676485d8e4e28da58b2f941acb9}



Possible "action" values `form` The form-processing entity is asking the form-submitting entity to complete a form. `submit` The form-submitting entity is submitting data to the form-processing entity. `cancel` The form-submitting entity has cancelled submission of data to the form-processing entity. `result` The form-processing entity is returning data to the form-submitting entity.

#### `public void setPartial(bool flag)` {#group__symple_1ga828e2ab5535b743420349e7bb1ff59f9}



Notifies the form is a partial section of the form. This is used for transmitting and updating live fields such as for auto-complete.

# class `scy::smpl::FormElement` {#classscy_1_1smpl_1_1FormElement}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FormElement()` | 
`public  FormElement(json::Value & root,const std::string & type,const std::string & id,const std::string & label)` | 
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
`public json::Value & root() const` | 
`protected json::Value * _root` | 

## Members

#### `public  FormElement()` {#group__symple_1ga7e047bc6b6e46abe01a946a82745ee11}





#### `public  FormElement(json::Value & root,const std::string & type,const std::string & id,const std::string & label)` {#group__symple_1ga7c687fb854685b8c190f54c25a6ff088}





#### `public  FormElement(const `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & r)` {#group__symple_1ga94f85597aa5b95ef7dcc246ef41787b9}





#### `public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & operator=(const `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` & r)` {#group__symple_1ga0d7c97e98878eb69ebd8830df6a01e58}





#### `public virtual  ~FormElement()` {#group__symple_1ga2c7bf72c2e0e6650fe27a68cd5e1beec}





#### `public std::string type() const` {#group__symple_1ga3b53f1858ddcc524004726f4d6a463b4}





#### `public std::string id() const` {#group__symple_1ga0eacc59669c12ecf4faf45395cc08f17}





#### `public std::string label() const` {#group__symple_1ga0bf79407646db8313d409ccc9a2f4ba8}





#### `public void setType(const std::string & type)` {#group__symple_1gae723f824695861e34898005f225e12b7}



Possible "type" values page, section, text, text-multi, list, list-multi, checkbox, media, custom

#### `public void setId(const std::string & id)` {#group__symple_1gaba4de61935c0d3795d537964f47c33d0}





#### `public void setLabel(const std::string & text)` {#group__symple_1ga52798e6759dd37b1116720a37a2c967d}





#### `public void setHint(const std::string & text)` {#group__symple_1ga1a9595540a1969bb17a3115be6e26df6}





#### `public void setError(const std::string & error)` {#group__symple_1gaf621f96173fcbe523d419fd4a3a87f3e}

Sets and optional validation error message.



#### `public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` addPage(const std::string & id,const std::string & label)` {#group__symple_1ga432f56acec954ed84897f7fc01059813}





#### `public `[`FormElement`](#classscy_1_1smpl_1_1FormElement)` addSection(const std::string & id,const std::string & label)` {#group__symple_1gab0619bb63ac7241e61c36f69bbc4ed07}





#### `public `[`FormField`](#classscy_1_1smpl_1_1FormField)` addField(const std::string & type,const std::string & id,const std::string & label)` {#group__symple_1ga474c9727e83a9ce0e0b206d246379d6c}





#### `public `[`FormField`](#classscy_1_1smpl_1_1FormField)` getField(const std::string & id,bool partial)` {#group__symple_1gac9715a4fffff97d29092698755242cab}





#### `public bool getField(const std::string & id,`[`FormField`](#classscy_1_1smpl_1_1FormField)` & field,bool partial)` {#group__symple_1gad7d9fdada931140ef706d58036afb0fe}





#### `public bool hasField(const std::string & id,bool partial)` {#group__symple_1gae28386fbc730fd60fea605384563dc1c}



Returns true if the given [Address](#structscy_1_1smpl_1_1Address) matches any of the internal form element IDs. If the partial flag is set then substring matches will be counted.

#### `public void setLive(bool flag)` {#group__symple_1ga88878fc79bdeed108b06703d29fab26b}



Live fields or elements are used to submit partial sections a form, without sending the entire form.

#### `public bool live() const` {#group__symple_1gace9f6fd60c701c643d660971c159ce43}



Returns true if this field is live, meaning the form-processing entity should auto-update this field's value whenever it changes.

#### `public bool clearElements(const std::string & id,bool partial)` {#group__symple_1gac2ee4183b9d5465c0036ee4a4c38e562}

Clears child elements matching the given ID.



#### `public void clear()` {#group__symple_1ga4cf4750e263530a525b89755b20677c6}

Clear the entire form.



#### `public bool valid() const` {#group__symple_1ga47cf107cf15ea93bdb9f377253f76aa6}

Returns true if the form element is valid.



#### `public int numElements()` {#group__symple_1gaae3b7577b27fb174619fc68801607ea1}

Returns the number of child elements.



#### `public bool hasErrors()` {#group__symple_1ga50aa0c3f85c44063771f28b75de0a428}

Returns true if any fields have errors.



#### `public bool hasPages()` {#group__symple_1ga61e65544fa1f0357bc0721864cc54768}

Returns true if the form has multiple pages.



#### `public json::Value & root() const` {#group__symple_1gaef747caa71c64bdacef798a8a471451a}





#### `protected json::Value * _root` {#group__symple_1ga2709396eb0a2cd3632e19864287706a7}



The root pointer is just a reference to the externally managed JSON value memory.

# class `scy::smpl::FormField` {#classscy_1_1smpl_1_1FormField}

```
class scy::smpl::FormField
  : public scy::smpl::FormElement
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FormField()` | 
`public  FormField(json::Value & root,const std::string & type,const std::string & id,const std::string & label)` | 
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
`public json::Value & values()` | Returns a JSON array of all values.
`public std::string value() const` | 
`public int intValue() const` | 
`public double doubleValue() const` | 
`public bool boolValue() const` | 

## Members

#### `public  FormField()` {#group__symple_1gaf9378e4d88099d33f5c73cd6e5301841}





#### `public  FormField(json::Value & root,const std::string & type,const std::string & id,const std::string & label)` {#group__symple_1gaf5b7e8d167d534dac2e6c722b16eea43}





#### `public virtual  ~FormField()` {#group__symple_1gac0c5aeeef91f25f93d981deff77382dd}





#### `public void addOption(const std::string & key,const std::string & value)` {#group__symple_1ga26777a113bae5c3083906e77b62d13f6}

Adds an option for list based fields.



#### `public void addOption(const std::string & value)` {#group__symple_1gaf3d1bf938ba0cbeade0da2a18543a35d}





#### `public void setValue(const std::string & value)` {#group__symple_1gaeee92fee4944e5013ebef43eee2b9183}

Sets the value clearing all other values.



#### `public void setValue(int value)` {#group__symple_1ga170d9a5ddc51953f406331b6da779bcf}





#### `public void setValue(double value)` {#group__symple_1gaa29f083206c8cbf9005f730f61b55dad}





#### `public void setValue(bool value)` {#group__symple_1gaee3ee4b29024eee4fc34e1dded5267b8}





#### `public void addValue(const std::string & value)` {#group__symple_1ga7cd9073b9e7c2a476f17104d6e9ce8fb}

Appends a value to the value array.



#### `public void addValue(int value)` {#group__symple_1ga89b16c8639181a01f676ea4ec1d48cbc}





#### `public void addValue(double value)` {#group__symple_1ga686348a5aacfc900b9a4d3d3aea903ad}





#### `public void addValue(bool value)` {#group__symple_1ga89eaf8cd95b3a7f081e30f154720e883}





#### `public json::Value & values()` {#group__symple_1ga5a793bf7d36b8f167c9f3749bf8f4fd6}

Returns a JSON array of all values.



#### `public std::string value() const` {#group__symple_1gae0051a3119d85c46034f1167a35fc9e5}



Returns the first value. Most formats (except multi) only require single value.

#### `public int intValue() const` {#group__symple_1ga6806b9902742ea0bfe06e2a4a3e18cb1}





#### `public double doubleValue() const` {#group__symple_1ga744a0a50084034c5179a92b6c5538956}





#### `public bool boolValue() const` {#group__symple_1ga6d0f293a15bce06dc455fc7535619eef}





# class `scy::smpl::Message` {#classscy_1_1smpl_1_1Message}

```
class scy::smpl::Message
  : public Value
  : public scy::IPacket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Message()` | 
`public  Message(const json::Value & root)` | 
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
`public json::Value & notes()` | 
`public void setNote(const std::string & type,const std::string & text)` | 
`public void addNote(const std::string & type,const std::string & text)` | 
`public json::Value data(const std::string & name) const` | 
`public json::Value & data(const std::string & name)` | 
`public json::Value & setData(const std::string & name)` | 
`public void setData(const std::string & name,const char * data)` | 
`public void setData(const std::string & name,const std::string & data)` | 
`public void setData(const std::string & name,const json::Value & data)` | 
`public void setData(const std::string & name,int data)` | 
`public void removeData(const std::string & name)` | 
`public bool hasData(const std::string & name)` | 
`public virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public virtual std::size_t read(const std::string & root)` | 
`public virtual void write(Buffer &) const` | 
`public bool isRequest() const` | 
`public virtual std::size_t size() const` | 
`public virtual void print(std::ostream & os) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public  Message()` {#group__symple_1ga993977c8fc55c8b8b8ec172b720df218}





#### `public  Message(const json::Value & root)` {#group__symple_1gaaf145f20ca306f76c0630d80b5ae9bd4}





#### `public  Message(const `[`Message`](#classscy_1_1smpl_1_1Message)` & root)` {#group__symple_1gad7ccee50283b43fb6203d5d38655b5a8}





#### `public virtual  ~Message()` {#group__symple_1ga6a2eb621f26dda23a4e00cf085e28733}





#### `public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__symple_1ga7772d48d3c40973f8ba273404efbc0f5}





#### `public virtual bool valid() const` {#group__symple_1ga6bc8ec508472b3a845517e37a7d9ecee}





#### `public virtual void clear()` {#group__symple_1gab6a00a852821506f2003ea5916e0937f}





#### `public virtual void clearData()` {#group__symple_1ga004941b724014d459a60784e9f638d62}





#### `public virtual void clearNotes()` {#group__symple_1ga2a2409a6b3bbff22b57394f3c64f232f}





#### `public std::string type() const` {#group__symple_1ga238a19ce026c212bbbb53242e22e0b35}





#### `public std::string id() const` {#group__symple_1ga36664179f4acfcacf616fdbae6f42cc3}





#### `public `[`Address`](#structscy_1_1smpl_1_1Address)` to() const` {#group__symple_1ga91b5760d36713eb890827f069f82b6c5}





#### `public `[`Address`](#structscy_1_1smpl_1_1Address)` from() const` {#group__symple_1ga97accfebe0c98bf39caa4f92159a52c9}





#### `public int status() const` {#group__symple_1gad93c255aca03896f8e91a360022521c8}





#### `public void setType(const std::string & type)` {#group__symple_1gad1ff57aeeaa0788b12714750c856aa97}





#### `public void setTo(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & to)` {#group__symple_1ga59aff14b0a347efc0d7cd13790aee564}





#### `public void setTo(const `[`Address`](#structscy_1_1smpl_1_1Address)` & to)` {#group__symple_1ga35ebeab4af091f299ea7813dec40b611}





#### `public void setTo(const std::string & to)` {#group__symple_1gaa95416e0a9ebdeae0311cfc2f5ec9a7a}





#### `public void setFrom(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & from)` {#group__symple_1ga883e4a76d045695cbeb1d95fd6b09a06}





#### `public void setFrom(const `[`Address`](#structscy_1_1smpl_1_1Address)` & from)` {#group__symple_1ga0121323c2d20f1ee7b6ec0b65949dd82}





#### `public void setFrom(const std::string & from)` {#group__symple_1gae951b7a32bf75402fc59b2eacd29e262}





#### `public void setStatus(int code)` {#group__symple_1gaaf9e802be207af079eb682439868720b}



HTTP status codes are used to describe the message response. 
**See also**: [http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html](http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html)

#### `public json::Value & notes()` {#group__symple_1ga40ae96f7dc5d2d18795713a4dcb9e011}





#### `public void setNote(const std::string & type,const std::string & text)` {#group__symple_1ga2a15db81f2ee696f70977047dc5c1388}



Set a note for the application Possible "type" values: `info`, `warn`, `error`

#### `public void addNote(const std::string & type,const std::string & text)` {#group__symple_1gabf23c5c1f2a42649d2c85ca0502bf7d5}





#### `public json::Value data(const std::string & name) const` {#group__symple_1gaa6c8e79ef5e09a08bd8259d46d482bd2}





#### `public json::Value & data(const std::string & name)` {#group__symple_1gab2f2bc1ea801887a429e8682178e8c45}





#### `public json::Value & setData(const std::string & name)` {#group__symple_1ga695363d392dd93a483a9863f2ce5966f}





#### `public void setData(const std::string & name,const char * data)` {#group__symple_1ga46b69d25c45569ff2c15f5c1982226c0}





#### `public void setData(const std::string & name,const std::string & data)` {#group__symple_1ga27601dfabf9f11b474046c765f01d0f6}





#### `public void setData(const std::string & name,const json::Value & data)` {#group__symple_1gab57e9355ebd1ce0bec13c7081716899f}





#### `public void setData(const std::string & name,int data)` {#group__symple_1gab75585a9f4e4555c0a7da64926a3a5fd}





#### `public void removeData(const std::string & name)` {#group__symple_1gaad4ce633c0a2f4be14c2dc34114aea14}





#### `public bool hasData(const std::string & name)` {#group__symple_1ga622a343fa5a0c26fe39d0a8822756d79}





#### `public virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` {#group__symple_1ga8e564e6db8aaeb93aeafc33c987c00f2}



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public virtual std::size_t read(const std::string & root)` {#group__symple_1ga6cbf166f853bb325ca55f6f91f3084e2}





#### `public virtual void write(Buffer &) const` {#group__symple_1gac31e3c36ff63b87942058867a282fa29}



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public bool isRequest() const` {#group__symple_1ga8893c0f725efc3aa1f42e0d8de208879}





#### `public virtual std::size_t size() const` {#group__symple_1gace4e8fa14b579336d0c4bcb0efee3a0f}



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__symple_1gac31e3c36ff63b87942058867a282fa29), but may not be the number of bytes that will be consumed by [read()](#group__symple_1ga8e564e6db8aaeb93aeafc33c987c00f2).

#### `public virtual void print(std::ostream & os) const` {#group__symple_1gaefb77bc8f9a03adc57f931d0b1d1e05d}





#### `public inline virtual const char * className() const` {#group__symple_1ga1762b3967c8aaa7a9a636e5d80c1cd6d}





# class `scy::smpl::Peer` {#classscy_1_1smpl_1_1Peer}

```
class scy::smpl::Peer
  : public Value
```  



This class represents a peer on the network. A [Peer](#classscy_1_1smpl_1_1Peer) object may also contain arbitrary data set by the client to share with other peers on the network. 
**See also**: [Address](#structscy_1_1smpl_1_1Address) for further methods and basic accessors.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Peer()` | 
`public  Peer(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & r)` | 
`public  Peer(const json::Value & r)` | 
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

#### `public  Peer()` {#group__symple_1ga21699ec274f5b4a4609fa1232c7c2759}





#### `public  Peer(const `[`Peer`](#classscy_1_1smpl_1_1Peer)` & r)` {#group__symple_1gaf1d1b51b210184552aa1e52ed9c1043f}





#### `public  Peer(const json::Value & r)` {#group__symple_1ga5385b739a9a490de6870d3181bf337f3}





#### `public virtual  ~Peer()` {#group__symple_1gab6b9fd40ad19b75f1476719938486687}





#### `public `[`Address`](#structscy_1_1smpl_1_1Address)` address() const` {#group__symple_1gae18b7a663a71100f65a480a309928274}





#### `public std::string id() const` {#group__symple_1ga477d474d5af82b4d59a762efa970bb1a}





#### `public std::string user() const` {#group__symple_1ga9731497468e46249d9640bb38637ebdd}





#### `public std::string name() const` {#group__symple_1ga52cc114ca838238810e3d554425194b7}





#### `public std::string type() const` {#group__symple_1gab1ff841dfeb0d69ac798ad12c55bf43d}





#### `public std::string host() const` {#group__symple_1gae1ac4cb0de0b4845a2f73403b3caa83e}





#### `public void setID(const std::string & id)` {#group__symple_1ga0901ffbe8ce396fb01d83553815bad54}





#### `public void setUser(const std::string & user)` {#group__symple_1ga6da2b366cdda5ed7dfaa11b69d20ef37}





#### `public void setName(const std::string & name)` {#group__symple_1gaae1a1e30ed070876d5dc7d124a99434e}





#### `public void setType(const std::string & type)` {#group__symple_1ga6b4ce3657cc8995ad02e7c51939f1d53}





#### `public void setHost(const std::string & host)` {#group__symple_1ga1bab6aa0690fff8ca6cbf8749fc0efd8}





#### `public virtual bool valid()` {#group__symple_1gad056870e5ed4649ca9d218c9cc6c8df3}

Updates the peer from the given data object.



#### `public void print(std::ostream & os) const` {#group__symple_1gaba929db58e4c8245ed8e0c61d7f6bc57}





#### `public inline virtual const char * className() const` {#group__symple_1ga4fad5617154d7c62279aa0bbc787da2c}





# class `scy::smpl::Presence` {#classscy_1_1smpl_1_1Presence}

```
class scy::smpl::Presence
  : public scy::smpl::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Presence()` | 
`public  Presence(const json::Value & root)` | 
`public  Presence(const `[`Presence`](#classscy_1_1smpl_1_1Presence)` & root)` | 
`public virtual  ~Presence()` | 
`public bool isProbe()` | 
`public void setProbe(bool flag)` | 

## Members

#### `public  Presence()` {#group__symple_1ga8e1bf8928c147286c6992a68ba000b22}





#### `public  Presence(const json::Value & root)` {#group__symple_1ga34417967f7d764866753df17eaf25800}





#### `public  Presence(const `[`Presence`](#classscy_1_1smpl_1_1Presence)` & root)` {#group__symple_1gaa1b5a011069c478b0cc808efa0b8e3d2}





#### `public virtual  ~Presence()` {#group__symple_1gae3237592f9d4e644c94eb9652b3ddd82}





#### `public bool isProbe()` {#group__symple_1ga2a03e7f09b7455a88ebc06bea78da377}





#### `public void setProbe(bool flag)` {#group__symple_1ga991b723ca51fbcc7d4b0a6e5ee9577ae}





# class `scy::smpl::Roster` {#classscy_1_1smpl_1_1Roster}

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

#### `public  Roster()` {#group__symple_1ga52cf6d7c73b07459e7231898cf3267c0}





#### `public virtual  ~Roster()` {#group__symple_1ga548c497780612733999913ad9be891a6}





#### `public `[`Peer`](#classscy_1_1smpl_1_1Peer)` * getByHost(const std::string & host)` {#group__symple_1ga2762fe4481b419996e1f6f8ca51a222d}

Returns the first peer which matches the given host address.



#### `public virtual PeerMap peers() const` {#group__symple_1ga82d3344e72e6d3cea1acee9929613076}





#### `public virtual void print(std::ostream & os) const` {#group__symple_1ga1938a8f8c5784b7cafd64225133a1bae}





#### `public inline virtual const char * className() const` {#group__symple_1gab1bf76602341bf0c187753aa5ea757a7}





# class `scy::smpl::SSLClient` {#classscy_1_1smpl_1_1SSLClient}

```
class scy::smpl::SSLClient
  : public scy::smpl::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SSLClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  SSLClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` {#group__symple_1ga03327af0bc01ae4e7a56affb83991aad}





# class `scy::smpl::TCPClient` {#classscy_1_1smpl_1_1TCPClient}

```
class scy::smpl::TCPClient
  : public scy::smpl::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TCPClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  TCPClient(const `[`Client::Options`](#structscy_1_1smpl_1_1Client_1_1Options)` & options,uv::Loop * loop)` {#group__symple_1gafc1541506c7fded90f495818835590b4}





# struct `scy::smpl::Address` {#structscy_1_1smpl_1_1Address}




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

#### `public std::string user` {#group__symple_1gab04caf4352786515fca8fce459a56931}





#### `public std::string id` {#group__symple_1gae830f59b90e94397743b6f9540a7afe0}





#### `public  Address()` {#group__symple_1gac343658359ea35750fc8db0954b4859b}





#### `public  Address(const std::string & addr)` {#group__symple_1ga7767484bbc9b2b9eef8ed405a1d6df4e}





#### `public  Address(const std::string & user,const std::string & id)` {#group__symple_1ga124f7f256546189d7bb40403f15d1aee}





#### `public virtual  ~Address()` {#group__symple_1gaec7089a1558336d72614e3ac3c6fe31b}





#### `public bool parse(const std::string & addr)` {#group__symple_1gadce752de542bbfc15cbbc6bd3399a609}





#### `public bool valid() const` {#group__symple_1gad9683597832591065ff7527d9d0d1e7b}





#### `public void print(std::ostream & os) const` {#group__symple_1gae575d3b61d86073b6644fb77525e12b8}





#### `public std::string toString() const` {#group__symple_1ga3e7d857b2da0971cc034191a13ce4936}





#### `public bool operator==(const `[`Address`](#structscy_1_1smpl_1_1Address)` & r)` {#group__symple_1ga8404d6640ae900b3a41bd5bc9eb1ec0e}





#### `public bool operator==(std::string & r)` {#group__symple_1gad2e78b790fca7e0f7a234cdb3c39bd3c}





# struct `scy::smpl::Filter` {#structscy_1_1smpl_1_1Filter}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Bitwise`](./doc/api-base.md#structscy_1_1Bitwise)` flags` | 
`public std::string path` | 
`public inline  Filter(const std::string & path,unsigned flags)` | 
`public inline  Filter(unsigned flags)` | 

## Members

#### `public `[`Bitwise`](./doc/api-base.md#structscy_1_1Bitwise)` flags` {#group__symple_1ga84b2f5acb1cf965cae191acd953dfad4}





#### `public std::string path` {#group__symple_1ga54304a93d73847a0a53f8f1da50a62a1}





#### `public inline  Filter(const std::string & path,unsigned flags)` {#group__symple_1ga7e21efcc6d5fc951be79a19e246294dd}





#### `public inline  Filter(unsigned flags)` {#group__symple_1ga1f3cd2015d9c9dd0f710ec47c2d68e03}





# struct `scy::smpl::Client::Options` {#structscy_1_1smpl_1_1Client_1_1Options}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string host` | 
`public std::uint16_t port` | 
`public bool reconnection` | Weather or not to reconnect if disconnected from the server.
`public int reconnectAttempts` | 
`public std::string user` | 
`public std::string name` | 
`public std::string type` | 
`public std::string token` | 
`public inline  Options()` | 

## Members

#### `public std::string host` {#group__symple_1ga05d0bdf5ab5eeefec7d0019bb110bb58}





#### `public std::uint16_t port` {#group__symple_1ga9e10345420629ea8ebf3bb06dd46ece8}





#### `public bool reconnection` {#group__symple_1ga8388c0e51b717793da8e136f24ef4df6}

Weather or not to reconnect if disconnected from the server.



#### `public int reconnectAttempts` {#group__symple_1gaad73b34182da94bd80d394d6c2cf8d59}



The number of times to attempt to reconnect if disconnected from the server. (0 = unlimited)

#### `public std::string user` {#group__symple_1gace5d72442ca0ddd400242766dcbe0075}





#### `public std::string name` {#group__symple_1ga937a706d0ff802b29881200dd0f9b140}





#### `public std::string type` {#group__symple_1ga3a94f72a2f8e72b1bb84f11630f9cd9a}





#### `public std::string token` {#group__symple_1gae46b1ce6b2a9c5e8ae983cf36137d18d}





#### `public inline  Options()` {#group__symple_1ga7dd4fee81e4318912e9d82a1e35c6550}





