# Module <!-- group --> `uv`

The `uv` module contains C++ wrappers for `libuv`.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::uv`](#namespacescy_1_1uv)    | 
# namespace `scy::uv` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::uv::Handle`](#classscy_1_1uv_1_1Handle)    | 
`struct `[`scy::uv::ShutdownCmd`](#structscy_1_1uv_1_1ShutdownCmd)    | 
# class `scy::uv::Handle` 




A base class for managing a `libuv` handle during it's lifecycle and safely handling its asynchronous destruction mechanism.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Handle(uv::Loop * loop,void * handle)` | 
`public virtual  ~Handle()` | 
`public virtual void setLoop(uv::Loop * loop)` | The event loop may be set before the handle is initialized.
`public virtual void close()` | Closes and destroys the associated `libuv` handle.
`public bool initialized()` | Returns true if the handle has been initialized.
`public bool ref()` | Reference main loop again, once unref'd.
`public bool unref()` | Unreference the main loop after initialized.
`public const `[`scy::Error`](#structscy_1_1Error)` & error() const` | Returns the error context if any.
`public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` | Sets the error content and triggers callbacks.
`public virtual void setAndThrowError(const std::string & prefix,int errorno)` | 
`public virtual void throwError(const std::string & prefix,int errorno) const` | 
`public virtual void setUVError(const std::string & prefix,int errorno)` | 
`public std::thread::id tid() const` | Returns the parent thread ID.
`public virtual uv::Loop * loop() const` | Returns a cast pointer to the managed `libuv` handle.
`public virtual bool active() const` | 
`public virtual bool closed() const` | Returns true after [close()](#group__uv_1ga299a571ac2e23a62521675e21d96780a) has been called.
`public template<class T>`  <br/>`inline T * ptr() const` | Returns a typecasted pointer to the managed `libuv` handle.
`public virtual uv_handle_t * ptr() const` | Returns a pointer to the managed `libuv` handle.
`public void assertThread() const` | Make sure we are calling from the event loop thread.
`protected uv::Loop * _loop` | 
`protected uv_handle_t * _ptr` | 
`protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` | 
`protected std::thread::id _tid` | 
`protected bool _closed` | 
`protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void onClose()` | Override to handle closure.
`protected  Handle(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` | 
`protected `[`Handle`](#classscy_1_1uv_1_1Handle)` & operator=(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` | 

## Members

#### `public  Handle(uv::Loop * loop,void * handle)` 





#### `public virtual  ~Handle()` 





#### `public virtual void setLoop(uv::Loop * loop)` 

The event loop may be set before the handle is initialized.



#### `public virtual void close()` 

Closes and destroys the associated `libuv` handle.



#### `public bool initialized()` 

Returns true if the handle has been initialized.



#### `public bool ref()` 

Reference main loop again, once unref'd.



#### `public bool unref()` 

Unreference the main loop after initialized.



#### `public const `[`scy::Error`](#structscy_1_1Error)` & error() const` 

Returns the error context if any.



#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` 

Sets the error content and triggers callbacks.



#### `public virtual void setAndThrowError(const std::string & prefix,int errorno)` 



Sets and throws the last error. Should never be called inside `libuv` callbacks.

#### `public virtual void throwError(const std::string & prefix,int errorno) const` 



Throws the last error. This function is const so it can be used for invalid getter operations on closed handles. The actual error would be set on the next iteraton.

#### `public virtual void setUVError(const std::string & prefix,int errorno)` 



Sets the last error and sends relevant callbacks. This method can be called inside `libuv` callbacks.

#### `public std::thread::id tid() const` 

Returns the parent thread ID.



#### `public virtual uv::Loop * loop() const` 

Returns a cast pointer to the managed `libuv` handle.



#### `public virtual bool active() const` 



Returns true when the handle is active. This method should be used instead of [closed()](#group__uv_1ga102cbff669d0f4d710d8eac02fd7f5d0) to determine the veracity of the `libuv` handle for stream operations.

#### `public virtual bool closed() const` 

Returns true after [close()](#group__uv_1ga299a571ac2e23a62521675e21d96780a) has been called.



#### `public template<class T>`  <br/>`inline T * ptr() const` 

Returns a typecasted pointer to the managed `libuv` handle.



#### `public virtual uv_handle_t * ptr() const` 

Returns a pointer to the managed `libuv` handle.



#### `public void assertThread() const` 

Make sure we are calling from the event loop thread.



#### `protected uv::Loop * _loop` 





#### `protected uv_handle_t * _ptr` 





#### `protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` 





#### `protected std::thread::id _tid` 





#### `protected bool _closed` 





#### `protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` & error)` 



Override to handle errors. The error may be a UV error, or a custom error.

#### `protected virtual void onClose()` 

Override to handle closure.



#### `protected  Handle(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` 





#### `protected `[`Handle`](#classscy_1_1uv_1_1Handle)` & operator=(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` 





# struct `scy::uv::ShutdownCmd` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * opaque` | 
`public std::function< void(void *)> callback` | 

## Members

#### `public void * opaque` 





#### `public std::function< void(void *)> callback` 





