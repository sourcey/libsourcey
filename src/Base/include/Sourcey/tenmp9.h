//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SOURCEY_Callback_H
#define SOURCEY_Callback_H


#include "Sourcey/Types.h"
#include <string>
#include <memory>


namespace scy {

/*
namespace internal
{
    class callback_object_base
    {
    public:
        callback_object_base(void* data)
            : data_(data)
        {
        }
        virtual ~callback_object_base()
        {
        }

        void* get_data() { return data_; }

    private:
        void* data_;
    };

    // SCO: serialized callback object
    template<typename callback_t>
    class callback_object : public callback_object_base
    {
    public:
        callback_object(const callback_t& callback, void* data=nullptr)
            : callback_object_base(data)
            , callback_(callback)
        {
        }

        virtual ~callback_object()
        {
        }

    public:
        template<typename ...A>
        typename std::result_of<callback_t(A...)>::type invoke(A&& ... args)
        {
            return callback_(std::forward<A>(args)...);
        }

    private:
        callback_t callback_;
    };
}

typedef std::shared_ptr<internal::callback_object_base> callback_object_ptr;

class callbacks
{
public:
    callbacks(int max_callbacks)
        : lut_(max_callbacks)
    {
    }
    ~callbacks()
    {
    }

    template<typename callback_t>
    static void store(void* target, int cid, const callback_t& callback, void* data=nullptr)
    {
        reinterpret_cast<callbacks*>(target)->lut_[cid] = callback_object_ptr(new internal::callback_object<callback_t>(callback, data));
    }

    template<typename callback_t>
    static void* get_data(void* target, int cid)
    {
        return reinterpret_cast<callbacks*>(target)->lut_[cid]->get_data();
    }

    template<typename callback_t, typename ...A>
    static typename std::result_of<callback_t(A...)>::type invoke(void* target, int cid, A&& ... args)
    {
        auto x = dynamic_cast<internal::callback_object<callback_t>*>(reinterpret_cast<callbacks*>(target)->lut_[cid].get());
        assert(x);
        return x->invoke(std::forward<A>(args)...);
    }

private:
    std::vector<callback_object_ptr> lut_;
};
*/


} // namespace scy


#endif // SOURCEY_Callback_H