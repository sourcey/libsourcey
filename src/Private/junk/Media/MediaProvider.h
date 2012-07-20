#ifndef VIDEO_PROVIDER_H
#define VIDEO_PROVIDER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Base/Encoder.h"
#include "Sourcey/Base/DataProvider.h"
#include "Sourcey/Base/Packet.h"
#include "Sourcey/Base/Packetizer.h"
#include "Sourcey/Base/Signal.h"


namespace Sourcey {
namespace Media {


class AbstractDataProvider {
	/// This is the base class for all instantiations of
	/// the DataProvider template.
public:
	AbstractDataProvider() {};
	virtual ~AbstractDataProvider() {};	
	
	virtual void start() = 0;
	virtual void stop() = 0;

	Signal<Packet&> PacketReady;
};


template <class EncoderT, class PacketizerT>
class DataProvider: public AbstractDataProvider, public EncoderT, public PacketizerT {
	// The Media Provider class is one step removed from a Capture
	// Encoder class, as it can be used to impliment codec specific 
	// packaging capabilities.
public:
	DataProvider(/*Encoder* encoder, bool destroyOnStop = false*/);
	virtual ~DataProvider() {
		//cout << "DataProvider::~DataProvider()" << endl;
		//if (_encoder)
		//	delete _encoder;
		//if (_packetizer)
		//	delete _packetizer;
	}

	virtual void start() {
		/*
		assert(_encoder);
		assert(_encoder->state().id() == EncoderState::Idle);
		//assert(_packetizer);
		//if (_packetizer)
		//	_packetizer->PacketReady += delegate(this, &DataProvider::onMediaPacketReady);	
		if (_encoder) {
			_encoder->MediaEncoded += delegate(this, &DataProvider::onMediaEncoded);	
			_encoder->start();
		}
		*/
		assert(state().id() == EncoderState::Idle);
		MediaEncoded += delegate(this, &DataProvider::onMediaEncoded);	
	}

	virtual void stop() {
		/*
		assert(_encoder);
		assert(_encoder->state().id() != EncoderState::Idle);
		//assert(_packetizer);
		//if (_packetizer)
		//	_packetizer->PacketReady -= delegate(this, &DataProvider::onMediaPacketReady);	
		if (_encoder) {
			_encoder->MediaEncoded -= delegate(this, &DataProvider::onMediaEncoded);	
			_encoder->stop();	
		}	
		*/
		assert(state().id() != EncoderState::Idle);
		MediaEncoded -= delegate(this, &DataProvider::onMediaEncoded);	
	}

	void onMediaEncoded(MediaPacket& packet) {
		PacketizerT::process((const char*)packet.data, packet.size);
	}
	//void onMediaPacketReady(void* sender, Sourcey::Packet& packet) {
	//	MediaPacketReady.send(packet);
	//}

protected:   
	//Encoder* _encoder;
	//Packetizer<T>* _packetizer;
	//bool _destroyOnStop;
};


//typedef std::list<DataProvider*> DataProviderList;


} } // namespace Sourcey::Media


#endif // VIDEO_PROVIDER_H