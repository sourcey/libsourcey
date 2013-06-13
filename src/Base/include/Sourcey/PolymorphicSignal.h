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


#include "Sourcey/Types.h"
#include "Sourcey/Signal.h"


namespace Scy {
	

typedef DelegateBase<IPolymorphic&> PolymorphicDelegateBase;
typedef SignalBase<PolymorphicDelegateBase, IPolymorphic&> PolymorphicSignal;


template<class C, typename PolymorphicT>
struct DelegateCallback<C, 8, false, PolymorphicT> 
{
	typedef void (C::*Method)(PolymorphicT&); 
	DefineCallbackFields
	virtual void emit(Void /* sender */, IPolymorphic& data, Void, Void, Void) const {
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
	virtual void emit(Void sender, IPolymorphic& data, Void, Void, Void) const {
		PolymorphicT* castData = dynamic_cast<PolymorphicT*>(&data);
		if (castData) {
			(_object->*_method)(sender, *castData);
		}
	}
};


// -------------------------------------------------------------------
//
#define DefinePolymorphicDelegate(Name, IPolymorphicBaseT, DelegateBaseT)\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, false, PolymorphicT>,					\
		IPolymorphicBaseT&												\
	> Name(C* pObj, void (C::*Method)(PolymorphicT&), int priority = 0) \
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, false, PolymorphicT>,				\
			IPolymorphicBaseT&											\
		>(pObj, Method, priority);										\
	}																	\
																		\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, true, PolymorphicT>,						\
		IPolymorphicBaseT&												\
	> Name(C* pObj, void (C::*Method)(Void, PolymorphicT&),				\
		int priority = 0)												\
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, true, PolymorphicT>,					\
			IPolymorphicBaseT&											\
		>(pObj, Method, priority);										\
	}																	\


#define DefinePolymorphicDelegateWithArg(Name, IPolymorphicBaseT,		\
	DelegateBaseT, ArgType, ArgDefault)									\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, false, PolymorphicT>,					\
		IPolymorphicBaseT&												\
	> Name(C* pObj, void (C::*Method)(PolymorphicT&),					\
		ArgType data = ArgDefault, int priority = 0)					\
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, false, PolymorphicT>,				\
			IPolymorphicBaseT&											\
		>(pObj, Method, data, priority);								\
	}																	\
																		\
																		\
	template <class C, typename PolymorphicT>							\
	static Delegate<C,													\
		DelegateBaseT,													\
		DelegateCallback<C, 8, true, PolymorphicT>, IPolymorphicBaseT&	\
	> Name(C* pObj, void (C::*Method)(Void, PolymorphicT&),				\
		ArgType data = ArgDefault, int priority = 0)					\
	{																	\
		return Delegate<C,												\
			DelegateBaseT,												\
			DelegateCallback<C, 8, true, PolymorphicT>,					\
			IPolymorphicBaseT&											\
		>(pObj, Method, data, priority);								\
	}																	\


DefinePolymorphicDelegate(PolymorphicDelegate, IPolymorphic, PolymorphicDelegateBase)


} // namespace Scy


#endif // SOURCEY_PolymorphicSignal_H