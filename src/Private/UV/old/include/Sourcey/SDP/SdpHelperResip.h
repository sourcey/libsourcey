#if !defined(SdpHelperResip_hxx)
#define SdpHelperResip_hxx

#include "Sdp.h"
#include "SdpMediaLine.h"

#include "Poco/Types.h"

//#include <resip/stack/SdpContents.h>

using namespace Sourcey::UV;

/*
namespace resip
{
   class ParseBuffer;
   class Data;
}
*/

/**
  This class is used to form a sipX representation of UV from
  a resiprocate representation.

  Author: Scott Godin (sgodin AT SipSpectrum DOT com)
*/

class SdpHelperResip
{
public:
   static Sdp::SdpAddressType convertResipAddressType(resip::SdpContents::AddrType resipAddrType);
   static SdpMediaLine::SdpEncryptionMethod convertResipEncryptionMethod(resip::SdpContents::Session::Encryption::KeyType resipMethod);
   static Sdp* createSdpFromResipSdp(const resip::SdpContents& resipSdp);

   static SdpMediaLine::SdpCrypto* parseCryptoLine(const std::string& cryptoLine);
   static bool parseFingerPrintLine(const std::string& fingerprintLine, SdpMediaLine::SdpFingerPrintHashFuncType& hashType, std::string& fingerPrint);
   static bool parseTransportCapabilitiesLine(const std::string& tcapLine, std::list<SdpMediaLine::SdpTransportProtocolCapabilities>& tcapList);
   static bool parsePotentialConfigurationLine(const std::string& pcfgLine, std::list<SdpMediaLine::SdpPotentialConfiguration>& pcfgList);

private:
   static SdpMediaLine* parseMediaLine(const resip::SdpContents::Session::Medium& resipMedia, const resip::SdpContents::Session& resipSession, bool sessionRtcpEnabled);
   static void parseCryptoParams(resip::ParseBuffer& pb, 
                                 SdpMediaLine::SdpCryptoKeyMethod& keyMethod, 
                                 std::string& keyValue, 
                                 unsigned int& srtpLifetime, 
                                 unsigned int& srtpMkiValue, 
                                 unsigned int& srtpMkiLength);

};

#endif  // _SdpHelperResip_hxx_


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
