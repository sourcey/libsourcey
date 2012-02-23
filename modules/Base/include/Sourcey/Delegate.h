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


#ifndef SOURCEY_Delegate_H
#define SOURCEY_Delegate_H


#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"


namespace Sourcey {
	

#define Void void*
#define DelegateDefaultParams typename P = Void, typename P2 = Void, typename P3 = Void, typename P4 = Void
#define DefineCallbackFields										\
																	\
	DelegateCallback(C* object, Method method) :					\
		_object(object), 											\
		_method(method) {}											\
																	\
	DelegateCallback(const DelegateCallback& r) : 					\
		_object(r._object), 										\
		_method(r._method) {}										\
																	\
	C*		_object;												\
	Method	_method;												\

	
#define DelegateVirtualFields(Class)								\
																	\
	virtual Class* clone() const = 0;								\
	virtual Void object() const = 0;								\
	virtual void cancel() = 0;										\
	virtual bool cancelled() const = 0;								\
	virtual int priority() const = 0;								\
	virtual bool equals(const Class*) const = 0;					\
	virtual void dispatch(Void, P, P2, P3, P4) const = 0;			\
	virtual bool accepts(Void, P, P2, P3, P4) { return true; }		\
	static bool ComparePrioroty(const Class* l, const Class* r) {	\
		return l->priority() > r->priority();						\
	}																\


// -------------------------------------------------------------------
//
template<class C, int N, bool withSender = true, DelegateDefaultParams> 
struct DelegateCallback 
{
};


template<class C>
struct DelegateCallback<C, 0, true>
{
	typedef void (C::*Method)(Void);	
	virtual void dispatch(Void sender, Void, Void, Void, Void) const {
		(_object->*_method)(sender);
	}

	DefineCallbackFields
};


template<class C>
struct DelegateCallback<C, 0, false>
{
	typedef void (C::*Method)();	
	virtual void dispatch(Void sender, Void, Void, Void, Void) const {
		(_object->*_method)();
	}

	DefineCallbackFields
};


template<class C, typename P>
struct DelegateCallback<C, 1, true, P> 
{
	typedef void (C::*Method)(Void, P);	
	virtual void dispatch(Void sender, P arg, Void, Void, Void) const {
		(_object->*_method)(sender, arg);
	}

	DefineCallbackFields
}; 


template<class C, typename P> 
struct DelegateCallback<C, 1, false, P> 
{
	typedef void (C::*Method)(P);	
	virtual void dispatch(Void, P arg, Void, Void, Void) const {
		(_object->*_method)(arg);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2> 
struct DelegateCallback<C, 2, true, P, P2> 
{
	typedef void (C::*Method)(Void, P, P2);	
	virtual void dispatch(Void sender, P arg, P2 arg2, Void, Void) const {
		(_object->*_method)(sender, arg, arg2);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2>
struct DelegateCallback<C, 2, false, P, P2> 
{
	typedef void (C::*Method)(P, P2);	
	virtual void dispatch(Void, P arg, P2 arg2, Void, Void) const {
		(_object->*_method)(arg, arg2);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2, typename P3>
struct DelegateCallback<C, 3, true, P, P2, P3> 
{
	typedef void (C::*Method)(Void, P, P2, P3);	
	virtual void dispatch(Void sender, P arg, P2 arg2, P3 arg3, Void) const {
		(_object->*_method)(sender, arg, arg2, arg3);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2, typename P3> //, typename P4
struct DelegateCallback<C, 3, false, P, P2, P3> 
{
	typedef void (C::*Method)(P, P2, P3);	
	virtual void dispatch(Void, P arg, P2 arg2, P3 arg3, Void) const {
		(_object->*_method)(arg, arg2, arg3);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2, typename P3, typename P4> 
struct DelegateCallback<C, 4, true, P, P2, P3, P4> 
{
	typedef void (C::*Method)(Void, P, P2, P3, P4);	
	virtual void dispatch(Void sender, P arg, P2 arg2, P3 arg3, P4 arg4) const {
		(_object->*_method)(sender, arg, arg2, arg3, arg4);
	}

	DefineCallbackFields
};


template<class C, typename P, typename P2, typename P3, typename P4> 
struct DelegateCallback<C, 4, false, P, P2, P3, P4> 
{
	typedef void (C::*Method)(P, P2, P3, P4);	
	virtual void dispatch(Void, P arg, P2 arg2, P3 arg3, P4 arg4) const {
		(_object->*_method)(arg, arg2, arg3, arg4);
	}

	DefineCallbackFields
};


// -------------------------------------------------------------------
//
template <DelegateDefaultParams>
struct DelegateBase
	/// The abstract class for all instantiations of the Delegate
	/// template classes.
{
	typedef typename Void DataT;
	Void data;

	DelegateBase(DataT data = 0) : data(data) {};
	DelegateBase(const DelegateBase& r) : data(r.data) {};
	DelegateVirtualFields(DelegateBase)

	//virtual bool accepts(Void, P, P2, P3, P4) const { return true; };
};


// -------------------------------------------------------------------
//
template <class C, class BaseT, class CallbackT, DelegateDefaultParams>
class Delegate: public BaseT, public CallbackT
	/// This template class implements an adapter that sits between
	/// an DelegateBase and an object receiving notifications from it.
{
public:
	typedef typename DelegateBase<P, P2, P3, P4> DerivedT;
	typedef typename CallbackT::Method Method;
	typedef typename BaseT::DataT DataT;

	Delegate(C* object, Method method, int priority = 0) : 
		CallbackT(object, method),
		_priority(priority), 
		_cancelled(false) {}

	Delegate(C* object, Method method, DataT filter, int priority = 0) :
		BaseT(filter), CallbackT(object, method), 
		_priority(priority), 
		_cancelled(false) {} 

	Delegate(const Delegate& r) : 
		BaseT(r), CallbackT(r), 
		_priority(r._priority), 
		_cancelled(r._cancelled) {}
	
	virtual BaseT* clone() const {
		return new Delegate(*this);
	}
	
	virtual void dispatch(Void sender, P arg, P2 arg2, P3 arg3, P4 arg4) const {
		if (!_cancelled)
			CallbackT::dispatch(sender, arg, arg2, arg3, arg4);
	}
	
	virtual bool equals(const DerivedT* r) const { 
		const Delegate* delegate = dynamic_cast<const Delegate*>(r);
		return delegate && 
			   delegate->_object == _object && 
			   delegate->_method == _method;
	}	

	virtual void cancel() { _cancelled = true; };
	virtual bool cancelled() const { return _cancelled; };
	virtual int priority() const { return _priority; };
	virtual Void object() const { return _object; };	

protected:
	Delegate();

	int		_priority;
	bool	_cancelled;
};


// -------------------------------------------------------------------
//
template <class C>
static Delegate<C, 
	DelegateBase<>, 
	DelegateCallback<C, 0, true>
> delegate(C* pObj, void (C::*Method)(Void), int priority = 0) 
{
	return Delegate<C,
		DelegateBase<>,
		DelegateCallback<C, 0, true>
	>(pObj, Method, priority);
}


template <class C, typename P>
static Delegate<C, 
	DelegateBase<P>, 
	DelegateCallback<C, 1, true, P>, P
> delegate(C* pObj, void (C::*Method)(Void,P), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P>, 
		DelegateCallback<C, 1, true, P>, P
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2>
static Delegate<C, 
	DelegateBase<P, P2>,
	DelegateCallback<C, 2, true, P, P2>, P, P2
> delegate(C* pObj, void (C::*Method)(Void, P, P2), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2>, 
		DelegateCallback<C, 2, true, P, P2>, P, P2
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2, typename P3>
static Delegate<C, 
	DelegateBase<P, P2, P3>, 
	DelegateCallback<C, 3, true, P, P2, P3>, P, P2, P3
> delegate(C* pObj, void (C::*Method)(Void, P, P2, P3), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2, P3>,
		DelegateCallback<C, 3, true, P, P2, P3>, P, P2, P3
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2, typename P3, typename P4>
static Delegate<C, 
	DelegateBase<P, P2, P3, P4>, 
	DelegateCallback<C, 4, true, P, P2, P3, P4>, P, P2, P3, P4
> delegate(C* pObj, void (C::*Method)(Void, P, P2, P3, P4), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2, P3, P4>, 
		DelegateCallback<C, 4, true, P, P2, P3, P4>, P, P2, P3, P4
	>(pObj, Method, priority);
}


} // namespace Sourcey


#endif // SOURCEY_Delegate_H