#include "Sourcey/HTTP/Form.h"
#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/URL.h"
//#include "Poco/Net/NVCollection.h"
#include "Poco/File.h"
#include "Poco/Random.h"
#include "Poco/NumberFormatter.h"



namespace scy {
namespace http {


//
// Part Source
//


PartSource::PartSource():
	_contentType("application/octet-stream")
{
}

	
PartSource::PartSource(const std::string& contentType):
	_contentType(contentType)
{
}


PartSource::~PartSource()
{
}


std::string PartSource::filename()
{
	return "";
}


NVCollection& PartSource::headers()
{
	return _headers;
}
	

const std::string& PartSource::contentType() const
{	
	return _contentType; 
}


//
// File Part Source
//


std::string getFileName(const std::string& s) {

   char sep = '/';

#ifdef _WIN32
   // FIXME: Not all paths will be in this format
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length( ));
   if (i != std::string::npos) {
      return(s.substr(i+1, s.length( ) - i));
   }

   return "";
}

	
FilePart::FilePart(const std::string& path):
	_istr(path.data(), std::ios::in | std::ios::binary)
{
	//Path p(path);
	_filename = getFileName(path); //p.getFileName();
	if (!_istr.is_open())
		throw OpenFileException(path);
}


FilePart::FilePart(const std::string& path, const std::string& contentType):
	PartSource(contentType),
	_istr(path.data(), std::ios::in | std::ios::binary)
{
	//Path p(path);
	//_filename = p.getFileName();
	_filename = getFileName(path); 
	if (!_istr.is_open())
		throw OpenFileException(path);
}


FilePart::FilePart(const std::string& path, const std::string& filename, const std::string& contentType):
	PartSource(contentType),
	_filename(filename),
	_istr(path.data(), std::ios::in | std::ios::binary)
{
	//Path p(path);
	if (!_istr.is_open())
		throw OpenFileException(path);
}


FilePart::~FilePart()
{
}


void FilePart::write(FormWriter& writer)
{
	traceL("FilePart", this) << "Write" << std::endl;

	char buffer[32384];

	// Send file chunks to the peer
	while (_istr.read(buffer, 32384)) {
		traceL("FilePart", this) << "Write chunk: " << _istr.gcount() << std::endl;
		writer.emit(buffer, (size_t)_istr.gcount());
	}

	if (_istr.eof()) {
		// Still a few bytes left to write?
		if (_istr.gcount() > 0) {
			traceL("FilePart", this) << "Write trailing chunk: " << _istr.gcount() << std::endl;
			writer.emit(buffer, (size_t)_istr.gcount());
		}
	}
	else if (_istr.bad())
		throw IOException("Cannot read multipart source file", _filename);
}


void FilePart::write(std::ostream& ostr)
{
	traceL("FilePart", this) << "Write" << std::endl;

	char buffer[32384];

	// Send file chunks to the peer
	while (_istr.read(buffer, 32384)) {
		traceL("FilePart", this) << "Write chunk: " << _istr.gcount() << std::endl;
		ostr.write(buffer, (size_t)_istr.gcount());
		//writer.emit(buffer, (size_t)_istr.gcount());
	}

	if (_istr.eof()) {
		// Still a few bytes left to write?
		if (_istr.gcount() > 0) {
			traceL("FilePart", this) << "Write trailing chunk: " << _istr.gcount() << std::endl;
			//writer.emit(buffer, (size_t)_istr.gcount());
			ostr.write(buffer, (size_t)_istr.gcount());
		}
	}
	else if (_istr.bad())
		throw IOException("Cannot read multipart source file", _filename);
}


std::string FilePart::filename()
{
	return _filename;
}


//
// Form Writer
//


const std::string FormWriter::ENCODING_URL       = "application/x-www-form-urlencoded";
const std::string FormWriter::ENCODING_MULTIPART = "multipart/form-data";


FormWriter::FormWriter(Connection& conn, const std::string& encoding) :
	_encoding(encoding),
	_initial(true),
	_connection(conn)
{
}

	
FormWriter::~FormWriter()
{
	for (PartVec::iterator it = _parts.begin(); it != _parts.end(); ++it)
		delete it->part;
}


void FormWriter::addPart(const std::string& name, PartSource* part)
{
	assert(part);

	Part p;
	p.name = name;
	p.part = part;
	_parts.push_back(p);
}


void FormWriter::prepareSubmit(http::Request& request)
{
	if (request.getMethod() == http::Request::HTTP_POST || 
		request.getMethod() == http::Request::HTTP_PUT) {
		if (_encoding == ENCODING_URL) {
			request.setContentType(_encoding);
			request.setChunkedTransferEncoding(false);
			std::ostringstream ostr;
			writeUrl(ostr);
			request.setContentLength((int)ostr.tellp());
		}
		else {
			if (_boundary.empty())
				_boundary = createBoundary();
			std::string ct(_encoding);
			ct.append("; boundary=\"");
			ct.append(_boundary);
			ct.append("\"");
			request.setContentType(ct);
		}
		if (request.getVersion() == http::Message::HTTP_1_0) {
			request.setKeepAlive(false);
			request.setChunkedTransferEncoding(false);
		}
		else if (_encoding != ENCODING_URL) {
			request.setChunkedTransferEncoding(true);
		}
	}
	else {
		std::string uri = request.getURI();
		std::ostringstream ostr;
		writeUrl(ostr);
		uri.append("?");
		uri.append(ostr.str());
		request.setURI(uri);
	}
}


void FormWriter::run()
{
	prepareSubmit(_connection.request());
	if (_encoding == ENCODING_URL) {
		std::ostringstream ostr;
		writeUrl(ostr);		
		emit(ostr.str());
	} else
		writeMultipart();
}


void FormWriter::writeUrl(std::ostream& ostr)
{
	for (NVCollection::ConstIterator it = begin(); it != end(); ++it) {
		if (it != begin()) ostr << "&";
		ostr << URL::encode(it->first) << "=" << URL::encode(it->first);
	}
}
		//std::string name;
		//assert(0);
		//URI::encode(it->first, "=&+;", name);
		//std::string value;
		//URI::encode(it->second, "=&+;", value);


void FormWriter::writePartHeader(const NVCollection& header, std::ostream& ostr)
{
	if (_initial)
		_initial = false;
	else
		ostr << "\r\n";
	ostr << "--" << _boundary << "\r\n";
	/*
	*/
	NVCollection::ConstIterator it = header.begin();
	while (it != header.end()) {
		ostr << it->first << ": " << it->second << "\r\n";
		++it;
	}

	ostr << "\r\n";
}

	
void FormWriter::writeEnd(std::ostream& ostr)
{
	ostr << "\r\n--" << _boundary << "--\r\n";
}


void FormWriter::writeMultipart()
{
	for (NVCollection::ConstIterator it = begin(); it != end(); ++it) {
		std::ostringstream ostr;
		NVCollection header;
		std::string disp("form-data; name=\"");
		disp.append(it->first);
		disp.append("\"");
		header.set("Content-Disposition", disp);
		writePartHeader(header, ostr);
		ostr << it->second;		
		emit(ostr.str());
	}	
	for (PartVec::iterator ita = _parts.begin(); ita != _parts.end(); ++ita) {
		std::ostringstream ostr;
		NVCollection header(ita->part->headers());
		std::string disp("form-data; name=\"");
		disp.append(ita->name);
		disp.append("\"");
		std::string filename = ita->part->filename();
		if (!filename.empty()) {
			disp.append("; filename=\"");
			disp.append(filename);
			disp.append("\"");
		}
		header.set("Content-Disposition", disp);
		header.set("Content-Type", ita->part->contentType());
		writePartHeader(header, ostr);		
		emit(ostr.str());		
		ita->part->write(*this);
	}
	std::ostringstream ostr;
	writeEnd(ostr);
	emit(ostr.str());

	//_connection.close();
	//std::string body(ostr.str());	
	//traceL("#############", this) << "Connected: " << body << std::endl;
	
	//_connection.request().setKeepAlive(false);
	//_connection.request().setChunkedTransferEncoding(false);
	//_connection.request().setContentLength(body.length());
	
	//emit(body);
	//emit("0\r\n\r\n", 5);

	_connection.write("0\r\n\r\n", 5, 0);

	//_connection.sendHeaders();
	//_connection.write(body);
	
	/*
	std::string chunk;
	std::ostringstream ss;
	ss << std::hex << body.size();
	chunk += ss.str();
	chunk += "\r\n";
	//emit(this, RawPacket(ss.str().data(), ss.str().length()));
	//emit(this, RawPacket("\r\n", 2));
	//emit(this, RawPacket(raw->data(), raw->size()));
	//emit(this, RawPacket("\r\n", 2)); // straight to Socket::send
	*/
	
	//_connection.write(chunk);
	//_connection.write(body);

	//_connection.write(std::string("\r\n", 2));
	//_connection.send();
	//emit(body);
	//conn.send();
	//emit(ostr.str());
	//conn.write(ostr.str());
	//_boundary = writer.boundary();
}


std::string FormWriter::createBoundary()
{
	std::string boundary("MIME_boundary_");
	Poco::Random rnd;
	Poco::NumberFormatter::appendHex(boundary, rnd.next(), 8);
	Poco::NumberFormatter::appendHex(boundary, rnd.next(), 8);
	return boundary;
}


void FormWriter::setEncoding(const std::string& encoding)
{
	_encoding = encoding;
}


void FormWriter::setBoundary(const std::string& boundary)
{
	_boundary = boundary;
}


const std::string& FormWriter::encoding() const
{
	return _encoding;
}


const std::string& FormWriter::boundary() const
{
	return _boundary;
}


} } // namespace scy::http



/*
void FormWriter::write(Connection& conn)
{
	if (_encoding == ENCODING_URL) {
		std::ostringstream ostr;
		writeUrl(ostr);		
		conn.write(ostr.str());
	} else
		writeMultipart(conn);
}
*/


	
	/*
MultipartWriter::MultipartWriter(
	Connection& _connection) : //
	_ostr(ostr),
	_boundary(createBoundary()),
	_initial(true)
{
}


MultipartWriter::MultipartWriter(std::ostream& ostr, const std::string& boundary):
	_ostr(ostr),
	_boundary(boundary),
	_initial(true)
{
	if (_boundary.empty())
		_boundary = createBoundary();
}


MultipartWriter::~MultipartWriter()
{
}

	
void MultipartWriter::writePartHeader(const NVCollection& header)
{
	if (_initial)
		_initial = false;
	else
		_ostr << "\r\n";
	_ostr << "--" << _boundary << "\r\n";
	assert(0);
	//header.write(_ostr);
	_ostr << "\r\n";
}


const std::string& MultipartWriter::boundary() const
{
	return _boundary;
}


std::string MultipartWriter::createBoundary()
{
	std::string boundary("MIME_boundary_");
	Poco::Random rnd;
	Poco::NumberFormatter::appendHex(boundary, rnd.next(), 8);
	Poco::NumberFormatter::appendHex(boundary, rnd.next(), 8);
	return boundary;
}
*/


	


/*
FormWriter::FormWriter(const http::Request& request, std::istream& requestBody, PartHandler& handler):
	_fieldLimit(DFL_FIELD_LIMIT)
{
	load(request, requestBody, handler);
}


FormWriter::FormWriter(const http::Request& request, std::istream& requestBody):
	_fieldLimit(DFL_FIELD_LIMIT)
{
	load(request, requestBody);
}


FormWriter::FormWriter(const http::Request& request):
	_fieldLimit(DFL_FIELD_LIMIT)
{
	load(request);
}
*/



/*
void FormWriter::readUrl(std::istream& istr)
{
	static const int eof = std::char_traits<char>::eof();

	int fields = 0;
	int ch = istr.get();
	while (ch != eof)
	{
		if (_fieldLimit > 0 && fields == _fieldLimit)
			throw FormWriterException("Too many form fields");
		std::string name;
		std::string value;
		while (ch != eof && ch != '=' && ch != '&')
		{
			if (ch == '+') ch = ' ';
			name += (char) ch;
			ch = istr.get();
		}
		if (ch == '=')
		{
			ch = istr.get();
			while (ch != eof && ch != '&')
			{
				if (ch == '+') ch = ' ';
				value += (char) ch;
				ch = istr.get();
			}
		}
		std::string decodedName;
		std::string decodedValue;
		URI::decode(name, decodedName);
		URI::decode(value, decodedValue);
		add(decodedName, decodedValue);
		++fields;
		if (ch == '&') ch = istr.get();
	}
}


void FormWriter::readMultipart(std::istream& istr, PartHandler& handler)
{
	static const int eof = std::char_traits<char>::eof();

	int fields = 0;
	MultipartReader reader(istr, _boundary);
	while (reader.hasNextPart())
	{
		if (_fieldLimit > 0 && fields == _fieldLimit)
			throw FormWriterException("Too many form fields");
		NVCollection header;
		reader.writePartHeader(header);
		std::string disp;
		NVCollection params;
		if (header.has("Content-Disposition"))
		{
			std::string cd = header.get("Content-Disposition");
			NVCollection::splitParameters(cd, disp, params);
		}
		if (params.has("filename"))
		{
			handler.handlePart(header, reader.stream());
			// Ensure that the complete part has been read.
			while (reader.stream().good()) reader.stream().get();
		}
		else
		{
			std::string name = params["name"];
			std::string value;
			std::istream& istr = reader.stream();
			int ch = istr.get();
			while (ch != eof)
			{
				value += (char) ch;
				ch = istr.get();
			}
			add(name, value);
		}
		++fields;
	}
}
*/

/*

void FormWriter::load(const http::Request& request, std::istream& requestBody, PartHandler& handler)
{
	clear();
	if (request.getMethod() == http::Request::HTTP_POST || request.getMethod() == http::Request::HTTP_PUT)
	{
		std::string contentType;
		NVCollection params;
		NVCollection::splitParameters(request.getContentType(), contentType, params); 
		_encoding = contentType;
		if (_encoding == ENCODING_MULTIPART)
		{
			_boundary = params["boundary"];
			readMultipart(requestBody, handler);
		}
		else
		{
			readUrl(requestBody);
		}
	}
	else
	{
		URI uri(request.getURI());
		std::istringstream istr(uri.getRawQuery());
		readUrl(istr);
	}
}


void FormWriter::load(const http::Request& request, std::istream& requestBody)
{
	NullPartHandler nah;
	load(request, requestBody, nah);
}


void FormWriter::load(const http::Request& request)
{
	NullPartHandler nah;
	NullInputStream nis;
	load(request, nis, nah);
}


void FormWriter::read(std::istream& istr, PartHandler& handler)
{
	if (_encoding == ENCODING_URL)
		readUrl(istr);
	else
		readMultipart(istr, handler);
}


void FormWriter::read(std::istream& istr)
{
	readUrl(istr);
}


void FormWriter::read(const std::string& queryString)
{
	std::istringstream istr(queryString);
	readUrl(istr);
}

*/

/*
void FormWriter::setFieldLimit(int limit)
{
	poco_assert (limit >= 0);
	
	_fieldLimit = limit;
}
*/

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
