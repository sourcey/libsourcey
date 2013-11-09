#ifndef SOURCEY_HTTP_Form_H
#define SOURCEY_HTTP_Form_H


#include "Sourcey/Collection.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Thread.h"
#include "Sourcey/Net/Types.h"


namespace scy {
namespace http {


class Request;
class ClientConnection;
class FilePart;


//
// HTML Form Writer
//


class FormWriter: public NVCollection, public PacketSource, public async::Startable
	/// FormWriter is a HTTP client connection adapter for writing HTML forms.
	///
	/// This class runs in its own thread so as not to block the event loop
	/// while uploading big files. Class members are not synchronized hence 
	/// they should not be accessed while the form is sending, not that there
	/// would be any reason to do so.
{
public:
	static FormWriter* create(ClientConnection& conn, const std::string& encoding = FormWriter::ENCODING_URL);
		// Creates the FormWriter that uses the given connection and
		// encoding type.
		//
		// Encoding must be either "application/x-www-form-urlencoded"
		// (which is the default) or "multipart/form-data".
		
	virtual ~FormWriter();
		// Destroys the FormWriter.

	void addFile(const std::string& name, FilePart* part);
		// Adds an part/attachment (file upload) to the form.
		//
		// The form takes ownership of the FilePart and deletes it when it
		// is no longer needed. The part will only be sent if the encoding
		// set for the form is "multipart/form-data"
		
	void start();
		// Starts the sending thread.
		
	void stop();
		// Stops the sending thread.

	bool complete() const;
		// Returns true if the request is complete.

	bool cancelled() const;
		// Returns true if the request is cancelled.
		
	void prepareSubmit();
		// Prepares the outgoing HTTP request object for submitting the form.
		//
		// If the request method is GET, the encoded form is appended to the
		// request URI as query std::string. Otherwise (the method is
		// POST), the form's content type is set to the form's encoding.
		// The form's parameters must be written to the
		// request body separately, with a call to write.
		// If the request's HTTP version is HTTP/1.0:
		//    - persistent connections are disabled
		//    - the content transfer encoding is set to identity encoding
		// Otherwise, if the request's HTTP version is HTTP/1.1:
		//    - the request's persistent connection state is left unchanged
		//    - the content transfer encoding is set to chunked

	void writeUrl(std::ostream& ostr);
		// Writes "application/x-www-form-urlencoded" encoded data to
		// the client connection.
	
#if 0
	void writeMultipart();
		// Writes the complete "multipart/form-data" request to the 
		// client connection. This method is blocking, and should be 
		// called from a thread, especially when sending large files.
#endif

	void writeMultipartChunk();
		// Writes the next multipart "multipart/form-data" encoded
		// to the client connection. This method is non-blocking,
		// and is suitable for use with the event loop.

	void writeAsync();
		// Called asynchronously by the Runner to write the next message chunk.
		// If "multipart/form-data" the next multipart chunk will be written.
		// If "application/x-www-form-urlencoded" the entire message will be written.
		// The complete flag will be set when the entire request has been written.

	void setEncoding(const std::string& encoding);
		// Sets the encoding used for posting the form.
		//
		// Encoding must be either "application/x-www-form-urlencoded"
		// (which is the default) or "multipart/form-data".
		
	const std::string& encoding() const;
		// Returns the encoding used for posting the form.

	void setBoundary(const std::string& boundary);
		// Sets the boundary to use for separating form parts.
		// Must be set before prepareSubmit() is called.

	const std::string& boundary() const;
		// Returns the MIME boundary used for writing multipart form data.

	ClientConnection& connection();
		// The associated HTTP client connection.
			
	PacketSignal emitter;
		// The outgoing packet emitter.

	static const char* ENCODING_URL;       /// "application/x-www-form-urlencoded"
	static const char* ENCODING_MULTIPART; /// "multipart/form-data"
	
protected:
	FormWriter(ClientConnection& conn, async::Runner::ptr runner, const std::string& encoding = FormWriter::ENCODING_URL);
		// Creates the FormWriter that uses the given encoding.
		//
		// Encoding must be either "application/x-www-form-urlencoded"
		// (which is the default) or "multipart/form-data".

	FormWriter(const FormWriter&);
	FormWriter& operator = (const FormWriter&);
		
	void writePartHeader(const NVCollection& header, std::ostream& ostr);
		// Writes the message boundary std::string, followed
		// by the message header to the output stream.
		
	void writeEnd(std::ostream& ostr);
		// Writes the final boundary std::string to the output stream.

	static std::string createBoundary();
		// Creates a random boundary std::string.
		//
		// The std::string always has the form boundary-XXXXXXXXXXXX, 
		// where XXXXXXXXXXXX is a randomly generate number.

	virtual void updateProgress(int nread);
		// Updates the upload progress via the associated 
		// ClientConnection object.

	friend class FilePart;

	struct Part
	{
		std::string name;
		FilePart* part;
	};
	
	typedef std::deque<Part> PartQueue;
	
	ClientConnection& _connection;
	async::Runner::ptr _runner;
	std::string _encoding;
	std::string _boundary;
	PartQueue _parts;
	UInt64 _filesSize;
	int _writeState;
	bool _initial;
	bool _complete;
};


//
// File Part
//


class FilePart
	/// An implementation of FilePart for plain files.
{
public:
	FilePart(const std::string& path);
		// Creates the FilePart for the given path.
		//
		// The MIME type is set to application/octet-stream.
		//
		// Throws an FileException if the file cannot be opened.
	
	FilePart(const std::string& path, const std::string& contentType);
		// Creates the FilePart for the given
		// path and MIME type.
		//
		// Throws an FileException if the file cannot be opened.

	FilePart(const std::string& path, const std::string& filename, const std::string& contentType);
		// Creates the FilePart for the given
		// path and MIME type. The given filename is 
		// used as part filename (see filename()) only.
		//
		// Throws an FileException if the file cannot be opened.

	virtual ~FilePart();
		// Destroys the FilePart.

	virtual void open(const std::string& path);
		// Opens the file.
		//
		// Throws an FileException if the file cannot be opened.

	virtual bool writeChunk(FormWriter& writer);
		// Writes a form data chunk to the given HTTP client connection.
		// Returns true if there is more data to be written.

	virtual void write(FormWriter& writer);
		// Writes the form data to the given HTTP client connection.

	virtual void write(std::ostream& ostr);
		// Writes the form data to the given output stream.	
				
	NVCollection& headers();
		// Returns a NVCollection containing additional header 
		// fields for the part.	
	
	const std::string& contentType() const;
		// Returns the MIME type for this part or attachment.
		
	const std::string& filename() const;
		// Returns the filename portion of the path.
		
	std::ifstream& stream();
		// Returns the file input stream.
		
	UInt64 fileSize() const;
		// Returns the file size.

protected:
	std::string _contentType;
	std::string _filename;
	std::ifstream _istr;
	UInt64 _fileSize;
	//UInt64 _nWritten;
	NVCollection _headers;	
};


} } // namespace scy::http


#endif // SOURCEY_HTTP_Form_H


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
