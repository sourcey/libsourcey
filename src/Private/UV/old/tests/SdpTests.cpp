#include <iostream>

#include "Sourcey/UV/SdpCodec.h"
#include "Sourcey/UV/Sdp.h"
#include "Sourcey/UV/SdpMediaLine.h"
#include "Sourcey/Util/Util.h"
//#include "Sourcey/UV/SdpHelperResip.h"

//#include <rutil/ParseBuffer.h>
//#include <resip/stack/Symbols.h>
//#include <resip/stack/SdpContents.h>
//#include <resip/stack/HeaderFieldValue.h>

#ifdef WIN32
#define UINT64_C(val) val##ui64
#elif __GNUC__
#define UINT64_C(val) val##ULL
#else
#error Unknown compiler for 64-bit integer constants.
#endif

//using namespace resip;
using namespace Sourcey::Util;
using namespace Sourcey::UV;
using namespace std;

//#ifdef WIN32
//int sdpTests()
//#else
int main(int argc, char* argv[])
//#endif
{
	
   std::cout << "Test get/set interfaces" << std::endl;
   {  // Test get/set interfaces
      Sdp sdp;      
      sdp.setSdpVersion(1);
      sdp.setOriginatorUserName("Test");
      sdp.setOriginatorSessionId(8888);
      sdp.setOriginatorSessionVersion(9999);
      sdp.setOriginatorNetType(Sdp::NET_TYPE_IN);
      sdp.setOriginatorAddressType(Sdp::ADDRESS_TYPE_IP4);
      sdp.setOriginatorUnicastAddress("127.0.0.1");
      sdp.setSessionName("SdpTestSession");
      sdp.setSessionInformation("sample session information");
      sdp.setSessionUri("http://www.sessionuri.com");
      assert(sdp.getSdpVersion() == 1);
      assert(sdp.getOriginatorUserName() == "Test");
      assert(sdp.getOriginatorSessionId() == 8888);
      assert(sdp.getOriginatorSessionVersion() == 9999);
      assert(sdp.getOriginatorNetType() == Sdp::NET_TYPE_IN);
      assert(sdp.getOriginatorAddressType() == Sdp::ADDRESS_TYPE_IP4);
      assert(sdp.getOriginatorUnicastAddress() == "127.0.0.1");
      assert(sdp.getSessionName() == "SdpTestSession");
      assert(sdp.getSessionInformation() == "sample session information");
      assert(sdp.getSessionUri() == "http://www.sessionuri.com");

		std::cout << "Test get/set interfaces 1" << std::endl;

      sdp.addEmailAddress("me@here.com");
      sdp.addEmailAddress("you@there.com");
      assert(sdp.getEmailAddresses().size() == 2);

      sdp.addPhoneNumber("555-555-5555");
      sdp.addPhoneNumber("123-123-1234");
      assert(sdp.getPhoneNumbers().size() == 2);

      sdp.addBandwidth(Sdp::BANDWIDTH_TYPE_CT, 1000);
      assert(sdp.getBandwidths().size() == 1);
      sdp.clearBandwidths();
      assert(sdp.getBandwidths().size() == 0);
      sdp.addBandwidth(Sdp::BANDWIDTH_TYPE_AS, 5000);
      sdp.addBandwidth(Sdp::BANDWIDTH_TYPE_CT, 100);
      assert(sdp.getBandwidths().size() == 2);
      assert(sdp.getBandwidths().front().getType() == Sdp::BANDWIDTH_TYPE_AS);
      assert(sdp.getBandwidths().front().getBandwidth() == 5000);
      assert(sdp.getBandwidths().back().getType() == Sdp::BANDWIDTH_TYPE_CT);
      assert(sdp.getBandwidths().back().getBandwidth() == 100);      

      Sdp::SdpTime* sdpTime = new Sdp::SdpTime(100, 200);
      Sdp::SdpTime::SdpTimeRepeat* repeat = new Sdp::SdpTime::SdpTimeRepeat(8, 800);
      sdpTime->addRepeat(*repeat);
      repeat = new Sdp::SdpTime::SdpTimeRepeat(9, 900);
      repeat->addOffsetFromStartTime(20);
      repeat->addOffsetFromStartTime(21);
      sdpTime->addRepeat(*repeat);
      sdp.addTime(*sdpTime);
      sdp.addTime(300, 400);
      assert(sdp.getTimes().size() == 2);      
      assert(sdp.getTimes().front().getStartTime() == 100);
      assert(sdp.getTimes().front().getStopTime() == 200);
      assert(sdp.getTimes().front().getRepeats().size() == 2);
      assert(sdp.getTimes().front().getRepeats().front().getRepeatInterval() == 8);
      assert(sdp.getTimes().front().getRepeats().front().getActiveDuration() == 800);
      assert(sdp.getTimes().front().getRepeats().back().getRepeatInterval() == 9);
      assert(sdp.getTimes().front().getRepeats().back().getActiveDuration() == 900);
      assert(sdp.getTimes().front().getRepeats().back().getOffsetsFromStartTime().size() == 2);
      assert(sdp.getTimes().front().getRepeats().back().getOffsetsFromStartTime().front() == 20);
      assert(sdp.getTimes().front().getRepeats().back().getOffsetsFromStartTime().back() == 21);
      assert(sdp.getTimes().back().getStartTime() == 300);
      assert(sdp.getTimes().back().getStopTime() == 400);
      sdp.addTimeZone(500, 600);
      sdp.addTimeZone(700, 800);
      assert(sdp.getTimeZones().size() == 2);
      assert(sdp.getTimeZones().front().getAdjustmentTime() == 500);
      assert(sdp.getTimeZones().front().getOffset() == 600);
      assert(sdp.getTimeZones().back().getAdjustmentTime() == 700);
      assert(sdp.getTimeZones().back().getOffset() == 800);

		std::cout << "Test get/set interfaces 2" << std::endl;

      sdp.setCategory("sample sdp category");
      sdp.setKeywords("sdp session description protocol");
      sdp.setToolNameAndVersion("sipX session description 1.0");
      sdp.setConferenceType(Sdp::CONFERENCE_TYPE_BROADCAST);
      sdp.setCharSet("UTF-8");
      sdp.setIcePassiveOnlyMode(true);
      assert(sdp.getCategory() == "sample sdp category");
      assert(sdp.getKeywords() == "sdp session description protocol");
      assert(sdp.getToolNameAndVersion() == "sipX session description 1.0");
      assert(sdp.getConferenceType() == Sdp::CONFERENCE_TYPE_BROADCAST);
      assert(sdp.getCharSet() == "UTF-8");
      assert(sdp.isIcePassiveOnlyMode() == true);

      Sdp::SdpGroup* group = new Sdp::SdpGroup(Sdp::GROUP_SEMANTICS_LS);
      group->addIdentificationTag("media1");
      group->addIdentificationTag("media2");
      sdp.addGroup(*group);
      group = new Sdp::SdpGroup(Sdp::GROUP_SEMANTICS_FID);
      group->addIdentificationTag("fid1");
      sdp.addGroup(*group);
      assert(sdp.getGroups().size() == 2);
      assert(sdp.getGroups().front().getSemantics() == Sdp::GROUP_SEMANTICS_LS);
      assert(sdp.getGroups().front().getIdentificationTags().size() == 2);
      assert(sdp.getGroups().front().getIdentificationTags().front() == "media1");
      assert(sdp.getGroups().front().getIdentificationTags().back() == "media2");
      assert(sdp.getGroups().back().getSemantics() == Sdp::GROUP_SEMANTICS_FID);
      assert(sdp.getGroups().back().getIdentificationTags().size() == 1);
      assert(sdp.getGroups().back().getIdentificationTags().front() == "fid1");

      sdp.setSessionLanguage("EN-US");
      sdp.setDescriptionLanguage("FR-CN");
      sdp.setMaximumPacketRate(1.5);
      assert(sdp.getSessionLanguage() == "EN-US");
      assert(sdp.getDescriptionLanguage() == "FR-CN");
      assert(sdp.getMaximumPacketRate() == 1.5);

		std::cout << "Test get/set interfaces 3" << std::endl;

      // Test Copy constructor
      Sdp sdp2(sdp);
      assert(sdp2.getSdpVersion() == 1);
      assert(sdp2.getOriginatorUserName() == "Test");
      assert(sdp2.getOriginatorSessionId() == 8888);
      assert(sdp2.getOriginatorSessionVersion() == 9999);
      assert(sdp2.getOriginatorNetType() == Sdp::NET_TYPE_IN);
      assert(sdp2.getOriginatorAddressType() == Sdp::ADDRESS_TYPE_IP4);
      assert(sdp2.getOriginatorUnicastAddress() == "127.0.0.1");
      assert(sdp2.getSessionName() == "SdpTestSession");
      assert(sdp2.getSessionInformation() == "sample session information");
      assert(sdp2.getSessionUri() == "http://www.sessionuri.com");
      assert(sdp2.getEmailAddresses().size() == 2);
      assert(sdp2.getBandwidths().size() == 2);
      assert(sdp2.getPhoneNumbers().size() == 2);
      assert(sdp2.getBandwidths().front().getType() == Sdp::BANDWIDTH_TYPE_AS);
      assert(sdp2.getBandwidths().front().getBandwidth() == 5000);
      assert(sdp2.getBandwidths().back().getType() == Sdp::BANDWIDTH_TYPE_CT);
      assert(sdp2.getBandwidths().back().getBandwidth() == 100);      
      assert(sdp2.getTimes().size() == 2);      
      assert(sdp2.getTimes().front().getStartTime() == 100);
      assert(sdp2.getTimes().front().getStopTime() == 200);
      assert(sdp2.getTimes().front().getRepeats().size() == 2);
      assert(sdp2.getTimes().front().getRepeats().front().getRepeatInterval() == 8);
      assert(sdp2.getTimes().front().getRepeats().front().getActiveDuration() == 800);
      assert(sdp2.getTimes().front().getRepeats().back().getRepeatInterval() == 9);
      assert(sdp2.getTimes().front().getRepeats().back().getActiveDuration() == 900);
      assert(sdp2.getTimes().front().getRepeats().back().getOffsetsFromStartTime().size() == 2);
      assert(sdp2.getTimes().front().getRepeats().back().getOffsetsFromStartTime().front() == 20);
      assert(sdp2.getTimes().front().getRepeats().back().getOffsetsFromStartTime().back() == 21);
      assert(sdp2.getTimes().back().getStartTime() == 300);
      assert(sdp2.getTimes().back().getStopTime() == 400);
      assert(sdp2.getTimeZones().size() == 2);
      assert(sdp2.getTimeZones().front().getAdjustmentTime() == 500);
      assert(sdp2.getTimeZones().front().getOffset() == 600);
      assert(sdp2.getTimeZones().back().getAdjustmentTime() == 700);
      assert(sdp2.getTimeZones().back().getOffset() == 800);
      assert(sdp2.getGroups().size() == 2);
      assert(sdp2.getGroups().front().getSemantics() == Sdp::GROUP_SEMANTICS_LS);
      assert(sdp2.getGroups().front().getIdentificationTags().size() == 2);
      assert(sdp2.getGroups().front().getIdentificationTags().front() == "media1");
      assert(sdp2.getGroups().front().getIdentificationTags().back() == "media2");
      assert(sdp2.getGroups().back().getSemantics() == Sdp::GROUP_SEMANTICS_FID);
      assert(sdp2.getGroups().back().getIdentificationTags().size() == 1);
      assert(sdp2.getGroups().back().getIdentificationTags().front() == "fid1");
      assert(sdp2.getSessionLanguage() == "EN-US");
      assert(sdp2.getDescriptionLanguage() == "FR-CN");
      assert(sdp2.getMaximumPacketRate() == 1.5);

		std::cout << "Test get/set interfaces 4" << std::endl;

      SdpMediaLine* mediaLine = new SdpMediaLine();
      mediaLine->setMediaType(SdpMediaLine::MEDIA_TYPE_AUDIO);
      mediaLine->setTransportProtocolType(SdpMediaLine::PROTOCOL_TYPE_RTP_AVP);
      mediaLine->setTitle("G729a Audio Codec");
      mediaLine->addConnection(Sdp::NET_TYPE_IN, Sdp::ADDRESS_TYPE_IP4, "127.0.0.1", 6000, 90);
      mediaLine->addRtcpConnection(Sdp::NET_TYPE_IN, Sdp::ADDRESS_TYPE_IP4, "127.0.0.1", 6001);
      mediaLine->addBandwidth(Sdp::BANDWIDTH_TYPE_CT, 10);
      mediaLine->setEncryptionKey(SdpMediaLine::ENCRYPTION_METHOD_BASE64, "0123456789ABCD");
      mediaLine->setDirection(SdpMediaLine::DIRECTION_TYPE_SENDRECV);
      mediaLine->setPacketTime(20);
      mediaLine->setMaxPacketTime(60);
      mediaLine->setOrientation(SdpMediaLine::ORIENTATION_TYPE_LANDSCAPE);
      mediaLine->setDescriptionLanguage("English");
      mediaLine->setLanguage("EN");
      mediaLine->setFrameRate(256);
      mediaLine->setQuality(10);
      mediaLine->setTcpConnectionAttribute(SdpMediaLine::TCP_CONNECTION_ATTRIBUTE_NEW);
      mediaLine->setTcpSetupAttribute(SdpMediaLine::TCP_SETUP_ATTRIBUTE_ACTPASS);
      assert(mediaLine->getMediaType() == SdpMediaLine::MEDIA_TYPE_AUDIO);
      assert(mediaLine->getTransportProtocolType() == SdpMediaLine::PROTOCOL_TYPE_RTP_AVP);
      assert(mediaLine->getTitle() == "G729a Audio Codec");
      assert(mediaLine->getConnections().size() == 1);
      assert(mediaLine->getConnections().front().getNetType() == Sdp::NET_TYPE_IN);
      assert(mediaLine->getConnections().front().getAddressType() == Sdp::ADDRESS_TYPE_IP4);
      assert(mediaLine->getConnections().front().getAddress() == "127.0.0.1");
      assert(mediaLine->getConnections().front().getPort() == 6000);
      assert(mediaLine->getConnections().front().getMulticastIpV4Ttl() == 90);
      assert(mediaLine->getRtcpConnections().size() == 1);
      assert(mediaLine->getRtcpConnections().front().getNetType() == Sdp::NET_TYPE_IN);
      assert(mediaLine->getRtcpConnections().front().getAddressType() == Sdp::ADDRESS_TYPE_IP4);
      assert(mediaLine->getRtcpConnections().front().getAddress() == "127.0.0.1");
      assert(mediaLine->getRtcpConnections().front().getPort() == 6001);
      assert(mediaLine->getRtcpConnections().front().getMulticastIpV4Ttl() == 0);
      assert(mediaLine->getBandwidths().size() == 1);
      assert(mediaLine->getEncryptionKey() == "0123456789ABCD");
      assert(mediaLine->getEncryptionMethod() == SdpMediaLine::ENCRYPTION_METHOD_BASE64);
      assert(mediaLine->getDirection() == SdpMediaLine::DIRECTION_TYPE_SENDRECV);
      assert(mediaLine->getPacketTime() == 20);
      assert(mediaLine->getMaxPacketTime() == 60);
      assert(mediaLine->getOrientation() == SdpMediaLine::ORIENTATION_TYPE_LANDSCAPE);
      assert(mediaLine->getDescriptionLanguage() == "English");
      assert(mediaLine->getLanguage() == "EN");
      assert(mediaLine->getFrameRate() == 256);
      assert(mediaLine->getQuality() == 10);
      assert(mediaLine->getTcpConnectionAttribute() == SdpMediaLine::TCP_CONNECTION_ATTRIBUTE_NEW);
      assert(mediaLine->getTcpSetupAttribute() == SdpMediaLine::TCP_SETUP_ATTRIBUTE_ACTPASS);

		std::cout << "Test get/set interfaces 5" << std::endl;

		/*
      SdpMediaLine::SdpCrypto *crypto = new SdpMediaLine::SdpCrypto;
      crypto->setTag(1);
      crypto->setSuite(SdpMediaLine::CRYPTO_SUITE_TYPE_AES_CM_128_HMAC_SHA1_32);
      crypto->setSrtpFecOrder(SdpMediaLine::CRYPTO_SRTP_FEC_ORDER_SRTP_FEC);
      crypto->setSrtpFecKey(SdpMediaLine::CRYPTO_KEY_METHOD_INLINE, "FECKEY", 200000, 2, 8);
      crypto->setAuthenticatedSrtp(true);
      crypto->setEncryptedSrtcp(true);
      crypto->setEncryptedSrtp(true);
      crypto->setSrtpKdr(64);
      crypto->setSrtpWsh(32);
      crypto->addCryptoKeyParam(SdpMediaLine::CRYPTO_KEY_METHOD_INLINE, "CyrptoKey", 10000, 1, 5);
      mediaLine->addCryptoSettings(*crypto);
      crypto = new SdpMediaLine::SdpCrypto;
      crypto->setTag(2);
      crypto->setSuite(SdpMediaLine::CRYPTO_SUITE_TYPE_F8_128_HMAC_SHA1_80);
      crypto->addCryptoKeyParam(SdpMediaLine::CRYPTO_KEY_METHOD_INLINE, "CryptoKey2");
      crypto->addGenericSessionParam("param1");
      crypto->addGenericSessionParam("param2");
      assert(crypto->getGenericSessionParams().size() == 2);
      assert(crypto->getTag() == 2);
      assert(crypto->getSuite() == SdpMediaLine::CRYPTO_SUITE_TYPE_F8_128_HMAC_SHA1_80);
      assert(crypto->getCryptoKeyParams().size() == 1);
      assert(crypto->getCryptoKeyParams().front().getKeyMethod() == SdpMediaLine::CRYPTO_KEY_METHOD_INLINE);
      assert(crypto->getCryptoKeyParams().front().getKeyValue() == "CryptoKey2");
      assert(crypto->getGenericSessionParams().size() == 2);
      mediaLine->addCryptoSettings(*crypto);
      assert(mediaLine->getCryptos().size() == 2);
	  */

	  /*
      mediaLine->setFingerPrint(SdpMediaLine::FINGERPRINT_HASH_FUNC_MD5, "this-is-a-finger-print");
      mediaLine->setKeyManagementProtocol(SdpMediaLine::KEYMANAGEMENT_PROTOCOL_MIKEY, "this-is-some-key-management-data");
      //mediaLine->addPreConditionCurrentStatus(SdpMediaLine::PRECONDITION_TYPE_QOS, SdpMediaLine::PRECONDITION_STATUS_E2E, SdpMediaLine::PRECONDITION_DIRECTION_SENDRECV);
      //mediaLine->addPreConditionCurrentStatus(SdpMediaLine::PRECONDITION_TYPE_QOS, SdpMediaLine::PRECONDITION_STATUS_REMOTE, SdpMediaLine::PRECONDITION_DIRECTION_RECV);
      //mediaLine->addPreConditionConfirmStatus(SdpMediaLine::PRECONDITION_TYPE_QOS, SdpMediaLine::PRECONDITION_STATUS_E2E, SdpMediaLine::PRECONDITION_DIRECTION_SENDRECV);
      //mediaLine->addPreConditionConfirmStatus(SdpMediaLine::PRECONDITION_TYPE_QOS, SdpMediaLine::PRECONDITION_STATUS_REMOTE, SdpMediaLine::PRECONDITION_DIRECTION_RECV);
      //mediaLine->addPreConditionDesiredStatus(SdpMediaLine::PRECONDITION_TYPE_QOS, SdpMediaLine::PRECONDITION_STRENGTH_MANDATORY, SdpMediaLine::PRECONDITION_STATUS_E2E, SdpMediaLine::PRECONDITION_DIRECTION_SENDRECV);
      //mediaLine->addPreConditionDesiredStatus(SdpMediaLine::PRECONDITION_TYPE_QOS, SdpMediaLine::PRECONDITION_STRENGTH_OPTIONAL, SdpMediaLine::PRECONDITION_STATUS_REMOTE, SdpMediaLine::PRECONDITION_DIRECTION_RECV);
      assert(mediaLine->getFingerPrint() == "this-is-a-finger-print");
      assert(mediaLine->getFingerPrintHashFunction() == SdpMediaLine::FINGERPRINT_HASH_FUNC_MD5);
      assert(mediaLine->getPreConditionCurrentStatus().size() == 2);
      assert(mediaLine->getPreConditionCurrentStatus().front().getType() == SdpMediaLine::PRECONDITION_TYPE_QOS);
      assert(mediaLine->getPreConditionCurrentStatus().front().getStatus() == SdpMediaLine::PRECONDITION_STATUS_E2E);
      assert(mediaLine->getPreConditionCurrentStatus().back().getDirection() == SdpMediaLine::PRECONDITION_DIRECTION_RECV);
      assert(mediaLine->getPreConditionConfirmStatus().size() == 2);
      assert(mediaLine->getPreConditionConfirmStatus().front().getType() == SdpMediaLine::PRECONDITION_TYPE_QOS);
      assert(mediaLine->getPreConditionConfirmStatus().back().getStatus() == SdpMediaLine::PRECONDITION_STATUS_REMOTE);
      assert(mediaLine->getPreConditionConfirmStatus().front().getDirection() == SdpMediaLine::PRECONDITION_DIRECTION_SENDRECV);
      assert(mediaLine->getPreConditionDesiredStatus().size() == 2);
      assert(mediaLine->getPreConditionDesiredStatus().front().getStrength() == SdpMediaLine::PRECONDITION_STRENGTH_MANDATORY);
      assert(mediaLine->getPreConditionDesiredStatus().back().getStatus() == SdpMediaLine::PRECONDITION_STATUS_REMOTE);
      assert(mediaLine->getPreConditionDesiredStatus().front().getDirection() == SdpMediaLine::PRECONDITION_DIRECTION_SENDRECV);

      mediaLine->setMaximumPacketRate(20);
      mediaLine->setLabel("G711 Label");
      mediaLine->setIdentificationTag("item1");
      mediaLine->setIceUserFrag("ICEUSER");
      mediaLine->setIcePassword("ICEPASSWORD");
      mediaLine->addRemoteCandidate(1, "127.0.0.1", 5060);
      mediaLine->addRemoteCandidate(2, "127.0.0.2", 5061);
      assert(mediaLine->getMaximumPacketRate() == 20);
      assert(mediaLine->getLabel() == "G711 Label");
      assert(mediaLine->getIdentificationTag() == "item1");
      assert(mediaLine->getIceUserFrag() == "ICEUSER");
      assert(mediaLine->getIcePassword() == "ICEPASSWORD");
      assert(mediaLine->getRemoteCandidates().size() == 2);
      assert(mediaLine->getRemoteCandidates().front().getComponentId() == 1);
      assert(mediaLine->getRemoteCandidates().front().getConnectionAddress() == "127.0.0.1");
      assert(mediaLine->getRemoteCandidates().front().getPort() == 5060);

      mediaLine->addCandidate("id1", 1, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 100, "127.0.0.1", 6000, SdpCandidate::CANDIDATE_TYPE_HOST);
      mediaLine->addCandidate("id1", 2, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 50, "127.0.0.1", 6000, SdpCandidate::CANDIDATE_TYPE_SRFLX);
      mediaLine->addCandidate("id2", 1, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 101, "192.168.1.2", 5060, SdpCandidate::CANDIDATE_TYPE_RELAY, "127.0.0.3", 5080);
      mediaLine->addCandidate("id2", 1, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 100, "127.0.0.1", 6001, SdpCandidate::CANDIDATE_TYPE_HOST);
      assert(mediaLine->getCandidates().size() == 4);
      // Note:  this list is ordered
      SdpMediaLine::SdpCandidateList::const_iterator it = mediaLine->getCandidates().begin();
      assert(it->getPriority() == 101);
      assert(it->getFoundation() == "id2");
      assert(it->getId() == 1);
      assert(it->getTransport() == SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP);
      assert(it->getConnectionAddress() == "192.168.1.2");
      assert(it->getPort() == 5060);
      assert(it->getCandidateType() == SdpCandidate::CANDIDATE_TYPE_RELAY);
      assert(it->getRelatedAddress() == "127.0.0.3");
      assert(it->getRelatedPort() == 5080);
      assert(it->getExtensionAttributes().size() == 0);   
      assert(it->isInUse() == false);
      it++;
      assert(it->getPriority() == 100);
      assert(it->getFoundation() == "id1");
      assert(it->getRelatedAddress() == "");
      assert(it->getRelatedPort() == 0);
      assert(it->isInUse() == true);
      it++;
      assert(it->getPriority() == 100);
      assert(it->getFoundation() == "id2");
      assert(it->isInUse() == true);
      it++;
      assert(it->getPriority() == 50);
      assert(it->getFoundation() == "id1");
      assert(it->isInUse() == true);

      assert(mediaLine->isIceSupported() == true);

      sdp.addMediaLine(mediaLine);
      assert(sdp.getMediaLines().size() == 1);

		std::cout << "Test get/set interfaces 5" << std::endl;
		*/

		//delete mediaLine;
      mediaLine = new SdpMediaLine();
		std::cout << "Test get/set interfaces 5.1" << std::endl;
      SdpCodec* codec = new SdpCodec(19, "audio", "G729", 8000, 20, 1, "annexb=no");
      assert(codec->getPayloadType() == 19);
      assert(codec->getMimeType() == "audio");
      assert(codec->getMimeSubtype() == "G729");
      assert(codec->getRate() == 8000);
      assert(codec->getPacketTime() == 20);
      assert(codec->getNumChannels() == 1);
      assert(codec->getFormatParameters() == "annexb=no");
		std::cout << "Test get/set interfaces 5.2" << std::endl;
      mediaLine->addCodec(*codec);
      assert(mediaLine->getCodecs().size() == 1);

		std::cout << "Test get/set interfaces 6" << std::endl;
      SdpCandidate* sdpLocalCandidate = new SdpCandidate("f1", 1, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 100, "192.168.1.1", 6000, SdpCandidate::CANDIDATE_TYPE_HOST);

	  SdpCandidate* sdpRemoteCandidate = new SdpCandidate("a1", 1, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 55, "192.168.1.1", 2345, SdpCandidate::CANDIDATE_TYPE_HOST);

	  mediaLine->addCandidate(*sdpLocalCandidate);
      assert(mediaLine->getCandidates().size() == 1);
      mediaLine->addCandidatePair(*sdpLocalCandidate, *sdpRemoteCandidate, SdpCandidatePair::OFFERER_REMOTE);
      assert(mediaLine->getCandidatePairs().size() == 1);
      delete sdpRemoteCandidate;
      sdpLocalCandidate = new SdpCandidate("f1", 2, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 101, "192.168.1.1", 6001, SdpCandidate::CANDIDATE_TYPE_HOST);
      sdpRemoteCandidate = new SdpCandidate("a1", 2, SdpCandidate::CANDIDATE_TRANSPORT_TYPE_UDP, 56, "192.168.1.1", 2346, SdpCandidate::CANDIDATE_TYPE_HOST);
      mediaLine->addCandidate(*sdpLocalCandidate);
      assert(mediaLine->getCandidates().size() == 2);
      mediaLine->addCandidatePair(*sdpLocalCandidate, *sdpRemoteCandidate, SdpCandidatePair::OFFERER_LOCAL);
      assert(mediaLine->getCandidatePairs().size() == 2);
      SdpMediaLine::SdpCandidatePairList& candidatePairs = mediaLine->getCandidatePairs();
      SdpMediaLine::SdpCandidatePairList::iterator it2 = candidatePairs.begin();
      assert(it2->getPriority() == UINT64_C(240518168779));
      assert(it2->getLocalCandidate().getPort() == 6001);
      assert(it2->getRemoteCandidate().getPort() == 2346);
      assert(it2->getOfferer() == SdpCandidatePair::OFFERER_LOCAL);
      assert(it2->getCheckState() == SdpCandidatePair::CHECK_STATE_FROZEN);
      // Note: TODO - In g++ std:set members are inmutable, and cannot be modified - will need to change to another type 
      //it2->setCheckState(SdpCandidatePair::CHECK_STATE_WAITING);  
      //assert(it2->getCheckState() == SdpCandidatePair::CHECK_STATE_WAITING);
      it2++;
      assert(it2->getPriority() == UINT64_C(236223201480));

		std::cout << "Test get/set interfaces 7" << std::endl;

      delete sdpRemoteCandidate;
      /*
	  */

      sdp.addMediaLine(mediaLine);
      assert(sdp.getMediaLines().size() == 2);

      // Ensure string builder does not crash
      //std::cout << sdp << std::endl;

		std::cout << "Test get/set interfaces 8" << std::endl;

      // Test FoundationId interface
      assert(sdp.getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.1") == "1");
      assert(sdp.getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.1") == "1");
      assert(sdp.getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.2") == "2");
      assert(sdp.getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.2", "192.168.1.1") == "3");
      assert(sdp.getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.2") == "2");
      assert(sdp.getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.2", "192.168.1.1") == "3");
   }
   std::cout << "Test get/set interfaces: END" << std::endl;

   {
      // Test helpers
		 /*
	   std::string sdpContent = "v=1\r\n"  
         "o=- 333525334858460 333525334858460 IN IP4 192.168.0.156\r\n"
         "s=test123\r\n"
         "e=unknown@invalid.net\r\n"
         "p=+972 683 1000\r\n"
         "c=IN IP4 127.0.0.1\r\n"
         "b=RR:0\r\n"
         "b=RS:0\r\n"
         "b=CT:10000\r\n"
         "t=4058038202 0\r\n"
         "k=base64:base64key\r\n"
         "a=tool:ResipParserTester\r\n"
         "a=inactive\r\n"
         "m=audio 41466/6 RTP/AVP 0 101\r\n"
         "i=Audio Stream\r\n"
         "c=IN IP4 192.168.0.156/100/3\r\n"
         "c=IN IP6 FF15::101/3\r\n"
         "k=clear:base64clearkey\r\n"
         "a=fmtp:101 0-11\r\n"
         "a=ptime:20\r\n"
         "a=fmtp:0 annexb=no\r\n"
         "a=maxptime:40\r\n"
         "a=setup:active\r\n"
         "a=sendrecv\r\n"
         "a=rtpmap:101 telephone-event/8000\r\n"
         "a=crypto:1 F8_128_HMAC_SHA1_80 inline:MTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5QUJjZGVm|2^20|1:4;inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20|2:4 FEC_ORDER=FEC_SRTP\r\n"

		 "m=video 21234 RTP/AVP 140\r\n"
         "b=RR:1\r\n"
         "b=RS:0\r\n"
         //"a=crypto:1 AES_CM_128_HMAC_SHA1_80 inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2:18;inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|21|3:4 KDR=23 FEC_ORDER=SRTP_FEC UNENCRYPTED_SRTP\r\n"
         //"a=crypto:2 AES_CM_128_HMAC_SHA1_32 inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20 FEC_KEY=inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20|2:4 WSH=60\r\n"
         //"a=fingerprint:sha-1 0123456789\r\n"
         //"a=key-mgmt:mikey thisissomebase64data\r\n"
         //"a=curr:qos e2e sendrecv\r\n"
         //"a=curr:qos local send\r\n"
         //"a=des:qos mandatory e2e sendrecv\r\n"
         //"a=des:qos optional local send\r\n"
         //"a=conf:qos e2e none\r\n"
         //"a=conf:qos remote recv\r\n"
         "a=remote-candidates:1 192.168.0.1 5060 2 192.168.0.1 5061\r\n"
         "a=remote-candidates:3 192.168.0.2 5063\r\n"
         "a=candidate:foundation1 1 udp 100000 127.0.0.1 21234 typ host raddr 127.0.0.8 rport 6667 name value name2 value2\r\n"
         //"a=candidate:foundation2 2 udp 100001 192.168.0.1 6667 raddr 127.0.0.9 rport 6668 name value name2 value2\r\n"
         //"a=candidate:foundation3 3 udp 100002 192.168.0.2 6668 raddr 127.0.0.9 name value name2 value2\r\n"
         //"a=candidate:foundation3 3 udp 100002 123.123.123.124 127 name value name2 value2\r\n"
         //"a=candidate:foundation3 3 udp 100002 192.168.0.2 6668 typ relay\r\n"
         "a=rtcp:127 IN IP4 123.123.123.124/60\r\n"
         "a=rtpmap:140 vp71/144000\r\n"
         "a=fmtp:140 CIF=1 QCIF=2 SQCIF\r\n";    */
		std::string sdpContent = "v=0\r\n"
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
		"a=candidate:1 1 UDP 2130706431 10.0.1.1 8998 typ host\r\n"
		"a=candidate:2 1 UDP 1694498815 192.0.2.3 45664 typ srflx raddr 10.0.1.1 rport 8998\r\n";

	  	Sdp sdp1;

		ByteBuffer buf(sdpContent.data(), sdpContent.length());
		sdp1.parse(buf);

		ByteBuffer buf1;
		sdp1.encode(buf1);
		std::string sdpResult(buf1.data(), buf1.length());

        std::cout << "\n\nOriginal:\n\n" << sdpContent << endl;
        std::cout << "\n\nNew:\n\n" << sdpResult << endl;


		 /*
      Sdp* convSdp;

      { // Test resip helper
         HeaderFieldValue hfv(txt, sizeof(txt));
         Mime type("application", "sdp");
         SdpContents resipSdp(&hfv, type);

         convSdp = SdpHelperResip::createSdpFromResipSdp(resipSdp);
         if(convSdp)
         {
            // Ensure string builder does not crash            
            std::cout << "\n\nResip Sdp Helper Test:" << endl << *convSdp << endl;

            // Perform some random assertions
            assert(convSdp->getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.1") == "1");
            assert(convSdp->getOriginatorUnicastAddress() == "192.168.0.156");
            assert(convSdp->getSessionName() == "test123");
            assert(convSdp->getEmailAddresses().size() == 1);
            assert(convSdp->getPhoneNumbers().size() == 1);
            assert(convSdp->getBandwidths().size() == 3);
            assert(convSdp->getTimes().size() == 1);
            assert(convSdp->getToolNameAndVersion() == "ResipParserTester");
            assert(convSdp->getMediaLines().size() == 2);
            assert(convSdp->getMediaLines().front()->getMediaType() == SdpMediaLine::MEDIA_TYPE_AUDIO);
            assert(convSdp->getMediaLines().back()->getMediaType() == SdpMediaLine::MEDIA_TYPE_VIDEO);
            assert(convSdp->getMediaLines().front()->getTransportProtocolType() == SdpMediaLine::PROTOCOL_TYPE_RTP_AVP);
            assert(convSdp->getMediaLines().front()->getCodecs().size() == 2);
            assert(convSdp->getMediaLines().back()->getCodecs().size() == 1);
            assert(convSdp->getMediaLines().front()->getCodecs().front().getPayloadType() == 0);
            assert(convSdp->getMediaLines().front()->getCodecs().back().getPayloadType() == 101);
            assert(convSdp->getMediaLines().back()->getCodecs().front().getPayloadType() == 140);
            assert(convSdp->getMediaLines().front()->getCodecs().back().getFormatParameters() == "0-11");
            assert(convSdp->getMediaLines().front()->getConnections().size() == 6);
            assert(convSdp->getMediaLines().back()->getConnections().size() == 1);
            assert(convSdp->getMediaLines().front()->getRtcpConnections().size() == 0);
            assert(convSdp->getMediaLines().back()->getRtcpConnections().size() == 1);
            assert(convSdp->getMediaLines().front()->getConnections().front().getAddress() == "192.168.0.156");
            assert(convSdp->getMediaLines().front()->getConnections().front().getPort() == 41466);
            assert(convSdp->getMediaLines().back()->getRtcpConnections().front().getPort() == 127);
            assert(convSdp->getMediaLines().front()->getBandwidths().size() == 0);
            assert(convSdp->getMediaLines().back()->getBandwidths().size() == 2);
            assert(convSdp->getMediaLines().front()->getEncryptionKey() == "base64clearkey");
            assert(convSdp->getMediaLines().front()->getDirection() == SdpMediaLine::DIRECTION_TYPE_SENDRECV);
            assert(convSdp->getMediaLines().back()->getDirection() == SdpMediaLine::DIRECTION_TYPE_INACTIVE);
            assert(convSdp->getMediaLines().front()->getPacketTime() == 20);
            assert(convSdp->getMediaLines().front()->getTcpConnectionAttribute() == SdpMediaLine::TCP_CONNECTION_ATTRIBUTE_NONE);
            assert(convSdp->getMediaLines().front()->getTcpSetupAttribute() == SdpMediaLine::TCP_SETUP_ATTRIBUTE_ACTIVE);
            assert(convSdp->getMediaLines().front()->getCryptos().size() == 1);
            assert(convSdp->getMediaLines().back()->getCryptos().size() == 2);
            assert(convSdp->getMediaLines().front()->getCandidates().size() == 0);
            assert(convSdp->getMediaLines().back()->getCandidates().size() == 5);
            assert(convSdp->getMediaLines().back()->getRemoteCandidates().size() == 3);
            assert(convSdp->getMediaLines().back()->getPreConditionCurrentStatus().size() == 2);
            assert(convSdp->getMediaLines().back()->getPreConditionConfirmStatus().size() == 2);
            assert(convSdp->getMediaLines().back()->getPreConditionDesiredStatus().size() == 2);

            // Copy test
            Sdp copySdp(*convSdp);

            // Ensure string builder does not crash
            std::cout << copySdp << endl;

            // Perform some random assertions on copy
            assert(copySdp.getLocalFoundationId(SdpCandidate::CANDIDATE_TYPE_HOST, "127.0.0.2") == "2");
            assert(copySdp.getOriginatorUnicastAddress() == "192.168.0.156");
            assert(copySdp.getSessionName() == "test123");
            assert(copySdp.getEmailAddresses().size() == 1);
            assert(copySdp.getPhoneNumbers().size() == 1);
            assert(copySdp.getBandwidths().size() == 3);
            assert(copySdp.getTimes().size() == 1);
            assert(copySdp.getToolNameAndVersion() == "ResipParserTester");
            assert(copySdp.getMediaLines().size() == 2);
            assert(copySdp.getMediaLines().front()->getMediaType() == SdpMediaLine::MEDIA_TYPE_AUDIO);
            assert(copySdp.getMediaLines().back()->getMediaType() == SdpMediaLine::MEDIA_TYPE_VIDEO);
            assert(copySdp.getMediaLines().front()->getTransportProtocolType() == SdpMediaLine::PROTOCOL_TYPE_RTP_AVP);
            assert(copySdp.getMediaLines().front()->getCodecs().size() == 2);
            assert(copySdp.getMediaLines().back()->getCodecs().size() == 1);
            assert(copySdp.getMediaLines().front()->getCodecs().front().getPayloadType() == 0);
            assert(copySdp.getMediaLines().front()->getCodecs().back().getPayloadType() == 101);
            assert(copySdp.getMediaLines().back()->getCodecs().front().getPayloadType() == 140);
            assert(copySdp.getMediaLines().front()->getCodecs().back().getFormatParameters() == "0-11");
            assert(copySdp.getMediaLines().front()->getConnections().size() == 6);
            assert(copySdp.getMediaLines().back()->getConnections().size() == 1);
            assert(copySdp.getMediaLines().front()->getRtcpConnections().size() == 0);
            assert(copySdp.getMediaLines().back()->getRtcpConnections().size() == 1);
            assert(copySdp.getMediaLines().front()->getConnections().front().getAddress() == "192.168.0.156");
            assert(copySdp.getMediaLines().front()->getConnections().front().getPort() == 41466);
            assert(copySdp.getMediaLines().back()->getRtcpConnections().front().getPort() == 127);
            assert(copySdp.getMediaLines().front()->getBandwidths().size() == 0);
            assert(copySdp.getMediaLines().back()->getBandwidths().size() == 2);
            assert(copySdp.getMediaLines().front()->getEncryptionKey() == "base64clearkey");
            assert(copySdp.getMediaLines().front()->getDirection() == SdpMediaLine::DIRECTION_TYPE_SENDRECV);
            assert(copySdp.getMediaLines().back()->getDirection() == SdpMediaLine::DIRECTION_TYPE_INACTIVE);
            assert(copySdp.getMediaLines().front()->getPacketTime() == 20);
            assert(copySdp.getMediaLines().front()->getTcpConnectionAttribute() == SdpMediaLine::TCP_CONNECTION_ATTRIBUTE_NONE);
            assert(copySdp.getMediaLines().front()->getTcpSetupAttribute() == SdpMediaLine::TCP_SETUP_ATTRIBUTE_ACTIVE);
            assert(copySdp.getMediaLines().front()->getCryptos().size() == 1);
            assert(copySdp.getMediaLines().back()->getCryptos().size() == 2);
            assert(copySdp.getMediaLines().front()->getCandidates().size() == 0);
            assert(copySdp.getMediaLines().back()->getCandidates().size() == 5);
            assert(copySdp.getMediaLines().back()->getRemoteCandidates().size() == 3);
            assert(copySdp.getMediaLines().back()->getPreConditionCurrentStatus().size() == 2);
            assert(copySdp.getMediaLines().back()->getPreConditionConfirmStatus().size() == 2);
            assert(copySdp.getMediaLines().back()->getPreConditionDesiredStatus().size() == 2);
            delete convSdp;
         }
      }
   */
   }

   system("pause");
	return 0;
}