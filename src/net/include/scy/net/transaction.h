///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_Transaction_H
#define SCY_Net_Transaction_H


#include "scy/net/packetsocket.h"
#include "scy/packettransaction.h"


namespace scy {
namespace net {


/// This class provides request/response functionality for IPacket
/// types emitted from a Socket.
template <class PacketT>
class SCY_EXTERN Transaction : public PacketTransaction<PacketT>,
                               public PacketSocketEmitter
{
public:
	typedef PacketTransaction<PacketT> BaseT;

    Transaction(const net::Socket::Ptr& socket, const Address& peerAddress,
                int timeout = 10000, int retries = 1,
                uv::Loop* loop = socket->loop())
        : PacketTransaction<PacketT>(timeout, retries, loop)
        , PacketSocketEmitter(socket)
        , _peerAddress(peerAddress)
    {
        TraceS(this) << "Create" << std::endl;
        // reinterpret_cast<net::PacketSocketEmitter*>(
        //     impl.get())->addReceiver(this, 100); // highest priority
        // PacketSocketEmitter::addReceiver(this, 100); // highest priority
    }

    virtual bool send()
    {
        TraceS(this) << "Send" << std::endl;
        if (impl->sendPacket(BaseT::_request, _peerAddress) > 0)
            return BaseT::send();
        BaseT::setState(this, TransactionState::Failed);
        return false;
    }

    virtual void cancel()
    {
        TraceS(this) << "Cancel" << std::endl;
        BaseT::cancel();
    }

    virtual void dispose()
    {
        TraceS(this) << "Dispose" << std::endl;
        // PacketSocketEmitter::impl->removeReceiver(this);
        BaseT::dispose(); // gc
    }

    Address peerAddress() const
    {
        return _peerAddress;
    }

protected:
    virtual ~Transaction() {}

    /// Overrides the PacketSocketEmitter::onPacket
    /// callback for checking potential response candidates.
    virtual void onPacket(IPacket& packet)
    {
        TraceS(this) << "On packet: " << packet.size() << std::endl;
        if (BaseT::handlePotentialResponse(static_cast<PacketT&>(packet))) {

            // Stop socket data propagation since
            // we have handled the packet
            throw StopPropagation(); 
        }
    }

    /// Called when a successful response match is received.
    virtual void onResponse()
    {
        TraceS(this) << "On success: " << BaseT::_response.toString() << std::endl;
        PacketSignal::emit(BaseT::_response);
    }

    /// Sub classes should derive this method to implement
    /// response checking logic.
    /// The base implementation only performs address matching.
    virtual bool checkResponse(const PacketT& packet)
    {
        assert(packet.info && "socket must provide packet info");
        if (!packet.info)
            return false;
        auto info = reinterpret_cast<net::PacketInfo*>(packet.info);
        return impl->address() == info->socket->address()
            && _peerAddress == info->peerAddress;
    }

    Address _peerAddress;
};


} // namespace net
} // namespace scy


#endif // SCY_Net_Transaction_H


/// @\}
