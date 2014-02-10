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


#ifndef SCY_Delegate_H
#define SCY_Delegate_H


#include "scy/types.h"
#include "scy/logger.h"


namespace scy {
	

#define DelegateDefaultArgs typename P = void*, typename P2 = void*, typename P3 = void*, typename P4 = void*
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
	virtual void* object() const = 0;								\
	virtual void cancel() = 0;										\
	virtual bool cancelled() const = 0;								\
	virtual int priority() const = 0;								\
	virtual bool equals(const Class*) const = 0;					\
	virtual void emit(void*, P, P2, P3, P4) /* const */ = 0;	    \
	virtual bool accepts(void*, P, P2, P3, P4) { return true; }		\
	static bool ComparePrioroty(const Class* l, const Class* r) {	\
		return l->priority() > r->priority();						\
	}																\


//
// Delegate Callback Functions
//


template<class C, int N, bool withSender = true, DelegateDefaultArgs> 
struct DelegateCallback 
{
};


template<class C>
struct DelegateCallback<C, 0, true>
{
	typedef void (C::*Method)(void*);	
	virtual void emit(void* sender, void*, void*, void*, void*) const {
		(_object->*_method)(sender);
	}

	DefineCallbackFields
};


template<class C>
struct DelegateCallback<C, 0, false>
{
	typedef void (C::*Method)();	
	virtual void emit(void* sender, void*, void*, void*, void*) const {
		(_object->*_method)();
	}

	DefineCallbackFields
};


template<class C, typename P>
struct DelegateCallback<C, 1, true, P> 
{
	typedef void (C::*Method)(void*, P);	
	virtual void emit(void* sender, P arg, void*, void*, void*) const {
		(_object->*_method)(sender, arg);
	}

	DefineCallbackFields
}; 


template<class C, typename P> 
struct DelegateCallback<C, 1, false, P> 
{
	typedef void (C::*Method)(P);	
	virtual void emit(void*, P arg, void*, void*, void*) const 
	{
		(_object->*_method)(arg);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2> 
struct DelegateCallback<C, 2, true, P, P2> 
{
	typedef void (C::*Method)(void*, P, P2);	
	virtual void emit(void* sender, P arg, P2 arg2, void*, void*) const 
	{
		(_object->*_method)(sender, arg, arg2);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2>
struct DelegateCallback<C, 2, false, P, P2> 
{
	typedef void (C::*Method)(P, P2);	
	virtual void emit(void*, P arg, P2 arg2, void*, void*) const 
	{
		(_object->*_method)(arg, arg2);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2, typename P3>
struct DelegateCallback<C, 3, true, P, P2, P3> 
{
	typedef void (C::*Method)(void*, P, P2, P3);	
	virtual void emit(void* sender, P arg, P2 arg2, P3 arg3, void*) const
	{
		(_object->*_method)(sender, arg, arg2, arg3);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2, typename P3>
struct DelegateCallback<C, 3, false, P, P2, P3> 
{
	typedef void (C::*Method)(P, P2, P3);	
	virtual void emit(void*, P arg, P2 arg2, P3 arg3, void*) const 
	{
		(_object->*_method)(arg, arg2, arg3);
	}

	DefineCallbackFields
}; 


template<class C, typename P, typename P2, typename P3, typename P4> 
struct DelegateCallback<C, 4, true, P, P2, P3, P4> 
{
	typedef void (C::*Method)(void*, P, P2, P3, P4);	
	virtual void emit(void* sender, P arg, P2 arg2, P3 arg3, P4 arg4) const 
	{
		(_object->*_method)(sender, arg, arg2, arg3, arg4);
	}

	DefineCallbackFields
};


template<class C, typename P, typename P2, typename P3, typename P4> 
struct DelegateCallback<C, 4, false, P, P2, P3, P4> 
{
	typedef void (C::*Method)(P, P2, P3, P4);	
	virtual void emit(void*, P arg, P2 arg2, P3 arg3, P4 arg4) const 
	{
		(_object->*_method)(arg, arg2, arg3, arg4);
	}

	DefineCallbackFields
};


//
// Delegate Virtual Base
//


template <DelegateDefaultArgs>
struct DelegateBase
	// The abstract base for all instantiations of the
	// Delegate template classes.
{
	typedef void* DataT;
	void* data;

	DelegateBase(DataT data = 0) : data(data) {};
	DelegateBase(const DelegateBase& r) : data(r.data) {};
	virtual ~DelegateBase() {};

	DelegateVirtualFields(DelegateBase)

	//virtual bool accepts(void*, P, P2, P3, P4) const { return true; };
};


//
// Delegate Implementation
//


template <class C, class BaseT, class CallbackT, DelegateDefaultArgs>
class Delegate: public BaseT, public CallbackT
	// This template class implements an adapter that sits between
	// an DelegateBase and an object receiving notifications from it.
{
public:
	typedef DelegateBase<P, P2, P3, P4> DerivedT;
	typedef typename CallbackT::Method Method;
	typedef typename BaseT::DataT DataT;

	Delegate(C* object, Method method, int priority = 0) : 
		CallbackT(object, method),
		_priority(priority), 
		_cancelled(false) 
	{
	}

	Delegate(C* object, Method method, DataT filter, int priority = 0) :
		BaseT(filter), CallbackT(object, method), 
		_priority(priority), 
		_cancelled(false) 
	{
	} 

	Delegate(const Delegate& r) : 
		BaseT(r), CallbackT(r), 
		_priority(r._priority), 
		_cancelled(r._cancelled) 
	{
	}	

	virtual ~Delegate() 
	{ 
	}
	
	BaseT* clone() const 
	{
		return new Delegate(*this);
	}
	
	void emit(void* sender, P arg, P2 arg2, P3 arg3, P4 arg4) /* const */ 
	{
		if (!cancelled())
			CallbackT::emit(sender, arg, arg2, arg3, arg4);
	}
	
	bool equals(const DerivedT* r) const 
	{ 
		const Delegate* delegate = dynamic_cast<const Delegate*>(r);
		return delegate && 
			   delegate->_object == CallbackT::_object && 
			   delegate->_method == CallbackT::_method;
	}	

	void cancel() { _cancelled = true; };
	bool cancelled() const { return _cancelled; };
	int priority() const { return _priority; };
	void* object() const { return CallbackT::_object; };	

protected:
	Delegate();

	int		_priority;
	bool	_cancelled;
};


//
// Delegate Specializations
//


template <class C>
static Delegate<C, 
	DelegateBase<>, 
	DelegateCallback<C, 0, true>
> sdelegate(C* pObj, void (C::*Method)(void*), int priority = 0) 
{
	return Delegate<C,
		DelegateBase<>,
		DelegateCallback<C, 0, true>
	>(pObj, Method, priority);
}


template <class C>
static Delegate<C, 
	DelegateBase<>, 
	DelegateCallback<C, 0, false>
> delegate(C* pObj, void (C::*Method)(), int priority = 0) 
{
	return Delegate<C,
		DelegateBase<>,
		DelegateCallback<C, 0, false>
	>(pObj, Method, priority);
}


template <class C, typename P>
static Delegate<C, 
	DelegateBase<P>, 
	DelegateCallback<C, 1, true, P>, P
> sdelegate(C* pObj, void (C::*Method)(void*,P), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P>, 
		DelegateCallback<C, 1, true, P>, P
	>(pObj, Method, priority);
}


template <class C, typename P>
static Delegate<C, 
	DelegateBase<P>, 
	DelegateCallback<C, 1, false, P>, P
> delegate(C* pObj, void (C::*Method)(P), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P>, 
		DelegateCallback<C, 1, false, P>, P
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2>
static Delegate<C, 
	DelegateBase<P, P2>,
	DelegateCallback<C, 2, true, P, P2>, P, P2
> sdelegate(C* pObj, void (C::*Method)(void*, P, P2), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2>, 
		DelegateCallback<C, 2, true, P, P2>, P, P2
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2>
static Delegate<C, 
	DelegateBase<P, P2>,
	DelegateCallback<C, 2, false, P, P2>, P, P2
> delegate(C* pObj, void (C::*Method)(P, P2), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2>, 
		DelegateCallback<C, 2, false, P, P2>, P, P2
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2, typename P3>
static Delegate<C, 
	DelegateBase<P, P2, P3>, 
	DelegateCallback<C, 3, true, P, P2, P3>, P, P2, P3
> sdelegate(C* pObj, void (C::*Method)(void*, P, P2, P3), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2, P3>,
		DelegateCallback<C, 3, true, P, P2, P3>, P, P2, P3
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2, typename P3>
static Delegate<C, 
	DelegateBase<P, P2, P3>, 
	DelegateCallback<C, 3, false, P, P2, P3>, P, P2, P3
> delegate(C* pObj, void (C::*Method)(P, P2, P3), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2, P3>,
		DelegateCallback<C, 3, false, P, P2, P3>, P, P2, P3
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2, typename P3, typename P4>
static Delegate<C, 
	DelegateBase<P, P2, P3, P4>, 
	DelegateCallback<C, 4, true, P, P2, P3, P4>, P, P2, P3, P4
> sdelegate(C* pObj, void (C::*Method)(void*, P, P2, P3, P4), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2, P3, P4>, 
		DelegateCallback<C, 4, true, P, P2, P3, P4>, P, P2, P3, P4
	>(pObj, Method, priority);
}


template <class C, typename P, typename P2, typename P3, typename P4>
static Delegate<C, 
	DelegateBase<P, P2, P3, P4>, 
	DelegateCallback<C, 4, false, P, P2, P3, P4>, P, P2, P3, P4
> delegate(C* pObj, void (C::*Method)(P, P2, P3, P4), int priority = 0) 
{
	return Delegate<C, 
		DelegateBase<P, P2, P3, P4>, 
		DelegateCallback<C, 4, false, P, P2, P3, P4>, P, P2, P3, P4
	>(pObj, Method, priority);
}


} // namespace scy


#endif // SCY_Delegate_H