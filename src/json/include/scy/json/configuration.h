///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup json
/// @{


#ifndef SCY_JSON_Configuration_H
#define SCY_JSON_Configuration_H


#include "scy/configuration.h"
#include "scy/json/json.h"

#include "scy/mutex.h"


namespace scy {
namespace json {


class Configuration: public scy::Configuration//, public json::Value
{
public:
    Configuration();
    virtual ~Configuration();

    virtual void load(const std::string& path, bool create = false);
    virtual void load(bool create = false);
    virtual void save();

    virtual bool remove(const std::string& key);
    virtual void removeAll(const std::string& baseKey);
    virtual void replace(const std::string& from, const std::string& to);
    virtual void keys(std::vector<std::string>& keys, const std::string& baseKey = "");
    virtual void print(std::ostream& ost);

    virtual std::string path();
    virtual bool loaded();

    /// See IConfiguration for data accessors

    json::Value root;

protected:
    virtual bool getRaw(const std::string& key, std::string& value) const;
    virtual void setRaw(const std::string& key, const std::string& value);

    bool _loaded;
    std::string _path;
    mutable Mutex _mutex;
};


} } // namespace scy::json


#endif // SCY_JSON_Configuration_H

/// @\}
