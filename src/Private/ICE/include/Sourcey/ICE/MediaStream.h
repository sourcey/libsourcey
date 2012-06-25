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


#ifndef SOURCEY_ICE_MEDIA_STREAM_H
#define SOURCEY_ICE_MEDIA_STREAM_H


#include "Sourcey/ICE/State.h"
#include "Sourcey/ICE/Component.h"
#include "Sourcey/ICE/Candidate.h"
#include "Sourcey/ICE/CheckList.h"
#include "Sourcey/ICE/CandidatePair.h"
#include "Sourcey/Media/Format.h"
#include "Sourcey/Media/Codec.h"
#include "Sourcey/SDP/Message.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/Address.h"

#include <iostream>
#include <sstream>
#include <assert.h>


namespace Sourcey {
namespace ICE {


class Agent;


class MediaStream: public Stateful<State>, public Flaggable, public ILoggable
	/// The MediaStream allocates candidates and provedes 
	/// callbacks to the Agent when candidates are ready.
{
public:	
	enum Flags 
	{
		LocalCandidatesGathered		= 0x01, 
		RemoteCandidatesReceived	= 0x02
	};
	
public:	
	static MediaStream* createFromSDP(Agent& agent, const SDP::Message& sdp, const SDP::M& m);

	MediaStream(Agent& agent, int index, const std::string& name, int nComponents = 2, 
		const std::string& mediaType = "video", const std::string& mediaProfile = "RTP/AVP");
	virtual ~MediaStream();
	
	//
	// State Machine
	//
	virtual bool tryTransitionState(UInt32 id);
	virtual bool canTransitionState(UInt32 id);

	//
	// SDP Processing
	//	
	bool processOfferSDP(const SDP::Message& sdp, const SDP::M& m);
		// Creates the media stream components and remote candidates from a remote offer.

	bool processAnswerSDP(const SDP::Message& sdp, const SDP::M& m);
		// Creates the media stream remote candidates from a remote answer.

	bool processUpdatedOfferSDP(const SDP::Message& sdp, const SDP::M& m);
		// Updates default candidates from an updated offer SDP.

	//
	// Actions
	//
	virtual Component* createComponent(int id = 0, UInt16 localPort = 0);	

	//virtual bool trySetLocalCandidatesGathered();
		// Checks if candidates have been gathered in the GatheringCandidates
		// state, and if true transitions the stream to the SendingSDP state.

	virtual bool generateSDP(std::ostream& ost) const;
	
	//virtual bool sendPacket(int componentID, const IPacket& packet);
	//virtual void startSendingMedia();
	//virtual void stopSendingMedia();

	//
	// Events
	//	
	virtual void onLocalCandidateStateChange(LocalCandidate* candidate);
	virtual void onComponentLocalCandidatesGathered(Component* component);

	//
	// Accessors
	//
	virtual bool tryAddLocalCandidate(LocalCandidate* candidate, bool deleteIfPruned = true);
	virtual Candidate* getLocalCandidateByAddress(const Net::Address& address) const;
	virtual LocalCandidateList getLocalCandidatesForComponent(int componentID) const;
	virtual LocalCandidateList localCandidates() const;

	virtual bool tryAddRemoteCandidate(RemoteCandidate* candidate, bool deleteIfPruned = true);
	virtual RemoteCandidate* getRemoteCandidateByAddress(const Net::Address& address) const;
	virtual RemoteCandidateList getRemoteCandidatesForComponent(int componentID) const;
	virtual RemoteCandidateList remoteCandidates() const;
	virtual void setRemoteCandidatesReceived();

	virtual void setRemoteUFrag(std::string ufrag) { _remoteUFrag = ufrag; };
	virtual void setRemotePassword(std::string pass) { _remotePassword = pass; };

	virtual void addCodec(const Media::Codec& codec);

	virtual Component* getComponent(int id) const;
	virtual ComponentMap components() const { return _components; };
	virtual CheckList& queue() { return _queue; };
	virtual Agent& agent() { return _agent; };
	virtual Media::CodecList codecs() const { return _codecs; };
	virtual int index() const { return _index; };
		// The index of this stream is it's position in the SDP offer.

	virtual std::string name() const { return _name; };
	    // The unique name of this media stream. For sessions with more than 
		// one distinct media stream of the same media type it may be set to 
		// the SDP session level i= field, otherwise it may be set to the m= 
		// media type.
		// The same applies for Jingle where it may be set to the 
		// <content name='*'> or <description media='*'> attribute values.

	virtual std::string mediaType() const { return _mediaType; };
	virtual std::string mediaProfile() const { return _mediaProfile; };
	virtual std::string remoteUFrag() const { return _remoteUFrag; };
	virtual std::string remotePassword() const { return _remotePassword; };

	virtual void printLog(std::ostream& ost) const;
	
	Signal<LocalCandidate&> LocalCandidateAdded;
	Signal<RemoteCandidate&> RemoteCandidateAdded;
	
	virtual const char* className() const { return "MediaStream"; }

private:	
	virtual void onStateChange(State& state, const State& oldState);
	//virtual bool setState(UInt32 id);

	// Candidate Gathering
	virtual void startGatheringLocalCandidates();

private:
	Agent& _agent;
	int _index;
	std::string _name;
	std::string _remoteUFrag;
	std::string _remotePassword;
	std::string _mediaType;
	std::string _mediaProfile;	
	Media::CodecList _codecs;		
		// A list of available encoding codecs to be sent with the initial
		// offer. Unused codecs will be removed when the answer is received.

	LocalCandidateList _localCandidates;	
	RemoteCandidateList _remoteCandidates;
	ComponentMap _components;
	CheckList _queue;

	mutable Poco::FastMutex _mutex;
	
	friend class Agent;
	friend class CheckList;
};


typedef std::map<int, MediaStream*> MediaStreamMap;
	/// Media streams are indexed by position in the SDP offer.


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_MEDIA_STREAM_H
