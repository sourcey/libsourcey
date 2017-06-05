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
#include "scy/request.h"
#include "scy/buffer.h"
#include "scy/signal.h"
#include "scy/logger.h"

#include <stdexcept>


namespace scy {


/// Basic stream type for sockets and pipes.
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
    virtual void close() override
    {
        // LTrace("Close: ", ptr())
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

        auto buf = uv_buf_init((char*)data, (int)len);
        return Handle::invoke(&uv_write, new uv_write_t, stream(), &buf, 1, [](uv_write_t* req, int) {
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

        auto buf = uv_buf_init((char*)data, (int)len);
        return Handle::invoke(&uv_write2, new uv_write_t, stream(), &buf, 1, send, [](uv_write_t* req, int) {
            delete req;
        });
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
        // LTrace("Read start: ", ptr())
        assert(!_started);
        _started = true;

        stream()->data = this;
        return Handle::invoke(&uv_read_start, stream(), Stream::allocReadBuffer, handleRead);
    }

    virtual bool readStop()
    {
        // LTrace("Read stop: ", ptr())
        assert(_started);
        _started = false;

        return Handle::invoke(&uv_read_stop, stream());
    }

    virtual void onRead(const char* data, size_t len)
    {
        // LTrace("On read: ", len)
        assert(Handle::initialized());
        assert(!Handle::closed());
        assert(_started);

        Read.emit(data, (const int)len);
    }

    //
    /// UV callbacks

    static void handleRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
    {
        // LTrace("Handle read: ", nread)
        auto self = reinterpret_cast<Stream*>(handle->data);
#ifdef SCY_EXCEPTION_RECOVERY
        try {
#endif
            if (nread >= 0) {
                self->onRead(buf->base, nread);
            }
            else {
                self->setUVError((int)nread, "Stream read error");
            }
#ifdef SCY_EXCEPTION_RECOVERY
        }
        catch (std::exception& exc) {
            // Exceptions thrown inside the read callback scope will set the
            // stream error in order to keep errors in the event loop
            LError("Stream exception: ", exc.what());
            self->setUVError(UV_UNKNOWN, exc.what());
        }
#endif
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
    bool _started{false};
};


} // namespace scy


#endif // SCY_Stream_H


/// @\}
