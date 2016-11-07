///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup json
/// @{


#ifndef SCY_JSON_ISerializable_h
#define SCY_JSON_ISerializable_h


#include "scy/json/json.h"
#include <ostream>


namespace scy {
namespace json {


class ISerializable
{
public:
   virtual ~ISerializable() {};
   virtual void serialize(json::Value& root) = 0;
   virtual void deserialize(json::Value& root) = 0;
};


inline bool serialize(ISerializable* pObj, std::string& output)
{
   if (pObj == NULL)
      return false;

   json::Value serializeRoot;
   pObj->serialize(serializeRoot);

   json::StyledWriter writer;
   output = writer.write(serializeRoot);

   return true;
}


inline bool deserialize(ISerializable* pObj, std::string& input)
{
   if (pObj == NULL)
      return false;

   json::Value deserializeRoot;
   json::Reader reader;

   if (!reader.parse(input, deserializeRoot))
      return false;

   pObj->deserialize(deserializeRoot);

   return true;
}


} } // namespace scy::json


#endif // SCY_JSON_ISerializable.h
