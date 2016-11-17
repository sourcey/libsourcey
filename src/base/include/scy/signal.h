///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Signal_H
#define SCY_Signal_H


#include "scy/delegate.h"
#include "scy/mutex.h"
#include <atomic>
#include <memory>
#include <vector>


namespace scy {


/// Internal classes
namespace internal {

/// Signal slot storage class.
template <typename RT, typename... Args> struct Slot;

} // namespace internal


/// Signal and slots implementation.
///
/// To create a signal, declare member variables of type
/// `Signal<...>` in your class. The template parameter is the
/// argument types that will be passed to the callback functions.
///
/// Here's a simple example with a class `MyClass` that has a single
/// signal `my_signal` which takes a single `int` argument:
///
///     class MyClass
///     {
///     public:
///         Signal<int> my_signal;
///         ...
///     };
///
/// To connect to a signal, call its `attach()` member function,
/// passing a function (in the `std::function` sense) as the argument.
/// The function can be a function pointer, a functor object, or
/// an anonymous lambda function.
///
/// Here's an example connecting to the above signal to a lambda function:
///
///     MyClass my_class;
///     my_class.my_signal.attach([](int x) {
///         cout << "value: " << x << endl;
///     });
///
/// The `attach()` function returns a unique ID of type `int`
/// which can be used later to disconnect the callback function.
///
/// Here's an example using the above code to then immediately disconnect
/// the signal connection with the `detach()` member function:
///
///     MyClass my_class;
///     auto id = my_class.my_signal.attach([](int x) {
///         cout << "value: " << x << endl;
///     });
///     my_class.my_signal.detach(id);
///
/// The `detach()` function is passed the callback ID and will
/// return `true` if a callback was disconnected or `false` if the
/// ID wasn't found. Note that `detach()` can be safely called from
/// within the callback scope.
///
/// In the case of class members there is a `slot()` helper that can be
/// used to bind the signal like so:
///
///     class TargetClass
///     {
///     public:
///         Signal<int> my_signal;
///
///         int print(int x)
///         {
///             cout << "say: " << x << endl;
///         }
///     };
///
///     MyClass my_class;
///
///     TargetClass target_class;
///     my_class.my_signal += slot(&target_class, &TargetClass::print)
///
/// The `slot()` helper can also be used to disconnect class member callbacks
/// like so:
///
///     my_class.my_signal -= slot(&target_class, &TargetClass::print)
///
/// To emit the signal, call its `emit()` member function passing
/// arguments matching the types of those in the signal variable
/// declaration.
///
/// Using the above example code, here's an example showing how to
/// emit `my_signal`:
///
///     my_class.my_signal.emit(42);
///
/// Since the signal was declared to take a single `int` argument, this
/// will cause any callbacks connected to the signal to be called,
/// passing the integer `42` as the only argument.
///
template <typename RT> class Signal;
template <typename RT, typename... Args> class Signal<RT(Args...)>
{
public:
    typedef std::function<RT(Args...)> Function;
    typedef std::shared_ptr<internal::Slot<RT, Args...>> SlotPtr;

    /// Connects a lambda or `std::function` to the `Signal`.
    /// The returned value can be used to detach the slot.
    int attach(Function const& func, void* instance= nullptr, int id= -1,
               int priority= -1) const
    {
        return attach(std::make_shared<internal::Slot<RT, Args...>>(
            new FunctionDelegate<RT, Args...>(func), instance, id, priority));
    }

    /// Connects a `internal::Slot` instance to the `Signal`.
    /// The returned value can be used to detach the slot.
    int attach(SlotPtr slot) const
    {
        detach(slot); // clear duplicates
        Mutex::ScopedLock lock(_mutex);
        if (slot->id == -1)
            slot->id= ++_lastId; // TODO: assert unique?
        _slots.push_back(slot);
        return slot->id;
    }

    /// Detaches a previously attached slot.
    bool detach(int id) const
    {
        Mutex::ScopedLock lock(_mutex);
        for (auto it= _slots.begin(); it != _slots.end();) {
            auto& slot= *it;
            if (slot->alive() && slot->id == id) {
                slot->kill();
                _slots.erase(it);
                return true;
            } else
                ++it;
        }
        return false;
    }

    /// Detaches all slots for the given instance.
    bool detach(const void* instance) const
    {
        Mutex::ScopedLock lock(_mutex);
        bool removed= true;
        for (auto it= _slots.begin(); it != _slots.end();) {
            auto& slot= *it;
            if (slot->alive() && slot->instance == instance) {
                slot->kill();
                it= _slots.erase(it);
                removed= true;
            } else
                ++it;
        }
        return removed;
    }

    /// Detaches all attached functions for the given instance.
    bool detach(SlotPtr other) const
    {
        Mutex::ScopedLock lock(_mutex);
        for (auto it= _slots.begin(); it != _slots.end();) {
            auto& slot= *it;
            if (slot->alive() && (*slot->delegate == *other->delegate)) {
                slot->kill();
                _slots.erase(it);
                return true;
            } else
                ++it;
        }
        return false;
    }

    /// Detaches all previously attached functions.
    void detachAll() const
    {
        Mutex::ScopedLock lock(_mutex);
        while (!_slots.empty()) {
            _slots.back()->kill();
            _slots.pop_back();
        }
    }

    /// Emits the signal to all attached functions.
    void emit(Args... args) const
    {
        for (auto const& slot : slots()) {
            if (slot->alive()) {
                (*slot->delegate)(std::forward<Args>(args)...);
            }
        }
    }

    /// Returns the managed slot list.
    std::vector<SlotPtr> slots() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _slots;
    }

    /// Returns the number of active slots.
    std::size_t nslots() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _slots.size();
    }

    /// Convenience operators
    int operator+=(Function const& func) { return attach(func); }
    int operator+=(SlotPtr slot) { return attach(slot); }
    bool operator-=(int id) { return detach(id); }
    bool operator-=(const void* instance) { return detach(instance); }
    bool operator-=(SlotPtr slot) { return detach(slot); }

private:
    /// Non-copyable and non-movable
    // Signal(const Signal&) = delete;
    // Signal& operator=(const Signal&) = delete;

    mutable Mutex _mutex;
    mutable std::vector<SlotPtr> _slots;
    mutable int _lastId= 0;
};


typedef Signal<void()> NullSignal;


//
// Inline Helpers
//


// Class member function slot
template <class Class, class RT, class... Args>
std::shared_ptr<internal::Slot<RT, Args...>> slot(Class* instance,
                                                  RT (Class::*method)(Args...),
                                                  int id= -1, int priority= -1)
{
    return std::make_shared<internal::Slot<RT, Args...>>(
        new ClassDelegate<Class, RT, Args...>(instance, method), instance, id,
        priority);
}

// Const class member function slot
template <class Class, class RT, class... Args>
std::shared_ptr<internal::Slot<RT, Args...>>
slot(Class* instance, RT (Class::*method)(Args...) const, int id= -1,
     int priority= -1)
{
    return std::make_shared<internal::Slot<RT, Args...>>(
        new ConstClassDelegate<Class, RT, Args...>(instance, method), instance,
        id, priority);
}

// Static function slot
template <class RT, class... Args>
std::shared_ptr<internal::Slot<RT, Args...>> slot(RT (*method)(Args...),
                                                  int id= -1, int priority= -1)
{
    return std::make_shared<internal::Slot<RT, Args...>>(
        new FunctionDelegate<RT, Args...>([method](Args... args) {
            return (*method)(std::forward<Args>(args)...);
        }),
        nullptr, id, priority);
}


/// Internal classes
namespace internal {

/// Signal slot storage class.
template <typename RT, typename... Args> struct Slot
{
    AbstractDelegate<RT, Args...>* delegate;
    void* instance;
    int id;
    int priority;
    std::atomic_flag flag;

    Slot(AbstractDelegate<RT, Args...>* delegate, void* instance= nullptr,
         int id= -1, int priority= -1)
        : delegate(delegate)
        , instance(instance)
        , id(id)
        , priority(priority)
        , flag(ATOMIC_FLAG_INIT)
    {
        flag.test_and_set();
    }

    void kill() { flag.clear(); }

    bool alive() { return flag.test_and_set(); }
};

} // namespace internal


// /// This exception is used to break out of a Signal callback scope.
// class StopPropagation: public std::exception
// {
// public:
//     virtual ~StopPropagation() throw() {};
// };
//
//
// /// This class implements a thread-safe signal which
// /// broadcasts arbitrary data to multiple receiver delegates.
// template <class DelegateT, DelegateDefaultArgs>
// class SignalBase
// {
// public:
//     typedef std::list<DelegateT*>                  DelegateList;
//     typedef typename DelegateList::iterator       Iterator;
//     typedef typename DelegateList::const_iterator ConstIterator;
//
//     SignalBase() :
//         _enabled(true),
//         _dirty(false),
//         _count(0)
//     {
//     }
//
//     virtual ~SignalBase()
//     {
//         clear();
//     }
//
//     void operator += (const DelegateT& delegate) { attach(delegate); }
//     void operator -= (const DelegateT& delegate) { detach(delegate); }
//     void operator -= (const void* klass) { detach(klass); }
//
//     /// Attaches a delegate to the signal. If the delegate
//     /// already exists it will overwrite the previous delegate.
//     void attach(const DelegateT& delegate)
//     {
//         detach(delegate);
//         Mutex::ScopedLock lock(_mutex);
//         _delegates.push_back(delegate.clone());
//         _delegates.sort(DelegateT::ComparePrioroty);
//         _count++;
//     }
//
//     /// Detaches a delegate from the signal.
//     /// Returns true if the delegate was detached, false otherwise.
//     bool detach(const DelegateT& delegate)
//     {
//         Mutex::ScopedLock lock(_mutex);
//         for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
//         {
//             if (delegate.equals(*it) && !(*it)->cancelled()) {
//                 (*it)->cancel();
//                 _dirty = true;
//                 _count--;
//                 return true;
//             }
//         }
//         return false;
//     }
//
//     /// Detaches all delegates associated with the given class instance.
//     void detach(const void* klass)
//     {
//         {
//             Mutex::ScopedLock lock(_mutex);
//             for (Iterator it = _delegates.begin(); it != _delegates.end();
//             ++it) {
//                 if (klass == (*it)->object() && !(*it)->cancelled()) {
//                     (*it)->cancel();
//                     _dirty = true;
//                     _count--;
//                 }
//             }
//         }
//         // Call cleanup after detaching a class
//         cleanup();
//     }
//
//     /// Deletes cancelled delegates.
//     void cleanup()
//     {
//         Mutex::ScopedLock lock(_mutex);
//         if (_dirty) {
//             _dirty = false;
//             Iterator it = _delegates.begin();
//             while (it != _delegates.end()) {
//                 DelegateT* delegate = *it;
//                 if (delegate->cancelled()) {
//                     delete delegate;
//                     it = _delegates.erase(it);
//                 }
//                 else
//                     ++it;
//             }
//         }
//     }
//
//     /// Retrieves a list of active delegates while
//     /// simultaneously deleting any redundant delegates.
//     void obtain(DelegateList& active)
//     {
//         Mutex::ScopedLock lock(_mutex);
//         if (!_enabled) // skip if disabled
//             return;
//         Iterator it = _delegates.begin();
//         while (it != _delegates.end()) {
//             DelegateT* delegate = *it;
//             if (delegate->cancelled()) {
//                 assert(_dirty);
//                 delete delegate;
//                 it = _delegates.erase(it);
//             }
//             else {
//                 active.push_back(delegate);
//                 ++it;
//             }
//         }
//
//         _dirty = false;
//     }
//
//     virtual void emit(void* sender)
//     {
//         void* empty = nullptr;
//         emit(sender, (P)empty, (P2)empty, (P3)empty, (P4)empty);
//     }
//
//     virtual void emit(void* sender, P arg)
//     {
//         void* empty = nullptr;
//         emit(sender, arg, (P2)empty, (P3)empty, (P4)empty);
//     }
//
//     virtual void emit(void* sender, P arg, P2 arg2)
//     {
//         void* empty = nullptr;
//         emit(sender, arg, arg2, (P3)empty, (P4)empty);
//     }
//
//     virtual void emit(void* sender, P arg, P2 arg2, P3 arg3)
//     {
//         void* empty = nullptr;
//         emit(sender, arg, arg2, arg3, (P4)empty);
//     }
//
//     virtual void emit(void* sender, P arg, P2 arg2, P3 arg3, P4 arg4)
//     {
//         DelegateList toNotify;
//         obtain(toNotify);
//         try {
//             for (ConstIterator it = toNotify.begin(); it != toNotify.end();
//             ++it) {
//                 if ((*it)->accepts(sender, arg, arg2, arg3, arg4))
//                     (*it)->emit(sender, arg, arg2, arg3, arg4);
//             }
//         }
//         catch (StopPropagation&) {
//         }
//     }
//
//     void clear()
//     {
//         Mutex::ScopedLock lock(_mutex);
//         util::clearList(_delegates);
//         _count = 0;
//     }
//
//     void enable(bool flag = true)
//     {
//         Mutex::ScopedLock lock(_mutex);
//         _enabled = flag;
//     }
//
//     bool enabled() const
//     {
//         Mutex::ScopedLock lock(_mutex);
//         return _enabled;
//     }
//
//     DelegateList delegates() const
//     {
//         Mutex::ScopedLock lock(_mutex);
//         return _delegates;
//     }
//
//     /// Returns the number of delegates connected to the signal.
//     /// Use this instead of delegates().size() since the container
//     /// is not updated in real time.
//     int nslots() const
//     {
//         Mutex::ScopedLock lock(_mutex);
//         return _count;
//     }
//
// protected:
//     DelegateList _delegates;
//     bool _enabled;
//     bool _dirty;
//     int _count;
//
//     mutable Mutex    _mutex;
// };
//
//
// //
// // Signal Types
// //
//
//
// class NullSignal: public SignalBase<DelegateBase<>> {};
//
//
// template <typename P>
// class Signal: public SignalBase<DelegateBase<P>, P> {};
//
//
// template <typename P, typename P2>
// class Signal2: public SignalBase<DelegateBase<P, P2>, P, P2> {};
//
//
// template <typename P, typename P2, typename P3>
// class Signal3: public SignalBase<DelegateBase<P, P2, P3>, P, P2, P3> {};
//
//
// template <typename P, typename P2, typename P3, typename P4>
// class Signal4: public SignalBase<DelegateBase<P, P2, P3, P4>, P, P2, P3, P4>
// {};


} // namespace scy


#endif // SCY_Signal_H


/// @\}
