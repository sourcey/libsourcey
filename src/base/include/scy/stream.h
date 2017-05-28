///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Stream_H
#define SCY_Stream_H


#include "scy/base.h"
#include "scy/handle.h"

#include "scy/buffer.h"
#include "scy/signal.h"

#include <stdexcept>


namespace scy {


template<typename T>
class Base_API Stream : public uv::Handle<T>
{
public:
    typedef uv::Handle<T> Handle;

    Stream(uv::Loop* loop = uv::defaultLoop())
        : uv::Handle<T>(loop)
        , _buffer(65536)
    {
    }

    virtual ~Stream()
    {
        close();
    }

    /// Closes and resets the stream handle.
    /// This will close the active socket/pipe and destroy the handle.
    ///
    /// If the stream is already closed this call will have no side-effects.
    virtual void close()
    {
        // TraceA("Close: ", ptr())
        if (_started)
            readStop();
        Handle::close();
    }

    /// Sends a shutdown packet to the connected peer.
    /// Return true if the shutdown packet was sent.
    bool shutdown()
    {
        Handle::assertThread();
        if (!Handle::active())
            return false;

        // XXX: Sending shutdown causes an eof error to be returned via
        // handleRead() which sets the stream to error state. This is not
        // really an error, perhaps it should be handled differently?
        return uv_shutdown(new uv_shutdown_t, stream(),
            [](uv_shutdown_t* req, int) {
                delete req;
            }) == 0;
    }

    /// Writes data to the stream.
    ///
    /// Return false if the underlying socket is closed.
    /// This method does not throw an exception.
    bool write(const char* data, size_t len)
    {
        if (!Handle::active())
            return false;

        assert(_started);

        auto req = new uv_write_t;
        auto buf = uv_buf_init((char*)data, (int)len);
        return Handle::invoke(&uv_write, req, stream(), &buf, 1, [](uv_write_t* req, int) {
            delete req;
        });
    }

    /// Write data to the target stream.
    ///
    /// This method is only valid for IPC streams.
    bool write(const char* data, size_t len, uv_stream_t* send)
    {
        if (!Handle::active())
            return false;

        assert(_started);
        assert(stream()->type == UV_NAMED_PIPE && this->template get<uv_pipe_t>()->ipc);

        auto req = new uv_write_t;
        auto buf = uv_buf_init((char*)data, (int)len);
        return Handle::invoke(&uv_write2, req, stream(), &buf, 1, send, [](uv_write_t* req, int) {
            delete req;
        });
    }

    /// Return true if the native stream handle is closed.
    virtual bool closed() const
    {
        return Handle::closed();
    }

    /// Return the uv_stream_t pointer.
    uv_stream_t* stream()
    {
        return this->template get<uv_stream_t>();
    }

    /// Signal the notifies when data is available for read.
    Signal<void(const char*, const int&)> Read;

protected:
    virtual bool readStart()
    {
        // TraceA("Read start: ", ptr())
        assert(!_started);
        _started = true;

        stream()->data = this;
        return Handle::invoke(&uv_read_start, stream(), Stream::allocReadBuffer, handleRead);
    }

    virtual bool readStop()
    {
        // TraceA("Read stop: ", ptr())
        assert(_started);
        _started = false;

        return Handle::invoke(&uv_read_stop, stream());
    }

    virtual void onRead(const char* data, size_t len)
    {
        // TraceA("On read: ", len)
        assert(Handle::initialized());
        assert(!Handle::closed());
        assert(_started);

        Read.emit(data, (const int)len);
    }

    //
    /// UV callbacks

    static void handleRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
    {
        // TraceA("Handle read: ", nread)
        auto self = reinterpret_cast<Stream*>(handle->data);

        // try {
            if (nread >= 0) {
                self->onRead(buf->base, nread);
            }
            else {
                // The stream was closed in error
                // The value of nread is the error number
                // ie. UV_ECONNRESET or UV_EOF etc ...
                self->setUVError("Stream error", (int)nread);
            }
        // }
        // catch (std::exception& exc) {
        //
        //     // Swallow exceptions and set the stream error
        //     // This keep errors in the event loop
        //     ErrorL << "Exception: " << exc.what() << std::endl;
        //     self->setUVError(exc.what());
        //     return;
        // }
    }

    static void allocReadBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
    {
        auto& buffer = reinterpret_cast<Stream*>(handle->data)->_buffer;
        assert(buffer.size() >= suggested_size);
        // if (suggested_size > buffer.capacity())
        //     buffer.capacity(suggested_size);

        buf->base = buffer.data();
        buf->len = buffer.size();
    }

protected:
    Buffer _buffer;
    bool _started { false };
};


} // namespace scy


#endif // SCY_Stream_H


/// @\}
