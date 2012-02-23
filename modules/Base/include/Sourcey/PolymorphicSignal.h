//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_PolymorphicSignal_H
#define SOURCEY_PolymorphicSignal_H


#include "Sourcey/Base.h"
#include "Sourcey/Signal.h"


namespace Sourcey {
	

typedef DelegateBase<IPolymorphic&> PolymorphicDelegateBase;
typedef SignalBase<PolymorphicDelegateBase, IPolymorphic&> PolymorphicSignal;


template<class C, typename PolymorphicT>
struct DelegateCallback<C, 8, false, PolymorphicT> 
{
	typedef void (C::*Method)(PolymorphicT&); 
	DefineCallbackFields
	virtual void dispatch(Void sender, IPolymorphic& data, Void, Void, Void) const {
		PolymorphicT* castData = dynamic_cast<PolymorphicT*>(&data);
		if (castData) {
			(_object->*_method)(*castData);
		}
	}
}; 


template<class C, typename PolymorphicT>
struct DelegateCallback<C, 8, true, PolymorphicT> 
{
	typedef void (C::*Method)(Void, PolymorphicT&);
	DefineCallbackFields
	virtual void dispatch(Void sender, IPolymorphic& data, Void, Void, Void) const {
		PolymorphicT* castData = dynamic_cast<PolymorphicT*>(&data);
		if (castData) {
			(_object->*_method)(sender, *castData);
		}
	}
};


// -------------------------------------------------------------------
//
#define DefinePolymorphicDelegate(Name, PolymorphicBaseT, DelegateBaseT)\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, false, PolymorphicT>,					\
		PolymorphicBaseT&												\
	> Name(C* pObj, void (C::*Method)(PolymorphicT&), int priority = 0) \
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, false, PolymorphicT>,				\
			PolymorphicBaseT&											\
		>(pObj, Method, priority);										\
	}																	\
																		\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, true, PolymorphicT>,						\
		PolymorphicBaseT&												\
	> Name(C* pObj, void (C::*Method)(Void, PolymorphicT&),				\
		int priority = 0)												\
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, true, PolymorphicT>,					\
			PolymorphicBaseT&											\
		>(pObj, Method, priority);										\
	}																	\


#define DefinePolymorphicDelegateWithArg(Name, PolymorphicBaseT,		\
	DelegateBaseT, ArgType, ArgDefault)									\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, false, PolymorphicT>,					\
		PolymorphicBaseT&												\
	> Name(C* pObj, void (C::*Method)(PolymorphicT&),					\
		ArgType data = ArgDefault, int priority = 0)					\
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, false, PolymorphicT>,				\
			PolymorphicBaseT&											\
		>(pObj, Method, data, priority);								\
	}																	\
																		\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, true, PolymorphicT>, PolymorphicBaseT&	\
	> Name(C* pObj, void (C::*Method)(Void, PolymorphicT&),				\
		ArgType data = ArgDefault, int priority = 0)					\
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, true, PolymorphicT>,					\
			PolymorphicBaseT&											\
		>(pObj, Method, data, priority);								\
	}																	\


DefinePolymorphicDelegate(polymorphicDelegate, IPolymorphic, PolymorphicDelegateBase)


} // namespace Sourcey


#endif // SOURCEY_PolymorphicSignal_H