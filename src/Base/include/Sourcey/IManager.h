//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#ifndef SOURCEY_IManager_H
#define SOURCEY_IManager_H


namespace Scy { 


template <class TKey, class TValue>
class IManager
	/// Abstract interface for managing
	/// an indexed map of pointers.
{	
public:
	IManager() {};
	virtual ~IManager() {}

	virtual bool add(const TKey& key, TValue* item, bool whiny = true) = 0;
	virtual bool remove(const TValue* item) = 0;
	virtual TValue* remove(const TKey& key) = 0;
	virtual bool exists(const TKey& key) const = 0;
	virtual bool exists(const TValue* item) const = 0;
	virtual bool free(const TKey& key) = 0;
	virtual bool empty() const = 0;
	virtual int size() const = 0;
	virtual TValue* get(const TKey& key, bool whiny = true) const = 0;
	virtual void clear() = 0;
};


} // namespace Scy


#endif // SOURCEY_IManager_H