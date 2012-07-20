#if !defined(SdpCodec_h)
#define SdpCodec_h

//#include "rutil/compat.h"
//#include "rutil/Data.h"

#include "Poco/Types.h"

namespace Sourcey {
namespace UV {

class SdpCodec
{
public:

   SdpCodec(unsigned int payloadType,
            const std::string& mimeType,
            const std::string& mimeSubType,
            unsigned int rate,
            unsigned int packetTime,
            unsigned int numChannels,
            const std::string& formatParameters);

   SdpCodec(const SdpCodec& rSdpCodec);

   virtual ~SdpCodec();

   SdpCodec& operator=(const SdpCodec& rhs);

   void setPayloadType(unsigned int payloadType) { _payloadType = payloadType; }
   void setMimeType(const std::string& mimeType) { _mimeType = mimeType; }
   void setMimeSubtype(const std::string& mimeSubtype) { _mimeSubtype = mimeSubtype; }
   void setRate(unsigned int rate) { _rate = rate; }
   void setNumChannels(unsigned int numChannels) { _numChannels = numChannels; }
   void setPacketTime(unsigned int packetTime) { _packetTime = packetTime; }
   void setFormatParameters(const std::string& formatParameters) { _formatParameters = formatParameters; }

   unsigned int getPayloadType() const { return _payloadType; }
   const std::string& getMimeType() const { return _mimeType; }   
   const std::string& getMimeSubtype() const { return _mimeSubtype; }   
   unsigned int getRate() const { return _rate; }
   unsigned int getNumChannels() const { return _numChannels; }
   unsigned int getPacketTime() const { return _packetTime; }
   const std::string& getFormatParameters() const { return _formatParameters; }   

   //void toString(std::string& sdpCodecString) const;

private:
    unsigned int _payloadType;      
    std::string _mimeType;        
    std::string _mimeSubtype;     
    unsigned int _rate;              
    unsigned int _packetTime; // ptime
    unsigned int _numChannels;
    std::string _formatParameters; 

    //friend ////EncodeStream& operator<<(EncodeStream& strm, const SdpCodec& );
};

////EncodeStream& operator<<(EncodeStream& strm, const SdpCodec& );

} } // namespace Sourcey::Util

#endif  
