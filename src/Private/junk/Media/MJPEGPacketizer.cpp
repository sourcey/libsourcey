#include "Sourcey/Media/Packetizer.h"
//#include "Sourcey/Media/Packet.h"
#include "Poco/Net/HTTPResponse.h"

#include <iostream>
#include <assert.h>


using namespace std;


namespace Sourcey {
namespace Media {


Packetizer::Packetizer() :
	_maxContentLength(1450),
	_sequenceNumber(0)
{
}


Packetizer::~Packetizer() 
{
}


void Packetizer::process(const Sourcey::Packet& packet) 
{

	if () {
		// Send the initial response header
		// TODO: Remove HTTPResponse dependency
		ostringstream ss;
		Net::HTTPResponse response;
		response.set("Server", "Spot");
		response.set("Expires", "0");
		response.set("Pragma", "no-cache");
		response.set("Cache-Control", "no-store");
		response.set("Content-Type", "multipart/x-mixed-replace; boundary=--eof");
		response.write(ss);	
		Packet packet((unsigned char *)ss.str().data(), ss.str().length());
		PacketReady.send(this, packet);
		_sequenceNumber++;	
	}

	cout << "Packetizer::process" << endl;	
	Buffer buf;
	packet.write(buf);

	//int nRead = 0;
	int nWanted = 0;
	while (!buf.eof()) {		
		nWanted = min(buf.length() - buf.position(), _maxContentLength);	
		_sequenceNumber++;	
		//Packet packet(&data[nRead], nWanted, _sequenceNumber, _timestamp, _ssrc);	
		//Packet(&buf.data(), nWanted, _sequenceNumber, _timestamp, _ssrc);	
		//nRead += nWanted;
		PacketReady.send(this, Packet(&buf.data(), nWanted, _sequenceNumber, _timestamp, _ssrc));
		buf.consume(nWanted);
	}
}


} // namespace Media
} // namespace Sourcey 