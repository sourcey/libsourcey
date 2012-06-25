#include "Sourcey/ICE/Agent.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/UDPPacketSocket.h"
//#include "Sourcey/ICE/IAgentObserver.h"
//#include "Sourcey/Media/MediaMJPEGProvider.h"

/*
#include "Poco/Exception.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
*/
#include "Poco/Util/ServerApplication.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;
using namespace Sourcey;
using namespace Sourcey::Net;


namespace Sourcey {
namespace ICE {


class RAgent: public Poco::Util::ServerApplication, public ICE::IAgentObserver//, public packetDelegate<RAgent, DataPacket>
{
public:
		/*
	void onOutgoingSDP(ICE::Agent* agent, SDP::Message& sdp)
	{
		Log("debug") << "LAGENT: LocalSDPReady #################################################" << endl;

		// The RAgent should be listening on port 8000.
		_socket->send(sdp, SocketAddress("127.0.0.1", 8001));
	}
	
	void onMediaStreamCreated(ICE::Agent* agent, ICE::MediaStream* stream)
	{
		Log("debug") << "LAGENT: MediaStreamCreated #################################################" << endl;
	}

	void onMediaStreamValid(ICE::Agent* agent, ICE::MediaStream* stream)
	{
		Log("debug") << "LAGENT: MediaStreamReady #################################################" << endl;
		//MediaStream* stream = const_cast<MediaStream*>(reinterpret_cast<const MediaStream*>(sender));
		// stream->sendPacket(1, "oi!!", 4);	

		Media::VideoCapture* video = new Media::VideoCapture("test.avi");
		Media::VideoParams params;
		params.fps = 1;
		Media::MediaMJPEGProvider* provider = new Media::MediaMJPEGProvider(video, params);
		provider->start();
		stream->setDataProvider(reinterpret_cast<Sourcey::ICE::DataProvider *>(provider));	
	}
		*/

	/*
	void onICEStateChanged(ICE::Agent* agent, ICE::State& state)
	{
		Log("debug") << "********************** RAGENT: STATE UPDATED: " << state.id() << ": " << state.toString() << endl;
		switch(state.id()) {	
		case State::Success:
			{
			}
			break;
		}
	}
	*/

	int main(const vector<string>& args)
	{		
		Logger::instance().add(new ConsoleChannel("debug"));

		Options o;
		o.title = "RAgent";
		o.localUFrag = "RAgent";
		o.localPassword = "password";
		o.stunAddress = SocketAddress("127.0.0.1", 3478);
		//SocketAddress stunAddr("stun.ekiga.net", 3478);
		o.turnAddress = SocketAddress("127.0.0.1", 3478);
		//o.disableHost = true;
		o.disableSTUN = false;
		o.disableRelay = true;

		_rAgent = new Agent(*this, o);
		//_rAgent->setIAgentObserver(this);

		// Authentictor
		//_rAgent->authenticator()->addUser(new BasicUser("LAgent", "password"));
		_user = new BasicUser("LAgent", "password");
		
		_socket = new UDPPacketSocket();
		_socket->bind(SocketAddress("127.0.0.1", 8000));
		_socket->attach(packetDelegate<RAgent, DataPacket>(this, &RAgent::onPacketReceived)); //, 100

		system("pause");

		_socket->detach(packetDelegate<RAgent, DataPacket>(this, &RAgent::onPacketReceived)); //, 100
		delete _socket;

		return Application::EXIT_OK;
	}

	void onMediaDataReceived(Agent* agent, MediaStream* stream, Component* component, const char* data, int size, const SocketAddress& localAddr, const SocketAddress& remoteAddr)
	{
		Log("debug") << "LAGENT: onMediaDataReceived: " << size << endl;
	}

	void onPacketReceived(void* sender, DataPacket& packet) { //, const SocketAddress& localAddr, const SocketAddress& remoteAddr
		Log("debug") << "RAGENT: Received remote SDP: " << string((const char*)packet.data, packet.size) << endl;
		
		SDP::Message sdp;
		sdp.read(string((const char*)packet.data, packet.size));
		_rAgent->processSDP(sdp);
	}


	// ---------------------------------------------------------------------
	//
	// ICE Events
	//
	// ---------------------------------------------------------------------
	bool onMediaStreamCreated(ICE::Agent& agent, ICE::MediaStream& stream) 
	{
		Log("debug") << "[RAgent] Media Stream Created" << endl;
		return true;
	}


	const IUser* authenticateBindingRequest(ICE::Agent& agent, const string& username) 
	{
		Log("debug") << "Authenticate ICE Connectivity Check: " << username << endl; //Spot::env().account().get(username);
		return _user; //Spot::env().account().get(username);
	
		/*(const IUser*)
		FastMutex::ScopedLock lock(_mutex);

		// The request username must equal the Jingle peer's JID node.
		const IUser* user = NULL;
	
		string peerUsername = _ice->isControlling() ? _responder : _initiator;
		string::size_type index = peerUsername.find("@");
		peerUsername = peerUsername.substr(0, index);
		if (username == peerUsername) {
			// If it is then we request authentication.
			AuthenticateICEConnectivityCheck.send(this, username, user);
		} else
			Log("error") << "[RAgent] ICE Binding request not equal to peer: " << username << ": " << peerUsername << endl;
		return user;
		*/
	}


	void onMediaStreamValidPairsChanged(ICE::Agent& agent, ICE::MediaStream& stream, 
		const CandidatePairList& validPairs) 
	{
		Log("debug") << "[RAgent] Media Stream Valid Pairs Changed" << endl;
	}


	void onSelectedPairsChanged(Agent& agent, MediaStream& stream, Component& component, 
		const CandidatePair* selectedPair, const CandidatePair* oldPair)
	{
		Log("debug") << "[RAgent] Media Stream Selected Pairs Changed" << endl;
	}
	

	void onMediaDataReceived(ICE::Agent& agent, ICE::MediaStream& stream, Component& component, 
											const DataPacket& packet, const Net::Address& localAddr, const Net::Address& remoteAddr) 
	{
		Log("trace") << "[RAgent] Media Data Received: " << packet.size << endl;

		// Receivers may wish to differentiate RTP from RTCP
		// packets by creating a callback delegate for each.
		//broadcast((IPacket&)packet);
	}


	void onOutgoingSDP(ICE::Agent& agent, SDP::Message& sdp) 
	{
		Log("debug") << "[RAgent] Outgoing SDP: " << sdp.toString() << endl;

		//FastMutex::ScopedLock lock(_mutex);
		

		// The LAgent should be listening on port 8001.
		_socket->send(sdp, SocketAddress("127.0.0.1", 8001));

		//_initiator(c.to()),		// the local client
		//_responder(c.from())	// the remote client
		/*.toString().c_str(), sdp->toString().size()*/
		/*
		assert(_ice == &agent);
		assert(!agent.streams().empty());
		//assert(!sources().empty());

		IQ iq;
		sdpToJingle(iq, sdp);
		if (state().id() == SessionState::Initializing) {
			agent.isControlling() ? setState(SessionState::SentInitiate) : setState(SessionState::SentAccept);
		}

		_manager.router() >> iq;
		*/
	}


	void onICEStateChanged(ICE::Agent& agent, ICE::State& state) 
	{
		Log("debug") << "[RAgent] ICE State Changed: " << state.toString() << endl;

		switch(state.id()) {	
		case ICE::State::GatheringCandidates:
			break;

		case ICE::State::SendingSDP:
			break;

		case ICE::State::CheckingConnectivity:
			break;

		case ICE::State::Success:
		
			try	{
				/*
				FastMutex::ScopedLock lock(_mutex);

				// Start seding media if enabled.
				if (agent.options().senders != "recvonly") {
					MediaStreamMap streams = agent.streams();

					for (ICE::MediaStreamMap::iterator it = streams.begin(); it != streams.end(); ++it) {
						MediaStream* stream = it->second;

						IPacketBroadcaster* source = geStreamMediaSource(*stream);
						if (!source)
							throw Exception("No media source for stream: " + stream->name());
					
						ComponentMap components = stream->components();
						if (components.size() >= 1) {
							Log("debug") << "[RAgent] Sending media:"
								<< "\n\tStream: " << stream->index() << ": " << stream->name() 
								<< "\n\tComponent: " << components[1]->id()
								<< endl;
							source->attach(packetDelegate(components[1], &Component::sendRTPPacket));
						}
						if (components.size() >= 2) {						
							Log("debug") << "[RAgent] Sending media:"
								<< "\n\tStream: " << stream->index() << ": " << stream->name() 
								<< "\n\tComponent: " << components[2]->id()
								<< endl;

							// If our stream has 2 components set up a delegate 
							// for receiving RTCP packet from the broadcaster.
							source->attach(packetDelegate(components[2], &Component::sendRTCPPacket));
						}
					}
				}
			
				setState(SessionState::InProgress);
				*/	
			}
			catch (Exception& exc) {
				// Set ICE processing to Failed, in turn terminating the Session.
				agent.setFailed();
			}

			break;	

		case ICE::State::Failed:
			{
				/*
				// TODO: In the unlikely event that one of the parties determines
				// that it cannot establish connectivity even after sending and
				// checking lower-priority candidates, it SHOULD terminate the 
				// session as described in XEP-0166.

				IQ iq("set", 
					_ice->isControlling() ? _responder : _initiator, 
					_ice->isControlling() ? _initiator : _responder);
				Jingle::create(iq, Jingle::Terminate("media-error", _initiator, _responder, _sid));
				_manager.router() >> iq;

				Log("error") << "Jingle ICE UDP Session failed: " << _sid << endl;
				*/
			}
			break;	

		case ICE::State::Terminating:
			//if (!stateEquals(PacketStreamState::Closing) &&
			//	!stateEquals(PacketStreamState::Closed)) {
			//	terminate();
			//}
			break;	
		}
	}
	
private:
	Agent* _rAgent;
	UDPPacketSocket* _socket;
	IUser* _user;
};


} // namespace Jingle
} // namespace ICE 


int main(int argc, char** argv)
{
	Sourcey::Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	Sourcey::ICE::RAgent app;
	app.run(argc, argv);
	//return app.run(argc, argv);
	return 0;

}
