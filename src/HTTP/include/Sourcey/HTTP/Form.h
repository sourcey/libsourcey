#ifndef SOURCEY_HTTP_Form_H
#define SOURCEY_HTTP_Form_H


#include "Sourcey/Containers.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Net/Types.h"


namespace scy {
namespace http {


class Request;
//class PartSource;
class Connection;
class FormWriter;


class PartSource
	/// The abstract class used for uploading multipart HTML  
	/// form files or mail messages attachments.
{
public:
	virtual void write(FormWriter& writer) = 0;
		/// Writes the form data to the given HTTP client connection.

	virtual void write(std::ostream& ostr) = 0;
		/// Writes the form data to the given output stream.
		
	virtual std::string filename();
		/// Returns the filename for the part or attachment.
		///
		/// May be overridden by subclasses. The default
		/// implementation returns an empty string.
	
	const std::string& contentType() const;
		/// Returns the MIME type for this part or attachment.
				
	NVCollection& headers();
		/// Returns a NVCollection containing additional 
		/// header fields for the part.

	virtual ~PartSource();
		/// Destroys the PartSource.
	
protected:
	PartSource();
		/// Creates the PartSource, using
		/// the application/octet-stream MIME type.
		
	PartSource(const std::string& contentType);
		/// Creates the PartSource, using the
		/// given MIME type.

private:
	PartSource(const PartSource&);
	PartSource& operator = (const PartSource&);

	std::string _contentType;
	NVCollection _headers;
};


//
// File Part
//


class FilePart: public PartSource
	/// An implementation of PartSource for plain files.
{
public:
	FilePart(const std::string& path);
		/// Creates the FilePart for the given path.
		///
		/// The MIME type is set to application/octet-stream.
		///
		/// Throws an OpenFileException if the file cannot be opened.
	
	FilePart(const std::string& path, const std::string& contentType);
		/// Creates the FilePart for the given
		/// path and MIME type.
		///
		/// Throws an OpenFileException if the file cannot be opened.

	FilePart(const std::string& path, const std::string& filename, const std::string& contentType);
		/// Creates the FilePart for the given
		/// path and MIME type. The given filename is 
		/// used as part filename (see filename()) only.
		///
		/// Throws an OpenFileException if the file cannot be opened.

	~FilePart();
		/// Destroys the FilePart.

	virtual void write(FormWriter& writer);
		/// Writes the form data to the given HTTP client connection.

	virtual void write(std::ostream& ostr);
		/// Writes the form data to the given output stream.
		
	std::string filename();
		/// Returns the filename portion of the path.

private:
	std::string _filename;
	std::ifstream _istr;
};


//
// HTML Form Writer
//


class FormWriter: public NVCollection, public RunnablePacketSource
	/// FormWriter is a helper class for working with HTML 
	/// forms on the client side.
{
public:
	FormWriter(Connection& conn, const std::string& encoding = FormWriter::ENCODING_URL);
		/// Creates the FormWriter that uses the given encoding.
		///
		/// Encoding must be either "application/x-www-form-urlencoded"
		/// (which is the default) or "multipart/form-data".
		
	~FormWriter();
		/// Destroys the FormWriter.

	void addPart(const std::string& name, PartSource* part);
		/// Adds an part/attachment (file upload) to the form.
		///
		/// The form takes ownership of the PartSource and deletes it
		/// when it is no longer needed.
		///
		/// The part will only be sent if the encoding
		/// set for the form is "multipart/form-data"
	
	void run();
		/// Begins writing the form data to the HTTP client connection.
		
	void prepareSubmit(http::Request& request);
		/// Fills out the request object for submitting the form.
		///
		/// If the request method is GET, the encoded form is appended to the
		/// request URI as query string. Otherwise (the method is
		/// POST), the form's content type is set to the form's encoding.
		/// The form's parameters must be written to the
		/// request body separately, with a call to write.
		/// If the request's HTTP version is HTTP/1.0:
		///    - persistent connections are disabled
		///    - the content transfer encoding is set to identity encoding
		/// Otherwise, if the request's HTTP version is HTTP/1.1:
		///    - the request's persistent connection state is left unchanged
		///    - the content transfer encoding is set to chunked

	void writeUrl(std::ostream& ostr);
		/// Writes "application/x-www-form-urlencoded" 
		/// encoded data to the given output stream.

	void writeMultipart(); //Connection& conn
		/// Writes "multipart/form-data" encoded data 
		/// to the given client connection.

	void setEncoding(const std::string& encoding);
		/// Sets the encoding used for posting the form.
		///
		/// Encoding must be either "application/x-www-form-urlencoded"
		/// (which is the default) or "multipart/form-data".
		
	const std::string& encoding() const;
		/// Returns the encoding used for posting the form.

	void setBoundary(const std::string& boundary);
		/// Sets the boundary to use for separating form parts.

	const std::string& boundary() const;
		/// Returns the MIME boundary used for writing
		/// multipart form data.

	static const std::string ENCODING_URL;       /// "application/x-www-form-urlencoded"
	static const std::string ENCODING_MULTIPART; /// "multipart/form-data"
	
protected:
	FormWriter(const FormWriter&);
	FormWriter& operator = (const FormWriter&);
		
	void writePartHeader(const NVCollection& header, std::ostream& ostr);
		/// Writes the message boundary string, followed
		/// by the message header to the output stream.
		
	void writeEnd(std::ostream& ostr);
		/// Writes the terminating boundary string to
		/// the output stream.

	static std::string createBoundary();
		/// Creates a random boundary string.
		///
		/// The string always has the form
		/// MIME_boundary_XXXXXXXXXXXX, where
		/// XXXXXXXXXXXX is a random hexadecimal
		/// number.

	struct Part
	{
		std::string name;
		PartSource* part;
	};
	
	typedef std::vector<Part> PartVec;
	
	bool _initial;
	Connection& _connection;
	std::string _encoding;
	std::string _boundary;
	PartVec     _parts;
};



} } // namespace scy::http


#endif // SOURCEY_HTTP_Form_H




	
/*
class MultipartWriter //Net_API
	/// This class is used to write MIME multipart
	/// messages to an output stream.
	///
	/// The format of multipart messages is described
	/// in section 5.1 of RFC 2046.
	///
	/// To create a multipart message, first create
	/// a MultipartWriter object.
	/// Then, for each part, call writePartHeader() and
	/// write the content to the output stream.
	/// Repeat for all parts. 
	/// After the last part has been written,
	/// call close() to finish the multipart message.
{
public:
	explicit MultipartWriter(std::ostream& ostr);
		/// Creates the MultipartWriter, using the
		/// given output stream.
		///
		/// Creates a random boundary string.

	MultipartWriter(std::ostream& ostr, const std::string& boundary);
		/// Creates the MultipartWriter, using the
		/// given output stream and boundary string.

	~MultipartWriter();
		/// Destroys the MultipartWriter.
		
	void writePartHeader(const NVCollection& header);
		/// Opens a new message part and writes
		/// the message boundary string, followed
		/// by the message header to the stream.
		
	void close();
		/// Closes the multipart message and writes
		/// the terminating boundary string.
		///
		/// Does not close the underlying stream.

	std::ostream& stream();
		/// Returns the writer's stream.

	const std::string& boundary() const;
		/// Returns the multipart boundary used by this writer.

	static std::string createBoundary();
		/// Creates a random boundary string.
		///
		/// The string always has the form
		/// MIME_boundary_XXXXXXXXXXXX, where
		/// XXXXXXXXXXXX is a random hexadecimal
		/// number.
		
private:
	MultipartWriter();
	MultipartWriter(const MultipartWriter&);
	MultipartWriter& operator = (const MultipartWriter&);

	std::ostream& _ostr;
	std::string   _boundary;
	bool          _firstPart;
};


//
// inlines
//
inline std::ostream& MultipartWriter::stream()
{
	return _ostr;
}
*/




/*
inline NVCollection& PartSource::headers()
{
	return _headers;
}


inline const NVCollection& PartSource::headers() const
{
	return _headers;
}
*/
		
	/*
protected:
	//void readUrl(std::istream& istr);
	//void readMultipart(std::istream& istr, PartHandler& handler);
	FormWriter(const http::Request& request, std::istream& requestBody, PartHandler& handler);
		/// Creates a FormWriter from the given HTTP request.
		///
		/// Uploaded files are passed to the given PartHandler.

	FormWriter(const http::Request& request, std::istream& requestBody);
		/// Creates a FormWriter from the given HTTP request.
		///
		/// Uploaded files are silently discarded.

	explicit FormWriter(const http::Request& request);
		/// Creates a FormWriter from the given HTTP request.
		///
		/// The request must be a GET request and the form data
		/// must be in the query string (URL encoded).
		///
		/// For POST requests, you must use one of the constructors
		/// taking an additional input stream for the request body.
		*/

	//int getFieldLimit() const;
		/// Returns the maximum number of header fields
		/// allowed.
		///
		/// See setFieldLimit() for more information.
		
	//void setFieldLimit(int limit);
		/// Sets the maximum number of header fields
		/// allowed. This limit is used to defend certain
		/// kinds of denial-of-service attacks.
		/// Specify 0 for unlimited (not recommended).
		///
		/// The default limit is 100.


/*
inline int FormWriter::getFieldLimit() const
{
	return _fieldLimit;
}
*/


	/*
	void load(const http::Request& request, std::istream& requestBody, PartHandler& handler);
		/// Reads the form data from the given HTTP request.
		///
		/// Uploaded files are passed to the given PartHandler.

	void load(const http::Request& request, std::istream& requestBody);
		/// Reads the form data from the given HTTP request.
		///
		/// Uploaded files are silently discarded.

	void load(const http::Request& request);
		/// Reads the form data from the given HTTP request.
		///
		/// The request must be a GET request and the form data
		/// must be in the query string (URL encoded).
		///
		/// For POST requests, you must use one of the overloads
		/// taking an additional input stream for the request body.

	void read(std::istream& istr, PartHandler& handler);
		/// Reads the form data from the given input stream.
		///
		/// The form data read from the stream must be
		/// in the encoding specified for the form.
		///
		/// Note that read() does not clear the form before
		/// reading the new values.

	void read(std::istream& istr);
		/// Reads the URL-encoded form data from the given input stream.
		///
		/// Note that read() does not clear the form before
		/// reading the new values.
		
	void read(const std::string& queryString);
		/// Reads the form data from the given HTTP query string.
		///
		/// Note that read() does not clear the form before
		/// reading the new values.
		*/