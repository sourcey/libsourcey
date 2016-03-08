//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SCY_Stream_H
#define SCY_Stream_H


#include "scy/uv/uvpp.h"
#include "scy/memory.h"

#include "scy/signal.h"
#include "scy/buffer.h"
#include <stdexcept>


namespace scy {


class Stream: public uv::Handle
{
 public:
    Stream(uv::Loop* loop = uv::defaultLoop(), void* stream = nullptr) :
        uv::Handle(loop, stream),
        _buffer(65536)
    {
    }

    void close()
        // Closes and resets the stream handle.
        // This will close the active socket/pipe
        // and destroy the uv_stream_t handle.
        //
        // If the stream is already closed this call
        // will have no side-effects.
    {
        TraceL << "Close: " << ptr() << std::endl;
        if (active())
            readStop();
        uv::Handle::close();
    }

    bool shutdown()
        // Sends a shutdown packet to the connected peer.
        // Returns true if the shutdown packet was sent.
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
        int r = uv_shutdown(new uv_shutdown_t, ptr<uv_stream_t>(), [](uv_shutdown_t* req, int) {
            delete req;
        });

        return r == 0;
    }

    bool write(const char* data, std::size_t len)
        // Writes data to the stream.
        //
        // Returns false if the underlying socket is closed.
        // This method does not throw an exception.
    {
        assertThread();

        //if (closed())
        //    throw std::runtime_error("IO error: Cannot write to closed stream");
        if (!active())
            return false;

        int r;
        uv_write_t* req = new uv_write_t;
        uv_buf_t buf = uv_buf_init((char*)data, len);
        uv_stream_t* stream = this->ptr<uv_stream_t>();
        bool isIPC = stream->type == UV_NAMED_PIPE &&
            reinterpret_cast<uv_pipe_t*>(stream)->ipc;

        if (!isIPC) {
            r = uv_write(req, stream, &buf, 1, [](uv_write_t* req, int) {
                delete req;
            });
        }
        else {
            r = uv_write2(req, stream, &buf, 1, nullptr, [](uv_write_t* req, int) {
                delete req;
            });
        }

        if (r) {
            delete req;
            //setAndThrowError(r, "Stream write error");
        }
        return r == 0;
    }

    Buffer& buffer()
        // Returns the read buffer.
    {
        assertThread();
        return _buffer;
    }

    virtual bool closed() const
        // Returns true if the native socket handle is closed.
    {
        return uv::Handle::closed();
    }

    Signal2<const char*, const int&> Read;
        // Signals when data can be read from the stream.

 protected:
    bool readStart()
    {
        //TraceL << "Read start: " << ptr() << std::endl;
        int r = uv_read_start(this->ptr<uv_stream_t>(), Stream::allocReadBuffer, handleRead);
        if (r) setUVError("Stream read error", r);
        return r == 0;
    }

    bool readStop()
    {
        //TraceL << "Read stop: " << ptr() << std::endl;
        int r = uv_read_stop(ptr<uv_stream_t>());
        if (r) setUVError("Stream read error", r);
        return r == 0;
    }

    virtual void onRead(const char* data, std::size_t len)
    {
        //TraceL << "On read: " << len << std::endl;
        Read.emit(self(), data, len);
    }

    static void handleReadCommon(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending)
    {
        auto self = reinterpret_cast<Stream*>(handle->data);
        //TraceL << "Handle read: " << nread << std::endl;

        if (nread >= 0) {
            self->onRead(buf->base, nread);
        }
        else {
            // The stream was closed in error
            // The value of nread is the error number
            // ie. UV_ECONNRESET or UV_EOF etc ...
            self->setUVError("Stream error", nread);
        }
    }

    virtual ~Stream()
    {
    }

    virtual void* self()
    {
        return this;
    }


    //
    // UV callbacks
    //

    static void handleRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
    {
        handleReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
    }

    static void handleRead2(uv_pipe_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending)
    {
        handleReadCommon((uv_stream_t*)handle, nread, buf, pending);
    }

    static void allocReadBuffer(uv_handle_t *handle, std::size_t suggested_size, uv_buf_t* buf)
    {
        auto self = reinterpret_cast<Stream*>(handle->data);

        // Reserve the recommended buffer size
        //if (suggested_size > self->_buffer.capacity())
        //    self->_buffer.capacity(suggested_size);
        assert(self->_buffer.size() >= suggested_size);

        // Reset the buffer position on each read
        buf->base = self->_buffer.data();
        buf->len = self->_buffer.size();
    }

    Buffer _buffer;
};


} // namespace scy


#endif // SCY_Stream_H
