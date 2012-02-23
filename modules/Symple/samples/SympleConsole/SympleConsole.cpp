#include "Sourcey/Symple/Socket.h"
//#include "Sourcey/TaskRunner.h"
//#include "Sourcey/PacketStream.h"
#include "Sourcey/Util.h"
#include "Sourcey/HTTP/Util.h"
//#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Transaction.h"
#include "Poco/Net/HTTPBasicCredentials.h"


#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <conio.h>

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Sourcey;
using namespace Sourcey::Util;
using namespace Sourcey::Net;
using namespace Sourcey::Symple;

namespace Sourcey { 
namespace Symple {


// Detect Memory Leaks
/*
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif<DataPacket>
*/


class Messenger: public Symple::Socket
{
public:
	PacketSignal TestSignal;

	Messenger(const Symple::Socket::Options& o) :
		Symple::Socket(o)
	{
		TestSignal.attach(commandDelegate(this, &Messenger::onRecvCommand, Filter("*", AcceptResquests)));
		TestSignal.attach(messageDelegate(this, &Messenger::onRecvMessage, Filter(AcceptResquests)));
		TestSignal.attach(messageDelegate(this, &Messenger::onRecvPresence, Filter(AcceptResquests)));
	}

	~Messenger()
	{
	}

	void login(const std::string& username, const std::string& password) 
	{		
		HTTP::Request* request = new HTTP::Request("POST", "http://localhost:3000/authenticate.json");
		Poco::Net::HTTPBasicCredentials cred(username, password);
		cred.authenticate(*request);
		HTTP::Transaction* transaction = new HTTP::Transaction(request);
		transaction->TransactionComplete += delegate(this, &Messenger::onAuthenticationResponse);
		transaction->start();
	}

	void onAuthenticationResponse(void*, HTTP::Response& response)
	{	
		_options.token = Util::parseCookieItem(response.get("Set-Cookie"), "rack.session");
		Log("debug") << "################################## Auth Token: " << _options.token << endl;
		if (_options.token.empty())
			throw Exception("Authentication failed: Bad session");
		
		connect();
	}

	void onRecvCommand(Command& c)
	{
		Log("debug") << "################################## Recv Command: " << c.node() << endl;
		c.print(cout);
	}

	void onRecvMessage(Message& m)
	{
		Log("debug") << "################################## Recv Message" << endl; //: " << m.type()
	}

	void onRecvPresence(Presence& p)
	{
		Log("debug") << "################################## Recv Presence: " << p.type() << endl;
	}

};


} } // namespace Sourcey::Symple




int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	
	//Log("debug") << "[SocketIO::Socket] list: " << elems0.size() << endl;

	Symple::Socket::Options options;
	options.name = "Spot Node";
	options.owner = "admin";
	//options.token = "cfd806acc02c044f3d5cdc8ed9948e669eacbb1a77aa989e5c0150ab60b2be29";
	options.serverAddr = Sourcey::Net::Address("localhost", 1337);



	//Sourcey::Net::Address srvAddr("localhost", 1337);
	Messenger client(options);

	Symple::Command c;
	c.setNode("badddd");
	client.TestSignal.dispatch(&client, c);


	//system("pause");
	//return 0;

	client.login("admin", "inusuvup");
	//Messenger client(options);	
	//client.setCredentials("admin", "inusuvup");
	//client.httpHeaders().set("X-Session-ID", "79df0fe289d33d755e0301fd46c9976b163bcd390803eb799e6383fe113deb22");
	//client.httpHeaders().set("X-Session-ID", "79df0fe289d33d755e0301fd46c9976b163bcd390803eb799e6383fe113deb22");
	//client.httpHeaders().set("Cookie", "_anionu_session=79df0fe289d33d755e0301fd46c9976b163bcd390803eb799e6383fe113deb22;");
	//client.httpHeaders().set("Cookie", "rack.session=4c791033bdb96e46fbb0a7f66d4c27088dc316b2d03a471b02c7e87a95a61930;_anionu_account=4");

	string network = "4";
	char o = ' ';
	while (o != 'Q') 
	{	
		try {
			cout << 
				"COMMANDS:\n\n"
				"  C	Connect / Reconnect to server.\n"
				"  E	Set Network.\n"
				//"  A	Announce.\n"
				"  1	Connect To Network.\n"
				"  M	Send Message to Client 1.\n"
				"  N	Send Message to Client 2.\n"
				//"  J	Send JSON Message.\n"
				//"  X	Send XMPP Message.\n"
				//"  U	Call Upload Asset Service.\n"
				//"  L	List Services.\n"
				//"  K	List Remote Packages.\n"
				//"  R	Reload Service List.\n"
				//"  I	Install Packages.\n"
				//"  U	Uninstall Packages.\n"
				//"  D	Update All Packages.\n"
				"  Q	Quit.\n\n";
			
		
			// Connect / Reconnect
			if (o == 'C') {	
				client.close();
				client.connect();
			}
		
			// Set Input File
			if (o == 'E') {	
				cout << "Enter network name: " << endl;
				getline(cin, network);
			}
		
			// Send Message
			else if (o == '1') {
				client.sendConnect(network);
			} 

			// Send Announce
			else if (o == 'A') {
				//Message m;
				//m.setTo("client1");
				//m.addData("client1", "client1");
				//client.emit("announce", "{\"username\":\"admin\",\"name\":\"Admin Dude\",\"account\":\"4\"}", true);
			} 		
		
			// Send Message 1
			else if (o == 'M') {
				Message m;
				m.setTo("client1");
				m.addData("client1", "client1");
				client.send(m);
				//client.send(Symple::Packet::Message, "wassssupppp!", true);
			} 
		
			// Send Message 2
			else if (o == 'N') {
				Message m;
				m.setTo("client2");
				m.addData("client2", "client2");
				client.send(m);
				//client.send(Symple::Packet::JSON, "{\"name\":\"value\"}", true);
			} 
		
			/*
			// Send XMPP Message
			else if (o == 'X') {
				client.emit("xmpp", "{\"name\":\"value\"}", true);
			} 
		
			// Call Upload Asset Service
			else if (o == 'U') {

				Path file("D:/test.avi");
			
				HTMLForm* form = new HTMLForm(HTMLForm::ENCODING_MULTIPART);
				form->set("asset[type]", "Video");
				form->addPart("asset[file]", new FilePartSource(file.toString()));

				APITransaction* transaction = client.upload("UploadAsset", form); //file.toString(), 
				transaction->APITransactionComplete += delegate(&client, &Messenger::onAPITransactionComplete);
				transaction->start();
			} 

			// List Services
			else if (o == 'L') {
				client.files().print(cout);
			} 

			// Reload Service List
			else if (o == 'R') {
				client.loadServices();
			} 
			*/
			
		
			o = toupper(getch());
		} 
		catch (Poco::Exception& e) 
		{			
			cerr << e.displayText() << endl;
		}		
	}


	return 0;
}

	
	/*
	Json::Value data = "{\"id\":\"5859594481248190527\",\"name\":\"Kam Low\",\"type\":\"Symple\"}";
	
	Json::StyledWriter writer;
	cout << writer.write(data);
		

	//string s ="16257489491607376529:15:25:websocket,htmlfile,xhr-polling,jsonp-polling,aaaaaaaaaaaaaaaaaaaaaaaaaa";
	string s ="::ba::a";
	StringList elems0;
	StringList list = split0(s, ':', elems0);	
		Log("debug") << "[SocketIO::Socket] list: " << elems0.size() << endl;
	for (int i = 0; i < elems0.size(); i++) {
		Log("debug") << "[SocketIO::Socket] elems0: " << elems0[i] << endl;
	}

	StringList elems;
	split1(s, ":", elems);	
		Log("debug") << "[SocketIO::Socket] list1: " << elems.size() << endl;
		
	for (int i = 0; i < elems.size(); i++) {
		Log("debug") << "[SocketIO::Socket] elems: " << elems[i] << endl;
	}
		
	//Log("debug") << "[SocketIO::Socket] --------------------------------------------: " << elems.size() << endl;
   // std::string s("Somewhere down the road");


	
	//Log("debug") << "[SocketIO::Socket] --------------------------------------------: " << elems.size() << endl;

		
    istringstream iss(s);
    copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             ostream_iterator<string>(cout, "\n"));


	for (int i = 0; i < list.size(); i++) {
		Log("debug") << "[SocketIO::Socket] list: " << list[i] << endl;
	}
	StringList list1 = Util::split(s, ",", 2);	
	for (int i = 0; i < list1.size(); i++) {
		Log("debug") << "[SocketIO::Socket] list1: " << list1[i] << endl;
	}
	StringList list2 = Util::split(s, ":");	
	for (int i = 0; i < list2.size(); i++) {
		Log("debug") << "[SocketIO::Socket] list1: " << list2[i] << endl;
	}
	*/