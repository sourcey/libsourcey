///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup http
/// @{


#ifndef SCY_HTTP_Message_H
#define SCY_HTTP_Message_H


#include "scy/collection.h"


namespace scy {
namespace http {


/// The base class for Request and Response.
///
/// Defines the common properties of all HTTP messages.
/// These are version, content length, content type
/// and transfer encoding.
class Message: public NVCollection
{
public:
    /// Sets the HTTP version for this message.
    void setVersion(const std::string& version);

    /// Returns the HTTP version for this message.
    const std::string& getVersion() const;

    /// Sets the Content-Length header.
    ///
    /// If length is UNKNOWN_CONTENT_LENGTH, removes
    /// the Content-Length header.
    void setContentLength(std::uint64_t length);

    /// Returns the content length for this message,
    /// which may be UNKNOWN_CONTENT_LENGTH if
    /// no Content-Length header is present.
    std::uint64_t getContentLength() const;

    /// Returns true if a Content-Length header is present.
    bool hasContentLength() const;

    /// Sets the transfer encoding for this message.
    ///
    /// The value should be either IDENTITY_TRANSFER_CODING
    /// or CHUNKED_TRANSFER_CODING.
    void setTransferEncoding(const std::string& transferEncoding);

    /// Returns the transfer encoding used for this
    /// message.
    ///
    /// Normally, this is the value of the Transfer-Encoding
    /// header field. If no such field is present,
    /// returns IDENTITY_TRANSFER_CODING.
    const std::string& getTransferEncoding() const;

    /// If flag is true, sets the Transfer-Encoding header to
    /// chunked. Otherwise, removes the Transfer-Encoding
    /// header.
    void setChunkedTransferEncoding(bool flag);

    /// Returns true if the Transfer-Encoding header is set
    /// and its value is chunked.
    bool isChunkedTransferEncoding() const;

    /// Sets the content type for this message.
    ///
    /// Specify NO_CONTENT_TYPE to remove the
    /// Content-Type header.
    void setContentType(const std::string& contentType);

    /// Returns the content type for this message.
    ///
    /// If no Content-Type header is present,
    /// returns UNKNOWN_CONTENT_TYPE.
    const std::string& getContentType() const;

    /// Sets the value of the Connection header field.
    ///
    /// The value is set to "Keep-Alive" if keepAlive is
    /// true, or to "Close" otherwise.
    void setKeepAlive(bool keepAlive);

    /// Returns true if
    ///   * the message has a Connection header field and its value is "Keep-Alive"
    ///   * the message is a HTTP/1.1 message and not Connection header is set
    /// Returns false otherwise.
    bool getKeepAlive() const;

    /// Writes the message header to the given output stream.
    ///
    /// The format is one name-value pair per line, with
    /// name and value separated by a colon and lines
    /// delimited by a carriage return and a linefeed
    /// character. See RFC 2822 for details.
    virtual void write(std::ostream& ostr) const;

    static const std::string HTTP_1_0;
    static const std::string HTTP_1_1;

    static const std::string IDENTITY_TRANSFER_ENCODING;
    static const std::string CHUNKED_TRANSFER_ENCODING;

    static const int         UNKNOWN_CONTENT_LENGTH;
    static const std::string UNKNOWN_CONTENT_TYPE;

    static const std::string CONTENT_LENGTH;
    static const std::string CONTENT_TYPE;
    static const std::string TRANSFER_ENCODING;
    static const std::string CONNECTION;

    static const std::string CONNECTION_KEEP_ALIVE;
    static const std::string CONNECTION_CLOSE;

    static const std::string EMPTY;

protected:
    /// Creates the Message with version HTTP/1.0.
    Message();

    /// Creates the Message and sets
    /// the version.
    Message(const std::string& version);

    /// Destroys the Message.
    virtual ~Message();

private:
    std::string _version;
};


} } // namespace scy::http


#endif // SCY_HTTP_Message_H

/// @\}


//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
