///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Stream_H
#define SCY_Stream_H


#include "scy/base.h"
#include "scy/uv/handle.h"

#include "scy/buffer.h"
#include "scy/signal.h"
#include <stdexcept>


namespace scy {


class Base_API Stream : public uv::Handle
{
public:
    Stream(uv::Loop* loop = uv::defaultLoop(), void* stream = nullptr);
    virtual ~Stream();

    /// Closes and resets the stream handle.
    /// This will close the active socket/pipe
    /// and destroy the uv_stream_t handle.
    ///
    /// If the stream is already closed this call
    /// will have no side-effects.
    virtual void close();

    /// Sends a shutdown packet to the connected peer.
    /// Returns true if the shutdown packet was sent.
    bool shutdown();

    /// Writes data to the stream.
    ///
    /// Returns false if the underlying socket is closed.
    /// This method does not throw an exception.
    bool write(const char* data, std::size_t len);

    /// Returns the read buffer.
    Buffer& buffer();

    /// Returns true if the native socket handle is closed.
    virtual bool closed() const;

    /// Signals when data can be read from the stream.
    Signal<void(Stream&, const char*, const int&)> Read;

protected:
    virtual bool readStart();
    virtual bool readStop();
    virtual void onRead(const char* data, std::size_t len);

    //
    /// UV callbacks

    static void handleReadCommon(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending);
    static void handleRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
    static void handleRead2(uv_pipe_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending);
    static void allocReadBuffer(uv_handle_t* handle, std::size_t suggested_size, uv_buf_t* buf);

    Buffer _buffer;
    bool _started;
};


} // namespace scy


#endif // SCY_Stream_H


/// @\}
