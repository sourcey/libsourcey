#ifndef ANIONU_SPOT_IModeRegistry_H
#define ANIONU_SPOT_IModeRegistry_H


#include "Sourcey/Base.h"
//#include "Sourcey/IRegistry.h"

#include <map>


namespace Sourcey {
namespace Spot {


class IMode;
class IChannel;
class IEnvironment;


// ---------------------------------------------------------------------
//
// IMode Factory
//
// ---------------------------------------------------------------------
template<typename T> 
IMode* createT(IEnvironment& env, IChannel& channel) { return new T(env, channel); }


class IModeRegistry//: public IRegistry<IMode>
{
public:
	typedef std::map<std::string, IMode*(*)(IEnvironment&, IChannel&)> ModeTypeMap;

	IModeRegistry() {};
	~IModeRegistry() {};

    IMode* createInstance(const std::string& name, IEnvironment& env, IChannel& channel) 
	{
        ModeTypeMap::iterator it = _types.find(name);
        if (it == _types.end())
            return NULL;
        return it->second(env, channel);
    }
	
	template<typename T>
    void registerMode(const std::string& name)	
	{ 
        //_types.insert(std::make_pair(name, &createT<T>)); // vc2010 bug
        _types[name] = &createT<T>;
		ModeRegistered.dispatch(this, name);
    }
	
    void unregisterMode(const std::string& name)	
	{ 
        ModeTypeMap::iterator it = _types.find(name);
        if (it == _types.end())
            return;
		_types.erase(it);
		ModeUnregistered.dispatch(this, name);
    }

    ModeTypeMap types() const { return _types; }

	Signal<const std::string&> ModeRegistered;
	Signal<const std::string&> ModeUnregistered;

private:
    ModeTypeMap _types;
};


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IModeRegistry_H