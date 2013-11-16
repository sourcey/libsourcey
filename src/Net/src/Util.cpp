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


#include "scy/net/util.h"


using namespace std;


namespace scy {
namespace util {


/*
// ---------------------------------------------------------------------
// Socket Tools
//


// Checks if a port local number is available and returns a boolean value.
bool portIsAvailable(int port) {
	bool res = false;
	ServerSocket serv;
	try {
		serv.bind(port);
		serv.listen();
		res = true;
	} catch (...) {}
	serv.close();
	return res;
}


// Checks if a network address is accessible from this device.
bool addressIsOK(const std::string& str, bool whiny)
{
	try 
	{
		std::string response;
		URI str(str);
		HTTPClientSession session(str.getHost(), str.getPort());
		session.setTimeout(2000000);
		HTTPRequest req(HTTPRequest::HTTP_GET, str.getPathAndQuery(), HTTPMessage::HTTP_1_1);
		session.sendRequest(req);
		HTTPResponse res;
		istream& rs = session.receiveResponse(res);
		StreamCopier::copyToString(rs,response);
		return response.empty() == false;
	}
	catch (std::exception& exc)
	{
		if (whiny)
			throw exc;
	}
	return false;
}


bool internetIsOK(bool whiny)
{
	bool res = false;
	try 
	{
		res = scy::addressIsOK("http://www.google.com");
	}
	catch (std::exception&)
	{
		if (whiny)
			throw std::runtime_error("No internet connection available.");
	}
	return res;
}


// Returns the remote/public IP address of the current network.
//
// The only truly reliable way of achieving this is through a public anio::api.
// There are a couple of services available for this purpose:
//		1) http://www.whatismyip.com/automation/n09230945.asp (IP) 
//		2) http://checkip.dyndns.org/ (IP) 
//		3) http://dynamic.zoneedit.com/checkip.html (IP & Hostname) 
//		4) http://ip-address.domaintools.com/myip.xml (Full host information) 
//
string getPublicIPAddress(bool whiny)
{
	IPAddress addr;
	std::string response;
	std::string strs[3] = { 
		"http://checkip.dyndns.org/", 
		"http://ip-address.domaintools.com/myip.xml", 
		"http://www.whatismyip.com/automation/n09230945.asp"

	};

	// Loop through the services until we have what we need
	for (int i = 0; i < 3; i++) 
	{
		try
		{
			response = "";
			URI str(strs[i]);
			HTTPClientSession session(str.getHost(), str.getPort());
			HTTPRequest req(HTTPRequest::HTTP_GET, str.getPathAndQuery(), HTTPMessage::HTTP_1_1);
			session.setTimeout(Timespan(2,0));
			session.sendRequest(req);
			HTTPResponse res;
			istream& rs = session.receiveResponse(res);
			StreamCopier::copyToString(rs,response);

			replaceInPlace(response,"\n","");					
			replaceInPlace(response," ","");
			string::size_type start = 0;
			string::size_type end = 0;

			switch(i) 
			{
				case 0:
					start = response.find("Address:");
					if (start != std::string::npos) {
						start += 8;
						end = response.find("</body>", start);
						response = response.substr(start, end-start);
					}
					addr = IPAddress(response);
					goto ok;
					break;
				case 1:
					start = response.find("<ip_address>");
					if (start != std::string::npos) {
						start += 12;
						end = response.find("</ip_address>", start);
						response = response.substr(start, end-start);
					}
					addr = IPAddress(response);
					goto ok;
					break;
				case 2:
					addr = IPAddress(response);
					goto ok;
					break;
			}
		}
		catch (std::exception&/Exception& exc)
		{
			errorL() << exc.what()/message()/ << endl;
		}
	}

	if (whiny)
		throw std::runtime_error("Cannot determine the public IP address of this computer.");
	else 
		return "";

ok:
	return addr.toString();
}
*/


} // namespace util
} // namespace scy