///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_FormatRegistry_H
#define SCY_AV_FormatRegistry_H


#include "scy/av/format.h"
#include "scy/mutex.h"

#include <vector>


namespace scy {
namespace av {


class FormatRegistry
{
public:
    static FormatRegistry& instance();

    FormatRegistry();
    virtual ~FormatRegistry();

    virtual Format& get(const std::string& name);
    virtual Format& getByID(const std::string& id);
    virtual Format&
    getOrDefault(const std::string& name); // Returns the default media format.
    /// If a default has been specified it will be
    /// returned, other the format with the highest
    /// priority will take precedence.
    virtual Format& getDefault();

    /// Registers the given media format overriding
    /// existing media formats of the same name.
    virtual void registerFormat(const Format& format);

    /// Unregisters the media format matching the
    /// given name.
    virtual bool unregisterFormat(const std::string& name);

    /// Sets the default fallback media format.
    virtual void setDefault(const std::string& name);


    virtual bool exists(const std::string& name);
    virtual void clear();

    virtual FormatList formats() const;

private:
    FormatRegistry(FormatRegistry const&){};
    // FormatRegistry& operator=(FormatRegistry const&) {};

    FormatList _formats;
    std::string _default;
    mutable Mutex _mutex;
};


} // namespace av
} // namespace scy


#endif // SCY_AV_FormatRegistry_H


/// @\}
