///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup util
/// @{


#ifndef SCY_StreamManager_H
#define SCY_StreamManager_H


#include "scy/collection.h"
#include "scy/packetstream.h"


namespace scy {


typedef LiveCollection<std::string, PacketStream,
                       std::default_delete<PacketStream>
                       // DeferredDeleter<PacketStream>
                       // DestroyMethodDeleter<PacketStream>
                       >
    StreamManagerBase;


class SCY_EXTERN StreamManager : public StreamManagerBase
{
public:
    typedef StreamManagerBase Manager;
    typedef Manager::Map Map;

public:
    StreamManager(bool freeClosedStreams = true);
    virtual ~StreamManager();

    virtual bool addStream(PacketStream* stream, bool whiny = true);
    virtual bool closeStream(const std::string& name, bool whiny = true);
    virtual void closeAll();
    virtual PacketStream* getStream(const std::string& name, bool whiny = true);

    /// Returns the first stream in the list, or NULL.
    virtual PacketStream* getDafaultStream();

    virtual Map streams() const;

    virtual void print(std::ostream& os) const;

protected:
    /// Called after a stream is added.
    virtual void onAdd(PacketStream* task);

    /// Called after a stream is removed.
    virtual void onRemove(PacketStream* task);

    virtual void onStreamStateChange(void* sender, PacketStreamState& state,
                                     const PacketStreamState&);

    virtual const char* className() const { return "Stream Manager"; };

protected:
    bool _freeClosedStreams;
};


} // namespace scy


#endif // SCY_StreamManager_H


/// @\}
