#include "Sourcey/UV/SdpCandidate.h"

using namespace Sourcey::UV;

const char* SdpCandidate::SdpCandidateTransportTypeString[] =
{
   "NONE",
   "UDP",
   "TCP-SO",
   "TCP-ACT",
   "TCP-PASS",
   "TLS-SO",
   "TLS-ACT",
   "TLS-PASS"
};

const char* SdpCandidate::SdpCandidateTypeString[] =
{
   "NONE",
   "HOST",
   "SRFLX",
   "PRFLX",
   "RELAY"
};

// Constructor
SdpCandidate::SdpCandidate(const std::string& foundation,
                           unsigned int id,
                           SdpCandidateTransportType transport,
                           Poco::UInt64 priority,
                           const std::string& connectionAddress,
                           unsigned int port,
                           SdpCandidateType candidateType,
                           const std::string& relatedAddress,
                           unsigned int relatedPort, 
                           bool inUse) :
      _foundation(foundation),
      _id(id),
      _transport(transport),
      _priority(priority),
      _connectionAddress(connectionAddress),
      _port(port),
      _candidateType(candidateType),
      _relatedAddress(relatedAddress),
      _relatedPort(relatedPort),
      _inUse(inUse)/* */
{
}

// Copy constructor
SdpCandidate::SdpCandidate(const SdpCandidate& rhs)
{
   operator=(rhs);  
}

// Destructor
SdpCandidate::~SdpCandidate()
{
}

SdpCandidate& 
SdpCandidate::operator=(const SdpCandidate& rhs)
{
   if (this == &rhs)            // handle the assignment to self case
      return *this;

   // Assign values
   _foundation = rhs._foundation;
   _id = rhs._id;
   _transport = rhs._transport;
   _priority = rhs._priority;
   _connectionAddress = rhs._connectionAddress;
   _port = rhs._port;
   _candidateType = rhs._candidateType;
   _relatedAddress = rhs._relatedAddress;
   _relatedPort = rhs._relatedPort;
   _inUse = rhs._inUse;
   _extensionAttributes = rhs._extensionAttributes;

   return *this;
}

bool 
SdpCandidate::operator==(const SdpCandidate& rhs) const
{
   return _foundation == rhs._foundation &&
          _id == rhs._id &&
          _transport == rhs._transport &&
          _priority == rhs._priority &&
          _connectionAddress == rhs._connectionAddress &&
          _port == rhs._port &&
          _candidateType == rhs._candidateType &&
          _relatedAddress == rhs._relatedAddress &&
          _relatedPort == rhs._relatedPort &&
          _extensionAttributes == rhs._extensionAttributes &&
          _inUse == rhs._inUse;
}

bool 
SdpCandidate::operator!=(const SdpCandidate& rhs) const
{
   return !operator==(rhs);
}

bool 
SdpCandidate::operator<(const SdpCandidate& rhs) const
{
   if(_priority != rhs._priority)
   {
      return _priority > rhs._priority;  // We want to order a list of these from highest priority to lowest - so condition is reversed
   }
   
   // Priority should be unique, so we shouldn't get here, but implementation is included for completeness
   if(_foundation != rhs._foundation)
   {
      return _foundation < rhs._foundation;
   }

   if(_id != rhs._id)
   {
      return _id < rhs._id;
   }

   if(_transport != rhs._transport)
   {
      return _transport < rhs._transport;
   }
   
   if(_connectionAddress != rhs._connectionAddress)
   {
      return _connectionAddress < rhs._connectionAddress;
   }

   if(_port != rhs._port)
   {
      return _port < rhs._port;
   }

   if(_candidateType != rhs._candidateType)
   {
      return _candidateType < rhs._candidateType;
   }

   if(_relatedAddress != rhs._relatedAddress)
   {
      return _relatedAddress < rhs._relatedAddress;
   }

   if(_relatedPort != rhs._relatedPort)
   {
      return _relatedPort < rhs._relatedPort;
   }

   return false;  // equal
}

SdpCandidate::SdpCandidateTransportType 
SdpCandidate::getCandidateTransportTypeFromString(const std::string& type)
{
   //std::string dataType(type);

   if(Util::icompare("udp", type))
   {
      return CANDIDATE_TRANSPORT_TYPE_UDP;
   }
   else if(Util::icompare("tcp-so", type))
   {
      return CANDIDATE_TRANSPORT_TYPE_TCP_SO;
   }
   else if(Util::icompare("tcp-act", type))
   {
      return CANDIDATE_TRANSPORT_TYPE_TCP_ACT;
   }
   else if(Util::icompare("tcp-pass", type))
   {
      return CANDIDATE_TRANSPORT_TYPE_TCP_PASS;
   }
   else if(Util::icompare("tls-so", type))
   {
      return CANDIDATE_TRANSPORT_TYPE_TLS_SO;
   }
   else if(Util::icompare("tls-act", type))
   {
      return CANDIDATE_TRANSPORT_TYPE_TLS_ACT;
   }
   else if(Util::icompare("tls-pass", type))
   {
      return CANDIDATE_TRANSPORT_TYPE_TLS_PASS;
   }
   else
   {
      return CANDIDATE_TRANSPORT_TYPE_NONE;
   }
}

SdpCandidate::SdpCandidateType 
SdpCandidate::getCandidateTypeFromString(const std::string& type)
{
   //std::string dataType(type);

   if(Util::icompare("host", type))
   {
      return CANDIDATE_TYPE_HOST;
   }
   else if(Util::icompare("srflx", type))
   {
      return CANDIDATE_TYPE_SRFLX;
   }
   else if(Util::icompare("prflx", type))
   {
      return CANDIDATE_TYPE_PRFLX;
   }
   else if(Util::icompare("relay", type))
   {
      return CANDIDATE_TYPE_RELAY;
   }
   else
   {
      return CANDIDATE_TYPE_NONE;
   }
}

/*
EncodeStream& 
sdpcontainer::operator<<( EncodeStream& strm, const SdpCandidate& sdpCandidate)
{
   strm << "SdpCandidate: foundation=" << sdpCandidate._foundation
        << ", id=" << sdpCandidate._id
        << ", transport=" << SdpCandidate::SdpCandidateTransportTypeString[sdpCandidate._transport]
        << ", priority=" << sdpCandidate._priority 
        << ", addr=" << sdpCandidate._connectionAddress
        << ", port=" << sdpCandidate._port
        << ", type=" << SdpCandidate::SdpCandidateTypeString[sdpCandidate._candidateType]
        << ", relatedAddr=" << sdpCandidate._relatedAddress 
        << ", relatedPort=" << sdpCandidate._relatedPort
        << ", ";

   SdpCandidate::SdpCandidateExtensionAttributeList::const_iterator it = sdpCandidate._extensionAttributes.begin();
   for(;it != sdpCandidate._extensionAttributes.end(); it++)
   {
      strm << it->getName() << "=" << it->getValue() << ", ";
   }

   strm << "inUse=" << sdpCandidate._inUse << std::endl;
   return strm;
}
*/