//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_ICE_Agent_H
#define SOURCEY_ICE_Agent_H


#include "Sourcey/ICE/IAgentObserver.h"
#include "Sourcey/ICE/MediaStream.h"
#include "Sourcey/UserManager.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Flaggable.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Timer.h"
#include "Sourcey/SDP/Message.h"


namespace Sourcey {
namespace ICE {


struct Options 
{
	std::string title;
	std::string software;
	std::string realm;
	std::string localUFrag;
	std::string localPassword;
	std::string senders;		// SDP a=sendrecv, recvonly, or sendonly
	long timeout;				// The overall ICE processing timeout
	long transactionTimeout;	// Connectivity check timeouts
	bool disableLocalhost;
	bool disableRelay;
	bool disableSTUN;
	bool disableHost;			// Forces the use of relay candidates
	int hostCandidateOrder;
	int srflxCandidateOrder;
	int prflxCandidateOrder;
	int relayCandidateOrder;
	int triggeredCheckDelay;
	int maxRequestRetries;		// How many times a request will be re-sent before considered failed
	int maxConnectivityChecks;	// TODO: implement me [18.5.2. STUN Amplification Attack]
	int maxRemoteCandidates;	// TODO: implement me [18.5.2. STUN Amplification Attack]
	Net::Address stunAddress;
	Net::Address turnAddress;

	Options() 
	{
		title					= "ICE Agent";
		software				= "Sourcey ICE [rfc5245]";
		realm					= "anionu.com";
		localUFrag				= CryptoProvider::generateRandomKey(4);
		localPassword			= CryptoProvider::generateRandomKey(22);
		timeout					= 10000;
		transactionTimeout		= 3000;
		disableLocalhost		= false;
		disableRelay			= false;
		disableSTUN				= false;
		disableHost				= false;
		senders					= "sendrecv";
		hostCandidateOrder		= 126;										// numeric 0-126, 126 is the highest preference, and a 0 is the lowest
		srflxCandidateOrder		= 100;
		prflxCandidateOrder		= 110;
		relayCandidateOrder		= 0;
		triggeredCheckDelay		= 100;
		maxRequestRetries		= 3;
		maxConnectivityChecks	= 100;
		maxRemoteCandidates		= 100;
		stunAddress				= Net::Address("0.0.0.0", 3478);
		turnAddress				= Net::Address("0.0.0.0", 3478);
	}
};


class Agent: public Stateful<State>, public Flaggable, public ILoggable
	/// This is a minimal implementation of Interactive Connectivity 
	/// Establishment (ICE), as defined in RFC5245.
{
public:
	enum Role 
	{
		Controlling					= 0x01, 
		Controlled					= 0x02
	};

	enum Flags 
	{
		LocalCandidatesSent			= 0x01, 
		RemoteCandidatesReceived	= 0x02,
		SentUpdatedOffer			= 0x04,
		ReceivedUpdatedOffer		= 0x08,
		SentUpdatedAnswer			= 0x10,
		ReceivedUpdatedAnswer		= 0x20,
	};

	Agent(IAgentObserver& observer, 
		  const Options& options = Options(), 
		  //Runner& runner = Runner::getDefault(), 
		  Net::Reactor& reactor = Net::Reactor::getDefault());	
	virtual ~Agent();

	virtual void initiate();	
		// Initiates ICE processing and sets us our role to
		// Controlling.

	virtual void terminate();	
		// Terminates ICE processing removes all media streams.

	virtual void restart();	
		// Restarts ICE processing for all media streams.
	
	virtual bool canTransitionState(UInt32 id);
	//virtual bool canTransitionState(unsigned int id);
	virtual bool tryTransitionState(UInt32 id);

	virtual void setFailed();
		// Some error has occured outside of the control of 
		// Agent. Use this to set the overall ICE this
		// processing to Failed.

	virtual IAgentObserver& observer();

	// Timeout
	//virtual void resetTimeout();
	virtual void onICETimeout(TimerCallback<Agent>& timer);

	// Media Streams
	virtual MediaStream* createMediaStream(const std::string& title, int nComponents = 1, const std::string& mediaType = "audio", const std::string& mediaProfile = "RTP/AVP");	
	virtual bool tryAddMediaStream(MediaStream* stream);
	virtual MediaStream* getMediaStream(int id);
	virtual bool removeMediaStream(int id);

	// Authentication
	//virtual void setAuthenticator(Authenticator* auth) { delete _authenticator; _authenticator = auth; };	
	//virtual Authenticator* authenticator() { return _authenticator; };

	// Candidate Gathering
	//virtual void onMediaStreamLocalCandidatesGathered(MediaStream* stream, LocalCandidateList& candidates);
	//virtual void onCandidateGatheringComplete(MediaStream* stream);

	// Connectivity Checking
	virtual void onMediaStreamStateChange(MediaStream* stream);
	virtual void onMediaStreamCheckingFailed(MediaStream* stream);
	virtual void onMediaStreamValidPairsCreated(MediaStream* stream, CandidatePairList& validPairs);		
	//virtual void onMediaStreamNominatedPairsCreated(MediaStream* stream, CandidatePairList& nominatedPairs);		
	virtual void onMediaStreamSubsequentCheckingComplete(MediaStream* stream);		
		
	// SDP Processing
	virtual bool generateSDP(SDP::Message& sdp);
	virtual bool processSDP(const SDP::Message& sdp);
	virtual bool processOfferSDP(const SDP::Message& sdp);
	virtual bool processAnswerSDP(const SDP::Message& sdp);
	virtual bool processUpdatedOfferSDP(const SDP::Message& sdp);
	//virtual bool processUpdatedAnswerSDP(const SDP::Message& sdp);
	virtual void sendUpdatedOffers();
	//virtual bool isValidSDP(const SDP::Message& sdp);

	virtual void printLog(std::ostream& ost) const;

	// Accessors
	//virtual Runner& runner() { return _runner; };
	Net::Reactor& reactor();
	//Runner& runner();
	Options& options();
	MediaStreamMap streams() const;

	//void stunAddress(const Net::Address& address) { _stunAddr = address; };
	//void turnAddress(const Net::Address& address) { _turnAddr = address; };
	//void srflxIP(const Poco::Net::Net::IP& ip) { _srflxIP = ip; };

	//Net::Address stunAddress() const { return _stunAddr; };
	//Net::Address turnAddress() const { return _turnAddr; };
	//std::string title() const { return _title; };
	//std::string localUFrag() const { return _options.localUFrag; };
	//std::string localPassword() const { return _options.localPassword; };
	virtual UInt64 tieBreaker() const { return _tieBreaker; };
	virtual Role role() const { return _role; };
	virtual bool isControlling() { return _role == Controlling; };

	//Signal<CandidateList> LocalCandidatesSent;
	//Signal<int> CandidateGatheringComplete;

	// Callback Events
	//Signal<SDP> OutgoingSDP;
	//Signal<int> MediaStreamCreated;
	//Signal<int> MediaStreamReady;
	//Signal<ICEPacket> MediaOnData;
	
	virtual const char* className() const { return "ICE Agent"; }

protected:	
	virtual void onStateChange(State& state, const State& oldState);

protected:
	IAgentObserver&			_observer;
	Options					_options;
	//Runner&				_runner;
	Net::Reactor&			_reactor;
	MediaStreamMap			_streams;
	std::string				_sessionID;
	UInt64					_tieBreaker;
	Role					_role;
	Poco::FastMutex			_mutex;
};


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_Agent_H