#if !defined(SdpCandidate_h)
#define SdpCandidate_h

//#include "rutil/compat.h"
//#include "rutil/Data.h"
#include "Poco/Types.h"
#include "Sourcey/Util/Util.h"

#include <list>
#include <assert.h>

namespace Sourcey {
namespace UV {

class SdpCandidate 
{
public:

   typedef enum
   {
      CANDIDATE_TRANSPORT_TYPE_NONE,
      CANDIDATE_TRANSPORT_TYPE_UDP,      // "udp"      - draft-ietf-mmusic-ice-12
      CANDIDATE_TRANSPORT_TYPE_TCP_SO,   // "tcp-so"   - TCP simultaneous-open - draft-ietf-mmusic-ice-tcp-02
      CANDIDATE_TRANSPORT_TYPE_TCP_ACT,  // "tcp-act"  - TCP active - draft-ietf-mmusic-ice-tcp-02
      CANDIDATE_TRANSPORT_TYPE_TCP_PASS, // "tcp-pass" - TCP passive - draft-ietf-mmusic-ice-tcp-02
      CANDIDATE_TRANSPORT_TYPE_TLS_SO,   // "tls-so"   - TCP simultaneous-open - draft-ietf-mmusic-ice-tcp-02
      CANDIDATE_TRANSPORT_TYPE_TLS_ACT,  // "tls-act"  - TCP active - draft-ietf-mmusic-ice-tcp-02
      CANDIDATE_TRANSPORT_TYPE_TLS_PASS  // "tls-pass" - TCP passive - draft-ietf-mmusic-ice-tcp-02
   } SdpCandidateTransportType;
   static const char* SdpCandidateTransportTypeString[];

   typedef enum
   {
      CANDIDATE_TYPE_NONE,
      CANDIDATE_TYPE_HOST,        // "host" - draft-ietf-mmusic-ice-12
      CANDIDATE_TYPE_SRFLX,       // "srflx" - server reflexive - draft-ietf-mmusic-ice-12
      CANDIDATE_TYPE_PRFLX,       // "prflx" - peer reflexive - draft-ietf-mmusic-ice-12
      CANDIDATE_TYPE_RELAY,       // "relay" - draft-ietf-mmusic-ice-12
   } SdpCandidateType;
   static const char* SdpCandidateTypeString[];

   class SdpCandidateExtensionAttribute 
   {
   public:
      SdpCandidateExtensionAttribute(const std::string& name, const std::string& value) : mName(name), mValue(value) {}
      SdpCandidateExtensionAttribute(const SdpCandidateExtensionAttribute& rhs) : mName(rhs.mName), mValue(rhs.mValue) {}

      bool operator==(const SdpCandidateExtensionAttribute& rhs) const { return mName == rhs.mName && mValue == rhs.mValue; }

      void setName(const std::string& name) { mName = name; }
      const std::string& getName() const { return mName; }

      void setValue(const std::string& value) { mValue = value; }
      const std::string& getValue() const { return mValue; }

   private:
      std::string mName;
      std::string mValue;
   };

   SdpCandidate(const std::string& foundation = "",
                unsigned int id = 0,
                SdpCandidateTransportType transport = CANDIDATE_TRANSPORT_TYPE_NONE,
                Poco::UInt64 priority = 0,
                const std::string& connectionAddress = "",
                unsigned int port = 0,
                SdpCandidateType candidateType = CANDIDATE_TYPE_NONE,
                const std::string& relatedAddress = "",
                unsigned int relatedPort = 0,
                bool inUse = false);

   SdpCandidate(const SdpCandidate& rSdpCandidate);

   virtual ~SdpCandidate();

   typedef std::list<SdpCandidateExtensionAttribute> SdpCandidateExtensionAttributeList;

   SdpCandidate& operator=(const SdpCandidate& rhs);
   bool operator<(const SdpCandidate& rhs) const;
   bool operator==(const SdpCandidate& rhs) const;
   bool operator!=(const SdpCandidate& rhs) const;

   void setFoundation(const std::string& foundation) { _foundation = foundation; }
   void setId(unsigned int id) { _id = id; }
   void setTransport(SdpCandidateTransportType transport) { _transport = transport; }
   void setPriority(Poco::UInt64 priority) { _priority = priority; }
   void setConnectionAddress(const std::string& connectionAddress) { _connectionAddress = connectionAddress; }
   void setPort(unsigned int port) { _port = port; }
   void setCandidateType(SdpCandidateType candidateType) { _candidateType = candidateType; }
   void setRelatedAddress(const std::string& relatedAddress) { _relatedAddress = relatedAddress; }
   void setRelatedPort(unsigned int relatedPort) { _relatedPort = relatedPort; }

   void addExtensionAttribute(const std::string& name, const std::string& value) { addExtensionAttribute(SdpCandidateExtensionAttribute(name, value)); }
   void addExtensionAttribute(const SdpCandidateExtensionAttribute& sdpCandidateExtensionAttribute) { _extensionAttributes.push_back(sdpCandidateExtensionAttribute); }
   void clearExtensionAttributes() { _extensionAttributes.clear(); }

   void setInUse(bool inUse) { _inUse = inUse; }

   //void toString(std::string& sdpCandidateString) const;

   const std::string& getFoundation() const { return _foundation; }
   unsigned int getId() const { return _id; }
   SdpCandidateTransportType getTransport() const { return _transport; }
   static SdpCandidateTransportType getCandidateTransportTypeFromString(const std::string& type);
   Poco::UInt64 getPriority() const { return _priority; }
   const std::string& getConnectionAddress() const { return _connectionAddress; }
   unsigned int getPort() const { return _port; }
   SdpCandidateType getCandidateType() const { return _candidateType; }
   static SdpCandidateType getCandidateTypeFromString(const std::string& type);
   const std::string& getRelatedAddress() const { return _relatedAddress; }
   unsigned int getRelatedPort() const { return _relatedPort; }
   const SdpCandidateExtensionAttributeList& getExtensionAttributes() const { return _extensionAttributes; }
   bool isInUse() const { return _inUse; }

private:
   std::string                _foundation;
   unsigned int               _id;
   SdpCandidateTransportType  _transport;
   Poco::UInt64               _priority;
   std::string                _connectionAddress;
   unsigned int               _port;
   SdpCandidateType           _candidateType;
   std::string                _relatedAddress;
   unsigned int               _relatedPort;
   SdpCandidateExtensionAttributeList _extensionAttributes;
   bool                       _inUse;

   //friend ////EncodeStream& operator<<(EncodeStream& strm, const SdpCandidate& );
};

//////EncodeStream& operator<< ( EncodeStream& strm, const SdpCandidate& );

} } // namespace Sourcey::Util

#endif  

/* ====================================================================

 Copyright (c) 2007-2008, Plantronics, Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are 
 met:

 1. Redistributions of source code must retain the above copyright 
    notice, this list of conditions and the following disclaimer. 

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution. 

 3. Neither the name of Plantronics nor the names of its contributors 
    may be used to endorse or promote products derived from this 
    software without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ==================================================================== */
