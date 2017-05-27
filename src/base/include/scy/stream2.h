///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Stream2_H
#define SCY_Stream2_H


#include "scy/base.h"
#include "scy/buffer.h"
#include "scy/signal.h"
#include "scy/uv/handle2.h"
#include <stdexcept>


namespace scy {


template<typename T>
class Base_API Stream2 : public uv::Handle2<T>
{
public:
    // using uv::Handle2<T>;
    typedef uv::Handle2<T> Handle2;

    Stream2(uv::Loop* loop = uv::defaultLoop())
        : uv::Handle2<T>(loop)
        , _buffer(65536)
        // , _started(false)
    {
    }

    virtual ~Stream2()
    {
        close(); // must close underlying stream
    }

    /// Closes and resets the stream handle.
    /// This will close the active socket/pipe
    /// and destroy the uv_stream_t handle.
    ///
    /// If the stream is already closed this call
    /// will have no side-effects.
    virtual void close()
    {
        // TraceA("Close: ", ptr())
        if (_started)
            readStop();
        Handle2::close();
    }

    /// Sends a shutdown packet to the connected peer.
    /// Returns true if the shutdown packet was sent.
    bool shutdown()
    {
        Handle2::assertThread();

        // TraceA("Send shutdown")
        if (!Handle2::active())
            return false;

        // XXX: Sending shutdown causes an eof error to be
        // returned via handleRead() which sets the stream
        // to error state. This is not really an error,
        // perhaps it should be handled differently?
        int r = uv_shutdown(new uv_shutdown_t, this->template get<uv_stream_t>(), [](uv_shutdown_t* req, int) {
            delete req;
        });

        return r == 0;
    }

    /// Writes data to the stream.
    ///
    /// Returns false if the underlying socket is closed.
    /// This method does not throw an exception.
    bool write(const char* data, size_t len)
    {
        Handle2::assertThread();
        assert(_started);
        assert(Handle2::initialized());
        assert(Handle2::active());
        assert(!Handle2::closed());

        // if (closed())
        //    throw std::runtime_error("IO error: Cannot write to closed stream");
        if (!Handle2::active())
            return false;

        int r;
        auto req = new uv_write_t;
        auto buf = uv_buf_init((char*)data, (int)len);
        auto stream = this->template get<uv_stream_t>();
        bool isIPC = stream->type == UV_NAMED_PIPE && this->template get<uv_pipe_t>()->ipc;

        if (!isIPC) {
            r = uv_write(req, stream, &buf, 1, [](uv_write_t* req, int) {
                delete req;
            });
        } else {
            r = uv_write2(req, stream, &buf, 1, nullptr, [](uv_write_t* req, int) {
                delete req;
            });
        }

        if (r) {
            assert(0);
            // setAndThrowError(r, "Stream2 write error");
        }
        return r == 0;
    }

    /// Returns the read buffer.
    Buffer& buffer()
    {
        Handle2::assertThread();
        return _buffer;
    }

    /// Returns true if the native stream handle is closed.
    virtual bool closed() const
    {
        return Handle2::closed();
    }

    /// Signals when data can be read from the stream.
    Signal<void(/*Stream2&, */const char*, const int&)> Read;

protected:
    virtual bool readStart()
    {
        // TraceA("Read start: ", ptr())
        assert(Handle2::initialized());
        assert(!_started);
        _started = true;

        auto stream = this->template get<uv_stream_t>();
        stream->data = this;
        int r = uv_read_start(stream, Stream2::allocReadBuffer, handleRead);
        if (r)
            Handle2::setUVError("Stream2 read error", r);
        return r == 0;
    }

    virtual bool readStop()
    {
        // TraceA("Read stop: ", ptr())
        assert(Handle2::initialized());
        assert(!Handle2::closed());
        assert(_started);
        _started = false;

        int r = uv_read_stop(this->template get<uv_stream_t>());
        if (r)
            Handle2::setUVError("Stream2 read error", r);
        return r == 0;
    }

    virtual void onRead(const char* data, size_t len)
    {
        // TraceA("On read: ", len)
        Read.emit(/**this, */data, (const int)len);
    }

    //
    /// UV callbacks

    static void handleReadCommon(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending)
    {
        // TraceA("Handle read: ", nread)
        auto self = reinterpret_cast<Stream2*>(handle->data);
        assert(self->_started);
        assert(self->initialized());
        assert(!self->closed());

        // try {
            if (nread >= 0) {
                self->onRead(buf->base, nread);
            }
            else {
                // The stream was closed in error
                // The value of nread is the error number
                // ie. UV_ECONNRESET or UV_EOF etc ...
                self->setUVError("Stream2 error", (int)nread);
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

    static void handleRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
    {
        handleReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
    }

    static void handleRead2(uv_pipe_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending)
    {
        handleReadCommon((uv_stream_t*)handle, nread, buf, pending);
    }

    static void allocReadBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
    {
        auto self = reinterpret_cast<Stream2*>(handle->data);

        // Reserve the recommended buffer size
        // if (suggested_size > self->_buffer.capacity())
        //     self->_buffer.capacity(suggested_size);
        assert(self->_buffer.size() >= suggested_size);

        // Reset the buffer position on each read
        buf->base = self->_buffer.data();
        buf->len = self->_buffer.size();
    }

protected:
    Buffer _buffer; // { 65536 };
    bool _started { false };
};


} // namespace scy


#endif // SCY_Stream2_H


/// @\}
