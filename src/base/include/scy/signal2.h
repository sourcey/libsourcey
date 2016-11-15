///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
// Based on the signal implementation from nano-signal-slot.
// Copyright (c) 2012-2015 ApEk, NoAvailableAlias, Nano-signal-slot Contributors
// Original MIT license at the bottom of this file.
//
/// @addtogroup base
/// @{


#ifndef SCY_SignalV2_H
#define SCY_SignalV2_H


#include <cstdint>
#include <array>


namespace scy {
namespace v2 {


using DelegateKey = std::array<std::uintptr_t, 2>;

template <typename RT> class Function;
template <typename RT, typename... Args>
class Function<RT(Args...)>
{
    using Thunk = RT (*)(void*, Args...);

    friend class Observer;

    void* m_this_ptr; // instance pointer
    Thunk m_stub_ptr; // free function pointer

    Function(void* this_ptr, Thunk stub_ptr) :
        m_this_ptr(this_ptr), m_stub_ptr(stub_ptr) {}

    Function(DelegateKey delegate_key) :
        m_this_ptr(reinterpret_cast<void*>(delegate_key[0])),
        m_stub_ptr(reinterpret_cast<Thunk>(delegate_key[1])) {}

    public:

    template <RT (*fun_ptr) (Args...)>
    static inline Function bind()
    {
        return { nullptr, [](void* /*NULL*/, Args... args)
            { return (*fun_ptr)(std::forward<Args>(args)...); } };
    }

    template <typename T, RT (T::* mem_ptr) (Args...)>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args)
            { return (static_cast<T*>(this_ptr)->*mem_ptr) (std::forward<Args>(args)...); } };
    }

    template <typename T, RT (T::* mem_ptr) (Args...) const>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args)
            { return (static_cast<T*>(this_ptr)->*mem_ptr) (std::forward<Args>(args)...); } };
    }

    template <typename L>
    static inline Function bind(L* pointer)
    {
        return { pointer, [](void *this_ptr, Args... args)
            { return (static_cast<L*>(this_ptr)->operator()(std::forward<Args>(args)...)); }};
    }

    inline operator DelegateKey() const
    {
        return
        {{
            reinterpret_cast<std::uintptr_t>(m_this_ptr),
            reinterpret_cast<std::uintptr_t>(m_stub_ptr)
        }};
    }

    template <typename... Uref>
    inline RT operator() (Uref&&... args)
    {
        return (*m_stub_ptr)(m_this_ptr, std::forward<Uref>(args)...);
    }
};

// } // namespace Nano ------------------------------------------------------------
//
//
// namespace Nano
// {

class Observer
{

    template <typename T> friend class Signal;

    struct DelegateKeyObserver { DelegateKey delegate; Observer* observer; };
    struct Node { DelegateKeyObserver data; Node* next; } *head = nullptr;

    //-----------------------------------------------------------PRIVATE METHODS

    void insert(DelegateKey const& key, Observer* obs)
    {
        head = new Node { { key, obs }, head };
    }

    void remove(DelegateKey const& key, Observer* obs)
    {
        Node* node = head;
        Node* prev = nullptr;

        // Only delete the first occurrence
        for (; node; prev = node, node = node->next)
        {
            if (node->data.delegate == key && node->data.observer == obs)
            {
                if (prev)
                {
                    prev->next = node->next;
                }
                else
                {
                    head = head->next;
                }
                delete node;
                break;
            }
        }
    }

    void removeAll()
    {
        for (auto node = head; node;)
        {
            auto temp = node;

            // If this is us we only need to delete
            if (this != node->data.observer)
            {
                // Remove this slot from this listening Observer
                node->data.observer->remove(node->data.delegate, this);
            }
            node = node->next;
            delete temp;
        }
        head = nullptr;
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

    template <typename Delegate, typename... Uref>
    void onEach(Uref&&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;
            // Perfect forward and emit
            Delegate(node->data.delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void onEach_Accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;

            // Perfect forward, emit, and accumulate the return value
            accumulate(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
        }
    }

    //-----------------------------------------------------------------PROTECTED

    protected:

    ~Observer()
    {
        removeAll();
    }

    //--------------------------------------------------------------------PUBLIC

    public:

    Observer() = default;
    Observer(const Observer& other) = delete; // non construction-copyable
    Observer& operator=(const Observer&) = delete; // non copyable

};

// } // namespace Nano ------------------------------------------------------------
//
// namespace Nano
// {

template <typename RT> class Signal;
template <typename RT, typename... Args>
class Signal<RT(Args...)> : private Observer
{
    template <typename T>
    void insert_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::insert(key, instance);
        instance->insert(key, this);
    }

    template <typename T>
    void remove_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::remove(key, instance);
        instance->remove(key, this);
    }

    template <typename T>
    void insert_sfinae(DelegateKey const& key, ...)
    {
        Observer::insert(key, this);
    }

    template <typename T>
    void remove_sfinae(DelegateKey const& key, ...)
    {
        Observer::remove(key, this);
    }

    public:

    using Delegate = Function<RT(Args...)>;

    //-------------------------------------------------------------------CONNECT

    template <typename L>
    void connect(L* instance)
    {
        Observer::insert(Delegate::template bind (instance), this);
    }

    template <typename L>
    void connect(L& instance)
    {
        connect(std::addressof(instance));
    }

    template <RT (* fun_ptr)(Args...)>
    void connect()
    {
        Observer::insert(Delegate::template bind<fun_ptr>(), this);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }

    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }

    //----------------------------------------------------------------DISCONNECT

    template <typename L>
    void disconnect(L* instance)
    {
        Observer::remove(Delegate::template bind (instance), this);
    }

    template <typename L>
    void disconnect(L& instance)
    {
        disconnect(std::addressof(instance));
    }

    template <RT (* fun_ptr)(Args...)>
    void disconnect()
    {
        Observer::remove(Delegate::template bind<fun_ptr>(), this);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }

    //----------------------------------------------------EMIT / EMIT ACCUMULATE

#ifdef SIGNALS_USE_DEPRECATED

    /// Will not benefit from perfect forwarding
    /// TODO [[deprecated]] when c++14 is comfortably supported

    void operator() (Args... args)
    {
        emit(std::forward<Args>(args)...);
    }
    template <typename Accumulate>
    void operator() (Args... args, Accumulate&& accumulate)
    {
        emit_accumulate<Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Args>(args)...);
    }

#endif

    template <typename... Uref>
    void emit(Uref&&... args)
    {
        Observer::onEach<Delegate>(std::forward<Uref>(args)...);
    }

    template <typename Accumulate, typename... Uref>
    void emit_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        Observer::onEach_Accumulate<Delegate, Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Uref>(args)...);
    }

    //-------------------------------------------------------------------UTILITY

    bool empty() const
    {
        return Observer::isEmpty();
    }

    void removeAll()
    {
        Observer::removeAll();
    }
};


} } // namespace scy::v2


// #include <future>
// #include <assert.h>
//
//
// // https://kernelnewbies.org/FAQ/LikelyUnlikely
// #ifndef likely
// #define likely(x) __builtin_expect(!!(x), 1)
// #endif
// #ifndef unlikely
// #define unlikely(x) __builtin_expect(!!(x), 0)
// #endif
//
//
// namespace scy {
// namespace v2 {
//
//
// namespace internal {
//
//
// /// ProtoSignal is the template implementation for callback list.
// template <typename, typename>
// class ProtoSignal; // left undefined
//
//
// /// AsyncSignal is the template implementation for callback lists with std::future returns.
// template <typename>
// class AsyncSignal; // left undefined
//
//
// /// CollectorInvocation invokes handlers differently depending on return type.
// template <typename, typename>
// struct CollectorInvocation;
//
//
// /// PromiseInvocation invokes handlers differently depending on the promise value type.
// template <typename, typename>
// struct PromiseInvocation;
//
//
// /// CollectorLast returns the result of the last handler from a signal emission.
// template <typename Result>
// struct CollectorLast
// {
//     typedef Result CollectorResult;
//
//     explicit CollectorLast()
//         : _last()
//     {
//     }
//
//     inline bool operator()(Result r)
//     {
//         _last = r;
//         return true;
//     }
//
//     CollectorResult result()
//     {
//         return _last;
//     }
//
// private:
//     Result _last;
// };
//
//
// /// CollectorDefault implements the default handler collection behaviour.
// template <typename Result>
// struct CollectorDefault : CollectorLast<Result>
// {
// };
//
//
// /// CollectorDefault specialisation for handlers with void return type.
// template <>
// struct CollectorDefault<void>
// {
//     typedef void CollectorResult;
//
//     void result()
//     {
//     }
//
//     inline bool operator()(void)
//     {
//         return true;
//     }
// };
//
//
// /// CollectorInvocation specialisation for regular handlers.
// template <class Collector, class R, class... Args>
// struct CollectorInvocation<Collector, R(Args...)>
// {
//     static inline bool invoke(
//         Collector& collector, const std::function<R(Args...)>& cbf, Args... args)
//     {
//         return collector(cbf(args...));
//     }
// };
//
//
// /// CollectorInvocation specialisation for handlers with void return type.
// template <class Collector, class... Args>
// struct CollectorInvocation<Collector, void(Args...)>
// {
//     static inline bool invoke(
//         Collector& collector, const std::function<void(Args...)>& cbf, Args... args)
//     {
//         cbf(args...);
//         return collector();
//     }
// };
//
//
// /// HandlerLink implements a doubly-linked ring with ref-counted nodes
// /// containing callback links.
// template <class Function>
// struct HandlerLink
// {
//     HandlerLink *next, *prev;
//     Function function;
//     int ref_count;
//
//     explicit HandlerLink(const Function& callback)
//         : next(nullptr)
//         , prev(nullptr)
//         , function(callback)
//         , ref_count(1)
//     {
//     }
//
//     /*dtor*/ ~HandlerLink()
//     {
//         assert(ref_count == 0);
//     }
//
//     void incref()
//     {
//         ref_count += 1;
//         assert(ref_count > 0);
//     }
//
//     void decref()
//     {
//         ref_count -= 1;
//         if (!ref_count)
//             delete this;
//         else
//             assert(ref_count > 0);
//     }
//
//     void unlink()
//     {
//         function = nullptr;
//         if (next)
//             next->prev = prev;
//         if (prev)
//             prev->next = next;
//         next = prev = nullptr;
//         decref();
//     }
//
//     size_t add_before(const Function& callback)
//     {
//         auto link = new HandlerLink(callback);
//         link->prev = prev; // link to last
//         link->next = this;
//         prev->next = link; // link from last
//         prev = link;
//         static_assert(sizeof(link) == sizeof(size_t), "sizeof size_t");
//         return size_t(link);
//     }
//
//     bool deactivate(const Function& callback)
//     {
//         if (callback == function)
//         {
//             function = nullptr; // deactivate static head
//             return true;
//         }
//
//         for (auto link = this->next ? this->next : this; link != this; link = link->next)
//         {
//             if (callback == link->function)
//             {
//                 link->unlink(); // deactivate and unlink sibling
//                 return true;
//             }
//         }
//         return false;
//     }
//
//     bool remove_sibling(size_t id)
//     {
//         for (auto link = this->next ? this->next : this; link != this; link = link->next)
//         {
//             if (id == size_t(link))
//             {
//                 link->unlink(); // deactivate and unlink sibling
//                 return true;
//             }
//         }
//         return false;
//     }
// };
//
//
// /// ProtoSignal template specialised for the callback signature and collector.
// template <class Collector, class R, class... Args>
// class ProtoSignal<R(Args...), Collector>
//     : private CollectorInvocation<Collector, R(Args...)>
// {
// protected:
//     typedef std::function<R(Args...)> CbFunction;
//     typedef typename CbFunction::result_type Result;
//     typedef typename Collector::CollectorResult CollectorResult;
//     typedef HandlerLink<CbFunction> SignalLink;
//
// private:
//     SignalLink* _callbackRing; // linked ring of callback nodes
//     ProtoSignal(const ProtoSignal&) = delete;
//     ProtoSignal& operator=(const ProtoSignal&) = delete;
//
//     void ensure_ring()
//     {
//         if (!_callbackRing)
//         {
//             _callbackRing = new SignalLink(CbFunction()); // ref_count = 1
//             _callbackRing->incref(); // ref_count = 2, head of ring, can be deactivated but
//                                      // not removed
//             _callbackRing->next = _callbackRing; // ring head initialization
//             _callbackRing->prev = _callbackRing; // ring tail initialization
//         }
//     }
//
// protected:
//
//     /// ProtoSignal constructor, connects default callback if non-nullptr.
//     ProtoSignal(const CbFunction& method)
//         : _callbackRing(nullptr)
//     {
//         if (method != nullptr)
//         {
//             ensure_ring();
//             _callbackRing->function = method;
//         }
//     }
//
//     /// ProtoSignal destructor releases all resources associated with this signal.
//     ~ProtoSignal()
//     {
//         if (_callbackRing)
//         {
//             while (_callbackRing->next != _callbackRing)
//                 _callbackRing->next->unlink();
//             assert(_callbackRing->ref_count >= 2);
//             _callbackRing->decref();
//             _callbackRing->decref();
//         }
//     }
//
// public:
//
//     /// Operator to add a new function or lambda as signal handler, returns a
//     /// handler connection ID.
//     size_t attach(const CbFunction& cb)
//     {
//         ensure_ring();
//         return _callbackRing->add_before(cb);
//     }
//
//     /// Operator to remove a signal handler through its connection ID, returns
//     /// true if a handler was removed.
//     bool detach(size_t connection)
//     {
//         return _callbackRing ? _callbackRing->remove_sibling(connection) : false;
//     }
//
//     /// Emit a signal, i.e. invoke all its callbacks and collect return types
//     /// with the Collector.
//     CollectorResult emit(Args... args)
//     {
//         // check if an emission is needed
//         Collector collector;
//         if (!_callbackRing)
//             return collector.result();
//
//         // capture and incref signal handler list
//         const bool have_link0 = _callbackRing->function != nullptr;
//         size_t capacity = 1 * have_link0;
//         SignalLink* link;
//         for (link = _callbackRing->next; link != _callbackRing; link = link->next)
//             capacity++; // capacity measuring is O(n), but we expect n to be small generally
//         SignalLink* links[capacity];
//         size_t nlinks = 0;
//         if (have_link0)
//         {
//             _callbackRing->incref();
//             links[nlinks++] = _callbackRing;
//         }
//         for (link = _callbackRing->next; link != _callbackRing; link = link->next)
//         {
//             link->incref();
//             links[nlinks++] = link;
//         }
//         assert(nlinks <= capacity);
//
//         // walk signal handler list, invoke and decref
//         size_t i;
//         for (i = 0; i < nlinks; i++)
//         {
//             SignalLink* link = links[i];
//             if (link->function)
//             {
//                 const bool continue_emission
//                     = this->invoke(collector, link->function, args...);
//                 if (!continue_emission)
//                     break;
//             }
//             link->decref();
//         }
//         for (; i < nlinks; i++)
//             links[i]->decref(); // continue decref after 'break'
//
//         // done
//         return collector.result();
//     }
// };
//
//
// /// PromiseInvocation specialisation for regular handlers.
// template <class Promise, class R, class... Args>
// struct PromiseInvocation<Promise, R(Args...)>
// {
//     static inline void invoke(
//         Promise& promise, const std::function<R(Args...)>& callback, Args&&... args)
//     {
//         promise.set_value(callback(std::forward<Args>(args)...));
//     }
// };
//
//
// /// PromiseInvocation specialisation for handlers with void return type.
// template <class Promise, class... Args>
// struct PromiseInvocation<Promise, void(Args...)>
// {
//     static inline void invoke(
//         Promise& promise, const std::function<void(Args...)>& callback, Args&&... args)
//     {
//         callback(std::forward<Args>(args)...);
//         promise.set_value();
//     }
// };
//
//
// /// AsyncSignal template specialised for the callback signature.
// template <class R, class... Args>
// class AsyncSignal<R(Args...)> : private PromiseInvocation<std::promise<R>, R(Args...)>
// {
// protected:
//     typedef std::function<std::future<R>(Args...)> FutureFunction;
//     typedef std::function<R(Args...)> CbFunction;
//     typedef HandlerLink<FutureFunction> SignalLink;
//
// private:
//     SignalLink* _callbackRing; // linked ring of callback nodes
//     explicit AsyncSignal(const AsyncSignal&) = delete; // non-copyable
//     AsyncSignal& operator=(const AsyncSignal&) = delete; // non-assignable
//
//     void ensure_ring()
//     {
//         if (!_callbackRing)
//         {
//             _callbackRing = new SignalLink(FutureFunction()); // ref_count = 1
//             _callbackRing->incref(); // ref_count = 2, head of ring, can be deactivated but
//                                       // not removed
//             _callbackRing->next = _callbackRing; // ring head initialization
//             _callbackRing->prev = _callbackRing; // ring tail initialization
//         }
//     }
//
// protected:
//
//     /// AsyncSignal constructor, connects default callback if non-nullptr.
//     AsyncSignal(const FutureFunction& method)
//         : _callbackRing(nullptr)
//     {
//         if (method != nullptr)
//         {
//             ensure_ring();
//             _callbackRing->function = method;
//         }
//     }
//
//     /// AsyncSignal destructor releases all resources associated with this signal.
//     ~AsyncSignal()
//     {
//         if (_callbackRing)
//         {
//             while (_callbackRing->next != _callbackRing)
//                 _callbackRing->next->unlink();
//             assert(_callbackRing->ref_count >= 2);
//             _callbackRing->decref();
//             _callbackRing->decref();
//         }
//     }
//
// public:
//     class Emission
//     {
//         typedef std::function<std::future<R>(const FutureFunction&)> FunctionTrampoline;
//         std::vector<SignalLink*> _links;
//         size_t _current;
//         std::future<R> _future;
//         FunctionTrampoline _trampoline;
//
//     public:
//         Emission(SignalLink* start_link, Args&&... args)
//             : _current(0)
//         {
//             SignalLink* link = start_link;
//             if (link)
//             {
//                 do
//                 {
//                     if (link->function != nullptr)
//                     {
//                         link->incref();
//                         _links.push_back(link);
//                     }
//                     link = link->next;
//                 } while (link != start_link);
//             }
//
//             // wrap args into lambda for deferred execution
//             auto lambda  = [link](const FutureFunction& ff, Args... args) // -> std::future<R>
//             {
//                 return std::move(ff(args...));
//             };
//
//             // FIXME: instead of std::bind, use lambda capture: [args...](){return
//             // ff(args...);}, but see gcc bug #41933
//             _trampoline = std::bind(lambda, std::placeholders::_1, std::forward<Args>(args)...);
//         }
//
//         ~Emission()
//         {
//             for (size_t i = 0; i < _links.size(); i++)
//                 _links[i]->decref();
//         }
//
//         bool has_value()
//         {
//             return _future.valid() && future_ready();
//         }
//
//         R get_value()
//         {
//             _future.wait();
//             return _future.get();
//         }
//
//         bool done()
//         {
//             return _current >= _links.size() && !_future.valid();
//         }
//
//         bool pending()
//         {
//             return has_value() || (!_future.valid() && _current < _links.size());
//         }
//
//         bool dispatch()
//         {
//             emit_stepwise();
//             return !done();
//         }
//
//     private:
//         bool future_ready()
//         {
//             return _future.wait_for(std::chrono::nanoseconds(0))
//                 == std::future_status::ready;
//         }
//
//         inline void emit_stepwise()
//         {
//             if (_future.valid())
//                 return; // processing handler, use has_value
//             if (_current >= _links.size())
//                 return; // done
//             const FutureFunction& function = _links[_current]->function;
//             _current++;
//             if (likely(function != nullptr))
//                 _future = std::move(_trampoline(function)); // valid() == true
//         }
//
//         explicit Emission(const Emission&) = delete; // non-copyable
//         Emission& operator=(const Emission&) = delete; // non-assignable
//     };
//
//     /// Operator to add a new function or lambda as signal handler, returns a
//     /// handler connection ID.
//     size_t connect_future(const FutureFunction& fcb)
//     {
//         ensure_ring();
//         return _callbackRing->add_before(fcb);
//     }
//
//     size_t attach(const CbFunction& callback)
//     {
//         auto lambda = [this, callback](Args&&... args) // -> std::future<R>
//         {
//             std::promise<R> promise;
//             this->invoke(promise, callback, std::forward<Args>(args)...);
//             return std::move(promise.get_future());
//         };
//         return connect_future(lambda);
//     }
//
//     /// Operator to remove a signal handler through its connection ID, returns
//     /// if a handler was removed.
//     bool detach(size_t connection)
//     {
//         return _callbackRing ? _callbackRing->remove_sibling(connection) : false;
//     }
//
//     /// Create an asynchronous signal emission.
//     Emission* emission(Args&&... args)
//     {
//         return new Emission(_callbackRing, std::forward<Args>(args)...);
//     }
// };
//
// } // namespace internal
// // namespace scy::v2
//
//
// /// Signal is a template type providing an interface for arbitrary callback lists.
// /// A signal type needs to be declared with the function signature of its callbacks,
// /// and optionally a return result collector class type.
// ///
// /// Signal callbacks can be added with operator+= to a signal and removed with
// /// operator-=,  using a callback connection ID return by operator+= as argument.
// /// The callbacks of a signal are invoked with the emit() method and arguments
// /// according to the signature.
// ///
// /// The result returned by emit() depends on the signal collector class. By default,
// /// the result of the last callback is returned from emit(). Collectors can be
// /// implemented to accumulate callback results or to halt a running emissions in
// /// correspondance to callback results.
// ///
// /// The signal implementation is safe against recursion, so callbacks may be removed
// /// and added during a signal emission and recursive emit() calls are also safe.
// /// The overhead of an unused signal is intentionally kept very low, around the
// /// size of a single pointer.
// ///
// /// Note that the Signal template type is non-copyable.
// template <typename SignalSignature, class Collector
//     = internal::CollectorDefault<typename std::function<SignalSignature>::result_type>>
// class Signal : protected internal::ProtoSignal<SignalSignature, Collector>
// {
//     typedef internal::ProtoSignal<SignalSignature, Collector> ProtoSignal;
//     typedef typename ProtoSignal::CbFunction CbFunction;
//
// public:
//     explicit Signal(const Signal&) = delete; // non-copyable
//     Signal& operator=(const Signal&) = delete; // non-assignable
//     using ProtoSignal::emit;
//
//     class Connector
//     {
//         friend class Signal;
//         Signal& _signal;
//
//         Connector& operator=(const Connector&) = delete;
//         explicit Connector(Signal& signal)
//             : _signal(signal)
//         {
//         }
//
//     public:
//         /// Operator to add a new function or lambda as signal handler, returns
//         /// a handler connection ID.
//         size_t operator+=(const CbFunction& cb)
//         {
//             return _signal.attach(cb);
//         }
//
//         /// Operator to remove a signal handler through its connection ID,
//         /// returns true if a handler was removed.
//         bool operator-=(size_t connection_id)
//         {
//             return _signal.detach(connection_id);
//         }
//     };
//
//     /// Signal constructor, supports a default callback as argument.
//     Signal(const CbFunction& method = CbFunction())
//         : ProtoSignal(method)
//     {
//     }
//
//     /// Retrieve a connector object with operator+= and operator-= to connect
//     /// and disconnect signal handlers.
//     Connector operator()()
//     {
//         return Connector(*this);
//     }
// };
//
//
// /// This function creates a std::function by binding @a object to the member
// /// function pointer @a method.
// template <class Instance, class Class, class R, class... Args>
// std::function<R(Args...)> slot(Instance& object, R (Class::*method)(Args...))
// {
//     return [&object, method](Args... args)
//     {
//         return (object.*method)(args...);
//     };
// }
//
//
// /// This function creates a std::function by binding @a object to the member
// /// function pointer @a method.
// template <class Class, class R, class... Args>
// std::function<R(Args...)> slot(Class* object, R (Class::*method)(Args...))
// {
//     return [object, method](Args... args)
//     {
//         return (object->*method)(args...);
//     };
// }
//
//
// /// Keep signal emissions going while all handlers return !0 (true).
// template <typename Result>
// struct CollectorUntil0
// {
//     typedef Result CollectorResult;
//
//     explicit CollectorUntil0()
//         : _result()
//     {
//     }
//
//     const CollectorResult& result()
//     {
//         return _result;
//     }
//
//     inline bool operator()(Result r)
//     {
//         _result = r;
//         return _result ? true : false;
//     }
//
// private:
//     CollectorResult _result;
// };
//
//
// /// Keep signal emissions going while all handlers return 0 (false).
// template <typename Result>
// struct CollectorWhile0
// {
//     typedef Result CollectorResult;
//
//     explicit CollectorWhile0()
//         : _result()
//     {
//     }
//
//     const CollectorResult& result()
//     {
//         return _result;
//     }
//
//     inline bool operator()(Result r)
//     {
//         _result = r;
//         return _result ? false : true;
//     }
//
// private:
//     CollectorResult _result;
// };
//
//
// /// CollectorVector returns the result of the all signal handlers from a signal
// /// emission in a std::vector.
// template <typename Result>
// struct CollectorVector
// {
//     typedef std::vector<Result> CollectorResult;
//
//     const CollectorResult& result()
//     {
//         return _result;
//     }
//
//     inline bool operator()(Result r)
//     {
//         _result.push_back(r);
//         return true;
//     }
//
// private:
//     CollectorResult _result;
// };
//
//
// /// Connector provides a simple (dis-)connect interfaces for signals on RemoteHandle
// template <class Object, class SignalSignature>
// class Connector
// {
//     typedef std::function<SignalSignature> CbFunction;
//     typedef size_t (Object::*PMF)(size_t, const CbFunction&);
//     Object& _instance;
//     PMF _method;
//
// public:
//     Connector(Object& instance, PMF method)
//         : _instance(instance)
//         , _method(method)
//     {
//     }
//
//     /// Operator to add a new function or lambda as signal handler, returns a
//     /// handler connection ID.
//     size_t operator+=(const CbFunction& cb)
//     {
//         return (_instance.*_method)(0, cb);
//     }
//
//     /// Operator to remove a signal handler through its connection ID, returns
//     /// if a handler was removed.
//     bool operator-=(size_t connection_id)
//     {
//         return connection_id ? (_instance.*_method)(connection_id, *(CbFunction*)nullptr) : false;
//     }
// };
//
//
// /// AsyncSignal is a Signal type with support for std::future returns from handlers.
// /// The API is genrally analogous to Signal, however handlers that return a
// /// std::future<ReturnValue> are supported via the connect_future() method, and
// /// emissions are handled differently.
// /// Note that even simple handlers are wrapped into std::promise/std::future constructs,
// /// which makes AsyncSignal emissions significantly slower than Signal.emit().
// /// The emisison() method creates an Emission object, subject to a delete() call later on.
// ///
// /// The general idiom for using emission objects is as follows:
// ///
// /// @code
// /// MySignal::Emission *emi = sig_my_signal.emission (args...);
// /// while (!emi->done()) {
// ///   if (emi->pending())
// ///     emi->dispatch();         // this calls signal handlers
// ///   if (emi->has_value())
// ///     use (emi->get_value());  // value return from a signal handler via resolved future
// ///   else
// ///     ThisThread::yield();     // allow for (asynchronous) signal handler execution
// /// }
// /// delete emi;
// /// @endcode
// ///
// /// Multiple emission objects may be created and used at the same time, and
// /// deletion of an emission before done() returns true is also supported.
// template <typename SignalSignature>
// class AsyncSignal : protected internal::AsyncSignal<SignalSignature>
// {
//     typedef internal::AsyncSignal<SignalSignature> BaseSignal;
//     typedef typename BaseSignal::CbFunction CbFunction;
//     typedef typename BaseSignal::FutureFunction FutureFunction;
//
//     class Connector
//     {
//         AsyncSignal& _signal;
//         friend class AsyncSignal;
//         Connector& operator=(const Connector&) = delete;
//         explicit Connector(AsyncSignal& signal)
//             : _signal(signal)
//         {
//         }
//
//     public:
//         /// Method to add a new std::future<ReturnValue> function or lambda as
//         /// signal handler, returns a handler connection ID.
//         size_t connect_future(const FutureFunction& ff)
//         {
//             return _signal.connect_future(ff);
//         }
//
//         /// Operator to add a new function or lambda as signal handler, returns
//         /// a handler connection ID.
//         size_t operator+=(const CbFunction& cb)
//         {
//             return _signal.attach(cb);
//         }
//
//         /// Operator to remove a signal handler through its connection ID,
//         /// returns if a handler was removed.
//         bool operator-=(size_t connection_id)
//         {
//             return _signal.detach(connection_id);
//         }
//     };
//
// public:
//     using typename BaseSignal::Emission;
//     using BaseSignal::emission;
//
//     /// Signal constructor, supports a default callback as argument.
//     AsyncSignal(const FutureFunction& method = FutureFunction())
//         : BaseSignal(method)
//     {
//     }
//
//     /// Retrieve a connector object with operator+= and operator-= to connect
//     /// and disconnect signal handlers.
//     Connector operator()()
//     {
//         return Connector(*this);
//     }
// };
//
// } } // scy::v2


#endif // SCY_SignalV2_H

/// @\}

// The MIT License (MIT)
//
// Copyright (c) 2012-2015 ApEk, NoAvailableAlias, Nano-signal-slot Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
