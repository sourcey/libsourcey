#include "Sourcey/Media/MediaProvider.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Video Media Provider
//
// ============================================================================
template <class T>
MediaProvider<T>::MediaProvider(Encoder* encoder, Packetizer<T>* packetizer, bool destroyOnStop) :
	_encoder(encoder),
	_packetizer(packetizer),
	_destroyOnStop(destroyOnStop) {
	cout << "MediaProvider::MediaProvider()" << endl;
}


/*
template <class T>
MediaProvider<T>::MediaProvider(bool destroyOnStop) : 
	//MediaProvider(destroyOnStop),
	_encoder(NULL),
	_packetizer(NULL),
	_destroyOnStop(destroyOnStop) {
	cout << "MediaProvider::MediaProvider()" << endl;
}
*/


template <class T>
MediaProvider<T>::~MediaProvider() {
	cout << "MediaProvider::~MediaProvider()" << endl;
	if (_encoder)
		delete _encoder;
	if (_packetizer)
		delete _packetizer;
}


//template <>
void MediaProvider::start() {
	assert(_encoder);
	assert(_encoder->state().id() == EncoderState::Idle);
	assert(_packetizer);
	if (_packetizer)
		_packetizer->PacketReady += delegate(this, &MediaProvider::onVideoPacketReady);	
	if (_encoder) {
		_encoder->VideoEncoded += delegate(this, &MediaProvider::onVideoEncoded);	
		_encoder->start();
	}
}


template <class T>
void MediaProvider<T>::stop() {
	assert(_encoder);
	assert(_encoder->state().id() != EncoderState::Idle);
	assert(_packetizer);
	if (_packetizer)
		_packetizer->PacketReady -= delegate(this, &MediaProvider::onVideoPacketReady);	
	if (_encoder) {
		_encoder->VideoEncoded -= delegate(this, &MediaProvider::onVideoEncoded);	
		_encoder->stop();	
	}	
}


template <class T>
void MediaProvider<T>::onVideoEncoded(void* sender, VideoPacket& packet) {
	_packetizer->process((const char*)packet.data, packet.size);
}


template <class T>
void MediaProvider<T>::onVideoPacketReady(void* sender, Sourcey::Packet& packet) {
	MediaPacketReady.send(packet);
}


} } // namespace Sourcey::Media