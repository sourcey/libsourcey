#ifndef SPOT_BASE_IStreamingSession_H
#define SPOT_BASE_IStreamingSession_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Timer.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/TCPPacketStreamConnection.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/JSON/ISerializable.h"
#include "Sourcey/Symple/Peer.h"
#include "Sourcey/Symple/Command.h"
#include "Sourcey/Media/IEncoder.h"

#include <sstream>
#include <map>


namespace Sourcey {
namespace Spot {


class IStreamingManager;
class IChannel;


struct StreamingParams: public Media::EncoderParams, public JSON::ISerializable
{	
	std::string token;
	std::string transport;
	std::string protocol;
	std::string encoding;
	int timeout;
	StreamingParams(
		const std::string& transport = "TCP",	/// UDP, TCP, TLS
		const std::string& protocol = "Raw",	/// Raw, HTTP, RTP/AVP, ...
		const std::string& encoding = "None",	/// None, Base64, ...
		const Media::Format& iformat = Media::Format(),
		const Media::Format& oformat = Media::Format(),
		int timeout = 20000);

	bool valid();

	void serialize(JSON::Value& root);
	void deserialize(JSON::Value& root);
};


typedef PacketStream ConnectionStream;
typedef Net::TCPPacketStreamConnection TCPStreamingConnection;
typedef std::vector<ConnectionStream*> ConnectionStreamList;


struct StreamingState: public StateT 
{
	enum Type 
	{
		None = 0,
		Initializing,
		Ready,
		Active,
		Failed,
		Terminating
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:				return "None";
		case Initializing:		return "Initializing";
		case Ready:				return "Ready";
		case Active:			return "Active";
		case Failed:			return "Failed";
		case Terminating:		return "Terminating";
		default:				assert(false);
		}
		return "undefined"; 
	}
};


class IStreamingSession: public StatefulSignal<StreamingState>, public ILoggable
{
public:
	IStreamingSession(IStreamingManager& service, 
					  IChannel& channel, 
					  const StreamingParams& params, 
					  Symple::Peer& peer = Symple::Peer(),
					  const Symple::Command& command = Symple::Command());

	virtual ~IStreamingSession();
		/// The media session is terminated(), not deleted.

	virtual void initialize();
		/// Session initialization logic should be implemented here.
		/// Derived methods should always call the base method first.
		/// An exception is thrown in case of failure.

	virtual void terminate();
		/// Session termination logic should be implemented here.
		/// Derived methods should always call the base method.
		/// NOTE: Sessions are never deleted, only terminated.

	virtual void addCandidate(const std::string& type, 
							  const Net::Address& address, 
							  const std::string& uri = "");
		// Adds a streaming candidate to the command.

	virtual bool removeCandidate(const std::string& type, 
							     const Net::Address& address);
		
	virtual ConnectionStream* createConnection() = 0;
		/// Creates a connection stream for this media session.

	virtual TCPStreamingConnection* createTCPConnection(const Poco::Net::StreamSocket& socket,
														const std::string& request) = 0;
		/// Creates a TCP socket connection for this media session.

	virtual std::string token() const;
	virtual IStreamingManager& service();
	virtual PacketStream& stream();
	virtual IChannel& channel();
	virtual Symple::Peer& peer();
	virtual Symple::Command& command();
	virtual StreamingParams& params();
	virtual ConnectionStreamList connections() const;

	virtual bool isActive() const;
	virtual bool isTerminating() const;

	virtual const char* className() const { return "StreamingSession"; }

protected:	
	virtual bool setState(unsigned int id, const std::string& message = "");
		// Sub-classes must use this method for setting state
		// because the sender instance must match the base.

	IStreamingManager&		_service;
	IChannel&				_channel; 
	PacketStream			_stream;
	Symple::Peer&			_peer;
	Symple::Command			_command;
	StreamingParams			_params;
	ConnectionStreamList	_connections;
	mutable Poco::FastMutex _mutex;
};


} } // namespace Sourcey::Spot


#endif // SPOT_BASE_IStreamingSession_H




	//std::string id;
	//std::string channel;
		//const std::string& id = "",				/// The session ID
		//const std::string& channel = "",
/*
struct Candidate
{
	std::string type; 
		/// Candidate type is implementation defined.
		/// Currently used are lan, host and relay.

	Net::Address address;
	std::string uri; 

	Candidate(const std::string& type = "", 
			  const Net::Address& address = Net::Address(), 
			  const std::string& uri = "") :
		type(type), address(address), uri(uri) {};	
	
	bool operator ==(const Candidate& r) const {
		return type == r.type 
			&& address == r.address 
			&& uri == r.uri;
	}
};
//typedef std::vector<Candidate> CandidateList;
*/
	//virtual JSON::Value& candidates();
	//virtual void startTimer(int timeout = 0);
	//virtual void stopTimer();
	//virtual void onTimeout(TimerCallback<IStreamingSession>& timer);

	//CandidateList			_candidates;
		/// Returns true if the stream is ready, or false if the, int timeout = 20000
		/// stream is still in the negotiating phase in which case
		/// the stream will manage it's own .
	//StreamingParams	_params;