///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_PolymorphicSignal_H
#define SCY_PolymorphicSignal_H


#include "scy/interface.h"
#include "scy/signal.h"


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
    template <class C, typename PolymorphicT>                            \
    static Delegate<C,                                                    \
        DelegateBaseT,                                                    \
        DelegateCallback<C, 8, false, PolymorphicT>,                    \
        PolymorphicBaseT&                                                \
    > Name(C* pObj, void (C::*Method)(PolymorphicT&), int priority = 0) \
    {                                                                    \
        return Delegate<C,                                                \
            DelegateBaseT,                                                \
            DelegateCallback<C, 8, false, PolymorphicT>,                \
            PolymorphicBaseT&                                            \
        >(pObj, Method, priority);                                        \
    }                                                                    \
                                                                        \
                                                                        \
    template <class C, typename PolymorphicT>                            \
    static Delegate<C,                                                    \
        DelegateBaseT,                                                    \
        DelegateCallback<C, 8, true, PolymorphicT>,                        \
        PolymorphicBaseT&                                                \
    > Name(C* pObj, void (C::*Method)(void*, PolymorphicT&),            \
        int priority = 0)                                                \
    {                                                                    \
        return Delegate<C,                                                \
            DelegateBaseT,                                                \
            DelegateCallback<C, 8, true, PolymorphicT>,                    \
            PolymorphicBaseT&                                            \
        >(pObj, Method, priority);                                        \
    }                                                                    \


#define DefinePolymorphicDelegateWithArg(Name, PolymorphicBaseT,        \
    DelegateBaseT, ArgType, ArgDefault)                                    \
                                                                        \
    template <class C, typename PolymorphicT>                            \
    static Delegate<C,                                                    \
        DelegateBaseT,                                                    \
        DelegateCallback<C, 8, false, PolymorphicT>,                    \
        PolymorphicBaseT&                                                \
    > Name(C* pObj, void (C::*Method)(PolymorphicT&),                    \
        ArgType data = ArgDefault, int priority = 0)                    \
    {                                                                    \
        return Delegate<C,                                                \
            DelegateBaseT,                                                \
            DelegateCallback<C, 8, false, PolymorphicT>,                \
            PolymorphicBaseT&                                            \
        >(pObj, Method, data, priority);                                \
    }                                                                    \
                                                                        \
                                                                        \
    template <class C, typename PolymorphicT>                            \
    static Delegate<C,                                                    \
        DelegateBaseT,                                                    \
        DelegateCallback<C, 8, true, PolymorphicT>,                     \
        PolymorphicBaseT&                                                \
    > Name(C* pObj, void (C::*Method)(void*, PolymorphicT&),            \
        ArgType data = ArgDefault, int priority = 0)                    \
    {                                                                    \
        return Delegate<C,                                                \
            DelegateBaseT,                                                \
            DelegateCallback<C, 8, true, PolymorphicT>,                    \
            PolymorphicBaseT&                                            \
        >(pObj, Method, data, priority);                                \
    }                                                                    \


DefinePolymorphicDelegate(polymorphicDelegate, basic::Polymorphic, PolymorphicDelegateBase)


} // namespace scy


#endif // SCY_PolymorphicSignal_H

/// @\}
