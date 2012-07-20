#ifndef ANIONU_JINGLE_DATA_PROVIDER_H
#define ANIONU_JINGLE_DATA_PROVIDER_H


#include "Sourcey/Util/UDPSocket.h"
#include <vector>
#include <string>


namespace Sourcey {


class DataProviderUDP: public UDPSocket {
public:
	DataProviderUDP(
		const Poco::Net::SocketAddress& localAddr, 
		const std::string& name, 
		const std::string& type,
		const std::string& profile = "RTP/AVP");
	virtual ~DataProviderUDP();

	virtual std::string rtpMediaType();
	virtual std::string rtpProfile();
	virtual std::string name();

private:
	std::string _name;
	std::string _type;
	std::string _profile;
};


typedef std::vector<DataProviderUDP*> DataProviderUDPList;


} // namespace Sourcey


#endif // ANIONU_JINGLE_DATA_PROVIDER_H