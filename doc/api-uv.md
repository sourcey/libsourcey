# Module <!-- group --> `uv`

The `uv` module contains C++ wrappers for `libuv`.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::uv`](#namespacescy_1_1uv)    | 
# namespace `scy::uv` {#namespacescy_1_1uv}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::uv::Handle`](#classscy_1_1uv_1_1Handle)    | UV [Handle](#classscy_1_1uv_1_1Handle).
`struct `[`scy::uv::ShutdownCmd`](#structscy_1_1uv_1_1ShutdownCmd)    | 
# class `scy::uv::Handle` {#classscy_1_1uv_1_1Handle}


UV [Handle](#classscy_1_1uv_1_1Handle).

A base class for managing a `libuv` handle during it's lifecycle and safely handling its asynchronous destruction mechanism.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Handle(uv_loop_t * loop,void * handle)` | 
`public virtual  ~Handle()` | 
`public virtual void setLoop(uv_loop_t * loop)` | The event loop may be set before the handle is initialized.
`public virtual uv_loop_t * loop() const` | Returns a cast pointer to the managed `libuv` handle.
`public template<class T>`  <br/>`inline T * ptr() const` | Returns a typecasted pointer to the managed `libuv` handle.
`public virtual uv_handle_t * ptr() const` | Returns a pointer to the managed `libuv` handle.
`public virtual bool active() const` | 
`public virtual bool closed() const` | Returns true after [close()](#group__uv_1ga299a571ac2e23a62521675e21d96780a) has been called.
`public bool ref()` | Reference main loop again, once unref'd.
`public bool unref()` | Unreference the main loop after initialized.
`public uv_thread_t tid() const` | Returns the parent thread ID.
`public const `[`scy::Error`](#structscy_1_1Error)` & error() const` | Returns the error context if any.
`public virtual void setAndThrowError(const std::string & prefix,int errorno)` | 
`public virtual void throwError(const std::string & prefix,int errorno) const` | 
`public virtual void setUVError(const std::string & prefix,int errorno)` | 
`public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` | Sets the error content and triggers callbacks.
`public virtual void close()` | Closes and destroys the associated `libuv` handle.
`public void assertThread() const` | Make sure we are calling from the event loop thread.
`protected uv_loop_t * _loop` | 
`protected uv_handle_t * _ptr` | 
`protected `[`scy::Error`](api-base.md#structscy_1_1Error)` _error` | 
`protected uv_thread_t _tid` | 
`protected bool _closed` | 
`protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` | 
`protected virtual void onClose()` | Override to handle closure.
`protected  Handle(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &)` | 
`protected `[`Handle`](#classscy_1_1uv_1_1Handle)` & operator=(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &)` | 

## Members

#### `public  Handle(uv_loop_t * loop,void * handle)` {#group__uv_1ga1d481d014e0a8b92fa9023639c9a23f3}





#### `public virtual  ~Handle()` {#group__uv_1gaeca46ff28ef0a19565154eac4018912e}





#### `public virtual void setLoop(uv_loop_t * loop)` {#group__uv_1gacb579307642d06ac22d481ec4fa40a95}

The event loop may be set before the handle is initialized.



#### `public virtual uv_loop_t * loop() const` {#group__uv_1ga6eda3e6f7593bf910eaa9694671544ad}

Returns a cast pointer to the managed `libuv` handle.



#### `public template<class T>`  <br/>`inline T * ptr() const` {#group__uv_1ga1be86c8fb7206e2f1bb9571221d16f30}

Returns a typecasted pointer to the managed `libuv` handle.



#### `public virtual uv_handle_t * ptr() const` {#group__uv_1ga69db247c9c09a21a760464cb52344e50}

Returns a pointer to the managed `libuv` handle.



#### `public virtual bool active() const` {#group__uv_1ga05add736becb3fa70de08071b6f8cc1e}



Returns true when the handle is active. This method should be used instead of [closed()](#group__uv_1ga102cbff669d0f4d710d8eac02fd7f5d0) to determine the veracity of the `libuv` handle for stream io operations.

#### `public virtual bool closed() const` {#group__uv_1ga102cbff669d0f4d710d8eac02fd7f5d0}

Returns true after [close()](#group__uv_1ga299a571ac2e23a62521675e21d96780a) has been called.



#### `public bool ref()` {#group__uv_1gae8a79268466e9b8b5440cf736dceaaee}

Reference main loop again, once unref'd.



#### `public bool unref()` {#group__uv_1ga7ffde9a69582156a985232f170f4c5a1}

Unreference the main loop after initialized.



#### `public uv_thread_t tid() const` {#group__uv_1ga76200c3c4f93dd1d9be947013a50b4d1}

Returns the parent thread ID.



#### `public const `[`scy::Error`](#structscy_1_1Error)` & error() const` {#group__uv_1ga108ab93af11444ce829340bf727f3131}

Returns the error context if any.



#### `public virtual void setAndThrowError(const std::string & prefix,int errorno)` {#group__uv_1gafc830f59dbb4e20e61d2d1383861112b}



Sets and throws the last error. Should never be called inside `libuv` callbacks.

#### `public virtual void throwError(const std::string & prefix,int errorno) const` {#group__uv_1ga4066f1521db02db28d4c94c7ae31a31e}



Throws the last error. This function is const so it can be used for invalid getter operations on closed handles. The actual error would be set on the next iteraton.

#### `public virtual void setUVError(const std::string & prefix,int errorno)` {#group__uv_1ga42dd550b32180fecc2f500c79072b043}



Sets the last error and sends relevant callbacks. This method can be called inside `libuv` callbacks.

#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` {#group__uv_1ga2192b88da4ffbc7e46f82763360b1a66}

Sets the error content and triggers callbacks.



#### `public virtual void close()` {#group__uv_1ga299a571ac2e23a62521675e21d96780a}

Closes and destroys the associated `libuv` handle.



#### `public void assertThread() const` {#group__uv_1ga5d26f45bfa9e71bd1f5c5e568f813d56}

Make sure we are calling from the event loop thread.



#### `protected uv_loop_t * _loop` {#group__uv_1gaa16cb36922409fb818eca4a693e9991c}





#### `protected uv_handle_t * _ptr` {#group__uv_1ga31ecf02ffa1644ecdce020a65dfd2309}





#### `protected `[`scy::Error`](api-base.md#structscy_1_1Error)` _error` {#group__uv_1gac0e5b3021a4588cb29786d1552981b74}





#### `protected uv_thread_t _tid` {#group__uv_1ga046fa5bcd961b4c27d0c253ecaaf6ba2}





#### `protected bool _closed` {#group__uv_1ga72af4825d7e6fbdd0f51de69e6fddd73}





#### `protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` {#group__uv_1ga8c7fd89959f9017379d4c34c5df43a63}



Override to handle errors. The error may be a UV error, or a custom error.

#### `protected virtual void onClose()` {#group__uv_1gabaa78e11ef78067eabc9faf4550c66bc}

Override to handle closure.



#### `protected  Handle(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &)` {#group__uv_1ga1f258c7f7e1331bd54d57ce7060414c4}





#### `protected `[`Handle`](#classscy_1_1uv_1_1Handle)` & operator=(const `[`Handle`](#classscy_1_1uv_1_1Handle)` &)` {#group__uv_1ga83ba9fc3f18a22452194317c044ae34b}





# struct `scy::uv::ShutdownCmd` {#structscy_1_1uv_1_1ShutdownCmd}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * opaque` | 
`public std::function< void(void *)> callback` | 

## Members

#### `public void * opaque` {#group__uv_1ga07de360e98f67a6a52122dd425bd7cfe}





#### `public std::function< void(void *)> callback` {#group__uv_1ga6e7a0420591daed9802cd9f1ae8516cb}





