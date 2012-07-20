#if !defined(SdpCandidatePair_h)
#define SdpCandidatePair_h

#include "Sourcey/UV/SdpCandidate.h"

namespace Sourcey {
namespace UV {

class SdpCandidatePair
{
public:

   typedef enum
   {
      CHECK_STATE_FROZEN,
      CHECK_STATE_WAITING,
      CHECK_STATE_INPROGRESS,
      CHECK_STATE_SUCCEEDED,
      CHECK_STATE_FAILED
   } SdpCandidatePairCheckState;
   static const char* SdpCandidatePairCheckStateString[];

   typedef enum
   {
      OFFERER_LOCAL,
      OFFERER_REMOTE
   } SdpCandidatePairOffererType;
   static const char* SdpCandidatePairOffererTypeString[];

   SdpCandidatePair(const SdpCandidate& localCandidate,
                    const SdpCandidate& remoteCandidate,
                    SdpCandidatePairOffererType offerer);

   SdpCandidatePair(const SdpCandidatePair& rSdpCandidatePair);

   virtual
   ~SdpCandidatePair();

   SdpCandidatePair& operator=(const SdpCandidatePair& rhs);
   bool operator<(const SdpCandidatePair& rhs) const;

   void setLocalCandidate(const SdpCandidate& localCandidate) { _localCandidate = localCandidate; resetPriority(); }
   void setRemoteCandidate(const SdpCandidate& remoteCandidate) { _remoteCandidate = remoteCandidate; resetPriority(); }
   void setOfferer(const SdpCandidatePairOffererType offerer) { _offerer = offerer; resetPriority(); }
   bool setCheckState(const SdpCandidatePairCheckState checkState);

   //void toString(std::string& sdpCandidateString) const;

   const SdpCandidate& getLocalCandidate() const { return _localCandidate; }
   const SdpCandidate& getRemoteCandidate() const { return _remoteCandidate; }
   SdpCandidatePairOffererType getOfferer() const { return _offerer; }
   Poco::UInt64 getPriority() const { return _priority; }
   SdpCandidatePairCheckState getCheckState() const { return _checkState; }

private:
   void resetPriority();

   SdpCandidate _localCandidate;
   SdpCandidate _remoteCandidate;
   SdpCandidatePairOffererType _offerer;
   Poco::UInt64     _priority;
   SdpCandidatePairCheckState _checkState;

   //friend ////EncodeStream& operator<<(EncodeStream& strm, const SdpCandidatePair& );
};

//////EncodeStream& operator<< ( EncodeStream& strm, const SdpCandidatePair& );

} } // namespace Sourcey::Util

#endif  