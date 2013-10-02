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


#ifndef SOURCEY_PolymorphicSignal_H
#define SOURCEY_PolymorphicSignal_H


#include "Sourcey/Interface.h"
#include "Sourcey/Signal.h"


namespace scy {
	

typedef DelegateBase<basic::Polymorphic&> PolymorphicDelegateBase;
typedef SignalBase<PolymorphicDelegateBase, basic::Polymorphic&> PolymorphicSignal;


template<class C, typename PolymorphicT>
struct DelegateCallback<C, 8, false, PolymorphicT> 
{
	typedef void (C::*Method)(PolymorphicT&); 
	DefineCallbackFields
	virtual void emit(void* /* sender */, basic::Polymorphic& data, void*, void*, void*) const {
		PolymorphicT* castData = dynamic_cast<PolymorphicT*>(&data);
		if (castData) {
			(_object->*_method)(*castData);
		}
	}
}; 


template<class C, typename PolymorphicT>
struct DelegateCallback<C, 8, true, PolymorphicT> 
{
	typedef void (C::*Method)(void*, PolymorphicT&);
	DefineCallbackFields
	virtual void emit(void* sender, basic::Polymorphic& data, void*, void*, void*) const {
		PolymorphicT* castData = dynamic_cast<PolymorphicT*>(&data);
		if (castData) {
			(_object->*_method)(sender, *castData);
		}
	}
};


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
	> Name(C* pObj, void (C::*Method)(void*, PolymorphicT&),			\
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
	> Name(C* pObj, void (C::*Method)(void*, PolymorphicT&),			\
		ArgType data = ArgDefault, int priority = 0)					\
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, true, PolymorphicT>,					\
			PolymorphicBaseT&											\
		>(pObj, Method, data, priority);								\
	}																	\


DefinePolymorphicDelegate(polymorphicDelegate, basic::Polymorphic, PolymorphicDelegateBase)


} // namespace scy


#endif // SOURCEY_PolymorphicSignal_H