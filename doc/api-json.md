# Module <!-- group --> `json`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::json`](#namespacescy_1_1json)    | 
# namespace `scy::json` {#namespacescy_1_1json}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::json::Configuration`](#classscy_1_1json_1_1Configuration)    | 
`class `[`scy::json::ISerializable`](#classscy_1_1json_1_1ISerializable)    | 
# class `scy::json::Configuration` {#classscy_1_1json_1_1Configuration}

```
class scy::json::Configuration
  : public scy::Configuration
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public json::Value root` | See IConfiguration for data accessors.
`public  Configuration()` | 
`public virtual  ~Configuration()` | Destroys the [Configuration](api-json.md#classscy_1_1json_1_1Configuration).
`public virtual void load(const std::string & path,bool create)` | 
`public virtual void load(bool create)` | 
`public virtual void save()` | 
`public virtual bool remove(const std::string & key)` | 
`public virtual void removeAll(const std::string & baseKey)` | 
`public virtual void replace(const std::string & from,const std::string & to)` | 
`public virtual void keys(std::vector< std::string > & keys,const std::string & baseKey)` | 
`public virtual void print(std::ostream & ost)` | 
`public virtual std::string path()` | 
`public virtual bool loaded()` | 
`protected bool _loaded` | 
`protected std::string _path` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected virtual bool getRaw(const std::string & key,std::string & value) const` | 
`protected virtual void setRaw(const std::string & key,const std::string & value)` | 

## Members

#### `public json::Value root` {#group__json_1ga65f130ba02d9aa9d19f78531c4209bbf}

See IConfiguration for data accessors.



#### `public  Configuration()` {#group__json_1gac214c65bca41dbeb2b93de599bf33860}





#### `public virtual  ~Configuration()` {#group__json_1ga041eaafa3e57334159495db3f7c8f31a}

Destroys the [Configuration](#classscy_1_1json_1_1Configuration).



#### `public virtual void load(const std::string & path,bool create)` {#group__json_1gaa92c45c4c4628a4edb8a3d7652e07750}





#### `public virtual void load(bool create)` {#group__json_1ga28c81762c1884c20e06818b09d2ea74a}





#### `public virtual void save()` {#group__json_1ga561f1c272c7e613c15b4827b5be26112}





#### `public virtual bool remove(const std::string & key)` {#group__json_1gaf79b732c4b7700e0aa93fe276e7e11d7}





#### `public virtual void removeAll(const std::string & baseKey)` {#group__json_1gaef6d50b63ce07b1137162050e5181520}





#### `public virtual void replace(const std::string & from,const std::string & to)` {#group__json_1ga81c6e1be7c048f7df27f45b04fe4c80b}





#### `public virtual void keys(std::vector< std::string > & keys,const std::string & baseKey)` {#group__json_1gaeb8525cb4022f0c4925e7152c0153af4}





#### `public virtual void print(std::ostream & ost)` {#group__json_1ga248b47860e2324f01700bf297946c5ea}





#### `public virtual std::string path()` {#group__json_1ga6950af9e99dc2356093aed42c82590c6}





#### `public virtual bool loaded()` {#group__json_1gaf4f7ea24a9de424f59d67691aa5c5f06}





#### `protected bool _loaded` {#group__json_1gabec246e99680f82539c83c0f3a6337ee}





#### `protected std::string _path` {#group__json_1gafb9956baead97160a9feb7a1e06d8ae5}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__json_1ga909b4a58f1164ff9ee8748aaf32d6294}





#### `protected virtual bool getRaw(const std::string & key,std::string & value) const` {#group__json_1ga57bd50c0cb6030db96a1750756e647b1}



If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false.

Must be overridden by subclasses.

#### `protected virtual void setRaw(const std::string & key,const std::string & value)` {#group__json_1gaab4c2e8ef140e16e3646fe896fa62cde}



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

The implementation is responsible for emitting the PropertyChanged signal.

# class `scy::json::ISerializable` {#classscy_1_1json_1_1ISerializable}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~ISerializable()` | 
`public void serialize(json::Value & root)` | 
`public void deserialize(json::Value & root)` | 

## Members

#### `public inline virtual  ~ISerializable()` {#group__json_1ga881bfa8114ca76527fd464f4b3f4f270}





#### `public void serialize(json::Value & root)` {#group__json_1ga30a08de7d54b74c98fd2a5ff7424021c}





#### `public void deserialize(json::Value & root)` {#group__json_1ga47453d8279115db3542fff205608d287}





