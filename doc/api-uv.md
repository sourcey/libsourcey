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
`struct `[`scy::uv::BasicEvent`](#structscy_1_1uv_1_1BasicEvent)    | Default request callback event.
`struct `[`scy::uv::Context`](#structscy_1_1uv_1_1Context)    | Shared `libuv` handle context.
`struct `[`scy::uv::Request`](#structscy_1_1uv_1_1Request)    | 
# class `scy::uv::Handle` 




Wrapper class for managing `uv_handle_t` variants.

This class manages the handle during it's lifecycle and safely handles the asynchronous destruction mechanism.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Handle(uv::Loop * loop)` | 
`public inline virtual  ~Handle()` | 
`public template<typename F,typename... Args>`  <br/>`inline bool init(F && f,Args &&... args)` | Initialize the handle.
`public template<typename F,typename... Args>`  <br/>`inline bool invoke(F && f,Args &&... args)` | Invoke a method on the handle.
`public template<typename F,typename... Args>`  <br/>`inline void invokeOrThrow(const std::string & message,F && f,Args &&... args)` | 
`public inline virtual void close()` | Close and destroy the handle.
`public inline void ref()` | Reference main loop again, once unref'd.
`public inline void unref()` | Unreference the main loop after initialized.
`public inline bool initialized() const` | Return true if the handle has been initialized.
`public inline virtual bool active() const` | Return true when the handle is active.
`public inline virtual bool closing() const` | Return true if the handle is closing.
`public inline virtual bool closed() const` | Return true is the handle has been closed.
`public inline const `[`Error`](#structscy_1_1Error)` & error() const` | Return the error context if any.
`public inline virtual void setError(const `[`Error`](#structscy_1_1Error)` & err)` | Set the error and triggers callbacks.
`public inline void setUVError(int errorno,const std::string & prefix)` | 
`public inline void setAndThrowError(int errorno,const std::string & prefix)` | 
`public inline void throwLastError(const std::string & prefix)` | 
`public inline uv::Loop * loop() const` | Return a cast pointer to the managed `libuv` handle.
`public inline virtual void * self()` | Return a pointer to the current or derived instance.
`public inline void reset()` | Reset the internal handle pointer and container state.
`public template<typename `[`Handle`](./doc/api-uv.md#classscy_1_1uv_1_1Handle)`>`  <br/>`inline `[`Handle`](#classscy_1_1uv_1_1Handle)` * get() const` | Return a typecasted pointer to the managed handle.
`public inline std::thread::id tid() const` | Return the parent thread ID.
`public inline std::shared_ptr< `[`Context`](#structscy_1_1uv_1_1Context)`< T > > context() const` | Return the shared handle pointer context.
`public inline void assertThread() const` | Assert the call is from the parent event loop thread.
`protected uv::Loop * _loop` | 
`protected std::shared_ptr< `[`Context`](./doc/api-uv.md#structscy_1_1uv_1_1Context)`< T > > _context` | 
`protected std::thread::id _tid` | 
`protected `[`Error`](./doc/api-base.md#structscy_1_1Error)` _error` | 
`protected inline virtual void onError(const `[`Error`](#structscy_1_1Error)` & error)` | 
`protected inline virtual void onClose()` | 
`protected  Handle(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` | NonCopyable and NonMovable.
`protected `[`Handle`](#classscy_1_1uv_1_1Handle)` & operator=(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` | 

## Members

#### `public inline  Handle(uv::Loop * loop)` 





#### `public inline virtual  ~Handle()` 





#### `public template<typename F,typename... Args>`  <br/>`inline bool init(F && f,Args &&... args)` 

Initialize the handle.



#### `public template<typename F,typename... Args>`  <br/>`inline bool invoke(F && f,Args &&... args)` 

Invoke a method on the handle.



#### `public template<typename F,typename... Args>`  <br/>`inline void invokeOrThrow(const std::string & message,F && f,Args &&... args)` 



Invoke a method on the handle.

An exception will be thrown if the invoked method returns an error.

#### `public inline virtual void close()` 

Close and destroy the handle.



#### `public inline void ref()` 

Reference main loop again, once unref'd.



#### `public inline void unref()` 

Unreference the main loop after initialized.



#### `public inline bool initialized() const` 

Return true if the handle has been initialized.



#### `public inline virtual bool active() const` 

Return true when the handle is active.



#### `public inline virtual bool closing() const` 

Return true if the handle is closing.



#### `public inline virtual bool closed() const` 

Return true is the handle has been closed.



#### `public inline const `[`Error`](#structscy_1_1Error)` & error() const` 

Return the error context if any.



#### `public inline virtual void setError(const `[`Error`](#structscy_1_1Error)` & err)` 

Set the error and triggers callbacks.



#### `public inline void setUVError(int errorno,const std::string & prefix)` 



Set the error and trigger relevant callbacks. This method can be called inside `libuv` callbacks.

#### `public inline void setAndThrowError(int errorno,const std::string & prefix)` 



Set the error and throw an exception. Should never be called inside `libuv` callbacks.

#### `public inline void throwLastError(const std::string & prefix)` 



Throw an exception if the handle is in error state. The error message prefix will be updated if provided.

#### `public inline uv::Loop * loop() const` 

Return a cast pointer to the managed `libuv` handle.



#### `public inline virtual void * self()` 

Return a pointer to the current or derived instance.



#### `public inline void reset()` 

Reset the internal handle pointer and container state.



#### `public template<typename `[`Handle`](./doc/api-uv.md#classscy_1_1uv_1_1Handle)`>`  <br/>`inline `[`Handle`](#classscy_1_1uv_1_1Handle)` * get() const` 

Return a typecasted pointer to the managed handle.



#### `public inline std::thread::id tid() const` 

Return the parent thread ID.



#### `public inline std::shared_ptr< `[`Context`](#structscy_1_1uv_1_1Context)`< T > > context() const` 

Return the shared handle pointer context.



#### `public inline void assertThread() const` 

Assert the call is from the parent event loop thread.



#### `protected uv::Loop * _loop` 





#### `protected std::shared_ptr< `[`Context`](./doc/api-uv.md#structscy_1_1uv_1_1Context)`< T > > _context` 





#### `protected std::thread::id _tid` 





#### `protected `[`Error`](./doc/api-base.md#structscy_1_1Error)` _error` 





#### `protected inline virtual void onError(const `[`Error`](#structscy_1_1Error)` & error)` 



[Error](#structscy_1_1Error) callback. Override to handle errors. The error may be a UV error, or a custom error.

#### `protected inline virtual void onClose()` 



Close callback. Override to handle closure.

#### `protected  Handle(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` 

NonCopyable and NonMovable.



#### `protected `[`Handle`](#classscy_1_1uv_1_1Handle)` & operator=(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &) = delete` 





# struct `scy::uv::BasicEvent` 


Default request callback event.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int status` | 

## Members

#### `public int status` 





# struct `scy::uv::Context` 


Shared `libuv` handle context.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Handle`](./doc/api-uv.md#classscy_1_1uv_1_1Handle)`< T > * handle` | 
`public T * ptr` | 
`public bool initialized` | 
`public bool deleted` | 
`public inline  Context(`[`Handle`](#classscy_1_1uv_1_1Handle)`< T > * h)` | 
`public inline  ~Context()` | 

## Members

#### `public `[`Handle`](./doc/api-uv.md#classscy_1_1uv_1_1Handle)`< T > * handle` 





#### `public T * ptr` 





#### `public bool initialized` 





#### `public bool deleted` 





#### `public inline  Context(`[`Handle`](#classscy_1_1uv_1_1Handle)`< T > * h)` 





#### `public inline  ~Context()` 





# struct `scy::uv::Request` 




Wrapper class for managing `uv_req_t` variants.

This class povides safe access to the parent handle incase the handle gets destroyed before the request callback returns, and should be used whenever the handle pointer is accessed via the callback.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public T req` | 
`public std::function< void(const E &)> callback` | 
`public inline  Request()` | 
`public template<typename F,typename... Args>`  <br/>`inline auto invoke(F && f,Args &&... args)` | 

## Members

#### `public T req` 





#### `public std::function< void(const E &)> callback` 





#### `public inline  Request()` 





#### `public template<typename F,typename... Args>`  <br/>`inline auto invoke(F && f,Args &&... args)` 





