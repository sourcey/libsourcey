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


#include "scy/av/win32/directshow.h"
#include "scy/platform.h"


#include <windows.h>
#include <dshow.h>
#include <dbt.h> // DBT_* & DEV_*
#include <ks.h>
#include <ksmedia.h> // KSCATEGORY_*


using std::endl;


namespace scy {
namespace av {
namespace dshow {


bool getDeviceCategoryList(Device::Type type, REFGUID category, std::vector<av::Device>& devices)
{
    IMoniker* m = nullptr;

    ICreateDevEnum* devenum = nullptr;
    if (CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER,
                             IID_ICreateDevEnum, (void**) &devenum) != S_OK)
        return false;

    IEnumMoniker* classenum = nullptr;
    if (devenum->CreateClassEnumerator(category, (IEnumMoniker**)&classenum, 0) != S_OK)
        return false;

    while (classenum->Next(1, &m, nullptr) == S_OK)
    {
        VARIANT var;
        IPropertyBag* bag = nullptr;
        LPMALLOC coMalloc = nullptr;
        IBindCtx* bindCtx = nullptr;
        LPOLESTR olestr = nullptr;
        std::string devId, devHumanName;

        if (CoGetMalloc(1, &coMalloc) != S_OK)
            goto fail;
        if (CreateBindCtx(0, &bindCtx) != S_OK)
            goto fail;

        // Get an uuid for the device that we can pass to ffmpeg directly
        if (m->GetDisplayName(bindCtx, nullptr, &olestr) != S_OK)
            goto fail;
        devId = scy::toUtf8(olestr);

        // replace ':' with '_' since FFmpeg uses : to delimitate sources
        for (size_t i = 0; i < strlen(devId); i++)
            if (devId[i] == ':')
                devId[i] = '_';

        // Get a human friendly name/description
        if (m->BindToStorage(nullptr, nullptr, IID_IPropertyBag, (void**)&bag) != S_OK)
            goto fail;

        var.vt = VT_BSTR;
        if (bag->Read(L"FriendlyName", &var, nullptr) != S_OK)
            goto fail;
        devHumanName = scy::toUtf8(var.bstrVal);

        devices.push_back(av::Device(type, "video="+devId, devHumanName));

fail:
        if (olestr && coMalloc)
            coMalloc->Free(olestr);
        if (bindCtx)
            bindCtx->Release();
        delete[] devId;
        delete[] devHumanName;
        if (bag)
            bag->Release();
        m->Release();
    }
    classenum->Release();

    return !devices.empty();
}


bool getDeviceList(Device::Type type, std::vector<av::Device>& devices)
{
    switch (type) {
    case Device::VideoInput:
        return getDeviceCategoryList(type, CLSID_VideoInputDeviceCategory, devices);
    case Device::AudioInput:
        return getDeviceCategoryList(type, CLSID_AudioInputDeviceCategory, devices);
    default:
        // assert(0 && "unknown dshow device type");
        DebugL << “DirectShow cannot enumerate output devices: Not implemented" << endl;
        break;
    }

    return false;
}


} } } // namespace scy::av::dshow


#endif
