///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Stream_H
#define SCY_Stream_H


#include "scy/memory.h"
#include "scy/uv/uvpp.h"

#include "scy/buffer.h"
#include "scy/signal.h"
#include <stdexcept>


namespace scy {


class Stream : public uv::Handle
{
public:
    Stream(uv::Loop* loop= uv::defaultLoop(), void* stream= nullptr)
        : uv::Handle(loop, stream)
        , _buffer(65536)
    {
    }

    /// Closes and resets the stream handle.
    /// This will close the active socket/pipe
    /// and destroy the uv_stream_t handle.
    ///
    /// If the stream is already closed this call
    /// will have no side-effects.
    void close()
    {
        TraceL << "Close: " << ptr() << std::endl;
        if (active())
            readStop();
        uv::Handle::close();
    }

    /// Sends a shutdown packet to the connected peer.
    /// Returns true if the shutdown packet was sent.
    bool shutdown()
    {
        assertThread();

        TraceL << "Send shutdown" << std::endl;
        if (!active()) {
            WarnL << "Attempted shutdown on closed stream" << std::endl;
            return false;
        }
        // XXX: Sending shutdown causes an eof error to be
        // returned via handleRead() which sets the stream
        // to error state. This is not really an error,
        // perhaps it should be handled differently?
        int r= uv_shutdown(new uv_shutdown_t, ptr<uv_stream_t>(),
                           [](uv_shutdown_t* req, int) { delete req; });

        return r == 0;
    }

    /// Writes data to the stream.
    ///
    /// Returns false if the underlying socket is closed.
    /// This method does not throw an exception.
    bool write(const char* data, std::size_t len)
    {
        assertThread();

        // if (closed())
        //    throw std::runtime_error("IO error: Cannot write to closed
        //    stream");
        if (!active())
            return false;

        int r;
        uv_write_t* req= new uv_write_t;
        uv_buf_t buf= uv_buf_init((char*)data, len);
        uv_stream_t* stream= this->ptr<uv_stream_t>();
        bool isIPC= stream->type == UV_NAMED_PIPE &&
                    reinterpret_cast<uv_pipe_t*>(stream)->ipc;

        if (!isIPC) {
            r= uv_write(req, stream, &buf, 1,
                        [](uv_write_t* req, int) { delete req; });
        } else {
            r= uv_write2(req, stream, &buf, 1, nullptr,
                         [](uv_write_t* req, int) { delete req; });
        }

        if (r) {
            delete req;
            // setAndThrowError(r, "Stream write error");
        }
        return r == 0;
    }

    Buffer& buffer() // Returns the read buffer.
    {
        assertThread();
        return _buffer;
    }

    /// Returns true if the native socket handle is closed.
    virtual bool closed() const { return uv::Handle::closed(); }

    /// Signals when data can be read from the stream.
    Signal<void(Stream&, const char*, const int&)> Read;

protected:
    virtual bool readStart()
    {
        // TraceL << "Read start: " << ptr() << std::endl;
        int r= uv_read_start(this->ptr<uv_stream_t>(), Stream::allocReadBuffer,
                             handleRead);
        if (r)
            setUVError("Stream read error", r);
        return r == 0;
    }

    virtual bool readStop()
    {
        // TraceL << "Read stop: " << ptr() << std::endl;
        int r= uv_read_stop(ptr<uv_stream_t>());
        if (r)
            setUVError("Stream read error", r);
        return r == 0;
    }

    virtual void onRead(const char* data, std::size_t len)
    {
        // TraceL << "On read: " << len << std::endl;
        Read.emit(*this, data, len);
    }

    static void handleReadCommon(uv_stream_t* handle, ssize_t nread,
                                 const uv_buf_t* buf, uv_handle_type pending)
    {
        // TraceL << "Handle read: " << nread << std::endl;
        auto self= reinterpret_cast<Stream*>(handle->data);


        if (nread >= 0) {
            self->onRead(buf->base, nread);
        } else {
            // The stream was closed in error
            // The value of nread is the error number
            // ie. UV_ECONNRESET or UV_EOF etc ...
            self->setUVError("Stream error", nread);
        }
    }

    virtual ~Stream() {}

    // virtual void* self()
    // {
    //     return this;
    // }


    ///
    /// UV callbacks
    //

    static void handleRead(uv_stream_t* handle, ssize_t nread,
                           const uv_buf_t* buf)
    {
        handleReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
    }

    static void handleRead2(uv_pipe_t* handle, ssize_t nread,
                            const uv_buf_t* buf, uv_handle_type pending)
    {
        handleReadCommon((uv_stream_t*)handle, nread, buf, pending);
    }

    static void allocReadBuffer(uv_handle_t* handle, std::size_t suggested_size,
                                uv_buf_t* buf)
    {
        auto self= reinterpret_cast<Stream*>(
            handle->data); // Reserve the recommended buffer size
        // if (suggested_size > self->_buffer.capacity())    //
        // self->_buffer.capacity(suggested_size);
        assert(self->_buffer.size() >=
               suggested_size); /// Reset the buffer position on each read
        buf->base= self->_buffer.data();
        buf->len= self->_buffer.size();
    }

    Buffer _buffer;
};


} // namespace scy


#endif // SCY_Stream_H


/// @\}
