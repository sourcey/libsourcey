#ifndef SOURCEY_MEDIA_Provider_H
#define SOURCEY_MEDIA_Provider_H


#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Base/IPacket.h"
#include "Sourcey/Base/IPacketizer.h"
#include "Sourcey/Base/IRunnable.h"
#include "Sourcey/Base/IProvider.h"
#include "Sourcey/Base/Callback.h"


namespace Sourcey {
namespace Media {


class IProvider: public Sourcey::IProvider
	/// Redeclaring the Data Provider interface under the Media
	/// namespace to declare a virtual encoder instance accessor
	/// so implimentations can access encoder format information.
{
public:
	IProvider() {};
	virtual ~IProvider() {};	

	virtual IEncoder* encoder() = 0;
};


typedef std::list<IProvider*> IProviderList;


template <class PacketizerT>
class Provider: public IProvider, public PacketizerT 
	/// The media provider consists of an encoder and a packetizer
	/// to streamline the process of packetizing and outputting 
	/// encoded media. 
{
public:
	Provider(IEncoder* encoder, bool freeEncoder = true) : 
		_encoder(encoder), 
		_freeEncoder(freeEncoder) {}

	virtual ~Provider() 
	{
		if (_encoder && _freeEncoder)
			delete _encoder;	
	}

	virtual void start() 
	{
		//assert(PacketizerT::numCallbacks() > 0);
		assert(_encoder);
		assert(_encoder->stateEquals(EncoderState::Idle));
		_encoder->PacketEncoded += delegate(this, &Provider::onPacketEncoded);

		// If our encoder has a live source then start it
		IRunnable* runnable = dynamic_cast<IRunnable*>(_encoder);
		if (runnable)
			runnable->start();
	}

	virtual void stop() 
	{
		PacketizerT::clearCallbacks();
		assert(_encoder);
		assert(_encoder->stateEquals(EncoderState::Idle));
		_encoder->PacketEncoded -= delegate(this, &Provider::onPacketEncoded);	

		// If our encoder has a live source then stop it
		IRunnable* runnable = dynamic_cast<IRunnable*>(_encoder);
		if (runnable)
			runnable->stop();
	}

	virtual bool isActive() const
	{
		return _encoder->stateEquals(EncoderState::Encoding);
	}

	virtual void operator += (const ICallback& callback) 
	{
		PacketizerT::addCallback(callback);
	}

	virtual void operator -= (const ICallback& callback) 
	{
		PacketizerT::removeCallback(callback);
	}

	void onPacketEncoded(void*, const IPacket& packet) 
	{
		PacketizerT::process(packet);
	}

	IEncoder* encoder() { return _encoder; }

protected:   
	IEncoder* _encoder;
	bool _freeEncoder;
};


} // namespace Media
} // namespace Sourcey


#endif // SOURCEY_MEDIA_Provider_H