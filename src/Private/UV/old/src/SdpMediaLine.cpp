#include "Sourcey/UV/SdpMediaLine.h"


using namespace std;
using namespace Sourcey;
using namespace Sourcey::Util;
using namespace Sourcey::UV;


const char* SdpMediaLine::SdpMediaTypeString[] =
{
	"NONE",
	"UNKNOWN",
	"AUDIO",
	"VIDEO",
	"TEXT",
	"APPLICATION",
	"MESSAGE"
};

const char* SdpMediaLine::SdpTransportProtocolTypeString[] =
{
	"NONE",
	"UNKNOWN",
	"UDP",
	"RTP/AVP",
	"RTP/SAVP",
	"RTP/SAVPF",
	"TCP",
	"TCP/RTP/AVP",
	"TCP/TLS",
	"UDP/TLS",
	"DCCP/TLS",
	"DCCP/TLS/RTP/SAVP",
	"UDP/TLS/RTP/SAVP",
	"TCP/TLS/RTP/SAVP"
};

const char* SdpMediaLine::SdpEncryptionMethodString[] =
{
	"NONE",
	"CLEAR",
	"BASE64",
	"URI",
	"PROMPT"
};

const char* SdpMediaLine::SdpDirectionTypeString[] =
{
	"NONE",
	"SENDRECV",
	"SENDONLY",
	"RECVONLY",
	"INACTIVE"
};

const char* SdpMediaLine::SdpOrientationTypeString[] =
{
	"NONE",
	"PORTRAIT",
	"LANDSCAPE",
	"SEASCAPE"
};

const char* SdpMediaLine::SdpTcpSetupAttributeString[] =
{
	"NONE",
	"ACTIVE",
	"PASSIVE",
	"ACTPASS",
	"HOLDCONN"
};

const char* SdpMediaLine::SdpTcpConnectionAttributeString[] =
{
	"NONE",
	"NEW",
	"EXISTING"
};

const char* SdpMediaLine::SdpCryptoSuiteTypeString[] =
{
	"NONE",
	"AES_CM_128_HMAC_SHA1_80",
	"AES_CM_128_HMAC_SHA1_32",
	"F8_128_HMAC_SHA1_80"
};

const char* SdpMediaLine::SdpCryptoKeyMethodString[] =
{
	"NONE",
	"INLINE"
};

const char* SdpMediaLine::SdpCryptoSrtpFecOrderTypeString[] = 
{
	"NONE",
	"FEC_SRTP",
	"SRTP_FEC"
};

const char* SdpMediaLine::SdpFingerPrintHashFuncTypeString[] =
{
	"NONE",
	"SHA-1",
	"SHA-224",
	"SHA-256",
	"SHA-384",
	"SHA-512",
	"MD5",
	"MD2"
};

const char* SdpMediaLine::SdpKeyManagementProtocolTypeString[] =
{
	"NONE",
	"MIKEY"
};

const char* SdpMediaLine::SdpPreConditionTypeString[] =
{
	"NONE",
	"QOS",
};

const char* SdpMediaLine::SdpPreConditionStrengthTypeString[] =
{
	"MANDATORY",
	"OPTIONAL",
	"NONE",
	"FAILURE",
	"UNKNOWN"
};

const char* SdpMediaLine::SdpPreConditionStatusTypeString[] =
{
	"NONE",
	"E2E",
	"LOCAL",
	"REMOTE"
};

const char* SdpMediaLine::SdpPreConditionDirectionTypeString[] =
{
	"NONE",
	"SEND",
	"RECV",
	"SENDRECV"
};

SdpMediaLine::SdpCrypto::SdpCrypto(const SdpMediaLine::SdpCrypto& rhs)
{
	operator=(rhs); 
}

SdpMediaLine::SdpCrypto&
SdpMediaLine::SdpCrypto::operator=(const SdpMediaLine::SdpCrypto& rhs)
{
	if (this == &rhs)            // handle the assignment to self case
		return *this;

	// Assign values
	_tag = rhs._tag;
	_suite = rhs._suite;
	_cryptoKeyParams = rhs._cryptoKeyParams;
	_srtpKdr = rhs._srtpKdr;
	_encryptedSrtp = rhs._encryptedSrtp;
	_encryptedSrtcp = rhs._encryptedSrtcp;
	_authenticatedSrtp = rhs._authenticatedSrtp;
	_srtpFecOrder = rhs._srtpFecOrder;
	_srtpFecKey = rhs._srtpFecKey;
	_srtpWsh = rhs._srtpWsh;
	_genericSessionParams = rhs._genericSessionParams;

	return *this;
}


// Constructor
SdpMediaLine::SdpMediaLine() :
_mediaType(SdpMediaLine::MEDIA_TYPE_NONE),
_transportProtocolType(SdpMediaLine::PROTOCOL_TYPE_NONE),
_encryptionMethod(SdpMediaLine::ENCRYPTION_METHOD_NONE),
_direction(SdpMediaLine::DIRECTION_TYPE_NONE),
_packetTime(0),
_maxPacketTime(0),
_orientation(SdpMediaLine::ORIENTATION_TYPE_NONE),
_frameRate(0),
_quality(0),
_tcpSetupAttribute(SdpMediaLine::TCP_SETUP_ATTRIBUTE_NONE),
_tcpConnectionAttribute(SdpMediaLine::TCP_CONNECTION_ATTRIBUTE_NONE),
_fingerPrintHashFunction(SdpMediaLine::FINGERPRINT_HASH_FUNC_NONE),
_keyManagementProtocol(SdpMediaLine::KEYMANAGEMENT_PROTOCOL_NONE),
_maximumPacketRate(0),
_rtpCandidatePresent(false),
_rtcpCandidatePresent(false)
{
}

// Copy constructor
SdpMediaLine::SdpMediaLine(const SdpMediaLine& rhs)
{
	operator=(rhs); 
}

// Destructor
SdpMediaLine::~SdpMediaLine()
{
	clearCandidates();
	clearCandidatePairs();
}

// Assignment operator
SdpMediaLine&
SdpMediaLine::operator=(const SdpMediaLine& rhs)
{
	if (this == &rhs)            // handle the assignment to self case
		return *this;

	// Assign values
	_mediaType = rhs._mediaType;
	_mediaTypeString = rhs._mediaTypeString;
	_transportProtocolType = rhs._transportProtocolType;
	_transportProtocolTypeString = rhs._transportProtocolTypeString;
	_codecs = rhs._codecs;
	_title = rhs._title;
	_connections = rhs._connections;
	_rtcpConnections = rhs._rtcpConnections;
	_bandwidths = rhs._bandwidths;
	_encryptionMethod = rhs._encryptionMethod;
	_encryptionKey = rhs._encryptionKey;
	_direction = rhs._direction;
	_packetTime = rhs._packetTime;
	_maxPacketTime = rhs._maxPacketTime;
	_orientation = rhs._orientation;
	_descriptionLanguage = rhs._descriptionLanguage;
	_language = rhs._language;
	_frameRate = rhs._frameRate;
	_quality = rhs._quality;
	_tcpSetupAttribute = rhs._tcpSetupAttribute;
	_tcpConnectionAttribute = rhs._tcpConnectionAttribute;
	_cryptos = rhs._cryptos;
	_fingerPrintHashFunction = rhs._fingerPrintHashFunction;
	_fingerPrint = rhs._fingerPrint;
	_keyManagementProtocol = rhs._keyManagementProtocol;
	_keyManagementData = rhs._keyManagementData;
	_preConditionCurrentStatus = rhs._preConditionCurrentStatus;
	_preConditionConfir_status = rhs._preConditionConfir_status;
	_preConditionDesiredStatus = rhs._preConditionDesiredStatus;
	_maximumPacketRate = rhs._maximumPacketRate;
	_label = rhs._label;
	_identificationTag = rhs._identificationTag;
	_iceUserFrag = rhs._iceUserFrag;
	_icePassword = rhs._icePassword;
	_remoteCandidates = rhs._remoteCandidates;
	_candidates = rhs._candidates;
	_rtpCandidatePresent = rhs._rtpCandidatePresent;
	_rtcpCandidatePresent = rhs._rtcpCandidatePresent;
	_candidatePairs = rhs._candidatePairs;

	return *this;
}


bool SdpMediaLine::parse(ByteBuffer& buf) {
	assert(buf.peekNextChar() == 'm');
	/*
	"m=video 21234 RTP/AVP 140\r\n"
	"b=RR:1\r\n"
	"b=RS:0\r\n"
	"a=crypto:1 AES_CM_128_HMAC_SHA1_80 inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2:18;inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|21|3:4 KDR=23 FEC_ORDER=SRTP_FEC UNENCRYPTED_SRTP\r\n"
	"a=crypto:2 AES_CM_128_HMAC_SHA1_32 inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20 FEC_KEY=inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20|2:4 WSH=60\r\n"
	"a=fingerprint:sha-1 0123456789\r\n"
	"a=key-mgmt:mikey thisissomebase64data\r\n"
	"a=curr:qos e2e sendrecv\r\n"
	"a=curr:qos local send\r\n"
	"a=des:qos mandatory e2e sendrecv\r\n"
	"a=des:qos optional local send\r\n"
	"a=conf:qos e2e none\r\n"
	"a=conf:qos remote recv\r\n"
	"a=remote-candidates:1 192.168.0.1 5060 2 192.168.0.1 5061\r\n"
	"a=remote-candidates:3 192.168.0.2 5063\r\n"
	"a=candidate:foundation1 1 udp 100000 127.0.0.1 21234 typ host raddr 127.0.0.8 rport 6667 name value name2 value2\r\n"
	"a=candidate:foundation2 2 udp 100001 192.168.0.1 6667 raddr 127.0.0.9 rport 6668 name value name2 value2\r\n"
	"a=candidate:foundation3 3 udp 100002 192.168.0.2 6668 raddr 127.0.0.9 name value name2 value2\r\n"
	"a=candidate:foundation3 3 udp 100002 123.123.123.124 127 name value name2 value2\r\n"
	"a=candidate:foundation3 3 udp 100002 192.168.0.2 6668 typ relay\r\n"
	"a=rtcp:127 IN IP4 123.123.123.124/60\r\n"
	"a=rtpmap:140 vp71/144000\r\n"
	"a=fmtp:140 CIF=1 QCIF=2 SQCIF\r\n"
	*/	

	/*
	*/
	// Iterate back through the buffer from the beginning to find any 
	// session-level attributes we need.
	size_t origPos = buf.position();
	buf.position(0);
	while (buf.position() < origPos) {
		char key;
		buf.readBytes(&key, 1);
		buf++;

		switch (key) {
		case 'c': 
			{		
			// "c=IN IP4 192.168.0.156/100/3\r\n"	
			std::string addrType; 
			std::string address; 

			buf.skipNextWord();
			buf.readNextWord(addrType);
			buf.readNextWord(address);

			std::cout << "### Media Stream addrType: " << addrType << std::endl;
			std::cout << "### Media Stream address: " << address << std::endl;

			addConnection(Sdp::NET_TYPE_IN, 
				Sdp::getAddressTypeFromString(addrType), 
				address, 
				0,	// Port is unknown!
				0);	// TTL is unknown!	
			}
			break;

		case 'a': 
			{	
				std::string type; 
				std::string value; 
				std::string data; 
				buf.readLine(data);
				string::size_type index = data.find(":");
				if (index != string::npos) {
					type = data.substr(0, index);
					value = data.substr(index+1, data.length());
				} else {
					type = data;
				}
				assert(type.size());
				if (type == "ice-ufrag")
				{
					setIceUserFrag(value);
				}
				if (type == "ice-pwd")
				{
					setIcePassword(value);
				}
			}
			break;
		}
		buf.skipToNextLine();
	}
	buf.position(origPos);

	// Iterate through the buffer to parse all attributes of this
	// media line.
	while (buf.remaining()) {
		char key;
		buf.readBytes(&key, 1);
		cout << "Media Parsing header: " << key << buf.peekNextChar() << endl;
		cout << "Media Remaining: " << buf.remaining() << endl;
		buf++;

		switch (key) {
		case 'm': 
			{
				std::string mediaType; 
				unsigned int port; 
				std::string transportProtocol; 

				buf.readNextWord(mediaType);				
				buf.readNextNumber(port);		
				buf.readNextWord(transportProtocol);

				setMediaType(mediaType);
				setPort(port);
				setTransportProtocolType(transportProtocol);

				assert(getMediaType(mediaType));
				assert(getTransportProtocolType(mediaType));
			}
			break;

		case 'i': 
			{
				std::string title; 
				buf.readLine(title);
				setTitle(title);
			}
			break;

			// Iterate Through Connections, c=
		case 'c':
			{	
				// "c=IN IP4 192.168.0.156/100/3\r\n"							
				std::string addrType; 
				std::string address; 

				buf.skipNextWord(); // Skipping IN
				buf.readNextWord(addrType);
				buf.readNextWord(address);

				std::cout << "### Media Stream addrType: " << addrType << std::endl;
				std::cout << "### Media Stream address: " << address << std::endl;

				addConnection(Sdp::NET_TYPE_IN, 
					Sdp::getAddressTypeFromString(addrType), 
					address, 
					0,	// Port is unknown!
					0);	// TTL is unknown!		
			}
			break;	

			// Iterrate through bandwidths, b= 
		case 'b': 
			{		
				std::string type;
				unsigned int bandwidth;

				//buf.readNextWord(type);
				//buf.readNextNumber(bandwidth);
				buf.readToChar(type, ':');
				buf++;
				buf.readNextNumber(bandwidth);

				std::cout << "### Media Stream bandwidth type: " << type << std::endl;
				std::cout << "### Media Stream bandwidth type 1: " << Sdp::SdpBandwidth::getTypeFromString(type) << std::endl;
				std::cout << "### Media Stream bandwidth type 2: " << Sdp::SdpBandwidth::getTypeFromString("RS") << std::endl;
				std::cout << "### Media Stream bandwidth type 3: " << Poco::icompare(string("RS"), type) << std::endl;
				
				std::cout << "### Media Stream bandwidth: " << bandwidth << std::endl;

				addBandwidth(Sdp::SdpBandwidth::getTypeFromString(type), bandwidth);

				
			//s << "b="
			//  << mModifier
			//  << Symbols::COLON[0] << mKbPerSecond
			//  << Symbols::CRLF;
				//cerr << "ERROR: UV Media Stream header not supported " << key << "=" << endl;
				
			// Sdp::SdpBandwidthType type = Sdp::SdpBandwidth::getTypeFromString(it->modifier().c_str());
			// if(type != Sdp::BANDWIDTH_TYPE_NONE)
			//{
			//   mediaLine->addBandwidth(type, it->kbPerSecond());
			}

			break;

		case 'k': 
			{			
				cerr << "ERROR: UV Media Stream header not supported " << key << "=" << endl;
			}
			break;

			// Look for any session level attributes
		case 'a': 
			{
				std::string type; 
				std::string value; 
				std::string data; 
				buf.readLine(data);
				string::size_type index = data.find(":");
				if (index != string::npos) {
					type = data.substr(0, index);
					value = data.substr(index+1, data.length());
				} else {
					type = data;
				}	
				assert(type.size());

				if (type == "cat")
				{
				}  
				if (type == "rtpmap")
				{
					// "a=rtpmap:140 vp71/144000\r\n"
					unsigned int payloadType;
					string codecName;	
					unsigned int codecRate;

					assert(value.size());
					ByteBuffer bb(value.data(), value.length());
					bb.readNextNumber(payloadType);				
					bb.skipWhitespace();
					bb.readToChar(codecName, '/');
					bb++;
					bb.readNextNumber(codecRate);

					std::cout << "### Attribute RTPMAP: " << payloadType << ", " << codecName << ", " << codecRate << std::endl;
					SdpCodec* codec = new SdpCodec(payloadType, "", codecName,  codecRate, 0, 0, "");
					addCodec(*codec);
					delete codec;
				}  
				if (type == "maxptime")
				{
					setMaxPacketTime(atoi(value));
				}
				if (type == "orient")
				{
					setOrientation(getOrientationTypeFromString(value));
				}
				if (type == "sdplang")
				{
					setDescriptionLanguage(value);
				} 
				if (type == "lang")
				{
					setLanguage(value);
				}
				if (type == "framerate")
				{
					setFrameRate(atoi(value));
				}
				if (type == "quality")
				{
					setQuality(atoi(value));
				}
				if (type == "setup")
				{
					setTcpSetupAttribute(getTcpSetupAttributeFromString(value));
				}
				if (type == "connection")
				{
					setTcpConnectionAttribute(getTcpConnectionAttributeFromString(value));
				}
				if (type == "crypto")
				{
				}
				if (type == "fingerprint")
				{
				}
				if (type == "key-mgmt")
				{
				}
				if (type == "curr")
				{
				}
				if (type == "conf")
				{
				}
				if (type == "des")
				{
				}
				if (type == "maxprate")
				{
					setMaximumPacketRate(atoi(value));
				}
				if (type == "label")
				{
					setLabel(value);
				}
				if (type == "mid")
				{
					setIdentificationTag(value);
				}
				if (type == "ice-ufrag")
				{
					setIceUserFrag(value);
				}
				if (type == "ice-pwd")
				{
					setIcePassword(value);
				}
				if (type == "candidate") 
				{
					// "a=candidate:foundation1 1 udp 100000 127.0.0.1 21234 typ host raddr 127.0.0.8 rport 6667 name value name2 value2\r\n"	
					string foundation;
					unsigned int componentId;
					string transport;
					unsigned int priority;
					string address;
					unsigned int port;
					// skipping <tpe>
					string candidateType;
					string relatedAddress;
					unsigned int relatedPort;

					assert(value.size());
					ByteBuffer bb(value.data(), value.length());
					bb.readNextWord(foundation);
					bb.readNextNumber(componentId);
					bb.readNextWord(transport);
					bb.readNextNumber(priority);
					bb.readNextWord(address);
					bb.readNextNumber(port);
					bb.skipNextWord(); // skipping tpe
					bb.readNextWord(candidateType);	
					if (candidateType != "host") {
						bb.skipNextWord(); // skipping raddr
						bb.readNextWord(relatedAddress);
						bb.skipNextWord(); // skipping rport
						bb.readNextNumber(relatedPort);
					}

					SdpCandidate* candidate = new SdpCandidate(foundation, componentId, 
						SdpCandidate::getCandidateTransportTypeFromString(transport), priority, address, port, 
						SdpCandidate::getCandidateTypeFromString(candidateType), relatedAddress, relatedPort);

					while (!bb.eof() && !bb.eol())
					{
						//std::cout << "### Attribute candidate: remaining: " << bb.remaining() << std::endl;
						//std::cout << "### Attribute candidate: peek r: " << (bb.peekNextChar() == '\r')  << std::endl;
						//std::cout << "### Attribute candidate: peek n: " << (bb.peekNextChar() == '\n')  << std::endl;
						std::string extAttrName;
						std::string extAttrValue;
						bb.readNextWord(extAttrName);
						bb.readNextWord(extAttrValue);
						candidate->addExtensionAttribute(extAttrName, extAttrValue);
					}

					addCandidate(*candidate);
					delete candidate;
				}
				if (type == "remote-candidates")
				{					
					// "a=remote-candidates:1 192.168.0.1 5060 2 192.168.0.1 5061\r\n"				
					unsigned int componentId;
					string address;
					unsigned int port;

					assert(value.size());
					ByteBuffer bb(value.data(), value.length());		
					while (!bb.eof() && !bb.eol())
					{
						std::cout << "### Attribute remote-candidates" << std::endl;
						bb.readNextNumber(componentId);
						bb.readNextWord(address);
						bb.readNextNumber(port);
						addRemoteCandidate(componentId, address, port);
					}
				}
				if (type == "sendrecv")
				{
					setDirection(DIRECTION_TYPE_SENDRECV);
				}
				else if (type == "sendonly")
				{
					setDirection(DIRECTION_TYPE_SENDONLY);
				}
				else if (type == "recvonly")
				{
					setDirection(DIRECTION_TYPE_RECVONLY);
				}
				else if (type == "inactive")
				{
					setDirection(DIRECTION_TYPE_INACTIVE);
				}

				std::cout << "### Attribute: " << type << ", " << value << std::endl;
			}
			break;
		default:
			cerr << "ERROR: Unknown UV header " << key << "=" << endl;
			break;
		} 

		buf.skipToNextLine();
	}

	std::cout << "SdpMediaLine::encode: END" << std::endl;
	return true;
}


bool SdpMediaLine::encode(Sourcey::Util::ByteBuffer& buf) 
{
	/*
	"o=- 2890844526 2890842807 IN IP4 10.0.1.1\r\n"
	"s=\r\n"
	"c=IN IP4 192.0.2.3\r\n"
	"t=0 0\r\n"
	"a=ice-pwd:asd88fgpdd777uzjYhagZg\r\n"
	"a=ice-ufrag:8hhY\r\n"
	"m=audio 45664 RTP/AVP 0\r\n"
	"b=RS:0\r\n"
	"b=RR:0\r\n"
	"a=rtpmap:0 PCMU/8000\r\n"
	"a=candidate:1 1 UDP 2130706431 10.0.1.1 8998 typ hs\r\n"
	"a=candidate:2 1 UDP 1694498815 192.0.2.3 45664 typ srflx raddr 10.0.1.1 rport 8998\r\n";
	*/
	assert(_connections.size());
	assert(_codecs.size());

	std::stringstream s;

	// m=
	// m=video 21234 RTP/AVP 140\r\n
	s << "m=";
	s << getMediaTypeString() << " "
	  << getPort();
		// Multicast??
	s << " "
	  << getTransportProtocolTypeString();
	SdpMediaLine::CodecList::const_iterator itCodec = _codecs.begin();
	for(;itCodec != _codecs.end(); itCodec++)
	{
		s << " " << itCodec->getPayloadType();
	}
	s << "\r\n";

	// i=title
	if (getTitle().size())
		s << "i=" << getTitle() << "\r\n";

	// b=
	if (getBandwidths().size()) {
		Sdp::BandwidthList::const_iterator itBandwidth = _bandwidths.begin();
		for(;itBandwidth != _bandwidths.end(); itBandwidth++)
		{	
			s << "b="
			  << Sdp::SdpBandwidthTypeString[itBandwidth->getType()] << " "
			  << itBandwidth->getBandwidth() 
			  << "\r\n";
		}
	}

	// c=
	// c=IN IP4 192.168.0.156/100/3
	if (getConnections().size()) {
		SdpMediaLine::ConnectionList::const_iterator itConnection = _connections.begin(); 
		for(;itConnection != _connections.end(); itConnection++)
		{
			s << "c=IN "	
			  << Sdp::SdpAddressTypeString[itConnection->getAddressType()] << " "
			  << itConnection->getAddress()
			  << "\r\n";
		}
	}

	// a=rtcp <port> [<nettype> <addrtype> <connection addr>] - RFC3605
	if (getRtcpConnections().size()) {
		SdpMediaLine::ConnectionList::const_iterator itRtcpConnection = _rtcpConnections.begin(); 
		for(;itRtcpConnection != _rtcpConnections.end(); itRtcpConnection++)
		{		
			s << "a=rtcp "		
			  << Sdp::SdpNetTypeString[itRtcpConnection->getNetType()] << " "
			  << Sdp::SdpAddressTypeString[itRtcpConnection->getAddressType()] << " "
			  << itRtcpConnection->getAddress() 
			  << "\r\n";
		}
	}   
	
	// t=
	// t=0 0\r\n
	// FIXME
	s << "t=0 0\r\n";

	// a=ice-ufrag:<ufrag> (min 4 characters) - draft-ietf-mmusic-ice-12
	if (getIceUserFrag().size())
		s << "a=ice-ufrag:" << getIceUserFrag() << "\r\n";

	// a=ice-pwd:<password> (min 22 characters) - draft-ietf-mmusic-ice-12
	if (getIcePassword().size())
		s << "a=ice-pwd:" << getIcePassword() << "\r\n";	
	
	// a=rtpmap:0 PCMU/8000
	itCodec = _codecs.begin();
	for(;itCodec != _codecs.end(); itCodec++)
	{
        s << "a=rtpmap:"
		  << itCodec->getPayloadType() << " " 
		  << itCodec->getMimeSubtype() << "/" // TODO: Should be using type or subtype?
		  << itCodec->getRate()
		  << "\r\n";

		if (itCodec->getFormatParameters().size())
        {
            s << "a=fmtp:"
              << itCodec->getPayloadType() << " " 
			  << itCodec->getFormatParameters()
			  << "\r\n";
        }		 
	}

	// a=candidate
	// a=candidate:1 1 UDP 2130706431 10.0.1.1 8998 typ host
	// a=candidate:2 1 UDP 1694498815 192.0.2.3 45664 typ srflx raddr 10.0.1.1 rport 8998
	SdpMediaLine::SdpCandidateList::const_iterator itCandidate = _candidates.begin();
	for(;itCandidate != _candidates.end(); itCandidate++)
	{
		//strm << *itCandidate;
		ost << "a=candidate:" 
			<< itCandidate->getFoundation() << " " 
			<< itCandidate->getComponentID() << " " 
			<< protoString(true) << " " 
			<< priority() << " " 
			<< transportAddr().host().toString() << " " 
			<< transportAddr().port() << " " 
			<< "typ " << typeString();
		if (typeString() != "host")
			ost << " raddr " << baseAddr().host().toString() << " " 
				<< "rport " << baseAddr().port();
		ost << "\r\n";
	}

	/*
	
   if (!mCodecs.empty())
   {
      // add codecs to information and attributes
      for (CodecContainer::const_iterator i = mCodecs.begin();
           i != mCodecs.end(); ++i)
      {
          // If codec is static (defined in RFC 3551) we probably shouldn't
          // add attributes for it. But some UAs do include them.
          //Codec::CodecMap& staticCodecs = Codec::getStaticCodecs();
          //if (staticCodecs.find(i->payloadType()) != staticCodecs.end())
          //{
          //    continue;
          //}

         s << "a=rtpmap:"
           << i->payloadType() << Symbols::SPACE[0] << *i
           << Symbols::CRLF;
         if (!i->parameters().empty())
         {
            s << "a=fmtp:"
              << i->payloadType() << Symbols::SPACE[0] << i->parameters()
              << Symbols::CRLF;
         }
      }
   }
   */


	// a=remote-candidates
	if (getRemoteCandidates().size()) 
	{
		SdpMediaLine::SdpRemoteCandidateList::const_iterator itRemoteCandidate = _remoteCandidates.begin();
		for(;itRemoteCandidate != _remoteCandidates.end(); itRemoteCandidate++)
		{
			s << "a=remote-candidates:" 
			  << itRemoteCandidate->getComponentId() << " "
			  << itRemoteCandidate->getConnectionAddress() << " "
			  << itRemoteCandidate->getPort() << " "
			  << "\r\n";
		}
	}

	buf.writeString(s.str());

	return true;
}


std::string SdpMediaLine::toString()
{
	std::stringstream strm;
	strm << "MediaLine:" << std::endl
		<< "Type: " << _mediaTypeString << std::endl
		<< "TransportProtocol: " << _transportProtocolTypeString << std::endl;

	/*
	SdpMediaLine::CodecList::const_iterator itCodec = _codecs.begin();
	for(;itCodec != _codecs.end(); itCodec++)
	{
		strm << *itCodec;
	}
	*/

	strm << "Title: '" << _title << "'" << std::endl;

	SdpMediaLine::ConnectionList::const_iterator itConnection = _connections.begin();
	for(;itConnection != _connections.end(); itConnection++)
	{
		strm << "Connection: netType=" << Sdp::SdpNetTypeString[itConnection->getNetType()]
		<< ", addrType=" << Sdp::SdpAddressTypeString[itConnection->getAddressType()]
		<< ", addr=" << itConnection->getAddress()
			<< ", port=" << itConnection->getPort()
			<< ", ttl=" << itConnection->getMulticastIpV4Ttl() << std::endl;
	}

	SdpMediaLine::ConnectionList::const_iterator itRtcpConnection = _rtcpConnections.begin();
	for(;itRtcpConnection != _rtcpConnections.end(); itRtcpConnection++)
	{
		strm << "RTCP Connection: netType=" << Sdp::SdpNetTypeString[itRtcpConnection->getNetType()]
		<< ", addrType=" << Sdp::SdpAddressTypeString[itRtcpConnection->getAddressType()]
		<< ", addr=" << itRtcpConnection->getAddress()
			<< ", port=" << itRtcpConnection->getPort()
			<< ", ttl=" << itRtcpConnection->getMulticastIpV4Ttl() << std::endl;
	}

	Sdp::BandwidthList::const_iterator itBandwidth = _bandwidths.begin();
	for(;itBandwidth != _bandwidths.end(); itBandwidth++)
	{
		strm << "Bandwidth: type=" << Sdp::SdpBandwidthTypeString[itBandwidth->getType()]
		<< ", bandwidth=" << itBandwidth->getBandwidth() << std::endl;
	}

	strm << "Encryption Key: method=" << SdpMediaLine::SdpEncryptionMethodString[_encryptionMethod] 
	<< ", data='" << _encryptionKey << "'" << std::endl
		<< "Direction: " << SdpMediaLine::SdpDirectionTypeString[_direction] << std::endl
		<< "PacketTime: " << _packetTime << std::endl
		<< "MaxPacketTime: " << _maxPacketTime << std::endl
		<< "Orientation: " << SdpMediaLine::SdpOrientationTypeString[_orientation] << std::endl
		<< "DescriptionLanguage: '" << _descriptionLanguage << "'" << std::endl
		<< "Language: '" << _language << "'" << std::endl
		<< "FrameRate: " << _frameRate << std::endl
		<< "Quality: " << _quality << std::endl
		<< "TcpSetupAttrib: " << SdpMediaLine::SdpTcpSetupAttributeString[_tcpSetupAttribute] << std::endl
		<< "TcpConnectionAttrib: " << SdpMediaLine::SdpTcpConnectionAttributeString[_tcpConnectionAttribute] << std::endl;

	SdpMediaLine::CryptoList::const_iterator itCrypto = _cryptos.begin();
	for(;itCrypto != _cryptos.end(); itCrypto++)
	{
		strm << "Crypto: tag=" << itCrypto->getTag()
			<< ", suite=" << SdpMediaLine::SdpCryptoSuiteTypeString[itCrypto->getSuite()];

		SdpMediaLine::SdpCrypto::CryptoKeyPara_list::const_iterator itKeyParam = itCrypto->getCryptoKeyParams().begin();
		for(;itKeyParam!=itCrypto->getCryptoKeyParams().end(); itKeyParam++)
		{
			strm << std::endl << "        Key Param: method=" << SdpMediaLine::SdpCryptoKeyMethodString[itKeyParam->getKeyMethod()]
			<< ", key=" << itKeyParam->getKeyValue()
				<< ", srtpLifetime=" << itKeyParam->getSrtpLifetime()
				<< ", srtpMkiValue=" << itKeyParam->getSrtpMkiValue()
				<< ", srtpMkiLength=" << itKeyParam->getSrtpMkiLength();
		}

		strm << std::endl << "        kdr=" << itCrypto->getSrtpKdr()
			<< ", encryptSrtp=" << itCrypto->getEncryptedSrtp()
			<< ", encryptSrtcp=" << itCrypto->getEncryptedSrtcp()
			<< ", authSrtp=" << itCrypto->getAuthenticatedSrtp()
			<< ", fecOrder=" << SdpMediaLine::SdpCryptoSrtpFecOrderTypeString[itCrypto->getSrtpFecOrder()]
		<< ", wsh=" << itCrypto->getSrtpWsh();
		if(itCrypto->getSrtpFecKey().getKeyMethod() != SdpMediaLine::CRYPTO_KEY_METHOD_NONE)
		{
			strm << std::endl << "        fecKeyMethod=" << SdpMediaLine::SdpCryptoKeyMethodString[itCrypto->getSrtpFecKey().getKeyMethod()]
			<< ", fecKey=" << itCrypto->getSrtpFecKey().getKeyValue()
				<< ", fecLifetime=" << itCrypto->getSrtpFecKey().getSrtpLifetime()
				<< ", fecMkiValue=" << itCrypto->getSrtpFecKey().getSrtpMkiValue()
				<< ", fecMkiLength=" << itCrypto->getSrtpFecKey().getSrtpMkiLength();
		}

		SdpMediaLine::SdpCrypto::GenericSessionPara_list::const_iterator itSessParam = itCrypto->getGenericSessionParams().begin();
		for(;itSessParam!=itCrypto->getGenericSessionParams().end(); itSessParam++)
		{
			strm << std::endl << "        sessParam=" << *itSessParam;
		}
		strm << std::endl;
	}

	strm << "FingerPrint: type=" << SdpMediaLine::SdpFingerPrintHashFuncTypeString[_fingerPrintHashFunction] 
	<< ", '" << _fingerPrint << "'" << std::endl
		<< "KeyManagement: type=" << SdpMediaLine::SdpKeyManagementProtocolTypeString[_keyManagementProtocol]
	<< ", '" << _keyManagementData << "'" << std::endl;

	SdpMediaLine::SdpPreConditionList::const_iterator itCurrentStatus = _preConditionCurrentStatus.begin();
	for(;itCurrentStatus != _preConditionCurrentStatus.end(); itCurrentStatus++)
	{
		strm << "PreConditionCurrentStatus: type=" << SdpMediaLine::SdpPreConditionTypeString[itCurrentStatus->getType()]
		<< ", status=" << SdpMediaLine::SdpPreConditionStatusTypeString[itCurrentStatus->getStatus()]
		<< ", direction=" << SdpMediaLine::SdpPreConditionDirectionTypeString[itCurrentStatus->getDirection()] << std::endl;
	}

	SdpMediaLine::SdpPreConditionList::const_iterator itConfir_status = _preConditionConfir_status.begin();
	for(;itConfir_status != _preConditionConfir_status.end(); itConfir_status++)
	{
		strm << "PreConditionConfir_status: type=" << SdpMediaLine::SdpPreConditionTypeString[itConfir_status->getType()]
		<< ", status=" << SdpMediaLine::SdpPreConditionStatusTypeString[itConfir_status->getStatus()]
		<< ", direction=" << SdpMediaLine::SdpPreConditionDirectionTypeString[itConfir_status->getDirection()] << std::endl;
	}

	SdpMediaLine::SdpPreConditionDesiredStatusList::const_iterator itDesiredStatus = _preConditionDesiredStatus.begin();
	for(;itDesiredStatus != _preConditionDesiredStatus.end(); itDesiredStatus++)
	{
		strm << "PreConditionDesiredStatus: type=" << SdpMediaLine::SdpPreConditionTypeString[itDesiredStatus->getType()]
		<< ", strength=" << SdpMediaLine::SdpPreConditionStrengthTypeString[itDesiredStatus->getStrength()]
		<< ", status=" << SdpMediaLine::SdpPreConditionStatusTypeString[itDesiredStatus->getStatus()]
		<< ", direction=" << SdpMediaLine::SdpPreConditionDirectionTypeString[itDesiredStatus->getDirection()] << std::endl;
	}

	strm << "MaximumPacketRate: " << _maximumPacketRate << std::endl
		<< "Label: '" << _label << "'" << std::endl
		<< "IdentificationTag: '" << _identificationTag << "'" << std::endl
		<< "IceUserFrag: '" << _iceUserFrag << "'" << std::endl
		<< "IcePassword: '" << _icePassword << "'" << std::endl;

	SdpMediaLine::SdpRemoteCandidateList::const_iterator itRemoteCandidate = _remoteCandidates.begin();
	for(;itRemoteCandidate != _remoteCandidates.end(); itRemoteCandidate++)
	{
		strm << "Remote Candidate: componentId=" << itRemoteCandidate->getComponentId()
			<< ", addr=" << itRemoteCandidate->getConnectionAddress() 
			<< ", port=" << itRemoteCandidate->getPort() << std::endl;
	}

	strm << "IceSupported: " << isIceSupported() << std::endl;

	/*
	SdpMediaLine::SdpCandidateList::const_iterator itCandidate = _candidates.begin();
	for(;itCandidate != _candidates.end(); itCandidate++)
	{
	strm << *itCandidate;
	}

	SdpMediaLine::SdpCandidatePairList::const_iterator itCandidatePair = _candidatePairs.begin();
	for(;itCandidatePair != _candidatePairs.end(); itCandidatePair++)
	{
	strm << *itCandidatePair;
	}

	SdpMediaLine::SdpMediaLineList::const_iterator itPotentialMediaLine = _potentialMediaViews.begin();
	for(;itPotentialMediaLine!=_potentialMediaViews.end();itPotentialMediaLine++)
	{
	strm << "PotentialMediaView:" << std::endl << *itPotentialMediaLine;
	}
	*/

	strm << "PotentialMediaViewString: '" << _potentialMediaViewString << "'" << std::endl;

	return strm.str();
}



void 
SdpMediaLine::addCandidate(SdpCandidate& candidate) 
{ 
	// Check if Candidate is in use (appears on m/c line or rtcp attributes)
	// First check m/c line(s)
	ConnectionList::iterator it = _connections.begin();
	for(;it != _connections.end(); it++)
	{
		if(candidate.getPort() == it->getPort() &&
			candidate.getConnectionAddress() == it->getAddress())  
		{
			_rtpCandidatePresent = true;
			candidate.setInUse(true);
			break;
		}
	}

	// Next check Rtcp Info
	if(isRtcpEnabled())        
	{
		it = _rtcpConnections.begin();
		for(;it != _rtcpConnections.end(); it++)
		{
			if(candidate.getPort() == it->getPort() &&
				candidate.getConnectionAddress() == it->getAddress())  
			{
				_rtcpCandidatePresent = true;
				candidate.setInUse(true);
				break;
			}
		}
	}

	_candidates.insert(candidate);
}

void 
SdpMediaLine::addCandidate(const std::string& foundation, unsigned int id, SdpCandidate::SdpCandidateTransportType transport, 
						   Poco::UInt64 priority, const std::string& connectionAddress, unsigned int port, 
						   SdpCandidate::SdpCandidateType candidateType, const std::string& relatedAddress, 
						   unsigned int relatedPort)
{ 
	SdpCandidate t(foundation, id, transport, priority, connectionAddress, port, candidateType, relatedAddress, relatedPort);
	addCandidate(t); 
}

SdpMediaLine::SdpMediaType 
SdpMediaLine::getMediaTypeFromString(const std::string& type)
{
	if(Util::icompare("audio", type))
	{
		return MEDIA_TYPE_AUDIO;
	}
	else if(Util::icompare("video", type))
	{
		return MEDIA_TYPE_VIDEO;
	}
	else if(Util::icompare("text", type))
	{
		return MEDIA_TYPE_TEXT;
	}
	else if(Util::icompare("application", type))
	{
		return MEDIA_TYPE_APPLICATION;
	}
	else if(Util::icompare("message", type))
	{
		return MEDIA_TYPE_MESSAGE;
	}
	else
	{
		return MEDIA_TYPE_UNKNOWN;
	}
}

SdpMediaLine::SdpTransportProtocolType 
SdpMediaLine::getTransportProtocolTypeFromString(const std::string& type)
{
	if(Util::icompare("udp", type))
	{
		return PROTOCOL_TYPE_UDP;
	}
	else if(Util::icompare("RTP/AVP", type))
	{
		return PROTOCOL_TYPE_RTP_AVP;
	}
	else if(Util::icompare("RTP/SAVP", type))
	{
		return PROTOCOL_TYPE_RTP_SAVP;
	}
	else if(Util::icompare("RTP/SAVPF", type))
	{
		return PROTOCOL_TYPE_RTP_SAVPF;
	}
	else if(Util::icompare("TCP", type))
	{
		return PROTOCOL_TYPE_TCP;
	}
	else if(Util::icompare("TCP/RTP/AVP", type))
	{
		return PROTOCOL_TYPE_TCP_RTP_AVP;
	}
	else if(Util::icompare("TCP/TLS", type))
	{
		return PROTOCOL_TYPE_TCP_TLS;
	}
	else if(Util::icompare("UDP/TLS", type))
	{
		return PROTOCOL_TYPE_UDP_TLS;
	}
	else if(Util::icompare("DCCP/TLS", type))
	{
		return PROTOCOL_TYPE_DCCP_TLS;
	}
	else if(Util::icompare("DCCP/TLS/RTP/SAVP", type))
	{
		return PROTOCOL_TYPE_DCCP_TLS_RTP_SAVP;
	}
	else if(Util::icompare("UDP/TLS/RTP/SAVP", type))
	{
		return PROTOCOL_TYPE_UDP_TLS_RTP_SAVP;
	}
	else if(Util::icompare("TCP/TLS/RTP/SAVP", type))
	{
		return PROTOCOL_TYPE_TCP_TLS_RTP_SAVP;
	}
	else
	{
		return PROTOCOL_TYPE_UNKNOWN;
	}
}

SdpMediaLine::SdpOrientationType 
SdpMediaLine::getOrientationTypeFromString(const std::string& type)
{
	if(Util::icompare("portrait", type))
	{
		return ORIENTATION_TYPE_PORTRAIT;
	}
	else if(Util::icompare("landscape", type))
	{
		return ORIENTATION_TYPE_LANDSCAPE;
	}
	else if(Util::icompare("seascape", type))
	{
		return ORIENTATION_TYPE_SEASCAPE;
	}
	else
	{
		return ORIENTATION_TYPE_NONE;
	}
}

SdpMediaLine::SdpTcpSetupAttribute 
SdpMediaLine::getTcpSetupAttributeFromString(const std::string& attrib)
{
	if(Util::icompare("active", attrib))
	{
		return TCP_SETUP_ATTRIBUTE_ACTIVE;
	}
	else if(Util::icompare("passive", attrib))
	{
		return TCP_SETUP_ATTRIBUTE_PASSIVE;
	}
	else if(Util::icompare("actpass", attrib))
	{
		return TCP_SETUP_ATTRIBUTE_ACTPASS;
	}
	else if(Util::icompare("holdconn", attrib))
	{
		return TCP_SETUP_ATTRIBUTE_HOLDCONN;
	}
	else
	{
		return TCP_SETUP_ATTRIBUTE_NONE;
	}
}

SdpMediaLine::SdpTcpConnectionAttribute 
SdpMediaLine::getTcpConnectionAttributeFromString(const std::string& attrib)
{
	if(Util::icompare("new", attrib))
	{
		return TCP_CONNECTION_ATTRIBUTE_NEW;
	}
	else if(Util::icompare("existing", attrib))
	{
		return TCP_CONNECTION_ATTRIBUTE_EXISTING;
	}
	else
	{
		return TCP_CONNECTION_ATTRIBUTE_NONE;
	}
}

SdpMediaLine::SdpCryptoSuiteType 
SdpMediaLine::getCryptoSuiteTypeFromString(const std::string& type)
{
	if(Util::icompare("AES_CM_128_HMAC_SHA1_80", type))
	{
		return CRYPTO_SUITE_TYPE_AES_CM_128_HMAC_SHA1_80;
	}
	else if(Util::icompare("AES_CM_128_HMAC_SHA1_32", type))
	{
		return CRYPTO_SUITE_TYPE_AES_CM_128_HMAC_SHA1_32;
	}
	else if(Util::icompare("F8_128_HMAC_SHA1_80", type))
	{
		return CRYPTO_SUITE_TYPE_F8_128_HMAC_SHA1_80;
	}
	else
	{
		return CRYPTO_SUITE_TYPE_NONE;
	}
}

SdpMediaLine::SdpCryptoKeyMethod 
SdpMediaLine::getCryptoKeyMethodFromString(const std::string& type)
{
	if(Util::icompare("inline", type))
	{
		return CRYPTO_KEY_METHOD_INLINE;
	}
	else
	{
		return CRYPTO_KEY_METHOD_NONE;
	}
}

SdpMediaLine::SdpCryptoSrtpFecOrderType 
SdpMediaLine::SdpCrypto::getSrtpFecOrderFromString(const std::string& order)
{
	if(Util::icompare("FEC_SRTP", order))
	{
		return CRYPTO_SRTP_FEC_ORDER_FEC_SRTP;
	}
	else if(Util::icompare("SRTP_FEC", order))
	{
		return CRYPTO_SRTP_FEC_ORDER_SRTP_FEC;
	}
	else
	{
		return CRYPTO_SRTP_FEC_ORDER_NONE;
	}
}

SdpMediaLine::SdpFingerPrintHashFuncType 
SdpMediaLine::getFingerPrintHashFuncTypeFromString(const std::string& type)
{
	if(Util::icompare("sha-1", type))
	{
		return FINGERPRINT_HASH_FUNC_SHA_1;
	}
	else if(Util::icompare("sha-224", type))
	{
		return FINGERPRINT_HASH_FUNC_SHA_224;
	}
	else if(Util::icompare("sha-256", type))
	{
		return FINGERPRINT_HASH_FUNC_SHA_256;
	}
	else if(Util::icompare("sha-384", type))
	{
		return FINGERPRINT_HASH_FUNC_SHA_384;
	}
	else if(Util::icompare("sha-512", type))
	{
		return FINGERPRINT_HASH_FUNC_SHA_512;
	}
	else if(Util::icompare("md5", type))
	{
		return FINGERPRINT_HASH_FUNC_MD5;
	}
	else if(Util::icompare("md2", type))
	{
		return FINGERPRINT_HASH_FUNC_MD2;
	}
	else
	{
		return FINGERPRINT_HASH_FUNC_NONE;
	}
}

SdpMediaLine::SdpKeyManagementProtocolType 
SdpMediaLine::getKeyManagementProtocolTypeFromString(const std::string& type)
{
	if(Util::icompare("mikey", type))
	{
		return KEYMANAGEMENT_PROTOCOL_MIKEY;
	}
	else
	{
		return KEYMANAGEMENT_PROTOCOL_NONE;
	}
}

SdpMediaLine::SdpPreConditionType 
SdpMediaLine::getPreConditionTypeFromString(const std::string& type)
{
	if(Util::icompare("qos", type))
	{
		return PRECONDITION_TYPE_QOS;
	}
	else
	{
		return PRECONDITION_TYPE_NONE;
	}
}

SdpMediaLine::SdpPreConditionStatusType 
SdpMediaLine::getPreConditionStatusTypeFromString(const std::string& type)
{
	if(Util::icompare("e2e", type))
	{
		return PRECONDITION_STATUS_E2E;
	}
	else if(Util::icompare("local", type))
	{
		return PRECONDITION_STATUS_LOCAL;
	}
	else if(Util::icompare("remote", type))
	{
		return PRECONDITION_STATUS_REMOTE;
	}
	else
	{
		return PRECONDITION_STATUS_NONE;
	}
}

SdpMediaLine::SdpPreConditionDirectionType 
SdpMediaLine::getPreConditionDirectionTypeFromString(const std::string& type)
{
	if(Util::icompare("send", type))
	{
		return PRECONDITION_DIRECTION_SEND;
	}
	else if(Util::icompare("recv", type))
	{
		return PRECONDITION_DIRECTION_RECV;
	}
	else if(Util::icompare("sendrecv", type))
	{
		return PRECONDITION_DIRECTION_SENDRECV;
	}
	else
	{
		return PRECONDITION_DIRECTION_NONE;
	}
}

SdpMediaLine::SdpPreConditionStrengthType 
SdpMediaLine::getPreConditionStrengthTypeFromString(const std::string& type)
{
	if(Util::icompare("mandatory", type))
	{
		return PRECONDITION_STRENGTH_MANDATORY;
	}
	else if(Util::icompare("optional", type))
	{
		return PRECONDITION_STRENGTH_OPTIONAL;
	}
	else if(Util::icompare("none", type))
	{
		return PRECONDITION_STRENGTH_NONE;
	}
	else if(Util::icompare("failure", type))
	{
		return PRECONDITION_STRENGTH_FAILURE;
	}
	else if(Util::icompare("unknown", type))
	{
		return PRECONDITION_STRENGTH_UNKNWOWN;
	}
	else
	{
		return PRECONDITION_STRENGTH_NONE;
	}
}
