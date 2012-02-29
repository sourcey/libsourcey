//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_JSON_ISerializable_h
#define SOURCEY_JSON_ISerializable_h


#include "Sourcey/JSON/JSON.h"
#include <ostream>


namespace Sourcey {
namespace JSON {


class ISerializable
{
public:
   virtual ~ISerializable() {};
   virtual void serialize(JSON::Value& root) = 0;
   virtual void deserialize(JSON::Value& root) = 0;
};


inline bool serialize(ISerializable* pObj, std::string& output)
{
   if (pObj == NULL)
      return false;
 
   JSON::Value serializeRoot;
   pObj->serialize(serializeRoot);
 
   JSON::StyledWriter writer;
   output = writer.write(serializeRoot);
 
   return true;
}


inline bool deserialize(ISerializable* pObj, std::string& input)
{
   if (pObj == NULL)
      return false;
 
   JSON::Value deserializeRoot;
   JSON::Reader reader;
 
   if (!reader.parse(input, deserializeRoot))
      return false;
 
   pObj->deserialize(deserializeRoot);
 
   return true;
}


} } // namespace Sourcey::JSON


#endif // SOURCEY_JSON_ISerializable.h
