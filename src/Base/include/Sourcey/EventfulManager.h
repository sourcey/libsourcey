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


#ifndef SOURCEY_EventfulManager_H
#define SOURCEY_EventfulManager_H


#include "Sourcey/BasicManager.h"
#include "Sourcey/Signal.h"


namespace scy { 

	
template <class TKey, class TValue>
class EventfulManager: public BasicManager<TKey, TValue>
{	
public:
	typedef BasicManager<TKey, TValue> Base;

public:	
	virtual void onAdd(const TKey&, TValue* item) 
	{
		ItemAdded.emit(this, *item);
	}

	virtual void onRemove(const TKey&, TValue* item) 
	{ 
		ItemRemoved.emit(this, *item); 
	}

	Signal<TValue&>			ItemAdded;
	Signal<const TValue&>	ItemRemoved;	
};



} // namespace scy


#endif // SOURCEY_EventfulManager_H