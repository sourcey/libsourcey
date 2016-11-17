///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_PacketTransaction_H
#define SCY_PacketTransaction_H


#include "scy/timer.h"
#include "scy/stateful.h"
#include "scy/interface.h"
#include "scy/packet.h"


namespace scy {


struct TransactionState: public State
{
    enum Type
    {
        Waiting = 0,
        Running,
        Success,
        Cancelled,
        Failed
    };

    std::string str(unsigned int id) const
    {
        switch(id) {
        case Waiting:        return "Waiting";
        case Running:        return "Running";
        case Success:        return "Success";
        case Cancelled:        return "Cancelled";
        case Failed:        return "Failed";
        }
        return "undefined";
    };
};


/// This class provides request/response functionality for IPacket types.
///
/// PacketTransactions are fire and forget. The object will be deleted
/// after a successful response or a timeout.
template <class PacketT>
class PacketTransaction: public async::Sendable, public Stateful<TransactionState>
{
public:
    PacketTransaction(long timeout = 10000, int retries = 0, uv::Loop* loop = uv::defaultLoop()) :
        _timer(loop),
        _timeout(timeout),
        _retries(retries),
        _attempts(0),
        _destroyed(false)
    {
    }

    PacketTransaction(const PacketT& request, long timeout = 10000, int retries = 0, uv::Loop* loop = uv::defaultLoop()) :
        _request(request),
        _timer(loop),
        _timeout(timeout),
        _retries(retries),
        _attempts(0),
        _destroyed(false)
    {
    }

    /// Starts the transaction timer and sends the request.
    /// Overriding classes should implement send logic here.
    virtual bool send()
    {
        if (!canResend())
            return false;

        _attempts++;
        if (_timer.active())
            _timer.stop();
        _timer.Timeout += slot(this, &PacketTransaction::onTimeout);
        _timer.start(_timeout, 0);

        return setState(this, TransactionState::Running);
    }

    /// Cancellation means that the agent will not retransmit
    /// the request, will not treat the lack of response to be
    /// a failure, but will wait the duration of the transaction
    /// timeout for a response.
    void cancel();
    bool cancelled() const;

    /// Schedules the transaction for deferred deletion.
    ///
    /// It is safe to call this function while the transaction
    /// is still active, providing the call is made from the same
    /// loop thread which the timer is running on.
    ///
    /// Protected by the base implementation as this is called
    /// by the internal state machine.
    virtual void dispose()
    {
        traceL("PacketTransaction", this) << "Dispose" << std::endl;

        if (!_destroyed) {
            _destroyed = true;
            _timer.Timeout -= slot(this, &PacketTransaction::onTimeout);
            _timer.stop();

            deleteLater<PacketTransaction>(this);
        }
    }

    virtual bool canResend();
    int attempts() const;
    int retries() const;

    PacketT& request();
    PacketT request() const;

    PacketT& response();
    PacketT response() const;

protected:
    virtual ~PacketTransaction()
    {
        //assert(!stateEquals(TransactionState::Running));
    }

    /// Override to handle post state change logic.
    virtual void onStateChange(TransactionState& state, const TransactionState&)
    {
        traceL("PacketTransaction", this) << "On state change: "
            << state.toString() << std::endl;

        if (state.equals(TransactionState::Success) ||
            state.equals(TransactionState::Failed))
            dispose();
    }

    /// Processes a potential response candidate
    /// and updates the state accordingly.
    virtual bool handlePotentialResponse(const PacketT& packet)
    {
        if (stateEquals(TransactionState::Running) && checkResponse(packet)) {
            _response = packet;
            onResponse();
            setState(this, TransactionState::Success);
            return true;
        }
        return false;
    }

    /// Checks a potential response candidate and
    /// returns true on successful match.
    virtual bool checkResponse(const PacketT& packet) = 0;

    /// Called when a successful response is received.
    virtual void onResponse()
    {
        traceL("PacketTransaction", this) << "Success: "
            << _response.toString() << std::endl;
    }

    virtual void onTimeout()
    {
        debugL("PacketTransaction", this) << "Timeout" << std::endl;

        if (!canResend()) {
            //if (!cancelled())
            //    setState(this, TransactionState::Failed, "Transaction timeout");
            //dispose();
            setState(this, TransactionState::Failed, "Transaction timeout");
        }
        else send();
    }

protected:
    friend struct std::default_delete<PacketTransaction>;

    PacketT _request;
    PacketT _response;
    Timer _timer;
    int _timeout;         ///< The request timeout in milliseconds.
    int _retries;         ///< The maximum number of attempts before the transaction is considered failed.
    int _attempts;        ///< The number of times the transaction has been sent.
    bool _destroyed;
};


template <class T> inline void PacketTransaction<T>::cancel() { setState(this, TransactionState::Cancelled); }
template <class T> inline bool PacketTransaction<T>::cancelled() const { return stateEquals(TransactionState::Cancelled); }

template <class T> inline bool PacketTransaction<T>::canResend() { return !cancelled() && attempts() <= retries(); }
template <class T> inline int PacketTransaction<T>::attempts() const { return _attempts; }
template <class T> inline int PacketTransaction<T>::retries() const { return _retries; }

template <class T> inline T& PacketTransaction<T>::request() { return _request; }
template <class T> inline T PacketTransaction<T>::request() const { return _request; }
template <class T> inline T& PacketTransaction<T>::response() { return _response;    }
template <class T> inline T PacketTransaction<T>::response() const { return _response; }


} // namespace scy


#endif // SCY_IDepacketizerR_H

/// @\}
