#if !defined(SdpMediaLine_h)
#define SdpMediaLine_h


#include "Sourcey/UV/Sdp.h"
#include "Sourcey/UV/SdpCodec.h"
#include "Sourcey/UV/SdpCandidate.h"
#include "Sourcey/UV/SdpCandidatePair.h"
#include "Sourcey/Util/ByteBuffer.h"
#include <set>

#include "Poco/Types.h"


namespace Sourcey {
namespace UV {


class SdpMediaLine
{
public:

   typedef enum 
   {
      MEDIA_TYPE_NONE,
      MEDIA_TYPE_UNKNOWN,
      MEDIA_TYPE_AUDIO,          // "audio" - RFC4566
      MEDIA_TYPE_VIDEO,          // "video" - RFC4566
      MEDIA_TYPE_TEXT,           // "text" - RFC4566
      MEDIA_TYPE_APPLICATION,    // "application" - RFC4566
      MEDIA_TYPE_MESSAGE         // "message" - RFC4566
   } SdpMediaType;
   static const char* SdpMediaTypeString[];

   typedef enum 
   {
      PROTOCOL_TYPE_NONE,
      PROTOCOL_TYPE_UNKNOWN,
      PROTOCOL_TYPE_UDP,         // "udp" - RFC4566
      PROTOCOL_TYPE_RTP_AVP,     // "RTP/AVP" - RFC4566
      PROTOCOL_TYPE_RTP_SAVP,    // "RTP/SAVP" - RFC4566
      PROTOCOL_TYPE_RTP_SAVPF,   // "RTP/SAVPF" - RFC3711
      PROTOCOL_TYPE_TCP,         // "TCP" - RFC4145
      PROTOCOL_TYPE_TCP_RTP_AVP, // "TCP/RTP/AVP" - RFC4571
      PROTOCOL_TYPE_TCP_TLS,     // "TCP/TLS" - RFC4572
      PROTOCOL_TYPE_UDP_TLS,     // "UDP/TLS" - draft-fischl-mmusic-sdp-dtls-04
      PROTOCOL_TYPE_DCCP_TLS,    // "DCCP/TLS" - draft-fischl-mmusic-sdp-dtls-04
      PROTOCOL_TYPE_DCCP_TLS_RTP_SAVP, // "DCCP/TLS/RTP/SAVP" - draft-fischl-mmusic-sdp-dtls-04
      PROTOCOL_TYPE_UDP_TLS_RTP_SAVP,  // "UDP/TLS/RTP/SAVP" - draft-fischl-mmusic-sdp-dtls-04
      PROTOCOL_TYPE_TCP_TLS_RTP_SAVP   // "TCP/TLS/RTP/SAVP" - draft-fischl-mmusic-sdp-dtls-04
   } SdpTransportProtocolType;     
   static const char* SdpTransportProtocolTypeString[];

   class SdpConnection 
   {
   public:
      SdpConnection(Sdp::SdpNetType netType = Sdp::NET_TYPE_NONE, 
                    Sdp::SdpAddressType addressType = Sdp::ADDRESS_TYPE_NONE, 
                    const std::string& address = "", 
                    unsigned int port = 0,
                    unsigned int multicastIpV4Ttl = 0) :
         _netType(netType), _addressType(addressType), _address(address), _port(port), _multicastIpV4Ttl(multicastIpV4Ttl) {}
      SdpConnection(const SdpConnection& rhs) :
         _netType(rhs._netType), _addressType(rhs._addressType), _address(rhs._address), _port(rhs._port), _multicastIpV4Ttl(rhs._multicastIpV4Ttl) {}

      void setNetType(Sdp::SdpNetType netType) { _netType = netType; }
      Sdp::SdpNetType getNetType() const { return _netType; }

      void setAddressType(Sdp::SdpAddressType addressType) { _addressType = addressType; }
      Sdp::SdpAddressType getAddressType() const { return _addressType; }

      void setAddress(const std::string& address) { _address = address; }
      const std::string& getAddress() const { return _address; }

      void setPort(unsigned int port) { _port = port; }
      unsigned int getPort() const { return _port; }

      void setMulticastIpV4Ttl(unsigned int multicastIpV4Ttl) { _multicastIpV4Ttl = multicastIpV4Ttl; }
      unsigned int getMulticastIpV4Ttl() const { return _multicastIpV4Ttl; }

   private:
      Sdp::SdpNetType      _netType;
      Sdp::SdpAddressType  _addressType;
      std::string          _address;
      unsigned int         _port;
      unsigned int         _multicastIpV4Ttl;
   };

   typedef enum 
   {
      ENCRYPTION_METHOD_NONE,
      ENCRYPTION_METHOD_CLEAR,   // "clear" - RFC4566
      ENCRYPTION_METHOD_BASE64,  // "base64" - RFC4566
      ENCRYPTION_METHOD_URI,     // "uri" - RFC4566
      ENCRYPTION_METHOD_PROMPT   // "prompt" - RFC4566
   } SdpEncryptionMethod;
   static const char* SdpEncryptionMethodString[];

   typedef enum 
   {
      DIRECTION_TYPE_NONE,
      DIRECTION_TYPE_SENDRECV,   // "sendrecv" - RFC4566
      DIRECTION_TYPE_SENDONLY,   // "sendonly" - RFC4566
      DIRECTION_TYPE_RECVONLY,   // "recvonly" - RFC4566
      DIRECTION_TYPE_INACTIVE    // "inactive" - RFC4566
   } SdpDirectionType;
   static const char* SdpDirectionTypeString[];

   typedef enum 
   {
      ORIENTATION_TYPE_NONE,
      ORIENTATION_TYPE_PORTRAIT, // "portrait" - RFC 4566
      ORIENTATION_TYPE_LANDSCAPE,// "landscape" - RFC 4566
      ORIENTATION_TYPE_SEASCAPE  // "seascape" - RFC 4566
   } SdpOrientationType;
   static const char* SdpOrientationTypeString[];

   typedef enum 
   {
      TCP_SETUP_ATTRIBUTE_NONE,
      TCP_SETUP_ATTRIBUTE_ACTIVE,  // "active" - RFC4145
      TCP_SETUP_ATTRIBUTE_PASSIVE, // "passive" - RFC4145
      TCP_SETUP_ATTRIBUTE_ACTPASS, // "actpass" - RFC4145
      TCP_SETUP_ATTRIBUTE_HOLDCONN // "holdconn" - RFC4145
   } SdpTcpSetupAttribute;
   static const char* SdpTcpSetupAttributeString[];

   typedef enum 
   {
      TCP_CONNECTION_ATTRIBUTE_NONE,
      TCP_CONNECTION_ATTRIBUTE_NEW,      // "new" - RFC4145
      TCP_CONNECTION_ATTRIBUTE_EXISTING  // "existing" - RFC4145
   } SdpTcpConnectionAttribute;
   static const char* SdpTcpConnectionAttributeString[];

   typedef enum 
   {
      CRYPTO_SUITE_TYPE_NONE,
      CRYPTO_SUITE_TYPE_AES_CM_128_HMAC_SHA1_80,   // "AES_CM_128_HMAC_SHA1_80" - RFC4568
      CRYPTO_SUITE_TYPE_AES_CM_128_HMAC_SHA1_32,   // "AES_CM_128_HMAC_SHA1_32" - RFC4568
      CRYPTO_SUITE_TYPE_F8_128_HMAC_SHA1_80        // "F8_128_HMAC_SHA1_80" - RFC4568
   } SdpCryptoSuiteType;
   static const char* SdpCryptoSuiteTypeString[];

   typedef enum 
   {
      CRYPTO_KEY_METHOD_NONE,
      CRYPTO_KEY_METHOD_INLINE  // "inline" - RFC4568
   } SdpCryptoKeyMethod;
   static const char* SdpCryptoKeyMethodString[];

   typedef enum
   {
      CRYPTO_SRTP_FEC_ORDER_NONE,
      CRYPTO_SRTP_FEC_ORDER_FEC_SRTP,              // "FEC_SRTP" - RFC 4568
      CRYPTO_SRTP_FEC_ORDER_SRTP_FEC               // "SRTP_FEC" - RFC 2568
   } SdpCryptoSrtpFecOrderType;
   static const char* SdpCryptoSrtpFecOrderTypeString[];

   class SdpCrypto 
   {
   public:
      class SdpCryptoKeyParam 
      {
      public:
         SdpCryptoKeyParam(SdpCryptoKeyMethod keyMethod=SdpMediaLine::CRYPTO_KEY_METHOD_NONE, const std::string& keyValue="", unsigned int srtpLifetime=0, unsigned int srtpMkiValue=0, unsigned int srtpMkiLength=0) :
            _keyMethod(keyMethod), _keyValue(keyValue), _srtpLifetime(srtpLifetime), _srtpMkiValue(srtpMkiValue), _srtpMkiLength(srtpMkiLength) {}
         SdpCryptoKeyParam(const SdpCryptoKeyParam& rhs) :
            _keyMethod(rhs._keyMethod), _keyValue(rhs._keyValue), _srtpLifetime(rhs._srtpLifetime), _srtpMkiValue(rhs._srtpMkiValue), _srtpMkiLength(rhs._srtpMkiLength) {}

         void setKeyMethod(SdpCryptoKeyMethod keyMethod) { _keyMethod = keyMethod; }
         SdpCryptoKeyMethod getKeyMethod() const { return _keyMethod; }

         void setKeyValue(const std::string& keyValue) { _keyValue = keyValue; }
         const std::string& getKeyValue() const { return _keyValue; }

         void setSrtpLifetime(unsigned int srtpLifetime) { _srtpLifetime = srtpLifetime; }
         unsigned int getSrtpLifetime() const { return _srtpLifetime; }

         void setSrtpMkiValue(unsigned int srtpMkiValue) { _srtpMkiValue = srtpMkiValue; }
         unsigned int getSrtpMkiValue() const { return _srtpMkiValue; }

         void setSrtpMkiLength(unsigned int srtpMkiLength) { _srtpMkiLength = srtpMkiLength; }
         unsigned int getSrtpMkiLength() const { return _srtpMkiLength; }

      private:
         SdpCryptoKeyMethod _keyMethod;
         std::string        _keyValue;  // srtp key-salt or generic key-info
         unsigned int       _srtpLifetime;
         unsigned int       _srtpMkiValue;
         unsigned int       _srtpMkiLength;
      };

      SdpCrypto() : _tag(0), _suite(SdpMediaLine::CRYPTO_SUITE_TYPE_NONE), _srtpKdr(0), _encryptedSrtp(1), _encryptedSrtcp(1),
         _authenticatedSrtp(1), _srtpFecOrder(SdpMediaLine::CRYPTO_SRTP_FEC_ORDER_FEC_SRTP), _srtpWsh(0) {}
      SdpCrypto(const SdpCrypto& rSdpCandidatePair);
      ~SdpCrypto() { }

      typedef std::list<SdpCryptoKeyParam> CryptoKeyPara_list;
      typedef std::list<std::string> GenericSessionPara_list;

      SdpCrypto& operator=(const SdpCrypto& rhs);

      void setTag(unsigned int tag) { _tag = tag; }
      unsigned int getTag() const { return _tag; }

      void setSuite(SdpCryptoSuiteType suite) { _suite = suite; }
      SdpCryptoSuiteType getSuite() const { return _suite; }

      void addCryptoKeyParam(SdpCryptoKeyMethod keyMethod, const std::string& keyValue, unsigned int srtpLifetime=0, unsigned int srtpMkiValue=0, unsigned int srtpMkiLength=0)
      { addCryptoKeyParam(SdpCryptoKeyParam(keyMethod, keyValue, srtpLifetime, srtpMkiValue, srtpMkiLength)); }
      void addCryptoKeyParam(const SdpCryptoKeyParam& keyParam) { _cryptoKeyParams.push_back(keyParam); }
      void clearCryptoKeyParams() { _cryptoKeyParams.clear(); }
      const CryptoKeyPara_list& getCryptoKeyParams() const { return _cryptoKeyParams; }

      void setSrtpKdr(unsigned int srtpKdr) { _srtpKdr = srtpKdr; }
      unsigned int getSrtpKdr() const { return _srtpKdr; }

      void setEncryptedSrtp(bool encryptedSrtp) { _encryptedSrtp = encryptedSrtp; }
      bool getEncryptedSrtp() const { return _encryptedSrtp; }

      void setEncryptedSrtcp(bool encryptedSrtcp) { _encryptedSrtcp = encryptedSrtcp; }
      bool getEncryptedSrtcp() const { return _encryptedSrtcp; }

      void setAuthenticatedSrtp(bool authenticatedSrtp) { _authenticatedSrtp = authenticatedSrtp; }
      bool getAuthenticatedSrtp() const { return _authenticatedSrtp; }

      void setSrtpFecOrder(SdpCryptoSrtpFecOrderType srtpFecOrder) { _srtpFecOrder = srtpFecOrder; }
      SdpCryptoSrtpFecOrderType getSrtpFecOrder() const { return _srtpFecOrder; }
      static SdpCryptoSrtpFecOrderType getSrtpFecOrderFromString(const std::string& order);

      void setSrtpFecKey(SdpCryptoKeyMethod keyMethod, const std::string& keyValue, unsigned int srtpLifetime=0, unsigned int srtpMkiValue=0, unsigned int srtpMkiLength=0) 
      { _srtpFecKey.setKeyMethod(keyMethod); _srtpFecKey.setKeyValue(keyValue); _srtpFecKey.setSrtpLifetime(srtpLifetime); 
        _srtpFecKey.setSrtpMkiValue(srtpMkiValue); _srtpFecKey.setSrtpMkiLength(srtpMkiLength); }
      const SdpCryptoKeyParam& getSrtpFecKey() const { return _srtpFecKey; }

      void setSrtpWsh(unsigned int srtpWsh) { _srtpWsh = srtpWsh; }
      unsigned int getSrtpWsh() const { return _srtpWsh; }

      void addGenericSessionParam(const std::string& sessionParam) { _genericSessionParams.push_back(sessionParam); }
      void clearGenericSessionParams() { _genericSessionParams.clear(); }
      const GenericSessionPara_list& getGenericSessionParams() const { return _genericSessionParams; }

   private:
      unsigned int       _tag;         
      SdpCryptoSuiteType _suite;
      CryptoKeyPara_list _cryptoKeyParams;
      unsigned int       _srtpKdr;
      bool               _encryptedSrtp;
      bool               _encryptedSrtcp;
      bool               _authenticatedSrtp;
      SdpCryptoSrtpFecOrderType _srtpFecOrder;
      SdpCryptoKeyParam  _srtpFecKey;
      unsigned int       _srtpWsh;
      GenericSessionPara_list _genericSessionParams; 
   };

   typedef enum 
   {
      FINGERPRINT_HASH_FUNC_NONE,
      FINGERPRINT_HASH_FUNC_SHA_1,        // "sha-1" - RFC4572
      FINGERPRINT_HASH_FUNC_SHA_224,      // "sha-224" - RFC4572
      FINGERPRINT_HASH_FUNC_SHA_256,      // "sha-256" - RFC4572
      FINGERPRINT_HASH_FUNC_SHA_384,      // "sha-384" - RFC4572
      FINGERPRINT_HASH_FUNC_SHA_512,      // "sha-512" - RFC4572
      FINGERPRINT_HASH_FUNC_MD5,          // "md5" - RFC4572
      FINGERPRINT_HASH_FUNC_MD2           // "md2" - RFC4572
   } SdpFingerPrintHashFuncType;
   static const char* SdpFingerPrintHashFuncTypeString[];

   typedef enum 
   {
      KEYMANAGEMENT_PROTOCOL_NONE,
      KEYMANAGEMENT_PROTOCOL_MIKEY        // 'mikey' - RFC4567
   } SdpKeyManagementProtocolType;
   static const char* SdpKeyManagementProtocolTypeString[];

   typedef enum 
   {
      PRECONDITION_TYPE_NONE,
      PRECONDITION_TYPE_QOS               // "qos" - RFC3312
   } SdpPreConditionType;
   static const char* SdpPreConditionTypeString[];

   typedef enum 
   {
      PRECONDITION_STRENGTH_MANDATORY,    // "mandatory" - RFC3312
      PRECONDITION_STRENGTH_OPTIONAL,     // "optional" - RFC3312
      PRECONDITION_STRENGTH_NONE,         // "none" - RFC3312
      PRECONDITION_STRENGTH_FAILURE,      // "failure" - RFC3312
      PRECONDITION_STRENGTH_UNKNWOWN      // "unknown" - RFC3312
   } SdpPreConditionStrengthType;
   static const char* SdpPreConditionStrengthTypeString[];

   typedef enum 
   {
      PRECONDITION_STATUS_NONE,
      PRECONDITION_STATUS_E2E,            // "e2e" - RFC3312
      PRECONDITION_STATUS_LOCAL,          // "local" - RFC3312
      PRECONDITION_STATUS_REMOTE,         // "remote" - RFC3312
   } SdpPreConditionStatusType;
   static const char* SdpPreConditionStatusTypeString[];

   typedef enum 
   {
      PRECONDITION_DIRECTION_NONE,        // "none" - RFC3312
      PRECONDITION_DIRECTION_SEND,        // "send" - RFC3312
      PRECONDITION_DIRECTION_RECV,        // "recv" - RFC3312
      PRECONDITION_DIRECTION_SENDRECV,    // "sendrecv" - RFC3312
   } SdpPreConditionDirectionType;
   static const char* SdpPreConditionDirectionTypeString[];

   class SdpPreCondition 
   {
   public:
      SdpPreCondition(SdpPreConditionType type, SdpPreConditionStatusType status, SdpPreConditionDirectionType direction) :
             _type(type), _status(status), _direction(direction) {}
      SdpPreCondition(const SdpPreCondition& rhs) :
             _type(rhs._type), _status(rhs._status), _direction(rhs._direction) {}

      void setType(SdpPreConditionType type) { _type = type; }
      SdpPreConditionType getType() const { return _type; }

      void setStatus(SdpPreConditionStatusType status) { _status = status; }
      SdpPreConditionStatusType getStatus() const { return _status; }

      void setDirection(SdpPreConditionDirectionType direction) { _direction = direction; }
      SdpPreConditionDirectionType getDirection() const { return _direction; }

   private:
      SdpPreConditionType          _type;
      SdpPreConditionStatusType    _status;
      SdpPreConditionDirectionType _direction;
   };

   class SdpPreConditionDesiredStatus : public SdpPreCondition
   {
   public:
      SdpPreConditionDesiredStatus(SdpPreConditionType type, SdpPreConditionStrengthType strength, SdpPreConditionStatusType status, SdpPreConditionDirectionType direction) :
         SdpPreCondition(type, status, direction), _strength(strength) {}
      SdpPreConditionDesiredStatus(const SdpPreConditionDesiredStatus& rhs) :
         SdpPreCondition(rhs), _strength(rhs._strength) {}

      void setStrength(SdpPreConditionStrengthType strength) { _strength = strength; }
      SdpPreConditionStrengthType getStrength() const { return _strength; }

   private:
      SdpPreConditionStrengthType  _strength;
   };

   class SdpRemoteCandidate 
   {
   public:
      SdpRemoteCandidate(unsigned int componentId, const std::string& connectionAddress, unsigned int port) :
         _componentId(componentId), _connectionAddress(connectionAddress), _port(port) {}
      SdpRemoteCandidate(const SdpRemoteCandidate& rhs) :
         _componentId(rhs._componentId), _connectionAddress(rhs._connectionAddress), _port(rhs._port) {}

      void setComponentId(unsigned int componentId) { _componentId = componentId; }
      unsigned int getComponentId() const { return _componentId; }

      void setConnectionAddress(const std::string& connectionAddress) { _connectionAddress = connectionAddress; }
      const std::string& getConnectionAddress() const { return _connectionAddress; }

      void setPort(unsigned int port) { _port = port; }
      unsigned int getPort() const { return _port; }

   private:
      unsigned int   _componentId;
      std::string    _connectionAddress;
      unsigned int   _port;
   };

   class SdpTransportProtocolCapabilities 
   {
   public:
      SdpTransportProtocolCapabilities(unsigned int id, SdpTransportProtocolType type) :
         _id(id), _type(type) {}
      SdpTransportProtocolCapabilities(const SdpTransportProtocolCapabilities& rhs) :
         _id(rhs._id), _type(rhs._type) {}
      void setId(unsigned int id) { _id = id; }
      const unsigned int getId() const { return _id; }

      void setType(SdpTransportProtocolType type) { _type = type; }
      SdpTransportProtocolType getType() const { return _type; }

   private:
      unsigned int _id;
      SdpTransportProtocolType _type;
   };

   class SdpPotentialConfiguration 
   {
   public:
      class ConfigIdItem 
      {
      public:
         ConfigIdItem(unsigned id, bool optional=false) : _id(id), _optional(optional) {}
         ConfigIdItem(const ConfigIdItem& rhs) : _id(rhs._id), _optional(rhs._optional) {}

         void setId(unsigned int id) { _id = id; }
         const unsigned int getId() const { return _id; }

         void setOptional(bool optional) { _optional = optional; }
         const bool getOptional() const { return _optional; }

      private:
         unsigned int _id;
         bool _optional;
      };

      SdpPotentialConfiguration(unsigned int id, bool deleteMediaAttributes, bool deleteSessionAttributes, unsigned int transportId) : 
         _id(id), _deleteMediaAttributes(deleteMediaAttributes), _deleteSessionAttributes(deleteSessionAttributes), _transportId(transportId) {}
      SdpPotentialConfiguration(const SdpPotentialConfiguration& rhs) :
         _id(rhs._id), _deleteMediaAttributes(rhs._deleteMediaAttributes), _deleteSessionAttributes(rhs._deleteSessionAttributes),
         _transportId(rhs._transportId), _attributeIdList(rhs._attributeIdList)  {}

      typedef std::list<ConfigIdItem> ConfigIdList;

      void setId(unsigned int id) { _id = id; }
      const unsigned int getId() const { return _id; }

      void setDeleteMediaAttributes(bool deleteMediaAttributes) { _deleteMediaAttributes = deleteMediaAttributes; }
      const bool getDeleteMediaAttributes() const { return _deleteMediaAttributes; }

      void setDeleteSessionAttributes(bool deleteSessionAttributes) { _deleteSessionAttributes = deleteSessionAttributes; }
      const bool getDeleteSessionAttributes() const { return _deleteSessionAttributes; }

      void setTransportId(unsigned int transportId) { _transportId = transportId; }
      const unsigned int getTransportId() const { return _transportId; }

      void addAttributeId(unsigned int id, bool optional) { addAttributeId(ConfigIdItem(id, optional)); }
      void addAttributeId(const ConfigIdItem& configIdItem) { _attributeIdList.push_back(configIdItem); }
      void clearAttributeIds() { _attributeIdList.clear(); }
      const ConfigIdList& getAttributeIds() const { return _attributeIdList; }

   private:
      unsigned int _id;
      bool _deleteMediaAttributes;
      bool _deleteSessionAttributes;
      unsigned int _transportId;
      ConfigIdList _attributeIdList;
   };

   SdpMediaLine();
   SdpMediaLine(const SdpMediaLine& rSdpMediaLine);
   virtual ~SdpMediaLine();

   SdpMediaLine& operator=(const SdpMediaLine& rhs);

   bool parse(Sourcey::Util::ByteBuffer& buf);
   bool encode(Sourcey::Util::ByteBuffer& buf);
   std::string toString();

   void setMediaType(SdpMediaType mediaType) { _mediaType = mediaType; _mediaTypeString = SdpMediaLine::SdpMediaTypeString[mediaType];}
   void setMediaType(const std::string& mediaTypeString) { _mediaType = getMediaTypeFromString(mediaTypeString); _mediaTypeString = mediaTypeString;} 
   void setTransportProtocolType(SdpTransportProtocolType transportProtocolType) { _transportProtocolType = transportProtocolType; _transportProtocolTypeString = SdpMediaLine::SdpTransportProtocolTypeString[transportProtocolType]; }
   void setTransportProtocolType(const std::string& transportProtocolTypeString) { _transportProtocolType = getTransportProtocolTypeFromString(transportProtocolTypeString); _transportProtocolTypeString = transportProtocolTypeString; }
   void setPort(unsigned int port) { _port = port; }

   void addCodec(const SdpCodec& codec) { _codecs.push_back(codec); }
   void clearCodecs() { _codecs.clear(); }

   void setTitle(const std::string& title) { _title = title; }

   void addConnection(Sdp::SdpNetType netType, Sdp::SdpAddressType addressType, const std::string& address, unsigned int port, unsigned int multicastIpV4Ttl=0) 
        { addConnection(SdpConnection(netType, addressType, address, port, multicastIpV4Ttl)); }
   void addConnection(const SdpConnection& connection) { _connections.push_back(connection); }
   void clearConnections() { _connections.clear(); }

   void addRtcpConnection(Sdp::SdpNetType netType, Sdp::SdpAddressType addressType, const std::string& address, unsigned int port, unsigned int multicastIpV4Ttl=0) 
        { addRtcpConnection(SdpConnection(netType, addressType, address, port, multicastIpV4Ttl)); }
   void addRtcpConnection(const SdpConnection& connection) { _rtcpConnections.push_back(connection); }
   void clearRtcpConnections() { _rtcpConnections.clear(); }

   void addBandwidth(Sdp::SdpBandwidthType type, unsigned int bandwidth) { addBandwidth(Sdp::SdpBandwidth(type, bandwidth)); }
   void addBandwidth(const Sdp::SdpBandwidth& sdpBandwidth) { _bandwidths.push_back(sdpBandwidth); }
   void clearBandwidths() { _bandwidths.clear(); }

   void setEncryptionKey(SdpEncryptionMethod method, const std::string& key) { _encryptionMethod = method; _encryptionKey = key; }
   void setDirection(SdpDirectionType direction) { _direction = direction; }
   void setPacketTime(unsigned int packetTime) { _packetTime = packetTime; }
   void setMaxPacketTime(unsigned int maxPacketTime) { _maxPacketTime = maxPacketTime; }
   void setOrientation(SdpOrientationType orientation) { _orientation = orientation; }
   void setDescriptionLanguage(const std::string& descriptionLanguage) { _descriptionLanguage = descriptionLanguage; }
   void setLanguage(const std::string& language) { _language = language; }
   void setFrameRate(unsigned int frameRate) { _frameRate = frameRate; }
   void setQuality(unsigned int quality) { _quality = quality; }

   void setTcpSetupAttribute(SdpTcpSetupAttribute tcpSetupAttribute) { _tcpSetupAttribute = tcpSetupAttribute; }
   void setTcpConnectionAttribute(SdpTcpConnectionAttribute tcpConnectionAttribute) { _tcpConnectionAttribute = tcpConnectionAttribute; }

//   void addCryptoSettings(unsigned int tag, SdpCryptoSuiteType suite, SdpCryptoKeyMethod keyMethod, const std::string& keyValue) { addCryptoSettings(new SdpCrypto(tag, suite, keyMethod, keyValue)); }
   void addCryptoSettings(const SdpCrypto& crypto) { _cryptos.push_back(crypto); }
   void clearCryptoSettings() { _cryptos.clear(); }

   void setFingerPrint(SdpFingerPrintHashFuncType fingerPrintHashFunction, const std::string& fingerPrint) { _fingerPrintHashFunction = fingerPrintHashFunction; _fingerPrint = fingerPrint; }
   void setKeyManagementProtocol(SdpKeyManagementProtocolType protocol, const std::string& data) { _keyManagementProtocol = protocol; _keyManagementData = data; }

   void addPreConditionCurrentStatus(SdpPreConditionType type, SdpPreConditionStatusType status, SdpPreConditionDirectionType direction) 
        { addPreConditionCurrentStatus(SdpPreCondition(type, status, direction)); }
   void addPreConditionCurrentStatus(const SdpPreCondition& preCondition) { _preConditionCurrentStatus.push_back(preCondition); }
   void clearPreConditionCurrentStatus() { _preConditionCurrentStatus.clear(); }

   void addPreConditionConfir_status(SdpPreConditionType type, SdpPreConditionStatusType status, SdpPreConditionDirectionType direction) 
        { addPreConditionConfir_status(SdpPreCondition(type, status, direction)); }
   void addPreConditionConfir_status(const SdpPreCondition& preCondition) { _preConditionConfir_status.push_back(preCondition); }
   void clearPreConditionConfir_status() { _preConditionConfir_status.clear(); }

   void addPreConditionDesiredStatus(SdpPreConditionType type, SdpPreConditionStrengthType strength, SdpPreConditionStatusType status, SdpPreConditionDirectionType direction) 
        { addPreConditionDesiredStatus(SdpPreConditionDesiredStatus(type, strength, status, direction)); }
   void addPreConditionDesiredStatus(const SdpPreConditionDesiredStatus& preConditionDesiredStatus) { _preConditionDesiredStatus.push_back(preConditionDesiredStatus); }
   void clearPreConditionDesiredStatus() { _preConditionDesiredStatus.clear(); }

   void setMaximumPacketRate(double maximumPacketRate) { _maximumPacketRate = maximumPacketRate; }
   void setLabel(const std::string& label) { _label = label; }
   void setIdentificationTag(const std::string& identificationTag) { _identificationTag = identificationTag; }

   void setIceUserFrag(const std::string& iceUserFrag) { _iceUserFrag = iceUserFrag; }
   void setIcePassword(const std::string& icePassword) { _icePassword = icePassword; }

   void addRemoteCandidate(unsigned int componentId, const std::string& connectionAddress, unsigned int port) { addRemoteCandidate(SdpRemoteCandidate(componentId, connectionAddress, port)); }
   void addRemoteCandidate(const SdpRemoteCandidate& remoteCandidate) { _remoteCandidates.push_back(remoteCandidate); }
   void clearRemoteCandidates() { _remoteCandidates.clear(); }

   // Note:  Candidates should be added after m/c line and rtcp information is set, so that the in-use candidate 
   //        can be properly tagged and CandidatePresents flag can be properly set
   void addCandidate(SdpCandidate& candidate);
   void addCandidate(const std::string& foundation, unsigned int id, SdpCandidate::SdpCandidateTransportType transport, 
                     Poco::UInt64 priority, const std::string& connectionAddress, unsigned int port, 
                     SdpCandidate::SdpCandidateType candidateType, const std::string& relatedAddress = "", 
                     unsigned int relatedPort = 0);
   void clearCandidates() { _candidates.clear(); _rtpCandidatePresent = false; _rtcpCandidatePresent = false; }

   void addCandidatePair(const SdpCandidate& localCandidate, const SdpCandidate& remoteCandidate, SdpCandidatePair::SdpCandidatePairOffererType offerer)
        { addCandidatePair(SdpCandidatePair(localCandidate, remoteCandidate, offerer)); }
   void addCandidatePair(const SdpCandidatePair& sdpCandidatePair) { _candidatePairs.insert(sdpCandidatePair); }
   void clearCandidatePairs() { _candidatePairs.clear(); }

   void addPotentialMediaView(const SdpMediaLine& potentialMediaView) { _potentialMediaViews.push_back(potentialMediaView); }
   void clearPotentialMediaViews() { _potentialMediaViews.clear(); }

   void setPotentialMediaViewString(const char *potentialMediaViewString) { _potentialMediaViewString = potentialMediaViewString; }

   //void toString(std::string& sdpMediaLineString) const;

   typedef std::list<SdpCodec> CodecList;
   typedef std::list<SdpConnection> ConnectionList;
   typedef std::list<SdpCrypto> CryptoList;
   typedef std::list<SdpPreCondition> SdpPreConditionList;
   typedef std::list<SdpPreConditionDesiredStatus> SdpPreConditionDesiredStatusList;
   typedef std::list<SdpRemoteCandidate> SdpRemoteCandidateList;
   typedef std::set<SdpCandidate> SdpCandidateList;
   typedef std::set<SdpCandidatePair> SdpCandidatePairList;
   typedef std::list<SdpMediaLine> SdpMediaLineList;
         
   const SdpMediaType getMediaType() const { return _mediaType; }
   const std::string& getMediaTypeString() const { return _mediaTypeString; }
   static SdpMediaType getMediaTypeFromString(const std::string& type);
   const SdpTransportProtocolType getTransportProtocolType() const { return _transportProtocolType; }
   const std::string& getTransportProtocolTypeString() const { return _transportProtocolTypeString; }
   static SdpTransportProtocolType getTransportProtocolTypeFromString(const std::string& type);   
   const unsigned int& getPort() const { return _port; }
   const CodecList& getCodecs() const { return _codecs; }
   const std::string& getTitle() const  { return _title; }
   const ConnectionList& getConnections() const { return _connections; }
   const ConnectionList& getRtcpConnections() const { return _rtcpConnections; }
   const Sdp::BandwidthList& getBandwidths() const { return _bandwidths; }
   SdpEncryptionMethod getEncryptionMethod() const { return _encryptionMethod; }
   const std::string& getEncryptionKey() const { return _encryptionKey; }   
   SdpDirectionType getDirection() const { return _direction; }
   unsigned int getPacketTime() const { return _packetTime; }
   unsigned int getMaxPacketTime() const { return _maxPacketTime; }
   SdpOrientationType getOrientation() const { return _orientation; }
   static SdpOrientationType getOrientationTypeFromString(const std::string& type);
   const std::string& getDescriptionLanguage() const { return _descriptionLanguage; }
   const std::string& getLanguage() const { return _language; }
   unsigned int getFrameRate() const { return _frameRate; }
   unsigned int getQuality() const { return _quality; }
   SdpTcpSetupAttribute getTcpSetupAttribute() const { return _tcpSetupAttribute; }
   static SdpTcpSetupAttribute getTcpSetupAttributeFromString(const std::string& attrib);
   SdpTcpConnectionAttribute getTcpConnectionAttribute() const { return _tcpConnectionAttribute; }
   static SdpTcpConnectionAttribute getTcpConnectionAttributeFromString(const std::string& attrib);
   const CryptoList& getCryptos() const { return _cryptos; }
   static SdpCryptoSuiteType getCryptoSuiteTypeFromString(const std::string& type);
   static SdpCryptoKeyMethod getCryptoKeyMethodFromString(const std::string& type);
   SdpFingerPrintHashFuncType getFingerPrintHashFunction() const { return _fingerPrintHashFunction; }
   static SdpFingerPrintHashFuncType getFingerPrintHashFuncTypeFromString(const std::string& type);
   const std::string& getFingerPrint() const { return _fingerPrint; }
   SdpKeyManagementProtocolType getKeyManagementProtocol() const { return _keyManagementProtocol; }
   static SdpKeyManagementProtocolType getKeyManagementProtocolTypeFromString(const std::string& type);
   const std::string& getKeyManagementData() const { return _keyManagementData; }
   const SdpPreConditionList& getPreConditionCurrentStatus() const { return _preConditionCurrentStatus; }
   const SdpPreConditionList& getPreConditionConfir_status() const { return _preConditionConfir_status; }
   const SdpPreConditionDesiredStatusList& getPreConditionDesiredStatus() const { return _preConditionDesiredStatus; }
   static SdpPreConditionType getPreConditionTypeFromString(const std::string& type);
   static SdpPreConditionStatusType getPreConditionStatusTypeFromString(const std::string& type);
   static SdpPreConditionDirectionType getPreConditionDirectionTypeFromString(const std::string& type);
   static SdpPreConditionStrengthType getPreConditionStrengthTypeFromString(const std::string& type);
   double getMaximumPacketRate() const { return _maximumPacketRate; }
   const std::string& getLabel() const { return _label; }
   const std::string& getIdentificationTag() const { return _identificationTag; }
   const std::string& getIceUserFrag() const { return _iceUserFrag; }
   const std::string& getIcePassword() const { return _icePassword; }
   const SdpRemoteCandidateList& getRemoteCandidates() const { return _remoteCandidates; }
   const SdpCandidateList& getCandidates() const { return _candidates; }

   const bool isRtcpEnabled() const { return _rtcpConnections.size() > 0; }
   const bool isRtpCandidatePresent() const { return _rtpCandidatePresent; }
   const bool isRtcpCandidatePresent() const { return _rtcpCandidatePresent; }
   const bool isIceSupported() const { return  _rtpCandidatePresent && (!isRtcpEnabled() || _rtcpCandidatePresent); }

   // TODO:  In g++ std::set members are const and cannot be modified, need to update to a new STL type
   const SdpCandidatePairList& getCandidatePairs() const { return _candidatePairs; }
   SdpCandidatePairList& getCandidatePairs() { return _candidatePairs; }  // non-const version for manipulation

   const SdpMediaLineList& getPotentialMediaViews() const { return _potentialMediaViews; }
   const std::string& getPotentialMediaViewString() const { return _potentialMediaViewString; }

private:
   // m=  Note:  port is stored in each connection
   SdpMediaType   _mediaType;
   std::string    _mediaTypeString;
   unsigned int   _port;
   SdpTransportProtocolType _transportProtocolType;
   std::string    _transportProtocolTypeString;
   CodecList      _codecs;

   // i=
   std::string    _title;

   // c=
   ConnectionList _connections;           // includes port from m- line
   ConnectionList _rtcpConnections;       // a=rtcp <port> [<nettype> <addrtype> <connection addr>] - RFC3605

   // b=
   Sdp::BandwidthList _bandwidths;

   // k=
   SdpEncryptionMethod  _encryptionMethod;
   std::string    _encryptionKey;

   // a= media level attributes (including defaults copied from session level attributes)
   SdpDirectionType _direction;           // a=sendrecv, a=sendonly, a=recvonly, a=inactive - RFC4566
   unsigned int   _packetTime;            // a=ptime:<packet time> in ms - RFC4566
   unsigned int   _maxPacketTime;         // a=maxptime:<maximum packet time> in ms - RFC4566
   SdpOrientationType _orientation;       // a=orient:<orientation> - RFC4566
   std::string    _descriptionLanguage;   // a=sdplang:<language tag> - RFC4566
   std::string    _language;              // a=lang:<language tag> - RFC4566
   unsigned int   _frameRate;             // a=framerate:<frame rate> in video frames/sec - RFC4566
   unsigned int   _quality;               // a=quality:<quality> 0-10 for vidoe (0 is worst, 10 is best) - RFC4566

   SdpTcpSetupAttribute _tcpSetupAttribute;// a=setup:<setup attribute> - RFC4145
   SdpTcpConnectionAttribute _tcpConnectionAttribute; // a=connection:<conn attribute> - RFC4145

   CryptoList     _cryptos;               // a=crypto:<tag> <crypto-suite> <key method>:<keyvalud> [<session-params>] - RFC4568

   SdpFingerPrintHashFuncType _fingerPrintHashFunction; // a=fingerprint:<hash func> <fingerprint> - RFC4572
   std::string      _fingerPrint;

   SdpKeyManagementProtocolType _keyManagementProtocol; // a=key-mgmt:<protocol id> <key mgmt data> - RFC4567   
   std::string      _keyManagementData;

   SdpPreConditionList _preConditionCurrentStatus; // a=curr:<pre cond type> <status type> <direction tag> - RFC3312
   SdpPreConditionList _preConditionConfir_status; // a=conf:<pre cond type> <status type> <direction tag> - RFC3312 - are multiple allowed?
   SdpPreConditionDesiredStatusList _preConditionDesiredStatus; // a=des:<pre cond type> <strength tag> <status type> <direction tag> - RFC3312

   double         _maximumPacketRate;     // a=maxprate:<packetrate> in packets/s - RFC3890
   std::string    _label;                 // a=label:<label> - RFC4574
   std::string    _identificationTag;     // a=mid:<id tag> - RFC3388

   // Ice settings
   std::string      _iceUserFrag;         // a=ice-ufrag:<ufrag> (min 4 characters) - draft-ietf-mmusic-ice-12
   std::string      _icePassword;         // a=ice-pwd:<password> (min 22 characters) - draft-ietf-mmusic-ice-12
   SdpRemoteCandidateList _remoteCandidates;    // a=remote-candidates:<component id> <connection address> <port> ... - draft-ietf-mmusic-ice-12
   SdpCandidateList _candidates;          // a=candidate:<foundation> <component id> <transport> <qvalue> <connection address> 
                                          //             <port> [<candidate type>] [<relay addr>] [<relay port>] 
                                          //             [<ext attrib name> <ext attrib value>] - draft-ietf-mmusic-ice-12
   bool           _rtpCandidatePresent;  
   bool           _rtcpCandidatePresent;
   SdpCandidatePairList _candidatePairs;       

   // UV Capabilities Negotiation
   SdpMediaLineList _potentialMediaViews; // List of Potential Media Configurations
   std::string _potentialMediaViewString; // string that would be used in a=acfg attribute of an answer using this potential view

   //friend //EncodeStream& operator<<(EncodeStream& strm, const SdpMediaLine& );
};

//EncodeStream& operator<<(EncodeStream& strm, const SdpMediaLine& );

} } // namespace Sourcey::Util

#endif  