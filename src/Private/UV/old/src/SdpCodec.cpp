#include "Sourcey/UV/SdpCodec.h"

using namespace Sourcey::UV;

SdpCodec::SdpCodec(unsigned int payloadType,
                   const std::string& mimeType,
                   const std::string& mimeSubtype,
                   unsigned int rate,
                   unsigned int packetTime,
                   unsigned int numChannels,
                   const std::string&formatParameters) :
   _payloadType(payloadType),
   _mimeType(mimeType),
   _mimeSubtype(mimeSubtype),
   _rate(rate),
   _packetTime(packetTime),
   _numChannels(numChannels),
   _formatParameters(formatParameters)
{
}

SdpCodec::SdpCodec(const SdpCodec& rSdpCodec)
{
   operator=(rSdpCodec); 
}

SdpCodec::~SdpCodec()
{
}

SdpCodec&
SdpCodec::operator=(const SdpCodec& rhs)
{
   if (this == &rhs)            
      return *this;

   _payloadType = rhs._payloadType;
   _mimeType = rhs._mimeType;
   _mimeSubtype = rhs._mimeSubtype;
   _rate = rhs._rate;
   _packetTime = rhs._packetTime;
   _numChannels = rhs._numChannels;
   _formatParameters = rhs._formatParameters;

   return *this;
}

/*
EncodeStream& 
sdpcontainer::operator<<( EncodeStream& strm, const SdpCodec& sdpCodec)
{
   strm << "SdpCodec: payloadId=" << sdpCodec._payloadType
        << ", mime=" << sdpCodec._mimeType << "/" << sdpCodec._mimeSubtype 
        << ", rate=" << sdpCodec._rate
        << ", packetTime=" << sdpCodec._packetTime
        << ", numCh=" << sdpCodec._numChannels 
        << ", fmtParam=" << sdpCodec._formatParameters << std::endl;
   return strm;
}
*/