#include "Sourcey/UV/Sdp.h"
#include "Sourcey/UV/SdpMediaLine.h"

//#include <rutil/Data.h>

using namespace std;
using namespace Sourcey;
using namespace Sourcey::Util;
using namespace Sourcey::UV;

const char* Sdp::SdpNetTypeString[] = 
{
	"NONE",
	"IN"
};

const char* Sdp::SdpAddressTypeString[] =
{
	"NONE",
	"IP4",
	"IP6"
};

const char* Sdp::SdpBandwidthTypeString[] =
{
	"NONE",
	"CT",
	"AS",
	"TIAS",
	"RS",
	"RR"
};

const char* Sdp::SdpConferenceTypeString[] = 
{
	"NONE",
	"BROADCAST",
	"MODERATED",
	"TEST",
	"H332"
};

const char* Sdp::SdpGroupSemanticsString[] =
{
	"NONE",
	"LS",
	"FID",
	"SRF",
	"ANAT"
};


// Constructor
Sdp::Sdp()
{
	_sdpVersion = 1;
	_originatorSessionId = 0;
	_originatorSessionVersion = 0;
	_originatorNetType = NET_TYPE_NONE;
	_originatorAddressType = ADDRESS_TYPE_NONE;
	_conferenceType = CONFERENCE_TYPE_NONE;
	_icePassiveOnlyMode = false;
	_maximumPacketRate = 0;
}


/*
Sdp::Sdp(const std::string& src) 
{
vector<string> lines = Util::tokenize(src, "\r\n", true); 
for (int i = 0; i < lines.size(); i++) {
switch (lines[i][0]) {
case 'v':
break;
case 'o':
break;
case 's':
break;
case 'i':
case 'c':
case 't':
break;
case 'm':
break;
case 'a':
break;
default:
cerr << "ERROR: Unknown UV header: "<< lines[i] << endl;
break;
}
}
}
*/


// Copy constructor
Sdp::Sdp(const Sdp& rhs)
{
	operator=(rhs); 
}

// Destructor
Sdp::~Sdp()
{
	clearMediaLines();
}

// Assignment operator
Sdp& 
Sdp::operator=(const Sdp& rhs)
{
	if (this == &rhs)            // handle the assignment to self case
		return *this;

	// Assign values
	_sdpVersion = rhs._sdpVersion;
	_originatorUserName = rhs._originatorUserName;
	_originatorSessionId = rhs._originatorSessionId;
	_originatorSessionVersion = rhs._originatorSessionVersion;
	_originatorNetType = rhs._originatorNetType;
	_originatorAddressType = rhs._originatorAddressType;
	_originatorUnicastAddress = rhs._originatorUnicastAddress;
	_sessionName = rhs._sessionName;
	_sessionInformation = rhs._sessionInformation;
	_sessionUri = rhs._sessionUri;
	_emailAddresses = rhs._emailAddresses;
	_phoneNumbers = rhs._phoneNumbers;
	_bandwidths = rhs._bandwidths;
	_times = rhs._times;
	_timeZones = rhs._timeZones;
	_category = rhs._category;
	_keywords = rhs._keywords;
	_toolNameAndVersion = rhs._toolNameAndVersion;
	_conferenceType = rhs._conferenceType;
	_charSet = rhs._charSet;
	_icePassiveOnlyMode = rhs._icePassiveOnlyMode;
	_groups = rhs._groups;
	_sessionLanguage = rhs._sessionLanguage;
	_descriptionLanguage = rhs._descriptionLanguage;
	_maximumPacketRate = rhs._maximumPacketRate;
	_foundationIds = rhs._foundationIds;

	// Copy over media lines - deep copy of pointers
	clearMediaLines();
	MediaLineList::const_iterator it = rhs._mediaLines.begin();
	for(;it != rhs._mediaLines.end(); it++)
	{
		SdpMediaLine* mediaLineCopy = new SdpMediaLine(*(*it));
		addMediaLine(mediaLineCopy);
	}

	return *this;
}


bool Sdp::parse(ByteBuffer& buf) {

	// Session level attributes to pass to the media stream.
	//SdpConnection* cLine = NULL;
	//SdpMediaLine* mLine = NULL;

	while (buf.remaining()) {

		//cout << "Parsing Init" << endl;

		char key;
		buf.readBytes(&key, 1);

		cout << "Parsing header: " << key << buf.peekNextChar() << endl;
		cout << "Remaining: " << buf.remaining() << endl;
		buf++;

		switch (key) { 
		case 'v': 
			{
				std::string version; 
				buf.readNextWord(version);
				std::cout << "### Version: " << version << "~" <<std::endl;
				setSdpVersion(atoi(version)); 
				//setSdpVersion(version); 
			}
			break;
		case 'o': {
				// "o=- 333525334858460 333525334858460 IN IP4 192.168.0.156\r\n"
				std::string user; 
				std::string sessionId; 
				std::string version; 
				std::string addrType;
				std::string address; 

				buf.readNextWord(user);
				buf.readNextWord(sessionId);
				buf.readNextWord(version);
				buf.skipNextWord(); 
				buf.readNextWord(addrType);
				buf.readNextWord(address);
				
				std::cout << "### User: " << user << "~" << std::endl;
				std::cout << "### sessionId: " << sessionId << "~" << std::endl;
				std::cout << "### version: " << version << "~" << std::endl;
				std::cout << "### addrType: " << addrType << std::endl;
				std::cout << "### address: " << address << std::endl;

				setOriginatorInfo(user, atoi(sessionId), atoi(version), Sdp::NET_TYPE_IN, getAddressTypeFromString(addrType), address);
			}
			break;
		case 's': {         
				// "s=test123\r\n"
				std::string sessionName; 
				buf.readNextWord(sessionName);
				std::cout << "### sessionName: " << sessionName << std::endl;
				setSessionName(sessionName);
			}
			break;
		case 'i': {
				std::string sessionInfo; 
				buf.readNextWord(sessionInfo);
				std::cout << "### sessionInfo: " << sessionInfo << std::endl;
				setSessionInformation(sessionInfo);
			}
			break;
   
		// Iterate Through Connections, c=
		case 'c': {					
				//c= is only stored in SdpMediaLine
			}
			break;	
			
		// Populate email addresses, e=
		case 'e': {
				cerr << "ERROR: UV header not supported " << key << "=" << endl;
			}
			break;
			
		// Populate phone numbers, p=
		case 'p': {
				cerr << "ERROR: UV header not supported " << key << "=" << endl;
			}
			break;
			
		// Populate bandwidths, b=
		case 'b': {
				cerr << "ERROR: UV header not supported " << key << "=" << endl;
			}
			break;

		// Populate Times, t=, r=
		case 't':
		case 'r': {
				cerr << "ERROR: UV header not supported " << key << "=" << endl;
			}
			break;
			
		// Populate Timezones, z=
		case 'z': {
				cerr << "ERROR: UV header not supported " << key << "=" << endl;
			}
			break;
			
		// Look for any session level attributes
		case 'a': {
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
					setCategory(value);
				}
				if (type == "keywds")
				{
					setKeywords(value);
				}
				if (type == "tool")
				{
					setToolNameAndVersion(value);
				}
				if (type == "type")
				{
					setConferenceType(Sdp::getConferenceTypeFromString(value));
				}
				if (type == "charset")
				{
					setCharSet(value);
				}
				if (type == "ice-passive")
				{
					setIcePassiveOnlyMode(true);
				}
				if (type == "group")
				{
					cerr << "ERROR: UV attribute parsing not supported " << key << "=group" << endl;
				}
				if (type == "lang")
				{
					setSessionLanguage(value);
				}
				if (type == "sdplang")
				{
					setDescriptionLanguage(value);
				}
				if (type == "maxprate")
				{
					setMaximumPacketRate(atoi(value));
				}

				std::cout << "### Attribute: " << type << ", " << value << std::endl;

			}
			break;
		case 'm': {
				SdpMediaLine* mLine = new SdpMediaLine();
				buf--;
				buf--;
				mLine->parse(buf);
				addMediaLine(mLine);
			}
			break;
		/*
			*/
		default:
			cerr << "ERROR: Unknown UV header " << key << "=" << endl;
			break;
		}	

		buf.skipToNextLine();
	}

	std::cout << "SdpMediaLine::encode: END" << std::endl;
	return true;
}


bool Sdp::encode(Sourcey::Util::ByteBuffer& buf) 
{
	std::ostringstream ost;

	ost << "v="
		<< _sdpVersion
		<< "\r\n";

	ost << "o="
		<< _originatorUserName << " "
		<< _originatorSessionId << " "
		<< _originatorSessionVersion << " "
		<< "IN "
		<< Sdp::SdpNetTypeString[_originatorNetType] << " "
		<< _originatorUnicastAddress 
		<< "\r\n";

	// TODO: Email addresses
	// TODO: Phone numbers
	// TODO: Bandwidth
	// TODO: Time
	// TODO: TimeZone
	// TODO: Category
	// TODO: Keywords
	// TODO: ToolNameAndVersion
	// TODO: ConferenceType
	// TODO: CharSet
	// TODO: IcePassiveOnlyMode
	// TODO: Group
	// TODO: SessionLanguage
	// TODO: DescriptionLanguage
	// TODO: MaximumPacketRate

	buf.writeString(ost.str());

	Sdp::MediaLineList::const_iterator itMediaLine = _mediaLines.begin();
	for(;itMediaLine!=_mediaLines.end();itMediaLine++)
	{
		(*itMediaLine)->encode(buf);
	}

	return true;
}


std::string Sdp::toString()
{
	std::stringstream strm;
	strm << "Sdp:" << std::endl
		<< "SdpVersion: " << _sdpVersion << std::endl
		<< "OrigUserName: '" << _originatorUserName << "'" << std::endl
		<< "OrigSessionId: " << _originatorSessionId << std::endl
		<< "OrigSessionVersion: " << _originatorSessionVersion << std::endl
		<< "OrigNetType: " << Sdp::SdpNetTypeString[_originatorNetType] << std::endl
		<< "OrigAddressType: " << Sdp::SdpAddressTypeString[_originatorAddressType] << std::endl
		<< "OrigUnicastAddr: '" << _originatorUnicastAddress << "'" << std::endl
		<< "SessionName: '" << _sessionName << "'" << std::endl
		<< "SessionInformation: '" << _sessionInformation << "'" << std::endl
		<< "SessionUri: '" << _sessionUri << "'" << std::endl;

	Sdp::EmailAddressList::const_iterator itEmail = _emailAddresses.begin();
	for(;itEmail != _emailAddresses.end(); itEmail++)
	{
		strm << "EmailAddress: '" << *itEmail << "'" << std::endl;
	}

	Sdp::PhoneNumberList::const_iterator itPhone = _phoneNumbers.begin();
	for(;itPhone != _phoneNumbers.end(); itPhone++)
	{
		strm << "PhoneNumber: '" << *itPhone << "'" << std::endl;
	}

	Sdp::BandwidthList::const_iterator itBandwidth = _bandwidths.begin();
	for(;itBandwidth != _bandwidths.end(); itBandwidth++)
	{
		strm << "Bandwidth: type=" << Sdp::SdpBandwidthTypeString[itBandwidth->getType()]
		<< ", bandwidth=" << itBandwidth->getBandwidth() << std::endl;
	}

	Sdp::TimeList::const_iterator itTime = _times.begin();
	for(;itTime != _times.end(); itTime++)
	{
		strm << "Time: start=" << itTime->getStartTime()
			<< ", stop=" << itTime->getStopTime() << std::endl;

		Sdp::SdpTime::RepeatsList::const_iterator itRepeat = itTime->getRepeats().begin();
		for(;itRepeat!=itTime->getRepeats().end(); itRepeat++)
		{
			strm << "TimeRepeat: interval=" << itRepeat->getRepeatInterval()
				<< ", duration=" << itRepeat->getActiveDuration();

			Sdp::SdpTime::SdpTimeRepeat::OffsetsList::const_iterator itOffset = itRepeat->getOffsetsFromStartTime().begin();
			for(;itOffset!=itRepeat->getOffsetsFromStartTime().end(); itOffset++)
			{
				strm << ", offset=" << *itOffset;
			}
			strm << std::endl;
		}
	}

	Sdp::TimeZoneList::const_iterator itTimeZone = _timeZones.begin();
	for(;itTimeZone != _timeZones.end(); itTimeZone++)
	{
		strm << "TimeZone: adjustment time=" << itTimeZone->getAdjustmentTime()
			<< ", offset=" << itTimeZone->getOffset() << std::endl;
	}

	strm << "Category: '" << _category << "'" << std::endl
		<< "Keywords: '" << _keywords << "'" << std::endl
		<< "ToolNameAndVersion: '" << _toolNameAndVersion << "'" << std::endl
		<< "ConferenceType: " << Sdp::SdpConferenceTypeString[_conferenceType] << std::endl
		<< "CharSet: '" << _charSet << "'" << std::endl
		<< "IcePassiveOnlyMode: " << _icePassiveOnlyMode << std::endl;

	Sdp::GroupList::const_iterator itGroup = _groups.begin();
	for(;itGroup != _groups.end(); itGroup++)
	{
		strm << "Group: semantics=" << Sdp::SdpGroupSemanticsString[itGroup->getSemantics()];
		Sdp::SdpGroup::TagsList::const_iterator itTag = itGroup->getIdentificationTags().begin();
		for(;itTag!=itGroup->getIdentificationTags().end(); itTag++)
		{
			strm << ", idTag=" << *itTag;
		}
		strm << std::endl;
	}

	strm << "SessionLanguage: '" << _sessionLanguage << "'" << std::endl
		<< "DescriptionLanguage: '" << _descriptionLanguage << "'" << std::endl
		<< "MaximumPacketRate: " << _maximumPacketRate << std::endl;

	Sdp::MediaLineList::const_iterator itMediaLine = _mediaLines.begin();
	for(;itMediaLine!=_mediaLines.end();itMediaLine++)
	{
		strm << std::endl << (*itMediaLine)->toString();
	}

	return strm.str();
}


Sdp::SdpAddressType 
Sdp::getAddressTypeFromString(const std::string& type)
{
	////std::string dataType(type);

	if (Util::icompare("IP4", type))
	{
		return ADDRESS_TYPE_IP4;
	}
	else if (Util::icompare("IP6", type))
	{
		return ADDRESS_TYPE_IP6;
	}
	else
	{
		return ADDRESS_TYPE_NONE;
	}
}

Sdp::SdpBandwidthType 
Sdp::SdpBandwidth::getTypeFromString(const std::string& type)
{
	////std::string dataType(type);

	if (Util::icompare("CT", type))
	{
		return BANDWIDTH_TYPE_CT;
	}
	else if (Util::icompare("AS", type))
	{
		return BANDWIDTH_TYPE_AS;
	}
	else if (Util::icompare("TIAS", type))
	{
		return BANDWIDTH_TYPE_TIAS;
	}
	else if (Util::icompare("RS", type))
	{
		return BANDWIDTH_TYPE_RS;
	}
	else if (Util::icompare("RR", type))
	{
		return BANDWIDTH_TYPE_RR;
	}
	else
	{
		return BANDWIDTH_TYPE_NONE;
	}
}

Sdp::SdpConferenceType 
Sdp::getConferenceTypeFromString(const std::string& type)
{
	////std::string dataType(type);

	if (Util::icompare("broadcast", type))
	{
		return CONFERENCE_TYPE_BROADCAST;
	}
	else if (Util::icompare("moderated", type))
	{
		return CONFERENCE_TYPE_MODERATED;
	}
	else if (Util::icompare("test", type))
	{
		return CONFERENCE_TYPE_TEST;
	}
	else if (Util::icompare("H332", type))
	{
		return CONFERENCE_TYPE_H332;
	}
	else
	{
		return CONFERENCE_TYPE_NONE;
	}
}

Sdp::SdpGroupSemantics 
Sdp::SdpGroup::getSemanticsFromString(const std::string& type)
{
	////std::string dataType(type);

	if (Util::icompare("LS", type))
	{
		return GROUP_SEMANTICS_LS;
	}
	else if (Util::icompare("FID", type))
	{
		return GROUP_SEMANTICS_FID;
	}
	else if (Util::icompare("SRF", type))
	{
		return GROUP_SEMANTICS_SRF;
	}
	else if (Util::icompare("ANAT", type))
	{
		return GROUP_SEMANTICS_ANAT;
	}
	else
	{
		return GROUP_SEMANTICS_NONE;
	}
}

void 
Sdp::setOriginatorInfo(const std::string& userName, 
					   Poco::UInt64 sessionId, 
					   Poco::UInt64 sessionVersion, 
					   SdpNetType netType, 
					   SdpAddressType addressType, 
					   const std::string& unicastAddress)
{
	_originatorUserName = userName;
	_originatorSessionId = sessionId;
	_originatorSessionVersion = sessionVersion;
	_originatorNetType = netType;
	_originatorAddressType = addressType;
	_originatorUnicastAddress = unicastAddress;
}

void 
Sdp::addMediaLine(SdpMediaLine* mediaLine) 
{ 
	_mediaLines.push_back(mediaLine); 
}

void 
Sdp::clearMediaLines() 
{ 
	MediaLineList::iterator it = _mediaLines.begin();
	for(;it != _mediaLines.end(); it++)
	{
		delete *it;
	}
	_mediaLines.clear(); 
}

const Sdp::MediaLineList& 
Sdp::getMediaLines() const 
{ 
	return _mediaLines; 
}

std::string 
Sdp::getLocalFoundationId(SdpCandidate::SdpCandidateType candidateType, 
						  const std::string& baseAddress, 
						  const std::string& stunAddress)
{
	SdpFoundation sdpFoundation(candidateType, baseAddress, stunAddress);

	std::map<std::string, SdpFoundation>::iterator it;
	for(it = _foundationIds.begin(); it != _foundationIds.end(); it++)
	{
		if (it->second == sdpFoundation)
		{
			return it->first;
		}
	}

	// Not found - insert
	char foundationId[15];
	sprintf(foundationId, "%d", _foundationIds.size() + 1);
	_foundationIds[foundationId] = sdpFoundation;

	return foundationId;
}

/*
EncodeStream& 
sdpcontainer::operator<<( EncodeStream& strm, const Sdp& sdp)
{
strm << "Sdp:" << std::endl
<< "SdpVersion: " << sdp._sdpVersion << std::endl
<< "OrigUserName: '" << sdp._originatorUserName << "'" << std::endl
<< "OrigSessionId: " << sdp._originatorSessionId << std::endl
<< "OrigSessionVersion: " << sdp._originatorSessionVersion << std::endl
<< "OrigNetType: " << Sdp::SdpNetTypeString[sdp._originatorNetType] << std::endl
<< "OrigAddressType: " << Sdp::SdpAddressTypeString[sdp._originatorAddressType] << std::endl
<< "OrigUnicastAddr: '" << sdp._originatorUnicastAddress << "'" << std::endl
<< "SessionName: '" << sdp._sessionName << "'" << std::endl
<< "SessionInformation: '" << sdp._sessionInformation << "'" << std::endl
<< "SessionUri: '" << sdp._sessionUri << "'" << std::endl;

Sdp::EmailAddressList::const_iterator itEmail = sdp._emailAddresses.begin();
for(;itEmail != sdp._emailAddresses.end(); itEmail++)
{
strm << "EmailAddress: '" << *itEmail << "'" << std::endl;
}

Sdp::PhoneNumberList::const_iterator itPhone = sdp._phoneNumbers.begin();
for(;itPhone != sdp._phoneNumbers.end(); itPhone++)
{
strm << "PhoneNumber: '" << *itPhone << "'" << std::endl;
}

Sdp::BandwidthList::const_iterator itBandwidth = sdp._bandwidths.begin();
for(;itBandwidth != sdp._bandwidths.end(); itBandwidth++)
{
strm << "Bandwidth: type=" << Sdp::SdpBandwidthTypeString[itBandwidth->getType()]
<< ", bandwidth=" << itBandwidth->getBandwidth() << std::endl;
}

Sdp::TimeList::const_iterator itTime = sdp._times.begin();
for(;itTime != sdp._times.end(); itTime++)
{
strm << "Time: start=" << itTime->getStartTime()
<< ", stop=" << itTime->getStopTime() << std::endl;

Sdp::SdpTime::RepeatsList::const_iterator itRepeat = itTime->getRepeats().begin();
for(;itRepeat!=itTime->getRepeats().end(); itRepeat++)
{
strm << "TimeRepeat: interval=" << itRepeat->getRepeatInterval()
<< ", duration=" << itRepeat->getActiveDuration();

Sdp::SdpTime::SdpTimeRepeat::OffsetsList::const_iterator itOffset = itRepeat->getOffsetsFromStartTime().begin();
for(;itOffset!=itRepeat->getOffsetsFromStartTime().end(); itOffset++)
{
strm << ", offset=" << *itOffset;
}
strm << std::endl;
}
}

Sdp::TimeZoneList::const_iterator itTimeZone = sdp._timeZones.begin();
for(;itTimeZone != sdp._timeZones.end(); itTimeZone++)
{
strm << "TimeZone: adjustment time=" << itTimeZone->getAdjustmentTime()
<< ", offset=" << itTimeZone->getOffset() << std::endl;
}

strm << "Category: '" << sdp._category << "'" << std::endl
<< "Keywords: '" << sdp._keywords << "'" << std::endl
<< "ToolNameAndVersion: '" << sdp._toolNameAndVersion << "'" << std::endl
<< "ConferenceType: " << Sdp::SdpConferenceTypeString[sdp._conferenceType] << std::endl
<< "CharSet: '" << sdp._charSet << "'" << std::endl
<< "IcePassiveOnlyMode: " << sdp._icePassiveOnlyMode << std::endl;

Sdp::GroupList::const_iterator itGroup = sdp._groups.begin();
for(;itGroup != sdp._groups.end(); itGroup++)
{
strm << "Group: semantics=" << Sdp::SdpGroupSemanticsString[itGroup->getSemantics()];
Sdp::SdpGroup::TagsList::const_iterator itTag = itGroup->getIdentificationTags().begin();
for(;itTag!=itGroup->getIdentificationTags().end(); itTag++)
{
strm << ", idTag=" << *itTag;
}
strm << std::endl;
}

strm << "SessionLanguage: '" << sdp._sessionLanguage << "'" << std::endl
<< "DescriptionLanguage: '" << sdp._descriptionLanguage << "'" << std::endl
<< "MaximumPacketRate: " << sdp._maximumPacketRate << std::endl;

Sdp::MediaLineList::const_iterator itMediaLine = sdp._mediaLines.begin();
for(;itMediaLine!=sdp._mediaLines.end();itMediaLine++)
{
strm << std::endl << *(*itMediaLine);
}

return strm;
}

*/