#ifndef ANIONU_SPOT_ISession_H
#define ANIONU_SPOT_ISession_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Logger.h"

#include "Poco/Path.h"

#include <iostream>
#include <string>
#include <assert.h>


namespace Sourcey {
namespace Spot {


struct SessionState: public StateT 
{
	enum Type 
	{
		None = 0,
		Authenticating,
		ActiveOffline,
		ActiveOnline,
		Failed,
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:				return "None";
		case Authenticating:	return "Authenticating";
		case ActiveOffline:		return "Active (Offline)";
		case ActiveOnline:		return "Active (Online)";
		case Failed:			return "Failed";
		default:				assert(false);
		}
		return "undefined"; 
	}
};


class IClient;


class ISession: public StatefulSignal<SessionState>, public ILoggable
{		
public:
	ISession(/*IEnvironment& env*/);
	virtual ~ISession();
	
	virtual std::string name() const = 0;
	virtual std::string username() const = 0;
	virtual std::string password() const = 0;

	virtual Poco::Path storagePath() const = 0;

	virtual bool isActive() const 
	{ 
		return stateEquals(SessionState::ActiveOffline)
			|| stateEquals(SessionState::ActiveOnline); 
	}
	
	virtual const char* className() const { return "Session"; }
};


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_ISession_H
