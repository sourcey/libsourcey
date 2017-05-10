///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/formatregistry.h"
#include "scy/singleton.h"


using std::endl;


namespace scy {
namespace av {


FormatRegistry::FormatRegistry()
{
}


FormatRegistry::~FormatRegistry()
{
}


FormatRegistry& FormatRegistry::instance()
{
    static Singleton<FormatRegistry> sh;
    return *sh.get();
}


Format& FormatRegistry::get(const std::string& name)
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (unsigned int i = 0; i < _formats.size(); i++) {
        if (_formats[i].name == name) {
            return _formats[i];
        }
    }

    throw std::runtime_error("Not found: No media format for: " + name);
}


Format& FormatRegistry::getByID(const std::string& id)
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (unsigned int i = 0; i < _formats.size(); i++) {
        if (_formats[i].id == id) {
            return _formats[i];
        }
    }

    throw std::runtime_error("Not found: No media format type: " + id);
}


Format& FormatRegistry::getOrDefault(const std::string& name)
{
    {
        std::lock_guard<std::mutex> guard(_mutex);
        for (unsigned int i = 0; i < _formats.size(); i++) {
            if (_formats[i].name == name) {
                return _formats[i];
            }
        }
    }

    return getDefault();
}


Format& FormatRegistry::getDefault()
{
    std::lock_guard<std::mutex> guard(_mutex);
    if (!_default.empty()) {
        return get(_default);
    } else if (!_formats.empty()) {
        return *_formats.begin();
    }

    throw std::runtime_error("Not found: No default media format.");
}


bool FormatRegistry::exists(const std::string& name)
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (unsigned int i = 0; i < _formats.size(); i++) {
        if (_formats[i].name == name) {
            return true;
        }
    }

    return false;
}


void FormatRegistry::clear()
{
    std::lock_guard<std::mutex> guard(_mutex);
    _formats.clear();
}


FormatList FormatRegistry::formats() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _formats;
}


void FormatRegistry::registerFormat(const Format& format)
{
    unregisterFormat(format.name);
    std::lock_guard<std::mutex> guard(_mutex);
    _formats.push_back(format);
}


bool FormatRegistry::unregisterFormat(const std::string& name)
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (FormatList::iterator it = _formats.begin(); it != _formats.end();
         ++it) {
        if ((*it).name == name) {
            _formats.erase(it);
            if (_default == name)
                _default = "";
            return true;
        }
    }
    return false;
}


void FormatRegistry::setDefault(const std::string& name)
{
    std::lock_guard<std::mutex> guard(_mutex);
    _default = name;
}


} // namespace av
} // namespace scy


/// @\}
