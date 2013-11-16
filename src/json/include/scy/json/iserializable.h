//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


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
