#if !defined(Sdp_h)
#define Sdp_h


#include <iostream>
#include <list>
#include <map>

#include "Sourcey/Util/Util.h"
#include "Sourcey/Util/ByteBuffer.h"
#include "Poco/Types.h"

#include "SdpCandidate.h"


namespace Sourcey {
namespace UV {

class SdpConnection;
class SdpMediaLine;

// Container for UV specification
// This class holds the information related to an UV.
class Sdp
{
public:

   typedef enum 
   {
      NET_TYPE_NONE,
      NET_TYPE_IN             // "IN" - Internet - RFC4566
   } SdpNetType;
   static const char* SdpNetTypeString[];

   typedef enum 
   {
      ADDRESS_TYPE_NONE,
      ADDRESS_TYPE_IP4,       // "IP4" - RFC4566
      ADDRESS_TYPE_IP6        // "IP6" - RFC4566
   } SdpAddressType;
   static const char* SdpAddressTypeString[];

   typedef enum 
   {
      BANDWIDTH_TYPE_NONE,
      BANDWIDTH_TYPE_CT,      // "CT" - Conference Total - RFC4566
      BANDWIDTH_TYPE_AS,      // "AS" - Application Specific - RFC4566
      BANDWIDTH_TYPE_TIAS,    // "TIAS" - Transport Independent Application Specific - RFC3890,
      BANDWIDTH_TYPE_RS,      // "RS" - RTCP bandwidth on active senders - RFC3556
      BANDWIDTH_TYPE_RR       // "RR" - RTCP bandwidth allocated to other participants - RFC3556
   } SdpBandwidthType;
   static const char* SdpBandwidthTypeString[];

   class SdpBandwidth
   {
   public:
      SdpBandwidth(SdpBandwidthType type, unsigned int bandwidth) : _type(type), _bandwidth(bandwidth) {}
      SdpBandwidth(const SdpBandwidth& rhs) : _type(rhs._type), _bandwidth(rhs._bandwidth) {}

      // Accessors
      void setType(SdpBandwidthType type) { _type = type; }
      SdpBandwidthType getType() const { return _type; }
      static SdpBandwidthType getTypeFromString(const std::string& type);

      void setBandwidth(unsigned int bandwidth) { _bandwidth = bandwidth; }
      unsigned int getBandwidth() const { return _bandwidth; }

   private:
      SdpBandwidthType  _type;
      unsigned int      _bandwidth;
   };

   class SdpTime 
   {
   public:
      class SdpTimeRepeat 
      {
      public:
         SdpTimeRepeat(unsigned int repeatInterval, unsigned int activeDuration) :
            _repeatInterval(repeatInterval), _activeDuration(activeDuration) {}
          SdpTimeRepeat(const SdpTimeRepeat& rhs) :
            _repeatInterval(rhs._repeatInterval), _activeDuration(rhs._activeDuration), _offsetsFromStartTime(rhs._offsetsFromStartTime) {}

         typedef std::list<unsigned int> OffsetsList;

         void setRepeatInterval(unsigned int repeatInterval) { _repeatInterval = repeatInterval; }
         unsigned int getRepeatInterval() const { return _repeatInterval; }

         void setActiveDuration(unsigned int activeDuration) { _activeDuration = activeDuration; }
         unsigned int getActiveDuration() const { return _activeDuration; }

         void addOffsetFromStartTime(unsigned int offset) { _offsetsFromStartTime.push_back(offset); }
         void clearOffsetsFromStartTime() { _offsetsFromStartTime.clear(); }
         const OffsetsList& getOffsetsFromStartTime() const { return _offsetsFromStartTime; }

      private:
         unsigned int _repeatInterval;
         unsigned int _activeDuration;
         OffsetsList  _offsetsFromStartTime;
      };

      SdpTime(Poco::UInt64 startTime, Poco::UInt64 stopTime) : _startTime(startTime), _stopTime(stopTime) {}
      SdpTime(const SdpTime& rhs) : _startTime(rhs._startTime), _stopTime(rhs._stopTime), _repeats(rhs._repeats) {}

      typedef std::list<SdpTimeRepeat> RepeatsList;

      void setStartTime(Poco::UInt64 startTime) { _startTime = startTime; }
      Poco::UInt64 getStartTime() const { return _startTime; }

      void setStopTime(Poco::UInt64 stopTime) { _stopTime = stopTime; }
      Poco::UInt64 getStopTime() const { return _stopTime; }

      void addRepeat(const SdpTimeRepeat& sdpTimeRepeat) { _repeats.push_back(sdpTimeRepeat); }
      void clearRepeats() { _repeats.clear(); }
      const RepeatsList& getRepeats() const { return _repeats; }

   private:
      Poco::UInt64    _startTime;
      Poco::UInt64    _stopTime;
      RepeatsList _repeats;       
   };

   class SdpTimeZone 
   {
   public:
      SdpTimeZone(int adjustmentTime, int offset) : _adjustmentTime(adjustmentTime), _offset(offset) {}
      SdpTimeZone(const SdpTimeZone& rhs) : _adjustmentTime(rhs._adjustmentTime), _offset(rhs._offset) {}

      void setAdjustmentTime(int adjustmentTime) { _adjustmentTime = adjustmentTime; }
      int getAdjustmentTime() const { return _adjustmentTime; }

      void setOffset(int offset) { _offset = offset; }
      int getOffset() const { return _offset; }

   private:
      int         _adjustmentTime;
      int         _offset;
   };

   typedef enum 
   {
      CONFERENCE_TYPE_NONE,
      CONFERENCE_TYPE_BROADCAST, // "broadcast" - RFC4566
      CONFERENCE_TYPE_MODERATED, // "moderated" - RFC4566
      CONFERENCE_TYPE_TEST,      // "test" - RFC4566
      CONFERENCE_TYPE_H332       // "H332" - RFC4566
   } SdpConferenceType;
   static const char* SdpConferenceTypeString[];

   typedef enum 
   {
      GROUP_SEMANTICS_NONE,
      GROUP_SEMANTICS_LS,        // "LS" - Lip Sync - RFC3388
      GROUP_SEMANTICS_FID,       // "FID" - Flow Identifier - RFC3388
      GROUP_SEMANTICS_SRF,       // "SRF" - Single Reservation Flow - RFC3524
      GROUP_SEMANTICS_ANAT       // "ANAT" - Alternative Network Address Types - RFC4091
   } SdpGroupSemantics;
   static const char* SdpGroupSemanticsString[];

   class SdpGroup
   {
   public:
      SdpGroup(SdpGroupSemantics semantics) : _semantics(semantics) {}
      SdpGroup(const SdpGroup& rhs) : _semantics(rhs._semantics), _identificationTags(rhs._identificationTags) {}

      typedef std::list<std::string> TagsList;

      void setSemantics(SdpGroupSemantics semantics) { _semantics = semantics; }
      SdpGroupSemantics getSemantics() const { return _semantics; }
      static SdpGroupSemantics getSemanticsFromString(const std::string& type);

      void addIdentificationTag(const std::string& identificationTag) { _identificationTags.push_back(identificationTag); }
      void clearIdentificationTags() { _identificationTags.clear(); }
      const TagsList& getIdentificationTags() const { return _identificationTags; }
      
   private:
      SdpGroupSemantics _semantics;
      TagsList          _identificationTags;
   };

   class SdpFoundation
   {
   public:
      SdpFoundation() : _candidateType(SdpCandidate::CANDIDATE_TYPE_NONE) {}
      SdpFoundation(SdpCandidate::SdpCandidateType candidateType, const std::string& baseAddress, const std::string& stunAddress) : 
         _candidateType(candidateType), _baseAddress(baseAddress), _stunAddress(stunAddress) {}
      SdpFoundation(const SdpFoundation& rhs) :
         _candidateType(rhs._candidateType), _baseAddress(rhs._baseAddress), _stunAddress(rhs._stunAddress) {}

      bool operator==(const SdpFoundation& rhs) { return _candidateType == rhs._candidateType &&
                                                         _baseAddress == rhs._baseAddress &&
                                                         _stunAddress == rhs._stunAddress; }
   private:
      SdpCandidate::SdpCandidateType _candidateType;
      std::string                    _baseAddress;
      std::string                    _stunAddress;
   };

   Sdp();
   Sdp(const Sdp& rSdp);
   virtual ~Sdp();

   Sdp& operator=(const Sdp& rhs);

   bool parse(Sourcey::Util::ByteBuffer& buf);
   bool encode(Sourcey::Util::ByteBuffer& buf);
   std::string toString();

   void setSdpVersion(unsigned int sdpVersion) { _sdpVersion = sdpVersion; }

   void setOriginatorInfo(const std::string& userName, Poco::UInt64 sessionId, Poco::UInt64 sessionVersion, SdpNetType netType, SdpAddressType addressType, const std::string& unicastAddress);
   void setOriginatorUserName(const std::string& originatorUserName) { _originatorUserName = originatorUserName; }
   void setOriginatorSessionId(Poco::UInt64 originatorSessionId) { _originatorSessionId = originatorSessionId; }
   void setOriginatorSessionVersion(Poco::UInt64 originatorSessionVersion) { _originatorSessionVersion = originatorSessionVersion; }
   void setOriginatorNetType(SdpNetType originatorNetType) { _originatorNetType = originatorNetType; }
   void setOriginatorAddressType(SdpAddressType originatorAddressType) { _originatorAddressType = originatorAddressType; }
   void setOriginatorUnicastAddress(const std::string& originatorUnicastAddress) { _originatorUnicastAddress = originatorUnicastAddress; }

   void setSessionName(const std::string& sessionName) { _sessionName = sessionName; }
   void setSessionInformation(const std::string& sessionInformation) { _sessionInformation = sessionInformation; }
   void setSessionUri(const std::string& sessionUri) { _sessionUri = sessionUri; }

   void addEmailAddress(const std::string& emailAddress) { _emailAddresses.push_back(emailAddress); }
   void clearEmailAddresses() { _emailAddresses.clear(); }

   void addPhoneNumber(const std::string& phoneNumber) { _phoneNumbers.push_back(phoneNumber); }
   void clearPhoneNumbers() { _phoneNumbers.clear(); }

   void addBandwidth(SdpBandwidthType type, unsigned int bandwidth) { addBandwidth(SdpBandwidth(type, bandwidth)); }
   void addBandwidth(const SdpBandwidth& sdpBandwidth) { _bandwidths.push_back(sdpBandwidth); }
   void clearBandwidths() { _bandwidths.clear(); }

   void addTime(Poco::UInt64 startTime, Poco::UInt64 stopTime) { addTime(SdpTime(startTime, stopTime)); }
   void addTime(const SdpTime& time) { _times.push_back(time); }
   void clearTimes() { _times.clear(); }

   void addTimeZone(int adjustmentTime, int offset) { addTimeZone(SdpTimeZone(adjustmentTime, offset)); }
   void addTimeZone(const SdpTimeZone& timeZone) { _timeZones.push_back(timeZone); }
   void clearTimeZones() { _timeZones.clear(); }

   void setCategory(const std::string& category) { _category = category; }
   void setKeywords(const std::string& keywords) { _keywords = keywords; }
   void setToolNameAndVersion(const std::string& toolNameAndVersion) { _toolNameAndVersion = toolNameAndVersion; }
   void setConferenceType(SdpConferenceType conferenceType) { _conferenceType = conferenceType; }
   void setCharSet(const std::string& charSet) { _charSet = charSet; }
   void setIcePassiveOnlyMode(bool icePassiveOnlyMode) { _icePassiveOnlyMode = icePassiveOnlyMode; }

   void addGroup(const SdpGroup& group) { _groups.push_back(group); }
   void clearGroups() { _groups.clear(); }

   void setSessionLanguage(const std::string& sessionLanguage) { _sessionLanguage = sessionLanguage; }
   void setDescriptionLanguage(const std::string& descriptionLanguage) { _descriptionLanguage = descriptionLanguage; }
   void setMaximumPacketRate(double maximumPacketRate) { _maximumPacketRate = maximumPacketRate; }

   void addMediaLine(SdpMediaLine* mediaLine);
   void clearMediaLines();
   
   //void toString(std::string& sdpString) const;

   static SdpAddressType getAddressTypeFromString(const std::string& type);

   unsigned int getSdpVersion() const { return _sdpVersion; }

   const std::string& getOriginatorUserName() const { return _originatorUserName; }
   Poco::UInt64 getOriginatorSessionId() const { return _originatorSessionId; }
   Poco::UInt64 getOriginatorSessionVersion() const { return _originatorSessionVersion; }
   SdpNetType getOriginatorNetType() const { return _originatorNetType; }
   SdpAddressType getOriginatorAddressType() const { return _originatorAddressType; }
   const std::string& getOriginatorUnicastAddress() const { return _originatorUnicastAddress; }

   const std::string& getSessionName() const { return _sessionName; }
   const std::string& getSessionInformation() const { return _sessionInformation; }
   const std::string& getSessionUri() const { return _sessionUri; }

   typedef std::list<std::string> EmailAddressList;
   typedef std::list<std::string> PhoneNumberList;
   typedef std::list<SdpBandwidth> BandwidthList;
   typedef std::list<SdpTime> TimeList;
   typedef std::list<SdpTimeZone> TimeZoneList;
   typedef std::list<SdpGroup> GroupList;
   typedef std::list<SdpMediaLine*> MediaLineList;

   const EmailAddressList& getEmailAddresses() const { return _emailAddresses; }
   const PhoneNumberList& getPhoneNumbers() const { return _phoneNumbers; }
   const BandwidthList& getBandwidths() const { return _bandwidths; }
   const TimeList& getTimes() const { return _times; }
   const TimeZoneList& getTimeZones() const { return _timeZones; }

   const std::string& getCategory() const { return _category; }
   const std::string& getKeywords() const { return _keywords; }
   const std::string& getToolNameAndVersion() const { return _toolNameAndVersion; }
   SdpConferenceType getConferenceType() const { return _conferenceType; }
   static SdpConferenceType getConferenceTypeFromString(const std::string& type);
   const std::string& getCharSet() const { return _charSet; }
   bool isIcePassiveOnlyMode() const { return _icePassiveOnlyMode; }

   const GroupList& getGroups() const { return _groups; }

   const std::string& getSessionLanguage() const { return _sessionLanguage; }
   const std::string& getDescriptionLanguage() const { return _descriptionLanguage; }
   double getMaximumPacketRate() const { return _maximumPacketRate; }

   const MediaLineList& getMediaLines() const;

   std::string getLocalFoundationId(SdpCandidate::SdpCandidateType candidateType, const std::string& baseAddress, const std::string& stunAddress=0);

private:
   // v=
   unsigned int   _sdpVersion;

   // o=
   std::string    _originatorUserName;
   Poco::UInt64   _originatorSessionId;
   Poco::UInt64   _originatorSessionVersion;
   SdpNetType     _originatorNetType;
   SdpAddressType _originatorAddressType;
   std::string    _originatorUnicastAddress;

   // s=
   std::string    _sessionName;

   // i=
   std::string    _sessionInformation;

   // u=         
   std::string    _sessionUri;

   // e=
   EmailAddressList _emailAddresses;

   // p=
   PhoneNumberList _phoneNumbers;

   // c= is only stored in SdpMediaLine

   // b=
   BandwidthList   _bandwidths;

   // t=, r=
   TimeList        _times;

   // z=
   TimeZoneList    _timeZones;

   // k= is only stored in sdpMediaLine

   // a= session level only attributes 
   std::string      _category;           // a=cat:<category> - RFC4566
   std::string      _keywords;           // a=keywds:<keywords> - RFC4566
   std::string      _toolNameAndVersion; // a=tool:<name and version of tool> - RFC4566
   SdpConferenceType _conferenceType;  // a=type:<conference type> - RFC4566
   std::string      _charSet;            // a=charset:<character set> - RFC4566
   bool             _icePassiveOnlyMode; // a=ice-passive - ietf-draft-mmusic-ice-12
   GroupList        _groups;             // a=group:<semantics> <id-tag> ... - RFC3388

   // a= attributes that have meaning when not associated to a particular media line
   std::string      _sessionLanguage;     // a=lang:<language tag> - RFC4566
   std::string      _descriptionLanguage; // a=sdplang:<language tag> - RFC4566
   double           _maximumPacketRate;   // a=maxprate:<packetrate> in packets/s - RFC3890

   // Media Lines
   MediaLineList    _mediaLines;

   // Foundation Id 
   std::map<std::string, SdpFoundation> _foundationIds;

   //friend ////EncodeStream& operator<<(EncodeStream& strm, const Sdp& );
};

////EncodeStream& operator<<(EncodeStream& strm, const Sdp& );

} } // namespace Sourcey::Util

#endif  
