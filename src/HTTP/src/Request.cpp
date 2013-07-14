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


#include "Sourcey/HTTP/Request.h"
//#include "Sourcey/HTTP/Authenticator.h"
#include "Sourcey/HTTP/Util.h"

#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"

#include <assert.h>


using namespace std;
using namespace Poco;



namespace scy { 
namespace http {


Request::Request() : 
	Poco::Net::HTTPRequest(HTTPMessage::HTTP_1_1), form(NULL)
{
}


Request::Request(const string& version) : 
	Poco::Net::HTTPRequest(version), form(NULL)
{
}


Request::Request(const string& method, const string& uri) : 
	Poco::Net::HTTPRequest(method, uri, HTTPMessage::HTTP_1_1), form(NULL)
{
}


Request::Request(const string& method, const string& uri, const string& version) : 
	Poco::Net::HTTPRequest(method, uri, version), form(NULL)
{
}


Request::~Request()
{
	if (form) delete form;
}


void Request::prepare()
{
	assert(!getMethod().empty());
	assert(!getURI().empty());

	string date = DateTimeFormatter::format(Timestamp(), DateTimeFormat::RFC822_FORMAT);
	set("Date", date);	
	set("User-Agent", "Sourcey C++ API");
	if (getMethod() == "POST" || 
		getMethod() == "PUT") {
		if (form) {
			form->prepareSubmit(*this);	
			form->write(body);
			streambuf* pbuf = body.rdbuf();
			long contentLength = (long)pbuf->pubseekoff(0, ios_base::end);
			assert(contentLength > 0);
			setContentLength(contentLength);
			setChunkedTransferEncoding(false);
			pbuf->pubseekpos(0);
		}
		else
			setContentLength(body.str().length());
	}
}


void Request::read(istream& istr)
{
	Poco::Net::HTTPRequest::read(istr);
	util::parseURIQuery(getURI(), _params);
}

			
const KVStore& Request::params() const
{	
	return _params;
}


bool Request::matches(const string& expression) const
{
	return util::matchURI(getURI(), expression);
}


} } // namespace scy::http