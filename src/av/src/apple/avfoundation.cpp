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


#include "scy/av/devicemanager_mac.h"

// #include <CoreAudio/CoreAudio.h>
// #include <QuickTime/QuickTime.h>


using std::endl;


namespace scy {
namespace av {
namespace avfoundation {


extern bool GetAVFoundationVideoDevices(std::vector<Device>* devices);


bool getDeviceList(Device::Type type, std::vector<av::Device>& devices)
{
    switch (type) {
    case Device::VideoInput:
    case Device::AudioInput:
        return GetAVFoundationVideoDevices(type, &devices);
    default:
        assert(0 && "unknown avfoundation device type");
        break;
    }

    return false;
}


} } } // namespace scy::av::avfoundation
