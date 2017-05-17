# Module <!-- group --> `json`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::json`](#namespacescy_1_1json)    | 
# namespace `scy::json` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::json::Configuration`](#classscy_1_1json_1_1Configuration)    | 
`class `[`scy::json::ISerializable`](#classscy_1_1json_1_1ISerializable)    | 
# class `scy::json::Configuration` 

```
class scy::json::Configuration
  : public scy::Configuration
```  



JSON configuration file

See base [Configuration](#classscy_1_1json_1_1Configuration) for all accessors

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public json::value root` | 
`public  Configuration()` | 
`public virtual  ~Configuration()` | Destroys the [Configuration](./doc/api-json.md#classscy_1_1json_1_1Configuration).
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
`protected mutable std::mutex _mutex` | 
`protected virtual bool getRaw(const std::string & key,std::string & value) const` | 
`protected virtual void setRaw(const std::string & key,const std::string & value)` | 

## Members

#### `public json::value root` 





#### `public  Configuration()` 





#### `public virtual  ~Configuration()` 

Destroys the [Configuration](#classscy_1_1json_1_1Configuration).



#### `public virtual void load(const std::string & path,bool create)` 





#### `public virtual void load(bool create)` 





#### `public virtual void save()` 





#### `public virtual bool remove(const std::string & key)` 





#### `public virtual void removeAll(const std::string & baseKey)` 





#### `public virtual void replace(const std::string & from,const std::string & to)` 





#### `public virtual void keys(std::vector< std::string > & keys,const std::string & baseKey)` 





#### `public virtual void print(std::ostream & ost)` 





#### `public virtual std::string path()` 





#### `public virtual bool loaded()` 





#### `protected bool _loaded` 





#### `protected std::string _path` 





#### `protected mutable std::mutex _mutex` 





#### `protected virtual bool getRaw(const std::string & key,std::string & value) const` 



If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false.

Must be overridden by subclasses.

#### `protected virtual void setRaw(const std::string & key,const std::string & value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

The implementation is responsible for emitting the PropertyChanged signal.

# class `scy::json::ISerializable` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~ISerializable()` | 
`public void serialize(json::value & root)` | 
`public void deserialize(json::value & root)` | 

## Members

#### `public inline virtual  ~ISerializable()` 





#### `public void serialize(json::value & root)` 





#### `public void deserialize(json::value & root)` 





