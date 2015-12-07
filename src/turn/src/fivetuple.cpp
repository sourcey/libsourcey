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


#include "scy/turn/fivetuple.h"


using namespace std;


namespace scy {
namespace turn {


FiveTuple::FiveTuple() : 
    _transport(net::UDP) 
{
}


FiveTuple::FiveTuple(const net::Address& remote, const net::Address& local, net::TransportType transport) : 
    _remote(remote), _local(local), _transport(transport) 
{
}


FiveTuple::FiveTuple(const FiveTuple& r) :
    _remote(r._remote), _local(r._local), _transport(r._transport)
{
}


bool FiveTuple::operator ==(const FiveTuple& r) const {
    return _remote == r._remote && 
        _local == r._local && 
        _transport == r._transport;
}


bool FiveTuple::operator <(const FiveTuple& r) const 
{
    if (_remote.port() < r._remote.port())
        return true;
    if (r._remote.port() < _remote.port())
        return false;
    if (_local.port() < r._local.port())
        return true;
    if (r._local.port() < _local.port())
        return false;
    return false;
}


string FiveTuple::toString() const 
{ 
    ostringstream ost;
    ost << "FiveTuple[" 
        << _remote.toString() << ":" 
        << _local.toString() << ":" 
        << _transport 
        << "]";
    return ost.str();
}


} } // namespace scy::turn