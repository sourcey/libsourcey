#ifndef MEDIA_PROVIDER_H
#define MEDIA_PROVIDER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCaptureEncoder.h"
#include "Sourcey/Util/Packet.h"
#include "Sourcey/Util/Packetizer.h"
#include "Sourcey/Util/Signal.h"


namespace Sourcey {
namespace Media {


class MediaProvider {
	// The Media Provider class is one step removed from a Capture
	// Encoder class, as it can be used to impliment codec specific 
	// packaging capabilities.
public:
	//MediaProvider(VideoCaptureEncoder* encoder, Packetizer* packetizer, bool destroyOnStop = false);
	MediaProvider(bool destroyOnStop = false);
	virtual ~MediaProvider();

	virtual void start() = 0;
	virtual void stop() = 0;

	//void onVideoEncoded(void* sender, VideoPacket& packet);
	//void onMediaPacketReady(void* sender, Sourcey::Packet& packet);

	Signal<MediaPacket&> MediaPacketReady;

protected:   
	//VideoCaptureEncoder* _encoder;
	//Packetizer* _packetizer;
	bool _destroyOnStop;
};


typedef std::list<MediaProvider*> MediaProviderList;


} } // namespace Sourcey::Media


#endif // MEDIA_PROVIDER_H