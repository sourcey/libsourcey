#ifndef SPOT_BASE_IStreamingManager_H
#define SPOT_BASE_IStreamingManager_H


#include "Sourcey/Logger.h"
#include "Sourcey/TimedManager.h"
#include "Sourcey/Spot/IStreamingSession.h"
//#include "Sourcey/BasicManager.h"
//#include "Sourcey/PacketStream.h"


namespace Sourcey {
namespace Spot {


class IStreamingManager: protected TimedManager<std::string, IStreamingSession>, public ILoggable
{
public:
	typedef TimedManager<std::string, IStreamingSession>	Manager;
	typedef Manager::Base::Map								Map;

public:
	IStreamingManager();
	virtual ~IStreamingManager();

	virtual void addSession(IStreamingSession* session);
		/// Adds the session, or throws an ExistsException if
		/// a session with a matching token already exists.
	
	virtual IStreamingSession* getSession(const std::string& token);
		/// Returns the IStreamingSession instance or throws
		/// a NotFoundException.

	IStreamingSession* removeSession(const std::string& token);
		/// Removes the session with the given token and returns
		/// the session pointer or NULL.
		
	Signal2<IStreamingSession&, bool&> InitializeStreamingSession;
		/// Provides plugins with the ability to override the
		/// creation of source and encoder objects for the
		/// given session. Overriding may be partial or
		/// complete. If stream creation is to be completely
		/// overridden the boolean paramerer must return true,
		/// otherwise default stream objects will be created or 
		/// skipped based on the existence of their polymorphic
		/// base in the session PacketStream.
		///
		/// Stream objects should derive from:
		///
		///		PacketDispatcher:		source
		///		Media::IPacketEncoder:	encoder
		///
		/// Stream ordering is as follows:
		///
		///		sources:				0 - 4
		///		encoders:				5 +
		///

	Signal3<IStreamingSession&, ConnectionStream&, bool&> InitializeStreamingConnection;
		/// Provides plugins with the ability to override the
		/// session connection objects. Mothodology is the same
		/// as the InitializeStreamingSession signal same except
		/// that stream objects should derive from:
		///
		///		IPacketProcessor:		processor
		///		IPacketizer:			packetizer
		///
		/// Stream ordering is as follows:
		///
		///		processors:				5 - 9
		///		packetizers:			10 - 14
		///

	virtual void onSessionStateChange(void* sender, StreamingState& state, const StreamingState&) = 0;
	virtual void onItemTimeout(TimerCallback<TimedManager>& timer);

	virtual const char* className() const { return "StreamingManager"; }
};


} } // namespace Sourcey::Spot


#endif // SPOT_BASE_IStreamingManager_H