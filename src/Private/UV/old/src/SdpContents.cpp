#if defined(HAVE_CONFIG_H)
#include "resip/stack/config.h"
#endif

#include "resip/stack/SdpContents.h"
#include "resip/stack/Helper.h"
#include "rutil/ParseBuffer.h"
#include "rutil/DataStream.h"
#include "resip/stack/Symbols.h"
#include "rutil/Logger.h"
#include "rutil/WinLeakCheck.h"

#define RESIPROCATE_SUBSYSTEM resip::Subsystem::UV

using namespace resip;
using namespace std;

const SdpContents SdpContents::Empty;

bool
SdpContents::init()
{
   static ContentsFactory<SdpContents> factory;
   (void)factory;
   return true;
}

const char* NetworkType[] = {"???", "IP4", "IP6"};

static const Data rtpmap("rtpmap");
static const Data fmtp("fmtp");

// RFC2327 6. page 9
// "parsers should be tolerant and accept records terminated with a single
// newline character"
void skipEol(ParseBuffer& pb)
{
   while(!pb.eof() && (*pb.position() == Symbols::SPACE[0] ||
                       *pb.position() == Symbols::TAB[0]))
   {
      pb.skipChar();
   }
   
   if (*pb.position() == Symbols::LF[0])
   {
      pb.skipChar();
   }
   else
   {
      // allow extra 0x0d bytes.
      while(*pb.position() == Symbols::CR[0])
      {
         pb.skipChar();
      } 
      pb.skipChar(Symbols::LF[0]);
   }
   
}

AttributeHelper::AttributeHelper(const AttributeHelper& rhs)
   : mAttributeList(rhs.mAttributeList),
     mAttributes(rhs.mAttributes)
{
}

AttributeHelper::AttributeHelper()
{
}

AttributeHelper&
AttributeHelper::operator=(const AttributeHelper& rhs)
{
   if (this != &rhs)
   {
      mAttributeList = rhs.mAttributeList;
      mAttributes = rhs.mAttributes;
   }
   return *this;
}

bool
AttributeHelper::exists(const Data& key) const
{
   return mAttributes.find(key) != mAttributes.end();
}

const list<Data>&
AttributeHelper::getValues(const Data& key) const
{
   if (!exists(key))
   {
      static const list<Data> emptyList;
      return emptyList;
   }
   return mAttributes.find(key)->second;
}

EncodeStream&
AttributeHelper::encode(EncodeStream& s) const
{
   for (std::list<std::pair<Data, Data> >::const_iterator i = mAttributeList.begin();
        i != mAttributeList.end(); ++i)
   {
      s << "a=" << i->first;
      if (!i->second.empty())
      {
         s << Symbols::COLON[0] << i->second;
      }
      s << Symbols::CRLF;
   }  
   return s;
}

void
AttributeHelper::parse(ParseBuffer& pb)
{
   while (!pb.eof() && *pb.position() == 'a')
   {
      Data key;
      Data value;

      pb.skipChar('a');
      const char* anchor = pb.skipChar(Symbols::EQUALS[0]);
      pb.skipToOneOf(Symbols::COLON, Symbols::CRLF);
      pb.data(key, anchor);
      if (!pb.eof() && *pb.position() == Symbols::COLON[0])
      {
         anchor = pb.skipChar(Symbols::COLON[0]);
         pb.skipToOneOf(Symbols::CRLF);
         pb.data(value, anchor);
      }

      if(!pb.eof()) skipEol(pb);

      mAttributeList.push_back(std::make_pair(key, value));
      mAttributes[key].push_back(value);
   }
}

void
AttributeHelper::addAttribute(const Data& key, const Data& value)
{
   mAttributeList.push_back(std::make_pair(key, value));
   mAttributes[key].push_back(value);
}

void
AttributeHelper::clearAttribute(const Data& key)
{
   for (std::list<std::pair<Data, Data> >::iterator i = mAttributeList.begin();
        i != mAttributeList.end(); )
   {
      std::list<std::pair<Data, Data> >::iterator j = i++;
      if (j->first == key)
      {
         mAttributeList.erase(j);
      }
   }
   mAttributes.erase(key);
}

SdpContents::SdpContents() : Contents(getStaticType())
{
}

SdpContents::SdpContents(HeaderFieldValue* hfv, const Mime& contentTypes)
   : Contents(hfv, contentTypes)
{
}

//SdpContents::SdpContents(const SdpContents& rhs)
//   : Contents(rhs),
//     mSession(rhs.mSession)
//{
//}

SdpContents::~SdpContents()
{
}


SdpContents&
SdpContents::operator=(const SdpContents& rhs)
{
   if (this != &rhs)
   {
      Contents::operator=(rhs);
      mSession = rhs.mSession;
   }
   return *this;
}

Contents*
SdpContents::clone() const
{
   return new SdpContents(*this);
}

void
SdpContents::parse(ParseBuffer& pb)
{
   mSession.parse(pb);
}

EncodeStream&
SdpContents::encodeParsed(EncodeStream& s) const
{
   mSession.encode(s);
   return s;
}

const Mime&
SdpContents::getStaticType()
{
   static Mime type("application", "sdp");
   return type;
}

static Data nullOrigin("0.0.0.0");

SdpContents::Session::Origin::Origin()
   : mUser(Data::Empty),
     mSessionId(0),
     mVersion(0),
     mAddrType(IP4),
     mAddress(nullOrigin)
{}

SdpContents::Session::Origin::Origin(const Origin& rhs)
   : mUser(rhs.mUser),
     mSessionId(rhs.mSessionId),
     mVersion(rhs.mVersion),
     mAddrType(rhs.mAddrType),
     mAddress(rhs.mAddress)
{
}

SdpContents::Session::Origin&
SdpContents::Session::Origin::operator=(const Origin& rhs)
{
   if (this != &rhs)
   {
      mUser = rhs.mUser;
      mSessionId = rhs.mSessionId;
      mVersion = rhs.mVersion;
      mAddrType = rhs.mAddrType;
      mAddress = rhs.mAddress;
   }
   return *this;
}


SdpContents::Session::Origin::Origin(const Data& user,
                                     const UInt64& sessionId,
                                     const UInt64& version,
                                     AddrType addr,
                                     const Data& address)
   : mUser(user),
     mSessionId(sessionId),
     mVersion(version),
     mAddrType(addr),
     mAddress(address)
{}

EncodeStream&
SdpContents::Session::Origin::encode(EncodeStream& s) const
{
   s << "o="
     << mUser << Symbols::SPACE[0]
     << mSessionId << Symbols::SPACE[0]
     << mVersion << Symbols::SPACE[0]
     << "IN "
     << NetworkType[mAddrType] << Symbols::SPACE[0]
     << mAddress << Symbols::CRLF;
   return s;
}

void
SdpContents::Session::Origin::setAddress(const Data& host, AddrType addr)
{
    mAddress = host;
    mAddrType = addr;
}

void
SdpContents::Session::Origin::parse(ParseBuffer& pb)
{
   pb.skipChar('o');
   const char* anchor = pb.skipChar(Symbols::EQUALS[0]);

   pb.skipToChar(Symbols::SPACE[0]);
   pb.data(mUser, anchor);

   anchor = pb.skipChar(Symbols::SPACE[0]);
   mSessionId = pb.uInt64();
   pb.skipToChar(Symbols::SPACE[0]);

   anchor = pb.skipChar(Symbols::SPACE[0]);
   mVersion = pb.uInt64();
   pb.skipToChar(Symbols::SPACE[0]);

   pb.skipChar(Symbols::SPACE[0]);
   pb.skipChar('I');
   pb.skipChar('N');

   anchor = pb.skipChar(Symbols::SPACE[0]);
   pb.skipToChar(Symbols::SPACE[0]);
   Data addrType;
   pb.data(addrType, anchor);
   if (addrType == NetworkType[IP4])
   {
      mAddrType = IP4;
   }
   else if (addrType == NetworkType[IP6])
   {
      mAddrType = IP6;
   }
   else
   {
      mAddrType = static_cast<AddrType>(0);
   }

   anchor = pb.skipChar(Symbols::SPACE[0]);
   pb.skipToOneOf(Symbols::CRLF);
   pb.data(mAddress, anchor);

   skipEol(pb);
}

SdpContents::Session::Email::Email(const Data& address,
                                   const Data& freeText)
   : mAddress(address),
     mFreeText(freeText)
{}

SdpContents::Session::Email::Email(const Email& rhs)
   : mAddress(rhs.mAddress),
     mFreeText(rhs.mFreeText)
{}

SdpContents::Session::Email&
SdpContents::Session::Email::operator=(const Email& rhs)
{
   if (this != &rhs)
   {
      mAddress = rhs.mAddress;
      mFreeText = rhs.mFreeText;
   }
   return *this;
}

EncodeStream&
SdpContents::Session::Email::encode(EncodeStream& s) const
{
   s << "e=" << mAddress;
   if (!mFreeText.empty())
   {
      s << Symbols::SPACE[0];
      s << Symbols::LPAREN[0] << mFreeText << Symbols::RPAREN[0];
   }
   s << Symbols::CRLF;

   return s;
}

// helper to parse email and phone numbers with display name
void parseEorP(ParseBuffer& pb, Data& eOrp, Data& freeText)
{
   // =mjh@isi.edu (Mark Handley)
   // =mjh@isi.edu
   // =Mark Handley <mjh@isi.edu>
   // =<mjh@isi.edu>

   const char* anchor = pb.skipChar(Symbols::EQUALS[0]);

   pb.skipToOneOf("<(\n\r");  // find a left angle bracket "<", a left paren "(", or a CR 
   switch (*pb.position())
   {
      case '\n':					// Symbols::CR[0]
      case '\r':					// Symbols::LF[0]
         // mjh@isi.edu
         //            ^
         pb.data(eOrp, anchor);
         break;

      case '<':					// Symbols::LA_QUOTE[0]
         // Mark Handley <mjh@isi.edu>
         //              ^
         // <mjh@isi.edu>
         // ^
		  
         pb.data(freeText, anchor);
         anchor = pb.skipChar();
         pb.skipToEndQuote(Symbols::RA_QUOTE[0]);
         pb.data(eOrp, anchor);
         pb.skipChar(Symbols::RA_QUOTE[0]);
         break;
		  
      case '(':					// Symbols::LPAREN[0]
         // mjh@isi.edu (Mark Handley)
         //             ^
		  
         pb.data(eOrp, anchor);
         anchor = pb.skipChar();
         pb.skipToEndQuote(Symbols::RPAREN[0]);
         pb.data(freeText, anchor);
         pb.skipChar(Symbols::RPAREN[0]);
         break;
      default:
         assert(0);
   }
}

void
SdpContents::Session::Email::parse(ParseBuffer& pb)
{
   pb.skipChar('e');
   parseEorP(pb, mAddress, mFreeText);
   skipEol(pb);
}

SdpContents::Session::Phone::Phone(const Data& number,
                                   const Data& freeText)
   : mNumber(number),
     mFreeText(freeText)
{}

SdpContents::Session::Phone::Phone(const Phone& rhs)
   : mNumber(rhs.mNumber),
     mFreeText(rhs.mFreeText)
{}

SdpContents::Session::Phone&
SdpContents::Session::Phone::operator=(const Phone& rhs)
{
   if (this != &rhs)
   {
      mNumber = rhs.mNumber;
      mFreeText = rhs.mFreeText;
   }
   return *this;
}

EncodeStream&
SdpContents::Session::Phone::encode(EncodeStream& s) const
{
  s << "p=" << mNumber;
   if (!mFreeText.empty())
   {
      s << Symbols::SPACE[0];
      s << Symbols::LPAREN[0] << mFreeText << Symbols::RPAREN[0];
   }
   s << Symbols::CRLF;

   return s;
}

void
SdpContents::Session::Phone::parse(ParseBuffer& pb)
{
   pb.skipChar('p');
   parseEorP(pb, mNumber, mFreeText);
   skipEol(pb);
}

SdpContents::Session::Connection::Connection(AddrType addType,
                                             const Data& address,
                                             unsigned long ttl)
   : mAddrType(addType),
     mAddress(address),
     mTTL(ttl)
{}

SdpContents::Session::Connection::Connection()
   : mAddrType(IP4),
     mAddress(Data::Empty),
     mTTL(0)
{}

SdpContents::Session::Connection::Connection(const Connection& rhs)
   : mAddrType(rhs.mAddrType),
     mAddress(rhs.mAddress),
     mTTL(rhs.mTTL)
{
}

SdpContents::Session::Connection&
SdpContents::Session::Connection::operator=(const Connection& rhs)
{
   if (this != &rhs)
   {
      mAddrType = rhs.mAddrType;
      mAddress = rhs.mAddress;
      mTTL = rhs.mTTL;
   }
   return *this;
}

EncodeStream&
SdpContents::Session::Connection::encode(EncodeStream& s) const
{
   s << "c=IN "
     << NetworkType[mAddrType] << Symbols::SPACE[0] << mAddress;

   if (mTTL)
   {
      s << Symbols::SLASH[0] << mTTL;
   }
   s << Symbols::CRLF;
   return s;
}

void
SdpContents::Session::Connection::setAddress(const Data& host, AddrType addr)
{
    mAddress = host;
    mAddrType = addr;
}

void
SdpContents::Session::Connection::parse(ParseBuffer& pb)
{
   pb.skipChar('c');
   pb.skipChar(Symbols::EQUALS[0]);
   pb.skipChar('I');
   pb.skipChar('N');

   const char* anchor = pb.skipChar(Symbols::SPACE[0]);
   pb.skipToChar(Symbols::SPACE[0]);
   Data addrType;
   pb.data(addrType, anchor);
   if (addrType == NetworkType[IP4])
   {
      mAddrType = IP4;
   }
   else if (addrType == NetworkType[IP6])
   {
      mAddrType = IP6;
   }
   else
   {
      mAddrType = static_cast<AddrType>(0);
   }

   anchor = pb.skipChar();
   pb.skipToOneOf(Symbols::SLASH, Symbols::CRLF);
   pb.data(mAddress, anchor);

   mTTL = 0;
   if (mAddrType == IP4 && !pb.eof() && *pb.position() == Symbols::SLASH[0])
   {
      pb.skipChar();
      mTTL = pb.integer();
   }

   // multicast dealt with above this parser
   if (!pb.eof() && *pb.position() != Symbols::SLASH[0])
   {
      skipEol(pb);
   }
}

SdpContents::Session::Bandwidth::Bandwidth(const Data& modifier,
                                           unsigned long kbPerSecond)
   : mModifier(modifier),
     mKbPerSecond(kbPerSecond)
{}

SdpContents::Session::Bandwidth::Bandwidth(const Bandwidth& rhs)
   : mModifier(rhs.mModifier),
     mKbPerSecond(rhs.mKbPerSecond)
{}

SdpContents::Session::Bandwidth&
SdpContents::Session::Bandwidth::operator=(const Bandwidth& rhs)
{
   if (this != &rhs)
   {
      mModifier = rhs.mModifier;
      mKbPerSecond = rhs.mKbPerSecond;
   }
   return *this;
}

EncodeStream&
SdpContents::Session::Bandwidth::encode(EncodeStream& s) const
{
   s << "b="
     << mModifier
     << Symbols::COLON[0] << mKbPerSecond
     << Symbols::CRLF;
   return s;
}

void
SdpContents::Session::Bandwidth::parse(ParseBuffer& pb)
{
   pb.skipChar('b');
   const char* anchor = pb.skipChar(Symbols::EQUALS[0]);

   pb.skipToOneOf(Symbols::COLON, Symbols::CRLF);
   if (*pb.position() == Symbols::COLON[0])
   {
      pb.data(mModifier, anchor);

      anchor = pb.skipChar(Symbols::COLON[0]);
      mKbPerSecond = pb.integer();

      skipEol(pb);
   }
   else
   {
      pb.fail(__FILE__, __LINE__);
   }
}

SdpContents::Session::Time::Time(unsigned long start,
                                 unsigned long stop)
   : mStart(start),
     mStop(stop)
{}

SdpContents::Session::Time::Time(const Time& rhs)
   : mStart(rhs.mStart),
     mStop(rhs.mStop)
{}

SdpContents::Session::Time&
SdpContents::Session::Time::operator=(const Time& rhs)
{
   if (this != &rhs)
   {
      mStart = rhs.mStart;
      mStop = rhs.mStop;
      mRepeats = rhs.mRepeats;
   }
   return *this;
}

EncodeStream&
SdpContents::Session::Time::encode(EncodeStream& s) const
{
   s << "t=" << mStart << Symbols::SPACE[0]
     << mStop
     << Symbols::CRLF;

   for (list<Repeat>::const_iterator i = mRepeats.begin();
        i != mRepeats.end(); ++i)
   {
      i->encode(s);
   }
   return s;
}

void
SdpContents::Session::Time::parse(ParseBuffer& pb)
{
   pb.skipChar('t');
   pb.skipChar(Symbols::EQUALS[0]);

   mStart = pb.uInt32();
   pb.skipChar(Symbols::SPACE[0]);
   mStop = pb.uInt32();

   skipEol(pb);

   while (!pb.eof() && *pb.position() == 'r')
   {
      addRepeat(Repeat());
      mRepeats.back().parse(pb);
   }
}

void
SdpContents::Session::Time::addRepeat(const Repeat& repeat)
{
   mRepeats.push_back(repeat);
}

SdpContents::Session::Time::Repeat::Repeat(unsigned long interval,
                                           unsigned long duration,
                                           list<int> offsets)
   : mInterval(interval),
     mDuration(duration),
     mOffsets(offsets)
{}

EncodeStream&
SdpContents::Session::Time::Repeat::encode(EncodeStream& s) const
{
   s << "r="
     << mInterval << Symbols::SPACE[0]
     << mDuration << 's';
   for (list<int>::const_iterator i = mOffsets.begin();
        i != mOffsets.end(); ++i)
   {
      s << Symbols::SPACE[0] << *i << 's';
   }

   s << Symbols::CRLF;
   return s;
}

int
parseTypedTime(ParseBuffer& pb)
{
   int v = pb.integer();
   if (!pb.eof())
   {
      switch (*pb.position())
      {
	 case 's' :
	    pb.skipChar();
	    break;
	 case 'm' :
	    v *= 60;
	    pb.skipChar();
	    break;
	 case 'h' :
	    v *= 3600;
	    pb.skipChar();
       break;
	 case 'd' :
	    v *= 3600*24;
	    pb.skipChar();
      }
   }
   return v;
}

void
SdpContents::Session::Time::Repeat::parse(ParseBuffer& pb)
{
   pb.skipChar('r');
   pb.skipChar(Symbols::EQUALS[0]);

   mInterval = parseTypedTime(pb);
   pb.skipChar(Symbols::SPACE[0]);

   mDuration = parseTypedTime(pb);

   while (!pb.eof() && *pb.position() != Symbols::CR[0])
   {
      pb.skipChar(Symbols::SPACE[0]);

      mOffsets.push_back(parseTypedTime(pb));
   }

   skipEol(pb);
}

SdpContents::Session::Timezones::Adjustment::Adjustment(unsigned long _time,
                                                        int _offset)
   : time(_time),
     offset(_offset)
{}

SdpContents::Session::Timezones::Adjustment::Adjustment(const Adjustment& rhs)
   : time(rhs.time),
     offset(rhs.offset)
{}

SdpContents::Session::Timezones::Adjustment&
SdpContents::Session::Timezones::Adjustment::operator=(const Adjustment& rhs)
{
   if (this != &rhs)
   {
      time = rhs.time;
      offset = rhs.offset;
   }
   return *this;
}

SdpContents::Session::Timezones::Timezones()
   : mAdjustments()
{}

SdpContents::Session::Timezones::Timezones(const Timezones& rhs)
   : mAdjustments(rhs.mAdjustments)
{}

SdpContents::Session::Timezones&
SdpContents::Session::Timezones::operator=(const Timezones& rhs)
{
   if (this != &rhs)
   {
      mAdjustments = rhs.mAdjustments;
   }
   return *this;
}

EncodeStream&
SdpContents::Session::Timezones::encode(EncodeStream& s) const
{
   if (!mAdjustments.empty())
   {
      s << "z=";
      bool first = true;
      for (list<Adjustment>::const_iterator i = mAdjustments.begin();
           i != mAdjustments.end(); ++i)
      {
         if (!first)
         {
            s << Symbols::SPACE[0];
         }
         first = false;
         s << i->time << Symbols::SPACE[0]
           << i->offset << 's';
      }

      s << Symbols::CRLF;
   }
   return s;
}

void
SdpContents::Session::Timezones::parse(ParseBuffer& pb)
{
   pb.skipChar('z');
   pb.skipChar(Symbols::EQUALS[0]);

   while (!pb.eof() && *pb.position() != Symbols::CR[0])
   {
      Adjustment adj(0, 0);
      adj.time = pb.integer();
      pb.skipChar(Symbols::SPACE[0]);
      adj.offset = parseTypedTime(pb);
      addAdjustment(adj);

      if (!pb.eof() && *pb.position() == Symbols::SPACE[0])
      {
         pb.skipChar();
      }
   }

   skipEol(pb);
}

void
SdpContents::Session::Timezones::addAdjustment(const Adjustment& adjust)
{
   mAdjustments.push_back(adjust);
}

SdpContents::Session::Encryption::Encryption()
   : mMethod(NoEncryption),
     mKey(Data::Empty)
{}

SdpContents::Session::Encryption::Encryption(const KeyType& method,
                                             const Data& key)
   : mMethod(method),
     mKey(key)
{}

SdpContents::Session::Encryption::Encryption(const Encryption& rhs)
   : mMethod(rhs.mMethod),
     mKey(rhs.mKey)
{}

SdpContents::Session::Encryption&
SdpContents::Session::Encryption::operator=(const Encryption& rhs)
{
   if (this != &rhs)
   {
      mMethod = rhs.mMethod;
      mKey = rhs.mKey;
   }
   return *this;
}

const char* KeyTypes[] = {"????", "prompt", "clear", "base64", "uri"};

EncodeStream&
SdpContents::Session::Encryption::encode(EncodeStream& s) const
{
   s << "k="
     << KeyTypes[mMethod];
   if (mMethod != Prompt)
   {
      s << Symbols::COLON[0] << mKey;
   }
   s << Symbols::CRLF;

   return s;
}

void
SdpContents::Session::Encryption::parse(ParseBuffer& pb)
{
   pb.skipChar('k');
   const char* anchor = pb.skipChar(Symbols::EQUALS[0]);

   pb.skipToChar(Symbols::COLON[0]);
   if (!pb.eof())
   {
      Data p;
      pb.data(p, anchor);
      if (p == KeyTypes[Clear])
      {
         mMethod = Clear;
      }
      else if (p == KeyTypes[Base64])
      {
         mMethod = Base64;
      }
      else if (p == KeyTypes[UriKey])
      {
         mMethod = UriKey;
      }

      anchor = pb.skipChar(Symbols::COLON[0]);
      pb.skipToOneOf(Symbols::CRLF);
      pb.data(mKey, anchor);
   }
   else
   {
      pb.reset(anchor);
      pb.skipToOneOf(Symbols::CRLF);

      Data p;
      pb.data(p, anchor);
      if (p == KeyTypes[Prompt])
      {
         mMethod = Prompt;
      }
   }

   skipEol(pb);
}

SdpContents::Session::Session(int version,
                              const Origin& origin,
                              const Data& name)
   : mVersion(version),
     mOrigin(origin),
     mName(name)
{}

SdpContents::Session::Session(const Session& rhs)
{
   *this = rhs;
}

SdpContents::Session&
SdpContents::Session::operator=(const Session& rhs)
{
   if (this != &rhs)
   {
      mVersion = rhs.mVersion;
      mOrigin = rhs.mOrigin;
      mName = rhs.mName;
      mMedia = rhs.mMedia;
      mInformation = rhs.mInformation;
      mUri = rhs.mUri;
      mEmails = rhs.mEmails;
      mPhones = rhs.mPhones;
      mConnection = rhs.mConnection;
      mBandwidths = rhs.mBandwidths;
      mTimes = rhs.mTimes;
      mTimezones = rhs.mTimezones;
      mEncryption = rhs.mEncryption;
      mAttributeHelper = rhs.mAttributeHelper;

      for (MediumContainer::iterator i=mMedia.begin(); i != mMedia.end(); ++i)
      {
         i->setSession(this);
      }
   }
   return *this;
}

void
SdpContents::Session::parse(ParseBuffer& pb)
{
   pb.skipChar('v');
   pb.skipChar(Symbols::EQUALS[0]);
   mVersion = pb.integer();
   skipEol(pb);

   mOrigin.parse(pb);

   pb.skipChar('s');
   const char* anchor = pb.skipChar(Symbols::EQUALS[0]);
   pb.skipToOneOf(Symbols::CRLF);
   pb.data(mName, anchor);
   skipEol(pb);

   if (!pb.eof() && *pb.position() == 'i')
   {
      pb.skipChar('i');
      const char* anchor = pb.skipChar(Symbols::EQUALS[0]);
      pb.skipToOneOf(Symbols::CRLF);
      pb.data(mInformation, anchor);
      skipEol(pb);
   }

   if (!pb.eof() && *pb.position() == 'u')
   {
      pb.skipChar('u');
      pb.skipChar(Symbols::EQUALS[0]);
      mUri.parse(pb);
      skipEol(pb);
   }

   while (!pb.eof() && *pb.position() == 'e')
   {
      addEmail(Email());
      mEmails.back().parse(pb);
   }

   while (!pb.eof() && *pb.position() == 'p')
   {
      addPhone(Phone());
      mPhones.back().parse(pb);
   }

   if (!pb.eof() && *pb.position() == 'c')
   {
      mConnection.parse(pb);
   }

   while (!pb.eof() && *pb.position() == 'b')
   {
      addBandwidth(Bandwidth());
      mBandwidths.back().parse(pb);
   }

   while (!pb.eof() && *pb.position() == 't')
   {
      addTime(Time());
      mTimes.back().parse(pb);
   }

   if (!pb.eof() && *pb.position() == 'z')
   {
      mTimezones.parse(pb);
   }

   if (!pb.eof() && *pb.position() == 'k')
   {
      mEncryption.parse(pb);
   }

   mAttributeHelper.parse(pb);

   while (!pb.eof() && *pb.position() == 'm')
   {
      addMedium(Medium());
      mMedia.back().parse(pb);
   }
}

EncodeStream&
SdpContents::Session::encode(EncodeStream& s) const
{
   s << "v=" << mVersion << Symbols::CRLF;
   mOrigin.encode(s);
   s << "s=" << mName << Symbols::CRLF;

   if (!mInformation.empty())
   {
      s << "i=" << mInformation << Symbols::CRLF;
   }

   if (!mUri.host().empty())
   {
      s << "u=";
      mUri.encode(s);
      s << Symbols::CRLF;
   }

   for (list<Email>::const_iterator i = mEmails.begin();
        i != mEmails.end(); ++i)
   {
      i->encode(s);
   }

   for (list<Phone>::const_iterator i = mPhones.begin();
        i != mPhones.end(); ++i)
   {
      i->encode(s);
   }

   if (!mConnection.getAddress().empty())
   {
      mConnection.encode(s);
   }

   for (list<Bandwidth>::const_iterator i = mBandwidths.begin();
        i != mBandwidths.end(); ++i)
   {
      i->encode(s);
   }

   if (mTimes.empty())
   {
      s << "t=0 0" << Symbols::CRLF;
   }
   else
   {
      for (list<Time>::const_iterator i = mTimes.begin();
           i != mTimes.end(); ++i)
      {
         i->encode(s);
      }
   }

   mTimezones.encode(s);

   if (mEncryption.getMethod() != Encryption::NoEncryption)
   {
      mEncryption.encode(s);
   }

   mAttributeHelper.encode(s);

   for (MediumContainer::const_iterator i = mMedia.begin();
        i != mMedia.end(); ++i)
   {
      i->encode(s);
   }

   return s;
}

void
SdpContents::Session::addEmail(const Email& email)
{
   mEmails.push_back(email);
}

void
SdpContents::Session::addTime(const Time& t)
{
   mTimes.push_back(t);
}

void
SdpContents::Session::addPhone(const Phone& phone)
{
   mPhones.push_back(phone);
}

void
SdpContents::Session::addBandwidth(const Bandwidth& bandwidth)
{
   mBandwidths.push_back(bandwidth);
}

void
SdpContents::Session::addMedium(const Medium& medium)
{
   mMedia.push_back(medium);
   mMedia.back().setSession(this);
}

void
SdpContents::Session::addAttribute(const Data& key, const Data& value)
{
   mAttributeHelper.addAttribute(key, value);

   if (key == rtpmap)
   {
      for (MediumContainer::iterator i = mMedia.begin();
           i != mMedia.end(); ++i)
      {
         i->mRtpMapDone = false;
      }
   }
}

void
SdpContents::Session::clearAttribute(const Data& key)
{
   mAttributeHelper.clearAttribute(key);

   if (key == rtpmap)
   {
      for (MediumContainer::iterator i = mMedia.begin();
           i != mMedia.end(); ++i)
      {
         i->mRtpMapDone = false;
      }
   }
}

bool
SdpContents::Session::exists(const Data& key) const
{
   return mAttributeHelper.exists(key);
}

const list<Data>&
SdpContents::Session::getValues(const Data& key) const
{
   return mAttributeHelper.getValues(key);
}

SdpContents::Session::Medium::Medium(const Data& name,
                                     unsigned long port,
                                     unsigned long multicast,
                                     const Data& protocol)
   : mSession(0),
     mName(name),
     mPort(port),
     mMulticast(multicast),
     mProtocol(protocol),
     mRtpMapDone(false)
{}

SdpContents::Session::Medium::Medium()
   : mSession(0),
     mPort(0),
     mMulticast(1),
     mRtpMapDone(false)
{}

SdpContents::Session::Medium::Medium(const Medium& rhs)
   : mSession(0),
     mName(rhs.mName),
     mPort(rhs.mPort),
     mMulticast(rhs.mMulticast),
     mProtocol(rhs.mProtocol),
     mFormats(rhs.mFormats),
     mCodecs(rhs.mCodecs),
     mTransport(rhs.mTransport),
     mInformation(rhs.mInformation),
     mConnections(rhs.mConnections),
     mBandwidths(rhs.mBandwidths),
     mEncryption(rhs.mEncryption),
     mAttributeHelper(rhs.mAttributeHelper),
     mRtpMapDone(rhs.mRtpMapDone),
     mRtpMap(rhs.mRtpMap)
{
}


SdpContents::Session::Medium&
SdpContents::Session::Medium::operator=(const Medium& rhs)
{
   if (this != &rhs)
   {
      mSession = 0;
      mName = rhs.mName;
      mPort = rhs.mPort;
      mMulticast = rhs.mMulticast;
      mProtocol = rhs.mProtocol;
      mFormats = rhs.mFormats;
      mCodecs = rhs.mCodecs;
      mTransport = rhs.mTransport;
      mInformation = rhs.mInformation;
      mConnections = rhs.mConnections;
      mBandwidths = rhs.mBandwidths;
      mEncryption = rhs.mEncryption;
      mAttributeHelper = rhs.mAttributeHelper;
      mRtpMapDone = rhs.mRtpMapDone;
      mRtpMap = rhs.mRtpMap;
   }
   return *this;
}

void
SdpContents::Session::Medium::setPort(int port)
{
   mPort = port;
}

void
SdpContents::Session::Medium::setSession(Session* session)
{
   mSession = session;
}

void
SdpContents::Session::Medium::parse(ParseBuffer& pb)
{
   pb.skipChar('m');
   const char* anchor = pb.skipChar(Symbols::EQUALS[0]);

   pb.skipToChar(Symbols::SPACE[0]);
   pb.data(mName, anchor);
   pb.skipChar(Symbols::SPACE[0]);

   mPort = pb.integer();

   if (*pb.position() == Symbols::SLASH[0])
   {
      pb.skipChar();
      mMulticast = pb.integer();
   }

   anchor = pb.skipChar(Symbols::SPACE[0]);
   pb.skipToOneOf(Symbols::SPACE, Symbols::CRLF);
   pb.data(mProtocol, anchor);

   while (*pb.position() != Symbols::CR[0] &&
          *pb.position() != Symbols::LF[0])
   {
      anchor = pb.skipChar(Symbols::SPACE[0]);
      pb.skipToOneOf(Symbols::SPACE, Symbols::CRLF);
	  if(pb.position() != anchor)
	  {
		Data format;
		pb.data(format, anchor);
		addFormat(format);
	  }
   }

   skipEol(pb);

   if (!pb.eof() && *pb.position() == 'i')
   {
      pb.skipChar('i');
      anchor = pb.skipChar(Symbols::EQUALS[0]);
      pb.skipToOneOf(Symbols::CRLF);
      pb.data(mInformation, anchor);

      skipEol(pb);
   }

   while (!pb.eof() && *pb.position() == 'c')
   {
      addConnection(Connection());
      mConnections.back().parse(pb);
      if (!pb.eof() && *pb.position() == Symbols::SLASH[0])
      {
         // Note:  we only get here if there was a /<number of addresses> 
         //        parameter following the connection address. 
         pb.skipChar();
         int num = pb.integer();

         Connection& con = mConnections.back();
         const Data& addr = con.getAddress();
         int i = addr.size() - 1;
         for (; i; i--)
         {
            if (addr[i] == '.' || addr[i] == ':') // ipv4 or ipv6
            {
               break;
            }
         }

         if (addr[i] == '.')  // add a number of ipv4 connections
         {
            Data before(addr.data(), i+1);
            ParseBuffer subpb(addr.data()+i+1, addr.size()-i-1);
            int after = subpb.integer();

            for (int i = 1; i < num; i++)
            {
               addConnection(con);
               mConnections.back().mAddress = before + Data(after+i);
            }
         }
         if (addr[i] == ':') // add a number of ipv6 connections
         {
            Data before(addr.data(), i+1);
            int after = Helper::hex2integer(addr.data()+i+1);
            char hexstring[9];

            for (int i = 1; i < num; i++)
            {
               addConnection(con);
               memset(hexstring, 0, sizeof(hexstring));
               Helper::integer2hex(hexstring, after+i, false /* supress leading zeros */);
               mConnections.back().mAddress = before + Data(hexstring);
            }
         }

         skipEol(pb);
      }
   }

   while (!pb.eof() && *pb.position() == 'b')
   {
      addBandwidth(Bandwidth());
      mBandwidths.back().parse(pb);
   }

   if (!pb.eof() && *pb.position() == 'k')
   {
      mEncryption.parse(pb);
   }

   mAttributeHelper.parse(pb);
}

EncodeStream&
SdpContents::Session::Medium::encode(EncodeStream& s) const
{
   s << "m="
     << mName << Symbols::SPACE[0]
     << mPort;
   if (mMulticast > 1)
   {
      s << Symbols::SLASH[0] << mMulticast;
   }
   s << Symbols::SPACE[0]
     << mProtocol;

   for (list<Data>::const_iterator i = mFormats.begin();
        i != mFormats.end(); ++i)
   {
      s << Symbols::SPACE[0] << *i;
   }

   if (!mCodecs.empty())
   {
      for (CodecContainer::const_iterator i = mCodecs.begin();
           i != mCodecs.end(); ++i)
      {
         s << Symbols::SPACE[0] << i->payloadType();
      }
   }

   s << Symbols::CRLF;

   if (!mInformation.empty())
   {
      s << "i=" << mInformation << Symbols::CRLF;
   }

   for (list<Connection>::const_iterator i = mConnections.begin();
        i != mConnections.end(); ++i)
   {
      i->encode(s);
   }

   for (list<Bandwidth>::const_iterator i = mBandwidths.begin();
        i != mBandwidths.end(); ++i)
   {
      i->encode(s);
   }

   if (mEncryption.getMethod() != Encryption::NoEncryption)
   {
      mEncryption.encode(s);
   }

   if (!mCodecs.empty())
   {
      // add codecs to information and attributes
      for (CodecContainer::const_iterator i = mCodecs.begin();
           i != mCodecs.end(); ++i)
      {
          // If codec is static (defined in RFC 3551) we probably shouldn't
          // add attributes for it. But some UAs do include them.
          //Codec::CodecMap& staticCodecs = Codec::getStaticCodecs();
          //if (staticCodecs.find(i->payloadType()) != staticCodecs.end())
          //{
          //    continue;
          //}

         s << "a=rtpmap:"
           << i->payloadType() << Symbols::SPACE[0] << *i
           << Symbols::CRLF;
         if (!i->parameters().empty())
         {
            s << "a=fmtp:"
              << i->payloadType() << Symbols::SPACE[0] << i->parameters()
              << Symbols::CRLF;
         }
      }
   }

   mAttributeHelper.encode(s);

   return s;
}

void
SdpContents::Session::Medium::addFormat(const Data& format)
{
   mFormats.push_back(format);
}

void
SdpContents::Session::Medium::setConnection(const Connection& connection)
{
   mConnections.clear();
   addConnection(connection);
}

void
SdpContents::Session::Medium::addConnection(const Connection& connection)
{
   mConnections.push_back(connection);
}

void
SdpContents::Session::Medium::setBandwidth(const Bandwidth& bandwidth)
{
   mBandwidths.clear();
   addBandwidth(bandwidth);
}

void
SdpContents::Session::Medium::addBandwidth(const Bandwidth& bandwidth)
{
   mBandwidths.push_back(bandwidth);
}

void
SdpContents::Session::Medium::addAttribute(const Data& key, const Data& value)
{
   mAttributeHelper.addAttribute(key, value);
   if (key == rtpmap)
   {
      mRtpMapDone = false;
   }
}

const list<SdpContents::Session::Connection>
SdpContents::Session::Medium::getConnections() const
{
   list<Connection> connections = const_cast<Medium*>(this)->getMediumConnections();
   // If there are connections specified at the medium level, then check if a session level
   // connection is present - if so then return it
   if (connections.empty() && mSession && !mSession->connection().getAddress().empty())
   {
      connections.push_back(mSession->connection());
   }

   return connections;
}

bool
SdpContents::Session::Medium::exists(const Data& key) const
{
   if (mAttributeHelper.exists(key))
   {
      return true;
   }
   return mSession && mSession->exists(key);
}

const list<Data>&
SdpContents::Session::Medium::getValues(const Data& key) const
{
   if (mAttributeHelper.exists(key))
   {
      return mAttributeHelper.getValues(key);
   }
   if (!mSession)
   {
      assert(false);
      static list<Data> error;
      return error;
   }
   return mSession->getValues(key);
}

void
SdpContents::Session::Medium::clearAttribute(const Data& key)
{
   mAttributeHelper.clearAttribute(key);
   if (key == rtpmap)
   {
      mRtpMapDone = false;
   }
}

void
SdpContents::Session::Medium::clearCodecs()
{
   mFormats.clear();
   clearAttribute(rtpmap);
   clearAttribute(fmtp);
   mCodecs.clear();
}

void
SdpContents::Session::Medium::addCodec(const Codec& codec)
{
   codecs();
   mCodecs.push_back(codec);
}


const SdpContents::Session::Medium::CodecContainer&
SdpContents::Session::Medium::codecs() const
{
   return const_cast<Medium*>(this)->codecs();
}

SdpContents::Session::Medium::CodecContainer&
SdpContents::Session::Medium::codecs()
{
#if defined(WIN32) && defined(_MSC_VER) && (_MSC_VER < 1310)  // CJ TODO fix 
	assert(0);
#else 
   if (!mRtpMapDone)
   {
      // prevent recursion
      mRtpMapDone = true;

      if (exists(rtpmap))
      {
         for (list<Data>::const_iterator i = getValues(rtpmap).begin();
              i != getValues(rtpmap).end(); ++i)
         {
            //DebugLog(<< "SdpContents::Session::Medium::getCodec(" << *i << ")");
            ParseBuffer pb(i->data(), i->size());
            int format = pb.integer();
            // pass to codec constructor for parsing
            // pass this for other codec attributes
            try
            {
               mRtpMap[format].parse(pb, *this, format);
            }
            catch (ParseException& e)
            {
               ErrLog(<<"Caught exception: "<< e);
               mRtpMap.erase(format);
            }
         }
      }

      for (list<Data>::const_iterator i = mFormats.begin();
           i != mFormats.end(); ++i)
      {
         int mapKey = i->convertInt();
         RtpMap::const_iterator ri = mRtpMap.find(mapKey);
         if (ri != mRtpMap.end())
         {
            //DebugLog(<< "SdpContents::Session::Medium::getCodec[](" << ri->second << ")");
            mCodecs.push_back(ri->second);
         }
         else
         {
             // !kk! Is it a static format?
             Codec::CodecMap& staticCodecs = Codec::getStaticCodecs();
             Codec::CodecMap::const_iterator ri = staticCodecs.find(mapKey);
             if (ri != staticCodecs.end())
             {
                //DebugLog(<< "Found static codec for format: " << mapKey);
                Codec codec(ri->second);

                // Look for format parameters, and assign
                codec.assignFormatParameters(*this);

                mCodecs.push_back(codec);
             }
         }
      }

      // don't store twice
      mFormats.clear();
      mAttributeHelper.clearAttribute(rtpmap);
      mAttributeHelper.clearAttribute(fmtp);  // parsed out in codec.parse
   }
#endif

   return mCodecs;
}

const Codec& 
SdpContents::Session::Medium::findFirstMatchingCodecs(const CodecContainer& codecList, Codec* pMatchingCodec) const
{
   const CodecContainer& internalCodecList = codecs();
   static Codec emptyCodec;
   resip::SdpContents::Session::Medium::CodecContainer::const_iterator sIter;
   resip::SdpContents::Session::Medium::CodecContainer::const_iterator sEnd = internalCodecList.end();
   resip::SdpContents::Session::Medium::CodecContainer::const_iterator eIter;
   resip::SdpContents::Session::Medium::CodecContainer::const_iterator eEnd = codecList.end();
   for (eIter = codecList.begin(); eIter != eEnd ; ++eIter)
   {
      for (sIter = internalCodecList.begin(); sIter != sEnd; ++sIter)
      {
         if (*sIter == *eIter)
         {
			   if (pMatchingCodec) 
            {
               *pMatchingCodec = *eIter;
            }
            return *sIter;
         }
      }
   }
   return emptyCodec;
}

const Codec& 
SdpContents::Session::Medium::findFirstMatchingCodecs(const Medium& medium, Codec* pMatchingCodec) const
{
   if (&medium == this)
   {
      return codecs().front();
   }
   else
   {
      return findFirstMatchingCodecs(medium.codecs(), pMatchingCodec);
   }
}

int
SdpContents::Session::Medium::findTelephoneEventPayloadType() const
{
   const CodecContainer& codecList = codecs();
   for (CodecContainer::const_iterator i = codecList.begin(); i != codecList.end(); i++)
   {
      if (i->getName() == SdpContents::Session::Codec::TelephoneEvent.getName())
      {
         return i->payloadType();
      }
   }
   return -1;
}

Codec::Codec(const Data& name,
             unsigned long rate,
             const Data& parameters,
             const Data& encodingParameters)
   : mName(name),
     mRate(rate),
     mPayloadType(-1),
     mParameters(parameters),
     mEncodingParameters(encodingParameters)
{
}

Codec::Codec(const Codec& rhs)
   : mName(rhs.mName),
     mRate(rhs.mRate),
     mPayloadType(rhs.mPayloadType),
     mParameters(rhs.mParameters),
     mEncodingParameters(rhs.mEncodingParameters)
{
}

Codec::Codec(const Data& name, int payloadType, int rate)
   : mName(name),
     mRate(rate),
     mPayloadType(payloadType)
{
}

Codec&
Codec::operator=(const Codec& rhs)
{
   if (this != &rhs)
   {
      mName = rhs.mName;
      mRate = rhs.mRate;
      mPayloadType = rhs.mPayloadType;
      mParameters = rhs.mParameters;
      mEncodingParameters = rhs.mEncodingParameters;
   }
   return *this;
}

void
Codec::parse(ParseBuffer& pb,
             const SdpContents::Session::Medium& medium,
             int payloadType)
{
   const char* anchor = pb.skipWhitespace();
   pb.skipToChar(Symbols::SLASH[0]);
   pb.data(mName, anchor);
   if(!pb.eof())
   {
      pb.skipChar(Symbols::SLASH[0]);
      mRate = pb.integer();
      pb.skipToChar(Symbols::SLASH[0]);
   }
   if(!pb.eof() && *pb.position() == Symbols::SLASH[0])
   {
      anchor = pb.skipChar(Symbols::SLASH[0]);
      pb.skipToEnd();
      pb.data(mEncodingParameters, anchor);
   }
   mPayloadType = payloadType;

   assignFormatParameters(medium); 
}

void
Codec::assignFormatParameters(const SdpContents::Session::Medium& medium)
{
   // get parameters if they exist
   if (medium.exists(fmtp))
   {
      for (list<Data>::const_iterator i = medium.getValues(fmtp).begin();
           i != medium.getValues(fmtp).end(); ++i)
      {
         try
         {
            ParseBuffer pb(i->data(), i->size());
            int payload = pb.integer();
            if (payload == mPayloadType)
            {
               const char* anchor = pb.skipWhitespace();
               pb.skipToEnd();
               pb.data(mParameters, anchor);
               break;
            }
         }
         catch (ParseException &pe)
         {
            InfoLog(<<"Caught exception when parsing a=fmtp: "<< pe);
         }
      }
   }
}

const Data&
Codec::getName() const
{
   return mName;
}

int
Codec::getRate() const
{
   return mRate;
}

Codec::CodecMap& Codec::getStaticCodecs()
{
   if (! sStaticCodecsCreated)
   {
      //
      // Build map of static codecs as defined in RFC 3551
      //
      sStaticCodecs = std::auto_ptr<CodecMap>(new CodecMap);

      // Audio codecs
      sStaticCodecs->insert(make_pair(0,Codec("PCMU",0,8000)));
      sStaticCodecs->insert(make_pair(3,Codec("GSM",3,8000)));
      sStaticCodecs->insert(make_pair(4,Codec("G723",4,8000)));
      sStaticCodecs->insert(make_pair(5,Codec("DVI4",5,8000)));
      sStaticCodecs->insert(make_pair(6,Codec("DVI4",6,16000)));
      sStaticCodecs->insert(make_pair(7,Codec("LPC",7,8000)));
      sStaticCodecs->insert(make_pair(8,Codec("PCMA",8,8000)));
      sStaticCodecs->insert(make_pair(9,Codec("G722",9,8000)));
      sStaticCodecs->insert(make_pair(10,Codec("L16-2",10,44100)));
      sStaticCodecs->insert(make_pair(11,Codec("L16-1",11,44100)));
      sStaticCodecs->insert(make_pair(12,Codec("QCELP",12,8000)));
      sStaticCodecs->insert(make_pair(13,Codec("CN",13,8000)));
      sStaticCodecs->insert(make_pair(14,Codec("MPA",14,90000)));
      sStaticCodecs->insert(make_pair(15,Codec("G728",15,8000)));
      sStaticCodecs->insert(make_pair(16,Codec("DVI4",16,11025)));
      sStaticCodecs->insert(make_pair(17,Codec("DVI4",17,22050)));
      sStaticCodecs->insert(make_pair(18,Codec("G729",18,8000)));

      // Video or audio/video codecs
      sStaticCodecs->insert(make_pair(25,Codec("CelB",25,90000)));
      sStaticCodecs->insert(make_pair(26,Codec("JPEG",26,90000)));
      sStaticCodecs->insert(make_pair(28,Codec("nv",28,90000)));
      sStaticCodecs->insert(make_pair(31,Codec("H261",31,90000)));
      sStaticCodecs->insert(make_pair(32,Codec("MPV",32,90000)));
      sStaticCodecs->insert(make_pair(33,Codec("MP2T",33,90000)));
      sStaticCodecs->insert(make_pair(34,Codec("H263",34,90000)));

      sStaticCodecsCreated = true;
   }
   return *(sStaticCodecs.get());
}

bool
resip::operator==(const Codec& lhs, const Codec& rhs)
{
   static Data defaultEncodingParameters(Data("1"));  // Default for audio streams (1-Channel)
   return (isEqualNoCase(lhs.mName, rhs.mName) && lhs.mRate == rhs.mRate && 
           (lhs.mEncodingParameters == rhs.mEncodingParameters ||
            (lhs.mEncodingParameters.empty() && rhs.mEncodingParameters == defaultEncodingParameters) ||
            (lhs.mEncodingParameters == defaultEncodingParameters && rhs.mEncodingParameters.empty())));
}

EncodeStream&
resip::operator<<(EncodeStream& str, const Codec& codec)
{
   str << codec.mName;
   str << Symbols::SLASH[0];
   str << codec.mRate;
   if(!codec.mEncodingParameters.empty())
   {
      str << Symbols::SLASH[0];
      str << codec.mEncodingParameters;
   }
   return str;
}

const Codec Codec::ULaw_8000("PCMU", 0, 8000);
const Codec Codec::ALaw_8000("PCMA", 8, 8000);
const Codec Codec::G729_8000("G729", 18, 8000);
const Codec Codec::G723_8000("G723", 4, 8000);
const Codec Codec::GSM_8000("GSM", 3, 8000);

const Codec Codec::TelephoneEvent("telephone-event", 101, 8000);
const Codec Codec::FrfDialedDigit("frf-dialed-event",102, 8000);
const Codec Codec::CN("CN", 13, 8000);

bool Codec::sStaticCodecsCreated = false;
std::auto_ptr<Codec::CodecMap> Codec::sStaticCodecs;

/* ====================================================================
 * The Vovida Software License, Version 1.0
 *
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * ====================================================================
 *
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
