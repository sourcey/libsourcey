#include "Sourcey/UV/SdpCandidatePair.h"

using namespace Sourcey::UV;

const char* SdpCandidatePair::SdpCandidatePairCheckStateString[] =
{
   "FROZEN",
   "WAITING",
   "INPROGRESS",
   "SUCCEEDED",
   "FAILED"
};

const char* SdpCandidatePair::SdpCandidatePairOffererTypeString[] =
{
   "LOCAL", 
   "REMOTE"
};

// Constructor
SdpCandidatePair::SdpCandidatePair(const SdpCandidate& localCandidate,
                                   const SdpCandidate& remoteCandidate,
                                   SdpCandidatePairOffererType offerer)
: _localCandidate(localCandidate)
, _remoteCandidate(remoteCandidate)
, _offerer(offerer)
{
   resetPriority();
   _checkState = CHECK_STATE_FROZEN;
}

// Copy constructor
SdpCandidatePair::SdpCandidatePair(const SdpCandidatePair& rhs)
{
   operator=(rhs); 
}

// Destructor
SdpCandidatePair::~SdpCandidatePair()
{
}

// Assignment operator
SdpCandidatePair&
SdpCandidatePair::operator=(const SdpCandidatePair& rhs)
{
   if (this == &rhs)            // handle the assignment to self case
      return *this;

   // Assign values
   _localCandidate = rhs._localCandidate;
   _remoteCandidate = rhs._remoteCandidate;
   _offerer = rhs._offerer;
   _priority = rhs._priority;
   _checkState = rhs._checkState;

   return *this;
}

bool SdpCandidatePair::setCheckState(const SdpCandidatePairCheckState checkState)
{
   bool stateChangeSuccess=false;
   switch(_checkState)
   {
   case CHECK_STATE_FROZEN:
      switch(checkState)
      {
      case CHECK_STATE_WAITING:
      case CHECK_STATE_INPROGRESS:
         _checkState = checkState;
         stateChangeSuccess = true;
         break;
      default:
         assert(false);
      }
      break;
   case CHECK_STATE_WAITING:
      switch(checkState)
      {
      case CHECK_STATE_INPROGRESS:
         _checkState = checkState;
         stateChangeSuccess = true;
         break;
      default:
         assert(false);
      }
      break;
   case CHECK_STATE_INPROGRESS:
      switch(checkState)
      {
      case CHECK_STATE_SUCCEEDED:
      case CHECK_STATE_FAILED:
         _checkState = checkState;
         stateChangeSuccess = true;
         break;
      default:
         assert(false);
      }
      break;
   case CHECK_STATE_SUCCEEDED:
   case CHECK_STATE_FAILED:
   default:
      assert(false);
      break;
   }
   return stateChangeSuccess;
}

bool SdpCandidatePair::operator<(const SdpCandidatePair& rhs) const
{
   if(_priority != rhs._priority)
   {
      return _priority > rhs._priority;  // We want to order a list of these from highest priority to lowest - so condition is reversed
   }
   
   if(_checkState != rhs._checkState)
   {
      return _checkState < rhs._checkState;
   }

   if(_localCandidate != rhs._localCandidate)
   {
      return _localCandidate < rhs._localCandidate;
   }

   if(_remoteCandidate != rhs._remoteCandidate)
   {
      return _remoteCandidate < rhs._remoteCandidate;
   } 

   return false;   // equal
}

#define sdpMin(a,b) (a < b ? a : a > b ? b : a)
#define sdpMax(a,b) (a > b ? a : a < b ? b : a)
void SdpCandidatePair::resetPriority()
{
   Poco::UInt64 offererPriority = _offerer == OFFERER_LOCAL ? _localCandidate.getPriority() : _remoteCandidate.getPriority();
   Poco::UInt64 answererPriority = _offerer == OFFERER_LOCAL ? _remoteCandidate.getPriority() : _localCandidate.getPriority();
   _priority = (sdpMin(offererPriority, answererPriority)<<32) + 
               (sdpMax(offererPriority, answererPriority)<<1) + 
               (offererPriority > answererPriority ? 1 : 0);
}

/*
EncodeStream& 
sdpcontainer::operator<<( EncodeStream& strm, const SdpCandidatePair& sdpCandidatePair)
{
   strm << "SdpCandidatePair:" << std::endl
        << "  Priority: " << sdpCandidatePair._priority << std::endl
        << "  State: " << SdpCandidatePair::SdpCandidatePairCheckStateString[sdpCandidatePair._checkState] << std::endl
        << "  Offerer: " << SdpCandidatePair::SdpCandidatePairOffererTypeString[sdpCandidatePair._offerer] << std::endl
        << "  " << sdpCandidatePair._localCandidate
        << "  " << sdpCandidatePair._remoteCandidate;

   return strm;
}
*/
