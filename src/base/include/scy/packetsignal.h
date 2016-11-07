///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_PacketSignal_H
#define SCY_PacketSignal_H


#include <cstdint>
#include "scy/packet.h"
#include "scy/polymorphicsignal.h"


namespace scy {


typedef DelegateBase<IPacket&> PacketDelegateBase;
typedef SignalBase<PacketDelegateBase, IPacket&> PacketSignal;
typedef std::vector<PacketSignal*> PacketSignalVec;

DefinePolymorphicDelegate(packetDelegate, IPacket, PacketDelegateBase)


} // namespace scy


#endif // SCY_PacketSignal_H

/// @\}
